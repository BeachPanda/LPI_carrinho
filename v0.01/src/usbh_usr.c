/**
  ******************************************************************************
  * @file    usbh_usr.c
  * @author  MCD Application Team and modified by Yuuichi Akagawa
  * @version V2.0.0
  * @date    22-July-2011, 2012/03/05
  * @brief   This file includes the user application layer
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usbh_usr.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include <math.h>
#include "mouse.h"
#include "car.h"
#include "define.h"


#ifdef MOUSE
	#define DEBUG1
#endif
#include "debug.h"

/** @addtogroup USBH_USER
* @{
*/

/** @addtogroup USBH_HID_DEMO_USER_CALLBACKS
* @{
*/

/** @defgroup USBH_USR
* @brief This file is the Header file for usbh_usr.c
* @{
*/ 


/** @defgroup USBH_CORE_Exported_TypesDefinitions
* @{
*/ 
#define KYBRD_FIRST_COLUMN               (uint16_t)319
#define KYBRD_LAST_COLUMN                (uint16_t)7
#define KYBRD_FIRST_LINE                 (uint8_t)120
#define KYBRD_LAST_LINE                  (uint8_t)200

////////////////////////////////////////////////
mouse mouz;
///////////////////////////////////////////////



/**
* @}
*/ 

/** @addtogroup USER
* @{
*/

/** @defgroup USBH_USR 
* @brief    This file includes the user application layer
* @{
*/ 

/** @defgroup USBH_CORE_Exported_Types
* @{
*/ 



/** @defgroup USBH_USR_Private_Defines
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_USR_Private_Macros
* @{
*/ 
/**
* @}
*/ 

/** @defgroup USBH_USR_Private_Variables
* @{
*/
extern USB_OTG_CORE_HANDLE           USB_OTG_Core_dev;
/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USR_Callbacks =
{
  USBH_USR_Init,
  USBH_USR_DeInit,
  USBH_USR_DeviceAttached,
  USBH_USR_ResetDevice,
  USBH_USR_DeviceDisconnected,
  USBH_USR_OverCurrentDetected,
  USBH_USR_DeviceSpeedDetected,
  USBH_USR_Device_DescAvailable,
  USBH_USR_DeviceAddressAssigned,
  USBH_USR_Configuration_DescAvailable,
  USBH_USR_Manufacturer_String,
  USBH_USR_Product_String,
  USBH_USR_SerialNum_String,
  USBH_USR_EnumerationDone,
  USBH_USR_UserInput,
  NULL,
  USBH_USR_DeviceNotSupported,
  USBH_USR_UnrecoveredError
};

/**
* @}
*/

/** @defgroup USBH_USR_Private_Constants
* @{
*/ 




/** @defgroup USBH_USR_Private_FunctionPrototypes
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_USR_Private_Functions
* @{
*/ 

/*--------------- Messages ---------------*/
const uint8_t MSG_HOST_INIT[]          = "> Host Library Initialized";
const uint8_t MSG_DEV_ATTACHED[]       = "> Device Attached";
const uint8_t MSG_DEV_DISCONNECTED[]   = "> Device Disconnected";
const uint8_t MSG_DEV_ENUMERATED[]     = "> Enumeration completed";
const uint8_t MSG_DEV_HIGHSPEED[]      = "> High speed device detected";
const uint8_t MSG_DEV_FULLSPEED[]      = "> Full speed device detected";
const uint8_t MSG_DEV_LOWSPEED[]       = "> Low speed device detected";
const uint8_t MSG_DEV_ERROR[]          = "> Device fault";

const uint8_t MSG_MSC_CLASS[]          = "> Mass storage device connected";
const uint8_t MSG_HID_CLASS[]          = "> HID device connected";

const uint8_t USB_HID_MouseStatus[]    = "> HID Demo Device : Mouse";
const uint8_t USB_HID_KeybrdStatus[]   = "> HID Demo Device : Keyboard";
const uint8_t MSG_UNREC_ERROR[]        = "> UNRECOVERED ERROR STATE";
/**
* @}
*/



