#include "KJLButton.h"
#include "ResMan/ResManager.h"
#include <QPainter>
#include <QDebug>


extern ResManager *g_pResManModule;//资源管理模块;
#include <MainWindow.h>
KJLButton::KJLButton(QWidget *parent/*=0*/, char *data/*=0*/):
    QPushButton(parent)
    ,bFocus(false)
    ,bPress(false)
    ,m_pSimulate(NULL)
    ,pKJLBaseCtrl(NULL)
{
    //setFocusPolicy(Qt::ClickFocus);
    if( data )//如果私有数据有效;
    {
        m_pData = (LPBaseCtrl)data;
        m_pPriData = (LPBtnPri)( (char *)m_pData + sizeof(BaseCtrl) );
        setGeometry(m_pData->rect);
        pKJLBaseCtrl = KJLBaseCtrl::getInstance();


        if(m_pData->bDynamic)
        {
            SimulateMan *pSimulateMan = g_pArchiveModule->simulateMap.value(m_pData->nDynamicID, NULL);//获取对应的变量;
            if(!pSimulateMan)
                return;
            if(pSimulateMan)
               connect(pSimulateMan, SIGNAL(updateData(int,SimulateMan*,QObject*)), this, SLOT(onUpdateData(int,SimulateMan*,QObject*)));

            //获取动态显示对应的变量的值;
            int temp = pSimulateMan->readInt();
            if( temp == m_pData->nDynamicCondition )
                show();
            else
                hide();
        }
        else
            show();

        if(m_pPriData->m_bHoverAct)
        {
            if(parent)
            {
                MainWindow *pParent = qobject_cast<MainWindow *>(parent);
                if(pParent)
                    pParent->m_animationButton.append(this);

            }
        }
    }

	setAttribute( Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_TranslucentBackground);

}

KJLButton::~KJLButton()
{

}

//重绘;
void KJLButton::paintEvent(QPaintEvent *event)
{
    QPen pen;
    QPainter painter(this);
    painter.setBackgroundMode(Qt::TransparentMode);
    QString *pText =g_pResManModule->getString(m_pData->nTextID);
    if(m_pPriData->m_nNormalPicID!=-1)
    {
        //获取字符串的高度;
        QFont font = painter.font();
        QFontMetrics fm(font);
        int fmHeight = fm.height();

        QPixmap *pBkPixmap;
        if(isDown())
            pBkPixmap= g_pResManModule->getPixmap(m_pPriData->m_nPressPicID);
        else if(bFocus)
            pBkPixmap= g_pResManModule->getPixmap(m_pPriData->m_nHoverPicID);
        else
            pBkPixmap= g_pResManModule->getPixmap(m_pPriData->m_nNormalPicID);
        if(pBkPixmap)
        {
            painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
            if(m_pPriData->m_nBtnType == 0)//普通按钮;
            {
                painter.drawPixmap(0, 0, width(), height(), *pBkPixmap);
                if(pText)
                {
                    QStringList strList = pText->split("_");
                    painter.setPen(QColor(m_pData->forwardColor));
                    painter.drawText(0, 0, width(), height(), Qt::AlignCenter, strList.at(0));
                }
            }else//工具按钮;
            {

                painter.drawPixmap(0,0,width(), height()-fmHeight, *pBkPixmap);
                if(pText)
                {
                    QStringList strList = pText->split("_");
                    painter.setPen(QColor(m_pData->forwardColor));
                    painter.drawText(0,height()-fmHeight,width(), fmHeight, Qt::AlignCenter, strList.at(0));
                }

            }
        }

    }
    else//纯背景颜色;
    {
        QRgb temp = m_pData->logbrush.lbColor;
        QColor bkColor(temp);
        painter.fillRect(0,0,width(), height(), QBrush(bkColor));
        if(pText)
        {
            QStringList strList = pText->split("_");
            painter.drawText(0, 0, width(), height(), Qt::AlignCenter, strList.at(0));
        }

        //画阴影;
        QColor tlColor;//上边左边线条颜色;
        QColor brColor;//底部右边颜色;
        if( bPress )//被按下
        {
            brColor = bkColor.lighter();
            tlColor = bkColor.darker();
        }/*else if(bFocus)//焦点;
        {
            brColor = bkColor.lighter();
            tlColor = bkColor.lighter();
            pen.setStyle(Qt::DotLine);
        }*/
        else
        {
            tlColor= bkColor.lighter();
            brColor= bkColor.darker();
        }
		pen.setWidth(3);
		pen.setColor(tlColor);
		painter.setPen(pen);
		painter.drawLine(0, 0, width(), 0);
		painter.drawLine(0, 0, 0, height());
		pen.setColor(brColor);
		painter.setPen(pen);
		painter.drawLine(0, height(), width(), height());
		painter.drawLine(width(), 0, width(), height());
    }

}


void KJLButton::focusInEvent(QFocusEvent *event)
{
    bFocus = true;
    update();
    //qDebug()<<m_pPriData->m_focusAct.nOption<<"_"<<m_pPriData->m_focusAct.nVar<<"_"<<m_pPriData->m_focusAct.nVal;
}

void KJLButton::focusOutEvent(QFocusEvent *event)
{
    bFocus = false;
    update();
}


void KJLButton::mousePressEvent(QMouseEvent *event)
{
   // qDebug()<<"press:"<<m_pData->nPermission;
    //检查控件权限;
   //qDebug()<<m_pPriData->m_pressAct.nOption<<"_"<<m_pPriData->m_pressAct.nVar<<"_"<<m_pPriData->m_pressAct.nVal;
    qDebug()<<"press!!!!!!";
    if(m_pPriData->m_bPressAct)
    {
        if(m_pPriData->m_pressAct.nOption > RUNSCRIPT)
            return;
        //qDebug()<<pKJLBaseCtrl<<":"<<m_pPriData->m_pressAct.nOption;
        (pKJLBaseCtrl->*(pKJLBaseCtrl->m_pFundoAct[m_pPriData->m_pressAct.nOption]))(m_pPriData->m_pressAct.nVar, m_pPriData->m_pressAct.nVal);
    }

    bPress = true;
    qDebug()<<"press!!!!!!done";

    if(m_pPriData->m_bHoverAct && parentWidget())
    {
        MainWindow *pParent = qobject_cast<MainWindow *>(parentWidget());
        if(pParent)
            pParent->startSmoothXSelection(geometry());

    }
}

void KJLButton::mouseReleaseEvent(QMouseEvent *event)
{
    //qDebug()<<m_pPriData->m_releaseAct.nOption<<"_"<<m_pPriData->m_releaseAct.nVar<<"_"<<m_pPriData->m_releaseAct.nVal;
    if(m_pPriData->m_bReleaseAct)
    {
        if(m_pPriData->m_pressAct.nOption > RUNSCRIPT)
            return;
        (pKJLBaseCtrl->*(pKJLBaseCtrl->m_pFundoAct[m_pPriData->m_pressAct.nOption]))(m_pPriData->m_pressAct.nVar, m_pPriData->m_pressAct.nVal);
    }
    bPress = false;
    update();
}

void KJLButton::onUpdateData(int data, SimulateMan *pVar, QObject *who)
{
    if(who == this)
        return;

    if(pVar != m_pSimulate)
    {
        //if( data%m_pData->nDynamicCondition == 0 )
        if( data == m_pData->nDynamicCondition )
            show();
        else
            hide();
    }

}






