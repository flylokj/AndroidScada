#include "SignalPage.h"
#include "ui_SignalPage.h"
#include <QCheckBox>
#include "UI/KJLEditBox.h"
#include "ResMan/ResManager.h"

//教导页面的子页面:塑机信号


SignalPage::SignalPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignalPage)
{
    double dXfactor = (double)g_pResManModule->m_pProjectParm->m_nWidth / 800;//因为在UI文件中布局时候是假设面板大小为800*600;
    ui->setupUi(this);

    ui->tableWidget->setRowCount(4);
    ui->tableWidget->setColumnWidth(0, 27*dXfactor);
    ui->tableWidget->setColumnWidth(1, 150*dXfactor);
    ui->tableWidget->setColumnWidth(2, 95*dXfactor);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setAttribute(Qt::WA_TranslucentBackground);

    setAttribute(Qt::WA_TranslucentBackground);
    QObjectList childList = children();
    foreach (QObject *obj, childList) {
        QWidget *w = qobject_cast<QWidget *>(obj);
        if(w)
            w->setAttribute(Qt::WA_TranslucentBackground);
    }

    //表格第一列;
    for(int i = 0; i < 4; i++)
    {
        QCheckBox *pBox = new QCheckBox();
        pBox->setStyleSheet( "QCheckBox::indicator:unchecked {image: url(:/res/uncheck.png);}"
                             "QCheckBox::indicator:checked {image: url(:/res/checkbox.png);}");
        ui->tableWidget->setCellWidget(i,0, pBox);
#ifdef MYANDROID
        ui->tableWidget->setRowHeight(i, 50);
#endif
    }
    QStringList list;
    list.append(tr("CM.Permit"));
    list.append(tr("Eje.Permit"));
    list.append(tr("Core1 Permit"));
    list.append(tr("Core2 Permit"));

    nameList.append(tr("Lock Mold"));
    nameList.append(tr("Eject"));
    nameList.append(tr("Core1"));
    nameList.append(tr("Core2"));

    //表格第二列;
    for(int i = 0; i < 4; i++)
    {
        QCheckBox *pBox = new QCheckBox(list.at(i));
        pBox->setStyleSheet( "QCheckBox::indicator:unchecked {image: url(:/res/signalflag0.png);}"
                             "QCheckBox::indicator:checked {image: url(:/res/signalflag1.png);}");
        ui->tableWidget->setCellWidget(i, 1, pBox);
    }

    //表格第三列;
    for(int i = 0; i < 4; i++)
    {
        KJLEditBox *pBox = new KJLEditBox();
        pBox->setDataFmt(2, "0.00", "300.00", "0.00");
        pBox->setStyleSheet("QLineEdit{border: 0px solid lightblue;border-radius: 4px;}");
        ui->tableWidget->setCellWidget(i, 2, pBox);
    }
}

SignalPage::~SignalPage()
{
    delete ui;
}

void SignalPage::onInsertTutorRecord(int nRow)
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
        pRecord->record.nType = SIGNAL_ACT_TUTOR_RECORD_TYPE;
        pRecord->record.pData = new qint16[3];
        qint16 *p = (qint16 *)pRecord->record.pData;
        p[0]=i;//标志是哪个治具(0:3, 关模、顶针、抽芯1、抽芯2);
        p[1]=pCheckZ->isChecked();//通断标志;
        p[2]=pBox->text().remove(".").toInt();//延时;
        pRecord->strList.append("0");
        pRecord->strList.append(QString("%1").arg(i+25));
        pRecord->strList.append(pCheckZ->isChecked()?"17":"18");
        pRecord->strList.append("5");
        pRecord->strList.append(pBox->text());
        emit(insertTutorRecord(pRecord, 1, row));
        row++;
    }
}
