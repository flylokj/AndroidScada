#include "OtherPage.h"
#include "ui_OtherPage.h"

//教导页面的子页面:其他

OtherPage::OtherPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OtherPage)
{
    ui->setupUi(this);
    setStyleSheet( "QCheckBox::indicator:unchecked {image: url(:/res/uncheck.png);}"
                                 "QCheckBox::indicator:checked {image: url(:/res/checkbox.png);}");

    m_btnGroup.append(ui->checkBox);
}

OtherPage::~OtherPage()
{
    delete ui;
}

void OtherPage::onInsertTutorRecord(int nRow)
{
    int row = nRow;
    for(int i = 0; i < m_btnGroup.size(); i++)
    {
        QCheckBox *pBtn = m_btnGroup.at(i);
        if(pBtn->isChecked())
        {
            LPTutorRecordStr pRecord = new TutorRecordStr;
            pRecord->record.nStep = 0;
            pRecord->record.nType = OTHER_TUTOR_RECORD_TYPE;
            pRecord->record.pData = new qint16[2];
            qint16 *p = (qint16 *)pRecord->record.pData;
            p[0]=i;
            p[1]=0;//延时;

            pRecord->strList.append("0");
            pRecord->strList.append(QString("%1").arg(i+60));
            pRecord->strList.append("5");
            pRecord->strList.append("0");
            emit(insertTutorRecord(pRecord, 1, row));
            row++;
        }
    }
}
