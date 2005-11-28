/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2004
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: function.cpp
**
** Description:
**
**
**
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
#include "functions.h"

/*
Risk cut points are determined by the values of apr, term, and loan amount:
APR	Term	Loan Amt
2.00%	12	 50000
2.50%	24	100000
3.00%	36	150000
3.50%	48	200000
4.00%	60	250000
4.50%		300000
5.00%		350000
5.50%		400000
6.00%		450000
6.50%		500000
7.00%		550000
7.50%		600000
8.00%
8.50%
9.00%
9.50%
10.00%
10.50%
11.00%
11.50%
12.00%
*/
float risk_cut[12][5][21] =  // loan_amt, term, apr
{
 { //[0] 50000
   /*2.00%  2.50%   3.00%   3.50%   4.00%   4.50%   5.00%   5.50%   6.00%   6.50%   7.00%   7.50%   8.00%   8.50%   9.00%   9.50%   10.00%  10.50%  11.00%  11.50%  12.00%*/
   {0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0050, 0.0100, 0.0150, 0.0200}, // 12
   {0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0025, 0.0075, 0.0125, 0.0150, 0.0200, 0.0250, 0.0250, 0.0300, 0.0350, 0.0375, 0.0425, 0.0475}, // 24
   {0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0025, 0.0050, 0.0100, 0.0125, 0.0175, 0.0200, 0.0250, 0.0300, 0.0325, 0.0350, 0.0375, 0.0425, 0.0475, 0.0500, 0.0550}, // 36
   {0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0025, 0.0075, 0.0125, 0.0150, 0.0200, 0.0225, 0.0275, 0.0300, 0.0350, 0.0375, 0.0375, 0.0425, 0.0450, 0.0500, 0.0550, 0.0575}, // 48
   {0.0000, 0.0000, 0.0000, 0.0000, 0.0025, 0.0050, 0.0100, 0.0125, 0.0175, 0.0200, 0.0250, 0.0275, 0.0325, 0.0350, 0.0400, 0.0400, 0.0425, 0.0475, 0.0500, 0.0550, 0.0600}  // 60
 },

 { //[1] 100000
   /*2.00%  2.50%   3.00%   3.50%   4.00%   4.50%   5.00%   5.50%   6.00%   6.50%   7.00%   7.50%   8.00%   8.50%   9.00%   9.50%   10.00%  10.50%  11.00%  11.50%  12.00%*/
   {0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0050, 0.0100, 0.0150, 0.0200, 0.0275, 0.0325, 0.0375, 0.0400, 0.0450, 0.0500, 0.0550, 0.0625, 0.0675}, // 12
   {0.0000, 0.0000, 0.0000, 0.0000, 0.0025, 0.0075, 0.0125, 0.0150, 0.0200, 0.0250, 0.0300, 0.0350, 0.0400, 0.0425, 0.0475, 0.0500, 0.0550, 0.0600, 0.0650, 0.0700, 0.0750}, // 24
   {0.0000, 0.0000, 0.0000, 0.0025, 0.0075, 0.0125, 0.0175, 0.0200, 0.0250, 0.0300, 0.0325, 0.0375, 0.0425, 0.0450, 0.0500, 0.0525, 0.0575, 0.0625, 0.0675, 0.0700, 0.0750}, // 36
   {0.0000, 0.0000, 0.0025, 0.0075, 0.0100, 0.0150, 0.0175, 0.0225, 0.0275, 0.0300, 0.0350, 0.0400, 0.0425, 0.0475, 0.0525, 0.0550, 0.0575, 0.0625, 0.0675, 0.0700, 0.0750}, // 48
   {0.0000, 0.0000, 0.0025, 0.0075, 0.0125, 0.0150, 0.0200, 0.0225, 0.0275, 0.0300, 0.0350, 0.0400, 0.0425, 0.0475, 0.0500, 0.0525, 0.0575, 0.0625, 0.0650, 0.0700, 0.0750}  // 60
 },

 { //[2] 150000
   /*2.00%  2.50%   3.00%   3.50%   4.00%   4.50%   5.00%   5.50%   6.00%   6.50%   7.00%   7.50%   8.00%   8.50%   9.00%   9.50%   10.00%  10.50%  11.00%  11.50%  12.00%*/
   {0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0050, 0.0100, 0.0150, 0.0200, 0.0250, 0.0325, 0.0375, 0.0425, 0.0475, 0.0525, 0.0575, 0.0625, 0.0675, 0.0750, 0.0800, 0.0850}, // 12
   {0.0000, 0.0000, 0.0000, 0.0050, 0.0100, 0.0150, 0.0200, 0.0250, 0.0275, 0.0325, 0.0375, 0.0425, 0.0475, 0.0525, 0.0575, 0.0600, 0.0650, 0.0700, 0.0750, 0.0800, 0.0850}, // 24
   {0.0000, 0.0000, 0.0050, 0.0075, 0.0125, 0.0175, 0.0225, 0.0250, 0.0300, 0.0350, 0.0400, 0.0425, 0.0475, 0.0525, 0.0575, 0.0600, 0.0650, 0.0700, 0.0750, 0.0775, 0.0825}, // 36
   {0.0000, 0.0025, 0.0050, 0.0100, 0.0150, 0.0175, 0.0225, 0.0275, 0.0300, 0.0350, 0.0400, 0.0425, 0.0475, 0.0525, 0.0550, 0.0600, 0.0650, 0.0675, 0.0725, 0.0775, 0.0825}, // 48
   {0.0000, 0.0025, 0.0075, 0.0100, 0.0150, 0.0175, 0.0225, 0.0275, 0.0300, 0.0350, 0.0400, 0.0425, 0.0475, 0.0500, 0.0550, 0.0575, 0.0625, 0.0675, 0.0700, 0.0750, 0.0800}  // 60
 },

 { //[3] 200000
   /*2.00%  2.50%   3.00%   3.50%   4.00%   4.50%   5.00%   5.50%   6.00%   6.50%   7.00%   7.50%   8.00%   8.50%   9.00%   9.50%   10.00%  10.50%  11.00%  11.50%  12.00%*/
   {0.0000, 0.0000, 0.0000, 0.0000, 0.0075, 0.0125, 0.0175, 0.0225, 0.0275, 0.0350, 0.0400, 0.0450, 0.0500, 0.0575, 0.0625, 0.0675, 0.0725, 0.0775, 0.0825, 0.0875, 0.0950}, // 12
   {0.0000, 0.0000, 0.0050, 0.0100, 0.0150, 0.0175, 0.0225, 0.0275, 0.0325, 0.0375, 0.0425, 0.0475, 0.0525, 0.0575, 0.0625, 0.0650, 0.0700, 0.0750, 0.0800, 0.0850, 0.0900}, // 24
   {0.0000, 0.0025, 0.0075, 0.0100, 0.0150, 0.0200, 0.0250, 0.0300, 0.0325, 0.0375, 0.0425, 0.0475, 0.0500, 0.0550, 0.0600, 0.0650, 0.0700, 0.0725, 0.0775, 0.0825, 0.0875}, // 36
   {0.0000, 0.0025, 0.0075, 0.0125, 0.0175, 0.0200, 0.0250, 0.0300, 0.0325, 0.0375, 0.0425, 0.0450, 0.0500, 0.0550, 0.0575, 0.0625, 0.0675, 0.0725, 0.0750, 0.0800, 0.0850}, // 48
   {0.0000, 0.0050, 0.0075, 0.0125, 0.0175, 0.0200, 0.0250, 0.0275, 0.0325, 0.0375, 0.0400, 0.0450, 0.0500, 0.0525, 0.0575, 0.0600, 0.0650, 0.0700, 0.0725, 0.0775, 0.0825}  // 60
 },

 { //[4] 250000
   /*2.00%  2.50%   3.00%   3.50%   4.00%   4.50%   5.00%   5.50%   6.00%   6.50%   7.00%   7.50%   8.00%   8.50%   9.00%   9.50%   10.00%  10.50%  11.00%  11.50%  12.00%*/
   {0.0000, 0.0000, 0.0000, 0.0050, 0.0100, 0.0175, 0.0225, 0.0275, 0.0325, 0.0400, 0.0450, 0.0500, 0.0550, 0.0625, 0.0675, 0.0725, 0.0775, 0.0825, 0.0900, 0.0950, 0.1000}, // 12
   {0.0000, 0.0025, 0.0075, 0.0125, 0.0150, 0.0200, 0.0250, 0.0300, 0.0350, 0.0400, 0.0450, 0.0500, 0.0550, 0.0600, 0.0650, 0.0675, 0.0725, 0.0775, 0.0825, 0.0875, 0.0925}, // 24
   {0.0000, 0.0050, 0.0075, 0.0125, 0.0175, 0.0225, 0.0250, 0.0300, 0.0350, 0.0400, 0.0450, 0.0475, 0.0525, 0.0575, 0.0625, 0.0675, 0.0700, 0.0750, 0.0800, 0.0850, 0.0900}, // 36
   {0.0000, 0.0050, 0.0100, 0.0125, 0.0175, 0.0225, 0.0250, 0.0300, 0.0350, 0.0400, 0.0425, 0.0475, 0.0525, 0.0550, 0.0600, 0.0650, 0.0700, 0.0725, 0.0775, 0.0825, 0.0875}, // 48
   {0.0000, 0.0050, 0.0100, 0.0125, 0.0175, 0.0225, 0.0250, 0.0300, 0.0325, 0.0375, 0.0425, 0.0450, 0.0500, 0.0550, 0.0575, 0.0625, 0.0675, 0.0700, 0.0750, 0.0800, 0.0850}  // 60
 },

 { //[5] 300000
   /*2.00%  2.50%   3.00%   3.50%   4.00%   4.50%   5.00%   5.50%   6.00%   6.50%   7.00%   7.50%   8.00%   8.50%   9.00%   9.50%   10.00%  10.50%  11.00%  11.50%  12.00%*/
   {0.0000, 0.0000, 0.0025, 0.0075, 0.0150, 0.0200, 0.0250, 0.0300, 0.0375, 0.0425, 0.0475, 0.0550, 0.0600, 0.0650, 0.0700, 0.0750, 0.0825, 0.0875, 0.0925, 0.0975, 0.1050}, // 12
   {0.0000, 0.0025, 0.0075, 0.0125, 0.0175, 0.0225, 0.0275, 0.0325, 0.0375, 0.0425, 0.0475, 0.0525, 0.0575, 0.0625, 0.0650, 0.0700, 0.0750, 0.0800, 0.0850, 0.0900, 0.0950}, // 24
   {0.0000, 0.0050, 0.0100, 0.0150, 0.0175, 0.0225, 0.0275, 0.0325, 0.0375, 0.0400, 0.0450, 0.0500, 0.0550, 0.0600, 0.0625, 0.0675, 0.0725, 0.0775, 0.0825, 0.0875, 0.0900}, // 36
   {0.0025, 0.0050, 0.0100, 0.0150, 0.0175, 0.0225, 0.0275, 0.0325, 0.0350, 0.0400, 0.0450, 0.0475, 0.0525, 0.0575, 0.0600, 0.0650, 0.0700, 0.0750, 0.0800, 0.0825, 0.0875}, // 48
   {0.0025, 0.0050, 0.0100, 0.0150, 0.0175, 0.0225, 0.0275, 0.0300, 0.0350, 0.0375, 0.0425, 0.0475, 0.0500, 0.0550, 0.0600, 0.0650, 0.0675, 0.0725, 0.0775, 0.0800, 0.0850}  // 60
 },

 { //[6] 350000
   /*2.00%  2.50%   3.00%   3.50%   4.00%   4.50%   5.00%   5.50%   6.00%   6.50%   7.00%   7.50%   8.00%   8.50%   9.00%   9.50%   10.00%  10.50%  11.00%  11.50%  12.00%*/
   {0.0000, 0.0000, 0.0050, 0.0100, 0.0175, 0.0225, 0.0275, 0.0325, 0.0400, 0.0450, 0.0500, 0.0575, 0.0625, 0.0675, 0.0725, 0.0775, 0.0850, 0.0900, 0.0950, 0.1000, 0.1075}, // 12
   {0.0000, 0.0050, 0.0100, 0.0150, 0.0200, 0.0250, 0.0275, 0.0325, 0.0375, 0.0425, 0.0475, 0.0525, 0.0575, 0.0625, 0.0675, 0.0725, 0.0775, 0.0825, 0.0875, 0.0925, 0.0975}, // 24
   {0.0000, 0.0050, 0.0100, 0.0150, 0.0200, 0.0225, 0.0275, 0.0325, 0.0375, 0.0425, 0.0475, 0.0500, 0.0550, 0.0600, 0.0650, 0.0700, 0.0725, 0.0775, 0.0825, 0.0875, 0.0925}, // 36
   {0.0025, 0.0050, 0.0100, 0.0150, 0.0200, 0.0225, 0.0275, 0.0325, 0.0350, 0.0400, 0.0450, 0.0500, 0.0525, 0.0575, 0.0625, 0.0675, 0.0700, 0.0750, 0.0800, 0.0850, 0.0875}, // 48
   {0.0025, 0.0075, 0.0100, 0.0150, 0.0175, 0.0225, 0.0275, 0.0300, 0.0350, 0.0400, 0.0425, 0.0475, 0.0525, 0.0550, 0.0600, 0.0650, 0.0700, 0.0725, 0.0775, 0.0825, 0.0850}  // 60
 },

 { //[7] 400000
   /*2.00%  2.50%   3.00%   3.50%   4.00%   4.50%   5.00%   5.50%   6.00%   6.50%   7.00%   7.50%   8.00%   8.50%   9.00%   9.50%   10.00%  10.50%  11.00%  11.50%  12.00%*/
   {0.0000, 0.0025, 0.0075, 0.0125, 0.0175, 0.0250, 0.0300, 0.0350, 0.0400, 0.0475, 0.0525, 0.0575, 0.0650, 0.0700, 0.0750, 0.0800, 0.0850, 0.0925, 0.0975, 0.1025, 0.1100}, // 12
   {0.0000, 0.0050, 0.0100, 0.0150, 0.0200, 0.0250, 0.0300, 0.0350, 0.0400, 0.0450, 0.0500, 0.0550, 0.0575, 0.0625, 0.0675, 0.0725, 0.0775, 0.0825, 0.0875, 0.0925, 0.0975}, // 24
   {0.0025, 0.0050, 0.0100, 0.0150, 0.0200, 0.0250, 0.0300, 0.0325, 0.0375, 0.0425, 0.0475, 0.0525, 0.0550, 0.0600, 0.0650, 0.0700, 0.0750, 0.0800, 0.0850, 0.0875, 0.0925}, // 36
   {0.0025, 0.0075, 0.0100, 0.0150, 0.0200, 0.0250, 0.0275, 0.0325, 0.0375, 0.0400, 0.0450, 0.0500, 0.0550, 0.0575, 0.0625, 0.0675, 0.0725, 0.0750, 0.0800, 0.0850, 0.0900}, // 48
   {0.0025, 0.0075, 0.0100, 0.0150, 0.0200, 0.0225, 0.0275, 0.0325, 0.0350, 0.0400, 0.0425, 0.0475, 0.0525, 0.0550, 0.0600, 0.0650, 0.0700, 0.0725, 0.0775, 0.0825, 0.0875}  // 60
 },

 { //[8] 450000
   /*2.00%  2.50%   3.00%   3.50%   4.00%   4.50%   5.00%   5.50%   6.00%   6.50%   7.00%   7.50%   8.00%   8.50%   9.00%   9.50%   10.00%  10.50%  11.00%  11.50%  12.00%*/
   {0.0000, 0.0025, 0.0075, 0.0150, 0.0200, 0.0250, 0.0300, 0.0375, 0.0425, 0.0475, 0.0550, 0.0600, 0.0650, 0.0700, 0.0775, 0.0825, 0.0875, 0.0925, 0.1000, 0.1050, 0.1100}, // 12
   {0.0000, 0.0050, 0.0100, 0.0150, 0.0200, 0.0250, 0.0300, 0.0350, 0.0400, 0.0450, 0.0500, 0.0550, 0.0600, 0.0650, 0.0700, 0.0750, 0.0800, 0.0850, 0.0900, 0.0950, 0.1000}, // 24
   {0.0025, 0.0075, 0.0100, 0.0150, 0.0200, 0.0250, 0.0300, 0.0350, 0.0375, 0.0425, 0.0475, 0.0525, 0.0575, 0.0600, 0.0650, 0.0700, 0.0750, 0.0800, 0.0850, 0.0900, 0.0925}, // 36
   {0.0025, 0.0075, 0.0125, 0.0150, 0.0200, 0.0250, 0.0275, 0.0325, 0.0375, 0.0425, 0.0450, 0.0500, 0.0550, 0.0575, 0.0625, 0.0675, 0.0725, 0.0775, 0.0800, 0.0850, 0.0900}, // 48
   {0.0025, 0.0075, 0.0100, 0.0150, 0.0200, 0.0225, 0.0275, 0.0325, 0.0350, 0.0400, 0.0450, 0.0475, 0.0525, 0.0575, 0.0600, 0.0650, 0.0700, 0.0750, 0.0775, 0.0825, 0.0875}  // 60
 },

 { //[9] 500000
   /*2.00%  2.50%   3.00%   3.50%   4.00%   4.50%   5.00%   5.50%   6.00%   6.50%   7.00%   7.50%   8.00%   8.50%   9.00%   9.50%   10.00%  10.50%  11.00%  11.50%  12.00%*/
   {0.0000, 0.0025, 0.0100, 0.0150, 0.0200, 0.0275, 0.0325, 0.0375, 0.0425, 0.0500, 0.0550, 0.0600, 0.0675, 0.0725, 0.0775, 0.0825, 0.0900, 0.0950, 0.1000, 0.1075, 0.1125}, // 12
   {0.0025, 0.0075, 0.0125, 0.0150, 0.0200, 0.0250, 0.0300, 0.0350, 0.0400, 0.0450, 0.0500, 0.0550, 0.0600, 0.0650, 0.0700, 0.0750, 0.0800, 0.0850, 0.0900, 0.0950, 0.1000}, // 24
   {0.0025, 0.0075, 0.0125, 0.0150, 0.0200, 0.0250, 0.0300, 0.0350, 0.0400, 0.0425, 0.0475, 0.0525, 0.0575, 0.0625, 0.0675, 0.0700, 0.0750, 0.0800, 0.0850, 0.0900, 0.0950}, // 36
   {0.0025, 0.0075, 0.0125, 0.0150, 0.0200, 0.0250, 0.0300, 0.0325, 0.0375, 0.0425, 0.0450, 0.0500, 0.0550, 0.0600, 0.0625, 0.0675, 0.0725, 0.0775, 0.0825, 0.0850, 0.0900}, // 48
   {0.0025, 0.0075, 0.0125, 0.0150, 0.0200, 0.0225, 0.0275, 0.0325, 0.0350, 0.0400, 0.0450, 0.0475, 0.0525, 0.0575, 0.0600, 0.0650, 0.0700, 0.0750, 0.0775, 0.0825, 0.0875}  // 60
 },

 { //[10] 550000
   /*2.00%  2.50%   3.00%   3.50%   4.00%   4.50%   5.00%   5.50%   6.00%   6.50%   7.00%   7.50%   8.00%   8.50%   9.00%   9.50%   10.00%  10.50%  11.00%  11.50%  12.00%*/
   {0.0000, 0.0050, 0.0100, 0.0150, 0.0225, 0.0275, 0.0325, 0.0400, 0.0450, 0.0500, 0.0550, 0.0625, 0.0675, 0.0725, 0.0800, 0.0850, 0.0900, 0.0950, 0.1025, 0.1075, 0.1125}, // 12
   {0.0025, 0.0075, 0.0125, 0.0175, 0.0225, 0.0275, 0.0325, 0.0350, 0.0400, 0.0450, 0.0500, 0.0550, 0.0600, 0.0650, 0.0700, 0.0750, 0.0800, 0.0850, 0.0900, 0.0950, 0.1000}, // 24
   {0.0025, 0.0075, 0.0125, 0.0175, 0.0200, 0.0250, 0.0300, 0.0350, 0.0400, 0.0450, 0.0475, 0.0525, 0.0575, 0.0625, 0.0675, 0.0725, 0.0750, 0.0800, 0.0850, 0.0900, 0.0950}, // 36
   {0.0025, 0.0075, 0.0125, 0.0150, 0.0200, 0.0250, 0.0300, 0.0325, 0.0375, 0.0425, 0.0475, 0.0500, 0.0550, 0.0600, 0.0625, 0.0675, 0.0725, 0.0775, 0.0825, 0.0875, 0.0900}, // 48
   {0.0025, 0.0075, 0.0125, 0.0150, 0.0200, 0.0250, 0.0275, 0.0325, 0.0350, 0.0400, 0.0450, 0.0475, 0.0525, 0.0575, 0.0600, 0.0675, 0.0700, 0.0750, 0.0800, 0.0825, 0.0875}  // 60
 },

 { //[11] 600000
   /*2.00%  2.50%   3.00%   3.50%   4.00%   4.50%   5.00%   5.50%   6.00%   6.50%   7.00%   7.50%   8.00%   8.50%   9.00%   9.50%   10.00%  10.50%  11.00%  11.50%  12.00%*/
   {0.0000, 0.0050, 0.0100, 0.0175, 0.0225, 0.0275, 0.0350, 0.0400, 0.0450, 0.0500, 0.0575, 0.0625, 0.0675, 0.0750, 0.0800, 0.0850, 0.0900, 0.0975, 0.1025, 0.1075, 0.1150}, // 12
   {0.0025, 0.0075, 0.0125, 0.0175, 0.0225, 0.0275, 0.0325, 0.0375, 0.0425, 0.0475, 0.0525, 0.0550, 0.0600, 0.0650, 0.0700, 0.0750, 0.0800, 0.0850, 0.0900, 0.0950, 0.1000}, // 24
   {0.0025, 0.0075, 0.0125, 0.0175, 0.0200, 0.0250, 0.0300, 0.0350, 0.0400, 0.0450, 0.0475, 0.0525, 0.0575, 0.0625, 0.0675, 0.0725, 0.0775, 0.0800, 0.0850, 0.0900, 0.0950}, // 36
   {0.0025, 0.0075, 0.0125, 0.0175, 0.0200, 0.0250, 0.0300, 0.0325, 0.0375, 0.0425, 0.0475, 0.0500, 0.0550, 0.0600, 0.0650, 0.0700, 0.0725, 0.0775, 0.0825, 0.0875, 0.0900}, // 48
   {0.0025, 0.0075, 0.0125, 0.0150, 0.0200, 0.0250, 0.0275, 0.0325, 0.0375, 0.0400, 0.0450, 0.0500, 0.0525, 0.0575, 0.0600, 0.0675, 0.0700, 0.0750, 0.0800, 0.0825, 0.0875}  // 60
 }
};

