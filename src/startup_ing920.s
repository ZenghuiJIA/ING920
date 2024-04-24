;/**************************************************************************//**
; * @file     startup_ARMCM3.s
; * @brief    CMSIS Core Device Startup File for
; *           ARMCM3 Device Series
; * @version  V1.08
; * @date     03. February 2012
; *
; * @note
; * Copyright (C) 2012 ARM Limited. All rights reserved.
; *
; * @par
; * ARM Limited (ARM) is supplying this software for use with Cortex-M
; * processor based microcontrollers.  This file can be freely distributed
; * within development tools that are supporting such ARM based processors.
; *
; * @par
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/
;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/

VER_MAJOR   EQU  0
VER_MINOR   EQU  1
VER_PATCH   EQU  0

; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000100

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                EXPORT CacheI_IRQHandler    [WEAK] ; n00_IRQn
                EXPORT KeyScan_IRQHandler   [WEAK] ; n01_IRQn
                EXPORT GPIO1_IRQHandler     [WEAK] ; n02_IRQn
                EXPORT GPIO0_IRQHandler     [WEAK] ; n03_IRQn
                EXPORT TIMER0_IRQHandler    [WEAK] ; n04_IRQn
                EXPORT WDT_IRQHandler       [WEAK] ; n05_IRQn
                EXPORT LLEERR_IRQHandler    [WEAK] ; n06_IRQn
                EXPORT LLEFUN_IRQHandler    [WEAK] ; n07_IRQn
                EXPORT SPIFlash_IRQHandler  [WEAK] ; n08_IRQn
                EXPORT APBSPI_IRQHandler    [WEAK] ; n09_IRQn
                EXPORT QSPI_IRQHandler      [WEAK] ; n10_IRQn
                EXPORT SADC_IRQHandler      [WEAK] ; n11_IRQn
                EXPORT UART1_IRQHandler     [WEAK] ; n12_IRQn
                EXPORT UART0_IRQHandler     [WEAK] ; n13_IRQn
                EXPORT I2C0_IRQHandler      [WEAK] ; n14_IRQn
                EXPORT DMA_IRQHandler       [WEAK] ; n15_IRQn
                EXPORT TRNG_IRQHandler      [WEAK] ; n16_IRQn
                EXPORT QDEC2_IRQHandler     [WEAK] ; n17_IRQn
                EXPORT QDEC1_IRQHandler     [WEAK] ; n18_IRQn
                EXPORT QDEC0_IRQHandler     [WEAK] ; n19_IRQn
                EXPORT USB_IRQHandler       [WEAK] ; n20_IRQn
                EXPORT XOREADY_IRQHandler   [WEAK] ; n21_IRQn
                EXPORT POW_CS_IRQHandler    [WEAK] ; n22_IRQn
                EXPORT ACTCNT32K_IRQHandler [WEAK] ; n23_IRQn
                EXPORT PWM0_IRQHandler      [WEAK] ; n24_IRQn
                EXPORT PWM1_IRQHandler      [WEAK] ; n25_IRQn
                EXPORT RCMFDTRIM_IRQHandler [WEAK] ; n26_IRQn
                EXPORT PMUPVD_IRQHandler    [WEAK] ; n27_IRQn
                EXPORT PMUPDR_IRQHandler    [WEAK] ; n28_IRQn
                EXPORT PWM2_IRQHandler      [WEAK] ; n29_IRQn
                EXPORT LLEAES_IRQHandler    [WEAK] ; n30_IRQn
                EXPORT TIMER1_IRQHandler    [WEAK] ; n31_IQRn
                EXPORT I2S_IRQHandler       [WEAK] ; n32_IQRn
				EXPORT TBCU_IRQHandler      [WEAK] ; n33_IRQn					
                EXPORT AudioEnc_IRQHandler  [WEAK] ; n34_IRQn

                DCD CacheI_IRQHandler
                DCD KeyScan_IRQHandler
                DCD GPIO1_IRQHandler
                DCD GPIO0_IRQHandler
                DCD TIMER0_IRQHandler
                DCD WDT_IRQHandler
                DCD LLEERR_IRQHandler
                DCD LLEFUN_IRQHandler
                DCD SPIFlash_IRQHandler
                DCD APBSPI_IRQHandler
                DCD QSPI_IRQHandler
                DCD SADC_IRQHandler
                DCD UART1_IRQHandler
                DCD UART0_IRQHandler
                DCD I2C0_IRQHandler
                DCD DMA_IRQHandler
                DCD TRNG_IRQHandler
                DCD QDEC2_IRQHandler
                DCD QDEC1_IRQHandler
                DCD QDEC0_IRQHandler
                DCD USB_IRQHandler
                DCD XOREADY_IRQHandler
                DCD POW_CS_IRQHandler
                DCD ACTCNT32K_IRQHandler
                DCD PWM0_IRQHandler
                DCD PWM1_IRQHandler
                DCD RCMFDTRIM_IRQHandler
                DCD PMUPVD_IRQHandler
                DCD PMUPDR_IRQHandler
                DCD PWM2_IRQHandler
                DCD LLEFUN_IRQHandler
                DCD TIMER1_IRQHandler
                DCD I2S_IRQHandler
				DCD TBCU_IRQHandler
                DCD AudioEnc_IRQHandler

__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

__PLATFORM_VER  DCD     (VER_MAJOR << 0) + (VER_MINOR << 16) + (VER_PATCH << 24)  ; Version
__APP_LOAD_ADD  DCD     0x2a000

                AREA    |.text|, CODE, READONLY



; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  __main
                IMPORT  chip_init

				; clear ahb_clk_lle_gate
				LDR     R0, =0x40000008
				LDR     R1, [R0]
				BIC     R1, R1, #0x2
				STR     R1, [R0]

                LDR     R0, =chip_init
                BLX     R0


    IF :DEF:POWER_SAVING

                IMPORT sysStateBackup

                LDR  R0, =0x40102010    ; reserved AON1 reg
                LDR  R1, [R0]
                TST  R1, #0x04000000    ; 1 << 26
                ORR  R1, R1, #0x04000000
                STR  R1, [R0]
                BNE  START_LOAD         ; not deep sleep, continue load

                LDR  R0, =sysStateBackup
                LDR  R0, [R0]
                BX   R0;

    ENDIF


START_LOAD      LDR     R0, =__main
                BX      R0
                ENDP



; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP



Default_Handler PROC
    EXPORT CacheI_IRQHandler    [WEAK] ; n00_IRQn
    EXPORT KeyScan_IRQHandler   [WEAK] ; n01_IRQn
    EXPORT GPIO1_IRQHandler     [WEAK] ; n02_IRQn
    EXPORT GPIO0_IRQHandler     [WEAK] ; n03_IRQn
    EXPORT TIMER0_IRQHandler    [WEAK] ; n04_IRQn
    EXPORT WDT_IRQHandler       [WEAK] ; n05_IRQn
    EXPORT LLEERR_IRQHandler    [WEAK] ; n06_IRQn
    EXPORT LLEFUN_IRQHandler    [WEAK] ; n07_IRQn
    EXPORT SPIFlash_IRQHandler  [WEAK] ; n08_IRQn
    EXPORT APBSPI_IRQHandler    [WEAK] ; n09_IRQn
    EXPORT QSPI_IRQHandler      [WEAK] ; n10_IRQn
    EXPORT SADC_IRQHandler      [WEAK] ; n11_IRQn
    EXPORT UART1_IRQHandler     [WEAK] ; n12_IRQn
    EXPORT UART0_IRQHandler     [WEAK] ; n13_IRQn
    EXPORT I2C0_IRQHandler      [WEAK] ; n14_IRQn
    EXPORT DMA_IRQHandler       [WEAK] ; n15_IRQn
    EXPORT TRNG_IRQHandler      [WEAK] ; n16_IRQn
    EXPORT QDEC2_IRQHandler     [WEAK] ; n17_IRQn
    EXPORT QDEC1_IRQHandler     [WEAK] ; n18_IRQn
    EXPORT QDEC0_IRQHandler     [WEAK] ; n19_IRQn
    EXPORT USB_IRQHandler       [WEAK] ; n20_IRQn
    EXPORT XOREADY_IRQHandler   [WEAK] ; n21_IRQn
    EXPORT POW_CS_IRQHandler    [WEAK] ; n22_IRQn
    EXPORT ACTCNT32K_IRQHandler [WEAK] ; n23_IRQn
    EXPORT PWM0_IRQHandler      [WEAK] ; n24_IRQn
    EXPORT PWM1_IRQHandler      [WEAK] ; n25_IRQn
    EXPORT RCMFDTRIM_IRQHandler [WEAK] ; n26_IRQn
    EXPORT PMUPVD_IRQHandler    [WEAK] ; n27_IRQn
    EXPORT PMUPDR_IRQHandler    [WEAK] ; n28_IRQn
    EXPORT PWM2_IRQHandler      [WEAK] ; n29_IRQn
    EXPORT LLEAES_IRQHandler    [WEAK] ; n30_IRQn
    EXPORT TIMER1_IRQHandler    [WEAK] ; n31_IQRn
    EXPORT I2S_IRQHandler       [WEAK] ; n32_IQRn
    EXPORT TBCU_IRQHandler      [WEAK] ; n33_IRQn		
    EXPORT AudioEnc_IRQHandler  [WEAK] ; n34_IRQn

CacheI_IRQHandler
KeyScan_IRQHandler
GPIO1_IRQHandler
GPIO0_IRQHandler
TIMER0_IRQHandler
WDT_IRQHandler
LLEERR_IRQHandler
LLEFUN_IRQHandler
SPIFlash_IRQHandler
APBSPI_IRQHandler
QSPI_IRQHandler
SADC_IRQHandler
UART1_IRQHandler
UART0_IRQHandler
I2C0_IRQHandler
DMA_IRQHandler
TRNG_IRQHandler
QDEC2_IRQHandler
QDEC1_IRQHandler
QDEC0_IRQHandler
USB_IRQHandler
XOREADY_IRQHandler
POW_CS_IRQHandler
ACTCNT32K_IRQHandler
PWM0_IRQHandler
PWM1_IRQHandler
RCMFDTRIM_IRQHandler
PMUPVD_IRQHandler
PMUPDR_IRQHandler
PWM2_IRQHandler
LLEAES_IRQHandler
TIMER1_IRQHandler
I2S_IRQHandler
TBCU_IRQHandler
AudioEnc_IRQHandler

                B       .
                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF


                END
