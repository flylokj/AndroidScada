#include "AxisActPage.h"
#include "ui_AxisActPage.h"
#include "UI/KJLEditBox.h"

//教导页面的子页面:轴动作


AxisActPage::AxisActPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AxisActPage)
{
    ui->setupUi(this);
    setStyleSheet("QRadioButton::indicator:unchecked {image: url(:/res/radiobtn0.png);}"
                  "QRadioButton::indicator:checked {image: url(:/res/radiobtn.png);}"
                  "QRadioButton::indicator:disabled {image: url(:/res/radiobtn0.png);}"
                  "QCheckBox::indicator:unchecked {image: url(:/res/uncheck.png);}"
                  "QCheckBox::indicator:checked {image: url(:/res/checkbox.png);}"
                  "QLineEdit{border: 1px solid lightblue;border-radius: 4px;}"
                  "QLineEdit:disabled{border: 1px solid lightblue;border-radius: 4px; background: transparent lightgray;}");


    //各个轴的最大位置，需要读取某些地址;
    ui->lineEditPosMY->setDataFmt(2, "0.00", "$84", "0.00");
    ui->lineEditPosMZ->setDataFmt(2, "0.00", "$89", "0.00");
    ui->lineEditPosX->setDataFmt(2, "0.00", "$94", "0.00");
    ui->lineEditPosSY->setDataFmt(2, "0.00", "$99", "0.00");
    ui->lineEditPosSZ->setDataFmt(2, "0.00", "$104", "0.00");

    //速度最小最大值
    ui->lineEditSpeedMY->setDataFmt(0, "0", "100", "80");
    ui->lineEditSpeedMZ->setDataFmt(0, "0", "100", "80");
    ui->lineEditSpeedX->setDataFmt(0, "0", "100", "80");
    ui->lineEditSpeedSY->setDataFmt(0, "0", "100", "80");
    ui->lineEditSpeedSZ->setDataFmt(0, "0", "100", "80");

    //延时最小最大值
    ui->lineEditDelayMY->setDataFmt(2, "0.00", "300.00", "0.00");
    ui->lineEditDelayMZ->setDataFmt(2, "0.00", "300.00", "0.00");
    ui->lineEditDelayX->setDataFmt(2, "0.00", "300.00", "0.00");
    ui->lineEditDelaySY->setDataFmt(2, "0.00", "300.00", "0.00");
    ui->lineEditDelaySZ->setDataFmt(2, "0.00", "300.00", "0.00");
    ui->lineEditDelayC->setDataFmt(2, "0.00", "300.00", "0.00");

    memset(m_nFlagArray, 0, sizeof(m_nFlagArray));
    m_nFlagArray[0]=6;
    on_checkBoxMY_clicked();
    on_checkBoxMZ_clicked();
    on_checkBoxX_clicked();
    on_checkBoxSY_clicked();
    on_checkBoxSZ_clicked();
    on_checkBoxC_clicked();

    //为了方便操作，将编辑框都加入容器进行定位;
    posVec.append(ui->lineEditPosMY);
    posVec.append(ui->lineEditPosMZ);
    posVec.append(ui->lineEditPosX);
    posVec.append(ui->lineEditPosSY);
    posVec.append(ui->lineEditPosSZ);

    speedVec.append(ui->lineEditSpeedMY);
    speedVec.append(ui->lineEditSpeedMZ);
    speedVec.append(ui->lineEditSpeedX);
    speedVec.append(ui->lineEditSpeedSY);
    speedVec.append(ui->lineEditSpeedSZ);

    delayVec.append(ui->lineEditDelayMY);
    delayVec.append(ui->lineEditDelayMZ);
    delayVec.append(ui->lineEditDelayX);
    delayVec.append(ui->lineEditDelaySY);
    delayVec.append(ui->lineEditDelaySZ);

    strVec.append(tr("MY:"));
    strVec.append(tr("MZ:"));
    strVec.append(tr("X:"));
    strVec.append(tr("SY:"));
    strVec.append(tr("SZ:"));

    setAttribute(Qt::WA_TranslucentBackground);
    QObjectList childList = children();
    foreach (QObject *obj, childList) {
        QWidget *w = qobject_cast<QWidget *>(obj);
        if(w)
            w->setAttribute(Qt::WA_TranslucentBackground);
    }

}

AxisActPage::~AxisActPage()
{
    delete ui;
}



//以下的CheckBox都是起到使能与禁止作用;
void AxisActPage::on_checkBoxMY_clicked()
{
    ui->lineEditPosMY->setDisabled(!ui->checkBoxMY->isChecked());
    ui->lineEditSpeedMY->setDisabled(!ui->checkBoxMY->isChecked());
    ui->lineEditDelayMY->setDisabled(!ui->checkBoxMY->isChecked());
    if(ui->checkBoxMY->isChecked())
    {
        m_nFlagArray[0]++;
        m_nFlagArray[1]=1;
    }
    else
    {
        m_nFlagArray[0]--;
        m_nFlagArray[1]=0;
    }

}

void AxisActPage::on_checkBoxMZ_clicked()
{
    ui->lineEditPosMZ->setDisabled(!ui->checkBoxMZ->isChecked());
    ui->lineEditSpeedMZ->setDisabled(!ui->checkBoxMZ->isChecked());
    ui->lineEditDelayMZ->setDisabled(!ui->checkBoxMZ->isChecked());
    if(ui->checkBoxMZ->isChecked())
    {
        m_nFlagArray[0]++;
        m_nFlagArray[2]=1;
    }
    else
    {
        m_nFlagArray[0]--;
        m_nFlagArray[2]=0;
    }
}

