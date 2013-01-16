

	//����һ��������ĺ������Բ������� ���� ����
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


 struct FileInfoStruct FileInfo;//�ļ���Ϣ

struct direntry MusicInfo;//Ҫ���ŵ�mp3�ļ���Ϣ Ŀ¼�ļ���Ϣ32�ֽ�
uint8 totalsongs;//�ܵ������ļ���Ŀ
uint8 type;//�ļ�����

uint16 filehandle=1;//�ж��ļ���״̬
unsigned long CardSize = 0;
unsigned char Status = 1;
unsigned int TimeOut = 0;
unsigned int itemp = 0;

unsigned int KEY[5];
//0:UP; 1:DOWN

uint8 buffer[512];

//�س��ӻ���

//���ַ���תΪ������
unsigned long convert(uint8 *str)//convert string to unsigned long
{
	uint8 *p=str;
	uint8 i=0;
	unsigned long x;
	unsigned long val;
	while(*p++)
	{
		i++;//���� �ж��ٸ��ַ�
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

//�Ƚ����ַ���
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

void PlayMusic(uint8 *path);//ԭ������ 

void PlayMusic(uint8 *path)//����ָ��·�������֣�ֻ���ڲ�������˳�
{
	uint16 count;
	uint8 i;//ѭ������ 
	uint16 j;//ѭ������ 
	WORD p;//��ָʾֵ 
	uint32 totalsect;//�ļ�ӵ�е������� 
	uint32 leftbytes;//ʣ���ֽ� 
	//uint8 buffer[512];//���� 
	uint16 sector; 
   
   //p���� ���ڵ��� 2
   FAT16_Open(path);
   	//p = FAT16_Open(path);//���ļ��״�  �õ�����һ
	p=FileInfo.StartCluster;


	totalsect = FileInfo.Size/512; //���������� 
	//����ȡ��Ҳ��   �ұߵĺ���ߵĲ���ͬ 
	leftbytes = FileInfo.Size%512; //����ʣ����ֽ��� 

	i=0;
	sector=0;
	while(1)
	{
		for(;i<SectorsPerClust;i++)
		{   
            //p �ļ���ʼ���״� i��ÿһ�����е�ƫ���� �����ݵ� buffer��
			FAT16_LoadPartCluster(p,i,buffer);//һ������ һ�������Ķ�����
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
						return;//�ļ�����
					if(count == 511)
						break;//512�ֽ�����
				}
			}
			sector++;//��ͣ�ļ����� 
		}
		
		i=0;//��ʼ�µ� һ�� �� 
		p=FAT16_NextCluster(p);//oxFFF8
		if(p == 0xffff)break;
	}
}


void Delay(INT16U n)//��ʱ
{
	while(n--);
}


