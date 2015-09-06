#include "TutorPage.h"
#include "ui_TutorPage.h"
#include <QDebug>
#include <QtCore/QModelIndex>
#include <QCheckBox>
#include <QLineEdit>
#include "UI/AxisActPage.h"
#include "UI/LayerPage.h"
#include "UI/ZtoolPage.h"
#include "UI/SignalPage.h"
#include "UI/ExtraDevPage.h"
#include "UI/PreservePage.h"
#include "UI/CheckPage.h"
#include "UI/WaitPage.h"
#include "UI/OtherPage.h"
#include "ArchivePage.h"
#include "UI/TutorEditDlg.h"
#include "ResMan/ResManager.h"
#include <QScrollBar>

//实现QAbstractTableView;

TutorModel::TutorModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_tutorVec(g_pArchiveModule->m_tutorVec)
{
    initTutorRecordVec();
    m_fmtRecord[AXIS_ACT_TUTOR_RECORD_TYPE]=&TutorModel::fmtAxisRecord;
    m_fmtRecord[LAYER_ACT_TUTOR_RECORD_TYPE]=&TutorModel::fmtLayerRecord;
    m_fmtRecord[ZTOOL_ACT_TUTOR_RECORD_TYPE]=&TutorModel::fmtZToolRecord;
    m_fmtRecord[SIGNAL_ACT_TUTOR_RECORD_TYPE]=&TutorModel::fmtSignalRecord;
    m_fmtRecord[EXTRA_DEV_TUTOR_RECORD_TYPE]=&TutorModel::fmtExtraDevRecord;
    m_fmtRecord[PRESERVE_TUTOR_RECORD_TYPE]=&TutorModel::fmtPreserveRecord;
    m_fmtRecord[CHECK_TUTOR_RECORD_TYPE]=&TutorModel::fmtCheckRecord;
    m_fmtRecord[WAIT_TUTOR_RECORD_TYPE]=&TutorModel::fmtWaitRecord;
    m_fmtRecord[OTHER_TUTOR_RECORD_TYPE]=&TutorModel::fmtOtherRecord;
	m_fmtRecord[MOLD_END_TUTOR_RECORD_TYPE]=&TutorModel::fmtEndRecord;


	//字符串;
	m_strListAxist.append("step *");//0
	m_strListAxist.append(tr("Speed:"));//1
	m_strListAxist.append(tr("Early End,"));//2
	m_strListAxist.append(tr("Early Speed-Down:"));//3
	m_strListAxist.append(tr("End Position:"));//4
	m_strListAxist.append(tr("Delay Time:"));//5
	m_strListAxist.append(tr("Posture:"));//6
	m_strListAxist.append(tr("Hor-1 "));//7
	m_strListAxist.append(tr("Ver-1 "));//8
	m_strListAxist.append("MY:");//9
	m_strListAxist.append("MZ:");//10
	m_strListAxist.append("X:");//11
	m_strListAxist.append("SY:");//12
	m_strListAxist.append("SZ:");//13

	m_strListAxist.append(tr("Layout On: "));//14
	m_strListAxist.append(tr("Selected:"));//15
	m_strListAxist.append(tr("Group "));//16

	m_strListAxist.append(tr("On "));//17
	m_strListAxist.append(tr("Off "));//18
	m_strListAxist.append(tr("Fixture1"));//19
	m_strListAxist.append(tr("Fixture2"));//20
	m_strListAxist.append(tr("Fixture3"));//21
	m_strListAxist.append(tr("Fixture4"));//22
	m_strListAxist.append(tr("Sucker1"));//23
	m_strListAxist.append(tr("Sucker2"));//24

	m_strListAxist.append(tr("Lock Mold"));//25
	m_strListAxist.append(tr("Eject"));//26
	m_strListAxist.append(tr("Core1"));//27
	m_strListAxist.append(tr("Core2"));//28

	m_strListAxist.append(tr("Times:"));//29
	m_strListAxist.append(tr("Action Time:"));//30
	m_strListAxist.append(tr("Injector "));//31
	m_strListAxist.append(tr("Conveyor "));//32

	m_strListAxist.append(tr("Fixture1"));//33
	m_strListAxist.append(tr("Fixture2"));//34
	m_strListAxist.append(tr("Fixture3"));//35
	m_strListAxist.append(tr("Fixture4"));//36
	m_strListAxist.append(tr("Sucker1"));//37
	m_strListAxist.append(tr("Sucker2"));//38
	m_strListAxist.append("X037");//39
	m_strListAxist.append("X023");//40
	m_strListAxist.append(tr("Cut "));//41
	m_strListAxist.append(tr(" Begin "));//42
	m_strListAxist.append(tr(" End "));//43

	m_strListAxist.append(tr("Wait OM"));//44
	m_strListAxist.append(tr("Wait SE.DR CL"));//45
	m_strListAxist.append(tr("Wait X043"));//46
	m_strListAxist.append(tr("Wait X044"));//47
	m_strListAxist.append(tr("Wait X037"));//48
	m_strListAxist.append(tr("Wait COREOUT"));//49
	m_strListAxist.append(tr("Wait EUEJF"));//50
	m_strListAxist.append(tr("Wait XCLIP1"));//51
	m_strListAxist.append(tr("Wait XCLIP2"));//52
	m_strListAxist.append(tr("Wait XCLIP3"));//53
	m_strListAxist.append(tr("Wait XCLIP4"));//54
	m_strListAxist.append(tr("Wait SUCKER1"));//55
	m_strListAxist.append(tr("Wait SUCKER2"));//56
	m_strListAxist.append(tr("Wait EUEJB"));//57
	m_strListAxist.append(tr("Wait COREIN"));//58
	m_strListAxist.append(tr("Limit Time:"));//59

	m_strListAxist.append(tr("Product Clear"));//60
	m_strListAxist.append(tr("Mold End"));//61


}


