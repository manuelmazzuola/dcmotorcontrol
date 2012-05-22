/*
 * File: rt_SATURATE.h
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

#ifndef RTW_HEADER_rt_SATURATE_h_
#define RTW_HEADER_rt_SATURATE_h_
#define rt_SATURATE(sig,ll,ul)         (((sig) >= (ul)) ? (ul) : (((sig) <= (ll)) ? (ll) : (sig)) )
#endif                                 /* RTW_HEADER_rt_SATURATE_h_ */

/*
 * File trailer for Real-Time Workshop generated code.
 *
 * [EOF]
 */
