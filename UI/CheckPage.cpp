#include "CheckPage.h"
#include "ui_CheckPage.h"
#include "ResMan/ResManager.h"
#include <QCheckBox>
#include <QScrollBar>
#include <QtCore/QDebug>

CheckPage::CheckPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CheckPage)
{
    double dXfactor = (double)g_pResManModule->m_pProjectParm->m_nWidth / 800;//因为在UI文件中布局时候是假设面板大小为800*600;
    ui->setupUi(this);
    ui->tableWidget->setRowCount(8);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background:transparent;width:30px; border: 2px solid transparent;}");
    ui->tableWidget->setColumnWidth(0, 27*dXfactor);
    ui->tableWidget->setColumnWidth(1, 213*dXfactor);


    //表格第一列;
    for(int i = 0; i < 8; i++)
    {
        QCheckBox *pBox = new QCheckBox();
        pBox->setStyleSheet( "QCheckBox::indicator:unchecked {image: url(:/res/uncheck.png);}"
                             "QCheckBox::indicator:checked {image: url(:/res/checkbox.png);}");
        ui->tableWidget->setCellWidget(i,0, pBox);
#ifdef MYANDROID
        ui->tableWidget->setRowHeight(i, 50);
#endif
    }

    //表格第二列;
    QStringList list;
    for(int i = 0; i < 4; i++)
    {
        list.append(tr("Fixture%1").arg(i+1));
    }
    for(int i = 0; i < 2; i++)
    {
        list.append(tr("Sucker%1").arg(i+1));
    }
    list.append("X037");
    list.append("X023");

    for(int i = 0; i < 8; i++)
    {
        QCheckBox *pBox = new QCheckBox(list.at(i));
        pBox->setStyleSheet( "QCheckBox::indicator:unchecked {image: url(:/res/signalflag0.png);}"
                             "QCheckBox::indicator:checked {image: url(:/res/signalflag1.png);}");
        ui->tableWidget->setCellWidget(i, 1, pBox);
    }

    setAttribute(Qt::WA_TranslucentBackground);
    QObjectList childList = children();
    foreach (QObject *obj, childList) {
        QWidget *w = qobject_cast<QWidget *>(obj);
        if(w)
            w->setAttribute(Qt::WA_TranslucentBackground);
    }

}

CheckPage::~CheckPage()
{
    delete ui;
    //qDebug()<<ui->tableWidget->columnWidth(1);
}

void CheckPage::onInsertTutorRecord(int nRow)
{
    int row = nRow;
    QStringList nameList;
    nameList.append(tr("Fixture1"));
    nameList.append(tr("Fixture2"));
    nameList.append(tr("Fixture3"));
    nameList.append(tr("Fixture4"));
    nameList.append(tr("Sucker1"));
    nameList.append(tr("Sucker2"));
    nameList.append("x037");
    nameList.append("x023");
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        QCheckBox *pCheck = static_cast<QCheckBox *>(ui->tableWidget->cellWidget(i,0));
        if(!pCheck->isChecked())
            continue;
        QCheckBox *pCheckZ = static_cast<QCheckBox *>(ui->tableWidget->cellWidget(i,1));
        LPTutorRecordStr pRecord = new TutorRecordStr;
        pRecord->record.nStep = 0;
        pRecord->record.nType = CHECK_TUTOR_RECORD_TYPE;
        pRecord->record.pData = new qint16[3];
        qint16 *p = (qint16 *)pRecord->record.pData;
        p[0]=i;//标志是哪个检测(0:7, 夹具1-4检测 吸1-2, x037和x023);
        p[1]=pCheckZ->isChecked();//通断标志;
        p[2]=0;//延时;
        pRecord->strList.append("0");
        pRecord->strList.append(QString("%1").arg(i+33));
        pRecord->strList.append(pCheckZ->isChecked()?"42":"43");
        pRecord->strList.append("41");
        pRecord->strList.append("5");
        pRecord->strList.append("0");
        emit(insertTutorRecord(pRecord, 1, row));
        row++;
    }
}
