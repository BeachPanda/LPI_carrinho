/**
  ******************************************************************************
  * FILE      main.c
  * AUTH      M.Phuong
  * Revision  v1.0
  * DATE      14/01/2013
  * DESC      USB Host HID Mouse, Keyboard demo      
  ******************************************************************************
 */
 
 
#include "main.h"
#include "general.h"
#include "car.h"
#include "joyStick.h"
#include "ESP8266.h"
#include "interface.h"

#include "debug.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "nRF24l01P.h"


int CYCLE_ENABLED = 1;

void init(RCC_ClocksTypeDef *RCC_Clocks);
void sensorAlert(uint16_t stat);
void manualControl(void);

unsigned char i;
unsigned char ADD[3]={0};
unsigned char tx_buff[32] = {0};
unsigned char rx_buff[32] = {0};


car mrCar;

char reset ;
/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/

char easterEGG = 0;

void easterEgg()
{
	printf("In my talons, \n"
	"I shape clay, crafting life forms as I please. \n"
	"Around me is a burgeoning empire of steel. \n"
	"From my throne room, lines of power careen into the skies of Earth. \n"
	"My whims will become lightning bolts that devastate the mounds of humanity. \n"
	"Out of the chaos, they will run and whimper, praying for me to end their tedious anarchy. \n"
	"I am drunk with this vision. God. The title suits me well.\n");
}

int main(void)
{	
	char buff[2][BUFFER_SIZE];
	char *input;
	char flag;
	int i = 0;
	unsigned int late = 0;
	
	RCC_ClocksTypeDef RCC_Clocks;
	
	reset = 1;
	input = buff[0];

	button_setup();
	
	if((GPIOA->IDR) & 0x01)
		easterEGG = 1;
		
	//while(!((~GPIOA->IDR) & 0x01));
	
	generalInit();

  while (1){		
		if(reset)
		{
			//ORANG_ON();
			init(&RCC_Clocks);
			//ORANG_OFF();

			DBG("%c", PROMPT);
			reset = 0;
		}
		
		if (CYCLE_ENABLED && (late++ > 0xEfff))	{
			// Host Task handler
			USBH_Process(&USB_OTG_Core_dev , &USB_Host);				
			//sensorAlert(updateCarStatus(&mrCar));
			updateCarStatus(&mrCar);
			//manualControl();
			late = 0;			
		}
		
		//if (0){
		if (!(isInBEmpty1())) {
			flag = promptInput(buff, input);
			if(flag != UNDEFINED){
				DBG("\n");

				if(flag == VALID){
					i = getCommand(&input);

					if(i >= 0)
					{
						if(commandVec[i].func(input) == ERROR){
							DBG("%s", commandVec[i].help);
						}else strcpy(buff[1], buff[0]);
					}
					else DBG("Unknown Command!");
				}
				else if(flag == INVALID) DBG("Invalid character!");
				_putchar2(PROMPT);
				input = buff[0];
			}
		}
  }
}


void init(RCC_ClocksTypeDef *RCC_Clocks)
{
	/* INIT EVERYTHING */	
	
	  /* SysTick end of count event each 1ms */
  RCC_GetClocksFreq(RCC_Clocks);
	SysTick_Config(RCC_Clocks->HCLK_Frequency / 1000);  
	
	usart1_init();

	if(easterEGG)
		easterEgg();
	
	DBG ("System Initializing...");
	DBG ("Initializing Car");
	carInit(&mrCar);
	DBG("Car Running");
	
	DBG("Initializing Sensors");
	//IRinit();
	//accelInit();
	DBG("SSensing");
	
	DBG("Initializing Wireless Coms");
	//ESPinit();
	//joyStickInit();
	//DBG("RF test Receive:");
	//joyStickTestR();
	
	//DBG("RF test Transmit:\n");
	//joyStickTestT();
	
	DBG("No one to talk to...");
	
	DBG("Initializing Advanced Position Control Unit");
	
		/* Init Host Library */
  USBH_Init(&USB_OTG_Core_dev,
            USB_OTG_FS_CORE_ID,				
            &USB_Host,
            &HID_cb,
            &USR_Callbacks);
	
	USBH_Process(&USB_OTG_Core_dev , &USB_Host);
	
	Delay(100);

	DBG("Done!");
	DBG("All sectors nominal");
  DBG1("System Ready");  
	
	playHorn();
}

void sensorAlert(uint16_t stat)
{	
	if(CHCKSTAT(stat, IR_FRONTR)){
		DBG("FR!");
	}
	if(CHCKSTAT(stat, IR_FRONTL)){
		DBG("IL!");
	}
	if(CHCKSTAT(stat, IR_MIDDLR)){
		DBG("MR!");
	}
	if(CHCKSTAT(stat, IR_MIDDLL)){
		DBG("ML!");
	}
	if(CHCKSTAT(stat, IR_BACKR)){
		DBG("BR!");
	}
	if(CHCKSTAT(stat, IR_BACKL)){
		DBG("BL!");
	}
	if(CHCKSTAT(stat, UPSIDEDOWN)){
		DBG("UD!");
	}
	if(CHCKSTAT(stat, RIGHTSIDE)){
		DBG("RS!");
	}
	if(CHCKSTAT(stat, LEFTSIDE)){
		DBG("LS!");
	}	
}

//user button
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
		//CYCLE_ENABLED = !CYCLE_ENABLED;
		mrCar.pos->absX = 0;
		mrCar.pos->absY = 0;
		mrCar.pos->teta = 90;
		DBG1("ST 0");
		reset = 1;
  }
}

void manualControl()
{
	static float dir = 0;
	static float vel = 0.5;
	
	char c = 0;
	
	if (!isInBEmpty2()){
		c = _getkey2();
		if(c == '+')
			dir += (float)0.1;
		else if (c == '-')
			dir -= (float)0.1;
		else if (c == 'u')
			vel += (float)0.1;
		else if (c == 'd')
			vel -= (float)0.1;
		
		if(vel > 1) 
			vel = 1;
		if(vel < -1) 
			vel = -1;
		
		if(dir > 1) 
			dir = 1;
		if(dir < -1) 
			dir = -1;
		//setOrientation(dir, vel, 0, 0);
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

