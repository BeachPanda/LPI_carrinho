#include "Accelerometer.h"
#include "debug.h"

accelerometer accel;

void accelInit()
{
	SPI_InitTypeDef SPI_InitTypeDefStruct;
	GPIO_InitTypeDef GPIO_InitTypeDefStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);

	SPI_InitTypeDefStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
	SPI_InitTypeDefStruct.SPI_Mode              = SPI_Mode_Master;
	SPI_InitTypeDefStruct.SPI_DataSize          = SPI_DataSize_8b;
	SPI_InitTypeDefStruct.SPI_CPOL              = SPI_CPOL_High;
	SPI_InitTypeDefStruct.SPI_CPHA              = SPI_CPHA_2Edge;
	SPI_InitTypeDefStruct.SPI_NSS               = SPI_NSS_Soft;
	SPI_InitTypeDefStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;     //seems like it was here causing trouble
	SPI_InitTypeDefStruct.SPI_FirstBit          = SPI_FirstBit_MSB;
	SPI_InitTypeDefStruct.SPI_CRCPolynomial			= 1;
	
	SPI_DeInit(SPI1);
	SPI_Init(SPI1, &SPI_InitTypeDefStruct);

	GPIO_InitTypeDefStruct.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_6;
	GPIO_InitTypeDefStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDefStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitTypeDefStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitTypeDefStruct);

	GPIO_InitTypeDefStruct.GPIO_Pin   = GPIO_Pin_3;
	GPIO_InitTypeDefStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDefStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitTypeDefStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOE, &GPIO_InitTypeDefStruct);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	GPIO_SetBits(GPIOE, GPIO_Pin_3);
	SPI_Cmd(SPI1, ENABLE);

	SPI_send(ACCEL_CTRL_REG6, 0x01); //reboot
	SPI_send(ACCEL_CTRL_REG3, 0xc9);                         // resetting the accelerometer internal circuit
	SPI_send(ACCEL_CTRL_REG4, 0x67);													// 100Hz data update rate, block data update disable, x/y/z enabled
	SPI_send(ACCEL_CTRL_REG4, 0x67);													// 100Hz data update rate, block data update disable, x/y/z enabled
	SPI_send(ACCEL_CTRL_REG4, 0x67);													// 100Hz data update rate, block data update disable, x/y/z enabled
	SPI_send(ACCEL_CTRL_REG5, 0x20);                         // Anti aliasing filter bandwidth 800Hz, 16G, no self-test, 4-wire interface
}

void SPI_send(uint8_t address, uint8_t data)
{
	GPIO_ResetBits(GPIOE, GPIO_Pin_3);

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, address);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(SPI1);

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, data);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(SPI1);

	GPIO_SetBits(GPIOE, GPIO_Pin_3);
}

uint8_t SPI_read(uint8_t address)
{
	GPIO_ResetBits(GPIOE, GPIO_Pin_3);
	address = 0x80 | address;                         // 0b10 - reading and clearing status

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, address);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(SPI1);

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, 0x00);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	GPIO_SetBits(GPIOE, GPIO_Pin_3);

	return SPI_I2S_ReceiveData(SPI1);
}


void updateAccel()
{
	accel.x = SPI_read(ACCEL_OUT_XL);
	accel.x |= (SPI_read(ACCEL_OUT_XH) << 8);
	
	accel.y = SPI_read(ACCEL_OUT_YL);
	accel.y |= (SPI_read(ACCEL_OUT_YH) << 8);
	
	accel.z = SPI_read(ACCEL_OUT_ZL);
	accel.z |= (SPI_read(ACCEL_OUT_ZH) << 8);


	//DBG("Ax %d\nAy %d\nAz %d\n", accel.x, accel.y, accel.z);
}
