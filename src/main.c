/* $Revision: 1.68.4.8 $
 *
 * File    : avr_main.c
 *
 *
 * Compiler specified defines:
 *	RT              - Required.
 *      MODEL=modelname - Required.
 *	NUMST=#         - Required. Number of sample times.
 *	NCSTATES=#      - Required. Number of continuous states.
 *      TID01EQ=1 or 0  - Optional. Only define to 1 if sample time task
 *                        id's 0 and 1 have equal rates.
 *      MULTITASKING    - Optional. (use MT for a synonym).
 */

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "rtmodel.h"
#include "rt_sim.h"
#include "rt_nonfinite.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/iom169.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include "LCD_Driver.h"

/*=========*
 * Defines *
 *=========*/

#define INC_FROM_DRIVER
#ifndef TRUE
#define FALSE (0)
#define TRUE  (1)
#endif


#define QUOTE1(name) #name
#define QUOTE(name) QUOTE1(name)    /* need to expand name    */

#define RUN_FOREVER -1.0

#define EXPAND_CONCAT(name1,name2) name1 ## name2
#define CONCAT(name1,name2) EXPAND_CONCAT(name1,name2)
#define RT_MODEL            CONCAT(MODEL,_rtModel)

/*====================*
 * External functions *
 *====================*/
extern RT_MODEL *MODEL(void);

extern void MdlInitializeSizes(void);
extern void MdlInitializeSampleTimes(void);
extern void MdlStart(void);
extern void MdlOutputs(int_T tid);
extern void MdlUpdate(int_T tid);
extern void MdlTerminate(void);

void OSCCAL_calibration(void);
//int ADC_read(void);


char intMode = 0; // By Carlo 2010, Enable RT interrupt mode in simulation
RT_MODEL  *S; // Make it global for unifor access



#if NCSTATES > 0
extern void rt_ODECreateIntegrationData(RTWSolverInfo *si);
extern void rt_ODEUpdateContinuousStates(RTWSolverInfo *si);

# define rt_CreateIntegrationData(S) \
	rt_ODECreateIntegrationData(rtmGetRTWSolverInfo(S));
# define rt_UpdateContinuousStates(S) \
	rt_ODEUpdateContinuousStates(rtmGetRTWSolverInfo(S));
# else
# define rt_CreateIntegrationData(S)  \
	rtsiSetSolverName(rtmGetRTWSolverInfo(S),"FixedStepDiscrete");
# define rt_UpdateContinuousStates(S) \
	rtmSetT(S, rtsiGetSolverStopTime(rtmGetRTWSolverInfo(S)));
#endif

#define VOLTAGE_SENSOR      0xC1

#define MAX_VEL 248    //in Rad/sec
#define MAX_POS 4000   // in tacche
#define MAX_COR 900   //riferimento in mA
           
           
//volatile uint16_t tempEncoder1, tempEncoder2;

/*==================================*
 * Global data local to this module *
 *==================================*/

static struct {
	int_T    stopExecutionFlag;
	int_T    isrOverrun;
	int_T    overrunFlags[NUMST];
	const char_T *errmsg;
} GBLbuf;

static volatile char     TextBuffer[LCD_TEXTBUFFER_SIZE + LCD_DISPLAY_SIZE    + 1] = {};
static volatile uint8_t  StrStart        = 0;
static volatile uint8_t  StrEnd          = 0;
static volatile uint8_t  ScrollCount     = 0;
static volatile uint8_t  UpdateDisplay   = false;
static volatile uint8_t  ShowColons      = false;
       volatile uint8_t  ScrollFlags     = 0;

static volatile uint8_T TCHAR = 255;                       /* '<S3>/Data Store Memory1' */
static volatile uint8_T TCHARL1;                       /* '<S3>/Data Store Memory1' */
static volatile uint8_T TCHARH1;                       /* '<S3>/Data Store Memory2' */
static volatile uint8_T TCHARL2;                       /* '<S3>/Data Store Memory4' */
static volatile uint8_T TCHARH2;                       /* '<S3>/Data Store Memory5' */


#  define rtExtModeSingleTaskUpload(S) /* Do nothing */

/* Function: rtOneStep 
 *      Perform one step of the model. This function is modeled such that
 */
void rt_OneStep()
{
	real_T tnext;
	if (GBLbuf.isrOverrun++) {  return;    }
	/* enable interrupts here */
    //sei();
	tnext = rt_SimGetNextSampleHit();
	rtsiSetSolverStopTime(rtmGetRTWSolverInfo(S),tnext);
	MdlOutputs(0);
	MdlUpdate(0);
	rt_SimUpdateDiscreteTaskSampleHits(rtmGetNumSampleTimes(S),
			rtmGetTimingData(S), rtmGetSampleHitPtr(S), rtmGetTPtr(S));
	if (rtmGetSampleTime(S,0) == CONTINUOUS_SAMPLE_TIME) 
		rt_UpdateContinuousStates(S);
	GBLbuf.isrOverrun--;
} /* end rtOneStep */


