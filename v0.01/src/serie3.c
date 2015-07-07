#include "serie3.h"
#include "misc.h" 

#define OLEN3 64 // size of serial transmission buffer
volatile unsigned int ostart3; // transmission buffer start index
volatile unsigned int oend3; // transmission buffer end index
volatile char outbuf3[OLEN3]; // storage for transmission buffer

#define ILEN3 64 // size of serial receiving buffer
volatile unsigned int istart3; // receiving buffer start index
volatile unsigned int iend3; // receiving buffer end index
volatile char inbuf3[ILEN3]; // storage for receiving buffer

volatile char sendfull3; // flag: marks transmit buffer full
volatile char sendactive3; // flag: marks transmitter active

#define DISABLE_SERIAL_INTERRUPTS() USART_ITConfig(usart3, USART_IT_RXNE , DISABLE)
#define ENABLE_SERIAL_INTERRUPTS() USART_ITConfig(usart3, USART_IT_RXNE , ENABLE)

//TODO: inicializar interrupção
void usart3_init(void)
{
	GPIO_InitTypeDef 	GPIO_InitStruct;//GPIO CONFIG
	USART_InitTypeDef USART_InitStruct;//USART CONFIG
	NVIC_InitTypeDef NVIC_InitStructure;//USART INTERRUPT CONFIG
		
		/*INIT GPIO*/		
	// Enable clock for GPIOA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	/**
	* Tell pins PA3 and PA3 which alternating function you will use
	* @important Make sure, these lines are before pins configuration!
	*/
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
	// Initialize pins as alternating function
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
		/*INIT usart3*/
	/**
	 * Enable clock for USART1 peripheral
	 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/**
	 * Set Baudrate to value you pass to function
	 * Disable Hardware Flow control
	 * Set Mode To TX and RX, so USART will work in full-duplex mode
	 * Disable parity bit
	 * Set 1 stop bit
	 * Set Data bits to 8
	 *
	 * Initialize USART1
	 * Activate USART1
	 */
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART3, &USART_InitStruct);
		
	/*Interrupt usart3*/
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // enable the usart3 receive interrupt
	
	USART_Cmd(USART3, ENABLE);
		
	istart3 = 0;
	iend3 = 0;
	ostart3 = 0;
	oend3 = 0;
	sendfull3 = 0;
	sendactive3 = 0;
}

char _getkey3 (void)
{
	char c;

	while (iend3 == istart3); // wait until there are characters
	
	USART3->CR1 &= ~USART_FLAG_RXNE;
	
	c = inbuf3[istart3++ & (ILEN3-1)];

	USART3->CR1 |= USART_FLAG_RXNE;
	
	return c;
}

char _putchar3 (char c)
{
	if (c == '\n') { // expand new line character:
		while (sendfull3); // wait until there is space in buffer
		putbuf3 (0x0D); // send CR before LF for <new line>
	}
	while (sendfull3); // wait until there is space in buffer
	putbuf3 (c); // place character into buffer
	return c;
}

/* putbuf: write a character to SBUF or transmission buffer */
void putbuf3 (char c)
{
	if (!sendfull3) { // transmit only if buffer not full
		if (!sendactive3) { // if transmitter not active:
			USART3->CR1 |= USART_FLAG_TXE;	      // enable TX interrupt
			sendactive3 = 1; // transfer first character direct
			USART3->DR = c; // to BUFF to start transmission		
		}
		else {
			//DISABLE_SERIAL_INTERRUPTS(); // disable serial interrupts during buffer update

			USART3->CR1 &= ~USART_FLAG_TXE;
			outbuf3[oend3++ & (OLEN3-1)] = c; // put char to transmission buffer
			if (((oend3 ^ ostart3) & (OLEN3-1)) == 0) {
				sendfull3 = 1;
			} // set flag if buffer is full

			USART3->CR1 |= USART_FLAG_TXE;	      // enable TX interrupt
			//ENABLE_SERIAL_INTERRUPTS();// enable serial interrupts again
		}
	}
}

void putString3(char *c)
{
	while (*c){
		_putchar3(*c);
		c++;
	}
}

void USART3_IRQHandler (void)
{
	volatile unsigned int IIR;

	IIR = USART3->SR;
	
	if (IIR & USART_FLAG_RXNE){ // read interrupt
                  
		 USART3->SR &= ~USART_FLAG_RXNE;	         // clear interrupt flag
			
		if (istart3 + ILEN3 != iend3)
			inbuf3[iend3++ & (ILEN3-1)] = USART3->DR; // put character into buffer
	}
	
	else if (IIR & USART_FLAG_TXE){
		USART3->SR &= ~USART_FLAG_TXE;	          // clear interrupt flag
		if ((ostart3 != oend3))
		{ // if characters in buffer
			USART3->DR = outbuf3[ostart3++ & (OLEN3-1)]; // transmit character
			sendfull3 = 0; // clear 'sendfull' flag
		}
		else
		{ // if all characters transmitted
			sendactive3 = 0; // clear 'sendactive'			
			USART3->CR1 &= ~USART_FLAG_TXE;
		}
	}
}



