#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

//LED¶Ë¿Ú¶¨Òå
#define LED PAout(8)	

void TIM6_Int_Init(u16 arr,u16 psc);
void LED_Init(void);
#endif























