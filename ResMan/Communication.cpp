    #include "Communication.h"
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QMessageBox>
#include <QException>
#include <QThread>
#include "ResMan/ArchiveManager.h"
#include "plc_instruction.h"

QTcpSocket *client;
int nReqSuccess = 1;
quint16 id;


uint16 writeFunc( quint8* data, uint16 sz )
{
    QString strtemp;
    for(int i = 0; i < sz; i++)
        strtemp +=QString(" %1").arg(QString::number(data[i]));
    qDebug()<<"send "<<strtemp;

    if(!g_pArchiveModule->m_pCommunication->isConnected())
    {
        QMessageBox::warning(0, QObject::tr("warning"), QObject::tr("Not found server, connected server again..."));
        g_pArchiveModule->m_pCommunication->connectToHost();
    }

    if(sz != client->write((const char*)data, sz) )
        QMessageBox::warning(0, QObject::tr("warning"), QObject::tr("Send failed!please check network"));

}



int API_WriteData(mbyte coil, uint16 addr, uint16 dataGet)
{
    //qDebug()<<QString("addr:%1 data:%2").arg(addr).arg(dataGet);
    g_pArchiveModule->m_pCommunication->writeData(coil, addr, dataGet);

    //g_pArchiveModule->m_pCommunication->setDCoilVar(dataGet);
    return 1;
}



Communication::Communication(QObject *parent) :
    QObject(parent),
    m_bSendSuccess(true),
    m_bMonitor(false),
    m_bDownLoadMoniotorSuccess(false),
    m_bSendOnece(false),
    m_nMonitorIndex(0)
{
    //初始化通讯,连接服务器;
    qDebug()<<"connect to communication server";
    client = new QTcpSocket();
    //client->connectToHost(QHostAddress::LocalHost, 6665);
    connectToHost();
    QObject::connect(client, SIGNAL(readyRead()), this, SLOT(readData()) );
    QObject::connect(client, SIGNAL(connected()), this, SLOT(doConnected()));
    QObject::connect(client, SIGNAL(disconnected()), this, SLOT(doDisConnected()) );

    //注册通讯函数;
    qDebug()<<"Initialize the ProtocolConf structure";
    ProtocolConf proConf;
    proConf.write = writeFunc;
    SetProtocolConf( &proConf, PROTOCOL_CONF_WRITEFUN );

    //检测ACK定时器;
    m_checkTimer = new QTimer(this);
    connect(m_checkTimer, SIGNAL(timeout()), this, SLOT(doCheckTimer()) );

    //定时读取监控变量;
    m_reqTimer = new QTimer(this);
    connect(m_reqTimer, SIGNAL(timeout()), this, SLOT(doMonitorVar()) );
    m_reqTimer->start(100);

    //置位M8040；
    m_readMonitorCmd.append(CMD_FORCEON);
    m_readMonitorCmd.append(dcoilM);
    m_readMonitorCmd.append((MONITOR_ADDR>>8)&0xff);
    m_readMonitorCmd.append(MONITOR_ADDR&0xff);

    id = 250;

}

Communication::~Communication()
{
    client->disconnectFromHost();
}

void Communication::sendData(QByteArray &srcBuffer)
{
    m_sendMutex.lock();
    m_sendBuf.append(srcBuffer);
    m_sendMutex.unlock();
    send();//读取缓冲区并发送;
}

bool Communication::isConnected() const
{
    return m_bConnected;
}

void Communication::connectToHost()
{
#ifdef MYANDROID
    client->connectToHost("192.168.123.88", 20158);
#else
    client->connectToHost(QHostAddress::LocalHost, 6665);
#endif
}

void Communication::addIdToReqBuf(int id)
{
    if(id < 0)
        return;

    m_reqBuf.append(id);  
}

void Communication::delIdFromReqBuf(int id)
{
    if(id < 0)
        return;

    m_reqBuf.removeOne(id);
}

void Communication::setDCoilVar(quint16 dataGet)
{
    if(m_nMonitorIndex >= m_reqBuf.size())
        return;
    SimulateMan *pSimula = g_pArchiveModule->simulateMap.value(m_reqBuf.at(m_nMonitorIndex), NULL);
    if( !pSimula )
        return;

    pSimula->write(dataGet, 0, false);
    nReqSuccess = 1;
}

void Communication::printMonitor()
{
    qDebug()<<"----------------------Monitor----------------";
    for(int i = 0; i < m_reqBuf.size(); i++)
        qDebug()<<m_reqBuf.at(i);
    qDebug()<<"---------------------------------------------";
}

