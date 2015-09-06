#include "KeyboardDialog.h"
#include "ui_keyboardDialog.h"
#include <QDebug>

//键盘;

KeyBoardDialog::KeyBoardDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyBoardDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setAlignment(Qt::AlignRight);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowOpacity(0.9);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(0xff,0xff,0xff,0xff));
    setPalette(palette);
    //setAttribute(Qt::WA_TranslucentBackground, false);//设置透明2-窗体标题栏不透明,背景透明
}

KeyBoardDialog::~KeyBoardDialog()
{
    delete ui;
}

void KeyBoardDialog::on_confirmButton_clicked()
{
    m_strResult = ui->lineEdit->text();
    if(m_strResult.isEmpty())
        reject();
    else
        accept();
}

void KeyBoardDialog::on_cancelButton_clicked()
{
    reject();
}

void KeyBoardDialog::on__1Button_clicked()
{
//    QString strTemp = ui->lineEdit->text();
//    strTemp+="1";
//    ui->lineEdit->setText(strTemp);
    ui->lineEdit->insert("1");
}

void KeyBoardDialog::on__2Button_clicked()
{
    QString strTemp = ui->lineEdit->text();
    strTemp+="2";
    ui->lineEdit->setText(strTemp);
}

void KeyBoardDialog::on__3Button_clicked()
{
    QString strTemp = ui->lineEdit->text();
    strTemp+="3";
    ui->lineEdit->setText(strTemp);
}

void KeyBoardDialog::on__4Button_clicked()
{
    QString strTemp = ui->lineEdit->text();
    strTemp+="4";
    ui->lineEdit->setText(strTemp);
}

void KeyBoardDialog::on__5Button_clicked()
{
    QString strTemp = ui->lineEdit->text();
    strTemp+="5";
    ui->lineEdit->setText(strTemp);
}

void KeyBoardDialog::on__6Button_clicked()
{
    QString strTemp = ui->lineEdit->text();
    strTemp+="6";
    ui->lineEdit->setText(strTemp);
}

void KeyBoardDialog::on__7Button_clicked()
{
    QString strTemp = ui->lineEdit->text();
    strTemp+="7";
    ui->lineEdit->setText(strTemp);
}

void KeyBoardDialog::on__8Button_clicked()
{
    QString strTemp = ui->lineEdit->text();
    strTemp+="8";
    ui->lineEdit->setText(strTemp);
}

void KeyBoardDialog::on__9Button_clicked()
{
    QString strTemp = ui->lineEdit->text();
    strTemp+="9";
    ui->lineEdit->setText(strTemp);
}

void KeyBoardDialog::on__0Button_clicked()
{
    QString strTemp = ui->lineEdit->text();
    strTemp+="0";
    ui->lineEdit->setText(strTemp);
}

void KeyBoardDialog::on_ceButton_clicked()
{
    ui->lineEdit->clear();
}

void KeyBoardDialog::on_pointButton_clicked()
{
    QString strTemp = ui->lineEdit->text();
    if(strTemp.indexOf(QChar('.')) != -1)
        return;
    strTemp+=".";
    ui->lineEdit->setText(strTemp);
}

void KeyBoardDialog::on_backButton_clicked()
{
    ui->lineEdit->backspace();
}

void KeyBoardDialog::on_positiveButton_clicked()
{
    QString strTemp;
    QString text = ui->lineEdit->text();
    if(text.isEmpty())
        strTemp = "+";
    else if( text.at(0) == QChar('+'))
        return;
    else if(text.at(0) == QChar('-'))
    {
        text.remove(0,1);
        strTemp = "+" + text;
    }
    else
        strTemp = "+" + text;
    ui->lineEdit->setText(strTemp);
}

void KeyBoardDialog::on_negativeButton_clicked()
{
    QString strTemp;
    QString text = ui->lineEdit->text();
    if(text.isEmpty())
        strTemp = "-";
    else if( text.at(0) == QChar('-'))
        return;
    else if(text.at(0) == QChar('+'))
    {
        text.remove(0,1);
        strTemp = "-" + text;
    }
    else
        strTemp = "-" + text;
    ui->lineEdit->setText(strTemp);
}
