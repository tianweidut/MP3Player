
#ifndef __SDCONFIG_H__
#define __SDCONFIG_H__


//different

#define SD_UCOSII_EN			0			/* �Ƿ���UCOS-II�����б�ģ�� */

#define SD_CRC_EN		   		0		/* �������ݴ���ʱ�Ƿ�ʹ��CRC */

#define SPI_CLOCK				5529600		/* ����ͨ��ʱ,SPIʱ��Ƶ�� frequency (Hz) */

#define SD_BLOCKSIZE 			512			/* SD����ĳ��� */

#define SD_BLOCKSIZE_NBITS		9  

/* ���溯��������,����û�����Ҫ,����Ϊ 0 �ü�ָ������ */

#define SD_ReadMultiBlock_EN    0			/* �Ƿ�ʹ�ܶ���麯�� */

#define SD_WriteMultiBlock_EN   0			/* �Ƿ�ʹ��д��麯�� */

#define SD_EraseBlock_EN		1			/* �Ƿ�ʹ�ܲ������� */

#define SD_ProgramCSD_EN   		0			/* �Ƿ�ʹ��дCSD�Ĵ������� */
	
#define SD_ReadCID_EN	   		0			/* �Ƿ�ʹ�ܶ�CID�Ĵ������� */
	
#define	SD_ReadSD_Status_EN		0			/* �Ƿ�ʹ�ܶ�SD_Status�Ĵ������� */

#define	SD_ReadSCR_EN			0			/* �Ƿ�ʹ�ܶ�SCR�Ĵ������� */

/* LPC22xx_EN Ӳ������ */
 
/* SCK���� */
#define  SPI_SCK				(0x01 << 4)						
#define  SPI_SCK_GPIO()			PINSEL0 &= ~(0x03 << 8)			/* ���� SCK ��ΪGPIO�� */
#define  SPI_SCK_OUT()			IODIR |= SPI_SCK				/* ���� SCK ��Ϊ����� */
#define	 SPI_SCK_CLR()			IOCLR = SPI_SCK				/* �� SCK Ϊ�͵�ƽ */	

/* MISO ���� */	
#define  SPI_MISO				(0x01 << 5)						
#define  SPI_MISO_GPIO()		PINSEL0 &= ~(0x03 << 10)		/* ���� MISO ��ΪGPIO�� */
#define  SPI_MISO_OUT()			IODIR |= SPI_MISO				/* ���� MISO ��Ϊ����� */
#define	 SPI_MISO_CLR()			IOCLR = SPI_MISO				/* �� MISO Ϊ�͵�ƽ */

/* MOSI ���� */	
#define  SPI_MOSI				(0x01 << 6)
#define  SPI_MOSI_GPIO()		PINSEL0 &= ~(0x03 << 12)		/* ���� MOSI ��ΪGPIO�� */
#define  SPI_MOSI_OUT()			IODIR |= SPI_MOSI				/* ���� MOSI ��Ϊ����� */
#define	 SPI_MOSI_CLR()			IOCLR = SPI_MOSI				/* �� MISO Ϊ�͵�ƽ */

/* CS ���� */		
#define  SPI_CS      			(0x01 << 7)              	 	
#define  SPI_CS_GPIO()			PINSEL0 &= ~(0x03 << 14)		 	/* ���� CS ��ΪGPIO�� */
#define  SPI_CS_OUT()			IODIR |= SPI_CS;			 	/* ���� CS ��Ϊ����� */
#define	 SPI_CS_SET()			IOSET |= SPI_CS;				/* �� CS Ϊ�ߵ�ƽ */
#define	 SPI_CS_CLR()			IOCLR |= SPI_CS;				/* �� CS Ϊ�͵�ƽ */

/* ��ʼ�� IO ��ΪSPI�ӿ� */
#define  SPI_INIT()		PINSEL0 |= 0x00001500;		
//PINSEL0 &= ~((0x03 << 8) + (0x03 << 10) + (0x03 << 12) + (0x03 << 14)); \
//PINSEL0 |= (0x01 << 8) + (0x01 << 10) + (0x01 << 12) + (0x00 << 14);
																


#define  SD_INSERT				(0x01 << 2)		
#define  SD_INSERT_GPIO()		PINSEL0 &= ~(0x03 << 4)			/* ���� INSERT ��ΪGPIO�� */	
#define  SD_INSERT_IN()			IODIR &= ~SD_INSERT			/* ���� INSERT ��Ϊ����� */	
#define  SD_INSERT_STATUS()  	(IOPIN & SD_INSERT)			/* ��ȡ INSERT �ڵ�״̬ */
                                  //������ȫ�����ʱ�� ��������ߵ�ƽ ��������͵�ƽ
                                  //����ʼ�����Ǹߵ�ƽ 
/* ��д����������� */
#define  SD_WP					(0x01 << 3)		
#define  SD_WP_GPIO()			PINSEL0 &= ~(0x03 << 6)		/* ���� WP ��ΪGPIO�� */	
#define  SD_WP_IN()				IODIR &= ~SD_WP				/* ���� WP ��Ϊ����� */	
#define  SD_WP_STATUS()  		(IOPIN & SD_WP)				/* ��ȡ WP �ڵ�״̬ */

#endif
