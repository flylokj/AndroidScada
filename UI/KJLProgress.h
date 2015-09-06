#ifndef KJLProgress_H
#define KJLProgress_H
#include<QLabel>
#include"Basectrl.h"
#include<ResMan/SimulateMan.h>

class KJLProgress : public QLabel
{
    Q_OBJECT
public:
    KJLProgress(QWidget *parent=0, char *data=0);
    ~KJLProgress();

private:
    enum {NORMAL, UPALARM, DOWNALRAM};
    SimulateMan *m_pSimulate;//绑定的模拟变量;
    LPBaseCtrl m_pData;
    LPProgressPri m_pPriData;
    int m_nProgressFlag;//进度标志;0-2:正常， 过高，过低;
    int m_nMax;//最大值;
    int m_nData;//当前值;
    int m_nMin;//最小值;
    int m_nDecimal;//小数点;
    int m_nUpAlarm;//过高警告值;
    int m_nDwnAlarm;//过低警告值;
    QString m_strMax;//最大值字符串;
    QString m_strMin;//最效值字符串;

protected:
    virtual void paintEvent(QPaintEvent *event);


private slots:
    void onUpdateTime(const QString time);
    void onUpdateData(int, SimulateMan*, QObject*);//更新对应的变量数据;
    void onUpdateSys(const SysStatus type, const int data, const QString str);//更新系统状态;
};

#endif // KJLProgress_H
