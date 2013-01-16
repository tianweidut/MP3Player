#include"FAT16.h"

extern WORD FirstDataSector;	// The first sector number of data
extern WORD BytesPerSector;	// Bytes per sector 512
extern WORD FATsectors;		// The amount sector a FAT occupied
extern WORD SectorsPerClust;	// Sector per cluster 
extern WORD FirstFATSector;	// The first FAT sector
extern WORD FirstDirSector;	// The first Dir sector
extern WORD RootDirSectors;	// The sector number a Root dir occupied 32
extern WORD RootDirCount;		// The count of directory in root dir 512

 extern struct FileInfoStruct FileInfo;
 extern BYTE buffer[512];
//different 
//����ָ��ָ��sd���Ķ�д����
BYTE  FAT16_ReadSector(DWORD sector, BYTE * buffer)
{
	return (SD_ReadBlock(sector,buffer));
        
}
//MMC_SD_ReadSingleBlock;//device read
BYTE  FAT16_WriteSector(DWORD sector, BYTE * buffer)
{
	return ( SD_WriteBlock(sector,buffer) );
}
//MMC_SD_WriteSingleBlock;//device write

extern struct FileInfoStruct FileInfo;//temporarily buffer for file information
//important
//FAT16��ʼ��������SD�ĳ�ʼ������֮ǰӦ�ȵ���sd�ĳ�ʼ��
unsigned char FAT16_Init()//Initialize of FAT16
{
	struct bootsector50 *bs  = 0;
	struct bpb50        *bpb = 0;
	
	WORD hidsec;
//unsigned   short abc=0;
unsigned   short efg=0;

	BYTE buffer[512];
	//�е�sd��BPB����0����
	//different
	for(hidsec=0;hidsec<0x1000;hidsec++)//Some card the BPB is not locat at secter 0
	{
		//if(FAT16_ReadSector(hidsec,buffer))return 1;// �еĲ�һ��
		FAT16_ReadSector(hidsec,buffer);
		if(buffer[0]==0xEB || buffer[0]==0xE9)break;
	}
	if(hidsec==0x1000)return 1; //error maybe the card hasn't been formated
	bs = (struct bootsector50 *)buffer;//DBR

	if(bs->bsJump[0]!=0xE9 && bs->bsJump[0]!=0xEB)
	return 1;

	bpb = (struct bpb50 *)bs->bsBPB;//�����е� 25 ���ֽ� ��Ϊ bpb
	  BytesPerSector= (WORD)(bpb->bpbBytesPerSec);//ÿ�����ֽ���   512 ���ܸ�ֵ 
	 	   BytesPerSector=512;

	  //= (WORD)512;//(unsigned short)abc;
   //????��һ�� ��Щ���ݶ��Ǽ�
	FATsectors= bpb->bpbFATsecs;//FATռ�õ�������
	SectorsPerClust	= (BYTE)(bpb->bpbSecPerClust);//ÿ��������
	FirstFATSector	= bpb->bpbResSectors+hidsec;//��һ��FAT������
	RootDirCount	= (WORD)bpb->bpbRootDirEnts;//��Ŀ¼����	???????
	//efg  = (WORD)bpb->bpbRootDirEnts;//��Ŀ¼����	???????
	//RootDirCount=512;//efg;

	 efg= (RootDirCount*32)>>9;//��Ŀ¼ռ�õ�������
	 RootDirSectors=32;//efg;
	FirstDirSector	= FirstFATSector+bpb->bpbFATs*FATsectors;//��һ��Ŀ¼����
	
	FirstDataSector	= FirstDirSector+RootDirSectors;//��һ����������
	return 0;
}

//��һ�����е�һ������
//cluster �ǿ�ʼ�Ĵ� part ��ƫ����
unsigned char FAT16_LoadPartCluster(unsigned int cluster,unsigned part,BYTE * buffer)
{
	DWORD sector;
	//���������ݴصĿ�ʼ����
	sector=FirstDataSector+(DWORD)(cluster-2)*(DWORD)SectorsPerClust;//calculate the actual sector number
	 //����ؿ�ʼ������
	if(FAT16_ReadSector(sector+part,buffer))//Ȼ��� һ�������� buffer ��
		return 1;//���� ��ƫ����
	else return 0;
}

//��������
//Read the a cluster
//Not suitable for system which has few data RAM  
unsigned char FAT16_LoadCluster(unsigned int cluster,BYTE * buffer)
{
	DWORD sector;
	unsigned char i;
	sector=FirstDataSector+(DWORD)(cluster-2)*(DWORD)SectorsPerClust;//calculate the actual sector number
	for(i=0;i<SectorsPerClust;i++)
	{
		if(FAT16_ReadSector(sector+i,buffer+(i<<9)))break;//2��9�η� ÿ512 ���ֽ� ���
	}
	if(i==SectorsPerClust)return 0;
	else return 1;
}

