

	//测试一下这里面的函数可以不可以用 就行 能用

//#include<avr/io.h>
//#include"MMC_SD/MMC_SD.h"
#include"FAT16.h"
//#include"VS1003B/VS1003B.h"
#define ENTER_KEY 0x0d
#define BACK_KEY  0x08
#define ESC_KEY   0x1b
#define MAX       64


#define STOP _BV(PD5)
#define NEXT _BV(PD3)
#define UP   _BV(PD6)
#define DOWN _BV(PD2)
#define PREV _BV(PD4)

#define MP3 1
#define WMA 2
#define MID 3

extern WORD SectorsPerClust;//每簇扇区数
extern WORD FirstDataSector;//第一个数据扇区数

extern struct FileInfoStruct FileInfo;//文件信息

struct direntry MusicInfo;//要播放的mp3文件信息 目录文件信息32字节
uint8 totalsongs;//总的音乐文件数目
uint8 type;//文件类型

uint16 filehandle=1;//判断文件是状态

//uint8 buffer[512];

//回车加换行

//将字符串转为长整型
unsigned long convert(uint8 *str)//convert string to unsigned long
{
	uint8 *p=str;
	uint8 i=0;
	unsigned long x;
	unsigned long val;
	while(*p++)
	{
		i++;//计算 有多少个字符
	}
	p=str;
	val=0;
	// mei you yan jiu
	for(x=1;i>0;i--,x*=10)
	{
		val+=(p[i-1]-0x30)*x;//
	}
	return val;
}

//比较俩字符串
uint8 compare(uint8 *str1,uint8 *str2)//compare the two string
{
	uint8 *p,*q;
	p=str1;
	q=str2;
	while(*q)
	{
		if(*p++ != *q++)return 0;
	}
	return 1;
}

void PlayMusic(uint8 *path);//原形声明 

void PlayMusic(uint8 *path)//播放指定路径的音乐，只能在播放完后退出
{
	uint16 count;
	uint8 i;//循环变量 
	uint16 j;//循环变量 
	uint16 p;//簇指示值 
	uint16 totalsect;//文件拥有的扇区数 
	uint16 leftbytes;//剩余字节 
	uint8 buffer[512];//缓冲 
	uint16 sector; 
   
   
   	p = FAT16_Open(path);//读文件首簇 
	totalsect = FileInfo.Size/512; //计算扇区数 
	leftbytes = FileInfo.Size%512; //计算剩余的字节数 

	i=0;
	sector=0;
	while(1)
	{
		for(;i<SectorsPerClust;i++)
		{
        
			FAT16_LoadPartCluster(p,i,buffer);//一个扇区 一个扇区的读数据
//d		 	VS1003B_SPI_High();
			count=0;
			while(count<512)
			{
//d				if(PINB & _BV(VS1003B_DREQ))
				{
					for(j=0;j<32;j++)
					{
//d						VS1003B_WriteDAT(buffer[count]);
						count++;
					}
					if(sector == totalsect && count >= leftbytes)return;//文件结束
					if(count == 511)break;//512字节送完
				}
			}
			sector++;
		}
		i=0;
		p=FAT16_NextCluster(p);
		if(p == 0xffff)break;
	}
}


void Delay(INT16U n)//延时
{
	while(n--);
}


