#ifndef KJLBUTTON_H
#define KJLBUTTON_H
#include "Basectrl.h"
#include "KJLBaseCtrl.h"
#include <QPushButton>
#include <ResMan/ArchiveManager.h>

class KJLButton : public QPushButton
{
    Q_OBJECT
public:
    KJLButton(QWidget *parent=0, char *data=0);
    ~KJLButton();

private:
    LPBaseCtrl m_pData;
    LPBtnPri m_pPriData;
    SimulateMan *m_pSimulate;
    KJLBaseCtrl *pKJLBaseCtrl;
    bool bFocus;
    bool bPress;

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void focusInEvent(QFocusEvent * event);
    virtual void focusOutEvent(QFocusEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);

signals:

private slots:
    void onUpdateData(int, SimulateMan*, QObject*);//更新对应的变量数据;

};

#endif // KJLBUTTON_H
