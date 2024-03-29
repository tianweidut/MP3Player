#include "config.h"

/* 检测卡是否完全插入  check weather card is insert entirely */
extern INT8U SD_ChkCard(void);


extern INT8U SD_ChkCardWP(void);


/* 初始化访问SD卡的硬件初始化 initialize the hardware that access sd card */
extern void SD_HardWareInit(void);

/* 设置SPI的时钟小于400kHZ set the clock of SPI less than 400kHZ */
extern void SPI_Clk400k(void);

/* 设置SPI的clock到最大值 set the clock of SPI to maximum */
extern void SPI_ClkToMax(void);

/* 通过SPI接口发送一个字节 send a byte by SPI interface */
extern void SPI_SendByte(INT8U byte);

/* 从SPI接口接收一个字节 receive a byte from SPI interface */
extern INT8U SPI_RecByte(void);
extern void SPIRecByte( INT8U* );
/* 片选SPI从机 select the SPI slave */
extern void SPI_CS_Assert(void);

/* 不片选SPI从机 not select the SPI slave */
extern void SPI_CS_Deassert(void);
