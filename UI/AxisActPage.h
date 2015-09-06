#ifndef AXISACTPAGE_H
#define AXISACTPAGE_H

#include <QWidget>
#include "Basectrl.h"
#include "KJLEditBox.h"

//教导页面的子页面:轴动作;


namespace Ui {
class AxisActPage;
}

class AxisActPage : public QWidget
{
    Q_OBJECT

public:
    explicit AxisActPage(QWidget *parent = 0);
    ~AxisActPage();
	void setAllSelected();

signals:
    //void insertTutorRecord(LPTutorRecordStr pRecord, int nCnt, int nRow);
	void insertTutorRecord(QVector<LPTutorRecordStr> &tutorVec, int nCnt, int nRow);

private slots:
    void on_checkBoxMY_clicked();

    void on_checkBoxMZ_clicked();

    void on_checkBoxX_clicked();

    void on_checkBoxSY_clicked();

    void on_checkBoxSZ_clicked();

    void on_checkBoxC_clicked();

    void on_pushButtonSet_clicked();

public slots:
	void onInsertTutorRecord(int nRow);

private:
    Ui::AxisActPage *ui;
    int m_nFlagArray[7];//记录被勾选的数目以及各勾选标记;
    //为了方便获取编辑框数据，定义如下容器;
    QVector<KJLEditBox *>posVec;
    QVector<KJLEditBox *>speedVec;
    QVector<KJLEditBox *>delayVec;
    QVector<QString>strVec;
	QVector<LPTutorRecordStr> m_tutorVec;


};

#endif // AXISACTPAGE_H
