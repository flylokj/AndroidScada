#ifndef KJLEDITBOX_H
#define KJLEDITBOX_H

#include <QLineEdit>
#include <QtGui/QKeyEvent>
#include "Basectrl.h"
#include "ResMan/ArchiveManager.h"
#include <QDebug>

class KJLEditBox : public QLineEdit
{
    Q_OBJECT
public:
    KJLEditBox(QWidget *parent=0, char *data=0);
    ~KJLEditBox();

    void setDataFmt(const int nDecimal, const QString &min, const QString &max, const QString &iniText);


protected:
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * e);
    virtual void closeEvent(QCloseEvent * event);


private:
//data:
    LPBaseCtrl m_pData;//控件通用属性;
    LPEditPri m_pPriData;//编辑框控件的私有属性;
    SimulateMan *m_pSimulate;//绑定的模拟变量;
    //QValidator *m_pValidator;//格式控制;
    int m_nMax;//最大值;
    int m_nMin;//最小值;
    int m_nDecimal;//小数位;
    QString m_strMax;//最大值字符串;
    QString m_strMin;//最小值字符串;
    QString m_strDisp;//显示的字符串;

//methods:
    void checkValidity(QString &);
    void showWarning();

private slots:
     void fmText(const QString &);//限制输入字符串;
     void onUpdateTime(const QString time);
     void onUpdateData(QString, SimulateMan*, QObject*);//更新对应的变量数据;
     void onUpdateData(int, SimulateMan*, QObject*);//更新对应的变量数据;


};

#endif // KJLEDITBOX_H
