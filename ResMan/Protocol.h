#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
//通讯协议相关操作;
#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#include "Device.h"

#define MAX_PROTOCL_BUF_SZ	50

#define STX		0x02
#define ETX		0x03
#define ESC		0x10

#define ENQ		0x05
#define ACK		0x06
#define NAK		0x15



#define CMD_READ		'0'
#define CMD_WRITE		'1'
#define CMD_FORCEON		'7'
#define CMD_FORCEOFF	'8'

#define CMD_PRO_READ	'a'
#define CMD_PRO_WRITE	'b'

#define CMDLEN_COIL		16
#define CMDLEN_PRO		18

#define MONITOR_ADDR    8040
#define MONITORSIZE_ADDR  8100

typedef uint16 (*pComWrite)( quint8* data, uint16 sz );
typedef void (*pTimerOut)( void* pData );
typedef void (*pSetTimer)( int nID, unsigned long time, void* pData, pTimerOut callback );
typedef void (*pSendSuccess)();

typedef struct tagProtocolConf{
	pComWrite write;
	pSetTimer setTimer;
    pSendSuccess sendSuccess;
	int nTimeOutCount;
	unsigned long lTimerout;
}ProtocolConf,*LPProtocolConf;

#define PROTOCOL_CONF_WRITEFUN		0x01
#define PROTOCOL_CONF_SETTIMERFUN	0x02
#define PROTOCOL_CONF_TIMEOUTCOUNT	0x04
#define PROTOCOL_CONF_TIMEOUT		0x08
#define PROTOCOL_CONF_SENDSUCCESSFUN		0x10

int API_Protocol( mbyte* pData, uint16 sz );
void API_InitProtocol( void );
void SetProtocolConf( LPProtocolConf conf, unsigned int uFlag );
int API_ProtocolSend( mbyte* pData, uint16 sz );


#define API_ReadData(x,y,z) 1
int API_WriteData(mbyte coil, uint16 addr, uint16 dataGet);
#define API_WriteBit(x,y,z) 1
#define API_IsPLCRuning() 1
#define API_ProGramRead(x,y) 1
#define API_ProGramWrite(x,y) 1

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __PROTOCOL_H__ */
