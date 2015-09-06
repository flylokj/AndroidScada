#include <QMessageBox>
#include <QDebug>
#include <QTime>
#include <QSettings>
#include <QTextCodec>
#include "ResManager.h"
#include "ArchiveManager.h"
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>

//QString ArchiveManager::m_moldName = "";

extern QTcpSocket *client;

ArchiveManager::ArchiveManager(QObject *parent):
    QObject(parent),
    m_nAddrOff(0),
    m_nAddrRange(0),
    m_nNewestWarnId(0),
    m_nMaxWarnCnt(0),
    m_pParameter(0)
{
    loadWarnRecord();
    m_pCommunication = new Communication(this);//通讯模块;
}

ArchiveManager::~ArchiveManager()
{
	qDebug()<<"~ArchiveManager()";
    //保存模号参数;
    saveMold();
    //保存面板参数;
    savePanel();
    //保存报警记录;
    saveWarnRecord();
    //释放报警记录占用的字符串空间;
    releaseWarnRecord();

    if(m_pParameter)
        delete m_pParameter;

    int nMax = simulateMap.size();
    for(int i = 0; i < nMax; i++)
    {
        SimulateMan *p = simulateMap.value(i);
        delete p;
        p = NULL;
    }
    simulateMap.clear();

    //清空教导记录数据;
    clearTutorVec();
}

void ArchiveManager::setupSimulateManager(char *p)
{
    char *pTemp =(char *)p;//模拟变量数目(4 bytes)
    int* pMax = (int *)pTemp;
    int nMax = *pMax;
    pTemp = pTemp + sizeof(int)*2;//跳过模拟变量数目和模拟变量区域大小共2字节;
    for(int i =  0; i < nMax; i++)
    {
        SimulateMan *pSimulate = new SimulateMan(this);
        pSimulate->setupPrivate(pTemp);
        simulateMap.insert(i, pSimulate);
//        if(pSimulate->m_pData->bWarning)//监控;
//            m_pCommunication->addIdToReqBuf(i);
        pTemp = pTemp + sizeof(SimulateStruct);
    }

}

void ArchiveManager::setupAddrManger(int nAddrOff, int nAddrRange)
{

    if(nAddrOff < 0 || nAddrRange < 0)
    {
        QMessageBox::about(0, QString("Error"), QString("nAddrOff or nAddrRange must > 0"));
        m_nAddrOff = 0;
        m_nAddrRange = 0;
        return;
    }

    m_nAddrOff = nAddrOff;
    m_nAddrRange = nAddrRange;
    //读取模号配置文件;
    QSettings settings(OPTIONGFILE, QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("GB2312"));
    QString fileName = settings.value("SYS/mold").toString();
    loadMold(fileName.split(".").at(0));

    //读取面板参数;
    fileName = settings.value("SYS/panel").toString();
    loadPanel(fileName.split(".").at(0));
    qDebug()<<"$$$$$$$$$";
    //开启定时器;
    timer = new QTimer();
//    connect(timer, SIGNAL(timeout()), this, SLOT(saveTime()));
//    timer->start(1000);
}


void ArchiveManager::writeByte(int addr, char val)
{

    if(!checkAddr(addr))
        return;

    m_pParameter[addr] = val;
}

char ArchiveManager::readByte(int addr)
{
    if(!checkAddr(addr))
        return 0;
    return m_pParameter[addr];
}

void ArchiveManager::writeWord(int addr, int val)
{
    char temp = val&0xff;
    writeByte(addr,temp);
    temp = (val>>8)&0xff;
    writeByte(addr+1, temp);
}

int ArchiveManager::readWord(int addr)
{
    quint8 lowBtye = readByte(addr);
    quint8 highByte = readByte(addr+1);
    int temp = (highByte<<8);
    temp |= lowBtye;
    return temp;
}

void ArchiveManager::writeDWord(int addr, int val)
{
    int temp = val&0xffff;
    writeWord(addr, temp);
    temp = (val>>16)&0xffff;
    writeWord(addr+2, temp);
}

int ArchiveManager::readDWord(int addr)
{
    int lowWord = readWord(addr);
    int highWord = readWord(addr+2);
    int temp = lowWord | (highWord<<16);
    return temp;
}

