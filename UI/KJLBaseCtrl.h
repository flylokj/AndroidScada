#ifndef KJLBASECTRL_H
#define KJLBASECTRL_H
#include <QtCore/QObject>
#include "ResMan/ResManager.h"

class KJLBaseCtrl;
typedef void (KJLBaseCtrl::*LPdoAct) (int, int);

class KJLBaseCtrl : public QObject
{

    Q_OBJECT
public:
    static KJLBaseCtrl* getInstance();
    ~KJLBaseCtrl();
    LPdoAct m_pFundoAct[6];

protected:
    KJLBaseCtrl(QObject *parent=0);
    void doActSingleSet(int var, int val);//单控操作(置位操作)
    void doActSet(int var, int val);//模拟量设置操作;
    void doActLinkPage(int var, int val);//画面链接操作;
    void doActRunTask(int var, int val);//执行任务操作;
    void doActLinkSubWin(int var, int val);//子窗切换页面操作;
    void doActRunScript(int var, int val);//退出操作;

signals:
    void changePage(int nPage);
    void subWinChangePage(int nSubWin, int nPage);
};



#endif // KJLBASECTRL_H

