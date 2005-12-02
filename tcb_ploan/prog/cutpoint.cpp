/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: cutpoint.cpp
**    
** Description: How to modify a database
**
**         This is a tutorial sample showing basic database
**         modification. For a more comprehensive sample, see dbuse.sqC.
**           
****************************************************************************/
#include <string.h>
#include <stdio.h>

#if ((__cplusplus >= 199711L) && !defined DB2HP) || \
    (DB2LINUX && (__LP64__ || (__GNUC__ >= 3)) )
   #include <iostream>
   #include <fstream>
   #include <string>
   using namespace std; 
#else
   #include <iostream.h>
   #include <fstream.h>
   #include <string.h>
#endif

#include "functions.h"


int main(int argc, char *argv[])
{
  int periods, app_amount, i, j;
//  String case_sn;
  char case_sn[13];
  char line[128], buf[16];
  double apr, cut_point;
//  FILE *fp;
  ifstream infile("cut_input.txt");

  while (!infile.eof()) { 
     infile.getline(line, 128);
     i = j =0;
     while (line[i] !='\t' && line[i] !='\0') {
     	buf[j++] = line[i++];
     }	
     buf[j]= '\0';
     strcpy (case_sn, buf);

     j=0; i++;
     while (line[i] !='\t' && line[i] !='\0') {
     	buf[j++] = line[i++];
     }	
     buf[j]= '\0';
     app_amount = atoi (buf);

     j=0; i++;
     while (line[i] !='\t' && line[i] !='\0') {
     	buf[j++] = line[i++];
     }	
     buf[j]= '\0';
     periods = atoi (buf);
    
     j=0; i++;
     while (line[i] !='\t' && line[i] !='\0') {
     	buf[j++] = line[i++];
     }	
     buf[j]= '\0';
     apr = atof (buf);
     
//     infile >> case_sn >> app_amount >> periods >> apr;
     printf ("%s,%lf,%d,%d,",case_sn, apr, periods, app_amount);
     cut_point = get_risk_cut_point(app_amount, periods, apr);
     printf ("%f\n", cut_point);
  }

/*
	fp = fopen("cut_input.txt", "r");
	while (fscanf(fp, "%s\t%d\t%d\t%d", case_sn, app_amount, periods, apr) != NULL) {
           printf ("%s,%f,%d,%d,",case_sn, apr, periods, app_amount);
           cut_point = get_risk_cut_point(apr, periods, app_amount);
	}
*/

  return 0;
} //main
