#include "joyStick.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"
#include <stdio.h>

extern void Delay(uint32_t);

void joyStickTestR(void)
{
	//unsigned char ADD[3]={0};
	//unsigned char tx_buff[32] = {0};
	unsigned char rx_buff[32] = {0};
	unsigned int i = 0;
	unsigned char status;
	
	printf("LOL!\n");
	
	while (1)
  {
		RX_Mode();
		if(nRF24L01_RxPacket(rx_buff))
		{			
			 for(i = 0; i<32; i++)
				 printf("%02x", rx_buff[i]);
			
			 printf("\n");
			
			 for(i = 0; i < 32; i++)
				 printf("%c",rx_buff[i]);
			 
			 printf("\n");
			 printf("\n");
		}
		
		status = SPI_RD_Reg(FIFO_STATUS);
		printf(" fifo 0x%02X\n", status);
		
		status = SPI_RD_Reg(NRFRegSTATUS);
		printf(" status 0x%02X\n", status);
		
//		status = SPI_RD_Reg(RF_CH);
// 		printf(" rf channel 0x%02X\n",status);
//		
// 		status = SPI_RD_Reg(RF_SETUP);
// 		printf(" rf set up 0x%02X\n",status);
//		
// 		status = SPI_RD_Reg(CONFIG);
// 		printf(" config 0x%02X\n",status);
//		
// 		status = SPI_RD_Reg(RX_PW_P0);
// 		printf(" number of byte P0 %d\n",status);
		
		printf("\n");
 		Delay(500);	
  }
}

void joyStickTestT(void)
{
	unsigned char tx_buff[32] = {0};
	unsigned char status;
	unsigned char Tx_Buf[32] = {"Ola' amigos, tudo bem com voces?"};
	
	nRF24L01_HW_Init();
	TX_Mode();
	SPI_WR_Reg(WRITE_nRF_REG + CONFIG, 0x38); // enable power up and ptx

 	Delay(500);
 	SPI_WR_Reg(WRITE_nRF_REG + NRFRegSTATUS, 0x70); // Write 1 to clear bit
	
  while (1)
  {
 		nRF24L01_TxPacket(Tx_Buf);
		printf("%s\n", Tx_Buf);
		
		Delay(10);
		
		status = SPI_RD_Reg(NRFRegSTATUS);

		status = SPI_RD_Reg(FIFO_STATUS);
		printf(" fifo 0x%02X\n", status);
		
		status = SPI_RD_Reg(NRFRegSTATUS);
		printf(" status 0x%02X\n", status);
		
//		status = SPI_RD_Reg(RF_CH);
// 		printf(" rf channel 0x%02X\n",status);
//		
// 		status = SPI_RD_Reg(RF_SETUP);
// 		printf(" rf set up 0x%02X\n",status);
//		
// 		status = SPI_RD_Reg(CONFIG);
// 		printf(" config 0x%02X\n",status);
		
		SPI_WR_Reg(WRITE_nRF_REG + CONFIG, 0x3B); // enable power up and prx
		
		printf("\n");
		
		Delay(3000);
  }
}


void joyStickInit(void)
{
	nRF24L01_HW_Init();	
}
