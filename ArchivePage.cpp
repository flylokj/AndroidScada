#include "ArchivePage.h"
#include "ui_ArchivePage.h"
#include <QMessageBox>
#include <QCheckBox>
#include <UI/Basectrl.h>
#include "ResMan/ArchiveManager.h"
#include "ResMan/ResManager.h"


Mdata::Mdata():QObjectUserData(),m_nId(0)
{

}

Mdata::~Mdata()
{

}



ArchivePage::ArchivePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArchivePage),
    m_currentCheck(NULL)
{
    ui->setupUi(this);
    double dXfactor = (double)g_pResManModule->m_pProjectParm->m_nWidth / 800;//因为在UI文件中布局时候是假设面板大小为800*600;
    ui->tableWidget->setColumnWidth(0,500*dXfactor);
    ui->tableWidget->setColumnWidth(1,165*dXfactor);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget->verticalHeader()->setDefaultSectionSize(50);

    ui->checkBoxExport->setStyleSheet("QRadioButton::indicator:unchecked {image: url(:/res/uncheck.png);}""QRadioButton::indicator:checked {image: url(:/res/checkbox.png);}");
    ui->checkBoxImport->setStyleSheet("QRadioButton::indicator:unchecked {image: url(:/res/uncheck.png);}""QRadioButton::indicator:checked {image: url(:/res/checkbox.png);}");
    setAttribute(Qt::WA_TranslucentBackground);
    QObjectList childList = children();
    foreach (QObject *obj, childList) {
        QWidget *w = qobject_cast<QWidget *>(obj);
        if(w)
            w->setAttribute(Qt::WA_TranslucentBackground);
    }

    listAllFile();
}

ArchivePage::~ArchivePage()
{
    delete ui;
}

void ArchivePage::copyMoldData(const QString &src, const QString &dest, const QString &ext)
{
    QString fileSrc = src;
    fileSrc.append(ext);
    fileSrc.prepend(MOLDDIR);
    QString fileNew = dest;
    fileNew.append(ext);
    fileNew.prepend(MOLDDIR);
    QFile::copy(fileSrc, fileNew);
    QFile f(fileNew);
    if(!f.open(QIODevice::ReadWrite))
        QMessageBox::warning(0,"fuck","fuck");
    f.close();

    qDebug()<<"copy:"<<fileSrc<<" to "<<fileNew;
}

void ArchivePage::onTableWidgetItemClicked()
{
    QCheckBox *pCheck = (QCheckBox *)sender();
    int nCurSel = ((Mdata *)pCheck->userData(0))->m_nId;
    ui->tableWidget->selectRow(nCurSel);
    ui->labelSrc->setText(pCheck->text());
}

//新建文件;
void ArchivePage::on_pushButtonNew_clicked()
{
    //检查新建文件名称是否存在;
    QString newFileName = ui->lineEditName->text();
    if(newFileName.isEmpty())
    {
        QMessageBox::warning(this, tr("Warning"), tr("fileName empty"));
        return;
    }

    //新建文件;
    newFileName.prepend(QString(MOLDDIR));
    newFileName.append(".bin");
    QFile file(newFileName);
    if(file.exists())
        return;
    if(!file.open(QIODevice::ReadWrite))
        QMessageBox::warning(0,"fuck","fuck");
    file.resize(MOLDSIZE);
    file.close();

    //获取文件信息并显示;
    QFileInfo fileInfo(file);
    QString strTemp = getFileCreatedTime(fileInfo);
    addToTableWidget(ui->lineEditName->text(), strTemp);


}

//列出所有文件;
void ArchivePage::listAllFile()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->clearContents();

    QDir dir(MOLDDIR);
    if(!dir.exists())
        return;

    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();

    int file_count = list.count();
    if(file_count <= 0)
    {
        return;
    }

    QStringList string_list;
    for(int i=0; i<file_count; i++)
    {
        QFileInfo file_info = list.at(i);
        QString suffix = file_info.suffix();
        if(QString::compare(suffix, QString("bin"), Qt::CaseInsensitive) == 0)
        {
            QString strTemp = file_info.baseName();
            string_list.append(strTemp);
            strTemp = getFileCreatedTime(file_info);
            string_list.append(strTemp);
        }
    }

    for(int i = 0; i < string_list.size(); i=i+2)
        addToTableWidget(string_list.at(i), string_list.at(i+1));
}

