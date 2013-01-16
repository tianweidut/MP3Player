
#include "config.h"
#include "sdconfig.h"
#include "sdhal.h"

	/**************************************************************
		
		��дSD����SPI�ӿں���: SPI�ӿ�����,����/�����ֽں���	
	
	**************************************************************/


/*******************************************************************************************************************
** ��������: void SD_HardWareInit()				Name:	  void SD_HardWareInit()
** ��������: ��ʼ������SD����Ӳ������			Function: initialize the hardware condiction that access sd card
** �䡡  ��: ��									Input:	  NULL
** �� �� ��: ��									Output:	  NULL
********************************************************************************************************************/
void SD_HardWareInit(void)
{ 
									/* �Կ����µ�,���ϵ�  turn off power of card, and turn on it */
	
	SPI_INIT();									/* ��ʼ��SPI�ӿ�      initialize SPI interface */	
	
	//SD_INSERT_GPIO();
	//SD_INSERT_IN();								/* ��⿨��ȫ�����Ϊ�����   the port checking card is input */
	
	//SD_WP_GPIO();								
	//SD_WP_IN();									/* д��������Ϊ�����  	  the port written protect is input */	
	SPI_CS_OUT();   
   	SPI_CS_SET();								/* CS�ø�	  				  set CS to high voltage */

 	SPI_ClkToMax();	//different128 							/* ����SPIƵ��С�ڵ���400kHZ  set frequency of SPI below 400kHZ */
        S0SPCR= 0x0830;	
 	//S0SPCR = (0x01 << 4) + (0x01 << 5);		/* ����SPI�ӿ�ģʽ��MSTR = 1,CPOL = 1,CPHA = 0,LSBF=0 */
}												/* configure SPI interface */


/*******************************************************************************************************************
** ��������: void SPI_Clk400k()					Name:	  void SPI_Clk400k()
** ��������: ����SPI��ʱ��С��400kHZ			Function: set the clock of SPI less than 400kHZ
** �䡡  ��: ��									Input:	  NULL
** �� �� ��: ��									Output:	  NULL
********************************************************************************************************************/
void SPI_Clk400k(void)
{
	S0SPCCR = 128;   						/* ����SPIʱ�ӷ�ƵֵΪ128  Set the value of dividing frequency to 128 */
}


/*******************************************************************************************************************
** ��������: void SPI_ClkToMax()				Name:	  void SPI_ClkToMax()
** ��������: ����SPI��clock�����ֵ				Function: set the clock of SPI to maximum
** �䡡  ��: ��									Input:	  NULL
** �� �� ��: ��									Output:	  NULL
********************************************************************************************************************/
void SPI_ClkToMax(void)
{
	S0SPCCR = 8;								/* ����SPIʱ�ӷ�ƵֵΪ8  Set the value of dividing frequency to 8 */
}


/*******************************************************************************************************************
** ��������: void SPI_SendByte()				Name:	  void SPI_SendByte()
** ��������: ͨ��SPI�ӿڷ���һ���ֽ�			Function: send a byte by SPI interface
** �䡡  ��: INT8U byte: ���͵��ֽ�				Input:	  INT8U byte: the byte that will be send
** �� �� ��: ��									Output:	  NULL
********************************************************************************************************************/
void SPI_SendByte(INT8U byte)
{
	INT8U temp;

	S0SPDR = byte;							/* �������ݷ���SPI���ݼĴ��� */
   
	while(0 == (S0SPSR & 0x80));				/* �ȴ�SPIF��λ�����ȴ����ݷ������ */
												/* wait for SPIF being set, that is, wait for finishing of data being send */
 	temp = S0SPDR;
}


/*******************************************************************************************************************
** ��������: INT8U SPI_RecByte()				Name:	  INT8U SPI_RecByte()
** ��������: ��SPI�ӿڽ���һ���ֽ�				Function: receive a byte from SPI interface
** �䡡  ��: ��									Input:	  NULL
** �� �� ��: �յ����ֽ�							Output:	  the byte that be received
********************************************************************************************************************/
INT8U SPI_RecByte(void)
{
INT8U temp;
	S0SPDR = 0xFF;
   
 	while(0 == (S0SPSR & 0x80));				/* �ȴ�SPIF��λ�����ȴ��յ����� */
	temp=S0SPDR;											/* wait for SPIF being set, that is, wait for being received data */
	return(temp); 							/* ��ȡ�յ����ֽ� read the byte received */
}

void SPIRecByte(INT8U* temp)
{

	S0SPDR = 0xFF;
   
 	while(0 == (S0SPSR & 0x80));				/* �ȴ�SPIF��λ�����ȴ��յ����� */
	*temp=S0SPDR;											/* wait for SPIF being set, that is, wait for being received data */
								/* ��ȡ�յ����ֽ� read the byte received */
}


/*******************************************************************************************************************
** ��������: void SPI_CS_Assert()				Name:	  void SPI_CS_Assert()
** ��������: ƬѡSPI�ӻ�						Function: select the SPI slave 
** �䡡  ��: ��									Input:	  NULL
** �� �� ��: ��									Output:	  NULL
********************************************************************************************************************/
void SPI_CS_Assert(void)
{
    SPI_CS_OUT();
	SPI_CS_CLR();			   					/* ƬѡSPI�ӻ�  select the SPI slave */  
}


/*******************************************************************************************************************
** ��������: void SPI_CS_Deassert()				Name:	  void SPI_CS_Deassert()
** ��������: ��ƬѡSPI�ӻ�						Function: not select the SPI slave 
** �䡡  ��: ��									Input:	  NULL
** �� �� ��: ��									Output:	  NULL
********************************************************************************************************************/
void SPI_CS_Deassert(void)
{
SPI_CS_OUT();
	SPI_CS_SET();			    				/* ��ƬѡSPI�ӻ�  not select the SPI slave */
}

/*******************************************************************************************************************
** ��������: void SD_ChkCard()					Name:	  void SD_ChkCard()
** ��������: ��⿨�Ƿ���ȫ����					Function: check weather card is insert entirely
** �䡡  ��: ��									Input:	  NULL
** �� �� ��: 1: ����ȫ����	0: ��û����ȫ����   Output:	  1: insert entirely	0: not insert entirely
********************************************************************************************************************/
INT8U SD_ChkCard(void)
{
	if (SD_INSERT_STATUS() != 0)
		return 0;								/* δ��ȫ���� not insert entirely */
	else
		return 1;								/* ��ȫ���� insert entirely */
}

/*******************************************************************************************************************
** ��������: void SD_ChkCardWP()				Name:	  void SD_ChkCardWP()
** ��������: ��⿨д����						Function: check weather card is write protect
** �䡡  ��: ��									Input:	  NULL
** �� �� ��: 1: ����д����	0: ��δд����	    Output:	  1: insert write protect	0: not write protect
********************************************************************************************************************/
INT8U SD_ChkCardWP(void)
{
	if (SD_WP_STATUS() != 0)
		return 1;								/* ��д���� */
	else
		return 0;								/* ��δд���� */
}








