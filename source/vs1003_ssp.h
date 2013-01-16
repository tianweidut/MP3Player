#include<iolpc2103.h>
#include "config.h"
#ifndef __VS1003_H__
#define __VS1003_H__


//好像没有用上呀 需要删除的
/*
#define 	VS1003_POWER				(0x01 << 15)	
#define 	VS1003_POWER_GPIO()			PINSEL0	&= ~(1<<30)	
#define		VS1003_POWER_OUT()			IODIR 	|= VS1003_POWER			
#define	 	VS1003_POWER_SET()			IOSET 	|= VS1003_POWER			
#define  	VS1003_POWER_CLR()			IOCLR 	|= VS1003_POWER	
//*/		

//p0.07		
#define  	SPI0_CS      				(0x01 << 21)//  24   17         	 	
//#define  	SPI0_CS_GPIO()				PINSEL1 &= ~(0x03 << 16)	 	
#define  	SPI0_CS_OUT()				IODIR |= SPI0_CS;			 	
#define	 	SPI0_CS_SET()				IOSET |= SPI0_CS;				
#define		SPI0_CS_CLR()				IOCLR |= SPI0_CS;				

//p0.06
#define  	SPI0_MOSI					(0x01 << 20)
//#define  	SPI0_MOSI_GPIO()			PINSEL1 &= ~(0x03 << 8)		
#define  	SPI0_MOSI_OUT()				IODIR |= SPI0_MOSI				
#define	 	SPI0_MOSI_CLR()				IOCLR |= SPI0_MOSI				

//p0.05
#define  	SPI0_MISO					(0x01 << 19)						
//#define 	SPI0_MISO_GPIO()			PINSEL1 &= ~(0x03 << 6)		
#define  	SPI0_MISO_OUT()				IODIR |= SPI0_MISO				
#define	 	SPI0_MISO_CLR()				IOCLR |= SPI0_MISO				

//p0.04
#define  	SPI0_SCK					(0x01 << 14)						
//#define  	SPI0_SCK_GPIO()				PINSEL0 &= ~(0x03 << 28)			
#define  	SPI0_SCK_OUT()				IO0DIR |= SPI0_SCK				
#define	 	SPI0_SCK_CLR()				IO0CLR |= SPI0_SCK				

//P1.24 
#define  	VS1003_XDCS					(0x01 << 18)	// 25	 
#//define  	VS1003_XDCS_GPIO()			PINSEL0 &= ~(0x03 <<18)			
#define  	VS1003_XDCS_OUT()			IODIR |= VS1003_XDCS			
#define  	VS1003_XDCS_SET()  			IOSET |= VS1003_XDCS			
#define  	VS1003_XDCS_CLR()  			IOCLR |= VS1003_XDCS			

//p0.03
#define  	VS1003_XRESET				(0x01 << 16)	//27	
#define  	VS1003_XRESET_GPIO()		//PINSEL1 &= ~(0x03 << 22)			
#define  	VS1003_XRESET_OUT()			IODIR |= VS1003_XRESET			
#define  	VS1003_XRESET_SET()  		IOSET |= VS1003_XRESET		
#define  	VS1003_XRESET_CLR()  		IOCLR |= VS1003_XRESET			

//p1.25
#define  	VS1003_DREQ					(0x01 << 15)	//16	
#define  	VS1003_DREQ_GPIO()			//PINSEL1	&= ~(0x03 <<0)			
#define  	VS1003_DREQ_IN()			IODIR  &= ~VS1003_DREQ			
#define  	VS1003_DREQ_STATUS()  		(IOPIN & VS1003_DREQ)		







//vs1003 
#define 	VS1003_WRITE_COM		0x02
#define 	VS1003_READ_COM 		0x03
#define 	VS1003_SPI_MODE        		0x00  
#define 	VS1003_SPI_STATUS     		0x01   
#define		VS1003_SPI_BASS        		0x02   
#define 	VS1003_SPI_CLOCKF      		0x03   
#define 	VS1003_SPI_DECODE_TIME 		0x04   
#define 	VS1003_SPI_AUDATA      		0x05   
#define 	VS1003_SPI_WRAM        		0x06   
#define 	VS1003_SPI_WRAMADDR  	  	0x07   
#define 	VS1003_SPI_HDAT0       		0x08   
#define 	VS1003_SPI_HDAT1       		0x09   
#define 	VS1003_SPI_AIADDR      		0x0a   
#define 	VS1003_SPI_VOL         		0x0b   
#define 	VS1003_SPI_AICTRL0     		0x0c   
#define 	VS1003_SPI_AICTRL1     		0x0d   
#define 	VS1003_SPI_AICTRL2     		0x0e   
#define 	VS1003_SPI_AICTRL3     		0x0f   
#define 	VS1003_SM_DIFF         		0x01   
#define 	VS1003_SM_JUMP         		0x02   
#define 	VS1003_SM_RESET        		0x04   
#define 	VS1003_SM_OUTOFWAV   	 	0x08   
#define 	VS1003_SM_PDOWN        		0x10   
#define 	VS1003_SM_TESTS        		0x20   
#define 	VS1003_SM_STREAM       		0x40   
#define 	VS1003_SM_PLUSV        		0x80   
#define 	VS1003_SM_DACT         		0x100   
#define 	VS1003_SM_SDIORD       		0x200   
#define 	VS1003_SM_SDISHARE     		0x400   
#define 	VS1003_SM_SDINEW       		0x800   
#define 	VS1003_SM_ADPCM        		0x1000   
#define 	VS1003_SM_ADPCM_HP     		0x2000  



extern uint8 VS1003_Init(void);

extern void VS1003_Test(void);

#endif