void Delay(unsigned int millisec)
{
	int i;

	while (millisec--)
		for (i=0; i<125; i++);
}

void OSCCAL_calibration(void)
{
	unsigned char calibrate = FALSE;
	int temp;
	unsigned char tempL;

	CLKPR = (1<<CLKPCE);        // set Clock Prescaler Change Enable
	// set prescaler = 8, Inter RC 8Mhz / 8 = 1Mhz
	CLKPR = (1<<CLKPS1) | (1<<CLKPS0);

	TIMSK2 = 0;             //disable OCIE2A and TOIE2

	ASSR = (1<<AS2);        //select asynchronous operation of timer2 (32,768kHz)

	OCR2A = 200;            // set timer2 compare value 

	TIMSK0 = 0;             // delete any interrupt sources

	TCCR1B = (1<<CS10);     // start timer1 with no prescaling
	TCCR2A = (1<<CS20);     // start timer2 with no prescaling

	while((ASSR & 0x01) | (ASSR & 0x04));       //wait for TCN2UB and TCR2UB to be cleared

	Delay(1000);    // wait for external crystal to stabilise

	while(!calibrate)
	{
		//SREG&=0x7F;// __disable_interrupt();  // disable global interrupt

		TIFR1 = 0xFF;   // delete TIFR1 flags
		TIFR2 = 0xFF;   // delete TIFR2 flags

		TCNT1H = 0;     // clear timer1 counter
		TCNT1L = 0;
		TCNT2 = 0;      // clear timer2 counter

		while ( !(TIFR2 & (1<<OCF2A)) );   // wait for timer2 compareflag

		TCCR1B = 0; // stop timer1

		//SREG|=0x80; //__enable_interrupt();  // enable global interrupt

		if ( (TIFR1 & (1<<TOV1)) )
		{
			temp = 0xFFFF;      // if timer1 overflows, set the temp to 0xFFFF
		}
		else
		{   // read out the timer1 counter value
			tempL = TCNT1L;
			temp = TCNT1H;
			temp = (temp << 8);
			temp += tempL;
		}

		if (temp > 6250)
		{
			OSCCAL--;   // the internRC oscillator runs to fast, decrease the OSCCAL
		}
		else if (temp < 6120)
		{
			OSCCAL++;   // the internRC oscillator runs to slow, increase the OSCCAL
		}
		else
			calibrate = TRUE;   // the interRC is correct

		TCCR1B = (1<<CS10); // start timer1
	}
}

/*****************************
 * Funzione richiamata da interruzione di Timer0 per leggere la conversione
 * della tensione inviata su ADC1
 */
void comandaMotore(void) {
    OCR1A = abs(DUTYmotore1);                                                                       
//    OCR1B = abs(DUTYmotore2);                                                                       
    if(DUTYmotore1>0) {                                             
        PORTB &= 0xF7; //0111 PB3=0                                                               
        PORTB |= 0x02; //PB1=1                                                                    
    } else {                                                                                      
        PORTB |= 0x08; //PB3=1                                                                    
        PORTB &= 0xFD;  //1101; //PB1=0                                                           
    }                                                                                                                                
}


double ADC_read(void)
{     
	double conversione=I;	
	ADCSRA |= 0x40;     //  Inizio Conversione ADSC=1 (bit6)
	while((ADCSRA & 0x40));
          conversione = ADCL;            // read out ADCL register
          conversione += (ADCH << 8);    // read out ADCH register        
          conversione *= 0.006445312; //((conversione*Vref=1.1)/1024)*6; // Calculate the voltage
    return conversione;
}


void TX(char data1,char data2,char data3,char data4,char data5) 
{
	while(!(UCSRA&(1<<UDRE))); //Attende che il buffer di trasmissione sia vuoto
	UDR = data1;
	while(!(UCSRA&(1<<UDRE))); //Attende che il buffer di trasmissione sia vuoto
	UDR = data1;
    while(!(UCSRA&(1<<UDRE))); //Attende che il buffer di trasmissione sia vuoto
	UDR = data1;
	while(!(UCSRA&(1<<UDRE))); //Attende che il buffer di trasmissione sia vuoto
	UDR = data2;
	while(!(UCSRA&(1<<UDRE))); //Attende che il buffer di trasmissione sia vuoto
	UDR = data3;
	while(!(UCSRA&(1<<UDRE))); //Attende che il buffer di trasmissione sia vuoto
	UDR = data4;
    while(!(UCSRA&(1<<UDRE))); //Attende che il buffer di trasmissione sia vuoto
	UDR = data5;
}


