/*
 * File: ControlloTXRX.c
 *
 * Real-Time Workshop code generated for Simulink model ControlloTXRX.
 *
 * Model version                        : 1.1361
 * Real-Time Workshop file version      : 7.5  (R2010a)  25-Jan-2010
 * Real-Time Workshop file generated on : Wed Dec 29 18:15:23 2010
 * TLC version                          : 7.5 (Jan 19 2010)
 * C/C++ source code generated on       : Wed Dec 29 18:15:24 2010
 *
 * Target selection: ert_avr.tlc
 * Embedded hardware selection: Atmel->AVR
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "ControlloTXRX.h"
#include "ControlloTXRX_private.h"

/* user code (top of source file) */
/* System '<Root>' */
void comandaMotore(void)
{
  OCR0A = DUTYmotore1;

  //    OCR1B = abs(DUTYmotore2);
  if (segno==0) {
    PORTB &= 0xF7;                     //0111 PB3=0
    PORTB |= 0x02;                     //PB1=1
  } else {
    PORTB |= 0x08;                     //PB3=1
    PORTB &= 0xFD;                     //1101 //PB1=0
  }
}

/* System '<Root>' */
ISR(USART0_RX_vect)
{
  uint8_T var = UDR;
  if (jRx == 0) {
    rif = var;                         //Byte meno significativo
    jRx++;
  } else {
    rif += var*256;
    if (rif >= 32768) {
      rif -= 32768;
      Riferimento = -rif;
    } else {
      Riferimento = rif;
    }

    jRx= 0;
  }
}

/* System '<Root>' */
float ADC_read(void)
{
  ADCSRA |= 0x40;                      //  Inizio Conversione ADSC=1 (bit6)
  while ((ADCSRA & 0x40)) ;
  float conversione = ADCL;            // read out ADCL register
  conversione += (ADCH << 8);          // read out ADCH register
  conversione *= (float)0.006445312;   //((conversione*Vref=1.1)/1024)*6; // Calculate the voltage
  return conversione;
}

/* System '<Root>' */
void TX(void)
{
  if (abs(Encoder1)>=30000)
    Encoder1 = Encoder1%30000;         /*
                                          TCHARL1 = abs(Encoder1);

                                          if(Encoder1 < 0) {
                                          TCHARH1 = (abs(Encoder1)>>8) + 128;
                                          }       else { TCHARH1 = (abs(Encoder1)>>8);    } */
  TCHARL1 = abs((int)(Posizione*100));
  if ((int)(Posizione) < 0) {
    TCHARH1 = (abs((int)(Posizione*100))>>8) + 128;
  } else {
    TCHARH1 = (abs((int)(Posizione*100))>>8);
  }

  while (!(UCSRA&(1<<UDRE))) ;         //Attende che il buffer di trasmissione sia vuoto
  UDR = 255;
  while (!(UCSRA&(1<<UDRE))) ;         //Attende che il buffer di trasmissione sia vuoto
  UDR = 255;
  while (!(UCSRA&(1<<UDRE))) ;         //Attende che il buffer di trasmissione sia vuoto
  UDR = 255;
  while (!(UCSRA&(1<<UDRE))) ;         //Attende che il buffer di trasmissione sia vuoto
  UDR = TCHARH1;
  while (!(UCSRA&(1<<UDRE))) ;         //Attende che il buffer di trasmissione sia vuoto
  UDR = TCHARL1;
  while (!(UCSRA&(1<<UDRE))) ;         //Attende che il buffer di trasmissione sia vuoto
  UDR = TCHARH1+ TCHARL1;
  while (!(UCSRA&(1<<UDRE))) ;         //Attende che il buffer di trasmissione sia vuoto
}

void agg_vel(void)
{
  Posizione = Encoder1*0.0126;

  //velocita1 = VNow * 12.57;
  if (overflow==1)
    velocita1= 12557/vel_tmp;
  else
    velocita1= 0;
  VNow= 0x00;
}

/* System '<Root>' */
void Delay(unsigned int millisec)
{
  int i;
  while (millisec--)
    for (i=0; i<125; i++) ;
}

