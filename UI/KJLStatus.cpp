#include <QDebug>
#include "KJLStatus.h"
#include "ResMan/ResManager.h"
#include "ResMan/ArchiveManager.h"

KJLStatus::KJLStatus(QWidget *parent/*=0*/, char *data/*=0*/):
    QLabel(parent)
    ,m_pStatus(NULL)
    ,m_pSimulate(NULL)
{
    setData(data);

	setAttribute( Qt::WA_DeleteOnClose, true);
}


KJLStatus::~KJLStatus()
{
    //qDebug()<<"release status";
}

void KJLStatus::setData(void *data, bool byHand)
{
    if(!data)
        return;

    m_pData = (LPBaseCtrl)data;
    //私有数据;
    m_pPriData = (LPStatusPri)( (char *)m_pData + sizeof(BaseCtrl) );

    if(!byHand)
        setGeometry(m_pData->rect);

    //边框;
    if(m_pData->bPen)
        setFrameShape(Box);
    //背景;
    QPalette palette;

    if(m_pData->bBrush)
    {
        setAutoFillBackground(true);
        QColor color(m_pData->logbrush.lbColor);
        palette.setColor(QPalette::Background, color);
        setPalette(palette);
    }
    else
        setAttribute(Qt::WA_TranslucentBackground);
    //前景色;
    QColor fwColor(m_pData->forwardColor);
    palette.setColor(QPalette::Foreground, fwColor);
    setPalette(palette);

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

    m_pStatus = &m_pPriData->m_nStatCnt;

    //显示日期状态;
    if(m_pPriData->m_nType == DateStat)
    {
        connect(g_pArchiveModule, SIGNAL(updateTime(QString)), this, SLOT(onUpdateTime(QString)));
    }else//显示文本状态或图片状态;
    {
         m_pSimulate = g_pArchiveModule->simulateMap.value(m_pData->nTextID, NULL);//获取对应的变量;
         //绑定变量数据更新信号槽
         if(m_pSimulate)
         {
             connect(m_pSimulate, SIGNAL(updateData(int,SimulateMan*,QObject*)), this, SLOT(onUpdateData(int,SimulateMan*,QObject*)));
             m_pSimulate->trigle();
         }

    }
    //文字对齐方式;
    if(m_pPriData->m_nHorizantalAlign == 0)
    {
        setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    }else if(m_pPriData->m_nHorizantalAlign == 1)
    {
        setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    }else
        setAlignment(Qt::AlignCenter);

}

void KJLStatus::onUpdateTime(const QString time)
{
    setText(time);
}

void KJLStatus::onUpdateData(int data, SimulateMan *pVar, QObject *who)
{
    if(who == this)
        return;

    if(pVar == m_pSimulate)
    {
        int *p = m_pStatus;
        int nMax = *p;
        if(nMax == 0)
            return;
        int nOff = (data) % nMax;
        if(m_pPriData->m_nType == TextStat)
        {
            QString *pStr = g_pResManModule->getString(p[nOff+1]);
            if(pStr)
            {
                QStringList strList = pStr->split("_");
                setText(strList.at(0));
            }

        }else if(m_pPriData->m_nType == ImgStat)
        {
            setAttribute(Qt::WA_TranslucentBackground);
            QPixmap *pBkPixmap= g_pResManModule->getPixmap(p[nOff+1]);
            //qDebug()<<p[nOff];
            if(pBkPixmap)
                setPixmap( *pBkPixmap );
        }
    }
    else//m_pData->nDynamicID动态显示ID;
    {
        if( data == m_pData->nDynamicCondition )
            show();
        else
            hide();
    }

}
