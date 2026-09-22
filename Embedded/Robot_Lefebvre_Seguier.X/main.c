#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "pwm.h"
#include "ADC.h"
#include "robot.h"
#include "main.h"

int ADCValue0;
int ADCValue1;
int ADCValue2;

volatile int etats = 0b00000;

int main(void) {
    /***********************************************************************************************/
    //Initialisation oscillateur
    /***********************************************************************************************/
    InitOscillator();
    /***********************************************************************************************/
    // Configuration des input et output (IO)
    /***********************************************************************************************/
    InitIO();
    InitPWM();

    InitADC1();

    InitTimer23();
    InitTimer1();
    InitTimer4();
    SetFreqTimer4(1000);



    LED_BLANCHE_1 = 1;
    LED_BLEUE_1 = 1;
    LED_ORANGE_1 = 1;
    LED_ROUGE_1 = 1;
    LED_VERTE_1 = 1;

    /***********************************************************************************************/
    // Boucle Principale
    /***********************************************************************************************/
    while (1) {
        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            etats = 0b00000;
            float volts = ((float) result [0])* 3.3 / 4096;
            robotState.distanceTelemetreEGauche = 34 / volts - 5;
            etats = (robotState.distanceTelemetreEGauche <= 37) ? (etats | 0b10000) : etats;

            volts = ((float) result [1])* 3.3 / 4096;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            etats = (robotState.distanceTelemetreGauche <= 37) ? (etats | 0b01000) : etats;

            volts = ((float) result [2])* 3.3 / 4096;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            etats = (robotState.distanceTelemetreCentre <= 37) ? (etats | 0b00100) : etats;

            volts = ((float) result [3])* 3.3 / 4096;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            etats = (robotState.distanceTelemetreDroit <= 37) ? (etats | 0b00010) : etats;

            volts = ((float) result [4])* 3.3 / 4096;
            robotState.distanceTelemetreEDroit = 34 / volts - 5;
            etats = (robotState.distanceTelemetreEDroit <= 37) ? (etats | 0b00001) : etats;
        }

        if (robotState.distanceTelemetreGauche >= 37) {
            LED_BLEUE_1 = 1;
        } else LED_BLEUE_1 = 0;
        if (robotState.distanceTelemetreCentre >= 37) {
            LED_ORANGE_1 = 1;
        } else LED_ORANGE_1 = 0;
        if (robotState.distanceTelemetreDroit >= 37) {
            LED_ROUGE_1 = 1;
        } else LED_ROUGE_1 = 0;
        if (robotState.distanceTelemetreEDroit >= 37) {
            LED_VERTE_1 = 1;
        } else LED_VERTE_1 = 0;
        if (robotState.distanceTelemetreEGauche >= 37) {
            LED_BLANCHE_1 = 1;
        } else LED_BLANCHE_1 = 0;

    } // fin main

}

//timestamp = 0;

unsigned char stateRobot;

void OperatingSystemLoop(void) {
    switch (etats) {
        case 0b00000: //avancer
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b00001:
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(SPIN_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b00010: 
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-SPIN_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b00011: 
            PWMSetSpeedConsigne(SPIN_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-SPIN_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b00100: 
            PWMSetSpeedConsigne(-SPIN_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(SPIN_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b00101: 
            PWMSetSpeedConsigne(SPIN_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-SPIN_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b00110: 
            PWMSetSpeedConsigne(SPIN_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-SPIN_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b00111: 
            PWMSetSpeedConsigne(SPIN_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-SPIN_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b01000: 
            PWMSetSpeedConsigne(-SPIN_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(SPIN_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b01001: 
            PWMSetSpeedConsigne(SPIN_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-SPIN_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b01010: 
            PWMSetSpeedConsigne(-SPIN_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(SPIN_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b01011: 
            PWMSetSpeedConsigne(SPIN_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-SPIN_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b01100: 
            PWMSetSpeedConsigne(-SPIN_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(SPIN_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b01101: 
            PWMSetSpeedConsigne(SPIN_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-SPIN_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b01110: 
            PWMSetSpeedConsigne(-SPIN_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(SPIN_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b01111: 
            PWMSetSpeedConsigne(-SPIN_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(SPIN_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b10000: 
            PWMSetSpeedConsigne(-SPIN_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(SPIN_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b10001: 
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_GAUCHE); //// ICICICICICICICICCICICICI
            break;
        case 0b10010: 
            PWMSetSpeedConsigne(-SPIN_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b10011: 
            PWMSetSpeedConsigne(-MAX_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b10100: 
            PWMSetSpeedConsigne(-MAX_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b10101: 
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-MAX_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b10110: 
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-MAX_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b10111: 
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-MAX_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b11000: 
            PWMSetSpeedConsigne(-SPIN_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b11001: 
            PWMSetSpeedConsigne(-SPIN_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(SPIN_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b11010: 
            PWMSetSpeedConsigne(-MAX_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b11011: 
            PWMSetSpeedConsigne(-MAX_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b11100: 
            PWMSetSpeedConsigne(-MAX_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b11101: 
            PWMSetSpeedConsigne(-MAX_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b11110: 
            PWMSetSpeedConsigne(-MAX_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_GAUCHE);
            break;
        case 0b11111: 
            PWMSetSpeedConsigne(-MAX_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_GAUCHE);
            break;
        default:
            break;
    }
}