float get_risk_cut_point(float apr, int term, int loan_amt)
{
 int i, j, k;

 i = j = k = 0;

 if (apr < 0.0200) i = -1;
 else if (apr < 0.0250) i = 0;
 else if (apr < 0.0300) i = 1;
 else if (apr < 0.0350) i = 2;
 else if (apr < 0.0400) i = 3;
 else if (apr < 0.0450) i = 4;
 else if (apr < 0.0500) i = 5;
 else if (apr < 0.0550) i = 6;
 else if (apr < 0.0600) i = 7;
 else if (apr < 0.0650) i = 8;
 else if (apr < 0.0700) i = 9;
 else if (apr < 0.0750) i = 10;
 else if (apr < 0.0800) i = 11;
 else if (apr < 0.0850) i = 12;
 else if (apr < 0.0900) i = 13;
 else if (apr < 0.0950) i = 14;
 else if (apr < 0.1000) i = 15;
 else if (apr < 0.1050) i = 16;
 else if (apr < 0.1100) i = 17;
 else if (apr < 0.1150) i = 18;
 else if (apr < 0.1200) i = 19;
 else i = 20;

 if (term < 12) j = -1;
 else if (term < 24) j = 0;
 else if (term < 36) j = 1;
 else if (term < 48) j = 2;
 else if (term < 60) j = 3;
 else j = 4;

 if (loan_amt <  50000) k = -1;
 else if (loan_amt < 100000) k = 0;
 else if (loan_amt < 150000) k = 1;
 else if (loan_amt < 200000) k = 2;
 else if (loan_amt < 250000) k = 3;
 else if (loan_amt < 300000) k = 4;
 else if (loan_amt < 350000) k = 5;
 else if (loan_amt < 400000) k = 6;
 else if (loan_amt < 450000) k = 7;
 else if (loan_amt < 500000) k = 8;
 else if (loan_amt < 550000) k = 9;
 else if (loan_amt < 600000) k = 10;
 else k = 11;

// printf ("cut[%d][%d][%d]= %5.4f\n", k, j,  i, risk_cut[k][j][i]);
 if ( i == -1 || j == -1 || k == -1)
    return(0.0);
 else
    return(risk_cut[k][j][i]);
}

void print_cut_point()
{
  int i, j, k;

  for (k = 0; k < 12; k++)
     for (j = 0; j < 5; j++)
        for (i = 0; i < 21; i++)
           printf ("cut[%d][%d][%4.2f%%]= %5.4f\n",
            (k+1)*500000, (j+1)*12,  (2.0+i*0.5), risk_cut[k][j][i]);

}


static char daytab[2][13] = {
 {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
 {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
};

int validate_date(char *date)  //format of date is yyymmdd
{
 int year, month, day, idate;
 int leap;

 if (date[0] == '\0') return 0;

 idate = atoi(date);
 year = idate / 10000;
 month = (idate % 10000) / 100;
 day = idate % 100;
 year += 1911;

 leap = (year%4 == 0 && year%100 != 0 || year%400 == 0);
 if (month > 12) return 0;
 if (day > daytab[leap][month]) return 0;

 return(1);
}

char *CurrTime_MinGuo ()
{
 time_t timer;
 struct tm *tblock;
 static char buf[20];

 timer = time(NULL);
 tblock = localtime(&timer);
 sprintf (buf, "%03d%02d%02d%02d%02d%02d", tblock->tm_year-11, tblock->tm_mon+1,
          tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec);
 return (buf);
}
