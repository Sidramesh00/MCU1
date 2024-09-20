/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#define RCC_BASE_ADDR			 0x40023800UL
#define RCC_CR_REG_OFFSET		 0x00UL
#define RCC_CR_REG_ADDR			 ( RCC_BASE_ADDR + RCC_CR_REG_OFFSET )

#define RCC_CFGR_REG_OFFSET		 0x08UL
#define RCC_CFGR_REG_ADDR		 ( RCC_BASE_ADDR + RCC_CFGR_REG_OFFSET )

#define GPIO_BASE_ADDRESS 		 0x40020000UL

int main(void)
{
	//enable HSE clock source
	//1.Enable the HSE clock using HSEON bit (RCC_CR)
	uint32_t *pRCCCrReg = (uint32_t*)RCC_CR_REG_ADDR ;
	*pRCCCrReg |= (1<<16);

	//2. Wait until HSE clock from the external crystal stabilizes (only if crystal is connected )
	while( ! (*pRCCCrReg & ( 1 << 17) ) );

	uint32_t *pRCCCfgrReg = (uint32_t*)RCC_CFGR_REG_ADDR;
	//3. Switch the system clock to HSE (RCC_CFGR)
	*pRCCCfgrReg |= (1<<0);
	/**************Do MCO1 settings to measure it*********************/

	//1. Configure the RCC_CFGR MCO1 bit fields to select HSE as clock source
	*pRCCCfgrReg |= (1<<22);//clear 21 and set 22

	//Configure MCO1 prescaler // divisor as 4
	*pRCCCfgrReg |= ( 1 << 25);
	*pRCCCfgrReg |= ( 1 << 26);

	//2. Configure PA8 to AF0 mode to behave as MCO1 signal
	//a ) Enable the peripheral clock for GPIOA peripheral
	uint32_t *pRCCAhb1EnrReg = (uint32_t*)(RCC_BASE_ADDR + 0x30UL );
	*pRCCAhb1EnrReg |= (1<<0);

	//b ) Configure the mode of GPIOA pin 8 as alternate function mode
	uint32_t *pGPIOAModerReg = (uint32_t*)( GPIO_BASE_ADDRESS + 0x00 );
	*pGPIOAModerReg &= ~(0x3<<16);//clear
	*pGPIOAModerReg |= (0x2 <<16);//set

	/*  8.4.10 GPIO alternate function high register (GPIOx_AFRH) (x = A..I/J)
	  	Address offset: 0x24
 	 	Bits 31:0AFRHy: Alternate function selection for port x bit y (y = 8..15)
	c ) Configure the alternation function register to set the mode 0 for PA8 */
	uint32_t *pGPIOAAltFunHigh =  (uint32_t*)( GPIO_BASE_ADDRESS + 0x24 );
	*pGPIOAAltFunHigh &= ~(0xf << 0);

    /* Loop forever */
	for(;;);
}
