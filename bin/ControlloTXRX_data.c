/*
 * File: ControlloTXRX_data.c
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

/* Block parameters (auto storage) */
Parameters_ControlloTXRX ControlloTXRX_P = {
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<Root>/Constant'
                                        */
  0.009999999776F,                     /* Computed Parameter: P2_Gain
                                        * Referenced by: '<Root>/P2'
                                        */
  20.0F,                               /* Computed Parameter: P3_Gain
                                        * Referenced by: '<S2>/P3'
                                        */
  0.02500000037F,                      /* Computed Parameter: I2_gainval
                                        * Referenced by: '<S2>/I2'
                                        */
  0.0F,                                /* Computed Parameter: I2_IC
                                        * Referenced by: '<S2>/I2'
                                        */
  0.1000000015F,                       /* Computed Parameter: I2_UpperSat
                                        * Referenced by: '<S2>/I2'
                                        */
  -0.1000000015F,                      /* Computed Parameter: I2_LowerSat
                                        * Referenced by: '<S2>/I2'
                                        */
  0.1599999964F,                       /* Computed Parameter: P_Gain
                                        * Referenced by: '<S3>/P'
                                        */
  0.02449999936F,                      /* Computed Parameter: I2_gainval_b
                                        * Referenced by: '<S3>/I2'
                                        */
  0.0F,                                /* Computed Parameter: I2_IC_c
                                        * Referenced by: '<S3>/I2'
                                        */
  0.1000000015F,                       /* Computed Parameter: I2_UpperSat_g
                                        * Referenced by: '<S3>/I2'
                                        */
  -0.1000000015F,                      /* Computed Parameter: I2_LowerSat_a
                                        * Referenced by: '<S3>/I2'
                                        */
  0.2210000008F,                       /* Computed Parameter: P1_Gain
                                        * Referenced by: '<Root>/P1'
                                        */
  0.200000003F,                        /* Computed Parameter: Gain1_Gain
                                        * Referenced by: '<Root>/Gain1'
                                        */
  0.5109999776F,                       /* Computed Parameter: I2_gainval_i
                                        * Referenced by: '<Root>/I2'
                                        */
  0.0F,                                /* Computed Parameter: I2_IC_a
                                        * Referenced by: '<Root>/I2'
                                        */
  0.1000000015F,                       /* Computed Parameter: I2_UpperSat_e
                                        * Referenced by: '<Root>/I2'
                                        */
  0.0F,                                /* Computed Parameter: I2_LowerSat_f
                                        * Referenced by: '<Root>/I2'
                                        */
  1.0F,                                /* Computed Parameter: Saturation1_UpperSat
                                        * Referenced by: '<Root>/Saturation1'
                                        */
  0.0F,                                /* Computed Parameter: Saturation1_LowerSat
                                        * Referenced by: '<Root>/Saturation1'
                                        */
  255.0F,                              /* Computed Parameter: Gain_Gain
                                        * Referenced by: '<S7>/Gain'
                                        */
  0.0F,                                /* Computed Parameter: DataStoreMemory2_InitialValue
                                        * Referenced by: '<S4>/Data Store Memory2'
                                        */
  0.0F,                                /* Computed Parameter: DataStoreMemory3_InitialValue
                                        * Referenced by: '<S4>/Data Store Memory3'
                                        */
  0.0F,                                /* Computed Parameter: DataStoreMemory1_InitialValue
                                        * Referenced by: '<S5>/Data Store Memory1'
                                        */
  0.0F,                                /* Computed Parameter: DataStoreMemory2_InitialValue_g
                                        * Referenced by: '<S5>/Data Store Memory2'
                                        */
  0.0F,                                /* Computed Parameter: DataStoreMemory3_InitialValue_f
                                        * Referenced by: '<Root>/Data Store Memory3'
                                        */
  0,                                   /* Computed Parameter: DataStoreMemory_InitialValue
                                        * Referenced by: '<S7>/Data Store Memory'
                                        */
  0,                                   /* Computed Parameter: DataStoreMemory1_InitialValue_k
                                        * Referenced by: '<S7>/Data Store Memory1'
                                        */
  0,                                   /* Computed Parameter: DataStoreMemory1_InitialValue_e
                                        * Referenced by: '<S6>/Data Store Memory1'
                                        */
  0,                                   /* Computed Parameter: DataStoreMemory1_InitialValue_i
                                        * Referenced by: '<S4>/Data Store Memory1'
                                        */
  20480,                               /* Computed Parameter: Gain1_Gain_b
                                        * Referenced by: '<S7>/Gain1'
                                        */
  0,                                   /* Computed Parameter: Switch_Threshold
                                        * Referenced by: '<Root>/Switch'
                                        */
  0,                                   /* Computed Parameter: DataStoreMemory2_InitialValue_d
                                        * Referenced by: '<S6>/Data Store Memory2'
                                        */
  0                                    /* Computed Parameter: DataStoreMemory1_InitialValue_o
                                        * Referenced by: '<Root>/Data Store Memory1'
                                        */
};

/*
 * File trailer for Real-Time Workshop generated code.
 *
 * [EOF]
 */