TutorModel::~TutorModel()
{

}


int TutorModel::rowCount(const QModelIndex &parent) const
{
    return m_tutorVec.count();
}

int TutorModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant TutorModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
            return QVariant();

    if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    else if(role == Qt::DisplayRole)//显示内容;
    {
        LPTutorRecordStr pRecord= m_tutorVec.at(index.row());
        TutorModel *pMod = const_cast<TutorModel *>(this);
        return (pMod->*(m_fmtRecord[pRecord->record.nType]))(pRecord);
    }
    else if(role == Qt::BackgroundColorRole)//背景颜色;
    {
        LPTutorRecordStr pRecord= m_tutorVec.at(index.row());
        if(pRecord->record.nStep == 0)
            return QColor(Qt::lightGray).lighter(125);
        else if(pRecord->record.nType == WAIT_TUTOR_RECORD_TYPE)
            return QColor(Qt::red).lighter(120);
        else if(pRecord->record.nStep % 2 == 0)
            return QColor(Qt::blue).lighter(180);
        else
            return QColor(Qt::yellow).lighter(180);
    }
    return QVariant();
}

bool TutorModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        //m_tutorVec.replace(index.row(), value.toString());

        //某项发生改变，发射信号;
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

bool TutorModel::insertRow(int row, const QModelIndex &parent)
{
    return insertRows(row, 1, parent);
}

bool TutorModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(QModelIndex(), row, row+count-1);
    endInsertRows();
    return true;
}

bool TutorModel::removeRow(int row, const QModelIndex &parent)
{
    return removeRows(row, 1, parent);
}

bool TutorModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), row, row+count-1);
    endRemoveRows();
    return true;
}

void TutorModel::onInsertTutorRecord(QVector<LPTutorRecordStr>& pRecord, int cnt, int nRow)
{
    int prevStep;
    int nextStep=0;
    if(nRow == 0)//表头;
    {
		for(int i = 0; i < cnt; i++)
		{
			pRecord.at(i)->record.nStep = 0;
			m_tutorVec.insert(nRow+i, pRecord.at(i));
		}
        updateRecordStep(nRow+cnt, 1);
    }
    else if(nRow > 0 && nRow < rowCount(QModelIndex())-1)//中间插入;
    {
        prevStep = m_tutorVec.at(nRow-1)->record.nStep;
        nextStep = m_tutorVec.at(nRow+1)->record.nStep;
        for(int i = 0; i < cnt; i++)
        {
            pRecord.at(i)->record.nStep = prevStep==nextStep?prevStep:prevStep+1;
            m_tutorVec.insert(nRow+i, pRecord.at(i));
        }
        if(prevStep!=nextStep)
            updateRecordStep(nRow+cnt, 1);
    }
    else//表尾;
    {
        prevStep = m_tutorVec.at(nRow-1)->record.nStep;
        for(int i = 0; i < cnt; i++)
        {
            pRecord.at(i)->record.nStep = prevStep+1;
            m_tutorVec.insert(nRow+i, pRecord.at(i));
        }
        updateRecordStep(nRow+cnt, 1);
    }

    insertRows(nRow, cnt, QModelIndex());
    return;

}

