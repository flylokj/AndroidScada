#include "WarningPage.h"
#include "ui_WarningPage.h"
#include "ResMan/ArchiveManager.h"
#include "ResMan/ResManager.h"
#include <QScrollBar>

WarningPage::WarningPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WarningPage)
{
    ui->setupUi(this);
    const double dXfactor = g_pResManModule->getdXfactor();
    ui->tableWidget->setColumnWidth(0, 100*dXfactor);
    ui->tableWidget->setColumnWidth(1, 300*dXfactor);
    ui->tableWidget->setColumnWidth(2, 175*dXfactor);
    ui->tableWidget->setColumnWidth(3, 175*dXfactor);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QFont font = ui->tableWidget->horizontalHeader()->font();
    font.setBold(true);
    ui->tableWidget->horizontalHeader()->setFont(font);
    ui->tableWidget->setCornerButtonEnabled(false);
    ui->tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background:transparent;width:30px; border: 2px solid transparent;}");


    connect(g_pArchiveModule, SIGNAL(updateWarn(QString, bool)), this, SLOT(onUpdateWarningTable(QString, bool)) );
    g_pArchiveModule->triggerDisplayRecord();
    setAttribute(Qt::WA_TranslucentBackground);
    QObjectList childList = children();
    foreach (QObject *obj, childList) {
        QWidget *w = qobject_cast<QWidget *>(obj);
        if(w)
            w->setAttribute(Qt::WA_TranslucentBackground);
    }

}

WarningPage::~WarningPage()
{
    delete ui;
}

void WarningPage::on_pushButtonWarn_clicked()
{
    g_pArchiveModule->insertWarning(ui->lineEdit->text().toInt());
}

void WarningPage::on_pushButtonNoWarn_clicked()
{
    g_pArchiveModule->removeWarning(ui->lineEdit->text().toInt());
}

void WarningPage::onUpdateWarningTable(const QString &pStr, bool reslv)
{
    QStringList warnText = pStr.split('_');
    if(!reslv)//添加报警记录
    {
        ui->tableWidget->insertRow(0);
    }
    else//解除报警记录
    {
        if(ui->tableWidget->rowCount() <=0 )
            return;
    }
    for(int i = 1; i < warnText.size(); i++)
    {
        QTableWidgetItem *pItem = new QTableWidgetItem(warnText.at(i));
        if(ui->tableWidget->rowCount()%2==0)
        {
            //设置颜色;
            pItem->setBackgroundColor(QColor(252,151,111));
            pItem->setTextColor(QColor(255,0,0));
        }
        else
        {
            //设置颜色;
        }
        ui->tableWidget->setItem(0, i-1, pItem);
    }

}
