#ifndef LAYERPAGE_H
#define LAYERPAGE_H

#include <QWidget>
#include "Basectrl.h"

namespace Ui {
class LayerPage;
}

class LayerPage : public QWidget
{
    Q_OBJECT

public:
    explicit LayerPage(QWidget *parent = 0);
    ~LayerPage();

signals:
    void insertTutorRecord(LPTutorRecordStr pRecord, int nCnt, int nRow);

private slots:
    void onInsertTutorRecord(int nRow);


private:
    Ui::LayerPage *ui;
};

#endif // LAYERPAGE_H