void TutorModel::onInsertTutorRecord(LPTutorRecordStr pRecord, int cnt, int nRow)
{
    int prevStep;
    int nextStep=0;
    if(nRow == 0)//表头;
    {
		/*for(int i = 0; i < cnt; i++)
		{
		pRecord[i].record.nStep = 1;
		m_tutorVec.insert(nRow+i, &pRecord[i]);
		}*/
		pRecord->record.nStep = 0;
		m_tutorVec.insert(nRow, pRecord);
        updateRecordStep(nRow+cnt, 1);
    }
    else if(nRow > 0 && nRow < rowCount(QModelIndex())-1)//中间插入;
    {
        prevStep = m_tutorVec.at(nRow-1)->record.nStep;
        nextStep = m_tutorVec.at(nRow+1)->record.nStep;
        for(int i = 0; i < cnt; i++)
        {
            pRecord[i].record.nStep = prevStep==nextStep?prevStep:prevStep+1;
            m_tutorVec.insert(nRow+i, &pRecord[i]);
        }
        if(prevStep!=nextStep)
            updateRecordStep(nRow+cnt, 1);
    }
    else//表尾;
    {
        prevStep = m_tutorVec.at(nRow-1)->record.nStep;
        for(int i = 0; i < cnt; i++)
        {
            pRecord[i].record.nStep = prevStep+1;
            m_tutorVec.insert(nRow+i, &pRecord[i]);
        }
        updateRecordStep(nRow+cnt, 1);
    }

    insertRows(nRow, cnt, QModelIndex());
    return;

}

//初始化教导线性表;
void TutorModel::initTutorRecordVec()
{

}

void TutorModel::deleteTutorRecord(int nRow)
{
    int prevStep;
    int nextStep=0;
    if(nRow == 0 || nRow == rowCount(QModelIndex())-1)//表头或者表尾;
    {
       updateRecordStep(nRow+1, -1);
    }else//中间;
    {
        prevStep = m_tutorVec.at(nRow-1)->record.nStep;
        nextStep = m_tutorVec.at(nRow+1)->record.nStep;
        int curStep = m_tutorVec.at(nRow)->record.nStep;
        if(prevStep != curStep && nextStep != curStep)
            updateRecordStep(nRow+1, -1);
    }

    LPTutorRecordStr pRecord = m_tutorVec.at(nRow);
	m_tutorVec.remove(nRow);//从vecotr中取走;

	//释放内存;
	qDebug()<<pRecord->record.pData;
    delete pRecord->record.pData;
    delete pRecord;

    removeRow(nRow, QModelIndex());
}

void TutorModel::updateRecordStep(int nRow, int add)
{
    for(int i = nRow; i < m_tutorVec.size(); i++)
    {
        m_tutorVec.at(i)->record.nStep+=add;
    }
}

void TutorModel::comboTutorRecord(int nRow)
{
    if(nRow == 0 || nRow == rowCount(QModelIndex())-1)//表头或者表尾;
    {
       return;
    }
    else//中间;
    {
        int prevStep = m_tutorVec.at(nRow-1)->record.nStep;
        int nextStep = m_tutorVec.at(nRow+1)->record.nStep;
        int curStep = m_tutorVec.at(nRow)->record.nStep;
        if(curStep != nextStep)
        {
            if(curStep!=prevStep)
            {
                updateRecordStep(nRow, -1);
                QModelIndex tlIndex = index(nRow, 0, QModelIndex());
                QModelIndex brIndex = index(rowCount(QModelIndex())-1, 0, QModelIndex());
                emit dataChanged(tlIndex, brIndex);
            }
        }
        else
        {
            m_tutorVec.at(nRow)->record.nStep = prevStep;
            QModelIndex tlIndex = index(nRow, 0, QModelIndex());
            emit dataChanged(tlIndex, tlIndex);
        }
    }
}

void TutorModel::splitTutorRecord(int nRow)
{
    if(nRow == 0 || nRow == rowCount(QModelIndex())-1)//表头或者表尾;
    {
       return;
    }
    else
    {
        int prevStep = m_tutorVec.at(nRow-1)->record.nStep;
        int curStep = m_tutorVec.at(nRow)->record.nStep;
        if(curStep == prevStep)//和前驱节点的步序号一样才能分解;
        {
            updateRecordStep(nRow, 1);
            QModelIndex tlIndex = index(nRow, 0, QModelIndex());
            QModelIndex brIndex = index(rowCount(QModelIndex())-1, 0, QModelIndex());
            emit dataChanged(tlIndex, brIndex);
        }
    }
}

LPTutorRecordStr TutorModel::getRawTutorRecord(int nRow)
{
    if(nRow >= rowCount(QModelIndex()))
        return NULL;
    else
        return m_tutorVec.at(nRow);
}


