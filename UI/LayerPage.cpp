#include "LayerPage.h"
#include "ui_LayerPage.h"
#include "UI/Basectrl.h"
#include "UI/KJLStatic.h"
#include "UI/KJLStatus.h"

//QRect rect;
//bool bPen;
//bool bBrush;
//LOGPEN logpen;
//LOGBRUSH logbrush;
//QRgb forwardColor;
//bool bVar;
//int nTextID;
//bool bDynamic;
//int nDynamicID;
//int nDynamicCondition;
//int nPermission;//控件的权限0:监控员, 1:操作员, 2:系统管理员;

//从上至下，用于配置个数那一列;
int b[12][20]={
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        55,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        59,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        63,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        56,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        60,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        64,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        57,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        61,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        65,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        58,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        62,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        66,//变量id
        0,
        -1,
        -1,
        2,
        2
    },

};

////类型:0-2：文字，图片，日期;
//int m_nType;
//// 水平对齐方式:0-2左、右、中;
//int m_nHorizantalAlign;
////状态条目数;
//int m_nStatCnt;
//配置顺序那一列;
int a[4][26]={
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,
        51,//变量
        0,
        -1,
        -1,
        2,
        //私有属性:
        0,
        2,
        4,
        149,151,150,152//状态;
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,
        52,//变量
        0,
        -1,
        -1,
        2,
        //私有属性:
        0,
        2,
        4,
        149,151,150,152//状态;
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,
        53,//变量
        0,
        -1,
        -1,
        2,
        //私有属性:
        0,
        2,
        4,
        149,151,150,152//状态;
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,
        54,//变量
        0,
        -1,
        -1,
        2,
        //私有属性:
        0,
        2,
        4,
        149,151,150,152//状态;
    },
};

//配置显示正反方向的label;
int c[12][26]={
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,
        172,//变量
        0,
        -1,
        -1,
        2,
        //私有属性:
        0,
        1,//对齐方式
        2,
        391,392//状态;(MY:反向/MY:正向)
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,
        173,//变量
        0,
        -1,
        -1,
        2,
        //私有属性:
        0,
        1,//对齐方式
        2,
        393,394//状态;(MZ:反向/MZ:正向)
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,
        174,//变量
        0,
        -1,
        -1,
        2,
        //私有属性:
        0,
        1,//对齐方式
        2,
        395,396//状态;(X:反向/X:正向)
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,
        176,//变量
        0,
        -1,
        -1,
        2,
        //私有属性:
        0,
        2,
        2,
        391,392//状态;(MY:反向/MY:正向)
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,
        177,//变量
        0,
        -1,
        -1,
        2,
        //私有属性:
        0,
        1,//对齐方式
        2,
        393,394//状态;(MZ:反向/MZ:正向)
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,
        178,//变量
        0,
        -1,
        -1,
        2,
        //私有属性:
        0,
        1,//对齐方式
        2,
        395,396//状态;(X:反向/X:正向)
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,
        180,//变量
        0,
        -1,
        -1,
        2,
        //私有属性:
        0,
        1,//对齐方式
        2,
        391,392//状态;(MY:反向/MY:正向)
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,
        181,//变量
        0,
        -1,
        -1,
        2,
        //私有属性:
        0,
        1,//对齐方式
        2,
        393,394//状态;(MZ:反向/MZ:正向)
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,
        182,//变量
        0,
        -1,
        -1,
        2,
        //私有属性:
        0,
        1,//对齐方式
        2,
        395,396//状态;(X:反向/X:正向)
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,
        184,//变量
        0,
        -1,
        -1,
        2,
        //私有属性:
        0,
        1,//对齐方式
        2,
        391,392//状态;(MY:反向/MY:正向)
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,
        185,//变量
        0,
        -1,
        -1,
        2,
        //私有属性:
        0,
        1,//对齐方式
        2,
        393,394//状态;(MZ:反向/MZ:正向)
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,
        186,//变量
        0,
        -1,
        -1,
        2,
        //私有属性:
        0,
        1,//对齐方式
        2,
        395,396//状态;(X:反向/X:正向)
    },
};

