//system.c
#include "system.h"
#include "iolpc210x.h"


void FreqInit(void)
{
  //crystal is 14 745 600
  //devide or multiplier
  //here is calculate frecuence
  PLLCFG_bit.MSEL = 0x2;  //M - multiplier -> 3
  PLLCFG_bit.PSEL = 0x1;  //P - devider -> 2
  // PLLCFG_bit.MSEL = 0x0;  //M - multiplier -> 0
  // PLLCFG_bit.PSEL = 0x3;  //P - devider -> 8

  //set changes (require from architecture)
  PLLFEED_bit.FEED = 0xAA;
  PLLFEED_bit.FEED = 0x55;


  //enable or connect PLL
  //enable PLL
  PLLCON_bit.PLLE = 1;
  //set changes (require from architecture)
  PLLFEED_bit.FEED = 0xAA;
  PLLFEED_bit.FEED = 0x55;

  //wait for PLOK (correct freq)
  while(PLLSTAT_bit.PLOCK == 0);

  //connect PLL
  PLLCON_bit.PLLC = 1;
  //set changes (require from architecture)
  PLLFEED_bit.FEED = 0xAA;
  PLLFEED_bit.FEED = 0x55;

}

