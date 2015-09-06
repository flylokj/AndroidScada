#include "TutorEditDlg.h"
#include "ui_TutorEditDlg.h"


#define BOXMAXWIDTH     150
#define BOXMINHEIGHT    30

TutorEditDlg::TutorEditDlg(LPTutorRecordStr pRecord, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TutorEditDlg),
    m_pRecord(pRecord),
    m_pSetButton(NULL),
    m_pPosBox(NULL),
    m_pSpeedBox(NULL),
    m_pDelayBox(NULL),
    m_pDeclineSpeedBox(NULL),
    m_pDeclinePosBox(NULL),
    m_pCheckEndPos(NULL),
    m_pCheckDeclineSpeed(NULL),
    m_pLayoutSel(NULL),
    m_pMoldInvervalBox(NULL)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowOpacity(0.9);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(Qt::lightGray).lighter(130));
    setPalette(palette);
    setStyleSheet("QCheckBox::indicator:unchecked {image: url(:/res/uncheck.png);}"
                  "QCheckBox::indicator:checked {image: url(:/res/checkbox.png);}"
                  "QLineEdit{border: 1px solid lightblue;border-radius: 4px;}"
                  "QLineEdit:disabled{border: 1px solid lightblue;border-radius: 4px; background: transparent lightgray;}");
    m_pLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    m_pLayout->setMargin(30);
    m_pSetButton = new QPushButton(tr("Set=>"));
    m_pSetButton->setMaximumWidth(BOXMAXWIDTH/2);
    m_pSetButton->setMinimumHeight(BOXMINHEIGHT);
    m_pLayout->addWidget(m_pSetButton);

    ui->buttonBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->buttonBox->setCenterButtons(true);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setMinimumHeight(BOXMINHEIGHT);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setMinimumHeight(BOXMINHEIGHT);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("OK"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
    ui->buttonBox->setContentsMargins(20,20,20,20);

    //根据不同的教导类型进行相应教导编辑画面布局;
    int nType = m_pRecord->record.nType;
    switch (nType)
    {
        case AXIS_ACT_TUTOR_RECORD_TYPE:
            setupAxisEidtLayout();
            break;
        case LAYER_ACT_TUTOR_RECORD_TYPE:
            setupLayerEditLayout();
            break;
        case ZTOOL_ACT_TUTOR_RECORD_TYPE:
            setupZToolEditLayout();
            break;
        case SIGNAL_ACT_TUTOR_RECORD_TYPE:
            setupSignalEditLayout();
            break;
        case EXTRA_DEV_TUTOR_RECORD_TYPE:
            setupExtraEditLayout();
            break;
        case PRESERVE_TUTOR_RECORD_TYPE:
            setupPreserveEditLayout();
            break;
        case CHECK_TUTOR_RECORD_TYPE:
            setupCheckEditLayout();
            break;
        case WAIT_TUTOR_RECORD_TYPE:
            setupWaitEditLayout();
            break;
        case OTHER_TUTOR_RECORD_TYPE:
		case MOLD_END_TUTOR_RECORD_TYPE:
            setupOtherEditLayout();
            break;
        default:
            break;
    }
}

TutorEditDlg::~TutorEditDlg()
{
    delete ui;
//    if(m_pSetButton)
//        delete m_pSetButton;
//    if(m_pPosBox)
//        delete m_pPosBox;
//    if(m_pSpeedBox)
//        delete m_pSpeedBox;
//    if(m_pDelayBox)
//        delete m_pDelayBox;
//    if(m_pDeclineBox)
//        delete m_pDeclineBox;
//    if(m_pDeclinePosBox)
//        delete m_pDeclinePosBox;
//    if(m_pCheckEndPos)
//        delete m_pCheckEndPos;
//    if(m_pCheckDeclineSpeed)
//        delete m_pCheckDeclineSpeed;
//    if(m_pLayoutSel)
//        delete m_pLayoutSel;
//    if(m_pMoldInvervalBox)
//        delete m_pMoldInvervalBox;
}

//list.replace(2, QString::number(p[1]/100.0, 'f', 2)+" ");
//list.replace(4, QString::number(p[2])+" ");
//list.replace(7, QString::number(p[5])+" ");
//list.replace(9, QString::number(p[6])+" ");
//list.replace(11, QString::number(p[3]/100.0, 'f', 2));

