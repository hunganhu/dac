#ifndef VCLH
#include <vcl.h>
#endif
#include <time.h>
#include <new>
#include <AdoHandle.h> 

/*
if not exists (KRM001 & KRM023 & BAM086 & STM001 & JAS002) {
   PB=101;
   error ("No JCIC data, please review manually.");
   return(-101);
}   

if ( bad in JAS002 > 3 year) {
   PD = 103;
   return (-103);
} else if (krm023(bucket) >= 4 ) {
   PD = 104;
   return (-104);
} else if (bam086 fs044 > 0) {
   PD = 105;
   return (-105);
} else if (cash card bucket >= 0.5) {
   PD = 106;
   return (-106);
} else if (cash card  # (utilization =100%) >= 1) {
   PD = 107;
   return (-107);
}
if ( not exists krm001 or
     not exists krm023) {
   PD=102;
   error ("No enough JCIC data, please review manually.");
   return (-102);
}
calculate score and PD;
calculate NPV;

data validation algorithm:

  if(principle < 10 || principle > 3000)
    error("�ӶU���B��������10�M3000�]�d���^�C");
  
  if((periods<1) || (periods>120))
    error ("�U�ڴ��ƿ��~�A��������1�M120�C");

  if((repayment!=1)&&(repayment!=2))
    error("�ٴڤ覡���~�A�����O1(�����k) ��2(�����k)�C ");

  if((grace_period<0) || (grace_period >= periods)) 
    error ("�e�������~�A��������0�M�U�ڴ��ơC");

  if((application_fee_ind!=0) || (application_fee_ind!=1))
      error ("����O�����覡�����O0(���) ��1(���B)");
  else if(application_fee_ind==0){
    if ((application_fee_pct<0)||(application_fee_pct>20)){
      error("����O��ҥ�������0�M20(%)");
    }
  else if(application_fee<0 || application_fee){
    if (application_fee<0 || application_fee > 10000){
       error("����O��������0�M10000�]���^");
      }
    }

  if((risk_ind!=0) || (risk_ind!=1))
      error ("���I�޲z�O�����覡�����O0(���) ��1(���B)");
  else if(risk_ind==0){
    if ((risk_mgmt_pct<0)||(risk_mgmt_pct>20)){
      error("���I�޲z�O��ҥ�������0�M20(%)");
    }
  else if(risk_ind==1){
    if (risk_mgmt_fee<0 || risk_mgmt_fee > 10000){
       error("���I�޲z�O��������0�M10000�]���^");
      }
    }

  if((acct_mgmt_ind!=0) || (acct_mgmt_ind!=1))
      error ("�b��޲z�O�����覡�����O0(���) ��1(���B)");
  else if(acct_mgmt_ind==0){
    if ((acct_mgmt_pct<0)||(acct_mgmt_pct>20)){
      error("�b��޲z�O��ҥ�������0�M20(%)");
    }
  else if(acct_mgmt_ind==1){
    if (acct_mgmt_fee<0 || acct_mgmt_fee > 10000){
       error("�b��޲z�O��������0�M10000�]���^");
      }
    }

  if((bt_ind!=0) || (bt_ind!=1))
      error ("�N�v�O�����覡�����O0(���) ��1(���B)");
  else if(bt_ind==0){
    if ((bt_pct<0)||(bt_pct>20)){
      error("�N�v�O��ҥ�������0�M20(%)");
    }
  else if(bt_ind==1){
    if (bt_fee<0 || bt_fee > 10000){
       error("�N�v�O��������0�M10000�]���^");
      }
    }
 if (appropriation != 1){
      error("���ڤ覡���~, �����O1(�@�����U)�C");
 }
    
 if ((apr<0)||(apr>20)){
      error("�U�ڧQ�v���~�A��������0�M20(%)");
    }
    
 if ((count of app_i) != num_int_period){
      error("�U�ڧQ�v����~");
 }
 if ((sum of period in app_i) != (period in app_info)){
      error("�U�ڧQ�v����~");
 }

SinoPac:
101: No JCIC data
102: No KRM023 data
104: KRM023 max_bucket >3
108: DAM103 has record
116: DAM203 has record
100: loan amount/ nav >=95%
112: KRM023 max bucket > 3 and DAM103 has a record
120: KRM023 max bucket > 3 and DAM203 has a record
124: DAM103 & DAM203 have a record
128: KRM023 max bucket > 3 and DAM103 has a record and DAM203 has a record
*/

int dac_ploan_ev(char *case_sn, char *idn, int dac_sn, char *ole_db, char *error_msg)
{
 TADOHandler *dbhandle;
 Loan *ptrLoan;
 dbhandle = new TADOHandler();
 if (! dbhandle->OpenDatabase(ole_db)) {
    fprintf(stderr, "Error: Could not connect to database.");
    delete dbhandle;
    return (-1);

 ptrLoan = new Loan(dbhandle, case_sn, idn, dac_sn);
 ptrLoan->validate();

}