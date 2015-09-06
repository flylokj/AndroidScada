#include "ResManager.h"
#include <QFile>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <QTextCodec>
#include <QTextStream>



ResManager::ResManager(QObject *parent)
    : QObject(parent)
    , m_nLanguageSel(0)
    , m_nMaxLanguage(0)
{
    pHex = NULL;

	m_pTranslator = new QTranslator();
    m_pTranslator->load(LAGNUAGEFILE);
}

ResManager::~ResManager()
{
	qDebug()<<"~ResManager()";
    if(pHex)
        delete pHex;
    pHex = NULL;

    m_pageStruct.pSubPageVector.clear();
        
	m_pTaskVector.clear();

    //释放图片资源;
    releaseImgRes();
    //释放字符串资源;
    releaseStrRes();
    //释放警告信息字符串;
    releaseWarnRes();

	m_pTranslator->deleteLater();
}

//加载Hex文件;
void ResManager::loadHex()
{
    //[0]变量参数;
    //[1]工程参数;
    //[2]面板参数;
    //[3]页面参数;
    char *pTemp;
    QFile file(HEXFILE);
    if (!file.open(QIODevice::ReadOnly))
        return;

    qint64 size = file.size();
//    if(pHex)
//    {
//        delete pHex;
//        pHex = NULL;
//    }
    //读取文件;
    pHex = new char[size];
    file.read((char *)pHex, size);
    m_pSimulateVar = (char *)pHex;//模拟变量数目(4 bytes)
    pTemp = m_pSimulateVar;
    pTemp = (char *)((int *)pTemp + 1);//模拟变量描述区域的大小(4 bytes)
    pTemp = (char *)(pTemp + *((int *)pTemp));//工程参数;
    m_pProjectParm = (LPProjectParm)pTemp;
    m_pPanelParm = (LPPanelParm)(pTemp+sizeof(ProjectParm));
    //页面数目;
    int *pNum = (int*)((char*)m_pPanelParm + sizeof(PanelParm));
    m_pageStruct.nPageNum = *pNum;
    LPPageHead pPageHead = (LPPageHead)((char*)pNum + sizeof(int));
    for(int i = 0; i < *pNum; i++)
    {
        m_pageStruct.pageVector.append(pPageHead);
        pPageHead = (LPPageHead)((char *)pPageHead + pPageHead->nPageSize);
    }

    //m_pageStruct.pSubPageVector = new (QVector<LPPageHead>)[*pNum];//建立子画面矩阵;
	m_pageStruct.pSubPageVector.resize(*pNum);//建立子画面矩阵;
    LPSubPageListHead pSubPageListHead = (LPSubPageListHead)pPageHead;
    for(int i = 0; i < *pNum; i++)
    {
//        qDebug()<<"画面:"<<i;
//        qDebug()<<"子画面数目:"<<pSubPageListHead->nSubPageCnt;
//        qDebug()<<"子画面大小:"<<pSubPageListHead->nSubPageListSize;
        setupSubPageVector(pSubPageListHead, i);
        pSubPageListHead = (LPSubPageListHead)((char *)pSubPageListHead + pSubPageListHead->nSubPageListSize);
    }


    pNum = (int *)pSubPageListHead;//任务个数;
    qDebug()<<"..."<<pNum;
    qDebug()<<"..."<<pHex;
    qDebug()<<"task num:"<<*pNum;
    //m_pTaskVector = new QVector<LPActDeal>[*pNum];
	m_pTaskVector.resize(*pNum);
    if(*pNum > 0)
    {
        int *p = pNum + 1;
        for(int i = 0; i < *pNum; i++)
        {
            setupTaskVector(p, i);
            p =(int *)((char*)p+ *p);
        }
    }
    //页面数据;
    file.close();

}

