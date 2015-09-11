#include "SimulateMan.h"
#include "ArchiveManager.h"
#include <QtCore/qmath.h>
#include <QDebug>
#include <QtNetwork>
#include "Protocol.h"


SimulateMan::SimulateMan(ArchiveManager *pArch, QObject *parent) :
    QObject(parent),
    m_pArch(pArch),
    m_pData(0)
{

}

SimulateMan::~SimulateMan()
{

}

void SimulateMan::setupPrivate(char *p)
{
    m_pData = (SimulateStruct *)p;
    quint16 nAddr = m_pData->nAddr;
    qint16 nOffset;
    qint16 nBase;
    if(m_pData->bWarning)//生成代表本软元件的字节码;
    {
        if(m_pData->nAddr < M_COIL_OFFSET)//D是字软元件,不应该在此出现;
        {
            nBase = 21000;
            nOffset = nAddr;
            nOffset /= 2;
        }
        else if(m_pData->nAddr >= M_COIL_OFFSET && m_pData->nAddr < X_COIL_OFFSET)//M字软元件
        {
            nBase = 2000;
            nOffset = nAddr-M_COIL_OFFSET;
        }
        else if(m_pData->nAddr >= X_COIL_OFFSET && m_pData->nAddr < Y_COIL_OFFSET)//X位软元件
        {
            nBase = 0;
            nOffset = nAddr-X_COIL_OFFSET;
        }
        else if(m_pData->nAddr >= Y_COIL_OFFSET && m_pData->nAddr < MOLDSIZE)//Y位软元件
        {
            nBase = 500;
            nOffset = nAddr-Y_COIL_OFFSET;
        }
        else
        {
            m_coilCode.clear();
            return;
        }
        //address
        m_nCoilCode = nOffset;
        nBase += nOffset;
        m_coilCode.append((nBase>>8)&0xff);
        m_coilCode.append(nBase&0xff);
    }

}

void SimulateMan::getFmt(int *pMax, int *pMin, int *pDecimal, QString &strMax, QString &strMin)
{
    if(pMax)
    {*pMax = m_pData->nMax;}
    if(pMin)
    {*pMin = m_pData->nMin;}
    if(pDecimal)
    {*pDecimal = m_pData->nDecimal;}

	if(m_pData->bMaxDollar)//最大值间接寻址;
	{
		SimulateMan *pSimulate = g_pArchiveModule->simulateMap.value(m_pData->nMax, NULL);//获取对应的变量;
		if(!pSimulate)
			strMax="0";
		else
			strMax = pSimulate->read();
	}
	else
	{
		int pow = qPow(10, m_pData->nDecimal);
		double temp = (double)(m_pData->nMax) / pow;
		strMax = QString::number(temp, 'f', m_pData->nDecimal);
	}
	if(m_pData->bMinDollar)//最小值间接寻址;
	{
		SimulateMan *pSimulate = g_pArchiveModule->simulateMap.value(m_pData->nMin, NULL);//获取对应的变量;
		if(!pSimulate)
			strMin="0";
		else
			strMin = pSimulate->read();
	}
	else
	{
		int pow = qPow(10, m_pData->nDecimal);
		double temp = (double)(m_pData->nMin) / pow;
		strMin = QString::number(temp, 'f', m_pData->nDecimal);
	}

    
    
}

void SimulateMan::getFmt(int *pMax, int *pMin, int *pDecimal, int *pUpAlarm, int *pDwnAlarm)
{

    if(pMax)
        *pMax = m_pData->nMax;
    if(pMin)
        *pMin = m_pData->nMin;
    if(pDecimal)
        *pDecimal = m_pData->nDecimal;

    if(pUpAlarm)
        *pUpAlarm = m_pData->nUpAlarm;
    if(pDwnAlarm)
        *pDwnAlarm = m_pData->nDownAlarm;
}



void SimulateMan::write(const QString &data, QObject *who, bool bSend)
{
    QString strTemp(data);
    double dTemp = strTemp.toDouble();
    strTemp = QString::number(dTemp, 'f', m_pData->nDecimal);
    strTemp.remove(".");

    //判断数据类型;
	switch(m_pData->nDataType)
	{
		//单字节;
		case 0:
		case 1:
			m_pArch->writeByte(m_pData->nAddr, strTemp.toInt());//写入内存;
			break;
		//两字节;
		case 2:
			m_pArch->writeWord(m_pData->nAddr, strTemp.toUShort());//写入内存;
			break;
		case 3:
			m_pArch->writeWord(m_pData->nAddr, strTemp.toShort());//写入内存;
			break;
		//三字节;
		case 4:
		case 5:
			break;
		//四字节;
		case 6:
		case 7:
			break;
	}

    emit(updateData(read(), this, who));
    emit(updateData(readInt(), this, who));

    if(bSend)
        sendOut();
}

void SimulateMan::write(const int data, QObject *who, bool bSend)
{
    //判断数据类型;
	switch(m_pData->nDataType)
	{
		//单字节;
	case 0:
	case 1:
		m_pArch->writeByte(m_pData->nAddr, data);
		break;
		//两字节;
	case 2:
	case 3:
		m_pArch->writeWord(m_pData->nAddr, data);//写入内存;
		break;
		//三字节;
	case 4:
	case 5:
		break;
		//四字节;
	case 6:
	case 7:
		break;
	}
    emit(updateData(readInt(), this, who));
    emit(updateData(read(), this, who));

    if(bSend)
        sendOut();
}


