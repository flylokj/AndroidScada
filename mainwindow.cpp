#include <QPainter>
#include <QLabel>
#include "MainWindow.h"
#include "ResMan/ResManager.h"
#include "UI/KJLButton.h"
#include "UI/KJLEditBox.h"
#include "UI/KJLComboBox.h"
#include "UI/KJLGroupBox.h"
#include "UI/KJLStatic.h"
#include "UI/KJLStatus.h"
#include "UI/KJLProgress.h"
#include "ArchivePage.h"
#include "WarningPage.h"
#include "TutorPage.h"

#include <QGraphicsView>

#include <QDebug>

#define POSY 60


extern  ResManager *g_pResManModule;//资源管理模块;


MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
	m_nXfocus(0),
	m_nYfocus(0),
	m_nWidgetFlag(0)
{
    gMainWindow = this;

    //焦点特效;
    QPushButton *btnAnimation =  new QPushButton(this);
    btnAnimation->setGeometry(550,650,100,100);
    btnAnimation->setStyleSheet("background-color: rgba(85, 170, 255, 35);");
    btnAnimation->show();
	//0:设置窗口大小;
	//1:获取当前的页面指针;
	//2:获取当前的控件焦点列表;
	//3:识别所有的控件类型和数据;
	//[0]
    resize(g_pResManModule->m_pProjectParm->m_nWidth, g_pResManModule->m_pProjectParm->m_nHeight);

	//[1]
    LPPageHead pPageHead = g_pResManModule->m_pageStruct.pageVector.at(0);//获取当前的页面指针;
	int id = pPageHead->nTitleID;
    QString *pStrTitle = g_pResManModule->getString(id);//标题;
	if(pStrTitle)
		setWindowTitle(*pStrTitle);

	//[2]
	int *pTemp = (int *)((char *)pPageHead + sizeof(PageHead));
	int *pTempNum = pTemp + 1;//最大焦点数;
	int maxFocus = *pTempNum;
	for(int i = 0 ; i < maxFocus; i++)
	{
		pTempNum++;
		m_xFocus.append(*(pTempNum));
		//qDebug()<<"x:"<<*pTempNum;
	}
	pTemp = (int *)((char *)pTemp + *pTemp);
	pTempNum = pTemp + 1;
	maxFocus = *pTempNum;
	for(int i = 0 ; i < maxFocus; i++)
	{
		pTempNum++;
		m_yFocus.append(*(pTempNum));
		//qDebug()<<"y:"<<*pTempNum;
	}

	//[3]
	LPCtrlHead pCtrlHead = (LPCtrlHead)((char *)pTemp + *pTemp);
    g_pArchiveModule->updateSysPage(0);
	for(int i = 0; i < pPageHead->nCtrlNum; i++)
	{
		char *pData = ((char *)pCtrlHead + sizeof(CtrlHead));
		switch (pCtrlHead->ctrlType)
		{
		case KJLBtnType:
			{
				KJLButton *pControl = new KJLButton(this,pData);
				m_ctrlVec.append(pControl);//加入控件列表;
				break;
			}
		case KJLEditBoxType:
			{
				KJLEditBox *pControl = new KJLEditBox(this,pData);
				m_ctrlVec.append(pControl);//加入控件列表;
				break;
			}
		case KJLComboBoxType:
			{
				KJLComboBox *pControl = new KJLComboBox(this,pData);
				m_ctrlVec.append(pControl);//加入控件列表;
				break;
			}
		case KJLGroupBoxType:
			{
				KJLGroupBox *pControl = new KJLGroupBox(this,pData);
				m_ctrlVec.append(pControl);//加入控件列表;
				break;
			}
		case KJLStaticType:
			{
                KJLStatic *pControl = new KJLStatic(this,pData);
//                QLabel *pControl = new QLabel(this);
//                pControl->setText("fuck");
				m_ctrlVec.append(pControl);//加入控件列表;
				break;
			}
		case KJLStatusType:
			{
				KJLStatus *pControl = new KJLStatus(this,pData);
				m_ctrlVec.append(pControl);//加入控件列表;
				break;
			}
		case KJLProgressType:
			{
				KJLProgress *pControl = new KJLProgress(this,pData);
				m_ctrlVec.append(pControl);//加入控件列表;
				break;
			}
		case KJLSubWinType:
			{
				KJLSubWin *pControl = new KJLSubWin(this,pData);
				m_ctrlVec.append(pControl);//加入控件列表;
				break;
			}
		default:
			break;
		}
		pCtrlHead = (LPCtrlHead)((char *)pCtrlHead + pCtrlHead->ctrlSize );
	}

	KJLBaseCtrl *pBase = KJLBaseCtrl::getInstance();
	connect(pBase, SIGNAL(changePage(int)), this, SLOT(onChangePage(int)) );
	connect(pBase, SIGNAL(subWinChangePage(int,int)), this, SLOT(onSubWinChangePage(int,int)) );
    connect(g_pResManModule, SIGNAL(updateDisp()), this, SLOT(onUpdateDisp()) );


    //平滑选择效果;
    smoothXSelection = new QPropertyAnimation(btnAnimation, "geometry");
    if(m_animationButton.size()>0)
        startSmoothXSelection(m_animationButton.at(0)->geometry());


    //翻转效果;


}