QString TutorModel::fmtAxisRecord(LPTutorRecordStr pRecord)
{
    qint16 *p = (qint16 *)pRecord->record.pData;
    QStringList list = pRecord->strList;
    if(p[0] < 5 && p[0] >=0)
    {
        list.replace(0, (pRecord->record.nStep==0 ? tr("Home * "):QString("%1 * ").arg(pRecord->record.nStep)) );
		list.replace(1, m_strListAxist.at(list.at(1).toInt()));//!!!!!!
        list.replace(2, QString::number(p[1]/100.0, 'f', 2)+" ");
		list.replace(3, m_strListAxist.at(list.at(3).toInt()));
        list.replace(4, QString::number(p[2])+" ");
		list.replace(5, m_strListAxist.at(list.at(5).toInt()));
		list.replace(6, m_strListAxist.at(list.at(6).toInt()));
        list.replace(7, QString::number(p[5])+" ");
		list.replace(8, m_strListAxist.at(list.at(8).toInt()));
        list.replace(9, QString::number(p[6]/100.0, 'f', 2)+" ");
		list.replace(10, m_strListAxist.at(list.at(10).toInt()));
        list.replace(11, QString::number(p[3]/100.0, 'f', 2));
        if( (p[4] & 0x0101) == 0)//去除减速位置和速度显示;
        {
            for(int i = 0; i < 5; i++)
                list.removeAt(5);
        }
        else if( (p[4] & 0x0100) == 0)//提前结束位置不使能;
        {
            list.removeAt(5);
        }
        else if( (p[4] & 0x0001) == 0)//提前减速不使能;
        {
            list.removeAt(6);
            list.removeAt(6);
        }
        return list.join("");
    }
    else if(p[0] == 5)//C轴;
    {
        list.replace(0, (pRecord->record.nStep==0 ? tr("Home * "):QString("%1 * ").arg(pRecord->record.nStep)));
		list.replace(1, m_strListAxist.at(list.at(1).toInt()));
        list.replace(2, m_strListAxist.at(list.at(2).toInt()));
		list.replace(3, m_strListAxist.at(list.at(3).toInt()));
        list.replace(4, QString::number(p[2]/100.0, 'f', 2));
        return list.join("");
    }

}

QString TutorModel::fmtLayerRecord(LPTutorRecordStr pRecord)
{
    qint16 *p = (qint16 *)pRecord->record.pData;
    QStringList list = pRecord->strList;
    list.replace(0, QString("%1 * ").arg(pRecord->record.nStep));
	list.replace(1, m_strListAxist.at(list.at(1).toInt()));
	list.replace(2, m_strListAxist.at(list.at(2).toInt()));
    list.replace(3, QString::number(p[0]));
	list.replace(4, m_strListAxist.at(list.at(4).toInt()));
	list.replace(5, m_strListAxist.at(list.at(5).toInt()));
    list.replace(6, QString::number(p[1]/100.0, 'f', 2));
    return list.join("");
}

QString TutorModel::fmtZToolRecord(LPTutorRecordStr pRecord)
{
    qint16 *p = (qint16 *)pRecord->record.pData;
    QStringList list = pRecord->strList;
    list.replace(0, QString("%1 * ").arg(pRecord->record.nStep));
	list.replace(1, m_strListAxist.at(list.at(1).toInt()));
	list.replace(2, m_strListAxist.at(list.at(2).toInt()));
	list.replace(3, m_strListAxist.at(list.at(3).toInt()));
    list.replace(4, QString::number(p[2]/100.0, 'f', 2));
    return list.join("");
}

QString TutorModel::fmtSignalRecord(LPTutorRecordStr pRecord)
{
    qint16 *p = (qint16 *)pRecord->record.pData;
    QStringList list = pRecord->strList;
    list.replace(0, QString("%1 * ").arg(pRecord->record.nStep));
	list.replace(1, m_strListAxist.at(list.at(1).toInt()));
	list.replace(2, m_strListAxist.at(list.at(2).toInt()));
	list.replace(3, m_strListAxist.at(list.at(3).toInt()));
    list.replace(4, QString::number(p[2]/100.0, 'f', 2));
    return list.join("");
}

QString TutorModel::fmtExtraDevRecord(LPTutorRecordStr pRecord)
{
    qint16 *p = (qint16 *)pRecord->record.pData;
    QStringList list = pRecord->strList;
    list.replace(0, QString("%1 * ").arg(pRecord->record.nStep));
	list.replace(1, m_strListAxist.at(list.at(1).toInt()));
    list.replace(2, m_strListAxist.at(list.at(2).toInt()));
	list.replace(3, m_strListAxist.at(list.at(3).toInt()));
    list.replace(4, QString::number(p[3])+" ");
	list.replace(5, m_strListAxist.at(list.at(5).toInt()));
    list.replace(6, QString::number(p[2]/100.0, 'f', 2));
    return list.join("");
}