//轴动作编辑布局;
void TutorEditDlg::setupAxisEidtLayout()
{
    qint16 *p = reinterpret_cast<qint16 *>(m_pRecord->record.pData);
    if(p[0] < 5)//5个轴
    {
        //位置;
        m_pPosBox = new KJLEditBox();
        m_pPosBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        m_pPosBox->setMaximumWidth(BOXMAXWIDTH);
        m_pPosBox->setMinimumHeight(BOXMINHEIGHT);
        m_pPosBox->setDataFmt(2, "0.00", "$57", QString::number(p[1]/100.0, 'f', 2));//这里需要根据轴来进行设定;


        QLabel *pLabel = new QLabel(tr("Pos"));
        pLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        QLabel *pLabelUint = new QLabel("mm");
        pLabelUint->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        pLabelUint->setMinimumWidth(20);

        QBoxLayout *pHLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        pHLayout->addWidget(pLabel);
        pHLayout->addWidget(m_pPosBox);
        pHLayout->addWidget(pLabelUint);
        m_pLayout->addLayout(pHLayout);

        //速度;
        m_pSpeedBox = new KJLEditBox();
        m_pSpeedBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        m_pSpeedBox->setMaximumWidth(BOXMAXWIDTH);
        m_pSpeedBox->setMinimumHeight(BOXMINHEIGHT);
        m_pSpeedBox->setDataFmt(0, "0", "100", QString::number(p[2]));

        pLabel = new QLabel(tr("Speed"));
        pLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        pLabelUint = new QLabel(tr("%"));
        pLabelUint->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        pLabelUint->setMinimumWidth(20);

        pHLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        pHLayout->addWidget(pLabel);
        pHLayout->addWidget(m_pSpeedBox);
        pHLayout->addWidget(pLabelUint);
        m_pLayout->addLayout(pHLayout);

        //延迟时间;
        m_pDelayBox = new KJLEditBox();
        m_pDelayBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        m_pDelayBox->setMaximumWidth(BOXMAXWIDTH);
        m_pDelayBox->setMinimumHeight(BOXMINHEIGHT);
        m_pDelayBox->setDataFmt(2, "0.00", "300.00", QString::number(p[3]/100.0, 'f', 2));

        pLabel = new QLabel(tr("Delay Time"));
        pLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        pLabelUint = new QLabel(tr("s"));
        pLabelUint->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        pLabelUint->setMinimumWidth(20);

        pHLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        pHLayout->addWidget(pLabel);
        pHLayout->addWidget(m_pDelayBox);
        pHLayout->addWidget(pLabelUint);
        m_pLayout->addLayout(pHLayout);

        //提前结束位置check;
        m_pCheckEndPos = new QCheckBox(tr("Early End"));
        m_pCheckEndPos->setChecked(p[4]&0xff00);
        m_pLayout->addWidget(m_pCheckEndPos);

        //提前减速;
        m_pDeclineSpeedBox = new KJLEditBox();
        m_pDeclineSpeedBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        m_pDeclineSpeedBox->setMaximumWidth(BOXMAXWIDTH);
        m_pDeclineSpeedBox->setMinimumHeight(BOXMINHEIGHT);
        m_pDeclineSpeedBox->setDataFmt(0, "0", "30", QString::number(p[5]));

        m_pCheckDeclineSpeed = new QCheckBox(tr("Early Speed-Down"));
        m_pCheckDeclineSpeed->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        m_pCheckDeclineSpeed->setChecked(p[4]&0x00ff);
        pLabelUint = new QLabel(tr("%"));
        pLabelUint->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        pLabelUint->setMinimumWidth(20);

        pHLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        pHLayout->addWidget(m_pCheckDeclineSpeed);
        pHLayout->addWidget(m_pDeclineSpeedBox);
        pHLayout->addWidget(pLabelUint);
        m_pLayout->addLayout(pHLayout);

        //提前位置;
        m_pDeclinePosBox = new KJLEditBox();
        m_pDeclinePosBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        m_pDeclinePosBox->setMaximumWidth(BOXMAXWIDTH);
        m_pDeclinePosBox->setMinimumHeight(BOXMINHEIGHT);
        m_pDeclinePosBox->setDataFmt(2, "0.00", "$57", QString::number(p[6]/100.0, 'f', 2));

        pLabel = new QLabel(tr("End Pos"));
        pLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        pLabelUint = new QLabel(tr("mm"));
        pLabelUint->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        pLabelUint->setMinimumWidth(20);

        pHLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        pHLayout->addWidget(pLabel);
        pHLayout->addWidget(m_pDeclinePosBox);
        pHLayout->addWidget(pLabelUint);
        m_pLayout->addLayout(pHLayout);

        m_pLayout->addWidget(ui->buttonBox);

        QObject::connect(m_pCheckEndPos, SIGNAL(clicked()), this, SLOT(onCheckEndPosClicked()) );
        QObject::connect(m_pCheckDeclineSpeed, SIGNAL(clicked()), this, SLOT(onCheckDeclineSpeedClicked()) );

        onCheckDeclineSpeedClicked();
        onCheckEndPosClicked();
    }
    else//治具
    {
        //水平与垂直选择;
        QBoxLayout *pHLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        m_pHorizantalCheck = new QRadioButton(tr("Hor"));
        m_pVerticalCheck = new QRadioButton(tr("Ver"));
        pHLayout->addWidget(m_pVerticalCheck);
        pHLayout->addWidget(m_pHorizantalCheck);
        p[1]>0?m_pHorizantalCheck->setChecked(true):m_pVerticalCheck->setChecked(true);
        m_pLayout->addLayout(pHLayout);

        //延迟时间;
        m_pDelayBox = new KJLEditBox();
        m_pDelayBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        m_pDelayBox->setMaximumWidth(BOXMAXWIDTH);
        m_pDelayBox->setMinimumHeight(BOXMINHEIGHT);
        m_pDelayBox->setDataFmt(2, "0.00", "300.00", QString::number(p[2]/100.0, 'f', 2));

        QLabel *pLabel = new QLabel(tr("Delay Time"));
        pLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        QLabel *pLabelUint = new QLabel(tr("s"));
        pLabelUint->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        pLabelUint->setMinimumWidth(20);

        pHLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        pHLayout->addWidget(pLabel);
        pHLayout->addWidget(m_pDelayBox);
        pHLayout->addWidget(pLabelUint);
        m_pLayout->addLayout(pHLayout);

        ui->buttonBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        ui->buttonBox->setCenterButtons(true);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setMinimumHeight(BOXMINHEIGHT);
        ui->buttonBox->button(QDialogButtonBox::Cancel)->setMinimumHeight(BOXMINHEIGHT);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("OK"));
        ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
        ui->buttonBox->setContentsMargins(20,20,20,20);
        m_pLayout->addWidget(ui->buttonBox);
    }

}

