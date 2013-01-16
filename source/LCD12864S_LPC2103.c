#include <ioLPC2103.h>
//#include"LCD12864S_LPC2103.c" 
#define LCD_CS			0x000000001			//LCD_CS -> TRST/P0.27
#define LCD_CS_DIR		IODIR
#define LCD_CS_HI		IOSET = LCD_CS
#define LCD_CS_LO		IOCLR = LCD_CS

#define LCD_DAT			0x00000002		//LCD_DAT -> TDI/P0.30
#define LCD_DAT_DIR	IODIR
#define LCD_DAT_IN		IOPIN
#define LCD_DAT_HI		IOSET = LCD_DAT
#define LCD_DAT_LO		IOCLR = LCD_DAT

#define LCD_CLK			0x00000004			//LCD_CLK -> TMS/P0.28
#define LCD_CLK_DIR	IODIR
#define LCD_CLK_HI		IOSET = LCD_CLK
#define LCD_CLK_LO		IOCLR = LCD_CLK

#define	LCD_RST			0x00000008		//LCD_RST -> TDO/P0.31
#define LCD_RST_DIR	IODIR
#define LCD_RST_LO		IOCLR = LCD_RST
#define LCD_RST_HI		IOSET = LCD_RST

#define BIT0                (0x0001)
#define BIT1                (0x0002)
#define BIT2                (0x0004)
#define BIT3                (0x0008)
#define BIT4                (0x0010)
#define BIT5                (0x0020)
#define BIT6                (0x0040)
#define BIT7                (0x0080)//1000 0000

void LCD_Init(void);
unsigned char CheckBusy(void);
void LCD_Delayt(unsigned int dtime);
void LCD_WriteComm(unsigned char wdata);
void LCD_WriteByte(unsigned char wdata);
void LCD_SetXY(unsigned char X,unsigned char Y);
void LCD_WriteDEC(unsigned char X,unsigned char Y,unsigned char Num);
void LCD_WriteString(unsigned char X,unsigned char Y,unsigned char *String);

unsigned char *aaa = "这是第一行吗？";















/*


int main(void) {
	
	LCD_Init();
	
	LCD_WriteDEC(1,2,20);
	
	while(1) {
		LCD_WriteString(1,1,aaa);
		LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);
		LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);
		LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);
		LCD_WriteString(1,1,"这是第一行的！");
		LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);
		LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);
		LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);LCD_Delayt(50000);
	}
}
*/
void LCD_Init(void) {  
  	LCD_RST_DIR |= LCD_RST;			//RST -> 复位输出
	LCD_RST_LO;				//复位状态

	LCD_CLK_DIR |= LCD_CLK;
	LCD_CLK_LO;

	LCD_DAT_DIR |= LCD_DAT;
	LCD_DAT_LO;

	LCD_CS_DIR |= LCD_CS;
	LCD_CS_LO;
	
	LCD_RST_HI;
	
	LCD_WriteComm(0x30);			//设定基本指令集
	LCD_WriteComm(0x01);			//清除显示
	LCD_WriteComm(0x02);			//地址归位
	LCD_WriteComm(0x0c);			//开显示状态
	LCD_WriteComm(0x03);
	LCD_WriteComm(0x06);			//进入设定点
}

void LCD_Delayt(unsigned int dtime) {
	unsigned int itemp;
	for(itemp=0;itemp<dtime;itemp++);
}

unsigned char CheckBusy(void) {
	unsigned char itemp;
	unsigned char Read = 0XFC;		//判断是否忙的指令
	unsigned char Read_H,Read_L;

	LCD_CS_LO;LCD_Delayt(100);
	LCD_CLK_LO;LCD_Delayt(100);
	LCD_DAT_LO;LCD_Delayt(100);

	LCD_CS_HI;

	for(itemp=0;itemp<8;itemp++) {
		if((Read<<itemp) & BIT7)
			LCD_DAT_HI;
		else
			LCD_DAT_LO;
		LCD_Delayt(100);
		LCD_CLK_HI;LCD_Delayt(100);
		LCD_CLK_LO;LCD_Delayt(100);
	}
	
	//LCD_DAT_DIR &= ~LCD_DAT;
	for(itemp=0;itemp<8;itemp++) {
		LCD_CLK_LO;LCD_Delayt(100);
		LCD_CLK_HI;LCD_Delayt(100);
		Read<<=1;
		if((LCD_DAT_IN&LCD_DAT) == LCD_DAT)
			Read |= BIT0;
		else
			Read &= ~BIT0;
	}
	Read_H = Read;
	for(itemp=0;itemp<8;itemp++) {
		LCD_CLK_LO;LCD_Delayt(100);
		LCD_CLK_HI;LCD_Delayt(100);
		Read<<=1;
		if((LCD_DAT_IN&LCD_DAT) == LCD_DAT)
			Read |= BIT0;
		else
			Read &= ~BIT0;
	}

	LCD_CS_LO;LCD_Delayt(100);
	
	LCD_DAT_DIR |= LCD_DAT;LCD_Delayt(100);
	LCD_DAT_LO;LCD_Delayt(100);
	
	Read_L = Read>>4;
	Read = Read_H|Read_L;
	return(Read);
}

