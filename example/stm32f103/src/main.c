/*
 * ************************************************
 * 
 *              STM32 blink gcc demo
 * 
 *  CPU: STM32F103C8
 *  PIN: PA1
 * 
 * ************************************************
*/

#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "sgl.h"
#include "tft.h"
#include <sgl_font.h>

#define LED_PERIPH RCC_APB2Periph_GPIOC
#define LED_PORT GPIOC
#define LED_PIN GPIO_Pin_13


#define  PANEL_WIDTH  240
#define  PANEL_HEIGHT 240



sgl_color_t panel_buffer[PANEL_WIDTH * 1];


void USART1_GPIO_Config(void) {  
    GPIO_InitTypeDef GPIO_InitStructure;  
  
    // 使能GPIOA和USART1时钟  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);  
  
    // USART1_TX   GPIOA.9  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
  
    // USART1_RX   GPIOA.10  
    // 注意：对于发送，我们不需要配置USART1_RX的GPIO  
    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入  
    // GPIO_Init(GPIOA, &GPIO_InitStructure);  
}  

// USART1 配置  
void USART1_Config(void) {  
    USART_InitTypeDef USART_InitStructure;  
    // USART1参数初始化  
    USART_InitStructure.USART_BaudRate = 115200; // 波特率115200
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 数据位8  
    USART_InitStructure.USART_StopBits = USART_StopBits_1; // 停止位1  
    USART_InitStructure.USART_Parity = USART_Parity_No; // 无校验位  
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控制  
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // 收发模式  
  
    // 初始化USART1  
    USART_Init(USART1, &USART_InitStructure);  
  
    // 使能USART1接收和发送中断  
    // 注意：如果你不使用中断，可以注释掉这两行  
    // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  
    // USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  
  
    // 使能USART1  
    USART_Cmd(USART1, ENABLE);  
}  
 
void UART1_SendString(const char *str) {
    while (*str != '\0') {
        // 等待发送寄存器为空
        while ((USART1->SR & USART_SR_TXE) == 0);
        // 写入字符到发送寄存器
        USART1->DR = (uint8_t)(*str++);
    }
    // 等待最后一个字符发送完成
    while ((USART1->SR & USART_SR_TC) == 0);
}


void delay(int x)
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < 1000; j++)
            __NOP();
    }
}


#include <stdio.h>


int main(void)
{
    GPIO_InitTypeDef gpioDef;
    RCC_APB2PeriphClockCmd(LED_PERIPH, ENABLE);
    gpioDef.GPIO_Mode = GPIO_Mode_Out_PP;
    gpioDef.GPIO_Pin = LED_PIN;
    gpioDef.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(LED_PORT, &gpioDef);

    sgl_device_fb_t fb_dev = {
        .xres = PANEL_WIDTH,
        .yres = PANEL_HEIGHT,
        .xres_virtual = PANEL_WIDTH,
        .yres_virtual = PANEL_HEIGHT,
        .flush_area = demo_panel_flush_area,
        .framebuffer = panel_buffer,
        .framebuffer_size = sizeof(panel_buffer),
    };

    sgl_device_fb_register(&fb_dev);
    sgl_device_log_register(UART1_SendString);

    USART1_GPIO_Config();
    USART1_Config();
    UART1_SendString("STM32F103C8T6 SGL DEMO\r\n");

    sgl_init();

    SPI1_Init();
    tft_init();

    // sgl_obj_t *rect = sgl_rect_create(NULL);
    // sgl_obj_set_pos(rect, 20, 20);
    // sgl_obj_set_size(rect, 480, 200);
    // sgl_obj_set_clickable(rect);
    // sgl_obj_set_alpha(rect, 255);

    // sgl_obj_t *rect2 = sgl_rect_create(rect);
    // sgl_obj_set_pos(rect2, 20, 20);
    // sgl_obj_set_size(rect2, 300, 200);
    // sgl_obj_set_color(rect2, SGL_BLUE);
    // sgl_obj_set_clickable(rect2);
    // sgl_obj_set_radius(rect2, 20);
    // sgl_obj_set_alpha(rect2, 128);
    // sgl_obj_set_border_width(rect2, 4);
    // sgl_obj_set_border_color(rect2, SGL_RED);

    sgl_event_pos_t pos_test = {
        .x = 100,
        .y = 182,
    };

    static uint16_t precent = 0;
    static uint8_t pos = 0;
    sgl_event_t event;
    while (1) {
        sgl_task_handle();
        // event.pos.x = 10,
        // event.pos.y = 10,
        // event.obj = button;
        //event.type = SGL_EVENT_PRESSED;
        // sgl_event_send(button, SGL_EVENT_PRESSED);
        // sgl_task_handle();
        // delay(1000);
        // sgl_event_send(button, SGL_EVENT_RELEASED);
        // sgl_task_handle();
        // delay(1000);
    }
}