//����һ�شغ�
//Return the cluster number of next cluster of file
//Suitable for system which has limited RAM
//���� �� �� �� ��� �ص� ��һ�� ��Ҳ���� ����fat ����Ŀ
unsigned int FAT16_NextCluster(unsigned int cluster)
{
	//BYTE buffer[512];
	DWORD sector;
	DWORD offset=cluster/256;
	//ÿһ��������256�� ���� ����������� fat ����� �ڼ�������
	if(cluster<2)//��С�� �� ���Ǵ����
		return 0xfff8;
	sector=FirstFATSector+offset;//calculate the actual sector
	
        
        if(FAT16_ReadSector(sector,buffer))
		return 0xfff8;//read fat table / return 0xfff8 when error occured

	offset=cluster%256;//�����������ƫ����
	//find the position
	//offset<<=1;
	//sector=buffer[offset+1];
//	sector<<=8;
	//sector+=buffer[offset];
	//���������� Ӧ���������ֽ� 
	sector=((unsigned short *)buffer)[offset];//Ӧ��ע�� ǿ��ת�� �ǲ���ȷ��ѽ 
	return (unsigned short)sector;//return the cluste number
}

//�ڸ���Ŀ¼�²����ļ�
//Find a item in the directory which specify by the parameter "cluster"
//Return the start cluster number
//���ִ������� ����һ������ �в��� BYTE name[]
unsigned int FAT16_FindItem(unsigned int cluster,BYTE * mingzi, struct FileInfoStruct *FileInfo)	//��Ҳ�ǲ����Դ�ֵѽ  name000
{
	BYTE buffer[512];
	DWORD tempclust;
	DWORD sector;
	unsigned char cnt;
	unsigned int offset;
	unsigned char i;
	struct direntry *item = 0;
	BYTE * name=mingzi;
	if(cluster==0)// root directory
	{
		//buffer=malloc(512);//apply memory
		//if(buffer==0)return 1;//if failed
		for(cnt=0;cnt<RootDirSectors;cnt++)//Ŀ¼��ռ��������Ŀ
		{
			FAT16_ReadSector(FirstDirSector+cnt,buffer)	;
			
			for(offset=0;offset<512;offset+=32)
			{
				item=(struct direntry *)(&buffer[offset]);
				if((item->deName[0] != 0x00) & (item->deName[0] != 0xe5) & (item->deAttributes != 0x0f))
				{
					for(i=0;i<11;i++)
					{
						if(buffer[offset+i]!=name[i])break;//ֻҪ�в���ͬ�ľ���ת
					}
					if(i==11)//��������
					{   //�Ȱ� sd ���е����� ����һ�¾��� ����
						//return the parameter of the item
						FileInfo->StartCluster = item->deStartCluster;
						FileInfo->Size         = item->deFileSize;
						FileInfo->Attr         = item->deAttributes;
						FileInfo->Sector       = FirstDirSector+cnt;
						FileInfo->Offset       = offset;
					//	free(buffer);
						return 0;
					}
				}
			}
		}
		//free(buffer);//release
	}


	
	else//other folders
	{
		tempclust=cluster;
		while(1)
		{
			sector=FirstDataSector+(DWORD)(tempclust-2)*(DWORD)SectorsPerClust;//calculate the actual sector number
	//		buffer=malloc(512);//apply memory
			if(buffer==0)return 1;//if failed
			for(cnt=0;cnt<SectorsPerClust;cnt++)
			{
				if(FAT16_ReadSector(sector+cnt,buffer));//{free(buffer);return 1;}
				for(offset=0;offset<512;offset+=32)
				{
					item=(struct direntry *)(&buffer[offset]);
					if((item->deName[0] != 0x00) & (item->deName[0] != 0xe5) & (item->deAttributes != 0x0f))
					{
						for(i=0;i<11;i++)
						{
							if(buffer[offset+i]!=name[i])break;
						}
						if(i==11)
						{
							FileInfo->StartCluster = item->deStartCluster;
							FileInfo->Size         = item->deFileSize;
							FileInfo->Attr         = item->deAttributes;
							FileInfo->Sector       = sector+cnt;
							FileInfo->Offset       = offset;
							//free(buffer);
							return 0;
						}
					}
				}
			}
			//free(buffer);//release
			tempclust=FAT16_NextCluster(tempclust);//next cluster
			if(tempclust == 0xffff || tempclust == 0xfff8)break;
		}
	}
	return 1;
}