void OSCCAL_calibration(void)
{
  unsigned char calibrate = FALSE;
  int temp;
  unsigned char tempL;
  CLKPR = (1<<CLKPCE);                 // set Clock Prescaler Change Enable

  // set prescaler = 8, Inter RC 8Mhz / 8 = 1Mhz
  CLKPR = (1<<CLKPS1) | (1<<CLKPS0);
  TIMSK2 = 0;                          //disable OCIE2A and TOIE2
  ASSR = (1<<AS2);                     //select asynchronous operation of timer2 (32,768kHz)
  OCR2A = 200;                         // set timer2 compare value
  TIMSK0 = 0;                          // delete any interrupt sources
  TCCR1B = (1<<CS10);                  // start timer1 with no prescaling
  TCCR2A = (1<<CS20);                  // start timer2 with no prescaling
  while ((ASSR & 0x01) | (ASSR & 0x04)) ;//wait for TCN2UB and TCR2UB to be cleared
  Delay(1000);                         // wait for external crystal to stabilise
  while (!calibrate) {
    //SREG&=0x7F;// __disable_interrupt();  // disable global interrupt
    TIFR1 = 0xFF;                      // delete TIFR1 flags
    TIFR2 = 0xFF;                      // delete TIFR2 flags
    TCNT1H = 0;                        // clear timer1 counter
    TCNT1L = 0;
    TCNT2 = 0;                         // clear timer2 counter
    while (!(TIFR2 & (1<<OCF2A)) ) ;   // wait for timer2 compareflag
    TCCR1B = 0;                        // stop timer1

    //SREG|=0x80; //__enable_interrupt();  // enable global interrupt
    if ((TIFR1 & (1<<TOV1)) ) {
      temp = 0xFFFF;                   // if timer1 overflows, set the temp to 0xFFFF
    } else {                           // read out the timer1 counter value
      tempL = TCNT1L;
      temp = TCNT1H;
      temp = (temp << 8);
      temp += tempL;
    }

    if (temp > 6250) {
      OSCCAL--;                        // the internRC oscillator runs to fast, decrease the OSCCAL
    } else if (temp < 6120) {
      OSCCAL++;                        // the internRC oscillator runs to slow, increase the OSCCAL
    } else
      calibrate = TRUE;                // the interRC is correct
    TCCR1B = (1<<CS10);                // start timer1
  }
}

/* Exported block states */
real32_T Posizione;                    /* '<S4>/Data Store Memory2' */
real32_T velocita1;                    /* '<S4>/Data Store Memory3' */
real32_T I;                            /* '<S5>/Data Store Memory1' */
real32_T Rif_I;                        /* '<S5>/Data Store Memory2' */
real32_T I_cmd;                        /* '<Root>/Data Store Memory3' */
int16_T DUTYmotore1;                   /* '<S7>/Data Store Memory' */
int16_T DUTYmotore2;                   /* '<S7>/Data Store Memory1' */
int16_T Riferimento;                   /* '<S6>/Data Store Memory1' */
int16_T Encoder1;                      /* '<S4>/Data Store Memory1' */
int8_T segno;                          /* '<S6>/Data Store Memory2' */
int8_T master_slave;                   /* '<Root>/Data Store Memory1' */

/* Block states (auto storage) */
D_Work_ControlloTXRX ControlloTXRX_DWork;

/* Real-time model */
RT_MODEL_ControlloTXRX ControlloTXRX_M_;
RT_MODEL_ControlloTXRX *ControlloTXRX_M = &ControlloTXRX_M_;