QString TutorModel::fmtPreserveRecord(LPTutorRecordStr pRecord)
{
    qint16 *p = (qint16 *)pRecord->record.pData;
    QStringList list = pRecord->strList;
    list.replace(0, QString("%1 * ").arg(pRecord->record.nStep));
	list.replace(1, tr("Reserve")+list.at(1)+" ");
    list.replace(2, m_strListAxist.at(list.at(2).toInt()));
	list.replace(3, m_strListAxist.at(list.at(3).toInt()));
    list.replace(4, QString::number(p[3])+" ");
	list.replace(5, m_strListAxist.at(list.at(5).toInt()));
    list.replace(6, QString::number(p[2]/100.0, 'f', 2));
    return list.join("");
}

QString TutorModel::fmtCheckRecord(LPTutorRecordStr pRecord)
{
    qint16 *p = (qint16 *)pRecord->record.pData;
    QStringList list = pRecord->strList;
    list.replace(0, QString("%1 * ").arg(pRecord->record.nStep));
	list.replace(1, m_strListAxist.at(list.at(1).toInt()));
    list.replace(2, m_strListAxist.at(list.at(2).toInt()));
	list.replace(3, m_strListAxist.at(list.at(3).toInt()));
	list.replace(4, m_strListAxist.at(list.at(4).toInt()));
    list.replace(5, QString::number(p[2]/100.0, 'f', 2));
    return list.join("");
}

QString TutorModel::fmtWaitRecord(LPTutorRecordStr pRecord)
{
    qint16 *p = (qint16 *)pRecord->record.pData;
    QStringList list = pRecord->strList;
    list.replace(0, QString("%1 * ").arg(pRecord->record.nStep));
	list.replace(1, m_strListAxist.at(list.at(1).toInt())+" ");
	list.replace(2, m_strListAxist.at(list.at(2).toInt()));
    list.replace(3, QString::number(p[1]/100.0, 'f', 2));
    return list.join("");
}

QString TutorModel::fmtOtherRecord(LPTutorRecordStr pRecord)
{
    return fmtWaitRecord(pRecord);
}

QString TutorModel::fmtEndRecord(LPTutorRecordStr pRecord)
{
	return fmtWaitRecord(pRecord);
}













TutorPage::TutorPage(QWidget *parent) :
    QWidget(parent),
    m_nTutorMode(-1),
    ui(new Ui::TutorPage)
{
    double dXfactor = (double)g_pResManModule->m_pProjectParm->m_nWidth / 800;//因为在UI文件中布局时候是假设面板大小为800*600;
    ui->setupUi(this);

    //设置视图模型
    TutorModel *pModel = new TutorModel(this);
    ui->tableView->setModel(pModel);
    ui->tableView->setColumnWidth(0,500*dXfactor);

    ui->tableView->horizontalHeader()->hide();
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //ui->tableView->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background:transparent;width:30px; border: 2px solid transparent;}");
    //ui->tableView->horizontalScrollBar()->setStyleSheet("QScrollBar:horizantal{background:transparent;width:30px; border: 2px solid transparent;}");

    setAttribute(Qt::WA_TranslucentBackground);
    QObjectList childList = children();
    foreach (QObject *obj, childList) {
        QWidget *w = qobject_cast<QWidget *>(obj);
        if(w)
            w->setAttribute(Qt::WA_TranslucentBackground);
    }

    ui->tableView->verticalHeader()->setDefaultSectionSize(50);


    on_pushButtonMenu_clicked();

}

TutorPage::~TutorPage()
{
    delete ui;


}

//插入;
void TutorPage::on_pushButtonInsert_clicked()
{
    //向view中加入数据;
    int row = 0;
    if(ui->tableView->model()->rowCount() == 0)
        row = 0;
    else
    {
        row = ui->tableView->currentIndex().row();
        if( row == -1 )
            return;
    }

    if(row < 6 )
	{
		QMessageBox::warning(0, tr("tips"), tr("Can not add standby position program"));
		return;
	}
    emit(insertTutorRecord(row));

}



//编辑;
void TutorPage::on_pushButtonEdit_clicked()
{
    int row = ui->tableView->currentIndex().row();
    if(row == -1)
        return;
    TutorModel *pMod = static_cast<TutorModel *>(ui->tableView->model());
    TutorEditDlg dlg(pMod->getRawTutorRecord(row), this->parentWidget());//建立编辑对话框;
    double dXfactor = (double)g_pResManModule->m_pProjectParm->m_nWidth / 800;//因为在UI文件中布局时候是假设面板大小为800*600;
    double dYfactor = (double)g_pResManModule->m_pProjectParm->m_nHeight / 600;//因为在UI文件中布局时候是假设面板大小为800*600;
    dlg.resize(dlg.width()*dXfactor, dlg.height()*dYfactor);
    dlg.exec();
}

//删除;
void TutorPage::on_pushButtonDel_clicked()
{
    int row = ui->tableView->currentIndex().row();
    if(row == -1)
        return;

	TutorModel *pMod = static_cast<TutorModel *>(ui->tableView->model());
	if(row < 6 )
	{
		QMessageBox::warning(0, tr("tips"), tr("Can not remove standby position program"));
		return;
	}
	else if(row == pMod->rowCount(QModelIndex())-1)
		return;

    pMod->deleteTutorRecord(row);
}



