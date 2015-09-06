#include "WaitPage.h"
#include "ui_WaitPage.h"
#include <QDebug>

//教导页面的子页面:等待

WaitPage::WaitPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaitPage)
{
    ui->setupUi(this);
    setStyleSheet( "QRadioButton::indicator:unchecked {image: url(:/res/radiobtn0.png);}"
                                 "QRadioButton::indicator:checked {image: url(:/res/radiobtn.png);}");
    m_pBtnGroup = new QButtonGroup(this);
    m_pBtnGroup->addButton(ui->radioButton, 0);
    m_pBtnGroup->addButton(ui->radioButton_2, 1);
    m_pBtnGroup->addButton(ui->radioButton_3, 2);
    m_pBtnGroup->addButton(ui->radioButton_4, 3);
    m_pBtnGroup->addButton(ui->radioButton_5, 4);
    m_pBtnGroup->addButton(ui->radioButton_6, 5);
    m_pBtnGroup->addButton(ui->radioButton_7, 6);
    m_pBtnGroup->addButton(ui->radioButton_15, 7);
    m_pBtnGroup->addButton(ui->radioButton_8, 8);
    m_pBtnGroup->addButton(ui->radioButton_9, 9);
    m_pBtnGroup->addButton(ui->radioButton_10, 10);
    m_pBtnGroup->addButton(ui->radioButton_11, 11);
    m_pBtnGroup->addButton(ui->radioButton_12, 12);
    m_pBtnGroup->addButton(ui->radioButton_13, 13);
    m_pBtnGroup->addButton(ui->radioButton_14, 14);


}

WaitPage::~WaitPage()
{
    delete ui;
    delete m_pBtnGroup;
}

void WaitPage::onInsertTutorRecord(int nRow)
{
    QRadioButton *pBtn = static_cast<QRadioButton *>( m_pBtnGroup->checkedButton() );
    if(!pBtn)
        return;

    LPTutorRecordStr pRecord = new TutorRecordStr;
    pRecord->record.nStep = 0;
    pRecord->record.nType = WAIT_TUTOR_RECORD_TYPE;
    pRecord->record.pData = new qint16[2];
    qint16 *p = (qint16 *)pRecord->record.pData;
    p[0]=m_pBtnGroup->checkedId();
    p[1]=0;//延时;

    pRecord->strList.append("0");
    pRecord->strList.append(QString("%1").arg(p[0]+44));
    pRecord->strList.append("59");
    pRecord->strList.append("0");
    emit(insertTutorRecord(pRecord, 1, nRow));
}
