#include "button_control.h"
#include "Button.h"
#include "led_wave.h"

Button_Typdef my_button;

static volatile	uint8_t led_mode = 0;

void btn_press_short_callback(Button_Typdef *ButtonX)
{
    if (led_mode != 0) {
        led_mode++;
        if (led_mode > 4) led_mode = 1;  
				LED_Effect_Enable(1);
				LED_Effect_SetSpeed(led_mode);
    }
}

void btn_press_timeout_callback(Button_Typdef *ButtonX)
{
    if (led_mode == 0) {
        led_mode = 1;
        LED_Effect_Enable(1);
        LED_Effect_SetSpeed(1);
    } else {
        led_mode = 0;
        LED_Effect_Enable(0);
    }
}

void Button_Init(void)
{
    button_init(&my_button, button_GPIO_Port, button_Pin);
    led_mode = 0;
    LED_Effect_Enable(0);
}

void Button_Check(void)
{
    button_handle(&my_button);
}
