   
  .syntax unified
  .cpu cortex-m3
  .fpu softvfp
  .thumb

.global  g_pfnVectors
.global  Default_Handler

/* start address for the initialization values of the .data section. 
defined in linker script */
.word  _sidata
/* start address for the .data section. defined in linker script */  
.word  _sdata
/* end address for the .data section. defined in linker script */
.word  _edata
/* start address for the .bss section. defined in linker script */
.word  _sbss
/* end address for the .bss section. defined in linker script */
.word  _ebss

.equ  BootRAM, 0xF108F85F
/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called. 
 * @param  None
 * @retval None
*/

  .section  .text.Reset_Handler
  .weak  Reset_Handler
  .type  Reset_Handler, %function
Reset_Handler:  

/* Copy the data segment initializers from flash to SRAM */  
  movs  r1, #0
  b     LoopCopyDataInit

CopyDataInit:
  ldr   r3, =_sidata
  ldr   r3, [r3, r1]
  str   r3, [r0, r1]
  adds  r1, r1, #4
    
LoopCopyDataInit:
  ldr   r0, =_sdata
  ldr   r3, =_edata
  adds  r2, r0, r1
  cmp   r2, r3
  bcc   CopyDataInit
  ldr   r2, =_sbss
  b     LoopFillZerobss
/* Zero fill the bss segment. */  
FillZerobss:
  movs  r3, #0
  str   r3, [r2], #4
    
LoopFillZerobss:
  ldr   r3, = _ebss
  cmp   r2, r3
  bcc   FillZerobss
/* Call the clock system intitialization function.*/
  bl  SystemInit   
/* Call the application's entry point.*/
  bl    main
  bx    lr    
.size   Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an 
 *         unexpected interrupt. This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 * @param  None     
 * @retval None       
*/
  .section  .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b  Infinite_Loop
  .size  Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M3. Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
* 
******************************************************************************/    
  .section  .isr_vector,"a",%progbits
  .type  g_pfnVectors, %object
  .size  g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
  .word  _estack
  .word  Reset_Handler
  .word  NMI_Handler
  .word  HardFault_Handler
  .word  MemManage_Handler
  .word  BusFault_Handler
  .word  UsageFault_Handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  DebugMon_Handler
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler

  /*External Interrupts*/
  .word  CacheI_IRQHandler      //n00_IRQn
  .word  KeyScan_IRQHandler     //n01_IRQn
  .word  GPIO1_IRQHandler       //n02_IRQn
  .word  GPIO0_IRQHandler       //n03_IRQn
  .word  TIMER0_IRQHandler      //n04_IRQn
  .word  WDT_IRQHandler         //n05_IRQn
  .word  LLEERR_IRQHandler      //n06_IRQn
  .word  LLEFUN_IRQHandler      //n07_IRQn
  .word  SPIFlash_IRQHandler    //n08_IRQn
  .word  APBSPI_IRQHandler      //n09_IRQn
  .word  QSPI_IRQHandler        //n10_IRQn
  .word  SADC_IRQHandler        //n11_IRQn
  .word  UART1_IRQHandler       //n12_IRQn
  .word  UART0_IRQHandler       //n13_IRQn
  .word  I2C0_IRQHandler        //n14_IRQn
  .word  DMA_IRQHandler         //n15_IRQn
  .word  TRNG_IRQHandler        //n16_IRQn
  .word  QDEC2_IRQHandler       //n17_IRQn
  .word  QDEC1_IRQHandler       //n18_IRQn
  .word  QDEC0_IRQHandler       //n19_IRQn
  .word  USB_IRQHandler         //n20_IRQn
  .word  XOREADY_IRQHandler     //n21_IRQn
  .word  POW_CS_IRQHandler      //n22_IRQn
  .word  ACTCNT32K_IRQHandler   //n23_IRQn
  .word  PWM0_IRQHandler        //n24_IRQn
  .word  PWM1_IRQHandler        //n25_IRQn
  .word  RCMFDTRIM_IRQHandler   //n26_IRQn
  .word  PMUPVD_IRQHandler      //n27_IRQn
  .word  PMUPDR_IRQHandler      //n28_IRQn
  .word  PWM2_IRQHandler        //n29_IRQn
  .word  LLEAES_IRQHandler      //n30_IRQn
  .word  TIMER1_IRQHandler      //n31_IRQn
  .word  I2S_IRQHandler         //n32_IRQn
  .word  TBCU_IRQHandler        //n33_IRQn
  .word  AudioEnc_IRQHandler    //n34_IRQn
  
  .word  0
  .word  BootRAM       /* @0x1E0. This is for boot in RAM mode for 
                         STM32F10x High Density Value line devices. */
   
