
#define SERIE_C_1

#include "serie1.h"
#include "misc.h" 

#define DISABLE_SERIAL_INTERRUPTS() USART_ITConfig(USART1, USART_IT_RXNE , DISABLE)
#define ENABLE_SERIAL_INTERRUPTS() USART_ITConfig(USART1, USART_IT_RXNE , ENABLE)

//TODO: inicializar interrupção
void usart1_init(void)
{
	GPIO_InitTypeDef 	GPIO_InitStruct;//GPIO CONFIG
	USART_InitTypeDef USART_InitStruct;//USART CONFIG
	NVIC_InitTypeDef NVIC_InitStructure;//USART INTERRUPT CONFIG
		
		/*INIT GPIO*/		
	// Enable clock for GPIOB
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);


	
	// Initialize pins as alternating function
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
	
		/*INIT USART1*/
	/**
	 * Enable clock for USART1 peripheral
	 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

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

	USART_Init(USART1, &USART_InitStruct);	
	
	/*Interrupt usart2*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // enable the USART1 receive interrupt
	
	USART_Cmd(USART1, ENABLE);
	
	
	istart1 = 0;
	iend1 = 0;
	ostart1 = 0;
	oend1 = 0;
	sendfull1 = 0;
	sendactive1 = 0;
}

char _getkey1(void)
{
	char c;

	while (iend1 == istart1); // wait until there are characters
	
	USART1->CR1 &= ~USART_FLAG_RXNE;
	
	c = inbuf1[istart1++ & (ILEN1-1)];

	USART1->CR1 |= USART_FLAG_RXNE;
	
	return c;
}

void putString1(char *c)
{
	while (*c){
		_putchar1(*c);
		c++;
	}
}

char _putchar1(char c)
{
	if (c == '\n') { // expand new line character:
		while (sendfull1); // wait until there is space in buffer
		putbuf1 (0x0D); // send CR before LF for <new line>
	}
	while (sendfull1); // wait until there is space in buffer
	putbuf1(c); // place character into buffer
	return c;
}

/* putbuf: write a character to SBUF or transmission buffer */
void putbuf1(char c)
{
	if (!sendfull1) { // transmit only if buffer not full
		if (!sendactive1) { // if transmitter not active:
			USART1->CR1 |= USART_FLAG_TXE;	      // enable TX interrupt
			sendactive1 = 1; // transfer first character direct
			USART1->DR = c; // to BUFF to start transmission		
		}
		else {
			//DISABLE_SERIAL_INTERRUPTS(); // disable serial interrupts during buffer update
			USART1->CR1 &= ~USART_FLAG_TXE;
			outbuf1[oend1++ & (OLEN1-1)] = c; // put char to transmission buffer
			if (((oend1 ^ ostart1) & (OLEN1-1)) == 0) {
				sendfull1 = 1;
			} // set flag if buffer is full

			USART1->CR1 |= USART_FLAG_TXE;	      // enable TX interrupt
			//ENABLE_SERIAL_INTERRUPTS();// enable serial interrupts again
		}
	}
}

void USART1_IRQHandler (void)
{
	volatile unsigned int IIR;

	IIR = USART1->SR;
	
	if (IIR & USART_FLAG_RXNE){ // read interrupt
                  
		 USART1->SR &= ~USART_FLAG_RXNE;	         // clear interrupt flag
			
		if (istart1 + ILEN1 != iend1)
			inbuf1[iend1++ & (ILEN1-1)] = USART1->DR; // put character into buffer
	}
	
	else if (IIR & USART_FLAG_TXE){
		USART1->SR &= ~USART_FLAG_TXE;	          // clear interrupt flag
		if ((ostart1 != oend1)) { // if characters in buffer
			USART1->DR = outbuf1[ostart1++ & (OLEN1-1)]; // transmit character
			sendfull1 = 0; // clear 'sendfull' flag
		}
		else { // if all characters transmitted
			sendactive1 = 0; // clear 'sendactive'			
			USART1->CR1 &= ~USART_FLAG_TXE;
		}
	}
}