void ResManager::loadConfig()
{
    //读取配置选项;
    QSettings settings(OPTIONGFILE, QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("GB2312"));
    m_nMaxLanguage = settings.value("SYS/MaxLanguage").toInt();//最大支持的语言数目;
    m_nLanguageSel = settings.value("SYS/LanguageSel").toInt();//当期那选中的语言;
    m_nAdminPwd = settings.value("SYS/AdminPwd").toInt();//管理员密码;
    m_nSeniorAdminPwd = settings.value("SYS/SeniorAdminPwd").toInt();//高级管理员密码;
}




//加载图片资源;
void ResManager::loadImg()
{
    QSettings settings(OPTIONGFILE, QSettings::IniFormat);
    QString strPath;
    QString strTemp;
    //图片资源数目;
    int num = settings.value("IMG/num").toInt();
    for(int i = 0; i < num; i++)
    {
        strTemp = QString("IMG/%1").arg(i);
        strPath = settings.value(strTemp).toString();
        qDebug()<<strPath;
        QPixmap *pPixmap = new QPixmap(strPath);
        m_imgVec.append(pPixmap);
    }
}

void ResManager::loadWarnText()
{
    QSettings settings(WARNINIFILE, QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("GB2312"));
    QString string;
    QString strTemp;
    //报警信息数目;
	
	int num = settings.value("WARN/num").toInt();
	for(int i = 0; i < num; i++)
	{
		strTemp = QString("WARN/%1").arg(i);
		string = settings.value(strTemp).toString();
		qDebug()<<string;
		QString *pString = new QString(string);
		m_warnVec.append(pString);
	}
	qDebug()<<"fucking load warn done";
}

//    QSettings settings("Option.ini", QSettings::IniFormat); // 当前目录的INI文件
//    settings.beginGroup("DevOption");
//    settings.setValue("1", QString("你好"));
//    settings.setValue("2", QString("你好"));
//    settings.setValue("3", QString("你好"));
//    settings.endGroup();

//加载字符串资源;
void ResManager::loadString()
{
     QSettings settings(OPTIONGFILE, QSettings::IniFormat);
     settings.setIniCodec(QTextCodec::codecForName("GB2312"));
     QString string;
     QString strTemp;
     QString strLanguage="STRING";//语言选择;
     strLanguage = strLanguage + QString::number(m_nLanguageSel);
     //字符串资源数目;
     int num = settings.value(strLanguage+"/num").toInt();
     for(int i = 0; i < num; i++)
     {
        strTemp = QString(strLanguage+"/%1").arg(i);
        string = settings.value(strTemp).toString();
        //qDebug()<<string;
        QString *pString = new QString(string);
        m_strVec.append(pString);
     }
    qDebug()<<"fucking load string done";
}

void ResManager::changeLanguage(int nLanguage, bool bInit/* = false*/)
{
	if(bInit)
		nLanguage = m_nLanguageSel;
    int nTemp = nLanguage%m_nMaxLanguage;
	//int nTemp = 0;
    if((!bInit) && (m_nLanguageSel == nTemp))//
	{
		return;
	}
	m_nLanguageSel = nTemp;
	releaseStrRes();
	loadString();

    //更新当前页面;
    emit(updateDisp());

    //保存到配置文件;
    QSettings settings(OPTIONGFILE, QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("GB2312"));
    settings.setValue("SYS/LanguageSel", m_nLanguageSel);

	if(m_nLanguageSel==0)
	{
		qApp->installTranslator(m_pTranslator);
	}
	else
	{
		qApp->removeTranslator(m_pTranslator);
	}

}

void ResManager::changePwd(int who, int oldPwd, int newPwd)
{
    if(who==0)//管理员;
    {
        if(oldPwd != m_nAdminPwd)
        {
            QMessageBox::warning(0, tr("Warning"),tr("PWD ERROR"));
            return;
        }
        m_nAdminPwd = newPwd;
        //保存到配置文件;
        QSettings settings(OPTIONGFILE, QSettings::IniFormat);
        settings.setIniCodec(QTextCodec::codecForName("GB2312"));
        settings.setValue("SYS/AdminPwd", m_nAdminPwd);

        QMessageBox::information(0, tr("Tips"), tr("Change Pwd success"));
    }
    else//高级管理员;
    {
        if(oldPwd != m_nSeniorAdminPwd)
        {
            QMessageBox::warning(0, tr("Warning"),tr("PWD ERROR"));
            return;
        }
        m_nSeniorAdminPwd = newPwd;
        //保存到配置文件;
        QSettings settings(OPTIONGFILE, QSettings::IniFormat);
        settings.setIniCodec(QTextCodec::codecForName("GB2312"));
        settings.setValue("SYS/SeniorAdminPwd", m_nSeniorAdminPwd);

        QMessageBox::information(0, tr("Tips"), tr("Change Pwd success"));
    }
}

