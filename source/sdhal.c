
#include "config.h"
#include "sdconfig.h"
#include "sdhal.h"

	/**************************************************************
		
		读写SD卡的SPI接口函数: SPI接口设置,发送/接收字节函数	
	
	**************************************************************/


/*******************************************************************************************************************
** 函数名称: void SD_HardWareInit()				Name:	  void SD_HardWareInit()
** 功能描述: 初始化访问SD卡的硬件条件			Function: initialize the hardware condiction that access sd card
** 输　  入: 无									Input:	  NULL
** 输 　 出: 无									Output:	  NULL
********************************************************************************************************************/
void SD_HardWareInit(void)
{ 
									/* 对卡先下电,再上电  turn off power of card, and turn on it */
	
	SPI_INIT();									/* 初始化SPI接口      initialize SPI interface */	
	
	//SD_INSERT_GPIO();
	//SD_INSERT_IN();								/* 检测卡完全插入口为输入口   the port checking card is input */
	
	//SD_WP_GPIO();								
	//SD_WP_IN();									/* 写保护检测口为输入口  	  the port written protect is input */	
	SPI_CS_OUT();   
   	SPI_CS_SET();								/* CS置高	  				  set CS to high voltage */

 	SPI_ClkToMax();	//different128 							/* 设置SPI频率小于等于400kHZ  set frequency of SPI below 400kHZ */
        S0SPCR= 0x0830;	
 	//S0SPCR = (0x01 << 4) + (0x01 << 5);		/* 设置SPI接口模式，MSTR = 1,CPOL = 1,CPHA = 0,LSBF=0 */
}												/* configure SPI interface */


/*******************************************************************************************************************
** 函数名称: void SPI_Clk400k()					Name:	  void SPI_Clk400k()
** 功能描述: 设置SPI的时钟小于400kHZ			Function: set the clock of SPI less than 400kHZ
** 输　  入: 无									Input:	  NULL
** 输 　 出: 无									Output:	  NULL
********************************************************************************************************************/
void SPI_Clk400k(void)
{
	S0SPCCR = 128;   						/* 设置SPI时钟分频值为128  Set the value of dividing frequency to 128 */
}


/*******************************************************************************************************************
** 函数名称: void SPI_ClkToMax()				Name:	  void SPI_ClkToMax()
** 功能描述: 设置SPI的clock到最大值				Function: set the clock of SPI to maximum
** 输　  入: 无									Input:	  NULL
** 输 　 出: 无									Output:	  NULL
********************************************************************************************************************/
void SPI_ClkToMax(void)
{
	S0SPCCR = 8;								/* 设置SPI时钟分频值为8  Set the value of dividing frequency to 8 */
}


/*******************************************************************************************************************
** 函数名称: void SPI_SendByte()				Name:	  void SPI_SendByte()
** 功能描述: 通过SPI接口发送一个字节			Function: send a byte by SPI interface
** 输　  入: INT8U byte: 发送的字节				Input:	  INT8U byte: the byte that will be send
** 输 　 出: 无									Output:	  NULL
********************************************************************************************************************/
void SPI_SendByte(INT8U byte)
{
	INT8U temp;

	S0SPDR = byte;							/* 发送数据放入SPI数据寄存器 */
   
	while(0 == (S0SPSR & 0x80));				/* 等待SPIF置位，即等待数据发送完毕 */
												/* wait for SPIF being set, that is, wait for finishing of data being send */
 	temp = S0SPDR;
}


/*******************************************************************************************************************
** 函数名称: INT8U SPI_RecByte()				Name:	  INT8U SPI_RecByte()
** 功能描述: 从SPI接口接收一个字节				Function: receive a byte from SPI interface
** 输　  入: 无									Input:	  NULL
** 输 　 出: 收到的字节							Output:	  the byte that be received
********************************************************************************************************************/
INT8U SPI_RecByte(void)
{
INT8U temp;
	S0SPDR = 0xFF;
   
 	while(0 == (S0SPSR & 0x80));				/* 等待SPIF置位，即等待收到数据 */
	temp=S0SPDR;											/* wait for SPIF being set, that is, wait for being received data */
	return(temp); 							/* 读取收到的字节 read the byte received */
}

void SPIRecByte(INT8U* temp)
{

	S0SPDR = 0xFF;
   
 	while(0 == (S0SPSR & 0x80));				/* 等待SPIF置位，即等待收到数据 */
	*temp=S0SPDR;											/* wait for SPIF being set, that is, wait for being received data */
								/* 读取收到的字节 read the byte received */
}


/*******************************************************************************************************************
** 函数名称: void SPI_CS_Assert()				Name:	  void SPI_CS_Assert()
** 功能描述: 片选SPI从机						Function: select the SPI slave 
** 输　  入: 无									Input:	  NULL
** 输 　 出: 无									Output:	  NULL
********************************************************************************************************************/
void SPI_CS_Assert(void)
{
    SPI_CS_OUT();
	SPI_CS_CLR();			   					/* 片选SPI从机  select the SPI slave */  
}


/*******************************************************************************************************************
** 函数名称: void SPI_CS_Deassert()				Name:	  void SPI_CS_Deassert()
** 功能描述: 不片选SPI从机						Function: not select the SPI slave 
** 输　  入: 无									Input:	  NULL
** 输 　 出: 无									Output:	  NULL
********************************************************************************************************************/
void SPI_CS_Deassert(void)
{
SPI_CS_OUT();
	SPI_CS_SET();			    				/* 不片选SPI从机  not select the SPI slave */
}

/*******************************************************************************************************************
** 函数名称: void SD_ChkCard()					Name:	  void SD_ChkCard()
** 功能描述: 检测卡是否完全插入					Function: check weather card is insert entirely
** 输　  入: 无									Input:	  NULL
** 输 　 出: 1: 卡完全插入	0: 卡没有完全插入   Output:	  1: insert entirely	0: not insert entirely
********************************************************************************************************************/
INT8U SD_ChkCard(void)
{
	if (SD_INSERT_STATUS() != 0)
		return 0;								/* 未完全插入 not insert entirely */
	else
		return 1;								/* 完全插入 insert entirely */
}

/*******************************************************************************************************************
** 函数名称: void SD_ChkCardWP()				Name:	  void SD_ChkCardWP()
** 功能描述: 检测卡写保护						Function: check weather card is write protect
** 输　  入: 无									Input:	  NULL
** 输 　 出: 1: 卡已写保护	0: 卡未写保护	    Output:	  1: insert write protect	0: not write protect
********************************************************************************************************************/
INT8U SD_ChkCardWP(void)
{
	if (SD_WP_STATUS() != 0)
		return 1;								/* 卡写保护 */
	else
		return 0;								/* 卡未写保护 */
}