//改变模号;
void ArchiveManager::saveMold()
{
    QString str(m_moldName);
    //模号参数保存;
    str.append(".bin");
    str.prepend(MOLDDIR);
    QFile file(str);
    if(!file.exists())
    {
        QMessageBox::warning(0, tr("error"), tr("file :%1 not exists").arg(str) );
        return;
    }

    if( !file.open(QIODevice::WriteOnly) )
        return;

    file.resize(MOLDSIZE);
    if( file.write(m_pParameter, MOLDSIZE) == -1)
        qDebug()<<"save mold data failed";
    else
        qDebug()<<"save mold data success";
    file.close();


    //教导参数保存;
    str = m_moldName;
    str.append(".tutor");
    str.prepend(MOLDDIR);
    QFile fileTutor(str);
    if( !fileTutor.open(QIODevice::WriteOnly) )
        return;
    QDataStream stream(&fileTutor);
    stream<<m_tutorVec.count();
    for(QVector<LPTutorRecordStr>::const_iterator it= m_tutorVec.begin(); it!=m_tutorVec.end(); it++)
    {
        LPTutorRecordStr pRecord = *it;
        stream<<pRecord->strList.count();
        for(QStringList::const_iterator i = pRecord->strList.begin(); i!= pRecord->strList.end(); i++)
        {
            stream<<(*i);
        }
        stream<<pRecord->record.nStep;
        stream<<pRecord->record.nType;
        qint16 *p = reinterpret_cast<qint16 *>(pRecord->record.pData);
        int nMax = 0;
        switch (pRecord->record.nType)
        {
            case AXIS_ACT_TUTOR_RECORD_TYPE://7
                nMax = 7;
            break;
            case ZTOOL_ACT_TUTOR_RECORD_TYPE://3
            case SIGNAL_ACT_TUTOR_RECORD_TYPE://3
            case CHECK_TUTOR_RECORD_TYPE://3
                nMax = 3;
            break;
            case EXTRA_DEV_TUTOR_RECORD_TYPE://4
            case PRESERVE_TUTOR_RECORD_TYPE://4
                nMax = 4;
                break;
            case LAYER_ACT_TUTOR_RECORD_TYPE://2
            case WAIT_TUTOR_RECORD_TYPE://2
            case OTHER_TUTOR_RECORD_TYPE://2
			case MOLD_END_TUTOR_RECORD_TYPE://2
                nMax = 2;
                break;
            default:
                break;
        }

        for(int i = 0; i < nMax; i++)
            stream<<p[i];

    }

    fileTutor.close();
    qDebug()<<"tutor record save success";
}

void ArchiveManager::changeMold(const QString strFile, bool bFromBackup)
{
    if(strFile.compare(m_moldName) == 0)
    {
        if(!bFromBackup)
            return;
    }
    else
        saveMold();
    loadMold(strFile);
    updateMold();

    QSettings settings(OPTIONGFILE, QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("GB2312"));
    settings.setValue("SYS/mold", m_moldName);
    qDebug()<<"change to mold "<<m_moldName;

}