//配置间距那一列;
int d[12][20]={
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        67,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        71,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        75,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        68,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        72,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        76,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        69,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        73,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        77,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        70,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        74,//变量id
        0,
        -1,
        -1,
        2,
        2
    },
    {
        0,0,0,0,
        0,
        0,0,0,0,
        0,0,0,
        0,
        1,//是否变量;
        78,//变量id
        0,
        -1,
        -1,
        2,
        2
    },

};



LayerPage::LayerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LayerPage)
{
    ui->setupUi(this);
    ui->label1_0->setData(b[0], true);
    ui->label1_1->setData(b[1], true);
    ui->label1_2->setData(b[2], true);
    ui->label1_3->setData(b[3], true);
    ui->label1_4->setData(b[4], true);
    ui->label1_5->setData(b[5], true);
    ui->label1_6->setData(b[6], true);
    ui->label1_7->setData(b[7], true);
    ui->label1_8->setData(b[8], true);
    ui->label1_9->setData(b[9], true);
    ui->label1_10->setData(b[10], true);
    ui->label1_11->setData(b[11], true);
    ui->label->setData(a[0], true);
    ui->label_2->setData(a[1], true);
    ui->label_3->setData(a[2], true);
    ui->label_4->setData(a[3], true);

    ui->label_5->setData(c[0], true);
    ui->label_6->setData(c[1], true);
    ui->label_7->setData(c[2], true);
    ui->label_8->setData(c[3], true);
    ui->label_9->setData(c[4], true);
    ui->label_10->setData(c[5], true);
    ui->label_11->setData(c[6], true);
    ui->label_12->setData(c[7], true);
    ui->label_13->setData(c[8], true);
    ui->label_14->setData(c[9], true);
    ui->label_15->setData(c[10], true);
    ui->label_16->setData(c[11], true);

    ui->label_17->setData(d[0], true);
    ui->label_18->setData(d[1], true);
    ui->label_19->setData(d[2], true);
    ui->label_20->setData(d[3], true);
    ui->label_21->setData(d[4], true);
    ui->label_22->setData(d[5], true);
    ui->label_23->setData(d[6], true);
    ui->label_24->setData(d[7], true);
    ui->label_25->setData(d[8], true);
    ui->label_26->setData(d[9], true);
    ui->label_27->setData(d[10], true);
    ui->label_28->setData(d[11], true);

    setStyleSheet("QRadioButton::indicator:unchecked {image: url(:/res/radiobtn0.png);}"
                  "QRadioButton::indicator:checked {image: url(:/res/radiobtn.png);}");

}

LayerPage::~LayerPage()
{
    delete ui;
}

void LayerPage::onInsertTutorRecord(int nRow)
{
    LPTutorRecordStr pRecord = new TutorRecordStr;
    pRecord->strList.append("0");
    pRecord->strList.append("14");
    pRecord->strList.append("15");
    pRecord->strList.append(tr("0"));
    pRecord->strList.append("16");
    pRecord->strList.append("5");
    pRecord->strList.append("0.00");

    pRecord->record.nStep = 0;
    pRecord->record.nType = LAYER_ACT_TUTOR_RECORD_TYPE;
    pRecord->record.pData = new qint16[2];
    qint16 *p = (qint16 *)pRecord->record.pData;
    //设置所选择的组;
    if(ui->radioButton->isChecked())
        p[0]=1;
    else if(ui->radioButton_2->isChecked())
        p[0]=2;
    else if(ui->radioButton_3->isChecked())
        p[0]=3;
    else if(ui->radioButton_4->isChecked())
        p[0]=4;
    p[1]=0;//延时;

    emit(insertTutorRecord(pRecord, 1, nRow));
}



