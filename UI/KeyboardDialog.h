#ifndef KEYBOARDDIALOG_H
#define KEYBOARDDIALOG_H

#include <QDialog>

//¼üÅÌ;

namespace Ui {
class KeyBoardDialog;
}

class KeyBoardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KeyBoardDialog(QWidget *parent = 0);
    ~KeyBoardDialog();
    QString m_strResult;

private slots:
    void on_confirmButton_clicked();

    void on_cancelButton_clicked();

    void on__1Button_clicked();

    void on__2Button_clicked();

    void on__3Button_clicked();

    void on__4Button_clicked();

    void on__5Button_clicked();

    void on__6Button_clicked();

    void on__7Button_clicked();

    void on__8Button_clicked();

    void on__9Button_clicked();

    void on__0Button_clicked();

    void on_ceButton_clicked();

    void on_pointButton_clicked();

    void on_backButton_clicked();

    void on_positiveButton_clicked();

    void on_negativeButton_clicked();

private:
    Ui::KeyBoardDialog *ui;
};

#endif // KEYBOARDDIALOG_H
