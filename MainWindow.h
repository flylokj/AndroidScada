#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ResMan/ResManager.h>
#include <QtCore/QVector>
#include <QPushButton>
#include <QKeyEvent>
#include <QPropertyAnimation>
#include "UI/KJLSubWin.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

        QVector<QWidget* >m_animationButton;        //特效按钮;
        void startSmoothXSelection(const QRect &end);     //平滑移动阴影动画;

protected:
        virtual void paintEvent(QPaintEvent * event);
        void keyPressEvent(QKeyEvent * event);      //实现方向按键切换焦点;
        void synchroNizeXfocus(void);               //同步x方向焦点顺序;
        void synchroNizeYfocus(void);               //同步y方向焦点顺序;

private:
        QVector<QWidget *>m_ctrlVec;                //控件列表;
        QVector<QWidget *>m_pageCtrlVec;            //页面控件列表;
        QVector<KJLSubWin *>m_subWinVec;            //子窗口列表;
        QVector<int>m_xFocus;                       //x轴方向焦点顺序;
        QVector<int>m_yFocus;                       //y轴方向焦点顺序;
        int m_nXfocus;                              //当前激活的焦点序号;
        int m_nYfocus;                              //当前y轴激活的焦点序号;
        int m_nWidgetFlag;                          //窗口标志;

        QPropertyAnimation *smoothXSelection;       //平滑移动阴影动画;


signals:
	void subWinChangPage(int nPage);

public slots:
        void onChangePage(int nPage);
        void onSubWinChangePage(int nSunWin, int nPage);
        void onUpdateDisp();

};

extern QMainWindow *gMainWindow;

#endif // MAINWINDOW_H