void Communication::updateMonitorToHost()
{
    m_bDownLoadMoniotorSuccess = false;
    m_bSendOnece = false;
    if(m_reqBuf.size() == 0)
        return;

    QByteArray temp("");

    //发送监控列表ID;
    id = (++id)%256;
    temp.append(CMD_WRITE);
    temp.append(dcoilD);
    temp.append((MONITORID_ADDR>>8)&0xff);
    temp.append((char)(MONITORID_ADDR&0xff));
    temp.append(0x01);
    temp.append((id>>8)&0xff);
    temp.append(id&0xff);
//    API_ProtocolSend((mbyte *)temp.data(), temp.size());
    sendData(temp);

    temp.clear();
    //发送监控列表;
    for(int i = 0; i < m_reqBuf.size(); i++)
    {
        SimulateMan *pSimula = g_pArchiveModule->simulateMap.value(m_reqBuf.at(i), NULL);
        if( !pSimula )
            continue;
        temp.append(pSimula->m_coilCode);
    }
    if(temp.size()!=m_reqBuf.size()*4)
        return;

    temp.prepend((char)(MONITORID_ADDR&0xff));//监控ID;
    temp.prepend((MONITORID_ADDR>>8)&0xff);
    temp.prepend(dcoilD);
    temp.prepend((char)0);
    temp.prepend((quint8)((m_reqBuf.size()+1)*2));//包含了id;
    temp.prepend(MONITOR_ADDR&0xff);
    temp.prepend((MONITOR_ADDR>>8)&0xff);
    temp.prepend(dcoilD);
    temp.prepend(CMD_WRITE);
    sendData(temp);
    //API_ProtocolSend((mbyte *)temp.data(), temp.size());
}

void Communication::writeData(mbyte coil, uint16 addr, uint16 dataGet)
{
    //qDebug()<<"fuck****************"<<coil<<" "<<addr<<" "<<dataGet;
    if((quint8)coil == dcoilD && addr >= 8040)//监控信息;
    {
        static bool flag = false;
        if(addr == MONITOR_ADDR && id == dataGet)//检查ID是否对应;
        {
            flag = true;
            m_nMonitorIndex = 0;
            m_bDownLoadMoniotorSuccess = true;
            return;
        }
        else if( addr == MONITOR_ADDR && id != dataGet )
            qDebug()<<"receive id not right "<< dataGet<<"!="<<id;
        if(!flag)
            return;

        if(m_nMonitorIndex >= m_reqBuf.size())
            return;

        int nSimulaIndex = m_reqBuf.at(m_nMonitorIndex++);
        SimulateMan *pSimula = g_pArchiveModule->simulateMap.value(nSimulaIndex, NULL);
        if(!pSimula)
            return;

        pSimula->write(dataGet, 0, false);
        //qDebug()<<"write data: "<<dataGet<<" id:"<<id;
    }
}


void Communication::send()
{
    //发送队列读取;
    if(m_sendBuf.size())
    {
        m_SendBuffer= m_sendBuf.at(0);
        API_ProtocolSend((mbyte *)m_SendBuffer.data(), m_SendBuffer.size());
        m_bSendSuccess = false;//等待ACK状态;
        m_checkTimer->start(1000);//开定时检测;
    }

}

void Communication::doCheckTimer()
{
    if(m_bSendSuccess)
        m_checkTimer->stop();
    else
        send();

    //qDebug()<<"resend..."<<m_bSendSuccess;
    //if(!m_bSendSuccess || nReqSuccess==0)

}

void Communication::doMonitorVar()
{
    if(m_reqBuf.size() == 0)
        return;

    if(!m_bDownLoadMoniotorSuccess)
    {
        if(!m_bSendOnece)
        {
            sendData(m_readMonitorCmd);
            m_bSendOnece = true;
        }

    }
    else
    {
        m_bSendOnece = false;
        sendData(m_readMonitorCmd);
    }


    if(!m_bSendSuccess)
        return;
    else
        send();
}

void Communication::readData()
{
    QByteArray data = client->readAll();
    qDebug()<<"recv:"<<data.size();
    for(int i = 0; i < data.size(); i++)
        qDebug()<<(quint8)data.at(i);

    if( data.size() == 1 )
    {
        //qDebug()<<(mbyte)data.at(0);
        if(m_bSendSuccess)
            return;
        if( data.at(0) == ACK )//接收到正确回复;
        {
            m_bSendSuccess = true;
            m_SendBuffer.clear();
            if(m_sendBuf.size() > 0)
            {
                m_sendBuf.removeFirst();
                if(m_sendBuf.size() > 0)
                    send();
            }

            QMessageBox::information(0, "info", "send success");
        }
    }
    else//读取的监控信息:X,Y,M等;
    {
        API_Protocol((mbyte *)data.data(), data.size());//解析协议;
        //检测是否成功;
    }
}

void Communication::doConnected()
{
    m_bConnected = true;
}

void Communication::doDisConnected()
{
    m_bConnected = false;
}