void AxisActPage::on_checkBoxX_clicked()
{
    ui->lineEditPosX->setDisabled(!ui->checkBoxX->isChecked());
    ui->lineEditSpeedX->setDisabled(!ui->checkBoxX->isChecked());
    ui->lineEditDelayX->setDisabled(!ui->checkBoxX->isChecked());
    if(ui->checkBoxX->isChecked())
    {
        m_nFlagArray[0]++;
        m_nFlagArray[3]=1;
    }
    else
    {
        m_nFlagArray[0]--;
        m_nFlagArray[3]=0;
    }
}

void AxisActPage::on_checkBoxSY_clicked()
{
    ui->lineEditPosSY->setDisabled(!ui->checkBoxSY->isChecked());
    ui->lineEditSpeedSY->setDisabled(!ui->checkBoxSY->isChecked());
    ui->lineEditDelaySY->setDisabled(!ui->checkBoxSY->isChecked());
    if(ui->checkBoxSY->isChecked())
    {
        m_nFlagArray[0]++;
        m_nFlagArray[4]=1;
    }
    else
    {
        m_nFlagArray[0]--;
        m_nFlagArray[4]=0;
    }
}

void AxisActPage::on_checkBoxSZ_clicked()
{
    ui->lineEditPosSZ->setDisabled(!ui->checkBoxSZ->isChecked());
    ui->lineEditSpeedSZ->setDisabled(!ui->checkBoxSZ->isChecked());
    ui->lineEditDelaySZ->setDisabled(!ui->checkBoxSZ->isChecked());
    if(ui->checkBoxSZ->isChecked())
    {
        m_nFlagArray[0]++;
        m_nFlagArray[5]=1;
    }
    else
    {
        m_nFlagArray[0]--;
        m_nFlagArray[5]=0;
    }
}

void AxisActPage::on_checkBoxC_clicked()
{
    ui->radioButtonH->setDisabled(!ui->checkBoxC->isChecked());
    ui->radioButtonV->setDisabled(!ui->checkBoxC->isChecked());
    ui->lineEditDelayC->setDisabled(!ui->checkBoxC->isChecked());
    if(ui->checkBoxC->isChecked())
    {
        m_nFlagArray[0]++;
        m_nFlagArray[6]=1;
    }
    else
    {
        m_nFlagArray[0]--;
        m_nFlagArray[6]=0;
    }
}

//设入
void AxisActPage::on_pushButtonSet_clicked()
{
    qDebug()<<"设入";
}

void AxisActPage::onInsertTutorRecord(int nRow)
{
    if(m_nFlagArray[0] == 0)
        return;
    //LPTutorRecordStr pRecord = new TutorRecordStr[m_nFlagArray[0]];
	m_tutorVec.clear();
    int i = 0;
    for(int j = 0; j < 5; j++)
    {
        if(m_nFlagArray[j+1] == 0)
            continue;
		LPTutorRecordStr pRecord = new TutorRecordStr;
        pRecord->strList.append("0");//0
        pRecord->strList.append(QString("%1").arg((j+9)));//1
        pRecord->strList.append(posVec[j]->text()+" ");//2
        pRecord->strList.append("1");//3
        pRecord->strList.append(speedVec[j]->text()+" ");//4
        pRecord->strList.append("2");//5
        pRecord->strList.append("3");//6
        pRecord->strList.append("0 ");//7
        pRecord->strList.append("4");//8
        pRecord->strList.append("0 ");//9
        pRecord->strList.append("5");//10
        pRecord->strList.append(delayVec[j]->text());//11

        //填充结教导记录数据;
        pRecord->record.nType = AXIS_ACT_TUTOR_RECORD_TYPE;
        pRecord->record.pData = new qint16[7];//14字节;
        qint16 *p = (qint16 *)pRecord->record.pData;
        p[0] = j;//轴(0-5: MY, MZ, X, SY, SZ);
        p[1] = posVec[j]->text().remove(".").toInt();//位置;2字节
        p[2] = speedVec[j]->text().remove(".").toInt();//速度;2字节
        p[3] = delayVec[j]->text().remove(".").toInt();//延时;2字节
        p[4] = 0;//提前结束位置使能|提前减速使能 1字节|1字节;
        p[5] = 0;//提前减速;2字节
        p[6] = 0;//提前位置;2字节

		m_tutorVec.append(pRecord);
    }
    if(m_nFlagArray[6])//夹具;
    {
		LPTutorRecordStr pRecord = new TutorRecordStr;
        pRecord->record.nType = AXIS_ACT_TUTOR_RECORD_TYPE;
        pRecord->record.pData = new qint16[7];//14字节;
        qint16 *p = (qint16 *)pRecord->record.pData;
        p[0]=5;//C轴;
        p[1]=ui->radioButtonH->isChecked();//水平和垂直;
        p[2]=ui->lineEditDelayC->text().remove(".").toInt();

        pRecord->strList.append("0");
        pRecord->strList.append("6");
        pRecord->strList.append(p[1]>0 ? "7" : "8");
        pRecord->strList.append("5");
        pRecord->strList.append(ui->lineEditDelayC->text());
		m_tutorVec.append(pRecord);
    }
	emit(insertTutorRecord(m_tutorVec, m_tutorVec.count(), nRow));
}

void AxisActPage::setAllSelected()
{
	memset(m_nFlagArray, 1, sizeof(m_nFlagArray));
	m_nFlagArray[0]=6;
}