//加载模号;
void ArchiveManager::loadMold(const QString strFile)
{
    //模号参数加载;
    QString str(strFile);
    str.append(".bin");
    str.prepend(MOLDDIR);
    int nExistflag = 1;
    QFile file(str);
    if(!file.exists())
        nExistflag = 0;
    if(!file.open(QIODevice::ReadWrite))
        return;//log!!!!!!!!!!!!!!!!;
    if(!nExistflag)
        file.resize(MOLDSIZE);

    if(!m_pParameter)
        m_pParameter = new char[PARMSIZE];

    memset(m_pParameter, 0, MOLDSIZE);

    if(!nExistflag)//如果不存在则初始化;
    {
       file.write(m_pParameter, MOLDSIZE);
    }
    else
    {
      if( file.read(m_pParameter, file.size()) != file.size())
         qDebug()<<QString("fucking read %1 error").arg(strFile);
      else
         qDebug()<<QString("fucking read %1 success").arg(strFile);
    }

    //qDebug()<<"parameter file success:off="<<m_nAddrOff<<"range="<<m_nAddrRange;
    m_moldName = strFile;
    file.close();


    //教导参数加载;
    str = m_moldName;
    str.append(".tutor");
    str.prepend(MOLDDIR);
    QFile fileTutor(str);
    if( !fileTutor.open(QIODevice::ReadOnly) )
        return;
    QDataStream stream(&fileTutor);
    clearTutorVec();
    int nCnt;
    stream>>nCnt;
    for(int i = 0; i < nCnt; i++)
    {
        LPTutorRecordStr pRecord = new TutorRecordStr;
        m_tutorVec.append(pRecord);
    }

    for(QVector<LPTutorRecordStr>::const_iterator it= m_tutorVec.begin(); it!=m_tutorVec.end(); it++)
    {
        QString strTemp;
        LPTutorRecordStr pRecord = *it;
        stream>>nCnt;
        for(int i= 0; i < nCnt; i++)
        {
            stream>>strTemp;
            pRecord->strList.append(strTemp);
        }
        stream>>pRecord->record.nStep;
        stream>>pRecord->record.nType;

        int nMax = 0;
        switch (pRecord->record.nType)
        {
            case AXIS_ACT_TUTOR_RECORD_TYPE://7
                nMax = 7;
            break;
            case ZTOOL_ACT_TUTOR_RECORD_TYPE://3
            case SIGNAL_ACT_TUTOR_RECORD_TYPE://3
            case CHECK_TUTOR_RECORD_TYPE://3
                nMax = 3;
            break;
            case EXTRA_DEV_TUTOR_RECORD_TYPE://4
            case PRESERVE_TUTOR_RECORD_TYPE://4
                nMax = 4;
                break;
            case LAYER_ACT_TUTOR_RECORD_TYPE://2
            case WAIT_TUTOR_RECORD_TYPE://2
            case OTHER_TUTOR_RECORD_TYPE://2
			case MOLD_END_TUTOR_RECORD_TYPE://2
                nMax = 2;
                break;
            default:
                break;
        }
        pRecord->record.pData = new qint16[nMax];
        qint16 *p = reinterpret_cast<qint16 *>(pRecord->record.pData);
        for(int i = 0; i < nMax; i++)
            stream>>p[i];

    }

    fileTutor.close();
    qDebug()<<"tutor record load success";
}

void ArchiveManager::loadPanel(const QString strFile)
{
    QString str(strFile);
    str.append(".param");
    str.prepend(MOLDDIR);
    int nExistflag = 1;
    QFile file(str);
    if(!file.exists())
        nExistflag = 0;
    if(!file.open(QIODevice::ReadWrite))
        return;
    if(!nExistflag)
        file.resize(PANELSIZE);
    qDebug()<<"$$$$$$$$$1";
    memset(m_pParameter+MOLDSIZE, 0, PANELSIZE);
    qDebug()<<"$$$$$$$$$2";
    if(!nExistflag)//如果不存在则初始化;
    {

       file.write(m_pParameter+MOLDSIZE, PANELSIZE);
    }
    else
    {
      if( file.read(m_pParameter+MOLDSIZE, PANELSIZE) != file.size())
         qDebug()<<QString("fucking read panel_param %1 error").arg(strFile);
      else
         qDebug()<<QString("fucking read panel_param %1 success").arg(strFile);
    }

    m_panelFile = strFile;
    file.close();
}

void ArchiveManager::savePanel()
{
    QString str(m_panelFile);
    str.append(".param");
    str.prepend(MOLDDIR);
    QFile file(str);
    if(!file.open(QIODevice::ReadWrite))
        return;

    file.write(m_pParameter+MOLDSIZE, PANELSIZE);

    file.close();

    qDebug()<<"saveing "<<str;
}

