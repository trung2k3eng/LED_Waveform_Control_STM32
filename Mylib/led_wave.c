#include "led_wave.h"

extern TIM_HandleTypeDef htim1;

static const uint16_t sine_table[TABLE_SIZE] = {
    500,526,552,578,603,628,652,675,698,719,
    740,759,777,794,809,823,835,846,855,862,
    868,872,874,875,874,872,868,862,855,846,
    835,823,809,794,777,759,740,719,698,675,
    652,628,603,578,552,526,500,473,447,421,
    396,371,347,324,301,280,259,240,222,205,
    190,176,164,153,144,137,131,127,125,124,
    125,127,131,137,144,153,164,176,190,205,
    222,240,259,280,301,324,347,371,396,421,
    447,473,500,526,552,578,603,628,652,675,
    698,719,740,759,777,794,809,823,835,846,
    855,862,868,872,874,875,874,872,868,862
};

static uint16_t dma_buffer_ch1[DMA_BUFFER_SIZE];
static uint16_t dma_buffer_ch2[DMA_BUFFER_SIZE];
static uint16_t dma_buffer_ch3[DMA_BUFFER_SIZE];

static uint8_t led_enable = 0;
static uint8_t speed_step = 1;

static void LED_Effect_GenerateBuffers(void)
{
    uint16_t i;
    uint16_t idxA;
    uint16_t idxB;
    uint16_t idxC;

    for (i = 0; i < DMA_BUFFER_SIZE; i++)
    {
        idxA = (i * speed_step) % TABLE_SIZE;
        idxB = ((i * speed_step) + 40) % TABLE_SIZE;
        idxC = ((i * speed_step) + 80) % TABLE_SIZE;

        dma_buffer_ch1[i] = sine_table[idxA];
        dma_buffer_ch2[i] = sine_table[idxB];
        dma_buffer_ch3[i] = sine_table[idxC];
    }
}

void LED_Effect_Init(void)
{
    LED_Effect_GenerateBuffers();
    TIM1->CCR1 = dma_buffer_ch1[0];
    TIM1->CCR2 = dma_buffer_ch2[0];
    TIM1->CCR3 = dma_buffer_ch3[0];
}

void LED_Effect_Start(void)
{
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)dma_buffer_ch1, DMA_BUFFER_SIZE);
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_2, (uint32_t*)dma_buffer_ch2, DMA_BUFFER_SIZE);
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_3, (uint32_t*)dma_buffer_ch3, DMA_BUFFER_SIZE);
    HAL_TIM_Base_Start(&htim1);
}

void LED_Effect_Stop(void)
{
    HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_3);
    HAL_TIM_Base_Stop(&htim1);
}

void LED_Effect_SetSpeed(uint8_t speed)
{
    if (speed < 1 || speed > 4)
    {
        return;
    }

    speed_step = speed;
    LED_Effect_GenerateBuffers();

    if (led_enable)
    {
        LED_Effect_Stop();
        LED_Effect_Start();
    }
}

void LED_Effect_Enable(uint8_t enable)
{
    if (enable && !led_enable)
    {
        led_enable = 1;
        LED_Effect_Start();
    }
    else if (!enable && led_enable)
    {
        led_enable = 0;
        LED_Effect_Stop();
    }
}
