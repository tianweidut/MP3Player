
#ifndef __SDCONFIG_H__
#define __SDCONFIG_H__


//different

#define SD_UCOSII_EN			0			/* 是否在UCOS-II上运行本模块 */

#define SD_CRC_EN		   		0		/* 设置数据传输时是否使用CRC */

#define SPI_CLOCK				5529600		/* 正常通信时,SPI时钟频率 frequency (Hz) */

#define SD_BLOCKSIZE 			512			/* SD卡块的长度 */

#define SD_BLOCKSIZE_NBITS		9  

/* 下面函数不常用,如果用户不需要,可置为 0 裁剪指定函数 */

#define SD_ReadMultiBlock_EN    0			/* 是否使能读多块函数 */

#define SD_WriteMultiBlock_EN   0			/* 是否使能写多块函数 */

#define SD_EraseBlock_EN		1			/* 是否使能擦卡函数 */

#define SD_ProgramCSD_EN   		0			/* 是否使能写CSD寄存器函数 */
	
#define SD_ReadCID_EN	   		0			/* 是否使能读CID寄存器函数 */
	
#define	SD_ReadSD_Status_EN		0			/* 是否使能读SD_Status寄存器函数 */

#define	SD_ReadSCR_EN			0			/* 是否使能读SCR寄存器函数 */

/* LPC22xx_EN 硬件配置 */
 
/* SCK引脚 */
#define  SPI_SCK				(0x01 << 4)						
#define  SPI_SCK_GPIO()			PINSEL0 &= ~(0x03 << 8)			/* 设置 SCK 口为GPIO口 */
#define  SPI_SCK_OUT()			IODIR |= SPI_SCK				/* 设置 SCK 口为输出口 */
#define	 SPI_SCK_CLR()			IOCLR = SPI_SCK				/* 置 SCK 为低电平 */	

/* MISO 引脚 */	
#define  SPI_MISO				(0x01 << 5)						
#define  SPI_MISO_GPIO()		PINSEL0 &= ~(0x03 << 10)		/* 设置 MISO 口为GPIO口 */
#define  SPI_MISO_OUT()			IODIR |= SPI_MISO				/* 设置 MISO 口为输出口 */
#define	 SPI_MISO_CLR()			IOCLR = SPI_MISO				/* 置 MISO 为低电平 */

/* MOSI 引脚 */	
#define  SPI_MOSI				(0x01 << 6)
#define  SPI_MOSI_GPIO()		PINSEL0 &= ~(0x03 << 12)		/* 设置 MOSI 口为GPIO口 */
#define  SPI_MOSI_OUT()			IODIR |= SPI_MOSI				/* 设置 MOSI 口为输出口 */
#define	 SPI_MOSI_CLR()			IOCLR = SPI_MOSI				/* 置 MISO 为低电平 */

/* CS 引脚 */		
#define  SPI_CS      			(0x01 << 7)              	 	
#define  SPI_CS_GPIO()			PINSEL0 &= ~(0x03 << 14)		 	/* 设置 CS 口为GPIO口 */
#define  SPI_CS_OUT()			IODIR |= SPI_CS;			 	/* 设置 CS 口为输出口 */
#define	 SPI_CS_SET()			IOSET |= SPI_CS;				/* 置 CS 为高电平 */
#define	 SPI_CS_CLR()			IOCLR |= SPI_CS;				/* 置 CS 为低电平 */

/* 初始化 IO 口为SPI接口 */
#define  SPI_INIT()		PINSEL0 |= 0x00001500;		
//PINSEL0 &= ~((0x03 << 8) + (0x03 << 10) + (0x03 << 12) + (0x03 << 14)); \
//PINSEL0 |= (0x01 << 8) + (0x01 << 10) + (0x01 << 12) + (0x00 << 14);
																


#define  SD_INSERT				(0x01 << 2)		
#define  SD_INSERT_GPIO()		PINSEL0 &= ~(0x03 << 4)			/* 设置 INSERT 口为GPIO口 */	
#define  SD_INSERT_IN()			IODIR &= ~SD_INSERT			/* 设置 INSERT 口为输入口 */	
#define  SD_INSERT_STATUS()  	(IOPIN & SD_INSERT)			/* 读取 INSERT 口的状态 */
                                  //当卡完全插入的时候 卡座输出高电平 否则输出低电平
                                  //即开始检测的是高电平 
/* 卡写保护检测引脚 */
#define  SD_WP					(0x01 << 3)		
#define  SD_WP_GPIO()			PINSEL0 &= ~(0x03 << 6)		/* 设置 WP 口为GPIO口 */	
#define  SD_WP_IN()				IODIR &= ~SD_WP				/* 设置 WP 口为输入口 */	
#define  SD_WP_STATUS()  		(IOPIN & SD_WP)				/* 读取 WP 口的状态 */

#endif