/* Model step function */
void ControlloTXRX_step(void)
{
  /* local block i/o variables */
  real32_T rtb_Sum2;
  real32_T rtb_Sum3;
  real32_T rtb_Sum;
  real32_T rtb_Switch;
  real32_T I_0;
  real32_T I2_b;
  real32_T I2_o;

  /* user code (Output function Body) */

  /* System '<Root>' */
  comandaMotore();

  /* System '<Root>' */
  //TX();
  agg_vel();

  /* Sum: '<Root>/Sum2' incorporates:
   *  DataStoreRead: '<S4>/Data Store Read'
   *  DataStoreRead: '<S6>/Data Store Read1'
   *  DataTypeConversion: '<S1>/Data Type Conversion'
   *  Gain: '<Root>/P2'
   */
  rtb_Sum2 = ControlloTXRX_P.P2_Gain * (real32_T)Riferimento - Posizione;

  /* DiscreteIntegrator: '<S2>/I2' */
  if (ControlloTXRX_DWork.I2_SYSTEM_ENABLE) {
    I_0 = ControlloTXRX_DWork.I2_DSTATE;
  } else {
    I_0 = ControlloTXRX_P.I2_gainval * rtb_Sum2 + ControlloTXRX_DWork.I2_DSTATE;
  }

  if (I_0 >= ControlloTXRX_P.I2_UpperSat) {
    I_0 = ControlloTXRX_P.I2_UpperSat;
  } else {
    if (I_0 <= ControlloTXRX_P.I2_LowerSat) {
      I_0 = ControlloTXRX_P.I2_LowerSat;
    }
  }

  /* Sum: '<Root>/Sum3' incorporates:
   *  DataStoreRead: '<S4>/Data Store Read2'
   *  Gain: '<S2>/P3'
   *  Sum: '<S2>/Sum2'
   */
  rtb_Sum3 = (ControlloTXRX_P.P3_Gain * rtb_Sum2 + I_0) - velocita1;

  /* DiscreteIntegrator: '<S3>/I2' */
  if (ControlloTXRX_DWork.I2_SYSTEM_ENABLE_g) {
    I2_b = ControlloTXRX_DWork.I2_DSTATE_c;
  } else {
    I2_b = ControlloTXRX_P.I2_gainval_b * rtb_Sum3 +
      ControlloTXRX_DWork.I2_DSTATE_c;
  }

  if (I2_b >= ControlloTXRX_P.I2_UpperSat_g) {
    I2_b = ControlloTXRX_P.I2_UpperSat_g;
  } else {
    if (I2_b <= ControlloTXRX_P.I2_LowerSat_a) {
      I2_b = ControlloTXRX_P.I2_LowerSat_a;
    }
  }

  /* DataStoreWrite: '<Root>/Data Store Write1' incorporates:
   *  Gain: '<S3>/P'
   *  Sum: '<S3>/Sum2'
   */
  I_cmd = ControlloTXRX_P.P_Gain * rtb_Sum3 + I2_b;

  /* Sum: '<Root>/Sum' incorporates:
   *  DataStoreRead: '<S5>/Data Store Read'
   *  DataStoreRead: '<S5>/Data Store Read1'
   *  Gain: '<Root>/Gain1'
   */
  rtb_Sum = Rif_I - ControlloTXRX_P.Gain1_Gain * I;

  /* DiscreteIntegrator: '<Root>/I2' */
  if (ControlloTXRX_DWork.I2_SYSTEM_ENABLE_p) {
    I2_o = ControlloTXRX_DWork.I2_DSTATE_g;
  } else {
    I2_o = ControlloTXRX_P.I2_gainval_i * rtb_Sum +
      ControlloTXRX_DWork.I2_DSTATE_g;
  }

  if (I2_o >= ControlloTXRX_P.I2_UpperSat_e) {
    I2_o = ControlloTXRX_P.I2_UpperSat_e;
  } else {
    if (I2_o <= ControlloTXRX_P.I2_LowerSat_f) {
      I2_o = ControlloTXRX_P.I2_LowerSat_f;
    }
  }

  /* Sum: '<Root>/Sum1' incorporates:
   *  DataStoreRead: '<S5>/Data Store Read1'
   *  Gain: '<Root>/P1'
   */
  rtb_Switch = ControlloTXRX_P.P1_Gain * Rif_I + I2_o;

  /* Switch: '<Root>/Switch' incorporates:
   *  Constant: '<Root>/Constant'
   *  DataStoreRead: '<Root>/Data Store Read'
   *  Saturate: '<Root>/Saturation1'
   */
  if (master_slave > ControlloTXRX_P.Switch_Threshold) {
    rtb_Switch = (real32_T)ControlloTXRX_P.Constant_Value;
  } else {
    rtb_Switch = rt_SATURATE(rtb_Switch, ControlloTXRX_P.Saturation1_LowerSat,
      ControlloTXRX_P.Saturation1_UpperSat);
  }

  /* DataStoreWrite: '<S7>/Data Store Write' incorporates:
   *  Gain: '<S7>/Gain'
   */
  rtb_Switch = (real32_T)fmod((real32_T)floor(ControlloTXRX_P.Gain_Gain *
    rtb_Switch), 65536.0F);
  DUTYmotore1 = rtb_Switch < 0.0F ? -((int16_T)(uint16_T)(-rtb_Switch)) :
    (int16_T)(uint16_T)rtb_Switch;

  /* DataStoreWrite: '<S7>/Data Store Write1' incorporates:
   *  Gain: '<S7>/Gain1'
   */
  DUTYmotore2 = (int16_T)((int32_T)ControlloTXRX_P.Gain1_Gain_b * (int32_T)0 >>
    11);

  /* user code (Update function Body) */

  /* System '<Root>' */
  I = ADC_read();
  if (I_cmd<0) {
    segno= 1;
    Rif_I = -I_cmd;
  }

  if (I_cmd>0) {
    segno= 0;
    Rif_I = I_cmd;
  }

  if (I_cmd==0)
    Rif_I= 0;

  /* Update for DiscreteIntegrator: '<S2>/I2' */
  ControlloTXRX_DWork.I2_SYSTEM_ENABLE = 0U;
  ControlloTXRX_DWork.I2_DSTATE = ControlloTXRX_P.I2_gainval * rtb_Sum2 + I_0;
  if (ControlloTXRX_DWork.I2_DSTATE >= ControlloTXRX_P.I2_UpperSat) {
    ControlloTXRX_DWork.I2_DSTATE = ControlloTXRX_P.I2_UpperSat;
  } else {
    if (ControlloTXRX_DWork.I2_DSTATE <= ControlloTXRX_P.I2_LowerSat) {
      ControlloTXRX_DWork.I2_DSTATE = ControlloTXRX_P.I2_LowerSat;
    }
  }

  /* Update for DiscreteIntegrator: '<S3>/I2' */
  ControlloTXRX_DWork.I2_SYSTEM_ENABLE_g = 0U;
  ControlloTXRX_DWork.I2_DSTATE_c = ControlloTXRX_P.I2_gainval_b * rtb_Sum3 +
    I2_b;
  if (ControlloTXRX_DWork.I2_DSTATE_c >= ControlloTXRX_P.I2_UpperSat_g) {
    ControlloTXRX_DWork.I2_DSTATE_c = ControlloTXRX_P.I2_UpperSat_g;
  } else {
    if (ControlloTXRX_DWork.I2_DSTATE_c <= ControlloTXRX_P.I2_LowerSat_a) {
      ControlloTXRX_DWork.I2_DSTATE_c = ControlloTXRX_P.I2_LowerSat_a;
    }
  }

  /* Update for DiscreteIntegrator: '<Root>/I2' */
  ControlloTXRX_DWork.I2_SYSTEM_ENABLE_p = 0U;
  ControlloTXRX_DWork.I2_DSTATE_g = ControlloTXRX_P.I2_gainval_i * rtb_Sum +
    I2_o;
  if (ControlloTXRX_DWork.I2_DSTATE_g >= ControlloTXRX_P.I2_UpperSat_e) {
    ControlloTXRX_DWork.I2_DSTATE_g = ControlloTXRX_P.I2_UpperSat_e;
  } else {
    if (ControlloTXRX_DWork.I2_DSTATE_g <= ControlloTXRX_P.I2_LowerSat_f) {
      ControlloTXRX_DWork.I2_DSTATE_g = ControlloTXRX_P.I2_LowerSat_f;
    }
  }
}

