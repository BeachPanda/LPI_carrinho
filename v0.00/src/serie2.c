#include "serie2.h"
#include "misc.h" 

#define OLEN2 64 // size of serial transmission buffer
volatile unsigned int ostart2; // transmission buffer start index
volatile unsigned int oend2; // transmission buffer end index
volatile char outbuf2[OLEN2]; // storage for transmission buffer

#define ILEN2 64 // size of serial receiving buffer
volatile unsigned int istart2; // receiving buffer start index
volatile unsigned int iend2; // receiving buffer end index
volatile char inbuf2[ILEN2]; // storage for receiving buffer

volatile char sendfull2; // flag: marks transmit buffer full
volatile char sendactive2; // flag: marks transmitter active

#define DISABLE_SERIAL_INTERRUPTS() USART_ITConfig(USART2, USART_IT_RXNE , DISABLE)
#define ENABLE_SERIAL_INTERRUPTS() USART_ITConfig(USART2, USART_IT_RXNE , ENABLE)

//TODO: inicializar interrupção
void uart2_init(void)
{
	GPIO_InitTypeDef 	GPIO_InitStruct;//GPIO CONFIG
	USART_InitTypeDef USART_InitStruct;//USART CONFIG
	NVIC_InitTypeDef NVIC_InitStructure;//USART INTERRUPT CONFIG
		
		/*INIT GPIO*/		
	// Enable clock for GPIOA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/**
	* Tell pins PA2 and PA3 which alternating function you will use
	* @important Make sure, these lines are before pins configuration!
	*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	// Initialize pins as alternating function
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
		/*INIT USART2*/
	/**
	 * Enable clock for USART1 peripheral
	 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

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
	USART_InitStruct.USART_BaudRate = 2000000;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART2, &USART_InitStruct);
	
	
	/*Interrupt usart2*/
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // enable the USART2 receive interrupt
	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE); // enable the USART2 transmit interrupt
	
	USART_Cmd(USART2, ENABLE);
	
	
	istart2 = 0;
	iend2 = 0;
	ostart2 = 0;
	oend2 = 0;
	sendfull2 = 0;
	sendactive2 = 0;
}

char _getkey2 (void)
{
	char c;

	while (iend2 == istart2); // wait until there are characters
	
	USART2->CR1 &= ~USART_FLAG_RXNE;
	
	c = inbuf2[istart2++ & (ILEN2-1)];

	USART2->CR1 |= USART_FLAG_RXNE;
	
	return c;
}

char _putchar2 (char c)
{
	if (c == '\n') { // expand new line character:
		while (sendfull2); // wait until there is space in buffer
		putbuf2 (0x0D); // send CR before LF for <new line>
	}
	while (sendfull2); // wait until there is space in buffer
	putbuf2 (c); // place character into buffer
	return c;
}

/* putbuf: write a character to SBUF or transmission buffer */
void putbuf2 (char c)
{
	if (!sendfull2) { // transmit only if buffer not full
		if (!sendactive2) { // if transmitter not active:
			USART2->CR1 |= USART_FLAG_TXE;	      // enable TX interrupt
			sendactive2 = 1; // transfer first character direct
			USART2->DR = c; // to BUFF to start transmission		
		}
		else {
			//DISABLE_SERIAL_INTERRUPTS(); // disable serial interrupts during buffer update

			USART2->CR1 &= ~USART_FLAG_TXE;
			outbuf2[oend2++ & (OLEN2-1)] = c; // put char to transmission buffer
			if (((oend2 ^ ostart2) & (OLEN2-1)) == 0) {
				sendfull2 = 1;
			} // set flag if buffer is full

			USART2->CR1 |= USART_FLAG_TXE;	      // enable TX interrupt
			//ENABLE_SERIAL_INTERRUPTS();// enable serial interrupts again
		}
	}
}

void putString2(char *c)
{
	while (*c){
		_putchar2(*c);
		c++;
	}
}

void USART2_IRQHandler (void)
{
	volatile unsigned int IIR;

	IIR = USART2->SR;
	
	if (IIR & USART_FLAG_RXNE){ // read interrupt
                  
		 USART2->SR &= ~USART_FLAG_RXNE;	         // clear interrupt flag
			
		if (istart2 + ILEN2 != iend2)
			inbuf2[iend2++ & (ILEN2-1)] = USART2->DR; // put character into buffer
	}
	
	else if (IIR & USART_FLAG_TXE){
		USART2->SR &= ~USART_FLAG_TXE;	          // clear interrupt flag
		if ((ostart2 != oend2))
		{ // if characters in buffer
			USART2->DR = outbuf2[ostart2++ & (OLEN2-1)]; // transmit character
			sendfull2 = 0; // clear 'sendfull' flag
		}
		else
		{ // if all characters transmitted
			sendactive2 = 0; // clear 'sendactive'			
			USART2->CR1 &= ~USART_FLAG_TXE;
		}
	}
}



