/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs 

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs  - 1.45
        Device            :  PIC18F46K22
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include "mcc_generated_files/mcc.h"

#define LShiftFB 7
#define RShiftFB 6
#define LShiftPWM 3
#define RShiftPWM 0

#define CW 1
#define CCW 0

//function declare
//PWM min is 0, max is 65535
void delay1000ms();
void MTR_A(uint8_t CW_CCW,uint16_t PWM);
void MTR_B(uint8_t CW_CCW,uint16_t PWM); 
void Control(uint8_t dataIn);

#define _8lvToPWM(speedIn) (uint16_t)((uint16_t)speedIn * 40)
//L_PWMA in put with _8lvToPWM(speedIn)
void MTR_Move(uint8_t L_CW_CCW, uint8_t R_CW_CCW, uint16_t L_PWM_A, uint16_t R_PWM_B);

uint8_t data = 0;
uint8_t MTR_A_CW_CCW = 0;
uint8_t MTR_B_CW_CCW = 0;
uint16_t MTR_A_PWM = 0;
uint16_t MTR_B_PWM = 0;


/*
                         Main application
 */

void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();

    uint16_t dutycycle = 0;

    EPWM1_Initialize();
    EPWM1_LoadDutyValue(dutycycle);
    
    EPWM2_Initialize();
    EPWM2_LoadDutyValue(dutycycle);
    
    EUSART2_Initialize();
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable high priority global interrupts
    //INTERRUPT_GlobalInterruptHighEnable();

    // Enable low priority global interrupts.
    //INTERRUPT_GlobalInterruptLowEnable();

    // Disable high priority global interrupts
    //INTERRUPT_GlobalInterruptHighDisable();

    // Disable low priority global interrupts.
    //INTERRUPT_GlobalInterruptLowDisable();

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    while (1)
    {   
 
        
        
        if(EUSART2_DataReady){
            RX_LED_SetHigh();
            data = EUSART2_Read();
            //EUSART2_Write(data);
            RX_LED_SetLow();
            Control(data);
        }
    }
}


void delay1000ms(){
    int j = 0;
    for(int i = 0; i< 20000; i++){
        j++;
    }
    for(int i = 0; i< 20000; i++){
        j++;
    }
        for(int i = 0; i< 20000; i++){
        j++;
    }
        for(int i = 0; i< 20000; i++){
        j++;
    }
}

void MTR_A(uint8_t CW_CCW,uint16_t PWM){
      
    //delay1000ms();
    
    if(CW_CCW == 1){
        //CW

        MTR_AIN2_SetHigh();
        MTR_AIN1_SetLow(); 

    }else{
        //CCW

        MTR_AIN1_SetHigh();
        MTR_AIN2_SetLow(); 

    }
    EPWM2_LoadDutyValue(PWM);
}
void MTR_B(uint8_t CW_CCW,uint16_t PWM){
    
    //EPWM1_LoadDutyValue(0);

    MTR_BIN1_SetLow(); 
    MTR_BIN2_SetLow();

    
    //delay1000ms();
    
    if(CW_CCW == 1){
        //CW

        MTR_BIN2_SetHigh();
        MTR_BIN1_SetLow(); 

    }else{
        //CCW
        MTR_BIN1_SetHigh();
        MTR_BIN2_SetLow();

    }
    EPWM1_LoadDutyValue(PWM);
}


/*
#define LShiftFB 7
#define RShiftFB 6
#define LShiftPWM 3
#define RShiftPWM 0
 */


void Control(uint8_t dataIn){
    //L R LLL RRR
    uint8_t L_isForward = 0;
    uint8_t R_isForward = 1;
    uint8_t L_speed = 0;
    uint8_t R_speed = 0;
    
   //decode
    L_isForward = (dataIn >> 7) & 1;
    R_isForward = (dataIn >> 6)& 1;
    L_speed = (dataIn >> 3) & 7;
    R_speed = (dataIn >> 0) & 7;
    
    /*
    L_isForward = (dataIn & (0x01 << LShiftFB)) >> LShiftFB;
    R_isForward = (dataIn & (0x01 << RShiftFB)) >> RShiftFB;
    L_speed = (dataIn & (0x07 << LShiftPWM)) >> LShiftPWM;
    R_speed = (dataIn & (0x07 << RShiftPWM)) >> RShiftPWM;     
     */
    
//    L_isForward = 0;
//    R_isForward = 1;
//    L_speed = 7;
//    R_speed = 7;
    
    //Forward
    MTR_Move(L_isForward, R_isForward, _8lvToPWM(L_speed), _8lvToPWM(R_speed));
   //MTR_Move(L_Forward, R_Forward, _8lvToPWM(L_speed), _8lvToPWM(R_speed));
}

void MTR_Move(uint8_t L_CW_CCW, uint8_t R_CW_CCW, uint16_t L_PWM_A, uint16_t R_PWM_B){
    MTR_A(L_CW_CCW,L_PWM_A);                        
    MTR_B(R_CW_CCW,R_PWM_B);
}

/**
 End of File
*/