QString SimulateMan::read()
{
    QString strResult="0";
    //数据类型(m_pData->nDataType);

    //地址类型(m_pData->nAddrType);

    //地址(m_pData->nAddr);

    //基数、系数、小数位数(假设2字节无符号数);
	switch(m_pData->nDataType)
	{
		case 0:
		{
			quint8 temp = m_pArch->readByte(m_pData->nAddr);//读2字节数据;
			temp = temp*(m_pData->nFactor)+m_pData->nBase;
			int nPow = qPow(10, m_pData->nDecimal);
			double result = (double)temp / nPow;
			strResult = QString::number(result, 'f', m_pData->nDecimal);
		}
		break;
		case 1:
		{
			qint8 temp = m_pArch->readByte(m_pData->nAddr);//读2字节数据;
			temp = temp*(m_pData->nFactor)+m_pData->nBase;
			int nPow = qPow(10, m_pData->nDecimal);
			double result = (double)temp / nPow;
			strResult = QString::number(result, 'f', m_pData->nDecimal);
		}
		break;
		case 2:
		{
			quint16 temp = m_pArch->readWord(m_pData->nAddr);//读2字节数据;
			temp = temp*(m_pData->nFactor)+m_pData->nBase;
			int nPow = qPow(10, m_pData->nDecimal);
			double result = (double)temp / nPow;
			strResult = QString::number(result, 'f', m_pData->nDecimal);
		}
		break;
		case 3:
		{
			qint16 temp = m_pArch->readWord(m_pData->nAddr);//读2字节数据;
			temp = temp*(m_pData->nFactor)+m_pData->nBase;
			int nPow = qPow(10, m_pData->nDecimal);
			double result = (double)temp / nPow;
			strResult = QString::number(result, 'f', m_pData->nDecimal);
		}
		break;
		case 4:
		case 5:
		break;
	}

    return strResult;
}

int SimulateMan::readInt()
{
	int temp;
	switch(m_pData->nDataType)
	{
		//单字节;
	case 0:
	case 1:
		temp = m_pArch->readByte(m_pData->nAddr);
		break;
		//两字节;
	case 2:
	case 3:
		temp = m_pArch->readWord(m_pData->nAddr);//读2字节数据;
		break;
		//三字节;
	case 4:
	case 5:
		break;
		//四字节;
	case 6:
	case 7:
		break;
	}
    temp = temp*(m_pData->nFactor)+m_pData->nBase;
    return temp;
}

void SimulateMan::trigle()
{
    emit(updateData(read(), this, NULL));
    emit(updateData(readInt(), this, NULL));
}

void SimulateMan::addToMonitorList()
{
    if(!m_pData->bWarning)
        return;

    int key = m_pArch->simulateMap.key(this);
    m_pArch->m_pCommunication->addIdToReqBuf(key);
}

void SimulateMan::delFromMonitorList()
{
    if(!m_pData->bWarning)
        return;

    int key = m_pArch->simulateMap.key(this);
    m_pArch->m_pCommunication->delIdFromReqBuf(key);
}


void SimulateMan::sendOut()
{
    quint16 temp = readInt();//读取变量的值;
    quint16 nAddr = m_pData->nAddr;
    QByteArray data("");
    switch (m_pData->nDataType) {
        //1字节;
        case 0:
        case 1:
        {
            //cmd[1], coil[1], addr[2]
            temp==1 ? data.append(CMD_FORCEON) : data.append(CMD_FORCEOFF);//cmd(force_on/force_off);
            if(m_pData->nAddr < M_COIL_OFFSET)//D是字软元件,不应该在此出现;
            {
                 data.clear();
                 break;
            }
            else if(m_pData->nAddr >= M_COIL_OFFSET && m_pData->nAddr < X_COIL_OFFSET)//M字软元件
                data.append(0x92);//M coil
            else if(m_pData->nAddr >= X_COIL_OFFSET && m_pData->nAddr < Y_COIL_OFFSET)//X位软元件
                data.append(0x90);//X coil
            else if(m_pData->nAddr >= Y_COIL_OFFSET && m_pData->nAddr < MOLDSIZE)//Y位软元件
                data.append(0x91);//Y coil
            else
            {
                data.clear();
                break;
            }

            //address
            data.append((nAddr>>8)&0xff);
            data.append(nAddr&0xff);
        }
        break;
        //2字节;
        case 2:
        case 3:
        {
            if(m_pData->nAddr < M_COIL_OFFSET)//D是字软元件
            {
                //cmd[1], coil[1], addr[2], size[1], data[2]
                qint16 addr = nAddr/2;
                data.append(CMD_WRITE);
                data.append(0x96);//D coil
                data.append((addr>>8)&0xff);
                data.append(addr&0xff);
                data.append(1);
                data.append((temp>>8)&0xff);
                data.append(temp&0xff);
            }
        }
        break;
        default:
            break;
    }
    //qDebug()<<"fuck send!!!!!!"<<data.size()<<"addr:"<<m_pData->nAddr<<"type:"<<m_pData->nDataType;
    if(data.size() > 0 )
        m_pArch->m_pCommunication->sendData(data);
}

