#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "pwm.h"

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

    InitTimer23();
    InitTimer1();

    


    LED_BLANCHE_1 = 1;
    LED_BLEUE_1 = 1;
    LED_ORANGE_1 = 1;
    LED_ROUGE_1 = 1;
    LED_VERTE_1 = 1;
    PWMSetSpeedConsigne(20.0f, MOTEUR_DROIT);
    /***********************************************************************************************/
    // Boucle Principale
    /***********************************************************************************************/
    while (1) {
        
   
    } // fin main

}

