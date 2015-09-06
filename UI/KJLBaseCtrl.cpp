#include "KJLBaseCtrl.h"
#include "ResMan/ArchiveManager.h"
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QTextCodec>
#include <QtScript/QScriptable>
#include <QtScript/QScriptEngine>

extern QScriptEngine *pGlobalScriptEngine;

KJLBaseCtrl::KJLBaseCtrl(QObject *parent/*=0*/)
    :QObject(parent)
{
    m_pFundoAct[0]=&KJLBaseCtrl::doActSingleSet;
    m_pFundoAct[1]=&KJLBaseCtrl::doActSet;
    m_pFundoAct[2]=&KJLBaseCtrl::doActLinkPage;
    m_pFundoAct[3]=&KJLBaseCtrl::doActRunTask;
    m_pFundoAct[4]=&KJLBaseCtrl::doActLinkSubWin;
    m_pFundoAct[5]=&KJLBaseCtrl::doActRunScript;
}

void KJLBaseCtrl::doActSingleSet(int var, int val)
{

}

void KJLBaseCtrl::doActSet(int var, int val)
{
    qDebug()<<"Act: doActSet"<<var<<":"<<val;
    SimulateMan *pSimulate = g_pArchiveModule->simulateMap.value(var, NULL);//获取对应的变量;
    QString setData = QString::number(val);
    if(pSimulate)
        pSimulate->write(setData, this);
}

void KJLBaseCtrl::doActLinkPage(int var, int val)
{
    qDebug()<<"Act:linkPage";
    if(val!=0)//系统页面;
    {
        emit(changePage(val+100));
    }
    else//普通用户页面;
        emit(changePage(var));
}

void KJLBaseCtrl::doActRunTask(int var, int val)
{
    //这种方法判断有问题;
    qDebug()<<"run task"<<var;
//    if(var >= resManModule.m_pTaskVector->size())
//    {
//        qDebug()<<"task out of range";
//        return;
//    }

    //QVector<LPActDeal> task = resManModule.m_pTaskVector[var];
	QVector<LPActDeal> task = g_pResManModule->m_pTaskVector.at(var);

    QVector<LPActDeal>::iterator it = task.begin();
    qDebug()<<"task cnt:"<<task.size();
    while(it != task.end())
    {
        LPActDeal pAct = *it;
        if(pAct->nOption >= 6 )
            qDebug()<<"action out of range";
        else
            (this->*m_pFundoAct[pAct->nOption])(pAct->nVar, pAct->nVal);
        it++;
    }
}

void KJLBaseCtrl::doActLinkSubWin(int var, int val)
{

    emit(subWinChangePage(var, val));
    QString strTemp = "emit(subWinChangePage:%1,%2)";
    qDebug()<<strTemp.arg(var).arg(val);
}

void KJLBaseCtrl::doActRunScript(int var, int val)
{
    QSettings settings(OPTIONGFILE, QSettings::IniFormat);
    QString strTemp("SCRIPT/");
    strTemp = strTemp + QString::number(var);
    QString fileName = settings.value(strTemp).toString();
    qDebug()<<"run script:"+fileName;

    //读取脚本文件;
    fileName.prepend(SCRIPTDIR);
    QFile scriptFile(fileName);
    if (!scriptFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0, "错误", "打开文件失败！");
        return;
    }

    QTextStream stream(&scriptFile);
    stream.setCodec("unicode");
    QString contents = stream.readAll();
    pGlobalScriptEngine->evaluate(contents, fileName);//执行脚本;
    scriptFile.close();
}


KJLBaseCtrl *KJLBaseCtrl::getInstance()
{
    static KJLBaseCtrl* g_pInstance = new KJLBaseCtrl;
    return g_pInstance;
}

KJLBaseCtrl::~KJLBaseCtrl()
{
    KJLBaseCtrl *p = getInstance();
    delete p;
}

