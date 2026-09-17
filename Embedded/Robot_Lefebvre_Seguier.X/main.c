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

            float volts = ((float) result [0])* 3.3 / 4096;
            robotState.distanceTelemetreEGauche = 34 / volts - 5;

            volts = ((float) result [1])* 3.3 / 4096;
            robotState.distanceTelemetreGauche = 34 / volts - 5;

            volts = ((float) result [2])* 3.3 / 4096;
            robotState.distanceTelemetreCentre = 34 / volts - 5;

            volts = ((float) result [3])* 3.3 / 4096;
            robotState.distanceTelemetreDroit = 34 / volts - 5;

            volts = ((float) result [4])* 3.3 / 4096;
            robotState.distanceTelemetreEDroit = 34 / volts - 5;
        }

        if (robotState.distanceTelemetreGauche >= 35) {
            LED_BLEUE_1 = 1;
        } else LED_BLEUE_1 = 0;
        if (robotState.distanceTelemetreCentre >= 35) {
            LED_ORANGE_1 = 1;
        } else LED_ORANGE_1 = 0;
        if (robotState.distanceTelemetreDroit >= 35) {
            LED_ROUGE_1 = 1;
        } else LED_ROUGE_1 = 0;
        if (robotState.distanceTelemetreEDroit >= 35) {
            LED_VERTE_1 = 1;
        } else LED_VERTE_1 = 0;
        if (robotState.distanceTelemetreEGauche >= 35) {
            LED_BLANCHE_1 = 1;
        } else LED_BLANCHE_1 = 0;

    } // fin main

}

unsigned char stateRobot;

void OperatingSystemLoop(void) {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(STOP, MOTEUR_DROIT);
            PWMSetSpeedConsigne(STOP, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;
        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;
        case STATE_AVANCE:
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(STOP, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(STOP, MOTEUR_DROIT);
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

            /* 
            //------------------ADDED--------------------//
        case STATE_TOURNE_GAUCHE_CENTRE:
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(MAX_SPEED / 3, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_CENTRE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_DROITE_CENTRE:
            PWMSetSpeedConsigne(MAX_SPEED / 3, MOTEUR_DROIT);
            PWMSetSpeedConsigne(MAX_SPEED, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_CENTER_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
            //------------------ADDED--------------------//
            */
        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(SPIN_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-SPIN_SPEED, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-SPIN_SPEED, MOTEUR_DROIT);
            PWMSetSpeedConsigne(SPIN_SPEED, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}
unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {
    unsigned char positionObstacle = PAS_D_OBSTACLE;
    //ÈDtermination de la position des obstacles en fonction des ÈÈËtlmtres
    if ( robotState.distanceTelemetreDroit < 30 &&
            robotState.distanceTelemetreCentre > 40 &&
             robotState.distanceTelemetreGauche > 30) //Obstacle ‡droite
        positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreCentre > 40 &&
             robotState.distanceTelemetreGauche < 30) //Obstacle ‡gauche
        positionObstacle = OBSTACLE_A_GAUCHE;

/*
         //-----------------ADDED-------------------//
    else if (robotState.distanceTelemetreDroit > 35 &&
            robotState.distanceTelemetreCentre < 40 &&
            robotState.distanceTelemetreGauche < 45) //Obstacle ‡gauche et centre
        positionObstacle = OBSTACLE_A_GAUCHE_CENTRE;

    else if (robotState.distanceTelemetreDroit < 45 &&
            robotState.distanceTelemetreCentre < 40 &&
            robotState.distanceTelemetreGauche > 35) //Obstacle ‡droite et centre
        positionObstacle = OBSTACLE_A_DROITE_CENTRE;

        //-----------------ADDED-------------------//
 */
    
    else if (robotState.distanceTelemetreCentre < 40) //Obstacle en face
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreCentre > 40 &&
            robotState.distanceTelemetreGauche > 30) //pas d?obstacle
        positionObstacle = PAS_D_OBSTACLE;



    //ÈDtermination de lÈ?tat ‡venir du robot
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;

        /* //-----------------ADDED-------------------//
    else if (positionObstacle == OBSTACLE_A_DROITE_CENTRE)
        nextStateRobot = STATE_TOURNE_GAUCHE_CENTRE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE_CENTRE)
        nextStateRobot = STATE_TOURNE_DROITE_CENTRE;
        //-----------------ADDED-------------------// 
         */

    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_EN_FACE)
        
    //----------------------ADDED---------------------//
        /*
         * if (robotState.distanceTelemetreEDroit < robotState.distanceTelemetreGauche) {
            nextStateRobot = STATE_TOURNE_GAUCHE;
        } else if (robotState.distanceTelemetreGauche < robotState.distanceTelemetreDroit) {
            nextStateRobot = STATE_TOURNE_DROITE;
        } else {
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE; //Seulement cette ligne ‡ l'origine
        }
         */
    nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    //----------------------ADDED---------------------//
    //Si l?on n?est pas dans la transition de lÈ?tape en cours
    if (nextStateRobot != stateRobot - 1)
        stateRobot = nextStateRobot;
}