//堆叠编辑布局;
void TutorEditDlg::setupLayerEditLayout()
{
    qint16 *p = reinterpret_cast<qint16 *>(m_pRecord->record.pData);
    //选择;
    m_pLayoutSel = new KJLEditBox();
    m_pLayoutSel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pLayoutSel->setMaximumWidth(BOXMAXWIDTH);
    m_pLayoutSel->setMinimumHeight(BOXMINHEIGHT);
    m_pLayoutSel->setDataFmt(0, "0", "4", QString::number(p[0]));

    QLabel *pLabel = new QLabel(tr("Select"));
    pLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QLabel *pLabelUint = new QLabel();
    pLabelUint->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pLabelUint->setMinimumWidth(20);

    QBoxLayout *pHLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    pHLayout->addWidget(pLabel);
    pHLayout->addWidget(m_pLayoutSel);
    pHLayout->addWidget(pLabelUint);
    m_pLayout->addLayout(pHLayout);


    //延迟时间;
    m_pDelayBox = new KJLEditBox();
    m_pDelayBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pDelayBox->setMaximumWidth(BOXMAXWIDTH);
    m_pDelayBox->setMinimumHeight(BOXMINHEIGHT);
    m_pDelayBox->setDataFmt(2, "0.00", "300.00", QString::number(p[1]/100.0, 'f', 2));

    pLabel = new QLabel(tr("Delay Time"));
    pLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    pLabelUint = new QLabel(tr("s"));
    pLabelUint->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pLabelUint->setMinimumWidth(20);

    pHLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    pHLayout->addWidget(pLabel);
    pHLayout->addWidget(m_pDelayBox);
    pHLayout->addWidget(pLabelUint);
    m_pLayout->addLayout(pHLayout);

    m_pLayout->addWidget(ui->buttonBox);
}

void TutorEditDlg::setupZToolEditLayout()
{
    setupSampleEditLayout();
}

void TutorEditDlg::setupSampleEditLayout(bool bMoldInterval, int delay_offset, int interval_off)
{
    qint16 *p = reinterpret_cast<qint16 *>(m_pRecord->record.pData);
    QLabel *pLabel, *pLabelUint;
    QBoxLayout *pHLayout;
    //间隔模数;
    if(bMoldInterval)
    {
        m_pMoldInvervalBox = new KJLEditBox();
        m_pMoldInvervalBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        m_pMoldInvervalBox->setMaximumWidth(BOXMAXWIDTH);
        m_pMoldInvervalBox->setMinimumHeight(BOXMINHEIGHT);
        m_pMoldInvervalBox->setDataFmt(0, "0", "20000", QString::number(p[interval_off]));

        pLabel = new QLabel(tr("Times"));
        pLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        pLabelUint = new QLabel();
        pLabelUint->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        pLabelUint->setMinimumWidth(20);

        pHLayout = new QBoxLayout(QBoxLayout::LeftToRight);
        pHLayout->addWidget(pLabel);
        pHLayout->addWidget(m_pMoldInvervalBox);
        pHLayout->addWidget(pLabelUint);
        m_pLayout->addLayout(pHLayout);

        m_pLayout->addWidget(ui->buttonBox);
    }


    //延迟时间;
    m_pDelayBox = new KJLEditBox();
    m_pDelayBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pDelayBox->setMaximumWidth(BOXMAXWIDTH);
    m_pDelayBox->setMinimumHeight(BOXMINHEIGHT);
    m_pDelayBox->setDataFmt(2, "0.00", "300.00", QString::number(p[delay_offset]/100.0, 'f', 2));

    pLabel = new QLabel(tr("Delay Time"));
    pLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    pLabelUint = new QLabel(tr("s"));
    pLabelUint->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pLabelUint->setMinimumWidth(20);

    pHLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    pHLayout->addWidget(pLabel);
    pHLayout->addWidget(m_pDelayBox);
    pHLayout->addWidget(pLabelUint);
    m_pLayout->addLayout(pHLayout);

    m_pLayout->addWidget(ui->buttonBox);
}