// find the location with the given path*************************************
unsigned int FAT16_Open(BYTE * dir)
{
	BYTE name[12];
	BYTE *p=dir;
	BYTE deep=0;
        unsigned int temp;  // different
	BYTE i,j;
	WORD cluster=0;
	if(*p != '\\')
	return 1;//invalid pathĿ¼����ʽ ������ \\aa\\aa
	while(*p)//Ŀ¼�� ����һ��
	{
		if(*p == '\\')
		{
			deep++;//ÿ����һ������ַ� �ͼ�һ ���� �м���
		}
		p++;
	}
	p=dir;
	for(i=0;i<deep-1;i++)
	{
		p++;
		for(j=0;j<12;j++)name[j]=0x20;//��ʼ��Ϊ�ո�
		j=0;
		while(*p != '\\')
		{
			if((*p) >= 'a' && (*p) <= 'z')name[j] = (*p++)-0x20;//��Ϊ��д
			else name[j] = *p++;
			j++;
		}
		if(FAT16_FindItem(cluster,name, &FileInfo))return 1;//find the directory
		cluster = FileInfo.StartCluster;
	}
	
	p++;//����\\
	
	for(j=0;j<12;j++)
	{	
		name[j]= (BYTE)' ';//��ʼ��Ϊ�ո�
	}
	j=0;// ���ļ���
	while(*p != '.')//file must have a extention ���ļ�����չ�����Ǻ�
	{               //����Ķ��Ǵ�д
		if(*p>='a' && *p<='z')name[j]=(*p++)-0x20;
		else name[j]=*p++;
		j++;
	}
	
	j=8;//�� ��չ��
	p++;
	while(*p)
	{
		if(*p>='a' && *p<='z')name[j]=(*p++)-0x20;
		else name[j]=*p++;
		j++;
	}
	//���ִ������� 
       temp= FAT16_FindItem(cluster,&name[0], &FileInfo);
	if(temp)
		return 1;// ??find the file
	cluster = FileInfo.StartCluster;//����Ϣ���� �ṹ�� �����״�
	return cluster;//�����ļ����� �Ĵغ� ����һ��λ��
}

// find a directory with the given path
unsigned int FAT16_OpenDir(BYTE * dir)
{
	BYTE name[11];
	BYTE *p=dir;
	BYTE deep=0;
	BYTE i,j;
	WORD cluster=0;
	if(*p != '\\')return 1;//invalid path
	while(*p)
	{
		if(*p == '\\')
		{
			deep++;
		}
		p++;
	}
	p=dir;
	for(i=0;i<deep-1;i++)
	{
		p++;
		for(j=0;j<11;j++)name[j]=0x20;
		j=0;
		while(*p != '\\')
		{
			if((*p) >= 'a' && (*p) <= 'z')name[j] = (*p++)-0x20;
			else name[j] = *p++;
			j++;
		}
		if(FAT16_FindItem(cluster,name, &FileInfo))return 1;//find the directory
		cluster = FileInfo.StartCluster;
	}
	p++;
	for(j=0;j<11;j++)name[j]=0x20;
	j=0;
	while(*p)
	{
		if(*p>='a' && *p<='z')name[j]=(*p++)-0x20;
		else name[j]=*p++;
		j++;
	}
	if(j == 0)return 0;
	if(FAT16_FindItem(cluster,name, &FileInfo))return 1;//find the final directory
	cluster = FileInfo.StartCluster;
	return cluster;
}

unsigned int FAT16_Close(unsigned int * p)
{
	*p=1;
	return 0;
}


//���Ƽ�¼����Ϣ
void CopyDirentruyItem(struct direntry *Desti,struct direntry *Source)
{
	BYTE i;
	for(i=0;i<8;i++)Desti->deName[i] = Source->deName[i];
	for(i=0;i<3;i++)Desti->deExtension[i] = Source->deExtension[i];
	Desti->deAttributes = Source->deAttributes;
	Desti->deLowerCase = Source->deLowerCase;
	Desti->deCHundredth = Source->deCHundredth;
	for(i=0;i<2;i++)Desti->deCTime[i] = Source->deCTime[i];
	for(i=0;i<2;i++)Desti->deCDate[i] = Source->deCDate[i];
	for(i=0;i<2;i++)Desti->deADate[i] = Source->deADate[i];
	Desti->deHighClust = Source->deHighClust;
	for(i=0;i<2;i++)Desti->deMTime[i] = Source->deMTime[i];
	for(i=0;i<2;i++)Desti->deMDate[i] = Source->deMDate[i];
	Desti->deStartCluster = Source->deStartCluster;
	Desti->deFileSize = Source->deFileSize;
}

