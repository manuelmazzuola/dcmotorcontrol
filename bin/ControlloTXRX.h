/*
 * File: ControlloTXRX.h
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

#ifndef RTW_HEADER_ControlloTXRX_h_
#define RTW_HEADER_ControlloTXRX_h_
#ifndef ControlloTXRX_COMMON_INCLUDES_
# define ControlloTXRX_COMMON_INCLUDES_
#include <stddef.h>
#include <math.h>
#include <string.h>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_SATURATE.h"
#endif                                 /* ControlloTXRX_COMMON_INCLUDES_ */

#include "ControlloTXRX_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm)      ((void*) 0)
#endif

/* user code (top of header file) */
/* System '<Root>' */
volatile uint16_T rif;
volatile uint8_T jRx;

/* System '<Root>' */
volatile uint8_T TCHAR;                /* '<S3>/Data Store Memory1' */
volatile uint8_T TCHARL1;              /* '<S3>/Data Store Memory1' */
volatile uint8_T TCHARH1;              /* '<S3>/Data Store Memory2' */
volatile uint8_T TCHARL2;              /* '<S3>/Data Store Memory4' */
volatile uint8_T TCHARH2;              /* '<S3>/Data Store Memory5' */
volatile char Aold;
volatile char Bold;
volatile int16_T vel_tmp;
volatile int8_T start;
volatile int8_T overflow;
volatile int8_T VNow;

/* System '<Root>' */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/iom169.h>
#include <avr/pgmspace.h>
#include <inttypes.h>

volatile int semaforo;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real32_T I2_DSTATE;                  /* '<S2>/I2' */
  real32_T I2_DSTATE_c;                /* '<S3>/I2' */
  real32_T I2_DSTATE_g;                /* '<Root>/I2' */
  uint8_T I2_SYSTEM_ENABLE;            /* '<S2>/I2' */
  uint8_T I2_SYSTEM_ENABLE_g;          /* '<S3>/I2' */
  uint8_T I2_SYSTEM_ENABLE_p;          /* '<Root>/I2' */
} D_Work_ControlloTXRX;

/* Parameters (auto storage) */
struct Parameters_ControlloTXRX_ {
  real_T Constant_Value;               /* Expression: 0
                                        * Referenced by: '<Root>/Constant'
                                        */
  real32_T P2_Gain;                    /* Computed Parameter: P2_Gain
                                        * Referenced by: '<Root>/P2'
                                        */
  real32_T P3_Gain;                    /* Computed Parameter: P3_Gain
                                        * Referenced by: '<S2>/P3'
                                        */
  real32_T I2_gainval;                 /* Computed Parameter: I2_gainval
                                        * Referenced by: '<S2>/I2'
                                        */
  real32_T I2_IC;                      /* Computed Parameter: I2_IC
                                        * Referenced by: '<S2>/I2'
                                        */
  real32_T I2_UpperSat;                /* Computed Parameter: I2_UpperSat
                                        * Referenced by: '<S2>/I2'
                                        */
  real32_T I2_LowerSat;                /* Computed Parameter: I2_LowerSat
                                        * Referenced by: '<S2>/I2'
                                        */
  real32_T P_Gain;                     /* Computed Parameter: P_Gain
                                        * Referenced by: '<S3>/P'
                                        */
  real32_T I2_gainval_b;               /* Computed Parameter: I2_gainval_b
                                        * Referenced by: '<S3>/I2'
                                        */
  real32_T I2_IC_c;                    /* Computed Parameter: I2_IC_c
                                        * Referenced by: '<S3>/I2'
                                        */
  real32_T I2_UpperSat_g;              /* Computed Parameter: I2_UpperSat_g
                                        * Referenced by: '<S3>/I2'
                                        */
  real32_T I2_LowerSat_a;              /* Computed Parameter: I2_LowerSat_a
                                        * Referenced by: '<S3>/I2'
                                        */
  real32_T P1_Gain;                    /* Computed Parameter: P1_Gain
                                        * Referenced by: '<Root>/P1'
                                        */
  real32_T Gain1_Gain;                 /* Computed Parameter: Gain1_Gain
                                        * Referenced by: '<Root>/Gain1'
                                        */
  real32_T I2_gainval_i;               /* Computed Parameter: I2_gainval_i
                                        * Referenced by: '<Root>/I2'
                                        */
  real32_T I2_IC_a;                    /* Computed Parameter: I2_IC_a
                                        * Referenced by: '<Root>/I2'
                                        */
  real32_T I2_UpperSat_e;              /* Computed Parameter: I2_UpperSat_e
                                        * Referenced by: '<Root>/I2'
                                        */
  real32_T I2_LowerSat_f;              /* Computed Parameter: I2_LowerSat_f
                                        * Referenced by: '<Root>/I2'
                                        */
  real32_T Saturation1_UpperSat;       /* Computed Parameter: Saturation1_UpperSat
                                        * Referenced by: '<Root>/Saturation1'
                                        */
  real32_T Saturation1_LowerSat;       /* Computed Parameter: Saturation1_LowerSat
                                        * Referenced by: '<Root>/Saturation1'
                                        */
  real32_T Gain_Gain;                  /* Computed Parameter: Gain_Gain
                                        * Referenced by: '<S7>/Gain'
                                        */
  real32_T DataStoreMemory2_InitialValue;/* Computed Parameter: DataStoreMemory2_InitialValue
                                          * Referenced by: '<S4>/Data Store Memory2'
                                          */
  real32_T DataStoreMemory3_InitialValue;/* Computed Parameter: DataStoreMemory3_InitialValue
                                          * Referenced by: '<S4>/Data Store Memory3'
                                          */
  real32_T DataStoreMemory1_InitialValue;/* Computed Parameter: DataStoreMemory1_InitialValue
                                          * Referenced by: '<S5>/Data Store Memory1'
                                          */
  real32_T DataStoreMemory2_InitialValue_g;/* Computed Parameter: DataStoreMemory2_InitialValue_g
                                            * Referenced by: '<S5>/Data Store Memory2'
                                            */
  real32_T DataStoreMemory3_InitialValue_f;/* Computed Parameter: DataStoreMemory3_InitialValue_f
                                            * Referenced by: '<Root>/Data Store Memory3'
                                            */
  int16_T DataStoreMemory_InitialValue;/* Computed Parameter: DataStoreMemory_InitialValue
                                        * Referenced by: '<S7>/Data Store Memory'
                                        */
  int16_T DataStoreMemory1_InitialValue_k;/* Computed Parameter: DataStoreMemory1_InitialValue_k
                                           * Referenced by: '<S7>/Data Store Memory1'
                                           */
  int16_T DataStoreMemory1_InitialValue_e;/* Computed Parameter: DataStoreMemory1_InitialValue_e
                                           * Referenced by: '<S6>/Data Store Memory1'
                                           */
  int16_T DataStoreMemory1_InitialValue_i;/* Computed Parameter: DataStoreMemory1_InitialValue_i
                                           * Referenced by: '<S4>/Data Store Memory1'
                                           */
  int16_T Gain1_Gain_b;                /* Computed Parameter: Gain1_Gain_b
                                        * Referenced by: '<S7>/Gain1'
                                        */
  int8_T Switch_Threshold;             /* Computed Parameter: Switch_Threshold
                                        * Referenced by: '<Root>/Switch'
                                        */
  int8_T DataStoreMemory2_InitialValue_d;/* Computed Parameter: DataStoreMemory2_InitialValue_d
                                          * Referenced by: '<S6>/Data Store Memory2'
                                          */
  int8_T DataStoreMemory1_InitialValue_o;/* Computed Parameter: DataStoreMemory1_InitialValue_o
                                          * Referenced by: '<Root>/Data Store Memory1'
                                          */
};