QString ArchivePage::getFileCreatedTime(QFileInfo &info)
{
    QDateTime time = info.created();
    QString strTemp;
    strTemp.sprintf("%d/%02d/%02d %02d:%02d:%02d", time.date().year(), time.date().month(), time.date().day(), time.time().hour(), time.time().minute(), time.time().second());
    return strTemp;
}

void ArchivePage::addToTableWidget(const QString &fileName, const QString &time)
{
    int cnt = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(cnt);
    QCheckBox *pCheck = new QCheckBox(fileName);
    pCheck->setStyleSheet("QCheckBox::indicator:unchecked {image: url(:/res/uncheck.png);}""QCheckBox::indicator:checked {image: url(:/res/checkbox.png);}");
    ui->tableWidget->setCellWidget(cnt, 0, pCheck);
    Mdata *pData;
    pData = new Mdata();
    pData->m_nId = cnt;
    pCheck->setUserData(0, pData);
    ui->tableWidget->setItem(cnt, 1, new QTableWidgetItem(time) );
    QObject::connect(pCheck, SIGNAL(clicked()), this, SLOT(onTableWidgetItemClicked()) );
}



//复制文件;
void ArchivePage::on_pushButtonCopy_clicked()
{
    //检查当前被选中行的有效性
    int nCur = ui->tableWidget->currentRow();
    if(nCur == -1)
        return;

    //检查文件名是否为空
    if(ui->lineEditName->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Warning"), tr("fileName empty"));
        return;
    }

    QCheckBox *pCheck = (QCheckBox *)ui->tableWidget->cellWidget(nCur, 0);
    copyMoldData(pCheck->text(), ui->lineEditName->text(), ".bin");
    listAllFile();

}

//载入模号;
void ArchivePage::on_pushButtonLoad_clicked()
{
    int nCur = ui->tableWidget->currentRow();
    if(nCur == -1)
        return;
    QCheckBox *pCheck = (QCheckBox *)ui->tableWidget->cellWidget(nCur, 0);
    QString moldName = pCheck->text();
    g_pArchiveModule->changeMold(moldName);
}

void ArchivePage::on_pushButtonDel_clicked()
{
    QMessageBox::StandardButton rb = QMessageBox::question(this, tr("Tips"), tr("Are you sure to delete"));
    if(rb == QMessageBox::No)
        return;

    int cnt = ui->tableWidget->rowCount();
    for(int i = 0; i < cnt; i++)
    {
        QCheckBox *pCheck = (QCheckBox *)ui->tableWidget->cellWidget(i, 0);
        if(pCheck->isChecked())
        {
            QString fileName = pCheck->text();
            fileName.append(".bin");
            fileName.prepend(MOLDDIR);

            QFile::remove(fileName);

        }
    }
    listAllFile();
}

void ArchivePage::on_pushButtonExport_clicked()
{

}

void ArchivePage::on_pushButtonImport_clicked()
{

}

//全选;
void ArchivePage::on_pushButtonSelAll_clicked()
{
    int cnt = ui->tableWidget->rowCount();
    for(int i = 0; i < cnt; i++)
    {
        QCheckBox *pCheck =(QCheckBox *)ui->tableWidget->cellWidget(i, 0);
        pCheck->setChecked(true);
    }
}

//全部选;
void ArchivePage::on_pushButtonSelnone_clicked()
{
    int cnt = ui->tableWidget->rowCount();
    for(int i = 0; i < cnt; i++)
    {
        QCheckBox *pCheck =(QCheckBox *)ui->tableWidget->cellWidget(i, 0);
        pCheck->setChecked(false);
    }
}

//反选;
void ArchivePage::on_pushButtonreselect_clicked()
{
    int cnt = ui->tableWidget->rowCount();
    for(int i = 0; i < cnt; i++)
    {
        QCheckBox *pCheck =(QCheckBox *)ui->tableWidget->cellWidget(i, 0);
        pCheck->setChecked(!pCheck->isChecked());
    }
}

//导出勾选;
void ArchivePage::on_checkBoxExport_clicked()
{

}

//导入勾选;
void ArchivePage::on_checkBoxImport_clicked()
{

}