void LCD_WriteComm(unsigned char wdata) {
	unsigned char itemp;
	unsigned char jtemp=0xf8;
	
	do {
		itemp = CheckBusy();  
	}while(itemp&0x80);
	
	LCD_CS_LO;LCD_Delayt(100);
	LCD_CLK_LO;LCD_Delayt(100);
	LCD_DAT_LO;LCD_Delayt(100);
	
	LCD_CS_HI;LCD_Delayt(100);

	for(itemp=0;itemp<8;itemp++) {
		if((jtemp<<itemp) & BIT7)
			LCD_DAT_HI;
		else
			LCD_DAT_LO;
		LCD_CLK_HI;LCD_Delayt(100);
		LCD_CLK_LO;LCD_Delayt(100);
	}

	jtemp = wdata&0xf0;
	for(itemp=0;itemp<8;itemp++) {
		if((jtemp<<itemp) & BIT7)
			LCD_DAT_HI;
		else
			LCD_DAT_LO;
		LCD_CLK_HI;LCD_Delayt(100);
		LCD_CLK_LO;LCD_Delayt(100);
	}
	
	itemp = wdata&0x0f;
	jtemp = (itemp<<4)&0xf0;
	for(itemp=0;itemp<8;itemp++) {
		if((jtemp<<itemp) & BIT7)
			LCD_DAT_HI;
		else
			LCD_DAT_LO;
		LCD_CLK_HI;LCD_Delayt(100);
		LCD_CLK_LO;LCD_Delayt(100);
	}
	
	LCD_CS_LO;
}

void LCD_WriteByte(unsigned char wdata) {
	unsigned char itemp;
	unsigned char jtemp=0xfa;//1111 1010    ABC  mcu->lcd   1显示数据 
	
	do {
		itemp = CheckBusy();  
	}while(itemp&0x80);//查看 是否是 繁忙
	
	LCD_CS_LO;
	LCD_CLK_LO;
	LCD_DAT_LO;
	
	LCD_CS_HI;

	for(itemp=0;itemp<8;itemp++) {
		if((jtemp<<itemp) & BIT7)//如果与 上 最高位
			LCD_DAT_HI;
		else
			LCD_DAT_LO;
		LCD_CLK_HI;LCD_Delayt(100);
		LCD_CLK_LO;LCD_Delayt(100);
	}

	jtemp = wdata&0xf0;
	for(itemp=0;itemp<8;itemp++) {
		if((jtemp<<itemp) & BIT7)
			LCD_DAT_HI;
		else
			LCD_DAT_LO;
		LCD_CLK_HI;LCD_Delayt(100);
		LCD_CLK_LO;LCD_Delayt(100);
	}
	
	itemp = wdata&0x0f;
	jtemp = (itemp<<4)&0xf0;
	for(itemp=0;itemp<8;itemp++) {
		if((jtemp<<itemp) & BIT7)
			LCD_DAT_HI;
		else
			LCD_DAT_LO;
		LCD_CLK_HI;LCD_Delayt(100);
		LCD_CLK_LO;LCD_Delayt(100);
	}
	LCD_CS_LO;
}

void LCD_SetXY(unsigned char X, unsigned char Y) {
	switch(Y) {
		case 1:
			LCD_WriteComm(0x7F + X);
			break;
		case 2:
			LCD_WriteComm(0x8F + X);
			break;
		case 3:
			LCD_WriteComm(0x87 + X);
			break;
		case 4:
			LCD_WriteComm(0x97 + X);
	}
}

void LCD_WriteString(unsigned char X,unsigned char Y,unsigned char *String) {
	unsigned char temp;
	LCD_SetXY(X,Y);
	temp = *String;
	while(temp!=0) {
		LCD_WriteByte(temp);
		temp = *(++String);
	}  
}

void LCD_WriteDEC(unsigned char X,unsigned char Y,unsigned char Num) {
	LCD_SetXY(X,Y);
	LCD_WriteByte((Num/10) + 0x30);
	LCD_WriteByte((Num%10) + 0x30);
}
