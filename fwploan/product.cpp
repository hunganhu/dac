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
    error("申貸金額必須介於10和3000（千元）。");
  
  if((periods<1) || (periods>120))
    error ("貸款期數錯誤，必須介於1和120。");

  if((repayment!=1)&&(repayment!=2))
    error("還款方式錯誤，必須是1(本金法) 或2(本息法)。 ");

  if((grace_period<0) || (grace_period >= periods)) 
    error ("寬限期錯誤，必須介於0和貸款期數。");

  if((application_fee_ind!=0) || (application_fee_ind!=1))
      error ("手續費收取方式必須是0(比例) 或1(金額)");
  else if(application_fee_ind==0){
    if ((application_fee_pct<0)||(application_fee_pct>20)){
      error("手續費比例必須介於0和20(%)");
    }
  else if(application_fee<0 || application_fee){
    if (application_fee<0 || application_fee > 10000){
       error("手續費必須介於0和10000（元）");
      }
    }

  if((risk_ind!=0) || (risk_ind!=1))
      error ("風險管理費收取方式必須是0(比例) 或1(金額)");
  else if(risk_ind==0){
    if ((risk_mgmt_pct<0)||(risk_mgmt_pct>20)){
      error("風險管理費比例必須介於0和20(%)");
    }
  else if(risk_ind==1){
    if (risk_mgmt_fee<0 || risk_mgmt_fee > 10000){
       error("風險管理費必須介於0和10000（元）");
      }
    }

  if((acct_mgmt_ind!=0) || (acct_mgmt_ind!=1))
      error ("帳戶管理費收取方式必須是0(比例) 或1(金額)");
  else if(acct_mgmt_ind==0){
    if ((acct_mgmt_pct<0)||(acct_mgmt_pct>20)){
      error("帳戶管理費比例必須介於0和20(%)");
    }
  else if(acct_mgmt_ind==1){
    if (acct_mgmt_fee<0 || acct_mgmt_fee > 10000){
       error("帳戶管理費必須介於0和10000（元）");
      }
    }

  if((bt_ind!=0) || (bt_ind!=1))
      error ("代償費收取方式必須是0(比例) 或1(金額)");
  else if(bt_ind==0){
    if ((bt_pct<0)||(bt_pct>20)){
      error("代償費比例必須介於0和20(%)");
    }
  else if(bt_ind==1){
    if (bt_fee<0 || bt_fee > 10000){
       error("代償費必須介於0和10000（元）");
      }
    }
 if (appropriation != 1){
      error("撥款方式錯誤, 必須是1(一次撥貸)。");
 }
    
 if ((apr<0)||(apr>20)){
      error("貸款利率錯誤，必須介於0和20(%)");
    }
    
 if ((count of app_i) != num_int_period){
      error("貸款利率表錯誤");
 }
 if ((sum of period in app_i) != (period in app_info)){
      error("貸款利率表錯誤");
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