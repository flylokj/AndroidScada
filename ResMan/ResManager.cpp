#include "ResManager.h"
#include <QFile>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <QTextCodec>
#include <QTextStream>


//!构造函数
/*!
 * 切换默认语言;
 * 加载16进制数据(建立画面列表和任务列表);
 * 加载系统配置;
 * 加载图片;
 * 加载字符串;
 * 加载警告信息;
 */
ResManager::ResManager(QObject *parent)
    : QObject(parent)
    , m_nLanguageSel(0)
    , m_nMaxLanguage(0)
    , pHex(0)
    , m_pTranslator(new QTranslator())
{
    g_pResManModule = this;
    m_pTranslator->load(LAGNUAGEFILE);//加载默认语言包;
    loadHex();
    loadLanguageConfig();
    loadImg();
    loadStr();
    loadWarnText();

    m_dXfactor = (double)g_pResManModule->m_pProjectParm->m_nWidth / 800;//因为在UI文件中布局时候是假设面板大小为800*600;
    m_dYfactor = (double)g_pResManModule->m_pProjectParm->m_nHeight / 600;//因为在UI文件中布局时候是假设面板大小为800*600;
}

//!析构函数
/*!
 * 释放m_pTranslator
 * 释放16进制数据缓冲区;
 * 清除子页面列表;
 * 清除任务列表;
 * 释放图片;
 * 释放字符串;
 * 释放警告信息;
 */
ResManager::~ResManager()
{
	qDebug()<<"~ResManager()";
    m_pTranslator->deleteLater();

    if(pHex)
        delete pHex;
    pHex = NULL;

    m_pageStruct.pSubPageVector.clear();
	m_pTaskVector.clear();
    releaseImgRes();
    releaseStrRes();
    releaseWarnRes();

}

//!加载16进制数据文件,Hex文件结构如下：
/*!
 *   |变量数目|(4)\n
 *   |变量区域大小|(4)\n
 *   |变量|(不定)\n
 *   |工程参数|(ProjectParm结构大小)\n
 *   |面板参数|(PanelParm结构大小)\n
 *   |一级页面数目|(4)\n
 *   |页面0头|(PageHead结构大小)\n
 *   |页面0控件列表参数|(不定)\n
 *   |页面1头|(PageHead结构大小)\n
 *   |页面1控件列表参数|(不定)\n
 *   ....\n
 *   |页面n头|(PageHead结构大小)\n
 *   |页面n控件列表参数|(不定)\n
 *   |一级页面0对应的子画面列表|\n
 *   |一级页面1对应的子画面列表|\n
 *   ...\n
 *   |一级页面n对应的自画面列表|\n
 *   |任务个数|(4)\n
 *   |任务0描述|\n
 *   |任务1描述|\n
 *   ....\n
 *   |任务n描述|\n
 */
void ResManager::loadHex()
{
    char *pTemp;
    QFile file(HEXFILE);
    if(!file.open(QIODevice::ReadOnly))
        return;

    qint64 size = file.size();
    pHex = new char[size];
    file.read(pHex, size);//是否该判断读取的大小?????????????????

    m_pSimulateVar = pHex;//模拟变量数目(占4 bytes)
    pTemp = m_pSimulateVar;
    pTemp = (char *)((int *)pTemp + 1);//模拟变量描述区域的大小(4 bytes)
    pTemp = (char *)(pTemp + *((int *)pTemp));//工程参数;
    m_pProjectParm = (LPProjectParm)pTemp;
    m_pPanelParm = (LPPanelParm)(pTemp+sizeof(ProjectParm));//面板参数;

    //页面数目;
    int *pNum = (int*)((char*)m_pPanelParm + sizeof(PanelParm));
    m_pageStruct.nPageNum = *pNum;
    LPPageHead pPageHead = (LPPageHead)((char*)pNum + sizeof(int));
    for(int i = 0; i < *pNum; i++)
    {
        m_pageStruct.pageVector.append(pPageHead);
        pPageHead = (LPPageHead)((char *)pPageHead + pPageHead->nPageSize);
    }

    //建立子画面矩阵;
    m_pageStruct.pSubPageVector.resize(*pNum);
    LPSubPageListHead pSubPageListHead = (LPSubPageListHead)pPageHead;
    for(int i = 0; i < *pNum; i++)
    {
        setupSubPageVector(pSubPageListHead, i);
        pSubPageListHead = (LPSubPageListHead)((char *)pSubPageListHead + pSubPageListHead->nSubPageListSize);
    }

    //建立任务列表;
    pNum = (int *)pSubPageListHead;//任务个数;
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

    file.close();

}