MainWindow::~MainWindow()
{
    qDebug()<<"~MainWindow()";
}

void MainWindow::startSmoothXSelection(const QRect &end)
{
    smoothXSelection->setEndValue(end.adjusted(-2,-2,2,2));
    smoothXSelection->start();
}

//!背景重绘
/*!
 * 根据背景模式
 * 0: 纯色背景
 * 1: 渐变背景
 * 2: 图片背景
 */
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    int nBkMode = g_pResManModule->m_pPanelParm->BkMode;
    if(nBkMode == 0)//纯色背景;
    {
        painter.fillRect(0,0,width(), height() ,QBrush(QColor(g_pResManModule->m_pPanelParm->bkColor)));
    }
    else if(nBkMode == 1)//渐变背景;
    {

    }
    else//图片背景;
    {
        QPixmap *pPixmap = g_pResManModule->getPanelBkPixmap();//获取面板背景图片;
        if(pPixmap)
            painter.drawPixmap(0,0,width(),height(),*pPixmap );
    }


}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
	int nFocus = 0;
	if(event->key() == Qt::Key_W)
	{
		m_nYfocus>0 ? m_nYfocus-- : m_nYfocus = m_yFocus.size()-1;
		nFocus = m_yFocus.at(m_nYfocus);
		synchroNizeXfocus();
		//qDebug()<<"fuck up";
	}
	else if(event->key() == Qt::Key_S)
	{
		m_nYfocus = (++m_nYfocus) % m_yFocus.size();
		nFocus = m_yFocus.at(m_nYfocus);
		synchroNizeXfocus();
		//qDebug()<<"fuck down";
	}
	else if(event->key() == Qt::Key_A)
	{
		m_nXfocus>0 ? m_nXfocus-- : m_nXfocus = m_xFocus.size()-1;
		nFocus = m_xFocus.at(m_nXfocus);
		synchroNizeYfocus();
		//qDebug()<<"fuck left";
	}
	else if(event->key() == Qt::Key_D)
	{
		m_nXfocus = (++m_nXfocus) % m_xFocus.size();
		nFocus = m_xFocus.at(m_nXfocus);
		synchroNizeYfocus();
		//qDebug()<<"fuck right";
	}

	m_ctrlVec.at(nFocus)->setFocus();

}

//同步x方向焦点顺序;
void MainWindow::synchroNizeXfocus()
{
	int i = 0;
	for(QVector<int>::ConstIterator it = m_xFocus.begin(); it != m_xFocus.end(); it++)
	{
		if( *it == m_yFocus.at(m_nYfocus))
		{
			m_nXfocus = i;
			return;
		}
		i++;
	}
	m_nXfocus = 0;
}

//同步y方向焦点顺序;
void MainWindow::synchroNizeYfocus()
{
	int i = 0;
	for(QVector<int>::ConstIterator it = m_yFocus.begin(); it != m_yFocus.end(); it++)
	{
		if( *it == m_xFocus.at(m_nXfocus) )
		{
			m_nYfocus = i;
			return;
		}
		i++;
	}
	m_nYfocus = 0;
}

