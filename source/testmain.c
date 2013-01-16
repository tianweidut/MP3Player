

	//测试一下这里面的函数可以不可以用 就行 能用
#include "config.h"
#include "system.h"
//#include<avr/io.h>
//#include"MMC_SD/MMC_SD.h"
#include"FAT16.h"
#include "VS1003_SSP.h"
//#include"VS1003B/VS1003B.h"
#define ENTER_KEY 0x0d
#define BACK_KEY  0x08
#define ESC_KEY   0x1b
#define MAX       64



#define STOP (0x01<<10)
#define NEXT (0x01<<12)
#define UP   (0x01<<13)
#define DOWN (0x01<<11)
#define PREV (0x01<<9)
#define PIND   IOPIN
#define MP3 1
#define WMA 2
#define MID 3

void PlayMusicwithKey();

 WORD FirstDataSector;	// The first sector number of data
 WORD BytesPerSector;	// Bytes per sector 512
 WORD FATsectors;		// The amount sector a FAT occupied
 WORD SectorsPerClust;	// Sector per cluster 
 WORD FirstFATSector;	// The first FAT sector
 WORD FirstDirSector;	// The first Dir sector
 WORD RootDirSectors;	// The sector number a Root dir occupied 32
 WORD RootDirCount;		// The count of directory in root dir 512


 struct FileInfoStruct FileInfo;//文件信息

struct direntry MusicInfo;//要播放的mp3文件信息 目录文件信息32字节
uint8 totalsongs;//总的音乐文件数目
uint8 type;//文件类型

uint16 filehandle=1;//判断文件是状态
unsigned long CardSize = 0;
unsigned char Status = 1;
unsigned int TimeOut = 0;
unsigned int itemp = 0;

unsigned int KEY[5];
//0:UP; 1:DOWN

uint8 buffer[512];

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
	WORD p;//簇指示值 
	uint32 totalsect;//文件拥有的扇区数 
	uint32 leftbytes;//剩余字节 
	//uint8 buffer[512];//缓冲 
	uint16 sector; 
   
   //p必须 大于等于 2
   FAT16_Open(path);
   	//p = FAT16_Open(path);//读文件首簇  得到的是一
	p=FileInfo.StartCluster;


	totalsect = FileInfo.Size/512; //计算扇区数 
	//问题取数也有   右边的和左边的不相同 
	leftbytes = FileInfo.Size%512; //计算剩余的字节数 

	i=0;
	sector=0;
	while(1)
	{
		for(;i<SectorsPerClust;i++)
		{   
            //p 文件开始的首簇 i是每一个簇中的偏移量 读数据到 buffer中
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
					if(sector == totalsect && count >= leftbytes)
						return;//文件结束
					if(count == 511)
						break;//512字节送完
				}
			}
			sector++;//不停的计扇区 
		}
		
		i=0;//开始新的 一个 簇 
		p=FAT16_NextCluster(p);//oxFFF8
		if(p == 0xffff)break;
	}
}


void Delay(INT16U n)//延时
{
	while(n--);
}


