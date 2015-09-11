#include "ExtraDevPage.h"
#include "ui_ExtraDevPage.h"
#include <QCheckBox>
#include "UI/KJLEditBox.h"
#include "ResMan/ResManager.h"

//教导页面的子页面:辅助设备

ExtraDevPage::ExtraDevPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExtraDevPage)
{
    const double dXfactor = g_pResManModule->getdXfactor();
    ui->setupUi(this);

    ui->tableWidget->setRowCount(2);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setColumnWidth(0, 27*dXfactor);
    ui->tableWidget->setColumnWidth(1, 85*dXfactor);
    ui->tableWidget->setColumnWidth(2, 68*dXfactor);
    ui->tableWidget->setColumnWidth(3, 75*dXfactor);

    setAttribute(Qt::WA_TranslucentBackground);
    QObjectList childList = children();
    foreach (QObject *obj, childList) {
        QWidget *w = qobject_cast<QWidget *>(obj);
        if(w)
            w->setAttribute(Qt::WA_TranslucentBackground);
    }

    //表格第一列;
    for(int i = 0; i < 2; i++)
    {
        QCheckBox *pBox = new QCheckBox();
        pBox->setStyleSheet( "QCheckBox::indicator:unchecked {image: url(:/res/uncheck.png);}"
                             "QCheckBox::indicator:checked {image: url(:/res/checkbox.png);}");
        ui->tableWidget->setCellWidget(i,0, pBox);
#ifdef MYANDROID
        ui->tableWidget->setRowHeight(i, 50);
#endif
    }


    nameList.append(tr("Injector"));
    nameList.append(tr("Conveyor"));
    //表格第二列;
    for(int i = 0; i < 2; i++)
    {
        QCheckBox *pBox = new QCheckBox(nameList.at(i));
        pBox->setStyleSheet( "QCheckBox::indicator:unchecked {image: url(:/res/signalflag0.png);}"
                             "QCheckBox::indicator:checked {image: url(:/res/signalflag1.png);}");
        ui->tableWidget->setCellWidget(i, 1, pBox);
    }

    //表格第三列;
    for(int i = 0; i < 2; i++)
    {
        KJLEditBox *pBox = new KJLEditBox();
        pBox->setDataFmt(2, "0.00", "300.00", "0.00");
        pBox->setStyleSheet("QLineEdit{border: 0px solid lightblue;border-radius: 4px;}");
        ui->tableWidget->setCellWidget(i, 2, pBox);
    }

    //表格第四列;
    for(int i = 0; i < 2; i++)
    {
        KJLEditBox *pBox = new KJLEditBox();
        pBox->setDataFmt(0, "0", "20000", "0");
        pBox->setStyleSheet("QLineEdit{border: 0px solid lightblue;border-radius: 4px;}");
        ui->tableWidget->setCellWidget(i, 3, pBox);
    }
}

ExtraDevPage::~ExtraDevPage()
{
//        qDebug()<<ui->tableWidget->columnWidth(0);
//        qDebug()<<ui->tableWidget->columnWidth(1);
//        qDebug()<<ui->tableWidget->columnWidth(2);
//        qDebug()<<ui->tableWidget->columnWidth(3);
    delete ui;
}

void ExtraDevPage::onInsertTutorRecord(int nRow)
{
    int row = nRow;
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        QCheckBox *pCheck = static_cast<QCheckBox *>(ui->tableWidget->cellWidget(i,0));
        if(!pCheck->isChecked())
            continue;
        QCheckBox *pCheckZ = static_cast<QCheckBox *>(ui->tableWidget->cellWidget(i,1));
        KJLEditBox *pTimeBox = static_cast<KJLEditBox *>(ui->tableWidget->cellWidget(i,2));//时间;
        KJLEditBox *pIntervalBox = static_cast<KJLEditBox *>(ui->tableWidget->cellWidget(i,3));//间隔模数;
        LPTutorRecordStr pRecord = new TutorRecordStr;
        pRecord->record.nStep = 0;
        pRecord->record.nType = EXTRA_DEV_TUTOR_RECORD_TYPE;
        pRecord->record.pData = new qint16[4];
        qint16 *p = (qint16 *)pRecord->record.pData;
        p[0]=i;//标志是哪个辅助设备(0:1, 喷油、输送带);
        p[1]=pCheckZ->isChecked();//通断标志;
        p[2]=pTimeBox->text().remove(".").toInt();//延时;
        p[3]=pIntervalBox->text().toInt();//间隔模数;
        pRecord->strList.append("0");
        pRecord->strList.append(QString("%1").arg(i+31));
        pRecord->strList.append(pCheckZ->isChecked()?"17":"18");
        pRecord->strList.append("29");
        pRecord->strList.append(pIntervalBox->text()+" ");
        pRecord->strList.append("30");
        pRecord->strList.append(pTimeBox->text());
        emit(insertTutorRecord(pRecord, 1, row));
        row++;
    }
}