/* Model initialize function */
void ControlloTXRX_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(ControlloTXRX_M, (NULL));

  /* states (dwork) */
  (void) memset((void *)&ControlloTXRX_DWork, 0,
                sizeof(D_Work_ControlloTXRX));

  /* exported global states */
  Posizione = 0.0F;
  velocita1 = 0.0F;
  I = 0.0F;
  Rif_I = 0.0F;
  I_cmd = 0.0F;
  DUTYmotore1 = 0;
  DUTYmotore2 = 0;
  Riferimento = 0;
  Encoder1 = 0;
  segno = 0;
  master_slave = 0;

  /* user code (Start function Body) */

  /* System '<Root>' */

  //Calibrazione                                                                                                                                                 semaforo=0x00;
  OSCCAL_calibration();

  // set Clock Prescaler Change Enable
  CLKPR = (1<<CLKPCE);

  // No prescaler f = 8Mhz
  CLKPR = (0<<CLKPS1)|(0<<CLKPS0);
  PRR = 0x00;                          //accensione di tutte le periferiche

  //TIMER0
  TCCR0A = 0x69;
  OCR0A= 0x00;
  TIMSK0= 0x00;
  TCCR2A = 12;                         //modalità CTC senza connessione con pin OC0A e prescaler clk/64        12
  OCR2A = 124;                         //(interruzione con f = fosc/64*(1+OCR0A)  Interrupt 1KHz
  TIMSK2 = 2;                          //attivazione intettuzione timer0 su compare 2
  ASSR = 0;
  DDRF= 0x00;                          //PortF ingresso
  PORTF= 0x00;                         //Pull-up disable

  //TRASMISSIONE E RICEZIONE
  UBRRH= 0;                            // Divider=32 U2X=0
  UBRRL= 8;                            // Divider=32 U2X=0
  UCSRA= 2;                            //UBRR calcolato secondo la formula ((fosc/8*BAUND) -1))
  UCSRB = (1<<TXEN) | (1<<RXEN) | (1<<RXCIE);//Abilito TX RX
  UCSRC = 0x06;                        // 8bit trasmissione

  //MOTORE
  //PIN0-PIN2 INTERRUPT  PIN1-PIN3 IN1 dell'Hbridge e PIN5 ENABLE PWM
  // forme d'onda PWM da connettere all'ENEBLE del Hbridge.
  DDRB= 0x7A;                          //(01111010b)
  PORTB= 0x00;                         //00000000 Pull-Up disable (5)

  // Enable pin change interrupt on PORTB
  //PCMSK1 |= 0x84;
  PCMSK1 |= 0x04;
  EIFR |= (1<<PCIF1);
  EIMSK |= 0x80;                       //abilita interruzioni da PCINT 8-15
  TCCR1A= 0x00;
  TCCR1B= 0x02;
  TIMSK1= 0x00;

  //f_clk=8Mhz quindi f_Fast_PWM= f_clk /(N*(1+TOP) ) =
  // = 8MHz/ (1*(1+399)) = 20kHz
  //OCR1A=0; //Duty iniziale 0
  //OCR1B=0; //Duty iniziale 0

  //UCSRB &= (~(1<<RXEN));
  //ADC_Init
  ADMUX = 0xC1;                        //riferimento su Vrif=1.1V interno
  ADCSRA = 0x86;                       //ADEN=1 PRESCALER=64 =>  8MHz/64 = 125kHz

  // Init button port pins
  DDRE &= ~(((1<<PINE2)|(1<<PINE3)));
  PORTE |= ((1<<PINE2)|(1<<PINE3));    // pullups on

  // Enable pin change interrupt on PORTE
  PCMSK0 |= ((1<<PINE2)|(1<<PINE3));
  EIFR |= (1<<PCIF0);
  EIMSK |= (1<<PCIE0);

  /* Start for DataStoreMemory: '<S7>/Data Store Memory' */
  DUTYmotore1 = ControlloTXRX_P.DataStoreMemory_InitialValue;

  /* Start for DataStoreMemory: '<S7>/Data Store Memory1' */
  DUTYmotore2 = ControlloTXRX_P.DataStoreMemory1_InitialValue_k;

  /* Start for DataStoreMemory: '<S6>/Data Store Memory1' */
  Riferimento = ControlloTXRX_P.DataStoreMemory1_InitialValue_e;

  /* Start for DataStoreMemory: '<S6>/Data Store Memory2' */
  segno = ControlloTXRX_P.DataStoreMemory2_InitialValue_d;

  /* Start for DataStoreMemory: '<S4>/Data Store Memory1' */
  Encoder1 = ControlloTXRX_P.DataStoreMemory1_InitialValue_i;

  /* Start for DataStoreMemory: '<S4>/Data Store Memory2' */
  Posizione = ControlloTXRX_P.DataStoreMemory2_InitialValue;

  /* Start for DataStoreMemory: '<S4>/Data Store Memory3' */
  velocita1 = ControlloTXRX_P.DataStoreMemory3_InitialValue;

  /* Start for DataStoreMemory: '<S5>/Data Store Memory1' */
  I = ControlloTXRX_P.DataStoreMemory1_InitialValue;

  /* Start for DataStoreMemory: '<S5>/Data Store Memory2' */
  Rif_I = ControlloTXRX_P.DataStoreMemory2_InitialValue_g;

  /* Start for DataStoreMemory: '<Root>/Data Store Memory1' */
  master_slave = ControlloTXRX_P.DataStoreMemory1_InitialValue_o;

  /* Start for DataStoreMemory: '<Root>/Data Store Memory3' */
  I_cmd = ControlloTXRX_P.DataStoreMemory3_InitialValue_f;

  /* user code (Initialize function Body) */

  /* System '<Root>' */
  rif = 0;
  jRx= 0;

  /* System '<Root>' */
  TCHAR = 255;
  Aold = 0x00;
  Bold = 0x00;
  VNow = 0x00;
  vel_tmp= 0x0000;
  start= 0x00;
  overflow= 0x00;

  /* InitializeConditions for DiscreteIntegrator: '<S2>/I2' */
  ControlloTXRX_DWork.I2_DSTATE = ControlloTXRX_P.I2_IC;

  /* InitializeConditions for DiscreteIntegrator: '<S3>/I2' */
  ControlloTXRX_DWork.I2_DSTATE_c = ControlloTXRX_P.I2_IC_c;

  /* InitializeConditions for DiscreteIntegrator: '<Root>/I2' */
  ControlloTXRX_DWork.I2_DSTATE_g = ControlloTXRX_P.I2_IC_a;

  /* Enable for DiscreteIntegrator: '<S2>/I2' */
  ControlloTXRX_DWork.I2_SYSTEM_ENABLE = 1U;

  /* Enable for DiscreteIntegrator: '<S3>/I2' */
  ControlloTXRX_DWork.I2_SYSTEM_ENABLE_g = 1U;

  /* Enable for DiscreteIntegrator: '<Root>/I2' */
  ControlloTXRX_DWork.I2_SYSTEM_ENABLE_p = 1U;
}

