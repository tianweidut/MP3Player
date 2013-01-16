#include "config.h"

#ifndef __FAT16_H__
#define __FAT16_H__
//变量的类型定义 应该修改成一样的 



//#include "UART.H"
#include "sddriver.h"
#include<stdlib.h>

//声明外部器件读写函数
extern BYTE MMC_SD_ReadSingleBlock(DWORD sector, BYTE* buffer);
extern BYTE MMC_SD_WriteSingleBlock(DWORD sector, BYTE* buffer);


#define MSDOSFSROOT     0               // cluster 0 means the root dir
#define CLUST_FREE      0               // cluster 0 also means a free cluster
#define MSDOSFSFREE     CLUST_FREE
#define CLUST_FIRST     2             	// first legal cluster number
#define CLUST_RSRVD     0xfff6      	// reserved cluster range
#define CLUST_BAD       0xfff7     		// a cluster with a defect
#define CLUST_EOFS      0xfff8     		// start of eof cluster range
#define CLUST_EOFE      0xffff      	// end of eof cluster range
 #pragma pack(1)
//扩展BPB
struct extboot {
	CHAR	exDriveNumber;
	/*/物理驱动器号( PhysicalDrive Number) 与BIOS 物理
驱动器号有关。软盘驱动器被标识为0x00，物理硬盘被
标识为0x80，而与物理磁盘驱动器无关。一般地，在发
出一个INT13h BIOS 调用之前设置该值，具体指定所访
问的设备。只有当该设备是一个引导设备时，这个值才*/
	// drive number (0x80)//0x00 for floopy disk 0x80 for hard disk
	CHAR	exReserved1;
	// reserved should always set 0
	//保留(Reserved) FAT32 分区总是将本字段的值设置为0
	CHAR	exBootSignature;//扩展引导标签(ExtendedBoot Signature) 本字段必须要有能被Windows 2000 所识别的值0x28 或0x29	
	// ext. boot signature (0x29)
#define EXBOOTSIG       0x29
	CHAR	exVolumeID[4];		
	//分区序号(Volume SerialNumber) 在格式化磁盘时所产生的一个随机序号，它有助于区分磁盘
	// volume ID number
	CHAR	exVolumeLabel[11];		
	// volume label "NO NAME"卷标(Volume Label) 本字段只能使用一次，它被用来保
//存卷标号。现在，卷标被作为一个特殊文件保存在根目录中
	CHAR	exFileSysType[8];	//	系统ID(System ID) FAT32文件系统中一般取为"FAT32"	// fs type (FAT12 or FAT16)
};

#pragma pack()
#pragma pack(1)
//三个字节的跳转指令DBR
struct bootsector50 {
	BYTE	bsJump[3];					// jump inst E9xxxx or EBxx90//三个字节的跳转指令
	CHAR	bsOemName[8];				// OEM name and version厂商标志 和 操作系统的 版本号 
	CHAR	bsBPB[25];					// BIOS parameter block 为什么是 25 硬盘是 53个字节
	CHAR	bsExt[26];					// Bootsector Extension 硬盘是420
	CHAR	bsBootCode[448];			// pad so structure is 512b
	BYTE	bsBootSectSig0;				// boot sector signature byte 0x55 
	BYTE	bsBootSectSig1;				// boot sector signature byte 0xAA
#define BOOTSIG0        0x55
#define BOOTSIG1        0xaa
};
 #pragma pack()
 #pragma pack(1)
struct bpb50 {
        WORD	bpbBytesPerSec; // bytes per sector				//512 1024 2048 or 4096
        BYTE	bpbSecPerClust; // sectors per cluster			// power of 2
        WORD	bpbResSectors;  // number of reserved sectors	//1 is recommend
        BYTE	bpbFATs;        // number of FATs				// 2 is recommend
        WORD	bpbRootDirEnts; // 根目录项数目number of root directory entries
        WORD	bpbSectors;     // total number of sectors
        BYTE	bpbMedia;       // media descriptor				//0xf8 match the fat[0]
        WORD	bpbFATsecs;     // number of sectors per FAT
        WORD	bpbSecPerTrack; // sectors per track
        WORD	bpbHeads;       // number of heads
        DWORD	bpbHiddenSecs;  // # of hidden sectors
        DWORD	bpbHugeSectors; // # of sectors if bpbSectors == 0
};

#pragma pack()
#pragma pack(1)
// Structure of a dos directory entry. 32
struct direntry {
		BYTE		deName[8];      	// filename, blank filled
#define SLOT_EMPTY      0x00            // slot has never been used
#define SLOT_E5         0x05            // the real value is 0xE5
#define SLOT_DELETED    0xE5            // file in this slot deleted
#define SLOT_DIR		0x2E			// a directorymmm
		BYTE		deExtension[3]; 	// 扩展名extension, blank filled
		BYTE		deAttributes;   	// 属性file attributes
#define ATTR_NORMAL     0x00            // normal file
#define ATTR_READONLY   0x01            // file is readonly
#define ATTR_HIDDEN     0x02            // file is hidden
#define ATTR_SYSTEM     0x04            // file is a system file
#define ATTR_VOLUME     0x08            // entry is a volume label
#define ATTR_LONG_FILENAME	0x0F		// this is a long filename entry			    
#define ATTR_DIRECTORY  0x10            // entry is a directory name
#define ATTR_ARCHIVE    0x20            // file is new or modified
		BYTE        deLowerCase;    	// 系统保留NT VFAT lower case flags  (set to zero)
#define LCASE_BASE      0x08            // filename base in lower case
#define LCASE_EXT       0x10            // filename extension in lower case
#define LCASE_EXT       0x10            // filename extension in lower case
		BYTE        deCHundredth;   	// hundredth of seconds in CTime
		BYTE        deCTime[2];     	// 文件创建时间create time
		BYTE        deCDate[2];     	//文件创建日期 create date
		BYTE        deADate[2];     	//文件最后访问日期 access date
		WORD        deHighClust; 		//文件起始簇号的高16 位 high bytes of cluster number
		BYTE        deMTime[2];     	// 文件的最近修改时间last update time
		BYTE        deMDate[2];     	// 文件的最近修改日期last update date
		WORD        deStartCluster; 	// 表示文件的首簇号starting cluster of file
		DWORD       deFileSize;  		// 表示文件的长度size of file in bytes
};

  #pragma pack()
