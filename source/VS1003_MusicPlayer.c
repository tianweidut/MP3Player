

	//����һ��������ĺ������Բ������� ���� ����

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

extern WORD SectorsPerClust;//ÿ��������
extern WORD FirstDataSector;//��һ������������

extern struct FileInfoStruct FileInfo;//�ļ���Ϣ

struct direntry MusicInfo;//Ҫ���ŵ�mp3�ļ���Ϣ Ŀ¼�ļ���Ϣ32�ֽ�
uint8 totalsongs;//�ܵ������ļ���Ŀ
uint8 type;//�ļ�����

uint16 filehandle=1;//�ж��ļ���״̬

//uint8 buffer[512];

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
	uint16 p;//��ָʾֵ 
	uint16 totalsect;//�ļ�ӵ�е������� 
	uint16 leftbytes;//ʣ���ֽ� 
	uint8 buffer[512];//���� 
	uint16 sector; 
   
   
   	p = FAT16_Open(path);//���ļ��״� 
	totalsect = FileInfo.Size/512; //���������� 
	leftbytes = FileInfo.Size%512; //����ʣ����ֽ��� 

	i=0;
	sector=0;
	while(1)
	{
		for(;i<SectorsPerClust;i++)
		{
        
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
					if(sector == totalsect && count >= leftbytes)return;//�ļ�����
					if(count == 511)break;//512�ֽ�����
				}
			}
			sector++;
		}
		i=0;
		p=FAT16_NextCluster(p);
		if(p == 0xffff)break;
	}
}


void Delay(INT16U n)//��ʱ
{
	while(n--);
}


int main()
{  unsigned long CardSize = 0;
unsigned char Status = 1;
unsigned int TimeOut = 0;
unsigned int itemp = 0;
 INT8U temp;
 uint8 *path; //path��Ҫ��ֵ 

do{

	Status=SD_Initialize();
       TimeOut++;
	}while(Status!=SD_NO_ERR&&TimeOut<100); //�ֶ��� 100 ��ʼ��


	 temp= SD_GetCardInfo();
	CardSize=	sds.block_len*sds.block_num;


//**************************************************

	Delay(0xffff);
	Delay(0xffff);
	Delay(0xffff);
	Delay(0xffff);
	  FAT16_Init();
	  PlayMusic(path);//��һ���ļ� 
	Search("\\",&MusicInfo,&totalsongs,&type);//��������   ���totalsongs ���� 
	//��COUNTΪ��ʱ�������������Ŀ¼���ܹ��ж��������� 
	//��Ϊ��ʱ��MusicInfo���ص�Count�׸����ϸ�ļ���Ϣ 
	while(1)
	{
	//	PlayMusicwithKey();//���Ÿ��� 
	}
}



  
  
  
  
  
  
  
  
  
  /*
 void PlayMusicwithKey()//�������ֺ�����һ��ִ�в����˳�
{
	uint16 keylen;//���ڽ�����
	uint16 count;//���ݼ���
	uint8 i;//ѭ������
	uint16 j;//ѭ������
	uint16 p;//��ָʾֵ
	DWORD totalsect;//�ļ�ӵ�е�������
	uint16 leftbytes;//ʣ���ֽ�
	uint8 buffer[512];//����
	DWORD sector;//����
	uint8 flag=1;//����/��ͣ��־
	uint16 vol=0x2020;//��ʼ������Ӧ��vs1003������ĳ�ʼ������ͬ
	uint8 songs=1;//Ĭ�Ϸŵ�һ�׸�
//	uint8 type;
	if(totalsongs==0)return;//���û�и������쳣�˳�

next://��һ�׸����ʼ�ط�
	count=0;//�����
	while(count<2048 && (type != MID))//recommand 2048 zeros honoring DREQ befor soft reset
	{									//���ڴ�wma��������һ�׸裬��һ�׸�����������//midi��ʽ����Ҫ
//d		if((PINB & _BV(VS1003B_DREQ))!=0)
		{
			for(j=0;j<32;j++)
			{
	//			VS1003B_WriteDAT(0x00);//���0
				count++;
			}
			if(count == 2047)break;
		//	if(sector == totalsect && count >= leftbytes)return;//�ļ�����
		}
	}
//	VS1003B_SoftReset();//soft reset //in case of playing wma files//�����λ
	Search("\\",&MusicInfo,&songs,&type);//�ҵ���Ӧ���ļ�
	p     = MusicInfo.deStartCluster;//���ļ��״�
		
	totalsect = MusicInfo.deFileSize/512; //����������
	leftbytes = MusicInfo.deFileSize%512; //����ʣ����ֽ���
	i=0;
	sector=0;
	
	while(1)
	{
		keylen=0;
    	for(;i<SectorsPerClust;i++)//һ����
		{
//			SPI_High();//�����ʺ�sd���������ٶ�
			FAT16_LoadPartCluster(p,i,buffer);//��һ������
		// 	VS1003B_SPI_High();//�����ʺ�vs1003���ٶ�
			count=0;
			while(count<512)
			{
				if(flag==0){if(keylen){Delay(100);keylen--;}}
				else if(type == MID){if(keylen){Delay(100);keylen--;}}
//d				if((PINB & _BV(VS1003B_DREQ))!=0 && flag)//������Ҫ������
				{
					for(j=0;j<32;j++)//ÿ����32������
					{
		//				VS1003B_WriteDAT(buffer[count]);
						count++;
					}
					if(keylen)keylen--;//���ڼ�����
					if(sector == totalsect && count >= leftbytes)//����ļ��ѽ���
					{
						if(type == MID)//waiting the midi file was decoded
						{//����mid����Ҫ������2048����
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
					}//�ļ�����
					if(count == 511)break;//512�ֽ���������
				}
				//con();
//d				if((PIND&STOP)==0 && keylen==0)//������ͣ��
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
//d				else if(!(PIND&DOWN) && keylen==0)  //����-
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
//d				 else if(!(PIND&UP) && keylen==0) //����+
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
//d				 else if(!(PIND&NEXT)) //��һ��
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
//d				 else if(!(PIND&PREV)) //��һ��
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
		p=FAT16_NextCluster(p);//����һ������
		if(p == 0xffff)
		{songs++;if(songs>totalsongs)songs=1;goto next;}//����޺������������//�����������ִ�е�
	}
}




*/