/* Model terminate function */
void ControlloTXRX_terminate(void)
{
  /* (no terminate code required) */
}

/* user code (bottom of source file) */
/* System '<Root>' */
ISR(PCINT1_vect)
{
  if (start==0) {
    TIMSK1= 0x01;
    TCNT1= 0x0000;
    start= 1;
  } else {
    char pinAB= 0x00;
    char A= 0x00;
    char B= 0x00;
    pinAB = (PINB) & 0x84;
    A = ((pinAB & 0x04)>>2);
    B = ((pinAB & 0x80)>>7);
    if ((A==1)&&(Aold==0)) {
      if (B==1) {
        Encoder1 -= 1;
        vel_tmp= -abs(TCNT1);
        TCNT1= 0x0000;
        VNow--;
      } else if (B==0) {
        Encoder1 +=1;
        vel_tmp= abs(TCNT1);
        TCNT1= 0x0000;
        VNow++;
      }
    }

    Bold= B;
    Aold= A;
    start= 1;
    overflow= 1;
  }
}

/* System '<Root>' */
ISR(TIMER2_COMP_vect)
{
  semaforo= 1;
}

ISR(TIMER1_OVF_vect)
{
  overflow= 0;
  TIMSK1= 0x00;
  start= 0;
}

ISR (PCINT0_vect)
{
  char buttons;
  char key;
  buttons = (~PINE) & ((1<<PINE2)|(1<<PINE3));

  // Output virtual keys
  if (buttons & (1<<3))
    key = 2;
  else
    key = 0;                           //invalid combination
  EIFR = (1<<PCIF0);                   // Reset pin change interrupt flags
  if ((key == 2))
    master_slave = (master_slave==1) ? 0 : 1;
}

/*
 * File trailer for Real-Time Workshop generated code.
 *
 * [EOF]
 */
