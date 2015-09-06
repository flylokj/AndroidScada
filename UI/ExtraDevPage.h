#ifndef EXTRADEVPAGE_H
#define EXTRADEVPAGE_H

#include <QWidget>
#include "Basectrl.h"

//教导页面的子页面:辅助设备


namespace Ui {
class ExtraDevPage;
}

class ExtraDevPage : public QWidget
{
    Q_OBJECT

public:
    explicit ExtraDevPage(QWidget *parent = 0);
    ~ExtraDevPage();

signals:
    void insertTutorRecord(LPTutorRecordStr pRecord, int nCnt, int nRow);

private slots:
    void onInsertTutorRecord(int nRow);

private:
    Ui::ExtraDevPage *ui;
    QStringList nameList;
};

#endif // EXTRADEVPAGE_H
