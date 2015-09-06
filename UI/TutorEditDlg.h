#ifndef TUTOREDITDLG_H
#define TUTOREDITDLG_H

#include <QDialog>
#include "Basectrl.h"
#include "KJLEditBox.h"
#include <QCheckBox>
#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QRadioButton>


namespace Ui {
class TutorEditDlg;
}

class TutorEditDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TutorEditDlg(LPTutorRecordStr m_pRecord, QWidget *parent = 0);
    ~TutorEditDlg();

//data:
    LPTutorRecordStr m_pRecord;

//controls
    QBoxLayout *m_pLayout;//布局
    QPushButton *m_pSetButton;//设入
    KJLEditBox *m_pPosBox;//位置;
    KJLEditBox *m_pSpeedBox;//速度;
    KJLEditBox *m_pDelayBox;//延迟时间;
    KJLEditBox *m_pDeclineSpeedBox;//提前减速;
    KJLEditBox *m_pDeclinePosBox;//提前位置;
    QCheckBox *m_pCheckEndPos;//提前结束位置;
    QCheckBox *m_pCheckDeclineSpeed;//提前减速;
    KJLEditBox *m_pLayoutSel;//堆叠选则;
    KJLEditBox *m_pMoldInvervalBox;//间隔模数;
    QRadioButton *m_pHorizantalCheck;//水平;
    QRadioButton *m_pVerticalCheck;//垂直;





private:
    Ui::TutorEditDlg *ui;

//methods:
    void setupAxisEidtLayout();//轴动编辑;
    void setupLayerEditLayout();//堆叠编辑;
    void setupZToolEditLayout();//堆叠编辑;
    void setupSignalEditLayout();//塑机信号编辑;
    void setupExtraEditLayout();//辅助设备编辑;
    void setupPreserveEditLayout();//预留编辑;
    void setupCheckEditLayout();//检测编辑;
    void setupWaitEditLayout();//等待编辑;
    void setupOtherEditLayout();//其他编辑;
    void setupSampleEditLayout(bool bMoldInterval = false, int delay_offset=2, int interval_off=3);//治具编辑;

private slots:
    void onCheckDeclineSpeedClicked();//提前结束位置被按下;
    void onCheckEndPosClicked();//提前减速被按下;

    void on_buttonBox_accepted();//确认操作，进行数据保存;
};

#endif // TUTOREDITDLG_H