// number of directory entries in one sector
#define DIRENTRIES_PER_SECTOR	0x10 //32	
//when the bpbBytesPerSec=512 
   #pragma pack(1)
// Structure of a Win95 long name directory entry
struct winentry {
		BYTE			weCnt;			// 
#define WIN_LAST        0x40
#define WIN_CNT         0x3f
		BYTE		wePart1[10];
		BYTE		weAttributes;
#define ATTR_WIN95      0x0f
		BYTE		weReserved1;
		BYTE		weChksum;
		BYTE		wePart2[12];
		WORD       	weReserved2;
		BYTE		wePart3[4];
};
 #pragma pack()
#define WIN_ENTRY_CHARS	13      // Number of chars per winentry

// Maximum filename length in Win95
// Note: Must be < sizeof(dirent.d_name)
#define WIN_MAXLEN      255

// This is the format of the contents of the deTime field in the direntry
// structure.
// We don't use bitfields because we don't know how compilers for
// arbitrary machines will lay them out.
#define DT_2SECONDS_MASK        0x1F    // seconds divided by 2
#define DT_2SECONDS_SHIFT       0
#define DT_MINUTES_MASK         0x7E0   // minutes
#define DT_MINUTES_SHIFT        5
#define DT_HOURS_MASK           0xF800  // hours
#define DT_HOURS_SHIFT          11

// This is the format of the contents of the deDate field in the direntry
// structure.
#define DD_DAY_MASK				0x1F	// day of month
#define DD_DAY_SHIFT			0
#define DD_MONTH_MASK			0x1E0	// month
#define DD_MONTH_SHIFT			5
#define DD_YEAR_MASK			0xFE00	// year - 1980
#define DD_YEAR_SHIFT			9


 // #pragma pack(1)
// Stuctures
struct FileInfoStruct
{
	unsigned short StartCluster;			//< file starting cluster for last file accessed
	unsigned long Size;					//< file size for last file accessed
	unsigned char Attr;					//< file attr for last file accessed
	//unsigned short CreateTime;			//< file creation time for last file accessed
	//unsigned short CreateDate;			//< file creation date for last file accessed
	unsigned int Sector;				//<file record place???? 文件目录开始的扇区
	unsigned int Offset;				//<file record offset在扇区的第几项
};
  //#pragma pack()

//function often to use 

//unsigned char FAT16_Init();

//unsigned char FAT16_DisDir(BYTE *dir);

//unsigned int FAT16_Open(BYTE * dir);

//unsigned char FAT16_Read(unsigned int pointer, unsigned long size);

//unsigned char FAT16_Rename(BYTE *dir,BYTE *newname);

//unsigned char FAT16_Delete(BYTE *dir);

//unsigned char FAT16_MkDir(BYTE * dir);

//unsigned char FAT16_RmDir(BYTE * dir);

//unsigned int FAT16_Create(BYTE * dir,unsigned long size);

//unsigned char FAT16_Write(unsigned int cluster,unsigned char *data,unsigned long size);

//MP3后添加的函数
BYTE Search(BYTE *dir,struct direntry *MusicInfo,BYTE *Count,BYTE *type);//查找音乐文件
unsigned char FAT16_LoadPartCluster(unsigned int cluster,unsigned part,BYTE * buffer);//加载文件


//FAT16原有的函数
unsigned char FAT16_Init();//初始化
unsigned char FAT16_LoadCluster(unsigned int cluster,BYTE * buffer);//读一个扇区
unsigned int FAT16_NextCluster(unsigned int cluster);//查找下一簇号
unsigned int FAT16_FindFreeCluster();//查找空簇
unsigned char FAT16_DisDir(BYTE *dir);//显示目录内容
unsigned int FAT16_FindItem(unsigned int cluster, BYTE * mingzi, struct FileInfoStruct *FileInfo);//查找文件
unsigned int FAT16_Open(BYTE * dir);//打开文件
unsigned int FAT16_OpenDir(BYTE * dir);//打开目录
unsigned char FAT16_Read(unsigned int pointer, unsigned long size);//读文件，size=0代表整个文件
unsigned char FAT16_Rename(BYTE *dir,BYTE *newname);//重命名
unsigned char FAT16_ModifyFAT(unsigned int cluster,unsigned int val);//修改FAT项
unsigned char FAT16_Delete(BYTE *dir);//删除文件
unsigned char FAT16_DelItem(struct FileInfoStruct *FileInfo);//删除记录项
unsigned int FAT16_FindFreeItem(unsigned int cluster, struct FileInfoStruct *FileInfo);//查找空余项
unsigned char FAT16_MkDir(BYTE * dir);//建目录
unsigned char FAT16_Write(unsigned int cluster,unsigned char *data,unsigned long size);//写文件
unsigned int FAT16_Create(BYTE * dir,unsigned long size);//建文件，必须指定文件大小
unsigned char FAT16_RmDir(BYTE * dir);//删除目录
unsigned int FAT16_Close(unsigned int * p);//关闭文件
#endif
