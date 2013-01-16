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
//函数指针指向sd卡的读写函数
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
//FAT16初始化，不含SD的初始化，用之前应先调用sd的初始化
unsigned char FAT16_Init()//Initialize of FAT16
{
	struct bootsector50 *bs  = 0;
	struct bpb50        *bpb = 0;
	
	WORD hidsec;
//unsigned   short abc=0;
unsigned   short efg=0;

	BYTE buffer[512];
	//有的sd卡BPB不在0扇区
	//different
	for(hidsec=0;hidsec<0x1000;hidsec++)//Some card the BPB is not locat at secter 0
	{
		//if(FAT16_ReadSector(hidsec,buffer))return 1;// 有的不一样
		FAT16_ReadSector(hidsec,buffer);
		if(buffer[0]==0xEB || buffer[0]==0xE9)break;
	}
	if(hidsec==0x1000)return 1; //error maybe the card hasn't been formated
	bs = (struct bootsector50 *)buffer;//DBR

	if(bs->bsJump[0]!=0xE9 && bs->bsJump[0]!=0xEB)
	return 1;

	bpb = (struct bpb50 *)bs->bsBPB;//把其中的 25 的字节 改为 bpb
	  BytesPerSector= (WORD)(bpb->bpbBytesPerSec);//每扇区字节数   512 不能赋值 
	 	   BytesPerSector=512;

	  //= (WORD)512;//(unsigned short)abc;
   //????查一下 这些数据都是几
	FATsectors= bpb->bpbFATsecs;//FAT占用的扇区数
	SectorsPerClust	= (BYTE)(bpb->bpbSecPerClust);//每簇扇区数
	FirstFATSector	= bpb->bpbResSectors+hidsec;//第一个FAT表扇区
	RootDirCount	= (WORD)bpb->bpbRootDirEnts;//根目录项数	???????
	//efg  = (WORD)bpb->bpbRootDirEnts;//根目录项数	???????
	//RootDirCount=512;//efg;

	 efg= (RootDirCount*32)>>9;//根目录占用的扇区数
	 RootDirSectors=32;//efg;
	FirstDirSector	= FirstFATSector+bpb->bpbFATs*FATsectors;//第一个目录扇区
	
	FirstDataSector	= FirstDirSector+RootDirSectors;//第一个数据扇区
	return 0;
}

//读一个簇中的一个扇区
//cluster 是开始的簇 part 是偏移量
unsigned char FAT16_LoadPartCluster(unsigned int cluster,unsigned part,BYTE * buffer)
{
	DWORD sector;
	//算出这的数据簇的开始扇区
	sector=FirstDataSector+(DWORD)(cluster-2)*(DWORD)SectorsPerClust;//calculate the actual sector number
	 //这个簇开始的扇区
	if(FAT16_ReadSector(sector+part,buffer))//然后读 一个扇区到 buffer 中
		return 1;//扇区 在偏移量
	else return 0;
}

//读整个簇
//Read the a cluster
//Not suitable for system which has few data RAM  
unsigned char FAT16_LoadCluster(unsigned int cluster,BYTE * buffer)
{
	DWORD sector;
	unsigned char i;
	sector=FirstDataSector+(DWORD)(cluster-2)*(DWORD)SectorsPerClust;//calculate the actual sector number
	for(i=0;i<SectorsPerClust;i++)
	{
		if(FAT16_ReadSector(sector+i,buffer+(i<<9)))break;//2的9次方 每512 个字节 相加
	}
	if(i==SectorsPerClust)return 0;
	else return 1;
}