static volatile int jRx=0;
static volatile int comando=0;
volatile char t = 2;
volatile char c = 0;

void rxInterrupt(char var) { //var � udr passata dall' interruzione
    static volatile uint16_t rif = 0;
    
	if(jRx == 0) {        
		rif = var; //Byte meno significativo
		jRx++;
	}
	else{
		rif += var*256;
		if(rif >= 32768){
			rif -= 32768;
			Riferimento = -rif;
		}else
			Riferimento = rif;
        
        //LCD_Off();
        
		//if(t == 0) { //Velocit�
            //if(rif > MAX_VEL) {
            //    rif = Riferimento = jRx = 0;
                //LCD_Init();
            //    LCD_puts("VELOCITA MAX");
            //    return;
            //}
            
			//Faccio l'abs
			/*int16_t tmp = (~(128 << 8)) & rif;
			if(Riferimento < 0) 
				vel_s[0] = '-'; 
			else
				vel_s[0] = '+';
			vel_s[1] = (tmp / 100) + 48;
			vel_s[2] = ((tmp / 10) % 10) + 48;
			vel_s[3] = (tmp % 10) + 48;
			LCD_puts(vel_s);*/
            
		//} else if((t == 1) || (t == 3)) { //Posizione o slave
           //if(rif > MAX_POS) {
            //    rif = Riferimento = jRx = 0;
                //LCD_Init();
            //    LCD_puts("POSIZIONE MAX");
            //    return;
            //}
           /*if(t != 3) {
                int16_t tmp = (~(128 << 8)) & rif;
                if(Riferimento < 0)
                    pos_s[0] = '-';
                else
                    pos_s[0] = '+';
                pos_s[1] = (tmp / 1000) + 48;
                if(tmp < 1000) {
                    pos_s[2] = (tmp / 100) + 48;
                    pos_s[3] = ((tmp / 10) % 10) + 48;
                    pos_s[4] = (tmp % 10) + 48;
                }else {
                    pos_s[2] = ((tmp / 100) % 10) + 48;
                    pos_s[3] = ((tmp / 10) % 10) + 48;
                    pos_s[4] = (tmp % 10) + 48;
                }
                LCD_puts(pos_s);
           }*/
		//}else if(t == 4) {
           //LCD_Init();
           c = 1;
        //}
        rif = jRx = 0;
		//start = t;        
	}
}



ISR(USART0_RX_vect) {
    volatile char tmp = UDR;
    rxInterrupt(tmp);
}

static volatile uint16_t tempEncoder1=0;

//ISR(TIMER0_COMP_vect, ISR_NOBLOCK) {     
/*ISR(TIMER0_COMP_vect) {
    rt_OneStep();
} */   

volatile static char Aold=0x00, Bold=0x00;
ISR(PCINT1_vect) {     
     char pinAB=0x00; 
     char A=0x00; 
     char B=0x00;
     pinAB = (PINB) & 0x84;
     A = ((pinAB & 0x04)>>2); 
     B = ((pinAB & 0x80)>>7);            
/*     if((A==1)&&(Aold==0)) {    //Interrupt di A PINB2 = A       
          if(((A^B)==1))  
               Encoder1++;        
     } else  if ((B==1)&&(Bold==0)) {        
          if(((A^B)==1))              
               Encoder1--;                
     }*/                    
     if((A==1)&&(Aold==0)) {
          if(B==1)
              Encoder1 -= 1;
          else 
              if(B==0)
                  Encoder1 +=1;            
     }     
     Bold=B;
     Aold=A;         
}
    



/*
    switch(tipo_c) {
        case 0: Trasmissione = 0xFF; UCSRB |= (1<<RXEN); break; //VELOCITA, Abilito RX
        case 1: Trasmissione = 0x00; UCSRB |= (1<<RXEN); break; //POSIZIONE Abilito RX
        case 2: Trasmissione = 0xFF; UCSRB |= (1<<RXEN); break;                     //MASTER TX
        case 3: UCSRB |= (1<<RXEN); break;                      //SLAVE Disabilito LCD e abilito RX
        case 4: UCSRB |= (1<<RXEN); break;
    }
    
    t = tipo_c;
}*/