void TutorEditDlg::setupSignalEditLayout()
{
    setupSampleEditLayout();
}

void TutorEditDlg::setupExtraEditLayout()
{
    setupSampleEditLayout(true);
}

void TutorEditDlg::setupPreserveEditLayout()
{
    setupSampleEditLayout(true);
}

void TutorEditDlg::setupCheckEditLayout()
{
    setupSampleEditLayout();
}

void TutorEditDlg::setupWaitEditLayout()
{
    setupSampleEditLayout(false, 1);
}

void TutorEditDlg::setupOtherEditLayout()
{
    setupSampleEditLayout(false, 1);
}

void TutorEditDlg::onCheckDeclineSpeedClicked()
{
    if(m_pCheckDeclineSpeed->isChecked())
    {
        m_pDeclinePosBox->setDisabled(false);
        m_pDeclineSpeedBox->setDisabled(false);
    }
    else
    {
        if(!m_pCheckEndPos->isChecked())
            m_pDeclinePosBox->setDisabled(true);
        m_pDeclineSpeedBox->setDisabled(true);
    }
}

void TutorEditDlg::onCheckEndPosClicked()
{
    if(m_pCheckEndPos->isChecked())
    {
        m_pDeclinePosBox->setDisabled(false);
    }
    else
    {
        if(!m_pCheckDeclineSpeed->isChecked())
            m_pDeclinePosBox->setDisabled(true);
    }
}

//确认操作;
//p[1]位置;2字节
//p[2]速度;2字节
//p[3]延时;2字节
//p[4]提前结束位置使能|提前减速使能 1字节|1字节
//p[5]提前减速;2字节
//p[6]提前位置;2字节
void TutorEditDlg::on_buttonBox_accepted()
{
    int nType = m_pRecord->record.nType;
    qint16 *p = reinterpret_cast<qint16 *>(m_pRecord->record.pData);
    switch (nType)
    {
        case AXIS_ACT_TUTOR_RECORD_TYPE:
        {

           if(p[0] < 5)//轴
           {
               p[1] = m_pPosBox->text().remove(".").toInt();
               p[2] = m_pSpeedBox->text().toInt();
               p[3] = m_pDelayBox->text().remove(".").toInt();
               p[4] = m_pCheckEndPos->isChecked()<<8 | m_pCheckDeclineSpeed->isChecked();
               p[5] = m_pDeclineSpeedBox->text().toInt();
               p[6] = m_pDeclinePosBox->text().remove(".").toInt();
           }
           else//治具
           {
               p[1] = m_pVerticalCheck->isChecked() ? 0 : 1;
               p[2] = m_pDelayBox->text().remove(".").toInt();
           }
        }
        break;
        case LAYER_ACT_TUTOR_RECORD_TYPE:
        {
            p[0] = m_pLayoutSel->text().toInt();
            p[1] = m_pDelayBox->text().remove(".").toInt();
        }
        break;
        case ZTOOL_ACT_TUTOR_RECORD_TYPE:
        case SIGNAL_ACT_TUTOR_RECORD_TYPE:
        case EXTRA_DEV_TUTOR_RECORD_TYPE:
        case PRESERVE_TUTOR_RECORD_TYPE:
        case CHECK_TUTOR_RECORD_TYPE:
        {
            if(m_pMoldInvervalBox)
                p[3] = m_pMoldInvervalBox->text().toInt();
            p[2] = m_pDelayBox->text().remove(".").toInt();
        }
        break;
        case WAIT_TUTOR_RECORD_TYPE:
        case OTHER_TUTOR_RECORD_TYPE:
		case MOLD_END_TUTOR_RECORD_TYPE:
        {
            p[1] = m_pDelayBox->text().remove(".").toInt();
        }
        break;

    default:
        break;
    }

}
