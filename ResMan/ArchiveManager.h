#ifndef ADDRMANAGER_H 
#define ADDRMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtCore/QMap>
#include <QtCore/QTimer>
#include <QtCore/QVector>
#include "SimulateMan.h"
#include "UI/Basectrl.h"
#include "Communication.h"

extern ArchiveManager *g_pArchiveModule;//档案管理模块;



class ArchiveManager : public QObject
{
    Q_OBJECT
public:
    ArchiveManager(QObject *parent = 0);
    ~ArchiveManager();

    //建立模拟变量管理;
    void setupSimulateManager(char *p);
    //建立地址缓冲管理区域;
    void setupAddrManger(int nAddrOff, int nAddrRange);//建立地址管理子模块;
    //写1字节地址操作;
    void writeByte(int addr, char val);
    //读1字节地址操作;
    char readByte(int addr);
    //写2字节地址操作;
    void writeWord(int addr, int val);//需要枷锁;
    //读2字节地址操作;
    int readWord(int addr);
    //写4字节地址操作;
    void writeDWord(int addr, int val);
    //读4字节地址操作;
    int readDWord(int addr);
    //保存操作;
    void saveMold();
    //模号改变操作;
    void changeMold(const QString strFile, bool bFromBackup = false);
    //加载模号;
    void loadMold(const QString strFile);
    //加载面板参数;
    void loadPanel(const QString strFile);
    //保存面板参数;
    void savePanel();

    //测试本模块的读写功能;
    void test();

//系统变量操作:
    //更新当前页;
    void updateSysPage(int page);
    int getSysPage();
    //更新模号;
    void updateMold();

//报警操作:
    void triggerRecord();//触发读取报警信息;
    void loadWarnRecord();//加载报警记录;
    void saveWarnRecord();//保存报警记录;
    void insertWarning(const int id);//插入报警信息;
    void removeWarning(const int id);//解除报警信息;
    void releaseWarnRecord();//释放m_WarningMap的空间;
    QMap<int, QString *>m_WarningMap;//报警信息缓冲区;
    QVector<LPTutorRecordStr> m_tutorVec;//教导信息;
    QMap<int, SimulateMan*>simulateMap; //模拟量管理链表;
    Communication *m_pCommunication;

    const QString &getMoldName();
    const QString &getPanelName();

private:
    int m_nAddrOff;//地址偏移量;
    int m_nAddrRange;//地址范围;
    char *m_pParameter;//参数缓冲区;
    QString m_moldName;//模号名称;
    QString m_panelFile;//面板参数文件名称;
    QTimer * timer;//定时器;
    int m_nNewestWarnId;//最新报警记录;
    int m_nMaxWarnCnt;//最大报警记录条目;

private:
    bool checkAddr(int addr);//检查地址有效性;
    void clearTutorVec();//清空教导记录;

signals:
    void updateTime(const QString time);
    void updateSys(const SysStatus type, const int data, const QString str);//更新系统信息;
    void updateWarn(const QString &pStr, bool reslv);

public slots:
    void saveTime();


};

#endif // ADDRMANAGER_H