int main()
{  
 WORD temp;
 uint8 *path="\\test.mp3"; //path需要赋值 
 

 FreqInit();
 
 IODIR &= ~(UP+DOWN+STOP);
do{

	Status=SD_Initialize();
       TimeOut++;
	}while(Status!=SD_NO_ERR&&TimeOut<100); //现定义 100 初始化


 	//temp= SD_GetCardInfo();
	CardSize=sds.block_len*sds.block_num; //如果是全局变量的话 可以有值呀  


//**************************************************


    	Delay(0xffff);
	  FAT16_Init();
	//  PlayMusic(path);//读一个文件 

	Search("\\",&MusicInfo,&totalsongs,&type);
        
     // for(temp=0;temp=1;temp++);
	//搜索歌曲   如果totalsongs 是零 
	//当COUNT为零时，有它带回这个目录下总共有多少首音乐 
	//不为零时有MusicInfo带回第Count首歌的详细文件信息 
	
		PlayMusicwithKey();//播放歌曲 
}

 void PlayMusicwithKey()//播放音乐函数，一旦执行不会退出
{
	uint16 keylen;//用于建处理
        uint16 ukey=0;
	uint16 count;//数据计数
	uint8 i;//循环变量
        uint8 k;
	uint16 j;//循环变量
	uint16 p;//簇指示值
	DWORD totalsect;//文件拥有的扇区数
	uint16 leftbytes;//剩余字节
	//uint8 buffer[512];//缓冲
	DWORD sector;//扇区
	uint8 flag=1;//播放/暂停标志
	uint16 vol=0x0202;//0x0202;//初始音量，应与vs1003函数里的初始音量相同
	uint8 songs=1;//默认放第一首歌
char namel[13];
uint16 retry = 0;
	if(totalsongs==0)return;//如果没有歌曲则异常退出
        
        VS1003_Init();
        
        LCD_Init();
        
        //while(1)VS1003_Test();
        

next://下一首歌的起始地方
	count=0;//清基数
	while(count<2048 && (type != MID))//recommand 2048 zeros honoring DREQ befor soft reset
	{									//用于从wma跳出到下一首歌，和一首歌结束填充数据//midi格式不需要
	if((IOPIN & VS1003_DREQ)!= 0)
		{
			for(j=0;j<32;j++)
			{
			VS1003_WriteDat(0x00);//填充0
				count++;
			}
			if(count == 2047)break;
		//	if(sector == totalsect && count >= leftbytes)return;//文件结束
		}
	}
       // VS1003_SfRst();//soft reset //in case of playing wma files//软件复位
	Search("\\",&MusicInfo,&songs,&type);//找到相应的文件
	p = MusicInfo.deStartCluster;//读文件首簇
		
	totalsect = MusicInfo.deFileSize/512; //计算扇区数
	leftbytes = MusicInfo.deFileSize%512; //计算剩余的字节数
	i=0;
	sector=0;
        
        
        //添加
        for(k=0;k<8;k++)
     {
       namel[k]=MusicInfo.deName[k];
     }
     namel[k]='.';
    for(k=0;k<3;k++)
      namel[k+9]=MusicInfo.deExtension[k];
    namel[12]='\0';
      
        LCD_WriteString(1,1,namel);
        // LCD_WriteString(1,1,MusicInfo.deExtension);
        
             
		
		//for(i=0;i<3;i++)
           //USART_putchar(MusicInfo.deExtension[i]);
        
        
        
        
	
	while(1)
	{
		keylen=0;
    	for(;i<SectorsPerClust;i++)//一个簇
		{
          //	SPI_High();//调到适合sd卡操作的速度
		



          FAT16_LoadPartCluster(p,i,buffer);//读一个扇区
		

                  // 	VS1003B_SPI_High();//调到适合vs1003的速度
			count=0;
                       
                       
                       while(count<512)
					   {
                               if(flag==0){if(keylen){Delay(100);keylen--;}}
				               else if(type == MID){if(keylen){Delay(100);keylen--;}}
                    if( (IOPIN & VS1003_DREQ)!= 0 && flag ){
				
					for(j=0;j<32;j++)
					{
						VS1003_WriteDat(buffer[count]);
						count++;
					}
                           if(keylen)keylen--;//用于键处理
					if(sector == totalsect && count >= leftbytes)
						{
                              if(type == MID)//waiting the midi file was decoded
							  {//对于mid音乐要接着送2048个零
						    	count=0;
							   while(count<2048)//recommand 2048 zeros honoring DREQ goto next songs
							   {
								   if((IOPIN & VS1003_DREQ)!= 0)
								   {
									  for(j=0;j<32;j++)
									  {
										VS1003_WriteDat(0x00);
										count++;
									  }
								    	if(count == 2047)break;
								   }
							   }
							  }                 
						
						
						
						
						
						                      /*  songs++;
                                                   if(songs>totalsongs)
                                                   songs=1;
                                                    goto next;*/
							  	i=SectorsPerClust;
								break;

                          }//如果无后续簇则结束，//正常情况不会执行到
					if(count == 511) break;
					}
                                        
				if((PIND&STOP)==STOP && keylen==0 )//播放暂停键
			       {
	 			    Delay(100);
					if((PIND&STOP)==STOP)
					{
						//while((PIND&STOP));
						keylen=10000;
						 if(flag)flag=0;
						 else flag=1;
					 }
				 }

				else if((PIND&DOWN)==DOWN )  //音量-
	 			{

				    Delay(100);
	   			 	if((PIND&DOWN)==DOWN)  
					{
					//*while((PIND&DOWN));
					keylen=10000;
					   vol+=0x0101;
					   if(vol>=0xF0F0) vol=0xF0F0; 
                                           VS1003_WriteReg(0x0b,vol); 
                                   LCD_WriteString(1,2,"音量是");                 
                                    LCD_WriteDEC(1,3,vol);
					 }
	 			 }
                           	else  if((PIND&UP)==UP && keylen==0 ) //音量+
	  			 {
					 Delay(50);
				 	 if((PIND&UP)==UP) 
					 {
					 //while((PIND&UP)==UP);
					 keylen=10000;
                                         
					   vol-=0x0101;
                                            
					   if(vol<=0x0202) vol=0x0202; 
                                        VS1003_WriteReg(0x0b,vol); 
                                           
					  
                                     LCD_WriteString(1,2,"音量是");
                                     LCD_WriteDEC(1,3,vol);
                                           
	   			     }
				  }
				else if((PIND&NEXT)==NEXT&& keylen==0) //下一首
	 			 {
                                         Delay(100);
					//Delay(50000);		
					if((PIND&NEXT)==NEXT)
					{
						//while((PIND&NEXT));
					  keylen=10000;
						songs++;
						if(songs > totalsongs)songs=1;
						goto next;
					}
	  			 }
                                /*
				 else if((PIND&PREV)==PREV&& keylen==0) //上一首
	 			 {
                   Delay(100);
					//Delay(50000);		
					if((PIND&PREV)==PREV)
					{
						//while((PIND&PREV));
					    keylen=10000;
						if(songs == 1)songs=totalsongs;
						else songs--;
						goto next;
					}
	  			 }*/
                                        
////////////////////////////////////////////////////////// 
                                        
				
			}
			sector++;
	}
		i=0;
		p=FAT16_NextCluster(p);//读下一簇数据
		if(p == 0xffff)
		  {
                    songs++;
                    if(songs>totalsongs)
                      songs=1;
                    goto next;
          }//如果无后续簇则结束，//正常情况不会执行到
	}
}