void ResManager::releaseStrRes()
{
    for(int i = 0; i < m_strVec.size(); i++)
    {
        delete m_strVec.at(i);
    }
    m_strVec.clear();
}

void ResManager::releaseImgRes()
{
    for(int i = 0; i < m_imgVec.size(); i++)
    {
        delete m_imgVec.at(i);
    }
    m_imgVec.clear();
}

void ResManager::releaseWarnRes()
{
    for(int i = 0; i < m_warnVec.size(); i++)
    {
        delete m_warnVec.at(i);
    }
    m_warnVec.clear();
}

//根据ID获取图片资源;
QPixmap *ResManager::getPixmap(const int id)
{
    if(id == -1)
        return NULL;
    if(id >= m_imgVec.size())
    {
        QMessageBox warn(QMessageBox::NoIcon, tr("Warning"),tr("img res id out of range"));
        warn.exec();
        return NULL;
    }
    return m_imgVec.at(id);
}

//获取面板的背景图片;
QPixmap *ResManager::getPanelBkPixmap()
{
    int id = m_pPanelParm->nBkPic;
    return getPixmap(id);
}

//根据ID获取字符串资源;
QString *ResManager::getString(const int id)
{
    if(id == -1)
        return NULL;
    if(id >= m_strVec.size())
    {
        QMessageBox warn(QMessageBox::NoIcon, tr("Warning"),tr("string res id:%1 out of range").arg(id));
        warn.exec();
        return NULL;
    }
    return m_strVec.at(id);
}

const QString &ResManager::getWarningText(const int id)
{
    if(id == -1)
        return m_nullString;
    if(id >= m_warnVec.size())
    {
        QMessageBox::warning(0, tr("Warning"),tr("Warning res id:%1 out of range").arg(id));
        return m_nullString;
    }
    return *m_warnVec.at(id);
}

void ResManager::setupSubPageVector(const LPSubPageListHead pSub, int nId)
{
    int nMax = pSub->nSubPageCnt;
    LPPageHead pPageHead = (LPPageHead)(pSub + 1);
	QVector<LPPageHead> *pPVector = const_cast< QVector<LPPageHead> *>(&m_pageStruct.pSubPageVector.at(nId) );
    for(int i = 0; i < nMax; i++)
    {
        //m_pageStruct.pSubPageVector[nId].append(pPageHead);
		pPVector->append(pPageHead);
        pPageHead = (LPPageHead)((char *)pPageHead + pPageHead->nPageSize);
    }
}

void ResManager::setupTaskVector(int *p, int nId)
{
    int *pp = p;
    pp = pp + 1;//子任务的个数;
    if(*pp == 0)
        return;
    int nCnt = *pp;
    qDebug()<<"action cnt:"<<nCnt;
    pp = pp + 1;//指向第一条任务;
    qDebug()<<"---"<<pp;
    LPActDeal pAct = (LPActDeal)(pp);
	QVector<LPActDeal> *pTVector = const_cast< QVector<LPActDeal> * >(&m_pTaskVector.at(nId) );
    for(int i = 0; i < nCnt; i++)
    {
        //qDebug()<<"*pp="<<*pp;
        //m_pTaskVector[nId].append(pAct);
		pTVector->append(pAct);
        pAct = pAct + 1;
    }
}

void ResManager::loadStr(void)
{
	changeLanguage(0, true);
	qDebug()<<"fuck1";
}