/*******************************************************************************
* Provide weak aliases for each Exception handler to the Default_Handler. 
* As they are weak aliases, any function with the same name will override 
* this definition.
*******************************************************************************/
    
  .weak  NMI_Handler
  .thumb_set NMI_Handler,Default_Handler
  
  .weak  HardFault_Handler
  .thumb_set HardFault_Handler,Default_Handler
  
  .weak  MemManage_Handler
  .thumb_set MemManage_Handler,Default_Handler
  
  .weak  BusFault_Handler
  .thumb_set BusFault_Handler,Default_Handler

  .weak  UsageFault_Handler
  .thumb_set UsageFault_Handler,Default_Handler

  .weak  SVC_Handler
  .thumb_set SVC_Handler,Default_Handler

  .weak  DebugMon_Handler
  .thumb_set DebugMon_Handler,Default_Handler

  .weak  PendSV_Handler
  .thumb_set PendSV_Handler,Default_Handler

  .weak  SysTick_Handler
  .thumb_set SysTick_Handler,Default_Handler

  .weak  CacheI_IRQHandler
  .thumb_set CacheI_IRQHandler,Default_Handler

  .weak  KeyScan_IRQHandler
  .thumb_set KeyScan_IRQHandler,Default_Handler

  .weak  GPIO1_IRQHandler
  .thumb_set GPIO1_IRQHandler,Default_Handler

  .weak  GPIO0_IRQHandler
  .thumb_set GPIO0_IRQHandler,Default_Handler

  .weak  TIMER0_IRQHandler
  .thumb_set TIMER0_IRQHandler,Default_Handler

  .weak  WDT_IRQHandler
  .thumb_set WDT_IRQHandler,Default_Handler

  .weak  LLEERR_IRQHandler
  .thumb_set LLEERR_IRQHandler,Default_Handler

  .weak  LLEFUN_IRQHandler
  .thumb_set LLEFUN_IRQHandler,Default_Handler

  .weak  SPIFlash_IRQHandler
  .thumb_set SPIFlash_IRQHandler,Default_Handler

  .weak  APBSPI_IRQHandler
  .thumb_set APBSPI_IRQHandler,Default_Handler

  .weak  QSPI_IRQHandler
  .thumb_set QSPI_IRQHandler,Default_Handler 

  .weak  SADC_IRQHandler
  .thumb_set SADC_IRQHandler,Default_Handler 

  .weak  UART1_IRQHandler
  .thumb_set UART1_IRQHandler,Default_Handler 

  .weak  UART0_IRQHandler
  .thumb_set UART0_IRQHandler,Default_Handler 

  .weak  I2C0_IRQHandler
  .thumb_set I2C0_IRQHandler,Default_Handler 

  .weak  DMA_IRQHandler
  .thumb_set DMA_IRQHandler,Default_Handler 

  .weak  TRNG_IRQHandler
  .thumb_set TRNG_IRQHandler,Default_Handler 

  .weak  QDEC2_IRQHandler
  .thumb_set QDEC2_IRQHandler,Default_Handler 

  .weak  QDEC1_IRQHandler
  .thumb_set QDEC1_IRQHandler,Default_Handler 

  .weak  QDEC0_IRQHandler
  .thumb_set QDEC0_IRQHandler,Default_Handler 

  .weak  USB_IRQHandler
  .thumb_set USB_IRQHandler,Default_Handler 

  .weak  XOREADY_IRQHandler
  .thumb_set XOREADY_IRQHandler,Default_Handler 

  .weak  POW_CS_IRQHandler
  .thumb_set POW_CS_IRQHandler,Default_Handler 

  .weak  ACTCNT32K_IRQHandler
  .thumb_set ACTCNT32K_IRQHandler,Default_Handler 

  .weak  PWM0_IRQHandler
  .thumb_set PWM0_IRQHandler,Default_Handler 

  .weak  PWM1_IRQHandler
  .thumb_set PWM1_IRQHandler,Default_Handler 

  .weak  RCMFDTRIM_IRQHandler
  .thumb_set RCMFDTRIM_IRQHandler,Default_Handler 

  .weak  PMUPVD_IRQHandler
  .thumb_set PMUPVD_IRQHandler,Default_Handler 

  .weak  PMUPDR_IRQHandler
  .thumb_set PMUPDR_IRQHandler,Default_Handler 

  .weak  PWM2_IRQHandler
  .thumb_set PWM2_IRQHandler,Default_Handler 

  .weak  LLEAES_IRQHandler
  .thumb_set LLEAES_IRQHandler,Default_Handler 

  .weak  TIMER1_IRQHandler
  .thumb_set TIMER1_IRQHandler,Default_Handler 

  .weak  I2S_IRQHandler
  .thumb_set I2S_IRQHandler,Default_Handler 

  .weak  TBCU_IRQHandler
  .thumb_set TBCU_IRQHandler,Default_Handler 

  .weak  AudioEnc_IRQHandler
  .thumb_set AudioEnc_IRQHandler,Default_Handler 
  
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/