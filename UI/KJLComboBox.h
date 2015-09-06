#ifndef KJLCOMBOBOX_H
#define KJLCOMBOBOX_H

#include<QComboBox>
#include"Basectrl.h"
#include"ResMan/SimulateMan.h"

class KJLComboBox : public QComboBox
{
    Q_OBJECT
public:
    KJLComboBox(QWidget *parent=0, char *data=0);
    ~KJLComboBox();

private:
    LPBaseCtrl m_pData;
    LPEditPri m_pPriData;
    SimulateMan *m_pSimulate;//绑定的模拟变量;
    int *m_pOpt;//选项;


private slots:
    void onCurrentIndexChanged(int index);
    void onUpdateData(int, SimulateMan*, QObject*);//更新对应的变量数据;

};

#endif // KJLCOMBOBOX_H
