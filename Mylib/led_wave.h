#ifndef LED_WAVE_H
#define LED_WAVE_H

#include "main.h"

#define TABLE_SIZE 120

#define DMA_BUFFER_SIZE TABLE_SIZE

void LED_Effect_Init(void);
void LED_Effect_Start(void);
void LED_Effect_Stop(void);
void LED_Effect_SetSpeed(uint8_t speed);
void LED_Effect_Enable(uint8_t enable);

#endif
