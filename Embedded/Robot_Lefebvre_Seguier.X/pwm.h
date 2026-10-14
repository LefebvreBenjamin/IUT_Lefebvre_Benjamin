/* 
 * File:   pwm.h
 * Author: E306-PC6
 *
 * Created on 8 septembre 2026, 17:09
 */

#ifndef PWM_H
#define	PWM_H

#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1



void InitPWM(void);

void PWMSetSpeedConsigne(float vitesseEnPourcents, char moteur);
void PWMUpdateSpeed();
#endif	/* PWM_H */