void ArchiveManager::test()
{
    char cTemp;
    int  nTemp;
    int addr = m_nAddrOff;
    writeByte(addr, 1);
    cTemp = readByte(addr);
    qDebug()<<QString("writeByte addr:%1 data:1").arg(addr);
    qDebug()<<QString("readByte addr:%1: data:%2").arg(addr).arg(QString::number(cTemp));
    addr = addr + 2;
    writeWord(addr, 65535);
    nTemp = readWord(addr);
    qDebug()<<QString("writeWord addr:%1 data:65535").arg(addr);
    qDebug()<<QString("readWord addr:%1: data:%2").arg(addr).arg(QString::number(nTemp));
    addr = addr + 4;
    writeDWord(addr, 4294967295);
    nTemp = readDWord(addr);
    qDebug()<<QString("writeDWord addr:%1 data:4294967295").arg(addr);//-1,因为int
    qDebug()<<QString("readDWord addr:%1: data:%2").arg(addr).arg(QString::number(nTemp));
}

void ArchiveManager::updateSysPage(int nPage)
{
    writeByte(SYS_ADDR_PAGE, nPage);
    emit(updateSys(PAGECHANGE, nPage, QString("")));
}

int ArchiveManager::getSysPage()
{
    int nPage = (int)readByte(SYS_ADDR_PAGE);
    return nPage;
}

void ArchiveManager::updateMold()
{
    emit(updateSys(MOLDNAME, 0, QString( m_moldName )));
}

void ArchiveManager::triggerRecord()
{
    int cnt = m_WarningMap.count();
    if(cnt < m_nMaxWarnCnt)
    {
        for(int i = 0; i < cnt; i++)
        {
            emit( updateWarn(*m_WarningMap.value(i), false) );
        }
    }
    else
    {
        int nBase = m_nNewestWarnId+1;
        int nMax = nBase+cnt;
        for(int i = nBase; i < nMax; i++)
        {
            emit( updateWarn(*m_WarningMap.value(i%m_nMaxWarnCnt), false) );
        }
    }

}

void ArchiveManager::loadWarnRecord()
{
    QSettings settings(".\\res\\warning.ini", QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("GB2312"));
    QString string;
    QString strTemp;
    //报警记录数目;
    int num = settings.value("RECORD/num").toInt();
    m_nMaxWarnCnt = settings.value("RECORD/max").toInt();
    m_nNewestWarnId = settings.value("RECORD/cur").toInt();
    for(int i = 0; i < num; i++)
    {
       strTemp = QString("RECORD/%1").arg(i);
       string = settings.value(strTemp).toString();
       qDebug()<<"record:"<<string;
       QString *pString = new QString(string);
       m_WarningMap.insert(i, pString);
    }
}

void ArchiveManager::saveWarnRecord()
{
    QSettings settings(".\\res\\warning.ini", QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("GB2312"));
    QString string;
    QString strTemp;
    //报警记录数目;
    settings.setValue(QString("RECORD/num"), m_WarningMap.count());
    settings.setValue(QString("RECORD/cur"), m_nNewestWarnId);
    qDebug()<<"saving warning record:"<<m_WarningMap.count();
    for(int i = 0; i < m_WarningMap.count(); i++)
    {
        strTemp = QString("RECORD/%1").arg(i);
        settings.setValue(strTemp, *m_WarningMap.value(i));
    }
}

void ArchiveManager::insertWarning(const int id)
{
    if(g_pResManModule->getWarningText(id).isEmpty())
        return;
    //获取当前时间，记录报警时间;
    QDate date = QDate::currentDate();
    QString text = date.toString(Qt::ISODate);
    QTime time = QTime::currentTime();
    text.append(" ");
    text.append( time.toString("hh:mm") );

    QString *strWarningText = new QString();
    strWarningText->append(QString::number(id));
    strWarningText->append("_");
    strWarningText->append(g_pResManModule->getWarningText(id));
    strWarningText->append("_");
    strWarningText->append(text);
    strWarningText->append("_");
    strWarningText->append(tr("not slove"));
    int cnt = m_WarningMap.count();
    if(cnt < m_nMaxWarnCnt)
    {
        m_WarningMap.insert(cnt, strWarningText);
        m_nNewestWarnId = cnt;
    }
    else
    {
        m_nNewestWarnId = ++m_nNewestWarnId%m_nMaxWarnCnt;
        QString *pStr = m_WarningMap.value(m_nNewestWarnId);
        m_WarningMap.remove(m_nNewestWarnId);
        delete pStr;
        m_WarningMap.insert(m_nNewestWarnId, strWarningText);
    }

    emit(updateWarn(*strWarningText, false));
    qDebug()<<*strWarningText;
}

