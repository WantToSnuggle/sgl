#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "tft.h"
#include "sgl.h"

/**
 * GPIO模拟SPI引脚
 */
#define SPI_SCK_PIN     GPIO_Pin_0  
#define SPI_MOSI_PIN    GPIO_Pin_1  
#define SPI_SCK_PORT    GPIOA
#define SPI_MOSI_PORT   GPIOA

#define SPI_DC_PIN      GPIO_Pin_3
#define SPI_DC_PORT     GPIOA
#define SPI_RST_PIN     GPIO_Pin_2
#define SPI_RST_PORT    GPIOA

/**
 * 硬件SPI引脚
 */
// SCK（时钟引脚）‌：连接到PA5引脚
// ‌MISO（主入从出引脚）‌：连接到PA6引脚
// ‌MOSI（主出从入引脚）‌：连接到PA7引脚

void tft_gpio_init(void)
{
    GPIO_InitTypeDef gpioDef;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    gpioDef.GPIO_Mode = GPIO_Mode_Out_PP;
    gpioDef.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    gpioDef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpioDef);
}


void SPI1_Init(void)
{
    /* 定义SPI结构体变量 */
    GPIO_InitTypeDef  GPIO_InitStructure;
    SPI_InitTypeDef   SPI_InitStructure;
 
    /* SPI的IO口和SPI外设打开时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
 
    /* SPI的IO口设置 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     // 复用输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    /* SPI的基本配置 */
    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;  // 设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;     // 设置SPI工作模式:设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // 设置SPI的数据大小:SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;       // 串行同步时钟的空闲状态为高电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;      // 串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;         // NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;// 定义波特率预分频的值:波特率预分频值为256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;// 指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 0;          // CRC值计算的多项式
    SPI_Init(SPI1, &SPI_InitStructure);               // 根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
    SPI_Cmd(SPI1, ENABLE);    // 使能SPI外设

	GPIO_WriteBit(GPIOA, GPIO_Pin_5, 1);
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, 1);
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, 1);
}


void SPI1_Init_2(void)
{
    /* 定义SPI结构体变量 */
    GPIO_InitTypeDef  GPIO_InitStructure;
    SPI_InitTypeDef   SPI_InitStructure;
 
    /* SPI的IO口和SPI外设打开时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
 
    /* SPI的IO口设置 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     // 复用输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    /* SPI的基本配置 */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  // 设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;     // 设置SPI工作模式:设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b; // 设置SPI的数据大小:SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;       // 串行同步时钟的空闲状态为高电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;      // 串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;         // NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;// 定义波特率预分频的值:波特率预分频值为256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;// 指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 0;          // CRC值计算的多项式
    SPI_Init(SPI1, &SPI_InitStructure);               // 根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
    SPI_Cmd(SPI1, ENABLE);    // 使能SPI外设

	GPIO_WriteBit(GPIOA, GPIO_Pin_5, 1);
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, 1);
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, 1);
}


void SPI1_WriteByte(uint16_t TxData)
{                     
	SPI_I2S_SendData(SPI1, TxData);                                 // 通过外设SPI1发送一个byte数据
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET); // 等待发送区空  
}


void SPI1_WriteMultByte(uint16_t *TxData, uint64_t len)
{
	while(len -- ){
    	SPI_I2S_SendData(SPI1, *TxData);                                 // 通过外设SPI1发送一个byte数据
		TxData ++;
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET); // 等待发送区空  
	}
}

static void tft_delay(int delay_ms)
{
    for(volatile int i = 0; i < delay_ms; i++) {
        for(volatile int j = 0; j < 10; j++);;
    }
}

void tft_spi_write_byte(uint16_t data)
{
	uint8_t i;
	GPIO_WriteBit(SPI_SCK_PORT, SPI_SCK_PIN, 1); // 时钟线为高，开始时钟周期
	for(i = 0;i < 8;i++)
	{
		GPIO_WriteBit(SPI_SCK_PORT, SPI_SCK_PIN, 0); // 时钟线为高，开始时钟周期
	#if 0
        SPI_MOSI((Byte & 0x80) >> 7);
	#else
		if(data & 0x80)
		{
			GPIO_WriteBit(SPI_MOSI_PORT, SPI_MOSI_PIN, 1);
		}
		else
		{
			GPIO_WriteBit(SPI_MOSI_PORT, SPI_MOSI_PIN, 0);
		}
	#endif
        data <<= 1;
		GPIO_WriteBit(SPI_SCK_PORT, SPI_SCK_PIN, 1); // 时钟线为高，开始时钟周期
	}
}

void tft_write_cmd(uint16_t cmd)
{
    GPIO_WriteBit(SPI_DC_PORT, SPI_DC_PIN, 0); // 设置PA0为低电平
    SPI1_WriteByte(cmd);
}

void tft_write_data(uint16_t data)
{
    GPIO_WriteBit(SPI_DC_PORT, SPI_DC_PIN, 1); // 设置PA0为高电平
    SPI1_WriteByte(data);
}