// ��ʼ ��\\����Ŀ¼��Ϣ ��  musicinfo��
//tpye ������ 1������ mp3 2���� wma 3����mid 
BYTE Search(BYTE *dir,struct direntry *MusicInfo,BYTE *Count,BYTE *type)
//��COUNTΪ��ʱ�������������Ŀ¼���ܹ��ж���������
{           //��Ϊ��ʱ��MusicInfo���ص�Count�׸����ϸ�ļ���Ϣ
	
	//BYTE buff[3];
	DWORD sector;
	WORD cluster;
	DWORD tempclust;
	unsigned char cnt;
	unsigned int offset;
	unsigned char i=0;
	struct direntry *item = 0;
	//buf[100]=0;
	//for(i=0;i<3;i++)buff[i]=0x20;
/*	i=0;
	while(*ext)
	{
		if(*ext>0x60 && *ext<0x7b)
		buff[i]=*ext - 0x20;
		else buff[i]=*ext;
		ext++;
		i++;
	}
	i=buf[100];*/
	cluster = FAT16_OpenDir(dir);
	if(cluster == 1)
		return 1;
	if(cluster==0)// root directory
	{
		//buffer=malloc(512);//apply memory
		//if(buffer==0)
			//return 1;//if failed

		for(cnt=0;cnt<RootDirSectors;cnt++)
			//Ŀ¼���������Ŀ
		{
			if(FAT16_ReadSector(FirstDirSector+cnt,buffer));//{free(buffer);return 1;}
			for(offset=0;offset<512;offset+=32)
			{
				item=(struct direntry *)(&buffer[offset]);//pointer convert
				//find a valid item and display it
				if((item->deName[0] != '.') & (item->deName[0] != 0x00) & (item->deName[0] != 0xe5) & (item->deAttributes != 0x0f))
				{
					if((item->deExtension[0] == 'M')&&(item->deExtension[1] == 'P')&&(item->deExtension[2] == '3'))
					{
						CopyDirentruyItem(MusicInfo,item);
						*type=1;
						i++;
						if(i==*Count)
                                                  return 0;
					}
					else if((item->deExtension[0] == 'W')&&(item->deExtension[1] == 'M')&&(item->deExtension[2] == 'A'))
					{
						CopyDirentruyItem(MusicInfo,item);
						*type=2;
						i++;
						if(i==*Count)
                                                  return 0;	
					}
					else if((item->deExtension[0] == 'M')&&(item->deExtension[1] == 'I')&&(item->deExtension[2] == 'D'))
					{
						CopyDirentruyItem(MusicInfo,item);
						*type=3;
						i++;
						if(i==*Count)
                                                  return 0;	
					}
				}
			}
		}
		//free(buffer);//release
	}
	else//other folders
	{
		tempclust=cluster;
		while(1)
		{
			sector=FirstDataSector+(DWORD)(tempclust-2)*(DWORD)SectorsPerClust;//calculate the actual sector number
		//	buffer=malloc(512);//apply memory
			if(buffer==0)return 1;//if failed
			for(cnt=0;cnt<SectorsPerClust;cnt++)
			{
				if(FAT16_ReadSector(sector+cnt,buffer));//{free(buffer);return 1;}
				for(offset=0;offset<512;offset+=32)
				{
					item=(struct direntry *)(&buffer[offset]);
					if((item->deName[0] != '.') & (item->deName[0] != 0x00) & (item->deName[0] != 0xe5) & (item->deAttributes != 0x0f))
					{
						if((item->deExtension[0] == 'M')&&(item->deExtension[1] == 'P')&&(item->deExtension[2] == '3'))
						{
							CopyDirentruyItem(MusicInfo,item);
							i++;
							if(i==*Count);//{free(buffer);return 0;}	
						}
						else if((item->deExtension[0] == 'W')&&(item->deExtension[1] == 'M')&&(item->deExtension[2] == 'A'))
						{
							CopyDirentruyItem(MusicInfo,item);
							i++;
							if(i==*Count);//{free(buffer);return 0;}	
						}
						else if((item->deExtension[0] == 'M')&&(item->deExtension[1] == 'I')&&(item->deExtension[2] == 'D'))
						{
							CopyDirentruyItem(MusicInfo,item);
							i++;
							if(i==*Count);//{free(buffer);return 0;}	
						}
					}
				}
			}
			//free(buffer);//release
			tempclust=FAT16_NextCluster(tempclust);//next cluster
			if(tempclust == 0xffff)break;
		}
	}
	if(*Count==0)
          *Count=i;//��������Ŀ
	return 0;	
}
