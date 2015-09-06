#ifndef TUTORPAGE_H
#define TUTORPAGE_H

#include <QWidget>
#include <QVector>
#include <QAbstractTableModel>
#include "UI/Basectrl.h"


/* @教导页面;
 * @说明:本模块实现教导页面的控制逻辑以及定义教导页面的底层数据结构。;
 * @数据结构:教导条目是教导画面中列表框显示的一行内容,对应的数据结构为一个教导记录结构体TutorRecord;
 *      线性表是教导记录的容器，存放所有的教导记录，这里使用的线性表形式为QT内置的QVecotr容器类，方便操作;
 * @控制逻辑:对于操作线性表有一套方法,分别是初始化、插入、删除等操作，这套方法对下操作线性表数据，对上承接Model的操作，也就是MVC模型中Model对数据的操作;
*/
class TutorModel;
typedef QString (TutorModel::*FmtTutorRecord)(LPTutorRecordStr pRecord);

class TutorModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TutorModel(QObject *parent = 0);
    ~TutorModel();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRow(int row, const QModelIndex & parent = QModelIndex());
    bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
    bool removeRow(int row, const QModelIndex & parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());

    void deleteTutorRecord(int nRow);//从线性表中删除由nRow指定的教导条目,5 < nRow < 最大的教导条目ID; deleteTutorRecord;
    void comboTutorRecord(int nRow);//组合nRow定位的教导条目，内部将调用updateRecordStep更新教导条目步序;
    void splitTutorRecord(int nRow);//分解nRow定位的教导条目，内部将调用updateRecordStep更新教导条目步序;
    LPTutorRecordStr getRawTutorRecord(int nRow);//获取nRow定位的教导条目的原始数据;
	

public slots:
    void onInsertTutorRecord(LPTutorRecordStr pRecord, int cnt, int nRow);//在线性表的第nRow行，插入cnt条教导记录;
	void onInsertTutorRecord(QVector<LPTutorRecordStr> &, int cnt, int nRow);//在线性表的第nRow行，插入cnt条教导记录;


private:
//data:
    QVector<LPTutorRecordStr> &m_tutorVec;//教导记录的集合;
    FmtTutorRecord m_fmtRecord[10];
	QStringList m_strListAxist;//轴动作记录用到的字符串，需要翻译;
//methods:
    void initTutorRecordVec();//初始化教导条目线性表:待机点位置[5个轴和治具]和模组结束，共7条教导记录;
    //插入教导条目到线性表,public slots:onInsertTutorRecord

    //获取线性表的大小:rowCount函数已经实现;
    
    void replaceTurtorRecord(int nRow, LPTutorRecordStr pRecord);//替换教导条目数据，编辑时候用到;
    void serializeTutorRecordVec();//序列化线性表到文件;
	void updateRecordStep(int nRow, int add);//更新教导条目步序号;
    //从教导条目数据中格式化成字符串,用于tableView的显示,data函数中用到;
    QString fmtAxisRecord(LPTutorRecordStr pRecord);//格式化轴动作数据为显示字符串;
    QString fmtLayerRecord(LPTutorRecordStr pRecord);//格式化堆叠动作数据为显示字符串;
    QString fmtZToolRecord(LPTutorRecordStr pRecord);//格式化治具数据为显示字符串;
    QString fmtSignalRecord(LPTutorRecordStr pRecord);//格式化塑机信号数据为显示字符串;
    QString fmtExtraDevRecord(LPTutorRecordStr pRecord);//格式化辅助设备数据为显示字符串;
    QString fmtPreserveRecord(LPTutorRecordStr pRecord);//格式化预留数据为显示字符串;
    QString fmtCheckRecord(LPTutorRecordStr pRecord);//格式化检测数据为显示字符串;
    QString fmtWaitRecord(LPTutorRecordStr pRecord);//格式化等待数据为显示字符串;
    QString fmtOtherRecord(LPTutorRecordStr pRecord);//格式化其他数据为显示字符串;
	QString fmtEndRecord(LPTutorRecordStr pRecord);//格式化模组结束为显示字符串;
};










namespace Ui {
class TutorPage;
}

class TutorPage : public QWidget
{
    Q_OBJECT

public:
    explicit TutorPage(QWidget *parent = 0);
    ~TutorPage();

signals:
    void insertTutorRecord(int nRow);//插入教导条目;

private slots:
    void on_pushButtonInsert_clicked();//插入;
    void on_pushButtonEdit_clicked();//编辑;
    void on_pushButtonDel_clicked();//删除;
    void on_pushButtonMenu_clicked();//菜单;
    void onPushBtnAxisActionClicked();//轴动作;
    void onPushBtnLayerClicked();//堆叠;
    void onPushBtnZToolClicked();//治具;
    void onPushBtnSignalClicked();//塑机信号;
    void onPushBtnExtraDeviceClicked();//辅助设备;
    void onPushBtnPreserveClicked();//预留;
    void onPushBtnCheckClicked();//检测;
    void onPushBtnWaitClicked();//等待;
    void onPushBtnOtherClicked();//其他;

    void on_pushButtonSplit_clicked();//分解;
    void on_pushButtonCombo_clicked();//组合;
	void on_pushButtonReset_clicked();//重新设置;

    void on_pushButtonQuickSet_clicked();

private:
//DATA:
    Ui::TutorPage *ui;
    int m_nTutorMode;

//METHOD:
    void clearSubPage();//清除子画面;
};









#endif // TUTORPAGE_H
