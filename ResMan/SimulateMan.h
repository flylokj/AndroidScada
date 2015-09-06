#ifndef SIMULATEMAN_H
#define SIMULATEMAN_H

#include <QtCore/QObject>

class ArchiveManager;

typedef struct tagSimulateStruct
{
    int nAddr;
    char nAddrType;
    char nDataType;
    char nDecimal;
    char nSavePeriod;
    int nMax;
    int nMin;
    int nUpAlarm;
    int nDownAlarm;
    int nBase;
    int nFactor;
    bool bWarning;
	bool bMinDollar;//最小值间接寻址;
	bool bMaxDollar;//最大值间接寻址;

}SimulateStruct, *LPSimulateStruct;


class SimulateMan : public QObject
{
    Q_OBJECT
public:
    explicit SimulateMan( ArchiveManager *pArch, QObject *parent = 0);
    ~SimulateMan();
    void setupPrivate(char *p);
    void getFmt(int *pMax, int *pMin, int *pDecimal, QString &strMax, QString &strMin);
    void getFmt(int *pMax, int *pMin, int *pDecimal, int *pUpAlarm=0, int *pDwnAlarm=0);
    void write(const QString &data, QObject *who = 0, bool bSend = true);
    void write(const int data, QObject *who = 0, bool bSend = true);
    QString read();
    int readInt();
    void trigle();//触发读取数据;
    void addToMonitorList();//加入监控队列;
    void delFromMonitorList();//从监控队列清除;


    QByteArray m_coilCode;
    SimulateStruct *m_pData;//私有数据;

signals:
    void updateData(const QString data, SimulateMan *pVar, QObject *who);
    void updateData(const int data, SimulateMan *pVar, QObject *who);
public slots:

private:
    ArchiveManager *m_pArch;

//methods:
    void sendOut();//将数据发送出去(主机);
};

#endif // SIMULATEMAN_H