int main()
{  
 WORD temp;
 uint8 *path="\\test.mp3"; //path��Ҫ��ֵ 
 

 FreqInit();
 
 IODIR &= ~(UP+DOWN+STOP);
do{

	Status=SD_Initialize();
       TimeOut++;
	}while(Status!=SD_NO_ERR&&TimeOut<100); //�ֶ��� 100 ��ʼ��


 	//temp= SD_GetCardInfo();
	CardSize=sds.block_len*sds.block_num; //�����ȫ�ֱ����Ļ� ������ֵѽ  


//**************************************************


    	Delay(0xffff);
	  FAT16_Init();
	//  PlayMusic(path);//��һ���ļ� 

	Search("\\",&MusicInfo,&totalsongs,&type);
        
     // for(temp=0;temp=1;temp++);
	//��������   ���totalsongs ���� 
	//��COUNTΪ��ʱ�������������Ŀ¼���ܹ��ж��������� 
	//��Ϊ��ʱ��MusicInfo���ص�Count�׸����ϸ�ļ���Ϣ 
	
		PlayMusicwithKey();//���Ÿ��� 
}

 void PlayMusicwithKey()//�������ֺ�����һ��ִ�в����˳�
{
	uint16 keylen;//���ڽ�����
        uint16 ukey=0;
	uint16 count;//���ݼ���
	uint8 i;//ѭ������
        uint8 k;
	uint16 j;//ѭ������
	uint16 p;//��ָʾֵ
	DWORD totalsect;//�ļ�ӵ�е�������
	uint16 leftbytes;//ʣ���ֽ�
	//uint8 buffer[512];//����
	DWORD sector;//����
	uint8 flag=1;//����/��ͣ��־
	uint16 vol=0x0202;//0x0202;//��ʼ������Ӧ��vs1003������ĳ�ʼ������ͬ
	uint8 songs=1;//Ĭ�Ϸŵ�һ�׸�
char namel[13];
uint16 retry = 0;
	if(totalsongs==0)return;//���û�и������쳣�˳�
        
        VS1003_Init();
        
        LCD_Init();
        
        //while(1)VS1003_Test();
        

next://��һ�׸����ʼ�ط�
	count=0;//�����
	while(count<2048 && (type != MID))//recommand 2048 zeros honoring DREQ befor soft reset
	{									//���ڴ�wma��������һ�׸裬��һ�׸�����������//midi��ʽ����Ҫ
	if((IOPIN & VS1003_DREQ)!= 0)
		{
			for(j=0;j<32;j++)
			{
			VS1003_WriteDat(0x00);//���0
				count++;
			}
			if(count == 2047)break;
		//	if(sector == totalsect && count >= leftbytes)return;//�ļ�����
		}
	}
       // VS1003_SfRst();//soft reset //in case of playing wma files//�����λ
	Search("\\",&MusicInfo,&songs,&type);//�ҵ���Ӧ���ļ�
	p = MusicInfo.deStartCluster;//���ļ��״�
		
	totalsect = MusicInfo.deFileSize/512; //����������
	leftbytes = MusicInfo.deFileSize%512; //����ʣ����ֽ���
	i=0;
	sector=0;
        
        
        //���
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
    	for(;i<SectorsPerClust;i++)//һ����
		{
          //	SPI_High();//�����ʺ�sd���������ٶ�
		



          FAT16_LoadPartCluster(p,i,buffer);//��һ������
		

                  // 	VS1003B_SPI_High();//�����ʺ�vs1003���ٶ�
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
                           if(keylen)keylen--;//���ڼ�����
					if(sector == totalsect && count >= leftbytes)
						{
                              if(type == MID)//waiting the midi file was decoded
							  {//����mid����Ҫ������2048����
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

                          }//����޺������������//�����������ִ�е�
					if(count == 511) break;
					}
                                        
				if((PIND&STOP)==STOP && keylen==0 )//������ͣ��
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

				else if((PIND&DOWN)==DOWN )  //����-
	 			{

				    Delay(100);
	   			 	if((PIND&DOWN)==DOWN)  
					{
					//*while((PIND&DOWN));
					keylen=10000;
					   vol+=0x0101;
					   if(vol>=0xF0F0) vol=0xF0F0; 
                                           VS1003_WriteReg(0x0b,vol); 
                                   LCD_WriteString(1,2,"������");                 
                                    LCD_WriteDEC(1,3,vol);
					 }
	 			 }
                           	else  if((PIND&UP)==UP && keylen==0 ) //����+
	  			 {
					 Delay(50);
				 	 if((PIND&UP)==UP) 
					 {
					 //while((PIND&UP)==UP);
					 keylen=10000;
                                         
					   vol-=0x0101;
                                            
					   if(vol<=0x0202) vol=0x0202; 
                                        VS1003_WriteReg(0x0b,vol); 
                                           
					  
                                     LCD_WriteString(1,2,"������");
                                     LCD_WriteDEC(1,3,vol);
                                           
	   			     }
				  }
				else if((PIND&NEXT)==NEXT&& keylen==0) //��һ��
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
				 else if((PIND&PREV)==PREV&& keylen==0) //��һ��
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
		p=FAT16_NextCluster(p);//����һ������
		if(p == 0xffff)
		  {
                    songs++;
                    if(songs>totalsongs)
                      songs=1;
                    goto next;
          }//����޺������������//�����������ִ�е�
	}
}