//切换页面;
void MainWindow::onChangePage(int nPage)
{
	//[0]清空当前页面;
	m_subWinVec.clear();//子窗口列表清除;
	for(QVector<QWidget *>::const_iterator it = m_pageCtrlVec.begin(); it != m_pageCtrlVec.end(); it++)
	{
		if(m_nWidgetFlag)
			(*it)->close();
		else
		{
			(*it)->close();//delete (*it);
		}

		m_nWidgetFlag = 0;
	}
	m_pageCtrlVec.clear();

	/****************系统页面**********************/
	if(nPage>100)
	{
        const double dXfactor = g_pResManModule->getdXfactor();
        const double dYfactor = g_pResManModule->getdYfactor();
		int nYPos = POSY*dYfactor;
		qDebug()<<"sys page:"<<nPage;
        g_pArchiveModule->updateSysPage(nPage);
		m_nWidgetFlag = 1;
		if(nPage == 101)//档案管理;
		{
			ArchivePage *pPage = new ArchivePage(this);
			pPage->setAttribute( Qt::WA_DeleteOnClose, true);
			pPage->move(0,nYPos);
			pPage->resize(pPage->size().width()*dXfactor, pPage->size().height()*dYfactor);
			pPage->show();
			m_pageCtrlVec.append(pPage);
		}
		else if(nPage == 102)//报警页面;
		{
			WarningPage *pPage = new WarningPage(this);
			pPage->setAttribute( Qt::WA_DeleteOnClose, true);
			pPage->move(0,nYPos);
			pPage->resize(pPage->size().width()*dXfactor, pPage->size().height()*dYfactor);
			pPage->show();
			m_pageCtrlVec.append(pPage);
		}
		else if(nPage == 103 )//教导页面;
		{
			TutorPage *pPage = new TutorPage(this);
			pPage->setAttribute( Qt::WA_DeleteOnClose, true);
			pPage->move(0,nYPos);
			pPage->resize(pPage->size().width()*dXfactor, pPage->size().height()*dYfactor);
			pPage->show();
			m_pageCtrlVec.append(pPage);
		}
		return;
	}
	/*********************************************/


	/****************普通用户页面**********************/
	//[1]获取当前页面指针;
    LPPageHead pPageHead = g_pResManModule->m_pageStruct.pageVector.at(nPage);//获取当前的页面指针;
	int id = pPageHead->nTitleID;
    QString *pStrTitle = g_pResManModule->getString(id);//标题;
	if(pStrTitle)
		setWindowTitle(*pStrTitle);
    g_pArchiveModule->updateSysPage(nPage);


	//[3]跳过焦点区域;
	int *pTemp = (int *)((char *)pPageHead + sizeof(PageHead));
	int *pTempNum = pTemp + 1;//最大焦点数;
	//    int maxFocus = *pTempNum;
	//    for(int i = 0 ; i < maxFocus; i++)
	//    {
	//        pTempNum++;
	//        m_xFocus.append(*(pTempNum));
	//        //qDebug()<<"x:"<<*pTempNum;
	//    }
	pTemp = (int *)((char *)pTemp + *pTemp);
	pTempNum = pTemp + 1;
	//    maxFocus = *pTempNum;
	//    for(int i = 0 ; i < maxFocus; i++)
	//    {
	//        pTempNum++;
	//        m_yFocus.append(*(pTempNum));
	//        //qDebug()<<"y:"<<*pTempNum;
	//    }

	//[4]生成控件;
	LPCtrlHead pCtrlHead = (LPCtrlHead)((char *)pTemp + *pTemp);
	for(int i = 0; i < pPageHead->nCtrlNum; i++)
	{
		char *pData = ((char *)pCtrlHead + sizeof(CtrlHead));
		switch (pCtrlHead->ctrlType)
		{
		case KJLBtnType:
			{
				KJLButton *pControl = new KJLButton(this,pData);
				m_pageCtrlVec.append(pControl);//加入控件列表;
				break;
			}
		case KJLEditBoxType:
			{
				KJLEditBox *pControl = new KJLEditBox(this,pData);
				m_pageCtrlVec.append(pControl);//加入控件列表;
				break;
			}
		case KJLComboBoxType:
			{
				KJLComboBox *pControl = new KJLComboBox(this,pData);
				m_pageCtrlVec.append(pControl);//加入控件列表;
				break;
			}
		case KJLGroupBoxType:
			{
				KJLGroupBox *pControl = new KJLGroupBox(this,pData);
				m_pageCtrlVec.append(pControl);//加入控件列表;
				break;
			}
		case KJLStaticType:
			{
				KJLStatic *pControl = new KJLStatic(this,pData);
				m_pageCtrlVec.append(pControl);//加入控件列表;
				break;
			}
		case KJLStatusType:
			{
				KJLStatus *pControl = new KJLStatus(this,pData);
				m_pageCtrlVec.append(pControl);//加入控件列表;
				break;
			}
		case KJLProgressType:
			{
				KJLProgress *pControl = new KJLProgress(this,pData);
				m_pageCtrlVec.append(pControl);//加入控件列表;
				break;
			}
		case KJLSubWinType:
			{
				KJLSubWin *pControl = new KJLSubWin(this,pData);
				m_pageCtrlVec.append(pControl);//加入控件列表;
				m_subWinVec.append(pControl);
				break;
			}
		default:
			break;
		}
		pCtrlHead = (LPCtrlHead)((char *)pCtrlHead + pCtrlHead->ctrlSize );
	}

    g_pArchiveModule->m_pCommunication->printMonitor();
    g_pArchiveModule->m_pCommunication->updateMonitorToHost();



    //特效旋转;

}

void MainWindow::onSubWinChangePage(int nSunWin, int nPage)
{
	qDebug()<<"MainWindow::onSubWinChangePage"<<nSunWin<<":"<<nPage<<"subWin cnt:"<<m_subWinVec.size();
	if(nSunWin >= m_subWinVec.size())
		return;

	m_subWinVec.at(nSunWin)->onChangePage(nPage);

    g_pArchiveModule->m_pCommunication->printMonitor();
    g_pArchiveModule->m_pCommunication->updateMonitorToHost();
}

void MainWindow::onUpdateDisp()
{
    int nPage = g_pArchiveModule->getSysPage();
	onChangePage(nPage);
	update();
}



