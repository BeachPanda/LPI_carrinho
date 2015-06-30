/**
  ******************************************************************************
  * FILE      main.c
  * AUTH      M.Phuong
  * Revision  v1.0
  * DATE      14/01/2013
  * DESC      USB Host HID Mouse, Keyboard demo      
  ******************************************************************************
  */
	

#ifndef DEBUG
	#define DEBUG
#endif

#include "main.h"
#include "general.h"
#include "car.h"
#include "interface.h"
#include "serie2.h"
#include "ESP8266.h"
#include "debug.h"

int CYCLE_ENABLED = 1;

void sensorAlert(uint16_t stat);
void manualControl(car car);

car mrCar;

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{
	char buff[2][BUFFER_SIZE];
	char *input;
	char flag;
	int i = 0;
	
  RCC_ClocksTypeDef RCC_Clocks;
	
  generalInit();
	ORANG_ON();
	
#ifdef DEBUG
	uart2_init();
#endif

	DBG ("System Initializing...\n");
	
	input = buff[0];
	
	  /* SysTick end of count event each 1ms */
  RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);  
	
		/* Init Host Library */
  USBH_Init(&USB_OTG_Core_dev,
            USB_OTG_FS_CORE_ID,				
            &USB_Host,
            &HID_cb,
            &USR_Callbacks);

	carInit(&mrCar); 
	USBH_Process(&USB_OTG_Core_dev , &USB_Host);

	carInit(&mrCar);
	initESP();
	Delay(100);
	
	DBG ("Done!\n");
  DBG ("System Ready\n");  
	ORANG_OFF();

	playHorn();
	
	_putchar2(PROMPT);

  while (1)
  {
		if(CYCLE_ENABLED)
		{
			/* Host Task handler */
			USBH_Process(&USB_OTG_Core_dev , &USB_Host);				
			sensorAlert(updateCarStatus(&mrCar));
			updateCarStatus(&mrCar);
			//manualControl(mrCar);
			
			Delay(1);
		}
		if(!(isInBEmpty2())){
			flag = promptInput(buff, input);
			if(flag != UNDEFINED){
				_putchar2('\n');

				if(flag == VALID){
					i = getCommand(&input);

					if(i >= 0)
					{
						if(commandVec[i].func(input) == ERROR){
							printf("%s", commandVec[i].help);
						}else strcpy(buff[1], buff[0]);
					}
					else printf("Unknown Command!\n");
				}
				else if(flag == INVALID) printf("Invalid character!\n");
				_putchar2(PROMPT);
				input = buff[0];
			}
		}
  }
}

void manualControl(car car)
{
	char c;
	float dir = 0;
	float vel = 0.6;
	//override manual control if obstacle or something wrong
	if(!isInBEmpty2()){	
		c = _getkey2();
	if(c == '+')
		dir += (float)0.05;
	else if(c == '-')
		dir -= (float)0.05;
	else if(c == 'u')
		vel += (float)0.05;
	else if(c == 'd')
		vel -= (float)0.05;
	else if(c == 'c')
		printf("Px %lld\nPy %lld\n\n", car.pos->absX, car.pos->absY);
	}
}

void sensorAlert(uint16_t stat)
{	
	if(CHCKSTAT(stat, IR_FRONTR)){
		printf("FR!\n");
	}
	if(CHCKSTAT(stat, IR_FRONTL)){
		printf("IL!\n");
	}
	if(CHCKSTAT(stat, IR_MIDDLR)){
		printf("MR!\n");
	}
	if(CHCKSTAT(stat, IR_MIDDLL)){
		printf("ML!\n");
	}
	if(CHCKSTAT(stat, IR_BACKR)){
		printf("BR!\n");
	}
	if(CHCKSTAT(stat, IR_BACKL)){
		printf("BL!\n");
	}
	if(CHCKSTAT(stat, UPSIDEDOWN)){
		printf("UD!\n");
	}
	if(CHCKSTAT(stat, RIGHTSIDE)){
		printf("RS!\n");
	}
	if(CHCKSTAT(stat, LEFTSIDE)){
		printf("LS!\n");
	}	
}

//user button
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
		CYCLE_ENABLED = !CYCLE_ENABLED;
  }
}


#ifdef USE_FULL_ASSERT
/**
* @brief  assert_failed
*         Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  File: pointer to the source file name
* @param  Line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line
  number,ex: printf("Wrong parameters value: file %s on line %d\r\n",
  file, line) */

  /* Infinite loop */
  while (1)
  {}
}

#endif

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 1 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