//分解;
void TutorPage::on_pushButtonSplit_clicked()
{
    int row = ui->tableView->currentIndex().row();
    if(row == -1)
        return;
    TutorModel *pMod = static_cast<TutorModel *>(ui->tableView->model());
    pMod->splitTutorRecord(row);
}

//组合;
void TutorPage::on_pushButtonCombo_clicked()
{
    int row = ui->tableView->currentIndex().row();
    if(row == -1)
        return;
    TutorModel *pMod = static_cast<TutorModel *>(ui->tableView->model());
    pMod->comboTutorRecord(row);
}

//菜单;
void TutorPage::on_pushButtonMenu_clicked()
{
    if(m_nTutorMode == 0)
        return;
    m_nTutorMode = 0;
    clearSubPage();
    //布局管理;
    QGridLayout *pLayout = new QGridLayout(ui->groupBox);
    pLayout->setContentsMargins(20,20,20,20);
    pLayout->setHorizontalSpacing(20);
    pLayout->setVerticalSpacing(5);
    //轴动作;
    QPushButton *pBtn = new QPushButton(ui->groupBox);
    pBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pBtn->setText(tr("Line"));
    QObject::connect(pBtn, SIGNAL(clicked()), this, SLOT(onPushBtnAxisActionClicked()) );
    pLayout->addWidget(pBtn, 0,0,1,1);

    //堆叠;
    pBtn = new QPushButton(ui->groupBox);
    pBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pBtn->setText(tr("Stack"));
    QObject::connect(pBtn, SIGNAL(clicked()), this, SLOT(onPushBtnLayerClicked()) );
    pLayout->addWidget(pBtn, 0,1,1,1);

    //治具;
    pBtn = new QPushButton(ui->groupBox);
    pBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pBtn->setText(tr("Fixture"));
    QObject::connect(pBtn, SIGNAL(clicked()), this, SLOT(onPushBtnZToolClicked()) );
    pLayout->addWidget(pBtn, 1,0,1,1);

    //塑机信号;
    pBtn = new QPushButton(ui->groupBox);
    pBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pBtn->setText(tr("Injector"));
    QObject::connect(pBtn, SIGNAL(clicked()), this, SLOT(onPushBtnSignalClicked()) );
    pLayout->addWidget(pBtn, 1,1,1,1);

    //辅助设备;
    pBtn = new QPushButton(ui->groupBox);
    pBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pBtn->setText(tr("Periphery"));
    QObject::connect(pBtn, SIGNAL(clicked()), this, SLOT(onPushBtnExtraDeviceClicked()) );
    pLayout->addWidget(pBtn, 2,0,1,1);

    //预留;
    pBtn = new QPushButton(ui->groupBox);
    pBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pBtn->setText(tr("Reserve"));
    QObject::connect(pBtn, SIGNAL(clicked()), this, SLOT(onPushBtnPreserveClicked()) );
    pLayout->addWidget(pBtn, 2,1,1,1);

    //检测;
    pBtn = new QPushButton(ui->groupBox);
    pBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pBtn->setText(tr("Cut"));
    QObject::connect(pBtn, SIGNAL(clicked()), this, SLOT(onPushBtnCheckClicked()) );
    pLayout->addWidget(pBtn, 3,0,1,1);

    //等待;
    pBtn = new QPushButton(ui->groupBox);
    pBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pBtn->setText(tr("Wait"));
    QObject::connect(pBtn, SIGNAL(clicked()), this, SLOT(onPushBtnWaitClicked()) );
    pLayout->addWidget(pBtn, 3,1,1,1);

    //其他;
    pBtn = new QPushButton(ui->groupBox);
    pBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pBtn->setText(tr("Other"));
    QObject::connect(pBtn, SIGNAL(clicked()), this, SLOT(onPushBtnOtherClicked()) );
    pLayout->addWidget(pBtn, 4,0,1,1);

    ui->groupBox->setLayout(pLayout);

    //setAttribute(Qt::WA_TranslucentBackground);
    QObjectList childList = ui->groupBox->children();
    foreach (QObject *obj, childList) {
        QWidget *w = qobject_cast<QWidget *>(obj);
        if(w)
            w->setAttribute(Qt::WA_TranslucentBackground);
    }
}

