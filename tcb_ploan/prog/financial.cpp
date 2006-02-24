/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: function.cpp
** Description: Common C++ functions used by others functions
** 1. double PMT(double Rate, int NPeriods, const double PresentValue,
**    		  const double FutureValue, TPaymentTime PaymentTime);
** 
**    PMT calculates the fully amortized payment of borrowing PresentValue at Rate 
**    percent per period over NPeriods. It assumes that interest is paid at the end of 
**    each period. 
**    
**    FutureValue is the value that the investment will reach at some point. 
**    PaymentTime indicates whether the cash flows occur at the beginning or end of 
**    the period.
**    
**    		  
** 2. double NPV(double Rate, const double * CashFlows, const int CashFlows_Size,
**    		  TPaymentTime PaymentTime);
** 
**    NPV calculates the current value of an array of estimated cash flow 
**    values, discounted at the given interest rate of Rate. 
**    
**    CashFlows is the array of estimated cash flow values, and CashFlows_Size is the 
**    index of the last element in that array (one less than the number of elements).
**    
**    PaymentTime indicates whether the cash flows occur at the beginning or end of 
**    the period.
**    
**    NetPresentValue helps determine how much an investment is currently worth, based 
**    on expected earnings, although its accuracy depends on the accuracy of the cash 
**    flows in the array.
** 
** 3. double PV(const double Rate, int NPeriods, const double Payment,
**    		 const double FutureValue, TPaymentTime PaymentTime);
**
**    PV calculates the present value of an investment where Payment is 
**    received for NPeriods and is discounted at the rate of Rate per period. 
**    FutureValue is the value the investment may reach at some point. PaymentTime 
**    indicates whether the cash flows occur at the beginning or end of the period.
** 
** 4. double FV(const double Rate, int NPeriods, const double Payment,
**    		 const double PresentValue, TPaymentTime PaymentTime)
**
**    FV returns the future value of an investment of PresentValue where 
**    Payment is invested for NPeriods at the rate of Rate per period. The PaymentTime 
**    parameter indicates whether the investment is an ordinary annuity or an annuity 
**    due (enter ptEndOfPeriod if payments are at the end of each period, 
**    ptStartOfPeriod if they are at the beginning).
**    
****************************************************************************/
#include <string.h>
#if ((__cplusplus >= 199711L) && !defined DB2HP) || \
    (DB2LINUX && (__LP64__ || (__GNUC__ >= 3)) )
   #include <iostream>
   using namespace std;
#else
   #include <iostream.h>
#endif
#include "financial.h"

//--------------------------------------------------------------------------------------------------
double PMT(double Rate, int NPeriods, const double PresentValue,
   	   const double FutureValue, TPaymentTime PaymentTime)
{
 double Payment;
 
 if (Rate == 0.0) {
     Payment = (-PresentValue - FutureValue) / NPeriods;
 } else if (Rate > 0.0) {
     Payment = ((-PresentValue) * pow((1 + Rate), NPeriods) - FutureValue) /
           ((1 + Rate * PaymentTime) * (pow((1 + Rate), NPeriods) - 1) / Rate);
 }
 
 return Payment;
}   		  
//--------------------------------------------------------------------------------------------------
double NPV(double Rate, const double * CashFlows, const int CashFlows_Size,
   	   TPaymentTime PaymentTime)
{
 double NetPresentValue = 0.0;
 
 for (int i = 0; i <= CashFlows_Size; i++)
    NetPresentValue += *(CashFlows + i) / pow(1 + Rate, i+1);
 NetPresentValue = NetPresentValue * (1 + Rate * PaymentTime);   

 return NetPresentValue;
}   		  
//--------------------------------------------------------------------------------------------------
double PV(const double Rate, int NPeriods, const double Payment,
   	  const double FutureValue, TPaymentTime PaymentTime)
{
 double PresentValue;

 if (Rate == 0.0) {
     PresentValue = -Payment * NPeriods - FutureValue;
 } else if (Rate > 0.0) {
     PresentValue = (-Payment * ((1 + Rate * PaymentTime) * (pow((1 + Rate), NPeriods) - 1) / Rate)
           - FutureValue) /  pow((1 + Rate), NPeriods);
 }
 return PresentValue;
}   		  
//--------------------------------------------------------------------------------------------------
double FV(const double Rate, int NPeriods, const double Payment,
   	  const double PresentValue, TPaymentTime PaymentTime)
{
 double FutureValue;
 
 if (Rate == 0.0) {
     FutureValue = -Payment * NPeriods - PresentValue;
 } else if (Rate > 0.0) {
     FutureValue = (-Payment * ((1 + Rate * PaymentTime) * (pow((1 + Rate), NPeriods) - 1) / Rate)
           - PresentValue * pow((1 + Rate), NPeriods));
 }
 return FutureValue;
}   		  
//--------------------------------------------------------------------------------------------------
