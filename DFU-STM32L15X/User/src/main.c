/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Device Firmware Upgrade(DFU) demo main file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/

#include "hw_config.h" 
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_pwr.h"
#include "dfu_mal.h"
#include "xprintf.h"
#include "bsp_usart.h"

/* Private typedef -----------------------------------------------------------*/
typedef  void (*pFunction)(void);

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
uint8_t DeviceState;
uint8_t DeviceStatus[6];
pFunction Jump_To_Application;
uint32_t JumpAddress;

void DispSysInfo(void)
{
	xputs("\r\n");	
	xputs("**********************************************************\r\n");
    xputs("*\r\n");	
	xputs("*          ARMILA RECEIVER USB IAP ");
    xputs("\r\n*\r\n");	
    xputs("*          Build date:");
	xputs(__DATE__);
	xputc('-');
	xputs(__TIME__);
    xputs("\r\n*\r\n");
	xputs("**********************************************************\r\n\r\n");	
}

/*******************************************************************************
* Function Name  : main.
* Description    : main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{
	uint8_t ret;
	uint32_t i;
	
	DFU_Button_Config();
	USART1_init();
	DispSysInfo();

	/* Check if the Key push-button on STM3210x-EVAL Board is pressed */
	ret = DFU_Button_Read();
	xprintf("DFU button state:%d \r\n", ret);
	
	if (ret != 0)
	{ 
		/* Test if user code is programmed starting from address 0x8003000 */
		if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
		{
			/* Jump to user application */
			xputs("Jump to user application \r\n");
			i = 0;
			while(i < 6000)
			{
				i++;
			}
			
			JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
			Jump_To_Application = (pFunction) JumpAddress;
			/* Initialize user application's Stack Pointer */
			__set_MSP(*(__IO uint32_t*) ApplicationAddress);
			Jump_To_Application();
		}
	}

	/* Enter DFU mode */
	xputs("Enter DFU mode \r\n");
	DeviceState = STATE_dfuERROR;
	DeviceStatus[0] = STATUS_ERRFIRMWARE;
	DeviceStatus[4] = DeviceState;

	Set_System();
	Set_USBClock();
	USB_Init();  

	/* Main loop */
	while (1)
	{
		
	}
}


#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
