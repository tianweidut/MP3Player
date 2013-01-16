/****************************************Copyright (c)**************************************************
**                               Guangzhou ZLG-MCU Development Co.,LTD.
**                                      graduate school
**                                 http://www.zlgmcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			sdtools.c
** Last modified Date:	2005-1-6
** Last Version:		V1.0
** Descriptions:		sd 卡驱动软件包: SD卡相关工具函数 ---- CRC校验头文件
**						Soft Packet of SD Card Driver: tool funcitons about sd card ---- crc check header file
**
**------------------------------------------------------------------------------------------------------
** Created by:			Ming Yuan Zheng
** Created date:		2005-1-6
** Version:				V1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:				
** Descriptions:		
**
**------------------------------------------------------------------------------------------------------
** Modified by: 
** Modified date:
** Version:	
** Descriptions: 
**
********************************************************************************************************/

extern INT16U SD_GetCRC16(INT8U *pSource,INT16U len);	//得到len个字节的CRC16

extern INT8U SD_GetCmdByte6(INT8U cmd,INT8U *param);	//得到SD命令的第6个字节: CRC7 + 停止位1

extern INT8U SD_GetCRC7(INT8U *pSource,INT16U len);		//得到len个字节的CRC7
