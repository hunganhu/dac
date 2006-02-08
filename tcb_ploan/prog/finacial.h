/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: finacial.h
**
** SAMPLE:  header file for finacial.cpp
**
****************************************************************************/

#ifndef FINACIAL_H
#define FINACIAL_H
#include <math.h>
#ifdef __cplusplus
extern "C"
{
#endif
enum TPaymentTime { ptEndOfPeriod, ptStartOfPeriod };
extern double PMT(double Rate, int NPeriods, const double PresentValue,
   		  const double FutureValue, TPaymentTime PaymentTime);
extern double NPV(double Rate, const double * CashFlows, const int CashFlows_Size,
   		  TPaymentTime PaymentTime);

extern double PV(const double Rate, int NPeriods, const double Payment,
   		 const double FutureValue, TPaymentTime PaymentTime);
extern double FV(const double Rate, int NPeriods, const double Payment,
   		 const double PresentValue, TPaymentTime PaymentTime);

#ifdef __cplusplus
}
#endif

#endif // FINACIAL_H

