#include "ZtoolPage.h"
#include "ui_Ztool.h"
#include <QDebug>
#include <QCheckBox>
#include "UI/KJLEditBox.h"
#include "ResMan/ResManager.h"
//教导页面的子页面:治具

Ztool::Ztool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ztool)
{

    ui->setupUi(this);
    ui->tableWidget->setRowCount(6);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //表格第一列;
    for(int i = 0; i < 6; i++)
    {
        QCheckBox *pBox = new QCheckBox();
        pBox->setStyleSheet( "QCheckBox::indicator:unchecked {image: url(:/res/uncheck.png);}"
                             "QCheckBox::indicator:checked {image: url(:/res/checkbox.png);}");
        ui->tableWidget->setCellWidget(i,0, pBox);
#ifdef MYANDROID
    ui->tableWidget->setRowHeight(i, 50);
#endif
    }

    nameList.append(tr("Fixture1"));
    nameList.append(tr("Fixture2"));
    nameList.append(tr("Fixture3"));
    nameList.append(tr("Fixture4"));
    nameList.append(tr("Sucker1"));
    nameList.append(tr("Sucker2"));
    //表格第二列;
    for(int i = 0; i < 6; i++)
    {
        QCheckBox *pBox = new QCheckBox(nameList.at(i));
        pBox->setStyleSheet( "QCheckBox::indicator:unchecked {image: url(:/res/signalflag0.png);}"
                             "QCheckBox::indicator:checked {image: url(:/res/signalflag1.png);}");
        ui->tableWidget->setCellWidget(i, 1, pBox);
    }

    //表格第三列;
    for(int i = 0; i < 6; i++)
    {
        KJLEditBox *pBox = new KJLEditBox();
        pBox->setDataFmt(2, "0.00", "300.00", "0.00");
        pBox->setStyleSheet("QLineEdit{border: 0px solid lightblue;border-radius: 4px;}");
        ui->tableWidget->setCellWidget(i, 2, pBox);
    }

    memset(m_nFlagArray, 0, sizeof(m_nFlagArray));
    m_nFlagArray[0]=6;

    setAttribute(Qt::WA_TranslucentBackground);
    QObjectList childList = children();
    foreach (QObject *obj, childList) {
        QWidget *w = qobject_cast<QWidget *>(obj);
        if(w)
            w->setAttribute(Qt::WA_TranslucentBackground);
    }
}

Ztool::~Ztool()
{
    delete ui;
//    qDebug()<<ui->tableWidget->columnWidth(0);
//    qDebug()<<ui->tableWidget->columnWidth(1);
//    qDebug()<<ui->tableWidget->columnWidth(2);
}

void Ztool::adjustHeaderWidth()
{
    int nWidth = width();
    const double dXfactor = g_pResManModule->getdXfactor();
    ui->tableWidget->setColumnWidth(0, 27*dXfactor);
    ui->tableWidget->setColumnWidth(1, 150*dXfactor);
    ui->tableWidget->setColumnWidth(2, 95*dXfactor);


}

void Ztool::onInsertTutorRecord(int nRow)
{
    int row = nRow;
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        QCheckBox *pCheck = static_cast<QCheckBox *>(ui->tableWidget->cellWidget(i,0));
        if(!pCheck->isChecked())
            continue;
        QCheckBox *pCheckZ = static_cast<QCheckBox *>(ui->tableWidget->cellWidget(i,1));
        KJLEditBox *pBox = static_cast<KJLEditBox *>(ui->tableWidget->cellWidget(i,2));
        LPTutorRecordStr pRecord = new TutorRecordStr;
        pRecord->record.nStep = 0;
        pRecord->record.nType = ZTOOL_ACT_TUTOR_RECORD_TYPE;
        pRecord->record.pData = new qint16[3];
        qint16 *p = (qint16 *)pRecord->record.pData;
        p[0]=i;//标志是哪个治具(0:4, 夹具1，2，3，4 吸1，2);
        p[1]=pCheckZ->isChecked();//通断标志;
        p[2]=pBox->text().remove(".").toInt();//延时;
        pRecord->strList.append("0");
        pRecord->strList.append(QString("%1").arg(19+i));
        pRecord->strList.append(pCheckZ->isChecked()?"17":"18");
        pRecord->strList.append("5");
        pRecord->strList.append(pBox->text());
        emit(insertTutorRecord(pRecord, 1, row));
        row++;
    }
}




