#ifndef KJLSTATIC_H
#define KJLSTATIC_H
#include<QLabel>
#include"Basectrl.h"
#include"ResMan/SimulateMan.h"
#include"ResMan/ArchiveManager.h"

class KJLStatic : public QLabel
{
    Q_OBJECT
public:
    KJLStatic(QWidget *parent=0, char *data=0);
    ~KJLStatic();

    void setData(void *data, bool byHand = false);

private:
    LPBaseCtrl m_pData;
    LPStaticPri m_pPriData;
    SimulateMan *m_pSimulate;
    QString m_dispData;

private slots:
    void onUpdateData(int, SimulateMan*, QObject*);//更新对应的变量数据;
    void onUpdateData(QString data, SimulateMan *pVar, QObject *who);

};

#endif // KJLSTATIC_H
