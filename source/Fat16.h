#include "config.h"

#ifndef __FAT16_H__
#define __FAT16_H__
//���������Ͷ��� Ӧ���޸ĳ�һ���� 



//#include "UART.H"
#include "sddriver.h"
#include<stdlib.h>

//�����ⲿ������д����
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
//��չBPB
struct extboot {
	CHAR	exDriveNumber;
	/*/������������( PhysicalDrive Number) ��BIOS ����
���������йء���������������ʶΪ0x00������Ӳ�̱�
��ʶΪ0x80��������������������޹ء�һ��أ��ڷ�
��һ��INT13h BIOS ����֮ǰ���ø�ֵ������ָ������
�ʵ��豸��ֻ�е����豸��һ�������豸ʱ�����ֵ��*/
	// drive number (0x80)//0x00 for floopy disk 0x80 for hard disk
	CHAR	exReserved1;
	// reserved should always set 0
	//����(Reserved) FAT32 �������ǽ����ֶε�ֵ����Ϊ0
	CHAR	exBootSignature;//��չ������ǩ(ExtendedBoot Signature) ���ֶα���Ҫ���ܱ�Windows 2000 ��ʶ���ֵ0x28 ��0x29	
	// ext. boot signature (0x29)
#define EXBOOTSIG       0x29
	CHAR	exVolumeID[4];		
	//�������(Volume SerialNumber) �ڸ�ʽ������ʱ��������һ�������ţ������������ִ���
	// volume ID number
	CHAR	exVolumeLabel[11];		
	// volume label "NO NAME"���(Volume Label) ���ֶ�ֻ��ʹ��һ�Σ�����������
//����š����ڣ���걻��Ϊһ�������ļ������ڸ�Ŀ¼��
	CHAR	exFileSysType[8];	//	ϵͳID(System ID) FAT32�ļ�ϵͳ��һ��ȡΪ"FAT32"	// fs type (FAT12 or FAT16)
};

#pragma pack()
#pragma pack(1)
//�����ֽڵ���תָ��DBR
struct bootsector50 {
	BYTE	bsJump[3];					// jump inst E9xxxx or EBxx90//�����ֽڵ���תָ��
	CHAR	bsOemName[8];				// OEM name and version���̱�־ �� ����ϵͳ�� �汾�� 
	CHAR	bsBPB[25];					// BIOS parameter block Ϊʲô�� 25 Ӳ���� 53���ֽ�
	CHAR	bsExt[26];					// Bootsector Extension Ӳ����420
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
        WORD	bpbRootDirEnts; // ��Ŀ¼����Ŀnumber of root directory entries
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
		BYTE		deExtension[3]; 	// ��չ��extension, blank filled
		BYTE		deAttributes;   	// ����file attributes
#define ATTR_NORMAL     0x00            // normal file
#define ATTR_READONLY   0x01            // file is readonly
#define ATTR_HIDDEN     0x02            // file is hidden
#define ATTR_SYSTEM     0x04            // file is a system file
#define ATTR_VOLUME     0x08            // entry is a volume label
#define ATTR_LONG_FILENAME	0x0F		// this is a long filename entry			    
#define ATTR_DIRECTORY  0x10            // entry is a directory name
#define ATTR_ARCHIVE    0x20            // file is new or modified
		BYTE        deLowerCase;    	// ϵͳ����NT VFAT lower case flags  (set to zero)
#define LCASE_BASE      0x08            // filename base in lower case
#define LCASE_EXT       0x10            // filename extension in lower case
#define LCASE_EXT       0x10            // filename extension in lower case
		BYTE        deCHundredth;   	// hundredth of seconds in CTime
		BYTE        deCTime[2];     	// �ļ�����ʱ��create time
		BYTE        deCDate[2];     	//�ļ��������� create date
		BYTE        deADate[2];     	//�ļ����������� access date
		WORD        deHighClust; 		//�ļ���ʼ�غŵĸ�16 λ high bytes of cluster number
		BYTE        deMTime[2];     	// �ļ�������޸�ʱ��last update time
		BYTE        deMDate[2];     	// �ļ�������޸�����last update date
		WORD        deStartCluster; 	// ��ʾ�ļ����״غ�starting cluster of file
		DWORD       deFileSize;  		// ��ʾ�ļ��ĳ���size of file in bytes
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
	unsigned int Sector;				//<file record place???? �ļ�Ŀ¼��ʼ������
	unsigned int Offset;				//<file record offset�������ĵڼ���
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

//MP3����ӵĺ���
BYTE Search(BYTE *dir,struct direntry *MusicInfo,BYTE *Count,BYTE *type);//���������ļ�
unsigned char FAT16_LoadPartCluster(unsigned int cluster,unsigned part,BYTE * buffer);//�����ļ�


//FAT16ԭ�еĺ���
unsigned char FAT16_Init();//��ʼ��
unsigned char FAT16_LoadCluster(unsigned int cluster,BYTE * buffer);//��һ������
unsigned int FAT16_NextCluster(unsigned int cluster);//������һ�غ�
unsigned int FAT16_FindFreeCluster();//���ҿմ�
unsigned char FAT16_DisDir(BYTE *dir);//��ʾĿ¼����
unsigned int FAT16_FindItem(unsigned int cluster, BYTE * mingzi, struct FileInfoStruct *FileInfo);//�����ļ�
unsigned int FAT16_Open(BYTE * dir);//���ļ�
unsigned int FAT16_OpenDir(BYTE * dir);//��Ŀ¼
unsigned char FAT16_Read(unsigned int pointer, unsigned long size);//���ļ���size=0���������ļ�
unsigned char FAT16_Rename(BYTE *dir,BYTE *newname);//������
unsigned char FAT16_ModifyFAT(unsigned int cluster,unsigned int val);//�޸�FAT��
unsigned char FAT16_Delete(BYTE *dir);//ɾ���ļ�
unsigned char FAT16_DelItem(struct FileInfoStruct *FileInfo);//ɾ����¼��
unsigned int FAT16_FindFreeItem(unsigned int cluster, struct FileInfoStruct *FileInfo);//���ҿ�����
unsigned char FAT16_MkDir(BYTE * dir);//��Ŀ¼
unsigned char FAT16_Write(unsigned int cluster,unsigned char *data,unsigned long size);//д�ļ�
unsigned int FAT16_Create(BYTE * dir,unsigned long size);//���ļ�������ָ���ļ���С
unsigned char FAT16_RmDir(BYTE * dir);//ɾ��Ŀ¼
unsigned int FAT16_Close(unsigned int * p);//�ر��ļ�
#endif
