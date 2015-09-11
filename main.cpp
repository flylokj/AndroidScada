#include "MainWindow.h"
#include <QApplication>
#include <QtScript/QScriptable>
#include <QtScript/QScriptEngine>
#include <QMessageBox>
#include <QTextCodec>
#include <QDebug>
#include <QDesktopWidget>
#include "ResMan/ResManager.h"
#include "ResMan/ArchiveManager.h"
#include "ArchivePage.h"
#include <QTimer>



ResManager *g_pResManModule;            //资源管理模块;
ArchiveManager *g_pArchiveModule;       //档案管理模块;
QScriptEngine *pGlobalScriptEngine;     //全局脚本引擎;
QMainWindow *gMainWindow = 0;           //全局主窗口句柄;

void setupGlobalScriptEngine();         //建立全局脚本管理,添加各种自定义的脚本函数;
void globalScriptEngineTest();          //测试脚本模块是否工作正常;





int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));//设置本地编码;
    ResManager resManModule;             //资源管理模块{提供图片、字符串、警告信息、语言、密码};
    ArchiveManager archiveModule;        //档案管理模块;{建立变量管理、建立地址管理}
    setupGlobalScriptEngine();           //建立脚本引擎模块{插入了各种功能性脚本函数};
    MainWindow w;//建立窗口管理;

    Q_UNUSED(resManModule);
    Q_UNUSED(archiveModule);
    //设置字体;
#ifdef WINLINUX
    QFont font;
    font.setPointSize(16);
    font.setFamily(("wenquanyi"));
    a.setFont(font);
#endif
#ifdef MYANDROID
    w.setWindowFlags(Qt::FramelessWindowHint);//去掉窗口边框;
    QFont f = a.font();
    f.setPointSize(20);
    a.setFont(f);
    w.showFullScreen();
#else
    w.show();
#endif

	a.exec();
	delete pGlobalScriptEngine;
}










/*******************************脚本函数********************************/
//脚本函数对应的CPP函数:
//对应的脚本函数为readIntVar();
QScriptValue readIntVar(QScriptContext *context, QScriptEngine *engine)
{
	QString var = context->argument(0).toString();
	var.remove(0,1);//去除$符号;
	int id = var.toInt();//变量ID;
    SimulateMan *pSimulate = g_pArchiveModule->simulateMap.value(id, NULL);//获取对应的变量;
	if(pSimulate == NULL)
		return engine->nullValue();
	else
		return pSimulate->readInt();
	//    return context->argument(0).toString();
}


//对应的脚本函数为readIntVar();
QScriptValue writeVar(QScriptContext *context, QScriptEngine *engine)
{
	QString var = context->argument(0).toString();
	QString val = context->argument(1).toString();
	var.remove(0,1);//去除$符号;
	int id = var.toInt();//变量ID;
    SimulateMan *pSimulate = g_pArchiveModule->simulateMap.value(id, NULL);//获取对应的变量;
	if(pSimulate != NULL)
	{
		pSimulate->write(val);
	}
	return engine->nullValue();
}

//对应的脚本函数为ioctl();
QScriptValue ioctlFunc(QScriptContext *context, QScriptEngine *engine)
{
	QString type = context->argument(0).toString();
	int cmd = context->argument(1).toNumber();
	int arg = context->argument(1).toNumber();
	qDebug()<<"fuck ioctrl";
	return engine->nullValue();
}

//对应的脚本函数为changeLanguage();
QScriptValue changeLanguage(QScriptContext *context, QScriptEngine *engine)
{
	int which = context->argument(0).toInt32();
    g_pResManModule->changeLanguage(which);
	return engine->nullValue();
}

//对应的脚本函数为changePwd();
QScriptValue changePwd(QScriptContext *context, QScriptEngine *engine)
{
	int who = context->argument(0).toInt32();
	int oldPwd = context->argument(1).toInt32();
	int newPwd = context->argument(2).toInt32();
    g_pArchiveModule->changePwd(who, oldPwd, newPwd);
	return engine->nullValue();
}