//读下一簇簇号
//Return the cluster number of next cluster of file
//Suitable for system which has limited RAM
//根据 簇 查 找 这个 簇的 下一个 簇也就是 查找fat 表项目
unsigned int FAT16_NextCluster(unsigned int cluster)
{
	//BYTE buffer[512];
	DWORD sector;
	DWORD offset=cluster/256;
	//每一个扇区有256个 表项 看看这个簇在 fat 表项的 第几个扇区
	if(cluster<2)//簇小于 二 就是错误的
		return 0xfff8;
	sector=FirstFATSector+offset;//calculate the actual sector
	
        
        if(FAT16_ReadSector(sector,buffer))
		return 0xfff8;//read fat table / return 0xfff8 when error occured

	offset=cluster%256;//在这个扇区的偏移量
	//find the position
	//offset<<=1;
	//sector=buffer[offset+1];
//	sector<<=8;
	//sector+=buffer[offset];
	//这里有问题 应该是两个字节 
	sector=((unsigned short *)buffer)[offset];//应该注意 强制转换 是不正确的呀 
	return (unsigned short)sector;//return the cluste number
}

//在给定目录下查找文件
//Find a item in the directory which specify by the parameter "cluster"
//Return the start cluster number
//名字传不过来 测试一下数组 行不行 BYTE name[]
unsigned int FAT16_FindItem(unsigned int cluster,BYTE * mingzi, struct FileInfoStruct *FileInfo)	//这也是不可以传值呀  name000
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
		for(cnt=0;cnt<RootDirSectors;cnt++)//目录项占的扇区数目
		{
			FAT16_ReadSector(FirstDirSector+cnt,buffer)	;
			
			for(offset=0;offset<512;offset+=32)
			{
				item=(struct direntry *)(&buffer[offset]);
				if((item->deName[0] != 0x00) & (item->deName[0] != 0xe5) & (item->deAttributes != 0x0f))
				{
					for(i=0;i<11;i++)
					{
						if(buffer[offset+i]!=name[i])break;//只要有不相同的就跳转
					}
					if(i==11)//看看数据
					{   //先把 sd 卡中的数据 对照一下就行 测试
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
	return 1;//invalid path目录的形式 必须是 \\aa\\aa
	while(*p)//目录名 遍历一遍
	{
		if(*p == '\\')
		{
			deep++;//每当有一个这个字符 就加一 看看 有几层
		}
		p++;
	}
	p=dir;
	for(i=0;i<deep-1;i++)
	{
		p++;
		for(j=0;j<12;j++)name[j]=0x20;//初始化为空格
		j=0;
		while(*p != '\\')
		{
			if((*p) >= 'a' && (*p) <= 'z')name[j] = (*p++)-0x20;//变为大写
			else name[j] = *p++;
			j++;
		}
		if(FAT16_FindItem(cluster,name, &FileInfo))return 1;//find the directory
		cluster = FileInfo.StartCluster;
	}
	
	p++;//跳过\\
	
	for(j=0;j<12;j++)
	{	
		name[j]= (BYTE)' ';//初始化为空格
	}
	j=0;// 找文件名
	while(*p != '.')//file must have a extention 当文件有扩展名的是后
	{               //储存的都是大写
		if(*p>='a' && *p<='z')name[j]=(*p++)-0x20;
		else name[j]=*p++;
		j++;
	}
	
	j=8;//找 扩展名
	p++;
	while(*p)
	{
		if(*p>='a' && *p<='z')name[j]=(*p++)-0x20;
		else name[j]=*p++;
		j++;
	}
	//名字传不过来 
       temp= FAT16_FindItem(cluster,&name[0], &FileInfo);
	if(temp)
		return 1;// ??find the file
	cluster = FileInfo.StartCluster;//把信息读到 结构中 返回首簇
	return cluster;//返回文件所在 的簇号 返回一个位置
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


//复制记录项信息
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

// 开始 是\\读出目录信息 到  musicinfo中
//tpye 是类型 1看看是 mp3 2还是 wma 3还是mid 
BYTE Search(BYTE *dir,struct direntry *MusicInfo,BYTE *Count,BYTE *type)
//当COUNT为零时，有它带回这个目录下总共有多少首音乐
{           //不为零时有MusicInfo带回第Count首歌的详细文件信息
	
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
			//目录项的扇区数目
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
          *Count=i;//歌曲的数目
	return 0;	
}