//!加载语言配置选项,如下选项;
/*!
 * 最大语言数目;\n
 * 当前选中的语言;\n
 */
void ResManager::loadLanguageConfig()
{
    QSettings settings(OPTIONGFILE, QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("GB2312"));
    m_nMaxLanguage = settings.value("SYS/MaxLanguage").toInt();//最大支持的语言数目;
    m_nLanguageSel = settings.value("SYS/LanguageSel").toInt();//选中的语言;
}



void ResManager::loadImg()
{
    QSettings settings(OPTIONGFILE, QSettings::IniFormat);
    QString strPath;
    QString strTemp;
    int num = settings.value("IMG/num").toInt();
    for(int i = 0; i < num; i++)
    {
        strTemp = QString("IMG/%1").arg(i);
        strPath = settings.value(strTemp).toString();
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
	
	int num = settings.value("WARN/num").toInt();
	for(int i = 0; i < num; i++)
	{
		strTemp = QString("WARN/%1").arg(i);
		string = settings.value(strTemp).toString();
//		qDebug()<<string;
		QString *pString = new QString(string);
		m_warnVec.append(pString);
	}
}


//加载字符串资源;
void ResManager::loadString()
{
     QSettings settings(OPTIONGFILE, QSettings::IniFormat);
     settings.setIniCodec(QTextCodec::codecForName("GB2312"));
     QString string;
     QString strTemp;
     QString strLanguage="STRING";//语言选择;
     strLanguage = strLanguage + QString::number(m_nLanguageSel);

     int num = settings.value(strLanguage+"/num").toInt();
     for(int i = 0; i < num; i++)
     {
        strTemp = QString(strLanguage+"/%1").arg(i);
        string = settings.value(strTemp).toString();
        //qDebug()<<string;
        QString *pString = new QString(string);
        m_strVec.append(pString);
     }
}

/**
* @param nLanguage 指示使用哪种语言 0:中文 1:英文.
* @param bInit 是否第一次启动该程序.
* @see ResManager()
* @return void
*/
void ResManager::changeLanguage(int nLanguage, bool bInit/* = false*/)
{
	if(bInit)
		nLanguage = m_nLanguageSel;
    int nTemp = nLanguage%m_nMaxLanguage;

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

    if(m_nLanguageSel==0)//中文;
	{
		qApp->installTranslator(m_pTranslator);
	}
    else//英文;
	{
		qApp->removeTranslator(m_pTranslator);
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
		pPVector->append(pPageHead);
        pPageHead = (LPPageHead)((char *)pPageHead + pPageHead->nPageSize);
    }
}

//!建立任务列表
/*!
 *  |任务大小|(4)
 *  |动作的个数|(4)
 *  |动作0|
 *  |动作1|
 *  ...
 *  |动作n|
 */
void ResManager::setupTaskVector(int *p, int nId)
{
    int *pp = p;
    pp = pp + 1;
    if(*pp == 0)
        return;
    int nCnt = *pp;//动作的个数;
    pp = pp + 1;//指向第一条动作;
    LPActDeal pAct = (LPActDeal)(pp);
	QVector<LPActDeal> *pTVector = const_cast< QVector<LPActDeal> * >(&m_pTaskVector.at(nId) );
    for(int i = 0; i < nCnt; i++)
    {
		pTVector->append(pAct);
        pAct = pAct + 1;
    }
}

void ResManager::loadStr(void)
{
	changeLanguage(0, true);
}