int main(void)
{
    //Disabilito Interrupt Globali
    cli();
    
    //Calibrazione 
  	OSCCAL_calibration();

    // set Clock Prescaler Change Enable
	CLKPR = (1<<CLKPCE);        
	// No prescaler f = 8Mhz
	CLKPR = (0<<CLKPS1)|(0<<CLKPS0);
	
    PRR = 0x00; //accensione di tutte le periferiche

	const char *status;
	(void)memset(&GBLbuf, 0, sizeof(GBLbuf));
	//rt_InitInfAndNaN(sizeof(real_T));
	S = MODEL();

	rtmSetTFinal(S,RUN_FOREVER);
	MdlInitializeSizes();
	MdlInitializeSampleTimes();
	status = rt_SimInitTimingEngine(rtmGetNumSampleTimes(S),
			rtmGetStepSize(S),
			rtmGetSampleTimePtr(S),
			rtmGetOffsetTimePtr(S),
			rtmGetSampleHitPtr(S),
			rtmGetSampleTimeTaskIDPtr(S),
			rtmGetTStart(S),
			&rtmGetSimTimeStep(S),
			&rtmGetTimingData(S));

	rt_CreateIntegrationData(S);
	MdlStart();
           
    ///******************///
    //INIZIALIZZO REGISTRI
    ///******************///   
    
    //TIMER0        
    TCCR0A = 11; //modalit� CTC senza connessione con pin OC0A e prescaler clk/64                                                                                                                                                                                                                                                                                                             
    OCR0A = 124;  //(interruzione con f = fosc/64*(1+OCR0A)  Interrupt 1KHz                                                                                                                                                                                                                                                                                                    
    TIMSK0  = 0;   //attivazione intettuzione timer0 su compare 2
    DDRF=0x00; //PortF ingresso                                                                                                                                                                                                                                                                                                                                                              
    PORTF=0x00; //Pull-up disable
    
    //TRASMISSIONE E RICEZIONE                                                                  
    UBRRH=0; // Divider=32 U2X=0                                                                               
    UBRRL=8; // Divider=32 U2X=0
    UCSRA=2; //UBRR calcolato secondo la formula ((fosc/8*BAUND) -1))                                                                                              
    UCSRB = (1<<TXEN) | (1<<RXEN) | (1<<RXCIE); //Abilito TX RX 
    UCSRC = 0x06; // 8bit trasmissione                                                                                                                                                 
    
    //MOTORE
    //PIN0-PIN2 INTERRUPT  PIN1-PIN3 IN1 dell'Hbridge e PIN5 ENABLE PWM
    // forme d'onda PWM da connettere all'ENEBLE del Hbridge.    
    DDRB=0x6A; //(01101010b)                                                                                                          
    PORTB=0x00; //00000000 Pull-Up disable (5)                                                                                                                                                                            

    // Enable pin change interrupt on PORTB
//    PCMSK1 |= 0x84;
    PCMSK1 |= 0x04;
    EIFR |= (1<<PCIF1);
    EIMSK |= 0x80; //abilita interruzioni da PCINT 8-15
    
    TCCR1A=0xA1; //10100010b                                                                                                                                                                                    
    TCCR1B=0x09; //00011001b                                                                                                                                                                                                       
    //f_clk=8Mhz quindi f_Fast_PWM= f_clk /(N*(1+TOP) ) =
    // = 8MHz/ (1*(1+399)) = 20kHz                                                                                                                   
    OCR1A=0; //Duty iniziale 0                                                                                                                                                                                                     
    OCR1B=0; //Duty iniziale 0                                                                                                                                                                                                   
    
    
    //UCSRB &= (~(1<<RXEN));   
//ADC_Init                                                                                                                                                                                                                       
    ADMUX = 0xC1;  //riferimento su Vrif=1.1V interno                                     
    ADCSRA = 0x86; //ADEN=1                                                            
                           // PRESCALER=64 =>                                    
                            //8MHz/64 = 125kHz                                           
    
    //Abilito Interrupt Globali
    sei();      
    //Sleep in attesa di interrupt
	while (1){
        comandaMotore();
        
        
        //if(abs(Encoder1)>=30000) Encoder1=Encoder1%30000;
        if(abs(Encoder1)>=500) Encoder1=Encoder1%500;
        tempEncoder1 = abs(Encoder1);
        TCHARL1 = tempEncoder1;
        TCHARH1 = (tempEncoder1>>8);        
        if(Encoder1 < 0) {
            TCHARH1 = TCHARH1 + 128;        
        }       
		
        ADMUX = 0xC1; //motore1 ADC1
        I = ADC_read();        
        double current = I*10;
        TCHARL2=DUTYmotore2;
        
        TX(255,0,((int)current),0,TCHARL2);        
        
        //TX(TCHAR,TCHARH1,TCHARL1,0,TCHARL2);
        
        
        rt_OneStep();
        
	}
} /* end main */ 