/**
* @brief  USBH_USR_Init 
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
void USBH_USR_Init(void)
{

}

/**
* @brief  USBH_USR_DeviceAttached 
*         Displays the message on LCD on device attached
* @param  None
* @retval None
*/
void USBH_USR_DeviceAttached(void)
{  
  DBG ("Device Attached");
}

/**
* @brief  USBH_USR_UnrecoveredError
* @param  None
* @retval None
*/
void USBH_USR_UnrecoveredError (void)
{

}

/**
* @brief  USBH_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval None
*/
void USBH_USR_DeviceDisconnected (void)
{
  DBG ("Device Disconnected"); 
}

/**
* @brief  USBH_USR_ResetUSBDevice 
*         Reset USB Device
* @param  None
* @retval None
*/
void USBH_USR_ResetDevice(void)
{
  /* Users can do their application actions here for the USB-Reset */
}


/**
* @brief  USBH_USR_DeviceSpeedDetected 
*         Displays the message on LCD for device speed
* @param  Devicespeed : Device Speed
* @retval None
*/
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
  if(DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED)
  {
		DBG ("%s", MSG_DEV_HIGHSPEED);
  }  
  else if(DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED)
  {
		DBG ("%s", MSG_DEV_FULLSPEED);		
  }
  else if(DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED)
  {
		DBG ("%s", MSG_DEV_LOWSPEED);		
  }
  else
  {
		DBG ("%s", MSG_DEV_ERROR);		
  }
}

/**
* @brief  USBH_USR_Device_DescAvailable 
*         Displays the message on LCD for device descriptor
* @param  DeviceDesc : device descriptor
* @retval None
*/
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
  USBH_DevDesc_TypeDef *hs;
  hs = DeviceDesc;  

	DBG ("VID : %04Xh" , (uint32_t)(*hs).idVendor);
	DBG ("PID : %04Xh" , (uint32_t)(*hs).idProduct);
}

/**
* @brief  USBH_USR_DeviceAddressAssigned 
*         USB device is successfully assigned the Address 
* @param  None
* @retval None
*/
void USBH_USR_DeviceAddressAssigned(void)
{
  
}

/**
* @brief  USBH_USR_Conf_Desc 
*         Displays the message on LCD for configuration descriptor
* @param  ConfDesc : Configuration descriptor
* @retval None
*/
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
  USBH_InterfaceDesc_TypeDef *id;
  
  id = itfDesc;  
  
  if((*id).bInterfaceClass  == 0x08)
  {
		DBG ("%s\n", MSG_MSC_CLASS);
  }
  else if((*id).bInterfaceClass  == 0x03)
  {
		DBG ("%s", MSG_HID_CLASS);
  }   
}

/**
* @brief  USBH_USR_Manufacturer_String 
*         Displays the message on LCD for Manufacturer String 
* @param  ManufacturerString : Manufacturer String of Device
* @retval None
*/
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
	DBG ("Manufacturer : %s", (char *)ManufacturerString);
}

/**
* @brief  USBH_USR_Product_String 
*         Displays the message on LCD for Product String
* @param  ProductString : Product String of Device
* @retval None
*/
void USBH_USR_Product_String(void *ProductString)
{
  DBG ("Product : %s", (char *)ProductString); 
}

/**
* @brief  USBH_USR_SerialNum_String 
*         Displays the message on LCD for SerialNum_String 
* @param  SerialNumString : SerialNum_String of device
* @retval None
*/
void USBH_USR_SerialNum_String(void *SerialNumString)
{
  DBG ("Serial Number : %s", (char *)SerialNumString);  	
} 

/**
* @brief  EnumerationDone 
*         User response request is displayed to ask for
*         application jump to class
* @param  None
* @retval None
*/
void USBH_USR_EnumerationDone(void)
{
   DBG ("Enumeration done!");  
} 

/**
* @brief  USBH_USR_DeviceNotSupported
*         Device is not supported
* @param  None
* @retval None
*/
void USBH_USR_DeviceNotSupported(void)
{         
	 DBG ("Device is not supported!"); 
}  

