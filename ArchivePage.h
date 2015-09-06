#ifndef ARCHIVEPAGE_H
#define ARCHIVEPAGE_H

#include <QWidget>
#include <QFileInfo>
#include <QTableWidget>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QObjectUserData>
#include <QCheckBox>

namespace Ui {
class ArchivePage;
}

class Mdata: public QObjectUserData
{
public:
    Mdata();
    ~Mdata();

    int m_nId;
};



class ArchivePage : public QWidget
{
    Q_OBJECT

public:
    explicit ArchivePage(QWidget *parent = 0);
    ~ArchivePage();

    static void copyMoldData(const QString &src, const QString &dest, const QString &ext);

private slots:
    void onTableWidgetItemClicked();
    void on_pushButtonNew_clicked();
    void on_pushButtonCopy_clicked();
    void on_pushButtonLoad_clicked();
    void on_pushButtonDel_clicked();
    void on_pushButtonExport_clicked();
    void on_pushButtonImport_clicked();
    void on_pushButtonSelAll_clicked();
    void on_pushButtonSelnone_clicked();
    void on_pushButtonreselect_clicked();
    void on_checkBoxExport_clicked();
    void on_checkBoxImport_clicked();

private:
    Ui::ArchivePage *ui;
    QCheckBox *m_currentCheck;
    void listAllFile();//列出所有模号文件;
    QString getFileCreatedTime(QFileInfo &info);//获取文件创建日期时间字符串;
    void addToTableWidget(const QString &fileName, const QString &time);//添加一条目到表格尾部;
};

#endif // ARCHIVEPAGE_H
