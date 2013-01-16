#include "vs1003_ssp.h"


// 延迟函数没有 问题的
void VS1003_Delay_Nmicrosecond(uint32 t)
{
	uint32 i;
	for(i=0;i<=t;i++);
}

/**************************************************************************************************
**Name:	  	void SPI0_SrByte(INT8U byte)
**Function:	send a byte by SPI0 interface
**Input:	INT8U byte: the byte that will be send
**Output:	NULL
**************************************************************************************************/
uint8 SPI0_SrByte(INT8U byte)
{
  unsigned char Dummy;
  /* as long as TNF bit is set, TxFIFO is not full, I can write */
while ( !(SSPSR & 0x02) );
SSPDR = byte;

while ( !(SSPSR & 0x04) );
Dummy = SSPDR; /* Flush the RxFIFO */

//	while(0 == (SSPSR & 0x01));						    /* wait for SPI0F being set, that is, wait for finishing of data being send */
	
	return(Dummy);
}
/**************************************************************************************************
**Name:		VS1003_WriteReg(uint8 addr, uint16 dat)
**Function:	write reg address and data
**Input:	addr		dat 
**Output:
**************************************************************************************************/
void VS1003_WriteReg(uint8 addr,uint16 dat)
{
	VS1003_XDCS_SET();								/*write reg of the vs1003*/
	SPI0_CS_CLR();									/*slect the sleve machine*/
	
	SPI0_SrByte(VS1003_WRITE_COM);	//0x02							
	
	SPI0_SrByte(addr);
	
	SPI0_SrByte((uint8)(dat>>8));
	SPI0_SrByte((uint8)(dat));
	
	SPI0_CS_SET();									/*free the sleve machine*/
}
/**************************************************************************************************
**Name: 	uint16 VS1003_ReadReg(uint16 addr)
**Function:	read reg address and data
**Input:	addr
**Output:	dat
**************************************************************************************************/
uint16 VS1003_ReadReg(uint8 addr)
{
	uint16 dat = 0x0000;
	uint16 datL = 0x0000;
		
	VS1003_XDCS_SET();
	SPI0_CS_CLR();								

	SPI0_SrByte(VS1003_READ_COM);
	SPI0_SrByte(addr);

	dat	= SPI0_SrByte(0xff);
	dat	= dat<<8;
	datL = SPI0_SrByte(0xff);
	dat	=dat|datL;
	
	SPI0_CS_SET();
	
	return dat;
}
/**************************************************************************************************
**Name: 	void VS1003_WriteDat(unsigned char dat)
**Function:	write music dat
**Input:	dat
**Output:	NULL
**************************************************************************************************/	
void VS1003_WriteDat(uint8 dat)
{
	VS1003_XDCS_CLR();
	
	SPI0_CS_CLR();

	SPI0_SrByte(dat);
	
	SPI0_CS_SET();
}
/**************************************************************************************************
**Name:	  	  void VS1003_HardWareInit()
**Function:   initialize the hardware condiction that access sd card
**Input:	  NULL
**Output:	  NULL
**************************************************************************************************/
void SPI0_Init(uint8 speed)
{ unsigned char Dummy,i;
 
	
	//SPI0_CS_GPIO();
	SPI0_CS_OUT();

	//VS1003_XDCS_GPIO();
	VS1003_XDCS_OUT();

	//VS1003_XRESET_GPIO();
	VS1003_XRESET_OUT();

	//VS1003_DREQ_GPIO();
	VS1003_DREQ_IN();
				
PINSEL0_bit.P0_14 = 2; //SSP1 - SCK1
PINSEL1_bit.P0_19 = 1; //SSP1 - MISO1
PINSEL1_bit.P0_20 = 1; //SSP1 - MOSI1

SSPCR1 = 0x00;
SSPCR0 = 0x0707;
SSPCPSR = 0x2;
SSPCR1 = 0x02;

for ( i = 0; i < 8; i++ )
{
Dummy = SSPDR;
}

}
/**************************************************************************************************
**Name: 	  void VS1003_HardWareInit()
**Function:   initialize the hardware condiction that access sd card
**Input:	  NULL
**Output:	  NULL
**************************************************************************************************/
void VS1003_HwRst(void)
{
	VS1003_XRESET_CLR();  						
	//delay		
        //VS1003_Delay_Nmicrosecond(99999);/*delay tiem >1.35ms*/
	VS1003_XRESET_SET();  	
       // VS1003_Delay_Nmicrosecond(99999);
	//VS1003_SoftWareInit();
}
/**************************************************************************************************
**Name: 	  void VS1003_HardWareInit()
**Function:   initialize the hardware condiction that access sd card
**Input:	  NULL
**Output:	  NULL
**************************************************************************************************/
void VS1003_SfRst(void)
{
	VS1003_WriteReg(0x00,0x0804);								//写复位

	while(VS1003_DREQ_STATUS() != VS1003_DREQ);					//等待软件复位结束

	VS1003_WriteReg(VS1003_SPI_CLOCKF,0x9000);					//设置时钟3倍频
	VS1003_WriteReg(VS1003_SPI_AUDATA,0xBB81);					//采样率48k，立体声
	VS1003_WriteReg(VS1003_SPI_BASS,0x0055);					//设置重音
	VS1003_WriteReg(0xb,2020);						//设置音量
	
	VS1003_WriteDat(0);											//向vs1003发送四个字节
	VS1003_WriteDat(0);											//无效数据，启动spi
	VS1003_WriteDat(0);
	VS1003_WriteDat(0);
}
/**************************************************************************************************
**Name: 	uint8 VS1003_Init()
**Function:	vs1003_Init
**Input:	dat
**Output:	sucess :1 FALSE:0
**************************************************************************************************/	
uint8 VS1003_Init(void)
{
  //xiu gai
  uint16 trmp=1;
	uint8 retry = 0;

	SPI0_Init(8);
	
	SPI0_CS_SET();
	VS1003_XDCS_SET();
	VS1003_XRESET_CLR();
	VS1003_Delay_Nmicrosecond(99999);
	VS1003_XRESET_SET();											  //使能芯片
    VS1003_Delay_Nmicrosecond(99999);

	while(VS1003_ReadReg(0x00) != 0x0800)				  			//写mode寄存器
	{
		VS1003_WriteReg(0x00,0x0800);
		if(retry++ >10 )
		{
			retry = 0;
		}
		else
		{
			break;											
		}
	}
	
	while(VS1003_ReadReg(0x03) != 0x9800)							//9800				//写时钟寄存器
	{
		VS1003_WriteReg(0x03,0x9800);
		if(retry++ >10 )
		{
			retry = 0;
		}
		else
		{
			break;
		}
	}
	while(VS1003_ReadReg(0x05 ) != 0xB199)						//写时钟寄存器
	{
		VS1003_WriteReg(0x05,0xB199);
		if(retry++ >10 )
		{
			retry = 0;
		}
		else
		{
			break;
		}
		VS1003_Delay_Nmicrosecond(99999);
	}
	while(VS1003_ReadReg(0x02) != 0x0055)					//写时钟寄存器
	{
		VS1003_WriteReg(0x02,0x0055);
		if(retry++ >10 )
		{
			retry = 0;
		}
		else
		{
			break;
		}
	}

	while((trmp=VS1003_ReadReg(0x0b)) != 0x2020)					//设音量
	{
		VS1003_WriteReg(0x0b,0x2020);
		if(retry++ >10 )
			retry = 0;
                else
			break;
	}
	
    
	if(retry > 10)
	{
           return trmp;
		return 0;
	}
	else
	{
		return 1;
	}
}
/**************************************************************************************************
**Name: 	
**Function:	
**Input:	
**Output:	
**************************************************************************************************/	
void VS1003_Test(void) 
{
	VS1003_HwRst(); 					
        VS1003_Delay_Nmicrosecond(1000);
	SPI0_SrByte(0xff);							//发送一个字节的无效数据，启动SPI传输

	SPI0_CS_SET(); 
	VS1003_XDCS_SET(); 
	VS1003_XRESET_SET();
												//delay100ms					  
	VS1003_WriteReg(VS1003_SPI_MODE,0x0820);	//enter vs1003 test model
	
	while ((IOPIN & VS1003_DREQ)== 0);	   		//wait for DREQ Highlevel
	
	 //向vs1003发送正弦测试命令：0x53 0xef 0x6e n 0x00 0x00 0x00 0x00
	 //其中n = 0x24, 设定vs1003所产生的正弦波的频率值，具体计算方法见vs1003的datasheet
        VS1003_XDCS_CLR();
	VS1003_WriteDat(0x53);	  
	VS1003_WriteDat(0xef);	  
	VS1003_WriteDat(0x6e);	  
	VS1003_WriteDat(0x7e);	  
	VS1003_WriteDat(0x00);	  
	VS1003_WriteDat(0x00);
	VS1003_WriteDat(0x00);
    VS1003_WriteDat(0x00);
    VS1003_Delay_Nmicrosecond(1000);
    VS1003_XDCS_SET();
  
  	VS1003_Delay_Nmicrosecond(50000);
	VS1003_Delay_Nmicrosecond(50000);
	VS1003_Delay_Nmicrosecond(50000);
	VS1003_Delay_Nmicrosecond(50000);
	//程序执行到这里后应该能从耳机听到一个单一频率的声音
	 
	 //退出正弦测试
        VS1003_XDCS_CLR();
	VS1003_WriteDat(0x45);
    VS1003_WriteDat(0x78);
	VS1003_WriteDat(0x69);
	VS1003_WriteDat(0x74);
	VS1003_WriteDat(0x00);
	VS1003_WriteDat(0x00);
	VS1003_WriteDat(0x00);
	VS1003_WriteDat(0x00);
	VS1003_Delay_Nmicrosecond(50000);
	VS1003_Delay_Nmicrosecond(50000);
	VS1003_Delay_Nmicrosecond(50000);
	VS1003_Delay_Nmicrosecond(50000);
	VS1003_XDCS_SET();
	
	//再次进入正弦测试并设置n值为0x44，即将正弦波的频率设置为另外的值
        VS1003_XDCS_CLR();
	VS1003_WriteDat(0x53);	  
	VS1003_WriteDat(0xef);	  
	VS1003_WriteDat(0x6e);	  
	VS1003_WriteDat(0x44);	  
	VS1003_WriteDat(0x00);	  
	VS1003_WriteDat(0x00);
	VS1003_WriteDat(0x00);
	VS1003_WriteDat(0x00);
	VS1003_Delay_Nmicrosecond(50000);
	VS1003_Delay_Nmicrosecond(50000);
	VS1003_Delay_Nmicrosecond(50000);
	VS1003_Delay_Nmicrosecond(50000);
        VS1003_XDCS_SET();
	
	//退出正弦测试
        VS1003_XDCS_CLR();
	VS1003_WriteDat(0x45);
	VS1003_WriteDat(0x78);
	VS1003_WriteDat(0x69);
	VS1003_WriteDat(0x74);
	VS1003_WriteDat(0x00);
	VS1003_WriteDat(0x00);
 	VS1003_WriteDat(0x00);
	VS1003_WriteDat(0x00);
	VS1003_Delay_Nmicrosecond(50000);
	VS1003_Delay_Nmicrosecond(50000);
	VS1003_Delay_Nmicrosecond(50000);
	VS1003_Delay_Nmicrosecond(50000);
	VS1003_Delay_Nmicrosecond(50000);
}   


/*****************for test*******************
int main()
{	  
  VS1003_Init() ;
  while(1){
  //
   // SPI0_SrByte(0xAA);
   VS1003_Test() ;
 }
}
*******************************************/