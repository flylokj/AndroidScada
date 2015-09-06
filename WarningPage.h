#ifndef WARNINGPAGE_H
#define WARNINGPAGE_H

#include <QWidget>

namespace Ui {
class WarningPage;
}

class WarningPage : public QWidget
{
    Q_OBJECT

public:
    explicit WarningPage(QWidget *parent = 0);
    ~WarningPage();

private slots:
    void on_pushButtonWarn_clicked();
    void on_pushButtonNoWarn_clicked();
    void onUpdateWarningTable(const QString &pStr, bool reslv);

private:
    Ui::WarningPage *ui;
};

#endif // WARNINGPAGE_H
