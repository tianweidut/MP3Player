
#ifndef __CONFIG_H 
#define __CONFIG_H
//��һ������Ķ�
//This segment should not be modified

#define CHAR	char
#define BYTE	unsigned char
#define WORD	unsigned short int
#define DWORD	unsigned long
//ע�� �����޷���int  ������ 16 λ
// ���� 2103 �� ���� �������� ��32 
/*
#define uint8  unsigned char
#define  int8    signed char
#define uint16 unsigned short
#define  int16   signed short
#define uint32 unsigned long
#define  int32   signed long
*/
typedef unsigned char  INT8U;                   /* �޷���8λ���ͱ���                        */
typedef signed   char  INT8;                    /* �з���8λ���ͱ���                        */
typedef unsigned int INT16U;                  /* �޷���16λ���ͱ���  short                     */
typedef signed   int INT16;                   /* �з���16λ���ͱ���                       */
typedef unsigned long   INT32U;                  /* �޷���32λ���ͱ���                       */
typedef signed   long   INT32;                   /* �з���32λ���ͱ���                       */
typedef float          FP32;                    /* �����ȸ�������32λ���ȣ�int                 */
typedef double         FP64;                    /* ˫���ȸ�������64λ���ȣ�                 */


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

//typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	�޷���8λ���ͱ���  */
//typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		�з���8λ���ͱ���  */
//typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable 	�޷���16λ���ͱ��� */
//typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		�з���16λ���ͱ��� */
//typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	�޷���32λ���ͱ��� */
//typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		�з���32λ���ͱ��� */
//typedef float          fp32;                    /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
//typedef double         fp64;                    /* double precision floating point variable (64bits) ˫���ȸ�������64λ���ȣ� */

/********************************/
/*      uC/OS-II specital code  */
/*      uC/OS-II���������      */
/********************************/

//#define     USER_USING_MODE    0x10                    /*  User mode ,ARM 32BITS CODE �û�ģʽ,ARM����                  */
// 
                                                     /*  Chosen one from 0x10,0x30,0x1f,0x3f.ֻ����0x10,0x30,0x1f,0x3f֮һ       */
//#include "Includes.h"


/********************************/
/*      ARM���������           */
/*      ARM specital code       */
/********************************/
//��һ������Ķ�
//This segment should not be modify

//#include  <lpc2103.h>
#include  <iolpc2103.h>


/********************************/
/*     Ӧ�ó�������             */
/*Application Program Configurations*/
/********************************/
//���¸�����Ҫ�Ķ�
//This segment could be modified as needed.
//#include    <stdio.h>
//#include    <ctype.h>
//#include    <stdlib.h>
//#include    <setjmp.h>
//#include    <rt_misc.h>


/********************************/
/*     �����ӵ�����             */
/*Configuration of the example */
/********************************/
/* System configuration .Fosc��Fcclk��Fcco��Fpclk must be defined */
/* ϵͳ����, Fosc��Fcclk��Fcco��Fpclk���붨��*/
#define Fosc            11059200                    //Crystal frequence,10MHz~25MHz��should be the same as actual status. 
						    //Ӧ����ʵ��һ������Ƶ��,10MHz~25MHz��Ӧ����ʵ��һ��
#define Fcclk           (Fosc * 4)                  //System frequence,should be (1~32)multiples of Fosc,and should be equal or less  than 60MHz. 
						    //ϵͳƵ�ʣ�����ΪFosc��������(1~32)����<=60MHZ
#define Fcco            (Fcclk * 4)                 //CCO frequence,should be 2��4��8��16 multiples of Fcclk, ranged from 156MHz to 320MHz. 
						    //CCOƵ�ʣ�����ΪFcclk��2��4��8��16������ΧΪ156MHz~320MHz
#define Fpclk           (Fcclk / 4) * 1             //VPB clock frequence , must be 1��2��4 multiples of (Fcclk / 4).
						    //VPBʱ��Ƶ�ʣ�ֻ��Ϊ(Fcclk / 4)��1��2��4��

//#include    "target.h"              //This line may not be deleted ��һ�䲻��ɾ��
//#include    "..\..\Arm_Pc\pc.h"
/********************************/
/*     ZLG/FS�������ͷ�ļ�     */
/********************************/
///#define MAX_DISK_CACHES             20          //�ļ�ϵͳCache��Ŀ



// fat �߲�Ҫ
//#include    "fat.h"
//#include    "OSFile.h"


//extern	char *strupr(char *Str);
//extern uint16 	CFCammand(uint8 Cammand, void *Parameter);
/********************************/
/* CF�洢�������ü�������ͷ�ļ� */
/********************************/
//#define 	UCOSII
//#include    "SysATA.H"
//#include  	"IDE.H"
#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
