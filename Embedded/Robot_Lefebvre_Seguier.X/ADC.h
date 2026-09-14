/* 
 * File:   ADC.h
 * Author: E306-PC6
 *
 * Created on 14 septembre 2026, 14:03
 */

#ifndef ADC_H
#define	ADC_H

void InitADC1(void);
void ADC1StartConversionSequence();
unsigned int * ADCGetResult(void);
unsigned char ADCIsConversionFinished(void);
void ADCClearConversionFinishedFlag(void);



#endif	/* ADC_H */