//轴动作页面;
void TutorPage::onPushBtnAxisActionClicked()
{
    if(m_nTutorMode == 1)
        return;
    m_nTutorMode = 1;
    clearSubPage();
    QGridLayout *pLayout = new QGridLayout(ui->groupBox);
    AxisActPage *pPage = new AxisActPage(ui->groupBox);
    pLayout->addWidget(pPage);
    pPage->show();
    TutorModel *pMod = static_cast<TutorModel *>(ui->tableView->model());
    QObject::connect(this, SIGNAL(insertTutorRecord(int)), pPage, SLOT(onInsertTutorRecord(int)) );
    //QObject::connect(pPage, SIGNAL(insertTutorRecord(LPTutorRecordStr, int, int)), pMod, SLOT(onInsertTutorRecord(LPTutorRecordStr, int, int)) );
	QObject::connect(pPage, SIGNAL(insertTutorRecord(QVector<LPTutorRecordStr> &, int, int)), pMod, SLOT(onInsertTutorRecord(QVector<LPTutorRecordStr> &, int, int)) );
}

//堆叠页面;
void TutorPage::onPushBtnLayerClicked()
{
    if(m_nTutorMode == 2)
        return;
    m_nTutorMode = 2;
    clearSubPage();
    QGridLayout *pLayout = new QGridLayout(ui->groupBox);
    LayerPage *pPage = new LayerPage(ui->groupBox);
    pLayout->addWidget(pPage);
    pPage->show();
    TutorModel *pMod = static_cast<TutorModel *>(ui->tableView->model());
    QObject::connect(this, SIGNAL(insertTutorRecord(int)), pPage, SLOT(onInsertTutorRecord(int)) );
    QObject::connect(pPage, SIGNAL(insertTutorRecord(LPTutorRecordStr, int, int)), pMod, SLOT(onInsertTutorRecord(LPTutorRecordStr, int, int)) );
}

//治具页面;
void TutorPage::onPushBtnZToolClicked()
{
    if(m_nTutorMode == 3)
        return;
    m_nTutorMode = 3;
    clearSubPage();
    QGridLayout *pLayout = new QGridLayout(ui->groupBox);

    Ztool *pPage = new Ztool(ui->groupBox);
    pLayout->addWidget(pPage);
    pPage->adjustHeaderWidth();
    pPage->show();
    TutorModel *pMod = static_cast<TutorModel *>(ui->tableView->model());
    QObject::connect(this, SIGNAL(insertTutorRecord(int)), pPage, SLOT(onInsertTutorRecord(int)) );
    QObject::connect(pPage, SIGNAL(insertTutorRecord(LPTutorRecordStr, int, int)), pMod, SLOT(onInsertTutorRecord(LPTutorRecordStr, int, int)) );
}

//塑机信号页面;
void TutorPage::onPushBtnSignalClicked()
{
    if(m_nTutorMode == 4)
        return;
    m_nTutorMode = 4;
    clearSubPage();
    QGridLayout *pLayout = new QGridLayout(ui->groupBox);
    SignalPage *pPage = new SignalPage(ui->groupBox);
    pLayout->addWidget(pPage);
    pPage->show();
    TutorModel *pMod = static_cast<TutorModel *>(ui->tableView->model());
    QObject::connect(this, SIGNAL(insertTutorRecord(int)), pPage, SLOT(onInsertTutorRecord(int)) );
    QObject::connect(pPage, SIGNAL(insertTutorRecord(LPTutorRecordStr, int, int)), pMod, SLOT(onInsertTutorRecord(LPTutorRecordStr, int, int)) );
}

//辅助设备页面;
void TutorPage::onPushBtnExtraDeviceClicked()
{
    if(m_nTutorMode == 5)
        return;
    m_nTutorMode = 5;
    clearSubPage();
    QGridLayout *pLayout = new QGridLayout(ui->groupBox);
    ExtraDevPage *pPage = new ExtraDevPage(ui->groupBox);
    pLayout->addWidget(pPage);
    pPage->show();
    TutorModel *pMod = static_cast<TutorModel *>(ui->tableView->model());
    QObject::connect(this, SIGNAL(insertTutorRecord(int)), pPage, SLOT(onInsertTutorRecord(int)) );
    QObject::connect(pPage, SIGNAL(insertTutorRecord(LPTutorRecordStr, int, int)), pMod, SLOT(onInsertTutorRecord(LPTutorRecordStr, int, int)) );
}

//预留页面;
void TutorPage::onPushBtnPreserveClicked()
{
    if(m_nTutorMode == 6)
        return;
    m_nTutorMode = 6;
    clearSubPage();
    QGridLayout *pLayout = new QGridLayout(ui->groupBox);
    PreservePage *pPage = new PreservePage(ui->groupBox);
    pLayout->addWidget(pPage);
    pPage->show();
    TutorModel *pMod = static_cast<TutorModel *>(ui->tableView->model());
    QObject::connect(this, SIGNAL(insertTutorRecord(int)), pPage, SLOT(onInsertTutorRecord(int)) );
    QObject::connect(pPage, SIGNAL(insertTutorRecord(LPTutorRecordStr, int, int)), pMod, SLOT(onInsertTutorRecord(LPTutorRecordStr, int, int)) );
}

