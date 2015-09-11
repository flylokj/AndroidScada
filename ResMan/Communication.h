#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QTimer>
#include <QList>
#include <QMutex>
#include <QTime>
#include "Protocol.h"

#define MONITOR_TIME 50

class Communication : public QObject
{
    Q_OBJECT
public:
    explicit Communication(QObject *parent = 0);
    ~Communication();
    void sendData(QByteArray &srcBuffer);//数据加入缓冲区;
    void addIdToReqBuf(int id);
    void delIdFromReqBuf(int id);
    void printMonitor();//打印监控列表;
    void updateMonitorToHost();//更新监控列表到主机;



private:
    QByteArray m_SendBuffer;            //发送缓冲区;
    QTimer *m_checkTimer;               //定时器，用于检测ACK
    QTimer *m_reqTimer;                 //定时监测定时器，用于定时获取监控变量;
    QTime m_testTime;                   //用于测试延时;
    int m_nMonitorIndex;                //当前的监控变量ID;
    bool m_bMonitor;                    //使能读取监控变量
    bool m_bSendSuccess;                //发送成功标志位;
    bool m_bConnected;                  //连接服务器标志;
    bool m_bDownLoadMoniotorSuccess;    //下载监控列表成功;
    bool m_bSendOnece;                  //只发一次;
    QList<QByteArray> m_sendBuf;        //发送队列;
    QList<int> m_reqBuf;                //变量监控列表，存储SimulateMan变量的ID值;
    QByteArray m_readMonitorCmd;        //读取监控信息,置位M8040命令;
    int m_nTempCoilCode;                //临时变量识别,识别监控列表第一个变量;
    QMutex m_sendMutex;


//methods:
    void send();//读取缓冲区并发送;
public:
    bool isConnected()const;
    void connectToHost();
    void setDCoilVar(quint16);//请求读监控变量成功;
    void writeData(mbyte coil, uint16 addr, uint16 dataGet);
    void sendSuccess();



public slots:
    void doCheckTimer();//定时检测;
    void doMonitorVar();//定时读取监控变量;
    void readData();//读取数据;
    void doConnected();//建立连接成功;
    void doDisConnected();//断开连接;
};

#endif // COMMUNICATION_H