//对应的脚本函数为backupData();
QScriptValue backupData(QScriptContext *context, QScriptEngine *engine)
{
	int which = context->argument(0).toInt32();
	if(which == 0)
	{
        ArchivePage::copyMoldData(g_pArchiveModule->getMoldName(), QString("backupMoldData") , QString(".bin"));
		qDebug()<<"backup machine data";
		//拷贝到u盘!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
	else if(which == 1)
	{
        ArchivePage::copyMoldData(g_pArchiveModule->getPanelName(), QString("backupPanelData") , QString(".param"));
		qDebug()<<"backup sys data";
	}
	else
	{
		engine->evaluate("backupData(0);backupData(1);");
		qDebug()<<"backup all";
	}


	return engine->nullValue();
}

//对应的脚本函数为restoreData();
QScriptValue restoreData(QScriptContext *context, QScriptEngine *engine)
{
	int which = context->argument(0).toInt32();
	if(which == 0)
	{
        QString strMold = g_pArchiveModule->getMoldName();
		QString strTemp = strMold;
		strTemp.prepend(MOLDDIR);
		strTemp.append(".bin");
		//将backupMoldData.bin从U盘拷贝到mold目录;

		//将backupMoldData.bin改名为当前模号.bin;
		QFile::remove(strTemp);
		QFile::rename(MOLDDIR+QString("backupMoldData.bin"), strTemp);
		//重新加载模号数据;
        g_pArchiveModule->changeMold(strMold, true);
		qDebug()<<"restore machine data";
	}
	else if(which == 1)
	{
        QString strTemp = g_pArchiveModule->getPanelName();
		strTemp.prepend(MOLDDIR);
		strTemp.append(".param");
		//将backupMoldData.bin从U盘拷贝到mold目录;

		//将backupMoldData.bin改名为当前模号.bin;
		QFile::remove(strTemp);
		QFile::rename(MOLDDIR+QString("backupPanelData.param"), strTemp);
        g_pArchiveModule->loadPanel(g_pArchiveModule->getPanelName());
		qDebug()<<"restore sys data "<<strTemp;

	}
	else
	{
		engine->evaluate("restoreData(0);restoreData(1);");
		qDebug()<<"restore all";
	}


	return engine->nullValue();
}



//对应的脚本函数为ShowMessage();
void ShowMessage(QString strText)
{
	QMessageBox::information(NULL, ("提示"), strText, ("确定(&O)"));
}
QScriptValue ShowMessageFunc(QScriptContext *context, QScriptEngine *engine)
{
	QString strMsg = context->argument(0).toString();
	ShowMessage(strMsg);
	return engine->nullValue();
}

//检测U盘;
QScriptValue probeUsb(QScriptContext *context, QScriptEngine *engine)
{
#ifdef WINLINUX
	QFile uFile("/udisk");
	if(uFile.exists())
		pGlobalScriptEngine->evaluate("writeVar(\"$322\",\"1\")");
#endif
	return engine->nullValue();
}



//为引擎添加接口函数;
void setupGlobalScriptEngine()
{
    pGlobalScriptEngine = new QScriptEngine();

	QScriptValue fun = pGlobalScriptEngine->newFunction(readIntVar);
	pGlobalScriptEngine->globalObject().setProperty("readIntVar", fun);

	fun = pGlobalScriptEngine->newFunction(writeVar);
	pGlobalScriptEngine->globalObject().setProperty("writeVar", fun);

	fun = pGlobalScriptEngine->newFunction(ioctlFunc);
	pGlobalScriptEngine->globalObject().setProperty("ioctl", fun);

	fun = pGlobalScriptEngine->newFunction(changeLanguage);
	pGlobalScriptEngine->globalObject().setProperty("changeLanguage", fun);

	fun = pGlobalScriptEngine->newFunction(changePwd);
	pGlobalScriptEngine->globalObject().setProperty("changePwd", fun);

	fun = pGlobalScriptEngine->newFunction(backupData);
	pGlobalScriptEngine->globalObject().setProperty("backupData", fun);

	fun = pGlobalScriptEngine->newFunction(restoreData);
	pGlobalScriptEngine->globalObject().setProperty("restoreData", fun);

	fun = pGlobalScriptEngine->newFunction(ShowMessageFunc);
	pGlobalScriptEngine->globalObject().setProperty("ShowMessage", fun);

	fun = pGlobalScriptEngine->newFunction(probeUsb);
	pGlobalScriptEngine->globalObject().setProperty("probeUsb", fun);
}

//测试脚本引擎;
void globalScriptEngineTest()
{
	qDebug()<<"script engine test";
	pGlobalScriptEngine->evaluate("x=readIntVar('$27');print(x);");
}