void ArchiveManager::removeWarning(const int id)
{
    if(g_pResManModule->getWarningText(id).isEmpty())
        return;
    //解除报警，获取当前时间;
    QDate date = QDate::currentDate();
    QString text = date.toString(Qt::ISODate);
    QTime time = QTime::currentTime();
    text.append(" ");
    text.append( time.toString("hh:mm") );

    int cnt = m_WarningMap.count();
    if(cnt <= m_nNewestWarnId)
        return;
    QString *pStr = m_WarningMap.value(m_nNewestWarnId);
    int index = pStr->indexOf('_');
    if( pStr->left(index).toInt() == id )
    {
        qDebug()<<"reslove warning success";
        pStr->replace(pStr->lastIndexOf('_')+1, text.size(), text);
        qDebug()<<*pStr;
        emit(updateWarn(*pStr, true));
    }
}

void ArchiveManager::releaseWarnRecord()
{
    for(QMap<int, QString *>::const_iterator it=m_WarningMap.begin(); it!=m_WarningMap.end(); it++ )
    {
        QString *pStr = *it;
        delete pStr;
    }
    m_WarningMap.clear();
}

const QString &ArchiveManager::getMoldName()
{
    return m_moldName;
}

const QString &ArchiveManager::getPanelName()
{
    return m_panelFile;
}

bool ArchiveManager::checkAddr(int addr)
{
//    if(addr < m_nAddrOff || (addr > (m_nAddrOff + m_nAddrRange)) )
//    {
//        QMessageBox::warning(0,QString("警告"), QString("%1地址超出范围").arg(addr));
//        return false;
//    }
    //if(addr < 0 || (addr > (m_nAddrRange+m_nAddrOff)) )
	if(addr < 0 || (addr > PARMSIZE) )
    {
        QMessageBox::warning(0,QString("警告"), QString("%1地址超出范围").arg(addr));
        return false;
    }
    return true;
}

void ArchiveManager::clearTutorVec()
{
    for(QVector<LPTutorRecordStr>::const_iterator it = m_tutorVec.begin(); it != m_tutorVec.end(); it++)
    {
        LPTutorRecordStr p = *it;
        delete p->record.pData;
        delete p;
    }
    m_tutorVec.clear();
}

#include "UI/KJLBaseCtrl.h"
#include <QScriptEngine>
extern QScriptEngine *pGlobalScriptEngine;
void ArchiveManager::saveTime()
{
#if 1
    QDate date = QDate::currentDate();
    QString text = date.toString(Qt::ISODate);
    QTime time = QTime::currentTime();
    text.append("  ");
    text.append(time.toString("hh:mm:ss"));
    emit(updateTime(text));

    //存入内存地址中;
    SimulateMan *pSimulate = g_pArchiveModule->simulateMap.value(3, NULL);//获取对应的变量;
	if(pSimulate)
		pSimulate->write(time.hour());
    pSimulate = g_pArchiveModule->simulateMap.value(4, NULL);//获取对应的变量;
	if(pSimulate)
		pSimulate->write(time.minute());
    pSimulate = g_pArchiveModule->simulateMap.value(5, NULL);//获取对应的变量;
	if(pSimulate)
		pSimulate->write(time.second());
		/*writeByte(0x0, time.second());
		writeByte(0x1, time.minute());
		writeByte(0x2, time.hour());*/
    qDebug()<<"timer";

    ////test
    //SimulateMan *pSimulate = simulateMap.value(3, NULL);//获取对应的变量;
    //if(pSimulate)
    //    pSimulate->write(time.second());

//    //自动切换画面;
   // KJLBaseCtrl *pBase = KJLBaseCtrl::getInstance();
//    int temp = time.second()%10;
//    (pBase->*(pBase->m_pFundoAct[2]))(temp+1, 0);

    //自动切换语言;
    //(pBase->*(pBase->m_pFundoAct[5]))(0, 0);
#endif
}