/* Real-time Model Data Structure */
struct RT_MODEL_ControlloTXRX {
  const char_T *errorStatus;
};

/* Block parameters (auto storage) */
extern Parameters_ControlloTXRX ControlloTXRX_P;

/* Block states (auto storage) */
extern D_Work_ControlloTXRX ControlloTXRX_DWork;

/*
 * Exported States
 *
 * Note: Exported states are block states with an exported global
 * storage class designation.  RTW declares the memory for these states
 * and exports their symbols.
 *
 */
extern real32_T Posizione;             /* '<S4>/Data Store Memory2' */
extern real32_T velocita1;             /* '<S4>/Data Store Memory3' */
extern real32_T I;                     /* '<S5>/Data Store Memory1' */
extern real32_T Rif_I;                 /* '<S5>/Data Store Memory2' */
extern real32_T I_cmd;                 /* '<Root>/Data Store Memory3' */
extern int16_T DUTYmotore1;            /* '<S7>/Data Store Memory' */
extern int16_T DUTYmotore2;            /* '<S7>/Data Store Memory1' */
extern int16_T Riferimento;            /* '<S6>/Data Store Memory1' */
extern int16_T Encoder1;               /* '<S4>/Data Store Memory1' */
extern int8_T segno;                   /* '<S6>/Data Store Memory2' */
extern int8_T master_slave;            /* '<Root>/Data Store Memory1' */

/* Model entry point functions */
extern void ControlloTXRX_initialize(void);
extern void ControlloTXRX_step(void);
extern void ControlloTXRX_terminate(void);

/* Real-time Model object */
extern struct RT_MODEL_ControlloTXRX *ControlloTXRX_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : ControlloTXRX
 * '<S1>'   : ControlloTXRX/Hardware
 * '<S2>'   : ControlloTXRX/P posizione
 * '<S3>'   : ControlloTXRX/PI Velocità1
 * '<S4>'   : ControlloTXRX/Hardware/EncoderMotore1 e 2
 * '<S5>'   : ControlloTXRX/Hardware/I_sense motor1
 * '<S6>'   : ControlloTXRX/Hardware/Rx
 * '<S7>'   : ControlloTXRX/Hardware/Subsystem
 */

/* user code (bottom of header file) */
/* System '<Root>' */
#define abs(x)                         ((x>0)?(x):-(x))
#endif                                 /* RTW_HEADER_ControlloTXRX_h_ */

/*
 * File trailer for Real-Time Workshop generated code.
 *
 * [EOF]
 */
