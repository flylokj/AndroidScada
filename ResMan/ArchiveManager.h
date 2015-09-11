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

//!档案管理模块,对外提供接口:
/*!
 * 模号切换.\n
 * 加载面板参数.\n
 * 获取模号名称.\n
 * 获取面板参数文件名称.\n
 * 更新当前页面id与标题显示.\n
 * 获取当前页面ID\n
 * 更改用户密码.\n
 * 触发显示报警信息.\n
 * 插入报警信息.\n
 * 删除报警信息.\n
 *
 * 教导参数列表.\n
 * 模拟变量列表.\n
 * 通讯管理模块.\n
 */
class ArchiveManager : public QObject
{
    Q_OBJECT
public:
    ArchiveManager(QObject *parent = 0);
    ~ArchiveManager();
    friend class SimulateMan;                               //友类SimulateMan的声明;

    void changeMold(const QString strFile, bool bFromBackup = false);//!<模号改变操作;
    void loadPanel(const QString strFile);                  //!<加载面板参数;
    const QString &getMoldName();                           //!<获取模号名称;
    const QString &getPanelName();                          //!<获取面板名称;
    void updateSysPage(int page);                           //!<更新当前页标题显示;
    int getSysPage();                                       //!<获取当前页面ID;
    void changePwd(int who, int oldPwd, int newPwd);        //!<更改密码;
    void test();                                            //!<测试本模块的读写功能;

    void triggerDisplayRecord();                            //!<触发显示报警信息;
    void insertWarning(const int id);                       //!<插入报警信息;
    void removeWarning(const int id);                       //!<解除报警信息;

    QVector<LPTutorRecordStr> m_tutorVec;                   //!<教导参数列表;
    QMap<int, SimulateMan*>simulateMap;                     //!<模拟量管理链表;
    Communication *m_pCommunication;                        //!<通讯模块;


private:
    int m_nAddrOff;                                         //地址偏移量;
    int m_nAddrRange;                                       //地址范围;
    char *m_pParameter;                                     //参数缓冲区;
    QString m_moldName;                                     //模号名称;
    QString m_panelFile;                                    //面板参数文件名称;
    int m_nAdminPwd;                                        //!<管理员密码;
    int m_nSeniorAdminPwd;                                  //!<高级管理员密码;
    QTimer * timer;                                         //定时器;
    int m_nNewestWarnId;                                    //最新报警记录;
    int m_nMaxWarnCnt;                                      //最大报警记录条目;
    QMap<int, QString *>m_WarningMap;                       //报警信息缓冲区;

    bool checkAddr(int addr);                               //检查地址有效性;
    void clearTutorVec();                                   //清空教导记录;
    void setupSimulateManager(char *p);                     //建立模拟变量管理;
    void setupAddrManger(int nAddrOff, int nAddrRange);     //建立地址管理;
    void loadMold(const QString strFile);                   //加载模号;
    void saveMold();                                        //保存模号操作;
    void updateMold();                                      //更新显示模号;
    void savePanel();                                       //保存面板参数;

    void writeByte(int addr, char val);                     //写1字节地址操作;
    void writeWord(int addr, int val);                      //写2字节地址操作;
    void writeDWord(int addr, int val);                     //写4字节地址操作;
    char readByte(int addr);                                //读1字节地址操作;
    int readWord(int addr);                                 //读2字节地址操作;
    int readDWord(int addr);                                //读4字节地址操作;

    void loadWarnRecord();                                  //加载报警记录;
    void saveWarnRecord();                                  //保存报警记录;
    void releaseWarnRecord();                               //释放m_WarningMap的空间;

signals:
    void updateTime(const QString time);
    void updateSys(const SysStatus type, const int data, const QString str);//更新系统信息;
    void updateWarn(const QString &pStr, bool reslv);

private slots:
    void saveTime();

};

#endif // ADDRMANAGER_H