/**
* @brief  USBH_USR_UserInput
*         User Action for application state entry
* @param  None
* @retval USBH_USR_Status : User response for key button
*/
USBH_USR_Status USBH_USR_UserInput(void)
{
  
  USBH_USR_Status usbh_usr_status;
  
  usbh_usr_status = USBH_USR_NO_RESP;  
  
	DBG ("USBH_USR_UserInput!"); 
    
  usbh_usr_status = USBH_USR_RESP_OK;
  
  return usbh_usr_status;  
} 

/**
* @brief  USBH_USR_OverCurrentDetected
*         Device Overcurrent detection event
* @param  None
* @retval None
*/
void USBH_USR_OverCurrentDetected (void)
{
  
}

/**
* @brief  USR_MOUSE_Init
*         Init Mouse window
* @param  None
* @retval None
*/

void USR_MOUSE_Init	(void)
{
  mouz.diffX = 0;
	mouz.diffY = 0;
	mouz.absX = 0;
	mouz.absY = 0;
	mouz.teta = 90;
}

/**
* @brief  USR_MOUSE_ProcessData
*         Process Mouse data
* @param  data : Mouse data to be displayed
* @retval None
*/	

void getAxisMov(int64_t *x, int64_t *y, float a)
{
	*x = cosf((a) * (float)3.141592 / 180) * CONV_TO_UNIT(RADIUS);
	*y = sinf((a) * (float)3.141592 / 180) * CONV_TO_UNIT(RADIUS);
}

float fixAngle(float a)
{
	while ((int)a >= 360)
		a -= 360;

	while ((int)a < 0)
		a += 360;
	
	return a;
}

extern void Delay(uint32_t);

extern car mrCar;

void USR_MOUSE_ProcessData(HID_MOUSE_Data_TypeDef *data)
{ 
	int64_t x = 0;
	int64_t y = 0;
	
	int8_t dx = 0;
	int8_t dy = 0;
	
	static int32_t yInteg = 0;
	static int16_t integCount = 0;
	
	float a;

	dx = (char)(data->x);	
	//dy = -(char)(data->y);	

	if((dx != 0) && ((dy < 100) && (dy > -100)))
	{
		a = CONV_TO_D(-dx) / PERIMETER * 360; // angulo de rota��o em graus
		
		mouz.teta = fixAngle(mouz.teta + a); // angulo actual	
	}
	else if((dy != 0) && (dx == 0)){
		mouz.absY += dy * sinf(mouz.teta * (float)3.141592 / 180);
		mouz.absX += dy * cosf(mouz.teta * (float)3.141592 / 180);
	} 
	else {
//		yInteg += dy;
//		if(yInteg > 1000)
//			integCount++;
//		else if(yInteg < -1000)
//			integCount--;
		if(dx > 0)
			setPWMLeftF(mrCar.ornt->left - 1);
		
		if(dx < 0)
			setPWMRightF(mrCar.ornt->right - 1);
		
		if((integCount > 10) || (integCount < -10)){		
			DBG("PANIC! LOST REFERENCE!");		
			while(1){
				TIM_Cmd(TIM3, DISABLE);
				turn(25);
				Delay(2000);
				turn(-25);
				Delay(2000);
			}
		}
	}
	
//	DBG1("");
//	DBG1("Dx %d", dx);
//	DBG1("Dy %d", dy);
	
	/*
	mouz.absX += dx;
	mouz.absY += dy;
	*/
	
	//DBG1("");
	//DBG1("Px %2lld", mouz.absX);
	//DBG1("Py %2lld", mouz.absY);
//	DBG1("Or %2.1f", mouz.teta);
}

/**
* @brief  USR_KEYBRD_Init
*         Init Keyboard window
* @param  None
* @retval None
*/
void  USR_KEYBRD_Init (void)
{

}


/**
* @brief  USR_KEYBRD_ProcessData
*         Process Keyboard data
* @param  data : Keyboard data to be displayed
* @retval None
*/
void  USR_KEYBRD_ProcessData (uint8_t data)
{
	DBG ("Key code %X \n", data);
}

/**
* @brief  USBH_USR_DeInit
*         Deint User state and associated variables
* @param  None
* @retval None
*/
void USBH_USR_DeInit(void)
{
}

/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/

/**
* @}
*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
