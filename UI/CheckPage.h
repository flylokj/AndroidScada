#ifndef CHECKPAGE_H
#define CHECKPAGE_H

#include <QWidget>
#include "Basectrl.h"

namespace Ui {
class CheckPage;
}

class CheckPage : public QWidget
{
    Q_OBJECT

public:
    explicit CheckPage(QWidget *parent = 0);
    ~CheckPage();

signals:
    void insertTutorRecord(LPTutorRecordStr pRecord, int nCnt, int nRow);

private slots:
    void onInsertTutorRecord(int nRow);

private:
    Ui::CheckPage *ui;
};

#endif // CHECKPAGE_H
