#ifndef ZTOOL_H
#define ZTOOL_H

#include <QWidget>
#include "Basectrl.h"

//教导页面的子页面:治具

namespace Ui {
class Ztool;
}

class Ztool : public QWidget
{
    Q_OBJECT

public:
    explicit Ztool(QWidget *parent = 0);
    ~Ztool();

    void adjustHeaderWidth();

signals:
    void insertTutorRecord(LPTutorRecordStr pRecord, int nCnt, int nRow);

private slots:
    void onInsertTutorRecord(int nRow);


private:
    Ui::Ztool *ui;
    QStringList nameList;
    int m_nFlagArray[7];//记录被勾选的数目以及各勾选标记;
};

#endif // ZTOOL_H