static void tft_set_win(u16 xStar, u16 yStar, u16 xEnd, u16 yEnd)
{
    tft_write_cmd(0x2a);
    //tft_write_data((xStar >> 8) & 0xff);
	tft_write_data(xStar & 0xffff);
   // tft_write_data((xEnd >> 8) & 0xff);
	tft_write_data(xEnd & 0xffff);
    tft_write_cmd(0x2b);
    //tft_write_data((yStar >> 8) & 0xff);
	tft_write_data(yStar & 0xffff );
    //tft_write_data((yEnd >> 8) & 0xff);
	tft_write_data(yEnd & 0xffff);
    tft_write_cmd(0x2c); 
}


void st7789_init(int rotation)
{
    tft_write_cmd(0x11);
    tft_delay(10);
	tft_write_cmd(0x3A);
	tft_write_data(0x05);
	tft_write_cmd(0xc5);
	tft_write_data(0x1a);
	tft_write_cmd(0x36); //rotation
	if(rotation == 0) {
		tft_write_data(0x00);
	}
	else if(rotation == 90) {
		tft_write_data(0xc0);
	}
	else if(rotation == 180) {
		tft_write_data(0x70);
	}
	else if(rotation == 270){
		tft_write_data(0xa0);
	}

//-------------ST7789V Frame rate setting-----------//
	tft_write_cmd(0xb2);
	tft_write_data(0x05);
	tft_write_data(0x05);
	tft_write_data(0x00);
	tft_write_data(0x33);
	tft_write_data(0x33);
	tft_write_cmd(0xb7);
	tft_write_data(0x35);
//--------------ST7789V Power setting---------------//
	tft_write_cmd(0xbb);
	tft_write_data(0x3f);
	tft_write_cmd(0xc0);
	tft_write_data(0x2c);
	tft_write_cmd(0xc2);
	tft_write_data(0x01);
	tft_write_cmd(0xc3);
	tft_write_data(0x0f);
	tft_write_cmd(0xc4);
	tft_write_data(0x20);
	tft_write_cmd(0xc6);
	tft_write_data(0x11);
	tft_write_cmd(0xd0);
	tft_write_data(0xa4);
	tft_write_data(0xa1);
	tft_write_cmd(0xe8);
	tft_write_data(0x03);
	tft_write_cmd(0xe9);
	tft_write_data(0x09);
	tft_write_data(0x09);
	tft_write_data(0x08);
	tft_write_cmd(0xe0);
	tft_write_data(0xd0);
	tft_write_data(0x05);
	tft_write_data(0x09);	
	tft_write_data(0x09);
	tft_write_data(0x08);
	tft_write_data(0x14);	
	tft_write_data(0x28);
	tft_write_data(0x33);
	tft_write_data(0x3f);
	tft_write_data(0x07);
	tft_write_data(0x13);
	tft_write_data(0x14);
	tft_write_data(0x28);
	tft_write_data(0x30);	
	tft_write_cmd(0xe1);
	tft_write_data(0xd0);
	tft_write_data(0x05);
	tft_write_data(0x09);	
	tft_write_data(0x09);
	tft_write_data(0x08);
	tft_write_data(0x03);	
	tft_write_data(0x24);
	tft_write_data(0x32);
	tft_write_data(0x32);
	tft_write_data(0x3b);
	tft_write_data(0x14);
	tft_write_data(0x13);
	tft_write_data(0x28);
	tft_write_data(0x2f);
	tft_write_cmd(0x21);
	tft_write_cmd(0x11);
    tft_delay(120);      //Delay 120ms
    tft_write_cmd(0x29); 
}



void tft_init(void)
{

    tft_gpio_init();
    GPIO_WriteBit(SPI_RST_PORT, SPI_RST_PIN, 1);
    tft_delay(200000);
    GPIO_WriteBit(SPI_RST_PORT, SPI_RST_PIN, 0);
    tft_delay(200000);
    GPIO_WriteBit(SPI_RST_PORT, SPI_RST_PIN, 1);
    tft_delay(200000);

    st7789_init(0);
    tft_delay(20);
    tft_set_win(0, 0, 239, 239);
	GPIO_WriteBit(SPI_DC_PORT, SPI_DC_PIN, 1); // 设置PA0为高电平
    for(int i=0;i<240*240;i++){
        SPI1_WriteByte(0xFF);
        SPI1_WriteByte(0xFF);
    }

	SPI1_Init_2();
}


void demo_panel_flush_area(int16_t x, int16_t y, int16_t w, int16_t h, sgl_color_t *src)
{
    tft_set_win(x, y, x + w - 1, y + h - 1);
	GPIO_WriteBit(SPI_DC_PORT, SPI_DC_PIN, 1); // 设置PA0为高电平
	SPI1_WriteMultByte((uint16_t*)src, w * h);
}
