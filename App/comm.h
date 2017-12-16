#ifndef __COMM_H
	#define __COMM_H
	

#define	COMMRXDATASIZE	8
#define	COMMTXDATASIZE	8

typedef enum{
 CODE_TEMPER_RH =(byte)0x01,
 CODE_XFMOTODUTY =(byte)0x02,
 CODE_PFMOTODUTY =(byte)0x03,
 CODE_CIRCLEMODE =(byte)0x04,
 CODE_HEATTEMPER =(byte)0x05,
 CODE_VENTILATE =(byte)0x06,
 CODE_IAQ_READ =(byte)0x07,
 CODE_CO2_READ =(byte)0x08,
 CODE_POWER_SET	 =(byte)0x80,
 CODE_BEEP_SETON =(byte)0x7F,
 CODE_VERSION =(byte)0xEE
}OrderCodeTypeDef;

bool CommTalk_Trans(byte order);
bool CommTalk_Echo(byte length);
	
#endif
