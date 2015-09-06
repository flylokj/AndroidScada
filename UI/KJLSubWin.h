#ifndef KJLSUBWIN_H
#define KJLSUBWIN_H

#include <QWidget>
#include "Basectrl.h"
#include "ResMan/ResManager.h"
#include "ResMan/ArchiveManager.h"
#include "ResMan/SimulateMan.h"

class KJLSubWin : public QWidget
{
    Q_OBJECT
public:
    explicit KJLSubWin(QWidget *parent = 0, char *data=0);
    ~KJLSubWin();

private:
    SimulateMan *m_pSimulate;
    LPBaseCtrl m_pData;
    LPSubWinPri m_pPriData;
    int *m_pSubPage;//子画面偏移量指针;
    QVector<QWidget *>m_ctrlVec;//控件列表;
    int m_nCurSubPage;//当前子画面;

    void clearCtrlVec();//清除控件列表;

protected:
    virtual void closeEvent(QCloseEvent * event);

signals:

public slots:
    void onChangePage(int nPage);
    void onUpdateData(int, SimulateMan*, QObject*);//更新对应的变量数据;
};

#endif // KJLSUBWIN_H