//检测页面;
void TutorPage::onPushBtnCheckClicked()
{
    if(m_nTutorMode == 7)
        return;
    m_nTutorMode = 7;
    clearSubPage();
    QGridLayout *pLayout = new QGridLayout(ui->groupBox);
    CheckPage *pPage = new CheckPage(ui->groupBox);
    pLayout->addWidget(pPage);
    pPage->show();
    TutorModel *pMod = static_cast<TutorModel *>(ui->tableView->model());
    QObject::connect(this, SIGNAL(insertTutorRecord(int)), pPage, SLOT(onInsertTutorRecord(int)) );
    QObject::connect(pPage, SIGNAL(insertTutorRecord(LPTutorRecordStr, int, int)), pMod, SLOT(onInsertTutorRecord(LPTutorRecordStr, int, int)) );
}

//等待页面;
void TutorPage::onPushBtnWaitClicked()
{
    if(m_nTutorMode == 8)
        return;
    m_nTutorMode = 8;
    clearSubPage();
    QGridLayout *pLayout = new QGridLayout(ui->groupBox);
    WaitPage *pPage = new WaitPage(ui->groupBox);
    pLayout->addWidget(pPage);
    pPage->show();
    TutorModel *pMod = static_cast<TutorModel *>(ui->tableView->model());
    QObject::connect(this, SIGNAL(insertTutorRecord(int)), pPage, SLOT(onInsertTutorRecord(int)) );
    QObject::connect(pPage, SIGNAL(insertTutorRecord(LPTutorRecordStr, int, int)), pMod, SLOT(onInsertTutorRecord(LPTutorRecordStr, int, int)) );

}

//其他画面;
void TutorPage::onPushBtnOtherClicked()
{
    if(m_nTutorMode == 9)
        return;
    m_nTutorMode = 9;
    clearSubPage();
    QGridLayout *pLayout = new QGridLayout(ui->groupBox);
    OtherPage *pPage = new OtherPage(ui->groupBox);
    pLayout->addWidget(pPage);
    pPage->show();
    TutorModel *pMod = static_cast<TutorModel *>(ui->tableView->model());
    QObject::connect(this, SIGNAL(insertTutorRecord(int)), pPage, SLOT(onInsertTutorRecord(int)) );
    QObject::connect(pPage, SIGNAL(insertTutorRecord(LPTutorRecordStr, int, int)), pMod, SLOT(onInsertTutorRecord(LPTutorRecordStr, int, int)) );
}

//清除子页面;
void TutorPage::clearSubPage()
{
    QObjectList list;//删除布局内的子布局对象;
    if( ui->groupBox->layout() )
    {
        list = ui->groupBox->layout()->children();
        for(QObjectList::const_iterator it = list.begin(); it!=list.end(); it++)
        {
            delete *it;
        }
    }

    list = ui->groupBox->children();
    for(QObjectList::const_iterator it = list.begin(); it!=list.end(); it++)
    {
        delete *it;
    }

}


//设置内定程序;
void TutorPage::on_pushButtonQuickSet_clicked()
{

}

//清空程序，重新设置;
void TutorPage::on_pushButtonReset_clicked()
{
	if(QMessageBox::Ok != QMessageBox::warning(0, tr("warning"), tr("Are you sure to reset?"), QMessageBox::Ok|QMessageBox::Cancel) )
		return;


	TutorModel *pMod = static_cast<TutorModel *>(ui->tableView->model());
	//[0]清除所有记录;
	int cnt = pMod->rowCount( QModelIndex() );
	for(int i = 0; i < cnt; i++)
	{
		pMod->deleteTutorRecord(0);
	}

	//[1]待机点设置;
	AxisActPage page;
	page.setAllSelected();
	QObject::connect(&page, SIGNAL(insertTutorRecord(QVector<LPTutorRecordStr> &, int, int)), pMod, SLOT(onInsertTutorRecord(QVector<LPTutorRecordStr> &, int, int)) );
	page.onInsertTutorRecord(0);

	//[2]模组结束;
	LPTutorRecordStr pRecord = new TutorRecordStr;
	pRecord->record.nStep = 0;
	pRecord->record.nType = MOLD_END_TUTOR_RECORD_TYPE;
	pRecord->record.pData = new qint16[2];
	qint16 *p = (qint16 *)pRecord->record.pData;
	p[0]=0;
	p[1]=0;//延时;

	pRecord->strList.append("0");
	pRecord->strList.append("61");
	pRecord->strList.append("5");
	pRecord->strList.append("0");

	pMod->onInsertTutorRecord(pRecord, 1, 6);

}
