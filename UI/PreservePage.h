#ifndef PRESERVEPAGE_H
#define PRESERVEPAGE_H

#include <QWidget>
#include "Basectrl.h"

//教导页面的子页面:预留

namespace Ui {
class PreservePage;
}

class PreservePage : public QWidget
{
    Q_OBJECT

public:
    explicit PreservePage(QWidget *parent = 0);
    ~PreservePage();

signals:
    void insertTutorRecord(LPTutorRecordStr pRecord, int nCnt, int nRow);

private slots:
    void onInsertTutorRecord(int nRow);

private:
    Ui::PreservePage *ui;
};

#endif // PRESERVEPAGE_H
