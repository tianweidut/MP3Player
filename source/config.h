
#ifndef __CONFIG_H 
#define __CONFIG_H
//这一段无需改动
//This segment should not be modified

#define CHAR	char
#define BYTE	unsigned char
#define WORD	unsigned short int
#define DWORD	unsigned long
//注意 他把无符号int  当成了 16 位
// 但是 2103 中 好像 整形数据 是32 
/*
#define uint8  unsigned char
#define  int8    signed char
#define uint16 unsigned short
#define  int16   signed short
#define uint32 unsigned long
#define  int32   signed long
*/
typedef unsigned char  INT8U;                   /* 无符号8位整型变量                        */
typedef signed   char  INT8;                    /* 有符号8位整型变量                        */
typedef unsigned int INT16U;                  /* 无符号16位整型变量  short                     */
typedef signed   int INT16;                   /* 有符号16位整型变量                       */
typedef unsigned long   INT32U;                  /* 无符号32位整型变量                       */
typedef signed   long   INT32;                   /* 有符号32位整型变量                       */
typedef float          FP32;                    /* 单精度浮点数（32位长度）int                 */
typedef double         FP64;                    /* 双精度浮点数（64位长度）                 */


typedef unsigned char   uint8;       
typedef signed   char   int8;             
typedef unsigned short 	uint16;    
typedef signed   short 	int16;    
typedef unsigned int   	uint32;      
typedef signed   int   	int32;      
typedef float          	fp32;             
typedef double         	fp64;      
            

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

//typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	无符号8位整型变量  */
//typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		有符号8位整型变量  */
//typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable 	无符号16位整型变量 */
//typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
//typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
//typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		有符号32位整型变量 */
//typedef float          fp32;                    /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
//typedef double         fp64;                    /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */

/********************************/
/*      uC/OS-II specital code  */
/*      uC/OS-II的特殊代码      */
/********************************/

//#define     USER_USING_MODE    0x10                    /*  User mode ,ARM 32BITS CODE 用户模式,ARM代码                  */
// 
                                                     /*  Chosen one from 0x10,0x30,0x1f,0x3f.只能是0x10,0x30,0x1f,0x3f之一       */
//#include "Includes.h"


/********************************/
/*      ARM的特殊代码           */
/*      ARM specital code       */
/********************************/
//这一段无需改动
//This segment should not be modify

//#include  <lpc2103.h>
#include  <iolpc2103.h>


/********************************/
/*     应用程序配置             */
/*Application Program Configurations*/
/********************************/
//以下根据需要改动
//This segment could be modified as needed.
//#include    <stdio.h>
//#include    <ctype.h>
//#include    <stdlib.h>
//#include    <setjmp.h>
//#include    <rt_misc.h>


/********************************/
/*     本例子的配置             */
/*Configuration of the example */
/********************************/
/* System configuration .Fosc、Fcclk、Fcco、Fpclk must be defined */
/* 系统设置, Fosc、Fcclk、Fcco、Fpclk必须定义*/
#define Fosc            11059200                    //Crystal frequence,10MHz~25MHz，should be the same as actual status. 
						    //应当与实际一至晶振频率,10MHz~25MHz，应当与实际一至
#define Fcclk           (Fosc * 4)                  //System frequence,should be (1~32)multiples of Fosc,and should be equal or less  than 60MHz. 
						    //系统频率，必须为Fosc的整数倍(1~32)，且<=60MHZ
#define Fcco            (Fcclk * 4)                 //CCO frequence,should be 2、4、8、16 multiples of Fcclk, ranged from 156MHz to 320MHz. 
						    //CCO频率，必须为Fcclk的2、4、8、16倍，范围为156MHz~320MHz
#define Fpclk           (Fcclk / 4) * 1             //VPB clock frequence , must be 1、2、4 multiples of (Fcclk / 4).
						    //VPB时钟频率，只能为(Fcclk / 4)的1、2、4倍

//#include    "target.h"              //This line may not be deleted 这一句不能删除
//#include    "..\..\Arm_Pc\pc.h"
/********************************/
/*     ZLG/FS需包含的头文件     */
/********************************/
///#define MAX_DISK_CACHES             20          //文件系统Cache数目



// fat 线不要
//#include    "fat.h"
//#include    "OSFile.h"


//extern	char *strupr(char *Str);
//extern uint16 	CFCammand(uint8 Cammand, void *Parameter);
/********************************/
/* CF存储卡需配置及包含的头文件 */
/********************************/
//#define 	UCOSII
//#include    "SysATA.H"
//#include  	"IDE.H"
#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