int main()
{  unsigned long CardSize = 0;
unsigned char Status = 1;
unsigned int TimeOut = 0;
unsigned int itemp = 0;
 INT8U temp;
 uint8 *path; //path需要赋值 

do{

	Status=SD_Initialize();
       TimeOut++;
	}while(Status!=SD_NO_ERR&&TimeOut<100); //现定义 100 初始化


	 temp= SD_GetCardInfo();
	CardSize=	sds.block_len*sds.block_num;


//**************************************************

	Delay(0xffff);
	Delay(0xffff);
	Delay(0xffff);
	Delay(0xffff);
	  FAT16_Init();
	  PlayMusic(path);//读一个文件 
	Search("\\",&MusicInfo,&totalsongs,&type);//搜索歌曲   如果totalsongs 是零 
	//当COUNT为零时，有它带回这个目录下总共有多少首音乐 
	//不为零时有MusicInfo带回第Count首歌的详细文件信息 
	while(1)
	{
	//	PlayMusicwithKey();//播放歌曲 
	}
}



  
  
  
  
  
  
  
  
  
  /*
 void PlayMusicwithKey()//播放音乐函数，一旦执行不会退出
{
	uint16 keylen;//用于建处理
	uint16 count;//数据计数
	uint8 i;//循环变量
	uint16 j;//循环变量
	uint16 p;//簇指示值
	DWORD totalsect;//文件拥有的扇区数
	uint16 leftbytes;//剩余字节
	uint8 buffer[512];//缓冲
	DWORD sector;//扇区
	uint8 flag=1;//播放/暂停标志
	uint16 vol=0x2020;//初始音量，应与vs1003函数里的初始音量相同
	uint8 songs=1;//默认放第一首歌
//	uint8 type;
	if(totalsongs==0)return;//如果没有歌曲则异常退出

next://下一首歌的起始地方
	count=0;//清基数
	while(count<2048 && (type != MID))//recommand 2048 zeros honoring DREQ befor soft reset
	{									//用于从wma跳出到下一首歌，和一首歌结束填充数据//midi格式不需要
//d		if((PINB & _BV(VS1003B_DREQ))!=0)
		{
			for(j=0;j<32;j++)
			{
	//			VS1003B_WriteDAT(0x00);//填充0
				count++;
			}
			if(count == 2047)break;
		//	if(sector == totalsect && count >= leftbytes)return;//文件结束
		}
	}
//	VS1003B_SoftReset();//soft reset //in case of playing wma files//软件复位
	Search("\\",&MusicInfo,&songs,&type);//找到相应的文件
	p     = MusicInfo.deStartCluster;//读文件首簇
		
	totalsect = MusicInfo.deFileSize/512; //计算扇区数
	leftbytes = MusicInfo.deFileSize%512; //计算剩余的字节数
	i=0;
	sector=0;
	
	while(1)
	{
		keylen=0;
    	for(;i<SectorsPerClust;i++)//一个簇
		{
//			SPI_High();//调到适合sd卡操作的速度
			FAT16_LoadPartCluster(p,i,buffer);//读一个扇区
		// 	VS1003B_SPI_High();//调到适合vs1003的速度
			count=0;
			while(count<512)
			{
				if(flag==0){if(keylen){Delay(100);keylen--;}}
				else if(type == MID){if(keylen){Delay(100);keylen--;}}
//d				if((PINB & _BV(VS1003B_DREQ))!=0 && flag)//根据需要送数据
				{
					for(j=0;j<32;j++)//每次送32个数据
					{
		//				VS1003B_WriteDAT(buffer[count]);
						count++;
					}
					if(keylen)keylen--;//用于键处理
					if(sector == totalsect && count >= leftbytes)//如果文件已结束
					{
						if(type == MID)//waiting the midi file was decoded
						{//对于mid音乐要接着送2048个零
							count=0;
							while(count<2048)//recommand 2048 zeros honoring DREQ goto next songs
							{
//								if((PINB & _BV(VS1003B_DREQ))!=0 )
								{
									for(j=0;j<32;j++)
									{
								//		VS1003B_WriteDAT(0x00);
										count++;
									}
									if(count == 2047)break;
								}
							}
						}
						i=SectorsPerClust;
						break;
					}//文件结束
					if(count == 511)break;//512字节送完跳出
				}
				//con();
//d				if((PIND&STOP)==0 && keylen==0)//播放暂停键
			    {
	 			    Delay(100);
//					if(!(PIND&STOP))
					{
						//while(!(PIND&STOP));
						keylen=10000;
						 if(flag)flag=0;
						 else flag=1;
					 }
				 }
//d				else if(!(PIND&DOWN) && keylen==0)  //音量-
	 			{
				    Delay(100);
//d	   			 	if(!(PIND&DOWN))  
					{
					//	while(!(PIND&DOWN));
					keylen=10000;
					   vol=vol+((uint8)(2<<8)+2);  //different
					   if(vol>=0xF0F0) vol=0xF0F0; 
//d	   			       else VS1003B_WriteCMD(0x0b,vol);
					 }
	 			 }
//d				 else if(!(PIND&UP) && keylen==0) //音量+
	  			 {
					 Delay(100);
//				 	 if(!(PIND&UP)) 
					 {
					 //	while(!(PIND&UP));
					 keylen=10000;
					   vol=vol-((uint8)(2<<8)+2);//different
					   if(vol<=0x0505) vol=0x0505;
//d					   else VS1003B_WriteCMD(0x0b,vol);
	   			     }
				  }
//d				 else if(!(PIND&NEXT)) //下一首
	 			 {
					Delay(50000);		
//					if(!(PIND&NEXT))
					{
//						while(!(PIND&NEXT));
					//keylen=10000;
						songs++;
						if(songs > totalsongs)songs=1;
						goto next;
					}
	  			 }
//d				 else if(!(PIND&PREV)) //上一首
	 			 {
					Delay(50000);		
//d					if(!(PIND&PREV))
					{
//						while(!(PIND&PREV));
					//keylen=10000;
						if(songs == 1)songs=totalsongs;
						else songs--;
						goto next;
					}
	  			 }
			}
			sector++;
		}
		i=0;
		p=FAT16_NextCluster(p);//读下一簇数据
		if(p == 0xffff)
		{songs++;if(songs>totalsongs)songs=1;goto next;}//如果无后续簇则结束，//正常情况不会执行到
	}
}




*/