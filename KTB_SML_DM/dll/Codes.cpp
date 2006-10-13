//---------------------------------------------------------------------------
#pragma hdrstop
#include <time.h>
#include "Codes.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

int DAC_SML_PRESCREEN(char *idn, char *msn, char *time_stamp, char *ole_db,
                      int monthly_income, char *error)
{
  AnsiString error_message="";
  AnsiString msn_no = static_cast<AnsiString>(msn);
  AnsiString time_stamp_no = static_cast<AnsiString>(time_stamp);
  AnsiString idno = static_cast<AnsiString>(idn);

  int prescreen_code = 0;
  AnsiString prescreen_msg = " ";

  double risk_score = 0;
//  bool is_il;
  double gender = ((idn[1]== '1')? 1: 0);

  unsigned char data_flag = 0x0;
  unsigned int filter_flag = 0x0;

  int status = 0;
  CoInitialize(NULL);

  TADOConnection *connection = new TADOConnection(NULL);
  TADOQuery *query = new TADOQuery(NULL);
  TADOQuery *query2 = new TADOQuery(NULL);
  TADOCommand *command = new TADOCommand(NULL);

  AnsiString inq_mon = time_stamp_no.SubString(1,8);
  bool time_lock = true;
  bool archive = false;
  bool use_krm037 = true;

  try {
    int now = yrmon_to_mon(inq_mon, time_lock, "20990531", archive, use_krm037);
    if (now > 0) {
	 AnsiString connection_string = static_cast<AnsiString>(ole_db);
	 connect(connection, connection_string);
 	 initialize_ado_objects(connection, query, query2, command);

	 copy_table(command, "KRM021", KRM001, msn_no, time_stamp_no);
 	 copy_table(command, "KRM023", KRM023, msn_no, time_stamp_no);
         copy_table(command, "KRM037", KRM037, msn_no, time_stamp_no);
   	 copy_table(command, "BAM086", BAM086, msn_no, time_stamp_no);
     	 copy_table(command, "STM007", STM007, msn_no, time_stamp_no);
     	 copy_table(command, "JAS002", JAS002, msn_no, time_stamp_no);

//       prepare_KRM023(command, KRM023, now);
         merge_prepare_KRM023_KRM037(command, KRM023, KRM037, now);
  	 prepare_KRM001(command, KRM001, now);
 	 prepare_BAM086(command, BAM086);
 	 prepare_STM007(command, STM007);
	 prepare_JAS002(command, JAS002, JAS002_T);
 	 create_common_working_tables(command);
     	 prepare_2xx_infra(command, now);
     	 get_idn_list(command, KRM001, KRM023, BAM086, STM007, JAS002_T);
     	 data_flag = get_data_availability(query, idno);
         filter_flag = in_pdaco_1_00(query, KRM023, KRM001, JAS002_T, BAM086, STM007, msn_no, idno, time_stamp_no, data_flag, monthly_income, now);

// PREMIER REVIEW
         if (filter_flag & 0x20) {    // Major derug
         	prescreen_code = DataErrorCode(OUTPUT_CODE_101);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_101);
         }
         else if (filter_flag & 0x04) { // credit card bucket_ef_1k > 3
        	prescreen_code = DataErrorCode(OUTPUT_CODE_102);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_102);
         }
         else if (filter_flag & 0x10) { // loan deliquent more then 90D+
        	prescreen_code = DataErrorCode(OUTPUT_CODE_103);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_103);
         }
         else if (filter_flag & 0x40) { // cash card deliquent
        	prescreen_code = DataErrorCode(OUTPUT_CODE_104);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_104);
         }
         else if (filter_flag & 0x80) { // cash card util >= 95%
        	prescreen_code = DataErrorCode(OUTPUT_CODE_105);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_105);
         }
         else if (filter_flag & 0x100){// balance of credit and cash card >= 500k
        	prescreen_code = DataErrorCode(OUTPUT_CODE_106);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_106);
         }
         else if (filter_flag & 0x2000){// loan deliquent in 30D
        	prescreen_code = DataErrorCode(OUTPUT_CODE_108);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_108);
         }
         else if (filter_flag & 0x4000){// unsecured loan >= 1M
        	prescreen_code = DataErrorCode(OUTPUT_CODE_109);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_109);
         }
         else if (filter_flag & 0x8000){// 22 fold of Monthly Income - unsecured loan < 0
        	prescreen_code = DataErrorCode(OUTPUT_CODE_110);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_110);
         }
         else if (filter_flag & 0x10000){// with VAM102 NOTE
        	prescreen_code = DataErrorCode(OUTPUT_CODE_111);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_111);
         }
         else if (filter_flag & 0x20000){// in KTB Credit Block list
        	prescreen_code = DataErrorCode(OUTPUT_CODE_112);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_112);
         }
         else { // pass prescreen, test data sufficiency
         	if (filter_flag & 0x01 || filter_flag & 0x02 || filter_flag & 0x08) { // IND001=1, not scorable
         	   if (data_flag & 0x01) { // k23 hit, big gray
		      if (filter_flag & 0x200){ // fs031 > 5
	        	 prescreen_code = DataErrorCode(OUTPUT_CODE_120);
	         	 prescreen_msg  = DataErrorMsg(OUTPUT_CODE_120);
	              } else if (filter_flag & 0x400){ // bucket_ef_1k > 0
	        	 prescreen_code = DataErrorCode(OUTPUT_CODE_121);
	         	 prescreen_msg  = DataErrorMsg(OUTPUT_CODE_121);
	              } else if (filter_flag & 0x800){ // k01 with stop_code=3
	        	 prescreen_code = DataErrorCode(OUTPUT_CODE_122);
	         	 prescreen_msg  = DataErrorMsg(OUTPUT_CODE_122);
	              } else {
	        	 prescreen_code = DataErrorCode(OUTPUT_CODE_201);
	         	 prescreen_msg  = DataErrorMsg(OUTPUT_CODE_201);
	              }
         	   }
         	   else if (data_flag & 0x02 || data_flag & 0x04) { // k01 or b86 hit, small gray
		      if (filter_flag & 0x1000){ // fs031 > 3
	        	 prescreen_code = DataErrorCode(OUTPUT_CODE_123);
	         	 prescreen_msg  = DataErrorMsg(OUTPUT_CODE_123);
	              } else if (filter_flag & 0x800){ // k01 with stop_code=3
	        	 prescreen_code = DataErrorCode(OUTPUT_CODE_124);
	         	 prescreen_msg  = DataErrorMsg(OUTPUT_CODE_124);
	              } else {
	        	 prescreen_code = DataErrorCode(OUTPUT_CODE_202);
	         	 prescreen_msg  = DataErrorMsg(OUTPUT_CODE_202);
	              }
         	   } else { // others, small white
        	      prescreen_code = DataErrorCode(OUTPUT_CODE_203);
         	      prescreen_msg  = DataErrorMsg(OUTPUT_CODE_203);
         	   }
                }
                else { // scorable
		   risk_score = pdaco_1_00(command, query, msn_no, idno, time_stamp_no, gender, data_flag,
		        KRM023, KRM001, BAM086, JAS002, STM007, now, true);
                   if (risk_score > 0.06657) {
        		prescreen_code = DataErrorCode(OUTPUT_CODE_107);
         		prescreen_msg  = DataErrorMsg(OUTPUT_CODE_107);
                   }
                } // end of scorable
         } // end of test data sufficiency,
           // end of prescreen

         if (prescreen_code == 0) { // prescreen pass
            // get property info
            // Two qualified properties are combined into one
            // if 4 conditions match, gav = nav = 0
//            if (gav == 0 || nav == 0 ) {
//               prescreen_code = DataErrorCode(OUTPUT_CODE_301);
//               prescreen_msg  = DataErrorMsg(OUTPUT_CODE_301);
               // write result - transfer to p-loan
//            } else {
               // write normal result
               prescreen_code = DataErrorCode(PRESCREEN_PASS);
               prescreen_msg  = DataErrorMsg(PRESCREEN_PASS);
//            }
            write_premier_result (command, msn_no, time_stamp_no, risk_score, prescreen_code, prescreen_msg);
         } else { // prescreen fails
            // write prescreen failure result
            write_premier_result (command, msn_no, time_stamp_no, risk_score, prescreen_code, prescreen_msg);
         }
// FINAL REVIEW

//      } // end of error_check() >0
    } // end of now >0
    else {
    	error_message = "This module has expired!";
    	status = -1;
    }//end of expiration check
  } //end of try
  catch (Exception &E){
     error_message += E.Message.c_str();
     status = -1;
  }

  try {
   	clean_up(command);
  }
  catch(Exception &E){
   	error_message += E.Message.c_str();
   	status = -1;
  }

  query->Close();
  query2->Close();
  connection->Close();
  delete query;
  delete query2;
  delete command;
  delete connection;
  CoUninitialize();

  if (status < 0) { // system error
     status = -1;
     strcpy(error, error_message.c_str());
  } else { // normal output
     strcpy (error, prescreen_msg.c_str());
     status = prescreen_code;
  }
  return status;
} // end of DAC_SML_PRESCREEN()
//---------------------------------------------------------------------------

int DAC_SML_NPV(char *idn, char *msn, char *time_stamp, char *ole_db,
                double principal, double apr, int period, double app_fee,
                double gav, double nav, char *zip, double first_lien_value,
                int monthly_income, char *error)
{
  AnsiString error_message="";
  AnsiString msn_no = static_cast<AnsiString>(msn);
  AnsiString time_stamp_no = static_cast<AnsiString>(time_stamp);
  AnsiString idno = static_cast<AnsiString>(idn);
  AnsiString zip_no = static_cast<AnsiString>(zip);

  int prescreen_code = 0;
  AnsiString prescreen_msg = "";
  int final_code = 0;
  AnsiString final_msg = "";

  double secured_npv = 0;
  double secured_pb = 0;
  double loan_amount_secured = 0;  //for secured LOC and IL
//  double unsecured_npv = 0;
//  double unsecured_pb = 0;
//  double p_score;
//  double loan_amount_unsecured = 0; //for unsecured LOC and IL
  double risk_score = 0;
  double existing_mortgage = first_lien_value / 1.2;
//  int propensity_decile = 0;
//  int product_type = -1;
//  bool is_il;
  int gender = ((idn[1]== '1')? 1: 0);
//  gender = 1; // remove after test


  unsigned char data_flag = 0x0;
  unsigned int filter_flag = 0x0;
  unsigned int filter;
  double balance = 0;
  int secured_npv_amount;

//  unsigned char group;
//  bool bsp_exclusion = ((nav == 0) && (gav == 0)) ? true : false;
//  int decline_code = 0;

//  if(bsp_exclusion)
//  	decline_code = 1;

  int status = 0;
  CoInitialize(NULL);

  TADOConnection *connection = new TADOConnection(NULL);
  TADOQuery *query = new TADOQuery(NULL);
  TADOQuery *query2 = new TADOQuery(NULL);
  TADOCommand *command = new TADOCommand(NULL);

  AnsiString inq_mon = time_stamp_no.SubString(1,8);
  bool time_lock = true;
  bool archive = false;
  bool use_krm037 = true;

  try {
    int now = yrmon_to_mon(inq_mon, time_lock, "20990531", archive, use_krm037);
    if (now > 0) {
	 AnsiString connection_string = static_cast<AnsiString>(ole_db);
	 connect(connection, connection_string);
 	 initialize_ado_objects(connection, query, query2, command);

	 copy_table(command, "KRM021", KRM001, msn_no, time_stamp_no);
 	 copy_table(command, "KRM023", KRM023, msn_no, time_stamp_no);
         copy_table(command, "KRM037", KRM037, msn_no, time_stamp_no);
   	 copy_table(command, "BAM086", BAM086, msn_no, time_stamp_no);
     	 copy_table(command, "STM007", STM007, msn_no, time_stamp_no);
     	 copy_table(command, "JAS002", JAS002, msn_no, time_stamp_no);

//       prepare_KRM023(command, KRM023, now);
         merge_prepare_KRM023_KRM037(command, KRM023, KRM037, now);
  	 prepare_KRM001(command, KRM001, now);
 	 prepare_BAM086(command, BAM086);
 	 prepare_STM007(command, STM007);
	 prepare_JAS002(command, JAS002, JAS002_T);
 	 create_common_working_tables(command);
     	 prepare_2xx_infra(command, now);
     	 get_idn_list(command, KRM001, KRM023, BAM086, STM007, JAS002_T);
     	 data_flag = get_data_availability(query, idno);
         filter_flag = in_pdaco_1_00(query, KRM023, KRM001, JAS002_T, BAM086, STM007, msn_no, idno, time_stamp_no, data_flag, monthly_income, now);

// PREMIER REVIEW
         if (filter_flag & 0x20) {    // Major derug
         	prescreen_code = DataErrorCode(OUTPUT_CODE_101);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_101);
         }
         else if (filter_flag & 0x04) { // credit card bucket_ef_1k > 3
        	prescreen_code = DataErrorCode(OUTPUT_CODE_102);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_102);
         }
         else if (filter_flag & 0x10) { // loan deliquent more then 90D+
        	prescreen_code = DataErrorCode(OUTPUT_CODE_103);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_103);
         }
         else if (filter_flag & 0x40) { // cash card deliquent
        	prescreen_code = DataErrorCode(OUTPUT_CODE_104);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_104);
         }
         else if (filter_flag & 0x80) { // cash card util >= 95%
        	prescreen_code = DataErrorCode(OUTPUT_CODE_105);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_105);
         }
         else if (filter_flag & 0x100){// balance of credit and cash card >= 500k
        	prescreen_code = DataErrorCode(OUTPUT_CODE_106);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_106);
         }
         else if (filter_flag & 0x2000){// loan deliquent in 30D
        	prescreen_code = DataErrorCode(OUTPUT_CODE_108);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_108);
         }
         else if (filter_flag & 0x4000){// unsecured loan >= 1M
        	prescreen_code = DataErrorCode(OUTPUT_CODE_109);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_109);
         }
         else if (filter_flag & 0x8000){// 22 fold of Monthly Income - unsecured loan < 0
        	prescreen_code = DataErrorCode(OUTPUT_CODE_110);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_110);
         }
         else if (filter_flag & 0x10000){// with VAM102 NOTE
        	prescreen_code = DataErrorCode(OUTPUT_CODE_111);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_111);
         }
         else if (filter_flag & 0x20000){// in KTB Credit Block list
        	prescreen_code = DataErrorCode(OUTPUT_CODE_112);
         	prescreen_msg  = DataErrorMsg(OUTPUT_CODE_112);
         }
         else { // pass prescreen, test data sufficiency
         	if (filter_flag & 0x01 || filter_flag & 0x02 || filter_flag & 0x08) { // IND001=1, not scorable
         	   if (data_flag & 0x01) { // k23 hit, big gray
		      if (filter_flag & 0x200){ // fs031 > 5
	        	 prescreen_code = DataErrorCode(OUTPUT_CODE_120);
	         	 prescreen_msg  = DataErrorMsg(OUTPUT_CODE_120);
	              } else if (filter_flag & 0x400){ // bucket_ef_1k > 0
	        	 prescreen_code = DataErrorCode(OUTPUT_CODE_121);
	         	 prescreen_msg  = DataErrorMsg(OUTPUT_CODE_121);
	              } else if (filter_flag & 0x800){ // k01 with stop_code=3
	        	 prescreen_code = DataErrorCode(OUTPUT_CODE_122);
	         	 prescreen_msg  = DataErrorMsg(OUTPUT_CODE_122);
	              } else {
	        	 prescreen_code = DataErrorCode(OUTPUT_CODE_201);
	         	 prescreen_msg  = DataErrorMsg(OUTPUT_CODE_201);
	              }
         	   }
         	   else if (data_flag & 0x02 || data_flag & 0x04) { // k01 or b86 hit, small gray
		      if (filter_flag & 0x1000){ // fs031 > 3
	        	 prescreen_code = DataErrorCode(OUTPUT_CODE_123);
	         	 prescreen_msg  = DataErrorMsg(OUTPUT_CODE_123);
	              } else if (filter_flag & 0x800){ // k01 with stop_code=3
	        	 prescreen_code = DataErrorCode(OUTPUT_CODE_124);
	         	 prescreen_msg  = DataErrorMsg(OUTPUT_CODE_124);
	              } else {
	        	 prescreen_code = DataErrorCode(OUTPUT_CODE_202);
	         	 prescreen_msg  = DataErrorMsg(OUTPUT_CODE_202);
	              }
         	   } else { // others, small white
        	      prescreen_code = DataErrorCode(OUTPUT_CODE_203);
         	      prescreen_msg  = DataErrorMsg(OUTPUT_CODE_203);
         	   }
                }
                else { // scorable
		   risk_score = pdaco_1_00(command, query, msn_no, idno, time_stamp_no, gender, data_flag,
		        KRM023, KRM001, BAM086, JAS002, STM007, now, true);
                   balance = krm023_balance(query, idno, now) + bam009_balance(query, idno);
                   if (risk_score > 0.06657) {
        		prescreen_code = DataErrorCode(OUTPUT_CODE_107);
         		prescreen_msg  = DataErrorMsg(OUTPUT_CODE_107);
                   }
                } // end of scorable
         } // end of test data sufficiency,
           // end of prescreen

// FINAL REVIEW

         if (prescreen_code == 0) { // prescreen pass
            // get property info
            // Two qualified properties are combined into one
            // if 4 conditions match, gav = nav = 0
//            if ((lien > 1) || (relationship > 3) || (location > 2) ||
//                ((is_ray + is_seasand + is_dangerous) > 0) ) {
//               gav = nav = 0;
//            }

            if (gav == 0 || nav == 0) {  // unqualified property
               final_code = DataErrorCode(OUTPUT_CODE_301);
               final_msg  = DataErrorMsg(OUTPUT_CODE_301);
               write_final_result_fail (command, msn_no, final_code, final_msg);
            } else {  // qualified property
               // calculate npv, pb, max. loan amount
   	       il loan(apr, period, gav, nav, existing_mortgage, zip_no, principal, app_fee/1000.0, balance,
       	            risk_score, msn_no, idno, time_stamp_no, &error_message);
   	       if (loan.status() != 0) {
		  status = 0;
                  filter = filter_flag & 0x1ff;  // Only the first 9 bits are checked
       	          loan_amount_secured = loan.npv(true, secured_npv, secured_pb, filter, -1);
       	          loan_amount_secured *= 1000.0;
                  secured_npv_amount = static_cast<int>(secured_npv + 0.5) * 1000;
               }
               else {
                  strcpy(error,error_message.c_str());
                  status = -1;
               }
               // lending amount and credit line  policy for scorable
               // within boundary
               if (loan_amount_secured >= 100 && secured_npv >= 0) {
               	  if (principal > loan_amount_secured) {
               	     if ((risk_score <= 0.01836) &&
               	         (((principal - loan_amount_secured) <= 100) ||
                          ((loan_amount_secured / principal) > 0.9)))
               	     	loan_amount_secured = principal;
               	  }
                  final_code = DataErrorCode(OUTPUT_CODE_0);
                  final_msg  = DataErrorMsg(OUTPUT_CODE_0);
               } else {
                  final_code = DataErrorCode(OUTPUT_CODE_204);
                  final_msg  = DataErrorMsg(OUTPUT_CODE_204);
               }
               write_final_result (command, msn_no, secured_pb, secured_npv_amount,
                       loan_amount_secured, final_code, final_msg);
            }
         } else { // prescreen fails
            // write prescreen failure result
            final_code = prescreen_code;
            final_msg  = prescreen_msg;
            write_final_result_fail (command, msn_no, final_code, final_msg);
         }

//      } // end of error_check() >0
    } // end of now >0
    else {
    	error_message = "This module has expired!";
    	status = -1;
    }//end of expiration check
  } //end of try
  catch (Exception &E){
     error_message += E.Message.c_str();
     status = -1;
  }

  try{
   	clean_up(command);
  }
  catch(Exception &E){
   	error_message += E.Message.c_str();
   	status = -1;
  }

  query->Close();
  query2->Close();
  connection->Close();
  delete query;
  delete query2;
  delete command;
  delete connection;
  CoUninitialize();

  if (status < 0) { // system error
     status = -1;
     strcpy(error, error_message.c_str());
  } else { // normal output
     strcpy (error, final_msg.c_str());
     status = final_code;
  }

  return status;
} // end of DAC_SML_NPV()
//---------------------------------------------------------------------------
/*
int DAC_SML_NPV(char *idn, char *idn1, char *idn2,
  char *msn, char *time_stamp, char *ole_db, int gender, int sc,
  double principal, int test_cell,
  double gav, double nav, char *zip, double first_lien_value,
  char *error)
{
  AnsiString error_message="";
  AnsiString msn_no = static_cast<AnsiString>(msn);
  AnsiString time_stamp_no = static_cast<AnsiString>(time_stamp);
  AnsiString idno = static_cast<AnsiString>(idn);
  AnsiString idno1 = static_cast<AnsiString>(idn1);
  AnsiString idno2 = static_cast<AnsiString>(idn2);
  AnsiString zip_no = static_cast<AnsiString>(zip);
  double secured_npv = 0;
  double unsecured_npv = 0;
  double unsecured_pb = 0;
  double secured_pb = 0;
  double p_score;
  double loan_amount_secured = 0;  //for secured LOC and IL
  double loan_amount_unsecured = 0; //for unsecured LOC and IL
  double risk_score = 0;
  double existing_mortgage = first_lien_value / 1.2;
  int propensity_decile = 0;
  int product_type = -1;
  bool is_il;

  unsigned char data_flag = 0x0;
  unsigned int filter_flag = 0x0;
  double balance = 0;

  unsigned char group;
  bool bsp_exclusion = ((nav == 0) && (gav == 0)) ? true : false;
  int decline_code = 0;

  if(bsp_exclusion)
  	decline_code = 1;

  int status = 0;
  CoInitialize(NULL);

  TADOConnection *connection = new TADOConnection(NULL);
  TADOQuery *query = new TADOQuery(NULL);
  TADOQuery *query2 = new TADOQuery(NULL);
  TADOCommand *command = new TADOCommand(NULL);

  AnsiString inq_mon = "";//time_stamp_no.SubString(1,8);
  bool time_lock = true;
  bool archive = false;
  bool use_krm037 = true;

  try {
    int now = yrmon_to_mon(inq_mon, time_lock, "20160531", archive, use_krm037);
    if (now > 0) {
      product_type = get_product_type(test_cell) ;
      is_il = product_type == 1 ? true: false;

      if (error_checking(gender, test_cell, gav, nav, first_lien_value, principal, is_il, msn, &error_message)){
  	 status = -1;
      }
      else {
	 AnsiString connection_string = static_cast<AnsiString>(ole_db);
	 connect(connection, connection_string);
 	 initialize_ado_objects(connection, query, query2, command);
//   	 store_input(command, idno, msn_no, time_stamp_no, gender, principal,
//     		     test_cell, gav, nav, zip_no, first_lien_value, sc);

//         if (nav == 0)
//            nav = 0.000000000000001;
//   	 if ((existing_mortgage / nav > 0.95) && decline_code != 1)
//   	    decline_code = 2;
	 copy_table(command, "KRM001", KRM001, msn_no, time_stamp_no);
 	 copy_table(command, "KRM023", KRM023, msn_no, time_stamp_no);
         copy_table(command, "KRM037", KRM037, msn_no, time_stamp_no);
   	 copy_table(command, "BAM086", BAM086, msn_no, time_stamp_no);
     	 copy_table(command, "STM007", STM007, msn_no, time_stamp_no);
     	 copy_table(command, "JAS002", JAS002, msn_no, time_stamp_no);

//       prepare_KRM023(command, KRM023, now);
         merge_prepare_KRM023_KRM037(command, KRM023, KRM037, now);
  	 prepare_KRM001(command, KRM001, now);
 	 prepare_BAM086(command, BAM086);
 	 prepare_STM007(command, STM007);
	 prepare_JAS002(command, JAS002, JAS002_T);
 	 create_common_working_tables(command);
     	 prepare_2xx_infra(command, now);
     	 get_idn_list(command, KRM001, KRM023, BAM086, STM007, JAS002_T);
     	 data_flag = get_data_availability(query, idno);
         filter_flag = in_pdaco_1_00(query, KRM023, KRM001, JAS002_T, BAM086, STM007, msn_no, idno, time_stamp_no, data_flag, now);

         if (decline_code != 1){
            if (data_flag == 0)
               decline_code = 101;
            else if((filter_flag & 0x1) || (filter_flag & 0x2) || (filter_flag & 0x8))
               decline_code = 102;
            else if(filter_flag == 0x80)
               decline_code = 107;
         }

 	 if ((decline_code == 0) &&((filter_flag & 0x01) || (filter_flag & 0x02)))
 	    decline_code = 5;
 	 else if (((filter_flag & 0x04) || (filter_flag & 0x10) ||
                   (filter_flag & 0x20) || (filter_flag & 0x40)))
    	    decline_code = 4;

         if (decline_code == 0 || decline_code ==2 || decline_code > 100) {
            if (filter_flag == 0 || filter_flag == 0x80){
//               p_score = propensity(command, query, idno, now);
//     	       propensity_decile = propensity_cut(query, idno);
               risk_score =
           		pdaco_1_00(command, query, msn_no, idno, time_stamp_no, gender, data_flag, KRM023, KRM001,
             		       BAM086, JAS002, STM007, now, true);
               balance = krm023_balance(query, idno, now) + bam009_balance(query, idno);

            }

            if (is_il){ //IL
   	       il loan(test_cell, gav, nav, existing_mortgage, zip_no, principal, balance,
       	            risk_score, msn_no, idno, time_stamp_no, &error_message);
   	       if (loan.status()!=0) {
       	          loan_amount_secured = loan.npv(true, secured_npv, secured_pb, filter_flag, -1);
          	       loan_amount_unsecured = loan.npv(false, unsecured_npv, unsecured_pb, filter_flag, loan_amount_secured);
               }
               else {
                  strcpy(error,error_message.c_str());
                  status = -1;
               }
            }
            else {  //LOC
               loc loan(test_cell, gav, nav, existing_mortgage, zip_no, principal, balance,
                        propensity_decile, risk_score, msn_no, idno, time_stamp_no, &error_message);
   	       if (loan.status()!=0){
       	          loan_amount_secured = loan.npv(true, secured_npv, secured_pb, filter_flag, -1);
                  loan_amount_unsecured = loan.npv(false, unsecured_npv, unsecured_pb, filter_flag, loan_amount_secured);
               }
               else {
   		  strcpy(error,error_message.c_str());
       		  status = -1;
               }
            }//End of IL / LOC
        } // end of if (decline_code == 0 || decline_code ==2 || decline_code > 100)

        if (risk_score == 0){
            int risk_code = 0;
	    if (data_flag == 0)
    	       risk_code |= 0x01; //No JCIC data
	    if (filter_flag & 0x01)
      	       risk_code |= 0x02; //No KRM023 data
            if (filter_flag & 0x02)
 	       risk_code |= 0x04; //No KRM001 data
    	    if (filter_flag & 0x08)
               risk_code |= 0x08; //IND001
            if (filter_flag & 0x04)
     	       risk_code |= 0x10; //App max bucket >= 3
            if (filter_flag & 0x10)
	       risk_code |= 0x20;  //Fs044, BAM009 with pass due amount
      	    if (filter_flag & 0x20)
               risk_code |= 0x40; //Major derug
	    if (filter_flag & 0x40)
    	       risk_code |= 0x80; //Cash card delinquent in most recent month
            if (filter_flag & 0x80)
 	       risk_code |= 0x100; //Cash card utilization >= 100%
      	    risk_code += 100;
            risk_score = risk_code;
         }
         if (status != -1){
            if ((loan_amount_secured + loan_amount_unsecured) > 0 && decline_code < 100)
           	decline_code = 0;
            else if(decline_code == 0)
           	decline_code = 3;

            if (((filter_flag == 0) && (decline_code == 0)) || (decline_code > 100)){
               AnsiString msn_no_space = msn_no.TrimRight();
               double lending_ratio = (first_lien_value / 1.2 +
                	                 loan_amount_secured * 1000) / nav;

               store_result(command, idno, msn_no, time_stamp_no, test_cell,
   		      loan_amount_secured, loan_amount_unsecured,
                         risk_score, propensity_decile, principal,
                         lending_ratio, bsp_exclusion,
                         secured_npv, unsecured_npv, secured_pb, unsecured_pb, decline_code, sc, balance);
            } //end of normal case adjustment
            else {

              store_result(command, idno, msn_no, time_stamp_no, test_cell,
               	         0, 0, risk_score, 0, principal, 0, bsp_exclusion, 0, 0,
                         0, 0, decline_code, sc, balance);
      	   }//end of abnormal case output
        } //end of if status != -1
      } // end of error_check() >0
    } // end of now >0
    else {
    	error_message = "This module has expired!";
    	status = -1;
    }//end of expiration check
  } //end of try
  catch (Exception &E){
     error_message += E.Message.c_str();
     status = -1;
  }

  try{
   	clean_up(command);
  }
  catch(Exception &E){
   	error_message += E.Message.c_str();
   	status = -1;
  }

  query->Close();
  query2->Close();
  connection->Close();
  delete query;
  delete query2;
  delete command;
  delete connection;
  CoUninitialize();

  if (error_message.Length()>0)
     status = -1;
  strcpy(error, error_message.c_str());

  return status;
}
*/
void connect(TADOConnection *connection, const AnsiString &connection_string,
             int connection_timeout, int command_timeout)
{
  if(connection->Connected)
    connection->Close();
  connection->CommandTimeout = command_timeout;
  connection->ConnectionTimeout = connection_timeout;

  if(connection_string != "")
    connection->ConnectionString = connection_string;
  else{
    AnsiString ConnectionProvider = "Provider=SQLOLEDB.1;";
    AnsiString ConnectionPassword = "Password=";
    AnsiString ConnectionPersist = "Persist Security Info=True;";
    AnsiString ConnectionUser = "User ID=";
    AnsiString ConnectionCatalog = "Initial Catalog=";
    AnsiString ConnectionSource = "Data Source=";
    AnsiString password_ansi = "JCIC";

    ConnectionUser += ("JCIC;");
    ConnectionPassword += (password_ansi + ";");
    ConnectionCatalog += ("DAPHNE;");
    ConnectionSource += "GIZA";
    connection->ConnectionString = ConnectionProvider + ConnectionPassword +
                                   ConnectionPersist + ConnectionUser +
                                   ConnectionCatalog + ConnectionSource;
    }
  connection->Connected = true;
}

void initialize_ado_objects(TADOConnection *connection, TADOQuery *query,
                            TADOQuery * query2, TADOCommand *command,
                            int command_timeout)
{
  if(query->Active)
    query->Close();
  query->Connection = connection;
  query->CommandTimeout = command_timeout;
  query->EnableBCD = false;

  if(query2->Active)
    query2->Close();
  query2->Connection = connection;
  query2->CommandTimeout = command_timeout;
  query2->EnableBCD = false;


  command->Connection = connection;
  command->CommandTimeout = command_timeout;
  command->CommandType = cmdText;
}

void copy_table(TADOCommand *command,
                const AnsiString &source_table, const AnsiString &destination_table,
                const AnsiString &msn, const AnsiString &time_stamp)
{
    AnsiString sql_stmt;
try{
  sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[" + destination_table + "]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE " + destination_table + ";";
//  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}

  sql_stmt = "SELECT * INTO " + destination_table;
  sql_stmt+= " FROM " + source_table + " WHERE MSN = :msn AND INQUIRY_DATE = :time_stamp;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["msn"] = msn;
  command->Parameters->ParamValues["time_stamp"] = time_stamp;
  command->Execute();
}

int yrmon_to_mon(const AnsiString &inquiry_month,
                 bool time_lock, const AnsiString &lock,
                 bool archive, bool use_krm037)
{
  int year;
  int month;
  int day;
  if (inquiry_month == ""){
    tm *today;
    time_t ltime;
    ltime = time( NULL );
    today = localtime( &ltime );
    year = today->tm_year + 1900;
    month = today->tm_mon + 1;
    day = today->tm_mday;
  }
  else{
    year = inquiry_month.SubString(1,4).ToInt();
    month = inquiry_month.SubString(5,2).ToInt();
    day = inquiry_month.SubString(7,2).ToInt();
  }
  if(time_lock && (lock.Length()==8)){
    int end_year = lock.SubString(1,4).ToInt();
    int end_month = lock.SubString(5,2).ToInt();
    int end_day = lock.SubString(7,2).ToInt();
    int end_days = end_year * 365 + end_month * 30 + end_day;
    int days = year * 365 + month * 30 + day;
    if (days > end_days)
      return -1;
  }
  if((!archive) && (!use_krm037)){
    if(day < 16)
      --month;
    if(month == 0){
      month = 12;
      --year;
    }
  }
  else if ((!archive) && (use_krm037)){
    --month;
    if(month == 0){
      month = 12;
      --year;
    }
  }
  return (year - 1911) * 12 + month;
}

void prepare_KRM023(TADOCommand *command, const AnsiString &table, int now)
{
  AnsiString sql_stmt;

  sql_stmt = "UPDATE " + table + " ";
  sql_stmt += "SET ";
  sql_stmt += "MSN = (CASE WHEN MSN = '' THEN NULL ELSE MSN END), ";
  sql_stmt += "INQUIRY_DATE = (CASE WHEN INQUIRY_DATE = '' THEN NULL ELSE INQUIRY_DATE END), ";
  sql_stmt += "IDN = (CASE WHEN IDN = '' THEN NULL ELSE IDN END), ";
  sql_stmt += "YRMON = (CASE WHEN YRMON = '' THEN NULL ELSE YRMON END), ";
  sql_stmt += "ISSUE = (CASE WHEN ISSUE = '' THEN NULL ELSE ISSUE END), ";
  sql_stmt += "ISSUE_NAME = (CASE WHEN ISSUE_NAME = '' THEN NULL ELSE ISSUE_NAME END), ";
  sql_stmt += "KR_CODE = (CASE WHEN KR_CODE = '' THEN NULL ELSE KR_CODE END), ";
  sql_stmt += "LIMIT = (CASE WHEN LIMIT = '' THEN NULL ELSE LIMIT END), ";
  sql_stmt += "PAYMENT = (CASE WHEN PAYMENT = '' THEN NULL ELSE PAYMENT END), ";
  sql_stmt += "CASH = (CASE WHEN CASH = '' THEN NULL ELSE CASH END), ";
  sql_stmt += "PAY_CODE = (CASE WHEN PAY_CODE = '' THEN NULL ELSE PAY_CODE END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "ALTER TABLE " + table + " ";
  sql_stmt += "ADD PAYMENT_AMT FLOAT, BUCKET_DEF_1K INT, BUCKET_EF_1K INT, BUCKET_F_1K INT, MON_SINCE INT;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  //Change YRMON to proper format so that year of 99 will be smaller than year of 100
  sql_stmt = "UPDATE " + table + " SET YRMON = ";
  sql_stmt += "(CASE WHEN LEFT(LTRIM(RTRIM(YRMON)),1) NOT IN ('1', '0', '*') ";
  sql_stmt += "THEN '0' + LTRIM(RTRIM(YRMON)) ELSE LTRIM(RTRIM(YRMON))  END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  //Put value INTO MON_SINCE, MON_SINCE means that month since the first month
  //of the establishment of ROC; actually it IS 12 larger than it logically should be
  //But since our use on the column IS to calculate the difference, it IS OK
  sql_stmt = "UPDATE " + table + " SET MON_SINCE = ";
  sql_stmt += "(CASE WHEN LEFT(YRMON,1) = '*' THEN NULL ";
  sql_stmt += "ELSE LEFT(YRMON, 3) * 12 + RIGHT(LEFT(YRMON, 5), 2) END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
//For two columns payment
/*  sql_stmt = "UPDATE " + table + " SET PAYMENT_AMT = ";
  sql_stmt += "(CASE PAYMENT2 WHEN 'L' THEN 2 WHEN 'M' THEN 5 WHEN 'H' THEN 8 ";
  sql_stmt += "ELSE 0 END) * POWER(10, ISNULL(PAYMENT,0)-1) / 1000.0;";
  command->CommandText = sql_stmt;
  command->Execute();*/
//For one column payment
  sql_stmt = "UPDATE " + table + " SET PAYMENT = ";
  sql_stmt += "(CASE WHEN RIGHT(LEFT(PAYMENT,2),1) IN ('H', 'M', 'L') THEN '0' + (LEFT(PAYMENT,2)) ELSE PAYMENT END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
  sql_stmt = "UPDATE " + table + " SET PAYMENT_AMT = ";
  sql_stmt += "(CASE RIGHT(PAYMENT,1) WHEN 'L' THEN 2 WHEN 'M' THEN 5 WHEN 'H' THEN 8 ";
  sql_stmt += "ELSE 0 END) * POWER(10, ISNULL(LEFT(PAYMENT,2),0)-1) / 1000.0;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//Calculate the buckets, first initialize the columns
  sql_stmt = "UPDATE " + table + " SET ";
  sql_stmt += "BUCKET_DEF_1K = 0, BUCKET_EF_1K = 0, BUCKET_F_1K = 0;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//The second phase of columns initialization for bucket assignment
  sql_stmt = "UPDATE " + table + " SET ";
  sql_stmt += "BUCKET_DEF_1K = (CASE WHEN PAY_CODE in ('D', 'E', 'F') AND ";
  sql_stmt += "PAYMENT_AMT > 1 THEN 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_EF_1K = (CASE WHEN PAY_CODE in ('E', 'F') AND ";
  sql_stmt += "PAYMENT_AMT > 1 THEN 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_F_1K = (CASE WHEN PAY_CODE = 'F' AND ";
  sql_stmt += "PAYMENT_AMT > 1 THEN 1 ELSE 0 END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//The bucket assignment process
  sql_stmt = "DECLARE @i INT SET @i=12 ";
  sql_stmt += "WHILE @i > 0 BEGIN UPDATE " + table + " SET ";
  sql_stmt += "BUCKET_DEF_1K =(CASE WHEN " + table + ".PAY_CODE IN ('D', 'E', 'F') ";
  sql_stmt += " AND " + table + ".PAYMENT_AMT > 1 THEN A.BUCKET_DEF_1K + 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_EF_1K =(CASE WHEN " + table + ".PAY_CODE IN ('E', 'F') ";
  sql_stmt += "AND " + table + ".PAYMENT_AMT > 1 THEN A.BUCKET_EF_1K + 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_F_1K =(CASE WHEN " + table + ".PAY_CODE = 'F' ";
  sql_stmt += "AND " + table + ".PAYMENT_AMT > 1 THEN A.BUCKET_F_1K + 1 ELSE 0 END) ";
  sql_stmt += "FROM " + table + " AS A INNER JOIN " + table + " ON ";
  sql_stmt += "A.IDN = " + table + ".IDN AND A.ISSUE = " + table + ".ISSUE ";
  sql_stmt += "AND A.MON_SINCE = (" + table + ".MON_SINCE - 1) ";
  sql_stmt += "WHERE (:inq_mon - A.MON_SINCE) = @i SET @i = @i - 1 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["inq_mon"] = now;
  command->Execute();
}
void merge_prepare_KRM023_KRM037(TADOCommand *command, const AnsiString &krm023,
                                 const AnsiString &krm037, int now)
{
  AnsiString sql_stmt;

  sql_stmt = "ALTER TABLE " + krm023 + " "
             " ADD PAYMENT_AMT FLOAT, BUCKET_DEF_1K INT, BUCKET_EF_1K INT, BUCKET_F_1K INT, MON_SINCE INT, "
             " REVOL_BAL INT, PRE_OWED INT ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + krm037 + " SET "
             " bill_date  = (case when ltrim(bill_date)  = '' then null else ltrim(rtrim(bill_date)) end), "
             " issue      = (case when ltrim(issue)      = '' then null else issue end), "
             " issue_name = (case when ltrim(issue_name) = '' then null else issue_name end), "
             " card_type  = (case when ltrim(card_type)  = '' then null else card_type end), "
             " perm_limit = (case when ltrim(perm_limit) = '' then null else perm_limit end), "
             " temp_limit = (case when ltrim(temp_limit) = '' then null else temp_limit end), "
             " cash_limit = (case when ltrim(cash_limit) = '' then null else cash_limit end), "
             " payable    = (case when ltrim(payable)    = '' then null else payable end), "
             " cash_lent  = (case when ltrim(cash_lent)  = '' then null else cash_lent end), "
             " last_paya  = (case when ltrim(last_paya)  = '' then null else last_paya end), "
             " revol_bal  = (case when ltrim(revol_bal)  = '' then null else revol_bal end), "
             " pay_stat   = (case when ltrim(pay_stat)   = '' then null else pay_stat end), "
             " pay_code   = (case when ltrim(pay_code)   = '' then null else pay_code end), "
             " revol_rate = (case when ltrim(revol_rate) = '' then null else revol_rate end), "
             " pre_owed   = (case when ltrim(pre_owed)   = '' then null else pre_owed end), "
             " debt_code  = (case when ltrim(debt_code)  = '' then null else debt_code end), "
             " close_code = (case when ltrim(close_code) = '' then null else close_code end), "
             " clear_date = (case when ltrim(clear_date) = '' then null else clear_date end) ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
/*
  try{
    sql_stmt = "DROP TABLE KRM023_TMP";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }
*/

  sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[KRM037_CONVERT]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "   drop table [KRM037_CONVERT]; "
             " CREATE TABLE KRM037_CONVERT ("
             " 	MSN		CHAR(20),      "
             " 	IDN		CHAR(14),      "
             "	INQUIRY_DATE	CHAR(10),      "
             " 	YRMON		CHAR(5),       "
             " 	ISSUE		CHAR(3),       "
             " 	ISSUE_NAME	CHAR(40),      "
             " 	KR_CODE		CHAR(7),       "
             " 	LIMIT		INT,           "
             " 	PAYMENT		CHAR(3),       "
             " 	CASH		CHAR(1),       "
             " 	PAY_CODE	CHAR(1),       "
             " 	MON_SINCE	INT,           "
             " 	PAYMENT_AMT	FLOAT,         "
             " 	BUCKET_DEF_1K	INT DEFAULT 0, "
             " 	BUCKET_EF_1K	INT DEFAULT 0, "
             " 	BUCKET_F_1K	INT DEFAULT 0, "
             "	NOW		INT,           "
             "	CURR_INQMON	INT,           "
             " 	CNT		INT,           "
             "	REVOL_BAL 	INT,           "
             "	PRE_OWED 	INT            "
             " )";
//  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

sql_stmt = " insert into krm037_convert (MSN, inquiry_date, IDN, issue, issue_name, limit, kr_code, payment, revol_bal, pre_owed, cash, pay_code, mon_since)"
           "   select MSN, inquiry_date, IDN,                                              "
           "          (case when (issue = 'A82' and card_type = 'A') then 'AEA'            "
           "                when (issue = 'A82' and card_type = 'E') then 'AEE'            "
           "                when (issue = '021' and card_type = 'V') then 'CTV'            "
           "                when (issue = '021' and card_type = 'M') then 'CTM'            "
           "                when (issue = '021' and card_type = 'D') then 'CTD'            "
           "                when issue = '150' then '103'                                  "
           "                else issue end),                                               "
           "          issue_name, cast(perm_limit as int), card_type,"
           "          (case when right(left(last_paya,2),1) in ('H', 'M', 'L') then '0'+(left(last_paya,2)) "
           "          else last_paya end),"
           "          cast(revol_bal as int), cast(pre_owed as int), 'N',                  "
           "          (case when debt_code in ('A', 'B') then 'F'                          "
           "                when pay_stat = 'X' and pay_code = 'X' then '*'                "
           "                when pay_stat = '1' and pay_code = 'N' then 'A'                "
           "                when pay_stat = '1' and pay_code = '0' then 'B'                "
           "                when pay_stat = '2' and pay_code = 'N' then 'C'                "
           "                when pay_stat = '2' and pay_code = '0' then 'D'                "
           "                when pay_stat = '3' and pay_code between '1' and '7' then 'E'  "
           "                when pay_stat = '4' and pay_code between '1' and '7' then 'F'  "
           "                else null end), "
           "          (case when LEN(bill_date) = 4 and left(bill_date,1) between '1' and '9' "
           "                     then cast(left(bill_date, 2) as int) * 12 + cast(right(bill_date, 2) as int) "
           "                when LEN(bill_date) = 7 and cast(right(bill_date,2) as int) > 15 "
           "                     then cast(left(bill_date, 3)as int) * 12 + cast(SUBSTRING(bill_date, 4, 2) as int) "
           "                when LEN(bill_date) = 7 then cast(left(bill_date, 3) as int) * 12 + cast(SUBSTRING(bill_date, 4, 2) as int) - 1 "
           "                else null end) "
           "   from " + krm037 + "; "
           " update krm037_convert "
           "   set payment_amt = "
           "         (case right(payment,1) when 'L' then 2 when 'M' then 5 when 'H' then 8 "
           "          else 0 end) * power(10, cast(left(payment,2) as int)-1) / 1000.0; "
           " insert into " + krm023 + " (MSN, inquiry_date, IDN, issue, issue_name, limit, payment_amt, revol_bal, pre_owed, cash, pay_code, mon_since) "
           "   select MSN, inquiry_date, IDN, issue, "
           "          issue_name, cast(sum(limit) as char(5)), sum(payment_amt),  "
           "          sum(revol_bal), sum(pre_owed), cash, max(pay_code), mon_since "
           "   from krm037_convert "
           "   where mon_since > 1140 "
           "   group by MSN, inquiry_date, IDN, issue, issue_name, cash, mon_since; "
           " update " + krm023 + " "
           "    set pay_code = 'X' "
           "  where pay_code='*'; "
           " update " + krm023 + " "
           "    set cash = 'Y' "
           "    where exists (select * "
           "                  from " + krm037 + " s "
           "                  where " + krm023 + ".msn = s.msn "
           "                    and " + krm023 + ".mon_since = cast(left(s.bill_date, 3) as int) * 12 * cast(SUBSTRING(s.bill_date, 4, 2) as int) "
           "                    and " + krm023 + ".issue = s.issue "
           "                    and cast(s.cash_lent as int) > 0); ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();




//  sql_stmt = "INSERT INTO " + krm023 + "(MSN, IDN, TIME_STAMP, INQUIRY_DATE, ";
//  sql_stmt += "MON_SINCE, ISSUE, ISSUE_NAME, LIMIT, PAYMENT, PAYMENT_AMT, ";
//  sql_stmt += "CASH, PAY_CODE) SELECT MSN, IDN, TIME_STAMP, INQUIRY_DATE, MON_SINCE, ";
//  sql_stmt += "ISSUE, ISSUE_NAME, LIMIT, PAYMENT, PAYMENT_AMT, CASH, PAY_CODE ";
//  sql_stmt += "FROM KRM023_TMP WHERE MON_SINCE > 1140"; //1140 = 94 * 12 + 12
//  sql_stmt = sql_stmt.UpperCase();
//  command->CommandText = sql_stmt;
//  command->Execute();

  sql_stmt = "UPDATE " + krm023 + " SET ";
  sql_stmt += "YRMON = (CASE WHEN YRMON = '' THEN NULL ELSE YRMON END), ";
  sql_stmt += "ISSUE = (CASE WHEN ISSUE = '' THEN NULL ELSE ISSUE END), ";
  sql_stmt += "ISSUE_NAME = (CASE WHEN ISSUE_NAME = '' THEN NULL ELSE ISSUE_NAME END), ";
  sql_stmt += "KR_CODE = (CASE WHEN KR_CODE = '' THEN NULL ELSE KR_CODE END), ";
  sql_stmt += "LIMIT = (CASE WHEN LIMIT = '' THEN NULL ELSE LIMIT END), ";
  sql_stmt += "PAYMENT = (CASE WHEN PAYMENT = '' THEN NULL ELSE PAYMENT END), ";
  sql_stmt += "CASH = (CASE WHEN CASH = '' THEN NULL ELSE CASH END), ";
  sql_stmt += "PAY_CODE = (CASE WHEN PAY_CODE = '' THEN NULL ELSE PAY_CODE END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  //Change YRMON to proper format so that year of 99 will be smaller than year of 100
  sql_stmt = "UPDATE " + krm023 + " SET YRMON = ";
  sql_stmt += "(CASE WHEN LEFT(LTRIM(RTRIM(YRMON)),1) NOT IN ('1', '0', '*') ";
  sql_stmt += "THEN '0' + LTRIM(RTRIM(YRMON)) ELSE LTRIM(RTRIM(YRMON))  END) ";
  sql_stmt += "WHERE YRMON IS NOT NULL";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  //Put value INTO MON_SINCE, MON_SINCE means that month since the first month
  //of the establishment of ROC; actually it IS 12 larger than it logically should be
  //But since our use on the column IS to calculate the difference, it IS OK
  sql_stmt = "UPDATE " + krm023 + " SET MON_SINCE = ";
  sql_stmt += "(CASE WHEN LEFT(YRMON,1) = '*' THEN NULL ";
  sql_stmt += "ELSE LEFT(YRMON, 3) * 12 + RIGHT(LEFT(YRMON, 5), 2) END) ";
  sql_stmt += "WHERE MON_SINCE IS NULL";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
//For two columns payment
/*  sql_stmt = "UPDATE " + table + " SET PAYMENT_AMT = ";
  sql_stmt += "(CASE PAYMENT2 WHEN 'L' THEN 2 WHEN 'M' THEN 5 WHEN 'H' THEN 8 ";
  sql_stmt += "ELSE 0 END) * POWER(10, ISNULL(PAYMENT,0)-1) / 1000.0;";
  command->CommandText = sql_stmt;
  command->Execute();*/
//For one column payment
  sql_stmt = "UPDATE " + krm023 + " SET PAYMENT = ";
  sql_stmt += "(CASE WHEN RIGHT(LEFT(PAYMENT,2),1) IN ('H', 'M', 'L') THEN '0' + (LEFT(PAYMENT,2)) ELSE PAYMENT END) ";
  sql_stmt += "WHERE PAYMENT IS NOT NULL";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = " update " + krm023 + " "
             "   set payment_amt = 0 "
             "   where payment_amt is null; "
             " update " + krm023 + " "
             "   set payment_amt = cast(limit as int) "
             "   where payment_amt > cast(limit as int) "
             "     and issue != 'AEE' "
             " update " + krm023 + " "
             "   set revol_bal = payment_amt * 1000, "
             "       pre_owed = 0 "
             "   where revol_bal is null " ;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + krm023 + " SET PAYMENT_AMT = ";
  sql_stmt += "(CASE RIGHT(PAYMENT,1) WHEN 'L' THEN 2 WHEN 'M' THEN 5 WHEN 'H' THEN 8 ";
  sql_stmt += "ELSE 0 END) * POWER(10, ISNULL(LEFT(PAYMENT,2),0)-1) / 1000.0 ";
  sql_stmt += "WHERE ((PAYMENT_AMT IS NULL) OR (PAYMENT_AMT = 0))";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//Calculate the buckets, first initialize the columns
  sql_stmt = "UPDATE " + krm023 + " SET ";
  sql_stmt += "BUCKET_DEF_1K = 0, BUCKET_EF_1K = 0, BUCKET_F_1K = 0;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//The second phase of columns initialization for bucket assignment
  sql_stmt = "UPDATE " + krm023 + " SET ";
  sql_stmt += "BUCKET_DEF_1K = (CASE WHEN PAY_CODE in ('D', 'E', 'F') AND ";
  sql_stmt += "PAYMENT_AMT > 1 THEN 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_EF_1K = (CASE WHEN PAY_CODE in ('E', 'F') AND ";
  sql_stmt += "PAYMENT_AMT > 1 THEN 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_F_1K = (CASE WHEN PAY_CODE = 'F' AND ";
  sql_stmt += "PAYMENT_AMT > 1 THEN 1 ELSE 0 END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//The bucket assignment process
  sql_stmt = "DECLARE @i INT SET @i=12 ";
  sql_stmt += "WHILE @i > 0 BEGIN UPDATE " + krm023 + " SET ";
  sql_stmt += "BUCKET_DEF_1K =(CASE WHEN " + krm023 + ".PAY_CODE IN ('D', 'E', 'F') ";
  sql_stmt += " AND " + krm023 + ".PAYMENT_AMT > 1 THEN A.BUCKET_DEF_1K + 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_EF_1K =(CASE WHEN " + krm023 + ".PAY_CODE IN ('E', 'F') ";
  sql_stmt += "AND " + krm023 + ".PAYMENT_AMT > 1 THEN A.BUCKET_EF_1K + 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_F_1K =(CASE WHEN " + krm023 + ".PAY_CODE = 'F' ";
  sql_stmt += "AND " + krm023 + ".PAYMENT_AMT > 1 THEN A.BUCKET_F_1K + 1 ELSE 0 END) ";
  sql_stmt += "FROM " + krm023 + " AS A INNER JOIN " + krm023 + " ON ";
  sql_stmt += "A.MSN = " + krm023 + ".MSN AND ";
  sql_stmt += "A.idn = " + krm023 + ".idn AND A.ISSUE = " + krm023 + ".ISSUE ";
  sql_stmt += "AND A.MON_SINCE = (" + krm023 + ".MON_SINCE - 1) ";
  sql_stmt += "WHERE (:inq_mon - A.MON_SINCE) = @i SET @i = @i - 1 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["inq_mon"] = now;
  command->Execute();
}

void merge_prepare_KRM023_KRM034(TADOCommand *command, const AnsiString &krm023,
                                 const AnsiString &krm034, int now)
{
  AnsiString sql_stmt;
  sql_stmt = "UPDATE " + krm034 + " SET ";
  sql_stmt += "BILL_DATE = (CASE WHEN BILL_DATE = '' THEN NULL ELSE BILL_DATE END), ";
  sql_stmt += "ISSUE = (CASE WHEN ISSUE = '' THEN NULL ELSE ISSUE END), ";
  sql_stmt += "ISSUE_NAME = (CASE WHEN ISSUE_NAME = '' THEN NULL ELSE ISSUE_NAME END), ";
  sql_stmt += "CARD_TYPE = (CASE WHEN CARD_TYPE = '' THEN NULL ELSE CARD_TYPE END), ";
  sql_stmt += "CASH_YN = (CASE WHEN CASH_YN = '' THEN NULL ELSE CASH_YN END), ";
//  sql_stmt += "PAYABLE = (CASE WHEN PAYABLE = '' THEN NULL ELSE PAYABLE END), ";
  sql_stmt += "LAST_PAYA = (CASE WHEN LAST_PAYA = '' THEN NULL ELSE LAST_PAYA END), ";
  sql_stmt += "PAY_STAT = (CASE WHEN PAY_STAT = '' THEN NULL ELSE PAY_STAT END), ";
  sql_stmt += "PAY_CODE = (CASE WHEN PAY_CODE = '' THEN NULL ELSE PAY_CODE END), ";
  sql_stmt += "DEBT_CODE = (CASE WHEN DEBT_CODE = '' THEN NULL ELSE DEBT_CODE END), ";
  sql_stmt += "CLOSE_CODE = (CASE WHEN CLOSE_CODE = '' THEN NULL ELSE CLOSE_CODE END), ";
  sql_stmt += "CLEAR_DATE = (CASE WHEN CLEAR_DATE = '' THEN NULL ELSE CLEAR_DATE END) ";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  try{
    sql_stmt = "DROP TABLE KRM023_TMP";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }


  sql_stmt = "CREATE TABLE KRM023_TMP ";
  sql_stmt += "(MSN CHAR(20), IDN CHAR(14), TIME_STAMP CHAR(12), INQUIRY_DATE CHAR(8), ";
  sql_stmt += "MON_SINCE INT, ISSUE CHAR(3), ISSUE_NAME CHAR(40), LIMIT CHAR(5),";
  sql_stmt += "PAYMENT CHAR(3), PAYMENT_AMT FLOAT, CASH CHAR, PAY_CODE CHAR)";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO KRM023_TMP (MSN, IDN, TIME_STAMP, INQUIRY_DATE, ";
  sql_stmt += "MON_SINCE, ISSUE, ISSUE_NAME, LIMIT, PAYMENT, ";
  sql_stmt += "CASH, PAY_CODE) SELECT MSN, IDN, TIME_STAMP, INQUIRY_DATE, ";
  sql_stmt += "(CASE WHEN LEFT(BILL_DATE,1) = '*' THEN NULL ";
  sql_stmt += "WHEN LEN(BILL_DATE) = 4 AND LEFT(BILL_DATE,1) BETWEEN '1' AND '9' ";
  sql_stmt += "THEN CONVERT(INT, LEFT(BILL_DATE, 2)) * 12 + CONVERT(INT, RIGHT(BILL_DATE, 2)) ";
  sql_stmt += "WHEN LEN(BILL_DATE) = 5 AND LEFT(BILL_DATE,1) = '0' THEN ";
  sql_stmt += "CONVERT(INT, LEFT(BILL_DATE, 3)) * 12 + CONVERT(INT, RIGHT(BILL_DATE, 2)) ";
  sql_stmt += "WHEN LEN(BILL_DATE) = 7 AND CONVERT(INT, RIGHT(BILL_DATE,2)) > 15 ";
  sql_stmt += "THEN CONVERT(INT, LEFT(BILL_DATE, 3)) * 12 + CONVERT(INT, SUBSTRING(BILL_DATE, 4, 2)) ";
  sql_stmt += "WHEN LEN(BILL_DATE) = 7 THEN CONVERT(INT,LEFT(BILL_DATE, 3)) * 12 + CONVERT(INT, SUBSTRING(BILL_DATE, 4, 2)) - 1 ";
  sql_stmt += "ELSE NULL END), ISSUE, ISSUE_NAME, PERM_LIMIT, LAST_PAYA, CASH_YN, ";
  sql_stmt += "(CASE WHEN PAY_STAT = 'X' AND PAY_CODE = 'X' THEN 'X' ";
  sql_stmt += "WHEN PAY_STAT = '1' AND PAY_CODE = 'N' THEN 'A' ";
  sql_stmt += "WHEN PAY_STAT = '1' AND PAY_CODE = '0' THEN 'B' ";
  sql_stmt += "WHEN PAY_STAT = '2' AND PAY_CODE = 'N' THEN 'C' ";
  sql_stmt += "WHEN PAY_STAT = '2' AND PAY_CODE = '0' THEN 'D' ";
  sql_stmt += "WHEN PAY_STAT = '3' AND PAY_CODE BETWEEN '1' AND '7' THEN 'E' ";
  sql_stmt += "WHEN PAY_STAT = '4' AND PAY_CODE BETWEEN '1' AND '7' THEN 'F' ";
  sql_stmt += "WHEN DEBT_CODE IN ('A', 'B') THEN 'F' ELSE NULL END) ";
  sql_stmt += "FROM " + krm034;
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "ALTER TABLE " + krm023 + " ";
  sql_stmt += "ADD PAYMENT_AMT FLOAT, BUCKET_DEF_1K INT, BUCKET_EF_1K INT, BUCKET_F_1K INT, MON_SINCE INT;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + krm023 + "(IDN, TIME_STAMP, INQUIRY_DATE, ";
  sql_stmt += "MON_SINCE, ISSUE, ISSUE_NAME, LIMIT, PAYMENT, CASH, ";
  sql_stmt += "PAY_CODE) SELECT IDN, TIME_STAMP, INQUIRY_DATE, MON_SINCE, ";
  sql_stmt += "ISSUE, ISSUE_NAME, LIMIT, PAYMENT, CASH, PAY_CODE ";
  sql_stmt += "FROM KRM023_TMP WHERE MON_SINCE > 1140"; //1140 = 94 * 12 + 12
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + krm023 + " SET ";
  sql_stmt += "YRMON = (CASE WHEN YRMON = '' THEN NULL ELSE YRMON END), ";
  sql_stmt += "ISSUE = (CASE WHEN ISSUE = '' THEN NULL ELSE ISSUE END), ";
  sql_stmt += "ISSUE_NAME = (CASE WHEN ISSUE_NAME = '' THEN NULL ELSE ISSUE_NAME END), ";
  sql_stmt += "KR_CODE = (CASE WHEN KR_CODE = '' THEN NULL ELSE KR_CODE END), ";
  sql_stmt += "LIMIT = (CASE WHEN LIMIT = '' THEN NULL ELSE LIMIT END), ";
  sql_stmt += "PAYMENT = (CASE WHEN PAYMENT = '' THEN NULL ELSE PAYMENT END), ";
  sql_stmt += "CASH = (CASE WHEN CASH = '' THEN NULL ELSE CASH END), ";
  sql_stmt += "PAY_CODE = (CASE WHEN PAY_CODE = '' THEN NULL ELSE PAY_CODE END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  //Change YRMON to proper format so that year of 99 will be smaller than year of 100
  sql_stmt = "UPDATE " + krm023 + " SET YRMON = ";
  sql_stmt += "(CASE WHEN LEFT(LTRIM(RTRIM(YRMON)),1) NOT IN ('1', '0', '*') ";
  sql_stmt += "THEN '0' + LTRIM(RTRIM(YRMON)) ELSE LTRIM(RTRIM(YRMON))  END) ";
  sql_stmt += "WHERE YRMON IS NOT NULL";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  //Put value INTO MON_SINCE, MON_SINCE means that month since the first month
  //of the establishment of ROC; actually it IS 12 larger than it logically should be
  //But since our use on the column IS to calculate the difference, it IS OK
  sql_stmt = "UPDATE " + krm023 + " SET MON_SINCE = ";
  sql_stmt += "(CASE WHEN LEFT(YRMON,1) = '*' THEN NULL ";
  sql_stmt += "ELSE LEFT(YRMON, 3) * 12 + RIGHT(LEFT(YRMON, 5), 2) END) ";
  sql_stmt += "WHERE MON_SINCE IS NULL";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
//For two columns payment
/*  sql_stmt = "UPDATE " + table + " SET PAYMENT_AMT = ";
  sql_stmt += "(CASE PAYMENT2 WHEN 'L' THEN 2 WHEN 'M' THEN 5 WHEN 'H' THEN 8 ";
  sql_stmt += "ELSE 0 END) * POWER(10, ISNULL(PAYMENT,0)-1) / 1000.0;";
  command->CommandText = sql_stmt;
  command->Execute();*/
//For one column payment
  sql_stmt = "UPDATE " + krm023 + " SET PAYMENT = ";
  sql_stmt += "(CASE WHEN RIGHT(LEFT(PAYMENT,2),1) IN ('H', 'M', 'L') THEN '0' + (LEFT(PAYMENT,2)) ELSE PAYMENT END) ";
  sql_stmt += "WHERE PAYMENT IS NOT NULL";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + krm023 + " SET PAYMENT_AMT = ";
  sql_stmt += "(CASE RIGHT(PAYMENT,1) WHEN 'L' THEN 2 WHEN 'M' THEN 5 WHEN 'H' THEN 8 ";
  sql_stmt += "ELSE 0 END) * POWER(10, ISNULL(LEFT(PAYMENT,2),0)-1) / 1000.0 ";
  sql_stmt += "WHERE ((PAYMENT_AMT IS NULL) OR (PAYMENT_AMT = 0))";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//Calculate the buckets, first initialize the columns
  sql_stmt = "UPDATE " + krm023 + " SET ";
  sql_stmt += "BUCKET_DEF_1K = 0, BUCKET_EF_1K = 0, BUCKET_F_1K = 0;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//The second phase of columns initialization for bucket assignment
  sql_stmt = "UPDATE " + krm023 + " SET ";
  sql_stmt += "BUCKET_DEF_1K = (CASE WHEN PAY_CODE in ('D', 'E', 'F') AND ";
  sql_stmt += "PAYMENT_AMT > 1 THEN 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_EF_1K = (CASE WHEN PAY_CODE in ('E', 'F') AND ";
  sql_stmt += "PAYMENT_AMT > 1 THEN 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_F_1K = (CASE WHEN PAY_CODE = 'F' AND ";
  sql_stmt += "PAYMENT_AMT > 1 THEN 1 ELSE 0 END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//The bucket assignment process
  sql_stmt = "DECLARE @i INT SET @i=12 ";
  sql_stmt += "WHILE @i > 0 BEGIN UPDATE " + krm023 + " SET ";
  sql_stmt += "BUCKET_DEF_1K =(CASE WHEN " + krm023 + ".PAY_CODE IN ('D', 'E', 'F') ";
  sql_stmt += " AND " + krm023 + ".PAYMENT_AMT > 1 THEN A.BUCKET_DEF_1K + 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_EF_1K =(CASE WHEN " + krm023 + ".PAY_CODE IN ('E', 'F') ";
  sql_stmt += "AND " + krm023 + ".PAYMENT_AMT > 1 THEN A.BUCKET_EF_1K + 1 ELSE 0 END), ";
  sql_stmt += "BUCKET_F_1K =(CASE WHEN " + krm023 + ".PAY_CODE = 'F' ";
  sql_stmt += "AND " + krm023 + ".PAYMENT_AMT > 1 THEN A.BUCKET_F_1K + 1 ELSE 0 END) ";
  sql_stmt += "FROM " + krm023 + " AS A INNER JOIN " + krm023 + " ON ";
  sql_stmt += "A.MSN = " + krm023 + ".MSN AND ";
  sql_stmt += "A.idn = " + krm023 + ".idn AND A.ISSUE = " + krm023 + ".ISSUE ";
  sql_stmt += "AND A.MON_SINCE = (" + krm023 + ".MON_SINCE - 1) ";
  sql_stmt += "WHERE (:inq_mon - A.MON_SINCE) = @i SET @i = @i - 1 END;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["inq_mon"] = now;
  command->Execute();
}

void prepare_KRM001(TADOCommand *command, const AnsiString &table, int now)
{
  AnsiString sql_stmt;

  sql_stmt = "UPDATE " + table + " SET ";
  sql_stmt += "MSN = (CASE WHEN MSN = '' THEN NULL ELSE MSN END), ";
  sql_stmt += "INQUIRY_DATE = (CASE WHEN INQUIRY_DATE = '' THEN NULL ELSE INQUIRY_DATE END), ";
  sql_stmt += "IDN = (CASE WHEN IDN = '' THEN NULL ELSE IDN END), ";
  sql_stmt += "CARD_BRAND = (CASE WHEN CARD_BRAND = '' THEN NULL ELSE CARD_BRAND END), ";
  sql_stmt += "CARD_TYPE = (CASE WHEN CARD_TYPE = '' THEN NULL ELSE CARD_TYPE END), ";
  sql_stmt += "ISSUE = (CASE WHEN ISSUE = '' THEN NULL ELSE ISSUE END), ";
  sql_stmt += "ISSUE_NAME = (CASE WHEN ISSUE_NAME = '' THEN NULL ELSE ISSUE_NAME END), ";
  sql_stmt += "START_DATE = (CASE WHEN START_DATE = '' THEN NULL ELSE START_DATE END), ";
  sql_stmt += "STOP_DATE = (CASE WHEN STOP_DATE = '' THEN NULL ELSE STOP_DATE END), ";
  sql_stmt += "STOP_CODE = (CASE WHEN STOP_CODE = '' THEN NULL ELSE STOP_CODE END), ";
  sql_stmt += "AB_CODE = (CASE WHEN AB_CODE = '' THEN NULL ELSE AB_CODE END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//Create START_MON_SINCE and END_MON_SINCE for KRM001

  sql_stmt = "ALTER TABLE " + table + " ADD START_MON_SINCE INT, END_MON_SINCE INT;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + table + " SET START_DATE = (CASE  ";
  sql_stmt += " WHEN (LEFT(LTRIM(RTRIM(START_DATE)),1) BETWEEN '1' AND '9') AND (LEN(LTRIM(RTRIM(START_DATE)))<7) ";
  sql_stmt += "THEN '0' + LTRIM(RTRIM(START_DATE)) ELSE LTRIM(RTRIM(START_DATE)) END), ";
  sql_stmt += "STOP_DATE = (CASE WHEN (LEFT(LTRIM(RTRIM(STOP_DATE)),1) BETWEEN '1' AND '9') AND ";
  sql_stmt += "(LEN(LTRIM(RTRIM(STOP_DATE)))<7) AND STOP_DATE != '9991231' ";
  sql_stmt += "THEN '0' + LTRIM(RTRIM(STOP_DATE)) ELSE LTRIM(RTRIM(STOP_DATE)) END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + table + " SET STOP_DATE = '9991231' ";
  sql_stmt += " WHERE ((STOP_DATE = '') OR (STOP_DATE IS NULL));";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + table + " SET START_MON_SINCE = (CASE ";
  sql_stmt += "WHEN LEFT(START_DATE,1) NOT BETWEEN '0' AND '9' THEN NULL ";
  sql_stmt += "ELSE LEFT(START_DATE, 3) * 12 + RIGHT(LEFT(START_DATE, 5), 2) END), ";
  sql_stmt += "END_MON_SINCE = (CASE ";
  sql_stmt += "WHEN LEFT(STOP_DATE, 1) NOT BETWEEN '0' AND '9' THEN NULL ";
  sql_stmt += "ELSE LEFT(STOP_DATE, 3) * 12 + RIGHT(LEFT(STOP_DATE, 5), 2) END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
}

void prepare_BAM086(TADOCommand *command, const AnsiString &table)
{
  AnsiString sql_stmt;
  sql_stmt = "UPDATE " + table + " SET ";
  sql_stmt += "MSN = (CASE WHEN MSN = '' THEN NULL ELSE MSN END), ";
  sql_stmt += "INQUIRY_DATE = (CASE WHEN INQUIRY_DATE = '' THEN NULL ELSE INQUIRY_DATE END), ";
  sql_stmt += "IDN = (CASE WHEN IDN = '' THEN NULL ELSE IDN END), ";
  sql_stmt += "DATA_YYY = (CASE WHEN DATA_YYY = '' THEN NULL ELSE DATA_YYY END), ";
  sql_stmt += "DATA_MM = (CASE WHEN DATA_MM = '' THEN NULL ELSE DATA_MM END), ";
  sql_stmt += "BANK_CODE = (CASE WHEN BANK_CODE = '' THEN NULL ELSE BANK_CODE END), ";
  sql_stmt += "BANK_NAME = (CASE WHEN BANK_NAME = '' THEN NULL ELSE BANK_NAME END), ";
  sql_stmt += "ACCOUNT_CODE = (CASE WHEN ACCOUNT_CODE = '' THEN NULL ELSE ACCOUNT_CODE END), ";
  sql_stmt += "ACCOUNT_CODE2 = (CASE WHEN ACCOUNT_CODE2 = '' THEN NULL ELSE ACCOUNT_CODE2 END), ";
  sql_stmt += "PURPOSE_CODE = (CASE WHEN PURPOSE_CODE = '' THEN NULL ELSE PURPOSE_CODE END), ";
  sql_stmt += "PAY_CODE_12 = (CASE WHEN PAY_CODE_12 = '' THEN NULL ELSE PAY_CODE_12 END), ";
  sql_stmt += "CO_LOAN = (CASE WHEN CO_LOAN = '' THEN NULL ELSE CO_LOAN END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

//Create Bank_Code2 for BAM009
  sql_stmt = "ALTER TABLE " + table + " ADD BANK_CODE2 CHAR(3), MON_SINCE INT, CYCLE INT;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + table + " SET BANK_CODE2 = LEFT(BANK_CODE,3),";
  sql_stmt += " MON_SINCE = CAST(DATA_YYY AS INT)* 12 + CAST(DATA_MM AS INT),"
              " CYCLE = LEN(LTRIM(RTRIM(PAY_CODE_12)));";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[BAM086_BUCKET]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "   drop table [BAM086_BUCKET]; "
             " CREATE TABLE BAM086_BUCKET (IDN CHAR(14), BANK_CODE CHAR(3), MON_SINCE INT, BUCKET FLOAT);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt =  "DECLARE @i INT SET @i=1 "
              " while @i <= 12"
              "    begin"
              "       insert into bam086_bucket (idn, bank_code, mon_since, bucket) "
              "          select idn, bank_code2, (mon_since - @i + 1), "
              "                 (case when substring(rtrim(pay_code_12), @i, 1) = 'A' then 0.25"
              "                       when substring(rtrim(pay_code_12), @i, 1) = 'B' then 0.5"
              "                       when substring(rtrim(pay_code_12), @i, 1) = 'X' then 0"
              "                       else convert(float, substring(rtrim(pay_code_12), @i, 1)) end)"
              "          from " + table +
              "          where cycle >= @i"
              "       set @i = @i + 1"
              "    end";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

}

void prepare_STM007(TADOCommand *command, const AnsiString &table)
{
  AnsiString sql_stmt;
  sql_stmt = "UPDATE " + table + " SET ";
  sql_stmt += "MSN = (CASE WHEN MSN = '' THEN NULL ELSE MSN END), ";
  sql_stmt += "INQUIRY_DATE = (CASE WHEN INQUIRY_DATE = '' THEN NULL ELSE INQUIRY_DATE END), ";
  sql_stmt += "IDN = (CASE WHEN IDN = '' THEN NULL ELSE IDN END), ";
  sql_stmt += "Query_Date = (CASE WHEN Query_Date = '' THEN NULL ELSE Query_Date END), ";
  sql_stmt += "Bank_Code = (CASE WHEN Bank_Code = '' THEN NULL ELSE Bank_Code END), ";
  sql_stmt += "Bank_Name = (CASE WHEN Bank_Name = '' THEN NULL ELSE Bank_Name END), ";
  sql_stmt += "Item_List = (CASE WHEN Item_List = '' THEN NULL ELSE Item_List END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "ALTER TABLE " + table + " ADD QUERY_MON_SINCE INT;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + table + " SET QUERY_DATE = (CASE ";
  sql_stmt += "WHEN LEFT(QUERY_DATE,1) NOT IN ('1', '0', '*') THEN '0' + ";
  sql_stmt += "(QUERY_DATE)  ELSE QUERY_DATE END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + table + " SET QUERY_MON_SINCE = (CASE ";
  sql_stmt += "WHEN LEFT(QUERY_DATE,1) = '*' THEN NULL ";
  sql_stmt += "ELSE LEFT(QUERY_DATE, 3) * 12 + RIGHT(LEFT(QUERY_DATE, 5), 2) ";
  sql_stmt += " END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
}

void prepare_DAM103(TADOCommand *command, const AnsiString &table)
{
  AnsiString sql_stmt;
  sql_stmt = "ALTER TABLE " + table + " ";
  sql_stmt += "ADD CK_SINCE INT;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + table + " SET CK_DATE = ";
  sql_stmt += "(CASE WHEN LEFT(LTRIM(CK_DATE),1) NOT IN ('1', '0', '*') ";
  sql_stmt += "THEN '0' + RTRIM(LTRIM(CK_DATE)) ELSE RTRIM(LTRIM(CK_DATE)) END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + table + " SET CK_SINCE = ";
  sql_stmt += "(CASE WHEN LEFT(CK_DATE,1) = '*' THEN NULL ";
  sql_stmt += "ELSE LEFT(CK_DATE, 3) * 12 + RIGHT(LEFT(CK_DATE, 5), 2) END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
}

void prepare_DAM203(TADOCommand *command, const AnsiString &table)
{
  AnsiString sql_stmt;
  sql_stmt = "ALTER TABLE " + table + " ";
  sql_stmt += "ADD BEG_SINCE INT;";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + table + " SET BEG_DATE = ";
  sql_stmt += "(CASE WHEN LEFT(LTRIM(BEG_DATE),1) NOT IN ('1', '0', '*') ";
  sql_stmt += "THEN '0' + RTRIM(LTRIM(BEG_DATE)) ELSE RTRIM(LTRIM(BEG_DATE)) END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + table + " SET BEG_SINCE = ";
  sql_stmt += "(CASE WHEN LEFT(BEG_DATE,1) = '*' THEN NULL ";
  sql_stmt += "ELSE LEFT(BEG_DATE, 3) * 12 + RIGHT(LEFT(BEG_DATE, 5), 2) END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
}

void create_common_working_tables(TADOCommand * command)
{
  AnsiString sql_stmt;
/*
try{
  sql_stmt = "DROP TABLE TMP;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();

//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  sql_stmt = "DROP TABLE TMP1;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();

//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  sql_stmt = "DROP TABLE OPEN_CARD;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();

//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  sql_stmt = "DROP TABLE OPEN_LINE;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();

//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  sql_stmt = "DROP TABLE OPEN_ISSUER;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();

//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  sql_stmt = "DROP TABLE LATEST_STMT_MON;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();

//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  sql_stmt = "DROP TABLE LATEST_LINE;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();

//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
*/
  sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[TMP]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "   drop table [TMP]; "
             "CREATE TABLE TMP(IDN CHAR(14), MON INT, V1 FLOAT, "
             " V2 FLOAT, V3 FLOAT);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[TMP1]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "   drop table [TMP1]; "
             "CREATE TABLE TMP1(IDN CHAR(14), MON INT, V1 FLOAT);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[OPEN_CARD]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "   drop table [OPEN_CARD]; "
             "CREATE TABLE OPEN_CARD (IDN CHAR(14), ISSUE CHAR(3), MON_SINCE INT);";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[OPEN_LINE]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "   drop table [OPEN_LINE]; "
             "CREATE TABLE OPEN_LINE (IDN CHAR(14), ISSUE CHAR(3), MON_SINCE INT, "
             " Cards INT, BUCKET INT)";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[LATEST_STMT_MON]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "   drop table [LATEST_STMT_MON]; "
             "CREATE TABLE LATEST_STMT_MON (IDN CHAR(14), ISSUE CHAR(3), "
             " MON_SINCE INT)";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[LATEST_LINE]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "   drop table [LATEST_LINE]; "
             "CREATE TABLE LATEST_LINE (IDN CHAR(14), ISSUE CHAR(3), "
             " MON_SINCE INT, MOB INT)";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[OPEN_ISSUER]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "   drop table [OPEN_ISSUER]; "
             "CREATE TABLE OPEN_ISSUER (IDN CHAR(14), ISSUE CHAR(3), MON INT);";
  command->CommandText = sql_stmt;
  command->Execute();

}

double propensity(TADOCommand *command, TADOQuery *query, const AnsiString &idn, int now)
{
  AnsiString sql_stmt;

  create_propensity_tables(command);
  load_and_initialize_propensity(command, idn);

//generating s variables

/*----FS029----*/
  sql_stmt = "DELETE FROM TMP1;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, NULL, COUNT(*) ";
  sql_stmt += "FROM " + STM007 + " WHERE ((ITEM_LIST LIKE '%K%') ";
  sql_stmt += "AND (:now - QUERY_MON_SINCE <= 12)) ";
  sql_stmt += "AND IDN = :idn GROUP BY IDN;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS029 = V1 FROM TMP1 AS A WHERE A.IDN = PM.IDN;";
  command->CommandText = sql_stmt;
  command->Execute();

/*----FS031----*/
/*---Init temp table----*/
  sql_stmt = "DELETE FROM TMP1;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, NULL, COUNT(*) ";
  sql_stmt += "FROM " + STM007 + " WHERE ITEM_LIST IS NOT NULL AND ";
  sql_stmt += "ITEM_LIST <> '' AND (:now - QUERY_MON_SINCE <= 12) ";
  sql_stmt += "AND IDN = :idn GROUP BY IDN;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS031 = V1 FROM TMP1 AS A WHERE A.IDN = PM.IDN;";
  command->CommandText = sql_stmt;
  command->Execute();

/*----FS030----*/

  sql_stmt = "UPDATE PM SET FS030 = FS031 - FS029;";
  command->CommandText = sql_stmt;
  command->Execute();


/*----FS042----*/
  sql_stmt = "DELETE FROM TMP1;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, NULL, COUNT(*) FROM " + BAM086;
  sql_stmt += " WHERE ACCOUNT_CODE in ('H','S') AND (ACCOUNT_CODE2 ='' or ";
  sql_stmt += "ACCOUNT_CODE2 IS NULL OR ACCOUNT_CODE2 = 'N') AND IDN = :idn GROUP BY IDN;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS042 = V1 FROM TMP1 AS A WHERE A.IDN = PM.IDN;";
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS064----*/
  sql_stmt = "DELETE FROM TMP1;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, NULL, SUM(CONVERT(FLOAT,LOAN_AMT) + CONVERT(FLOAT,PASS_DUE_AMT)) ";
  sql_stmt += "FROM " + BAM086 + " WHERE (ACCOUNT_CODE2 = '' or ACCOUNT_CODE2 IS NULL OR ACCOUNT_CODE2 = 'N') ";
  sql_stmt += "AND ACCOUNT_CODE IN ('W','C', 'D', 'E') AND IDN = :idn GROUP BY IDN;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS064 = V1 FROM TMP1 AS A WHERE A.IDN = PM.IDN;";
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS011----*/
  sql_stmt = "DELETE FROM TMP;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, MON_SINCE, SUM(CASE ";
  sql_stmt += "WHEN PAY_CODE IN ('A', 'B') THEN PAYMENT_AMT ELSE 0 END) ";
  sql_stmt += "FROM " + KRM023 + " WHERE IDN = :idn GROUP BY IDN, MON_SINCE;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 3 WHILE @i<=6 BEGIN INSERT INTO TMP ";
  sql_stmt += "SELECT IDN, @i, AVG(V1), NULL, NULL FROM TMP1 ";
  sql_stmt += "WHERE (:now - MON)<= @i GROUP BY IDN SET @i = @i + 3 END";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS011_3M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=3;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS011_6M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=6;";
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS023----*/
  sql_stmt = "DELETE FROM TMP;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, MON_SINCE, SUM(CONVERT(FLOAT,LIMIT))";
  sql_stmt += "FROM " + KRM023 + " WHERE IDN = :idn GROUP BY IDN, MON_SINCE;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

  sql_stmt = "INSERT INTO TMP SELECT IDN, 9, AVG(V1), NULL, NULL FROM TMP1 ";
  sql_stmt += "WHERE (:now - MON)<= 9 AND (V1 > 0) GROUP BY IDN;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS023_9M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=9;";
  command->CommandText = sql_stmt;
  command->Execute();

/*----FS105----*/
  sql_stmt = "DELETE FROM TMP;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, MON_SINCE, SUM(CASE WHEN CASH = 'Y' ";
  sql_stmt += "THEN 1 ELSE 0 END) FROM " + KRM023 + " WHERE IDN =:idn GROUP BY IDN, MON_SINCE;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

  sql_stmt = "INSERT INTO TMP SELECT IDN, 9, AVG(V1), NULL, NULL FROM TMP1 WHERE ";
  sql_stmt += "(:now - MON)<= 9 GROUP BY IDN;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS105_9M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=9;";
  command->CommandText = sql_stmt;
  command->Execute();

/*----FS106----*/
  sql_stmt = "DELETE FROM TMP;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, MON_SINCE, SUM(CASE WHEN PAY_CODE ";
  sql_stmt += "IN ('C', 'D', 'E', 'F') THEN 1 ELSE 0 END) FROM " + KRM023;
  sql_stmt += " WHERE IDN = :idn GROUP BY IDN, MON_SINCE;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 9 WHILE @i<=12 BEGIN INSERT INTO TMP ";
  sql_stmt += "SELECT IDN, @i, AVG(V1), NULL, NULL FROM TMP1 ";
  sql_stmt += "WHERE (:now - MON)<= @i AND IDN = :idn GROUP BY IDN SET @i = @i + 3 END;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS106_9M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=9;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS106_12M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=12;";
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS104----*/
  sql_stmt = "DELETE FROM TMP;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 3 WHILE @i<=9 BEGIN INSERT INTO TMP ";
  sql_stmt += "SELECT IDN, @i, SUM(PAYMENT_AMT), NULL, NULL FROM " + KRM023 + " WHERE ";
  sql_stmt += "(:now - MON_SINCE)<= @i AND PAY_CODE IN ('A', 'B', 'C') AND ";
  sql_stmt += "LIMIT <> 0 AND IDN = :idn GROUP BY IDN SET @i = @i + 3 END;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS104_3M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=3;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS104_6M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=6;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS104_9M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=9;";
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS105----*/
/*---Init temp table----*/
  sql_stmt = "DELETE FROM TMP;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, MON_SINCE, SUM(CASE WHEN PAY_CODE ";
  sql_stmt += "IN ('C', 'D') THEN PAYMENT_AMT ELSE 0 END) ";
  sql_stmt += "FROM " + KRM023 + " WHERE IDN = :idn GROUP BY IDN, MON_SINCE;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

  sql_stmt = "INSERT INTO TMP SELECT IDN, 9, AVG(V1), NULL, NULL FROM TMP1 ";
  sql_stmt += "WHERE (:now - MON)<= 9 GROUP BY IDN;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS105_9M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=9";
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS106----*/
  sql_stmt = "DELETE FROM TMP;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, MON_SINCE, SUM(CASE WHEN PAY_CODE ";
  sql_stmt += "IN ('C', 'D', 'E', 'F') THEN PAYMENT_AMT ELSE 0 END) ";
  sql_stmt += "FROM " + KRM023 + " WHERE IDN = :idn GROUP BY IDN, MON_SINCE;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

  sql_stmt = "INSERT INTO TMP SELECT IDN, 9, AVG(V1), NULL, NULL FROM TMP1 ";
  sql_stmt += "WHERE (:now - MON)<= 9 GROUP BY IDN;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS106_9M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=9;";
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS108----*/
  sql_stmt = "DELETE FROM TMP;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, MON_SINCE, SUM(CASE WHEN PAY_CODE ";
  sql_stmt += "IN ('A', 'B', 'C') THEN PAYMENT_AMT ELSE 0 END) ";
  sql_stmt += "FROM " + KRM023 + " WHERE IDN = :idn GROUP BY IDN, MON_SINCE;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

  sql_stmt = "INSERT INTO TMP SELECT IDN, 12, AVG(V1), NULL, NULL FROM TMP1 ";
  sql_stmt += "WHERE (:now - MON)<= 12 GROUP BY IDN;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS108_12M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND MON=12;";
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS118----*/
  sql_stmt = "DELETE FROM TMP1;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 3 WHILE @i<=9 BEGIN INSERT INTO TMP1 ";
  sql_stmt += "SELECT IDN, @i, MIN(PAYMENT_AMT/CONVERT(FLOAT,LIMIT)) ";
  sql_stmt += "FROM " + KRM023 + " WHERE (:now - MON_SINCE)<= @i AND PAY_CODE IN ";
  sql_stmt +="('C', 'D', 'E', 'F') AND LIMIT <> 0 AND IDN = :idn GROUP BY IDN ";
  sql_stmt +="SET @i = @i + 3 END;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS118_3M = V1 FROM TMP1 AS A WHERE A.IDN = PM.IDN AND MON=3;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS118_6M = V1 FROM TMP1 AS A WHERE A.IDN = PM.IDN AND MON=6;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS118_9M = V1 FROM TMP1 AS A WHERE A.IDN = PM.IDN AND MON=9;";
  command->CommandText = sql_stmt;
  command->Execute();

/*----MS119----*/
/*---Init temp table----*/
  sql_stmt = "DELETE FROM TMP1;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO TMP1 SELECT IDN, 3, MIN(PAYMENT_AMT/CONVERT(FLOAT,LIMIT)) ";
  sql_stmt += "FROM " + KRM023 + " WHERE (:now - MON_SINCE)<= 3 AND ";
  sql_stmt +="PAY_CODE IN ('A', 'B', 'C', 'D') AND LIMIT <> 0 AND IDN = :idn GROUP BY IDN;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

  sql_stmt = "UPDATE PM SET MS119_3M = V1 FROM TMP1 AS A WHERE A.IDN = PM.IDN AND MON=3;";
  command->CommandText = sql_stmt;
  command->Execute();

/*----FS212----*/
  sql_stmt = "DELETE FROM TMP;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM TMP1;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 1 WHILE @i <= 12 BEGIN INSERT INTO TMP1 ";
  sql_stmt += "SELECT IDN, @i, COUNT(*) FROM " + KRM023 + " AS A WHERE ";
  sql_stmt += "PAY_CODE IN ('C', 'D', 'E', 'F') AND PAYMENT_AMT > 1 AND ";
  sql_stmt += "(:now - MON_SINCE) = @i AND ISSUE IN ";
  sql_stmt += "(SELECT ISSUE FROM LATEST_LINE WHERE (MON_SINCE - MOB + 1) >= ";
  sql_stmt += "(:now1 - 6) AND IDN = A.IDN) AND IDN = :idn ";
  sql_stmt += "GROUP BY IDN SET @i = @i + 1 END;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["now1"] = now;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 3 WHILE @i<=6 BEGIN INSERT INTO TMP ";
  sql_stmt += "SELECT IDN, @i, AVG(V1), NULL, NULL FROM TMP1 ";
  sql_stmt += "WHERE (MON)<= @i GROUP BY IDN SET @i = @i + 3 END";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS212_3M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND A.MON = 3;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET FS212_6M = V1 FROM TMP AS A WHERE A.IDN = PM.IDN AND A.MON = 6;";
  command->CommandText = sql_stmt;
  command->Execute();

//generating interaction and t variables
  sql_stmt = "UPDATE PM SET MT011_43 = 2 * MS011_3M - MS011_6M, ";
  sql_stmt += "MT118_42 = MS118_3M - MS118_9M + MS118_6M, ";
  sql_stmt += "MT104_42 = MS104_3M - MS104_9M + MS104_6M, ";
  sql_stmt += "FT212_43 = 2 * FS212_3M - FS212_6M, ";
  sql_stmt += "INT084_9 = FS105_9M / (CASE WHEN FS106_9M = 0 THEN NULL ELSE FS106_9M END), ";
  sql_stmt += "INT062_9 = MS023_9M - MS105_9M, ";
  sql_stmt += "INT037_9 = MS106_9M / (CASE WHEN MS023_9M = 0 THEN NULL ELSE MS023_9M END) ";
  sql_stmt += "WHERE IDN = :idn;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

//generating squared, square_rooted, and zero indicator variables
  sql_stmt = "UPDATE PM SET MT104_42_Q = POWER(MT104_42, 2), ";
  sql_stmt += "MS119_3M_R = POWER((CASE WHEN MS119_3M<0 THEN NULL ELSE MS119_3M END), 0.5), ";
  sql_stmt += "MS108_12M_R = POWER(CONVERT(FLOAT,(CASE WHEN MS108_12M<0 THEN NULL ELSE MS108_12M END)), 0.5), ";
  sql_stmt += "MS064_R = POWER((CASE WHEN MS064<0 THEN NULL ELSE MS064 END), 0.5), ";
  sql_stmt += "INT084_9_R = POWER((CASE WHEN INT084_9<0 THEN NULL ELSE INT084_9 END), 0.5), ";
  sql_stmt += "INT037_9_Q = POWER(INT037_9, 2),";
  sql_stmt += "FT212_43_R = POWER((CASE WHEN FT212_43<0 THEN NULL ELSE FT212_43 END), 0.5), ";
  sql_stmt += "FS042_Q = POWER(FS042, 2) WHERE IDN = :idn;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

//variable transformation
  sql_stmt = "UPDATE PM SET ";
  sql_stmt += "MT118_42_T = (CASE WHEN MT118_42>1.375 THEN -0.93 ELSE MT118_42 END), ";
  sql_stmt += "MT104_42_Q_T = (CASE WHEN MT104_42_Q>350000 THEN 304792.43 ELSE MT104_42_Q END), ";
  sql_stmt += "MT011_43_Z = (CASE WHEN MT011_43 = 0 THEN 1 ELSE 0 END), ";
  sql_stmt += "MS119_3M_R_T = (CASE WHEN MS119_3M_R>0.52 THEN 0.45 ELSE MS119_3M_R END), ";
  sql_stmt += "MS108_12M_R_T = (CASE WHEN MS108_12M_R>7.5 THEN 10.35 ELSE MS108_12M_R END), ";
  sql_stmt += "MS064_R_T = (CASE WHEN MS064_R IS NULL THEN -7.60 WHEN MS064_R>15 THEN 5.44 ELSE MS064_R END), ";
  sql_stmt += "INT084_9_R_T = (CASE WHEN INT084_9_R IS NULL THEN -0.43 WHEN INT084_9_R>0.6 THEN 0.29 ELSE INT084_9_R END), ";
  sql_stmt += "INT062_9_T1 = (CASE WHEN INT062_9>480 THEN 484.44 ELSE INT062_9 END), ";
  sql_stmt += "INT037_9_Q_T1 = (CASE WHEN INT037_9_Q IS NULL THEN -0.1 WHEN INT037_9_Q>0.96 THEN 0.30 ELSE INT037_9_Q END), ";
  sql_stmt += "FT212_43_R_T = (CASE WHEN FT212_43_R IS NULL THEN 0.10 WHEN FT212_43_R>1 THEN -1.51 ELSE FT212_43_R END), ";
  sql_stmt += "FS106_12M_T = (CASE WHEN FS106_12M>1.25 THEN 0.85 ELSE FS106_12M END), ";
  sql_stmt += "FS042_Q_T = (CASE WHEN FS042_Q IS NULL THEN -0.35 WHEN (FS042_Q>2 AND FS042_Q<6) THEN 0.64 WHEN FS042_Q>16 THEN -2.55 ELSE FS042_Q END), ";
  sql_stmt += "FS030_T = (CASE WHEN FS030 IS NULL THEN 0.12 WHEN FS030>4 THEN -1.19 ELSE FS030 END) ";
  sql_stmt += "WHERE IDN = :idn;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

//score calculation
  sql_stmt = "UPDATE PM SET SCORE = 6983.56585+FS030_T*1573.47038+";
  sql_stmt +="FS042_Q_T*1185.2782+FS106_12M_T*2228.0058+FT212_43_R_T*4435.87825+";
  sql_stmt += "INT037_9_Q_T1*21844+INT062_9_T1*-19.98018+INT084_9_R_T*8933.21573+";
  sql_stmt +="MS064_R_T*489.60121+MS108_12M_R_T*504.85556+MS119_3M_R_T*11820+";
  sql_stmt +="MT011_43_Z*4135.46056+MT104_42_Q_T*0.0372+MT118_42_T*5906.03891 ";
  sql_stmt += "WHERE IDN = :idn;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

//decile cut
  sql_stmt = "UPDATE PM SET DECILE = (CASE WHEN SCORE <=-1766.96 THEN 1 ";
  sql_stmt += "WHEN SCORE <=1215.25 THEN 2 WHEN SCORE <=3306.38 THEN 3 ";
  sql_stmt += "WHEN SCORE <=5422.75 THEN 4 WHEN SCORE <=7761.67 THEN 5 ";
  sql_stmt += "WHEN SCORE <=10909.35 THEN 6 WHEN SCORE <=14857.95 THEN 7 ";
  sql_stmt += "WHEN SCORE <=21364.09 THEN 8 WHEN SCORE <=29957.94 THEN 9 ";
  sql_stmt += "WHEN SCORE >29957.94 THEN 10 ELSE 0 END) WHERE EXCLUSION = 0 ";
  sql_stmt += "AND IDN = :idn;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();

  sql_stmt = "SELECT SCORE FROM PM WHERE IDN = :idn;";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["idn"] = idn;
  query->Open();
  double score = query->FieldValues["SCORE"];
  query->Close();
  return score;
}

int propensity_cut(TADOQuery *query, const AnsiString &idno)
{
  AnsiString sql_stmt = "SELECT DECILE, EXCLUSION FROM PM WHERE IDN = :idn;";
  int decile;
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["idn"]= idno;
  query->Open();
  if(query->RecordCount == 0)
    decile = -1; //No data at all
  else if(query->FieldValues["EXCLUSION"] == 1)
    decile = -2; //No KRM023 data
  else if(query->FieldValues["EXCLUSION"] == 2)
    decile = -4; //KRM023 max bucket > 3
  else
    decile = query->FieldValues["DECILE"];
  query->Close();
  return decile;
}

void create_propensity_tables(TADOCommand *command)
{
  AnsiString sql_stmt;
/*
try{
  sql_stmt = "DROP TABLE PM;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
*/
  sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[PM]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "   drop table [PM]; "
             "CREATE TABLE PM (IDN CHAR(14), HIT INT, EXCLUSION INT, ";
             " FS029 INT, FS031 INT, FS042 INT, MS064 FLOAT, MS119_3M FLOAT, "
             " MS011_3M DECIMAL(13,5), MS011_6M DECIMAL(13,5), "
             " MS023_9M FLOAT, FS105_9M FLOAT, FS106_9M FLOAT, "
             " FS106_12M FLOAT, MS104_3M FLOAT, MS104_6M FLOAT, "
             " MS104_9M FLOAT, MS105_9M FLOAT, "
             " MS106_9M FLOAT, MS108_12M FLOAT, "
             " MS118_3M FLOAT, MS118_6M FLOAT, "
             " MS118_9M FLOAT, FS212_3M FLOAT, FS212_6M FLOAT, "
             " FS030 INT, FS042_Q FLOAT, FT212_43 FLOAT, FT212_43_R FLOAT, "
             " INT037_9 FLOAT, INT037_9_Q FLOAT, "
             " INT062_9 FLOAT, INT084_9 FLOAT, "
             " INT084_9_R FLOAT, MS064_R FLOAT, "
             " MS108_12M_R FLOAT, MS119_3M_R FLOAT, "
             " MT011_43 FLOAT, MT104_42 FLOAT, "
             " MT104_42_Q FLOAT, MT118_42 FLOAT, "
             " FS030_T FLOAT, FS042_Q_T FLOAT, "
             " FS106_12M_T FLOAT, FT212_43_R_T FLOAT, "
             " INT037_9_Q_T1 FLOAT, INT062_9_T1 FLOAT, "
             " INT084_9_R_T FLOAT, MS064_R_T FLOAT, "
             " MS108_12M_R_T FLOAT, MS119_3M_R_T FLOAT, "
             " MT011_43_Z INT, MT104_42_Q_T FLOAT, "
             " MT118_42_T FLOAT, SCORE FLOAT, DECILE INT)";
  command->CommandText = sql_stmt;
  command->Execute();
}

void load_and_initialize_propensity(TADOCommand *command, const AnsiString &idn)
{
  AnsiString sql_stmt;

//load data
  sql_stmt = "INSERT INTO PM(IDN, HIT) SELECT IDN, HIT FROM IDN_LIST WHERE IDN = :idn;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["idn"] = idn;
  command->Execute();
//KRM001 and KRM023 variables
  sql_stmt = "UPDATE PM SET FS212_3M=0, FS212_6M=0 WHERE (HIT & 0x3) = 0x3;";
  command->CommandText = sql_stmt;
  command->Execute();
//KRM023 variables
  sql_stmt = "UPDATE PM SET FS105_9M=0, FS106_9M=0, FS106_12M=0, MS011_3M=0,";
  sql_stmt += "MS011_6M=0, MS023_9M=0, MS104_3M=0, MS104_6M=0, MS104_9M=0, ";
  sql_stmt += "MS105_9M=0, MS106_9M=0, MS108_12M=0, MS118_3M=0, MS118_6M=0, ";
  sql_stmt += "MS118_9M=0, MS119_3M=0 WHERE (HIT & 0x2) = 0x2;";
  command->CommandText = sql_stmt;
  command->Execute();
//BAM009 variables
  sql_stmt = "UPDATE PM SET FS042=0, MS064=0 WHERE (HIT & 0x4) = 0x4;";
  command->CommandText = sql_stmt;
  command->Execute();
//STM007 variables
  sql_stmt = "UPDATE PM SET FS029=0, FS030=0, FS031=0 WHERE (HIT & 0x8) = 0x8;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE PM SET EXCLUSION = 0;";
  command->CommandText = sql_stmt;
  command->Execute();
}

void prepare_2xx_infra(TADOCommand *command, int now)
{
  AnsiString sql_stmt;
  sql_stmt = "DECLARE @i INT SET @i = 0 WHILE @i<=13 BEGIN INSERT INTO ";
  sql_stmt += "OPEN_CARD SELECT IDN, ";
  sql_stmt += "(CASE WHEN CARD_BRAND = 'A' AND ISSUE = 'A82' THEN 'AEA' ELSE ISSUE END), ";
  sql_stmt += "(:now - @i) FROM " + KRM001;
  sql_stmt += " WHERE (END_MON_SINCE > (:now1 - @i)) AND (START_MON_SINCE <= ";
  sql_stmt += "(:now2 - @i)) AND ISSUE != '021' SET @i = @i + 1 END;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["now1"] = now;
  command->Parameters->ParamValues["now2"] = now;
  command->Execute();

  sql_stmt = "INSERT INTO OPEN_LINE SELECT IDN, ISSUE, MON_SINCE, Count(*), NULL FROM ";
  sql_stmt += "OPEN_CARD GROUP BY IDN, ISSUE, MON_SINCE;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 0 WHILE @i<=13 BEGIN INSERT INTO ";
  sql_stmt += "OPEN_LINE SELECT IDN, (CASE WHEN CARD_BRAND = 'M' THEN 'CTM' ";
  sql_stmt += "WHEN CARD_BRAND = 'V' THEN 'CTV' ";
  sql_stmt +="WHEN CARD_BRAND = 'D' THEN 'CTD' END), (:now - @i),1, NULL ";
  sql_stmt += "FROM " + KRM001 + " WHERE (END_MON_SINCE > (:now1 - @i)) AND ";
  sql_stmt += "(START_MON_SINCE <= (:now2 - @i)) AND ISSUE = '021' ";
  sql_stmt +="SET @i = @i + 1 END;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Parameters->ParamValues["now1"] = now;
  command->Parameters->ParamValues["now2"] = now;
  command->Execute();

  sql_stmt = "UPDATE OPEN_LINE SET BUCKET = 1;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE OPEN_LINE SET BUCKET = 100 WHERE MON_SINCE = (:now - 13);";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "DECLARE @i INT SET @i = 13 WHILE @i > 0 BEGIN UPDATE OPEN_LINE ";
  sql_stmt += "SET BUCKET = A.BUCKET + 1 FROM OPEN_LINE AS A INNER JOIN ";
  sql_stmt += "OPEN_LINE ON A.IDN = OPEN_LINE.IDN AND ";
  sql_stmt +="A.ISSUE = OPEN_LINE.ISSUE AND A.MON_SINCE = (OPEN_LINE.MON_SINCE - 1) ";
  sql_stmt += "WHERE (:now - A.MON_SINCE) = @i SET @i = @i - 1 END;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "INSERT INTO LATEST_STMT_MON SELECT IDN, ISSUE, MAX(MON_SINCE) ";
  sql_stmt += "FROM OPEN_LINE WHERE MON_SINCE <= :now GROUP BY IDN, ISSUE;";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["now"] = now;
  command->Execute();

  sql_stmt = "INSERT INTO LATEST_LINE SELECT A.IDN, A.ISSUE, A.MON_SINCE, A.BUCKET ";
  sql_stmt += "FROM OPEN_LINE AS A INNER JOIN LATEST_STMT_MON AS B ";
  sql_stmt += "ON A.IDN = B.IDN AND A.ISSUE = B.ISSUE AND A.MON_SINCE = B.MON_SINCE;";
  command->CommandText = sql_stmt;
  command->Execute();
}

void capture_propensity_result(TADOCommand *command)
{
  AnsiString sql_stmt;
try{
  sql_stmt = "CREATE TABLE PM_RESULT(IDN INT, ";
  sql_stmt += "FS029 INT, FS031 INT, FS042 INT, MS064 FLOAT, MS119_3M FLOAT, ";
  sql_stmt += "MS011_3M FLOAT, MS011_6M FLOAT, ";
  sql_stmt += "MS023_9M FLOAT, FS105_9M FLOAT, FS106_9M FLOAT, ";
  sql_stmt += "FS106_12M FLOAT, MS104_3M FLOAT, MS104_6M FLOAT, ";
  sql_stmt += "MS104_9M FLOAT, MS105_9M FLOAT, ";
  sql_stmt += "MS106_9M FLOAT, MS108_12M INT, ";
  sql_stmt += "MS118_3M FLOAT, MS118_6M FLOAT, ";
  sql_stmt += "MS118_9M FLOAT, FS212_3M FLOAT, FS212_6M FLOAT, ";
  sql_stmt += "FS030 INT, FS042_Q FLOAT, FT212_43 FLOAT, FT212_43_R FLOAT, ";
  sql_stmt += "INT037_9 FLOAT, INT037_9_Q FLOAT, ";
  sql_stmt += "INT062_9 FLOAT, INT084_9 FLOAT, ";
  sql_stmt += "INT084_9_R FLOAT, MS064_R FLOAT, ";
  sql_stmt += "MS108_12M_R FLOAT, MS119_3M_R FLOAT, ";
  sql_stmt += "MT011_43 FLOAT, MT104_42 FLOAT, ";
  sql_stmt += "MT104_42_Q FLOAT, MT118_42 FLOAT, ";
  sql_stmt += "FS030_T FLOAT, FS042_Q_T FLOAT, ";
  sql_stmt += "FS106_12M_T FLOAT, FT212_43_R_T FLOAT, ";
  sql_stmt += "INT037_9_Q_T1 FLOAT, INT062_9_T1 FLOAT, ";
  sql_stmt += "INT084_9_R_T FLOAT, MS064_R_T FLOAT, ";
  sql_stmt += "MS108_12M_R_T FLOAT, MS119_3M_R_T FLOAT, ";
  sql_stmt += "MT011_43_Z INT, MT104_42_Q_T FLOAT, ";
  sql_stmt += "MT118_42_T FLOAT, SCORE FLOAT, DECILE INT)";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 2714)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0, 20) == "資料庫已經有一個名為");
}
  sql_stmt = "INSERT INTO PM_RESULT SELECT IDN, ";
  sql_stmt += "FS029, FS031, FS042, MS064, MS119_3M, ";
  sql_stmt += "MS011_3M, MS011_6M, ";
  sql_stmt += "MS023_9M, FS105_9M, FS106_9M, ";
  sql_stmt += "FS106_12M, MS104_3M, MS104_6M, ";
  sql_stmt += "MS104_9M, MS105_9M, ";
  sql_stmt += "MS106_9M, MS108_12M, ";
  sql_stmt += "MS118_3M, MS118_6M, ";
  sql_stmt += "MS118_9M, FS212_3M, FS212_6M, ";
  sql_stmt += "FS030, FS042_Q, FT212_43, FT212_43_R, ";
  sql_stmt += "INT037_9, INT037_9_Q, ";
  sql_stmt += "INT062_9, INT084_9, ";
  sql_stmt += "INT084_9_R, MS064_R, ";
  sql_stmt += "MS108_12M_R, MS119_3M_R, ";
  sql_stmt += "MT011_43, MT104_42, ";
  sql_stmt += "MT104_42_Q, MT118_42, ";
  sql_stmt += "FS030_T, FS042_Q_T, ";
  sql_stmt += "FS106_12M_T, FT212_43_R_T, ";
  sql_stmt += "INT037_9_Q_T1, INT062_9_T1, ";
  sql_stmt += "INT084_9_R_T, MS064_R_T, ";
  sql_stmt += "MS108_12M_R_T, MS119_3M_R_T, ";
  sql_stmt += "MT011_43_Z, MT104_42_Q_T, ";
  sql_stmt += "MT118_42_T, SCORE, DECILE FROM PM;";
  command->CommandText = sql_stmt;
  command->Execute();
}

//---------------------------------------------------------------------------
void write_premier_result(TADOCommand *command, AnsiString msn_no, AnsiString time_stamp_no,
                 double rscore, int code, AnsiString msg)
{
  AnsiString sql_stmt;
  AnsiString curr_dtime = CurrDateTime();

  sql_stmt = "INSERT INTO app_premier (msn, premier_date, rscore, premier_code, premier_msg) "
             " VALUES (:msn, :premier_date, :rscore, :premier_code, :premier_msg);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["msn"] = msn_no;
  command->Parameters->ParamValues["premier_date"] = curr_dtime;
  command->Parameters->ParamValues["rscore"] = rscore;
  command->Parameters->ParamValues["premier_code"] = code;
  command->Parameters->ParamValues["premier_msg"] = msg;
  command->Execute();
}
//---------------------------------------------------------------------------
void write_final_result(TADOCommand *command, AnsiString msn_no, double pb, int npv, 
                 int optimal_amount, int code, AnsiString msg)
{
  AnsiString sql_stmt;
  AnsiString curr_dtime = CurrDateTime();

  sql_stmt = "INSERT INTO app_final (msn, final_date, pb, npv, optimal_amount, final_code, final_msg) "
             " VALUES (:msn, :final_date, :pb, :npv, :optimal_amount, :final_code, :final_msg);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["msn"] = msn_no;
  command->Parameters->ParamValues["final_date"] = curr_dtime;
  command->Parameters->ParamValues["pb"] = pb;
  command->Parameters->ParamValues["npv"] = npv;
  command->Parameters->ParamValues["optimal_amount"] = optimal_amount;
  command->Parameters->ParamValues["final_code"] = code;
  command->Parameters->ParamValues["final_msg"] = msg;
  command->Execute();
}

//---------------------------------------------------------------------------
void write_final_result_fail(TADOCommand *command, AnsiString msn_no, 
                  int code, AnsiString msg)
{
  AnsiString sql_stmt;
  AnsiString curr_dtime = CurrDateTime();
  sql_stmt = "INSERT INTO app_final (msn, final_date, final_code, final_msg) "
             " VALUES (:msn, :final_date, :final_code, :final_msg);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["msn"] = msn_no;
  command->Parameters->ParamValues["final_date"] = curr_dtime;
  command->Parameters->ParamValues["final_code"] = code;
  command->Parameters->ParamValues["final_msg"] = msg;
  command->Execute();
}
//---------------------------------------------------------------------------
void store_input(TADOCommand *command, AnsiString idno, AnsiString msn_no, AnsiString time_stamp_no,
                 double gender, double principal, int test_cell,double gav,double nav,
                 AnsiString zip, double first_lien_value, int sc)
{
  AnsiString sql_stmt;
  sql_stmt = "INSERT INTO APP_INFO (IDN, MSN, TIME_STAMP, ";
  sql_stmt +="PRINCIPAL, TEST_CELL, GAV, NAV, ZIP, FIRST_LIEN_VALUE, GENDER, SC) VALUES ";
  sql_stmt +="(:idn, :msn, :time_stamp, :principal, :test_cell, ";
  sql_stmt +=":gav, :nav, :zip, :lien, :gender, :sc);";

  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["msn"] = msn_no;
  command->Parameters->ParamValues["time_stamp"] = time_stamp_no;
  command->Parameters->ParamValues["idn"] = idno;
  command->Parameters->ParamValues["principal"] = principal;
  command->Parameters->ParamValues["test_cell"] = test_cell;
  command->Parameters->ParamValues["gav"] = gav;
  command->Parameters->ParamValues["nav"] = nav;
  command->Parameters->ParamValues["zip"] = zip;
  command->Parameters->ParamValues["lien"] = first_lien_value;
  command->Parameters->ParamValues["gender"] = gender;
  command->Parameters->ParamValues["sc"] = sc;
  command->Execute();
}

double propensity_score(TADOQuery *query, AnsiString idn)
{
  AnsiString sql_stmt;
  sql_stmt = "SELECT SCORE FROM PM WHERE IDN = :idn;";
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["idn"]= idn;
  query->Open();
  double score = query->FieldValues["SCORE"];
  query->Close();
  return score;
}

void store_result_test(TADOCommand * command, AnsiString idno, AnsiString msn_no,
                  AnsiString time_stamp_no,
                  int test_cell, double secured_amount, double unsecured_amount,
                  double risk_score, int propensity_decile, double principal,
                  double lending_ratio, bool bsp_exclusion, double secured_npv,
                  double unsecured_npv, double secured_pb, double unsecured_pb,
                  int decline_code, int sc, double balance)
{
  AnsiString sql_stmt;
  sql_stmt = "INSERT INTO RESULT (IDN,MSN, TIME_STAMP, TEST_CELL, ";
  sql_stmt +="HIGHEST_1, HIGHEST_2, RISK_SCORE, SC, ";
  sql_stmt +="UNSECURED_BALANCE, FILTER_FLAG, PROPENSITY_DECILE, ";
  sql_stmt +="REVIEWED, SECURED_NPV, UNSECURED_NPV, SECURED_PB, UNSECURED_PB, DECLINE_REASON) VALUES ";
  sql_stmt +="(:idn, :msn, :time_stamp, :cell,  :secured, ";
  sql_stmt +=":unsecured, :risk_score, :sc, ";
  sql_stmt += ":balance, :filter, :pdecile, ";
  sql_stmt += ":manual, :secured_npv, :unsecured_npv, :secured_pb, :unsecured_pb, :decline_code);";
  int secured = (secured_amount * 1000 / 10000);
  secured *= 10000;
  int unsecured = (unsecured_amount *1000 / 10000);
  unsecured *= 10000;

  secured_npv = static_cast<int>(secured_npv + 0.5) * 1000;
  unsecured_npv = static_cast<int>(unsecured_npv + 0.5) * 1000;

  int review_flag = 0;

  double number = risk_score;
  if(propensity_decile != 0){
      number *= 100000000000000;
    if(number > 0)
      number = floor(number);
    else
      number = ceil(number);
    number *= 10;
    int pdecile_code = propensity_decile - 1;
    if(number > 0)
      number += pdecile_code;
    else
      number -= pdecile_code;
    number /=1000000000000000;
  }

  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["msn"] = msn_no;
  command->Parameters->ParamValues["time_stamp"] = time_stamp_no;
  command->Parameters->ParamValues["idn"] = idno;
  command->Parameters->ParamValues["secured"] = secured;
  command->Parameters->ParamValues["unsecured"] = unsecured;
  command->Parameters->ParamValues["risk_score"] = number;
  command->Parameters->ParamValues["cell"] = test_cell;
  command->Parameters->ParamValues["secured_npv"] = secured_npv;
  command->Parameters->ParamValues["unsecured_npv"] = unsecured_npv;
  command->Parameters->ParamValues["sc"] = sc;
  command->Parameters->ParamValues["balance"] = balance;
  command->Parameters->ParamValues["filter"] = decline_code;
  command->Parameters->ParamValues["pdecile"] = propensity_decile;
    double secured_diff = principal - secured;
    double unsecured_diff = principal - unsecured;
    if(!bsp_exclusion){
      if (principal >= 100000 && (risk_score<=0.04759)){
        if (
            (((unsecured_diff > 0) && (unsecured_diff < 100000)) || ((secured_diff > 0) && (secured_diff < 100000)))
            ||
            (((unsecured / principal)>0.9 &&(unsecured / principal)<1.0) || ((secured / principal)>0.9&&(secured / principal)<1.0))
            )
          review_flag = -1;
        else if ((secured_diff > 0) &&
                 ((lending_ratio > 0) && (lending_ratio < 1.2) ) &&
                 ((risk_score<=0.04759) && (unsecured < 500000)))
          review_flag = -2;
      }//end of requested
    } //end of bsp_exclusion (NAV = GAV = 0)
    else
      review_flag = -3;

  if (((review_flag == 0) || (lending_ratio == -1)) && (review_flag != -3))
    review_flag = 1;
  if(decline_code == 5)
    review_flag = -4;
/*  else if(decline_code == 107){
    if(secured == 0)
      decline_code = 3;
    else
      decline_code = 0;
    review_flag = 1;
  }
  else if((decline_code == 101) || (decline_code == 102)){
    decline_code = 5;
    review_flag = -4;
  }*/

  command->Parameters->ParamValues["manual"] = review_flag;
  command->Parameters->ParamValues["secured_pb"] = secured_pb;
  command->Parameters->ParamValues["unsecured_pb"] = unsecured_pb;
  command->Parameters->ParamValues["decline_code"] = decline_code;
  command->Execute();

  sql_stmt = "INSERT INTO RESULT_R (IDN, MSN, TIME_STAMP, TEST_CELL, ";
  sql_stmt +="HIGHEST_1, HIGHEST_2, RISK_SCORE, SC, ";
  sql_stmt +="REVIEWED, SECURED_NPV, UNSECURED_NPV, SECURED_PB, UNSECURED_PB, DECLINE_REASON) VALUES ";
  sql_stmt +="(:idn, :msn, :time_stamp, :cell,  :secured, ";
  sql_stmt +=":unsecured, :risk_score, :sc, ";
  sql_stmt += ":manual, :secured_npv, :unsecured_npv, :secured_pb, :unsecured_pb, :decline_code);";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["msn"] = msn_no;
  command->Parameters->ParamValues["time_stamp"] = time_stamp_no;
  command->Parameters->ParamValues["idn"] = idno;
  command->Parameters->ParamValues["secured"] = secured;
  command->Parameters->ParamValues["unsecured"] = unsecured;
  command->Parameters->ParamValues["risk_score"] = number;
  command->Parameters->ParamValues["cell"] = test_cell;
  command->Parameters->ParamValues["secured_npv"] = secured_npv;
  command->Parameters->ParamValues["unsecured_npv"] = unsecured_npv;
  command->Parameters->ParamValues["manual"] = review_flag;
  command->Parameters->ParamValues["secured_pb"] = secured_pb;
  command->Parameters->ParamValues["unsecured_pb"] = unsecured_pb;
  command->Parameters->ParamValues["decline_code"] = decline_code;
  command->Parameters->ParamValues["sc"] = sc;
  command->Execute();
}

void store_result(TADOCommand * command, AnsiString idno, AnsiString msn_no, AnsiString time_stamp_no,
                  int test_cell, double secured_amount, double unsecured_amount,
                  double risk_score, int propensity_decile, double principal,
                  double lending_ratio, bool bsp_exclusion, double secured_npv,
                  double unsecured_npv, double secured_pb, double unsecured_pb,
                  int decline_code, int sc, double balance)
{
  AnsiString sql_stmt;
  sql_stmt = "INSERT INTO RESULT (IDN, MSN, TIME_STAMP, TEST_CELL, ";
  sql_stmt +="HIGHEST_1, HIGHEST_2, RISK_SCORE, SC, ";
//  sql_stmt +="UNSECURED_BALANCE, FILTER_FLAG, PROPENSITY_DECILE, ";
  sql_stmt +="REVIEWED, SECURED_NPV, UNSECURED_NPV, SECURED_PB, UNSECURED_PB, DECLINE_REASON) VALUES ";
  sql_stmt +="(:idn, :msn, :time_stamp, :cell,  :secured, ";
  sql_stmt +=":unsecured, :risk_score, :sc, ";
//  sql_stmt += ":balance, :filter, :pdecile, ";
  sql_stmt += ":manual, :secured_npv, :unsecured_npv, :secured_pb, :unsecured_pb, :decline_code);";
  int secured = (secured_amount * 1000 / 10000);
  secured *= 10000;
  int unsecured = (unsecured_amount *1000 / 10000);
  unsecured *= 10000;

  int secured_npv_amount = static_cast<int>(secured_npv + 0.5) * 1000;
  int unsecured_npv_amount = static_cast<int>(unsecured_npv + 0.5) * 1000;

  int review_flag = 0;

  double number = risk_score;
  if(propensity_decile != 0){
      number *= 100000000000000;
    if(number > 0)
      number = floor(number);
    else
      number = ceil(number);
    number *= 10;
    int pdecile_code = propensity_decile - 1;
    if(number > 0)
      number += pdecile_code;
    else
      number -= pdecile_code;
    number /=1000000000000000;
  }

  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["msn"] = msn_no;
  command->Parameters->ParamValues["time_stamp"] = time_stamp_no;
  command->Parameters->ParamValues["idn"] = idno;
  command->Parameters->ParamValues["secured"] = secured;
  command->Parameters->ParamValues["unsecured"] = unsecured;
  command->Parameters->ParamValues["risk_score"] = number;
  command->Parameters->ParamValues["cell"] = test_cell;
  command->Parameters->ParamValues["secured_npv"] = secured_npv_amount;
  command->Parameters->ParamValues["unsecured_npv"] = unsecured_npv_amount;
  command->Parameters->ParamValues["sc"] = sc;
//  command->Parameters->ParamValues["balance"] = balance;
//  command->Parameters->ParamValues["filter"] = decline_code;
//  command->Parameters->ParamValues["pdecile"] = propensity_decile;
    double secured_diff = principal - secured;
    double unsecured_diff = principal - unsecured;
    if(!bsp_exclusion){
      if (principal >= 100000 && (risk_score<=0.04759)){
        if (
            (((unsecured_diff > 0) && (unsecured_diff < 100000)) || ((secured_diff > 0) && (secured_diff < 100000)))
            ||
            (((unsecured / principal)>0.9 &&(unsecured / principal)<1.0) || ((secured / principal)>0.9&&(secured / principal)<1.0))
            )
          review_flag = -1;
        else if ((secured_diff > 0) &&
                 ((lending_ratio > 0) && (lending_ratio < 1.2) ) &&
                 ((risk_score<=0.04759) && (unsecured < 500000)))
          review_flag = -2;
      }//end of requested
    } //end of bsp_exclusion (NAV = GAV = 0)
    else
      review_flag = -3;

  if (((review_flag == 0) || (lending_ratio == -1)) && (review_flag != -3))
    review_flag = 1;
  if(decline_code == 5)
    review_flag = -4;
  else if(decline_code == 107){
    if(secured == 0)
      decline_code = 3;
    else
      decline_code = 0;
    review_flag = 1;
  }
  else if((decline_code == 101) || (decline_code == 102)){
    decline_code = 5;
    review_flag = -4;
  }

  command->Parameters->ParamValues["manual"] = review_flag;
  command->Parameters->ParamValues["secured_pb"] = secured_pb;
  command->Parameters->ParamValues["unsecured_pb"] = unsecured_pb;
  command->Parameters->ParamValues["decline_code"] = decline_code;
  command->Execute();

  sql_stmt = "INSERT INTO RESULT_R (IDN, MSN, TIME_STAMP, TEST_CELL, ";
  sql_stmt +="HIGHEST_1, HIGHEST_2, RISK_SCORE, SC, ";
  sql_stmt +="REVIEWED, SECURED_NPV, UNSECURED_NPV, SECURED_PB, UNSECURED_PB, DECLINE_REASON) VALUES ";
  sql_stmt +="(:idn, :idn1, :idn2, :msn, :time_stamp, :cell,  :secured, ";
  sql_stmt +=":unsecured, :risk_score, :sc, ";
  sql_stmt += ":manual, :secured_npv, :unsecured_npv, :secured_pb, :unsecured_pb, :decline_code);";
  command->CommandText = sql_stmt;
  command->Parameters->ParamValues["msn"] = msn_no;
  command->Parameters->ParamValues["time_stamp"] = time_stamp_no;
  command->Parameters->ParamValues["idn"] = idno;
  command->Parameters->ParamValues["secured"] = secured;
  command->Parameters->ParamValues["unsecured"] = unsecured;
  command->Parameters->ParamValues["risk_score"] = number;
  command->Parameters->ParamValues["cell"] = test_cell;
  command->Parameters->ParamValues["secured_npv"] = secured_npv;
  command->Parameters->ParamValues["unsecured_npv"] = unsecured_npv;
  command->Parameters->ParamValues["manual"] = review_flag;
  command->Parameters->ParamValues["secured_pb"] = secured_pb;
  command->Parameters->ParamValues["unsecured_pb"] = unsecured_pb;
  command->Parameters->ParamValues["decline_code"] = decline_code;
  command->Parameters->ParamValues["sc"] = sc;
  command->Execute();
}


void clean_up(TADOCommand *command)
{
try{
  AnsiString sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[" + BAM086 + "]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE " + BAM086 + ";";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  AnsiString sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[" + KRM023 + "]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "   DROP TABLE " + KRM023 + ";"
             " if exists (select * from dbo.sysobjects where id = object_id(N'[" + KRM037 + "]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "   DROP TABLE " + KRM037 + ";";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  AnsiString sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[" + KRM001 + "]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE " + KRM001 + ";";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  AnsiString sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[" + JAS002 + "]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE " + JAS002 + ";";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  AnsiString sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[" + JAS002_T + "]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE " + JAS002_T + ";";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  AnsiString sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[" + STM007 + "]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE " + STM007 + ";";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  AnsiString sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[TMP]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE TMP;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  AnsiString sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[TMP1]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE TMP1;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  AnsiString sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[HU41]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE HU41;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  AnsiString sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[LU41]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE LU41;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  AnsiString sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[PM]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE PM;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  AnsiString sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[IDN_LIST]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE IDN_LIST;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  AnsiString sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[IDN_LIST_TMP]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE IDN_LIST_TMP;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  AnsiString sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[OPEN_CARD]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE OPEN_CARD;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  AnsiString sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[OPEN_LINE]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE OPEN_LINE;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  AnsiString sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[LATEST_STMT_MON]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE LATEST_STMT_MON;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  AnsiString sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[LATEST_LINE]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE LATEST_LINE;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  AnsiString sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[OPEN_ISSUER]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE OPEN_ISSUER;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}

try{
  AnsiString sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[PDACO_V1_00_CAL]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE PDACO_V1_00_CAL;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}

  AnsiString sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[BAM086_BUCKET]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "   drop table [BAM086_BUCKET]; "
             " if exists (select * from dbo.sysobjects where id = object_id(N'[FS_ISSUE]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "   drop table [FS_ISSUE]; "
             " if exists (select * from dbo.sysobjects where id = object_id(N'[KRM023_RANGE_TMP]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "   drop table [KRM023_RANGE_TMP]; "
             " if exists (select * from dbo.sysobjects where id = object_id(N'[KRM037_CONVERT]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "   drop table [KRM037_CONVERT]; ";
  command->CommandText = sql_stmt;
  command->Execute();

}

/*void get_idn_list(TADOCommand *command, AnsiString KRM001, AnsiString KRM023,
                  AnsiString BAM086, AnsiString STM007, AnsiString DAM103,
                  AnsiString DAM203)*/
void get_idn_list(TADOCommand *command, AnsiString KRM001, AnsiString KRM023,
                  AnsiString BAM086, AnsiString STM007, AnsiString JAS002_T)
{
  AnsiString sql_stmt;
/*
try{
  sql_stmt = "DROP TABLE IDN_LIST_TMP;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
try{
  sql_stmt = "DROP TABLE IDN_LIST;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
*/
try{
  sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[IDN_LIST_TMP]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE IDN_LIST_TMP;"
             "CREATE TABLE IDN_LIST_TMP (IDN CHAR(14), HIT INT);";
             //KRM001 = 2, KRM023 = 1, BAM086 = 4, STM007 = 16, DAM103 or JAS002 = 8, DMAM203 = 32
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 2714)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0, 20) == "資料庫已經有一個名為");
}

try{
  sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[IDN_LIST]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE IDN_LIST;"
             "CREATE TABLE IDN_LIST (IDN CHAR(14), HIT INT);";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 2714)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0, 20) == "資料庫已經有一個名為");
}

  sql_stmt = "DELETE FROM IDN_LIST_TMP;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "DELETE FROM IDN_LIST;";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO IDN_LIST_TMP SELECT DISTINCT IDN, 1 FROM " + KRM023 + " ";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO IDN_LIST_TMP SELECT DISTINCT IDN, 2 FROM " + KRM001 + " ";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO IDN_LIST_TMP SELECT DISTINCT IDN, 4 FROM " + BAM086 + " ";
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO IDN_LIST_TMP SELECT DISTINCT IDN, 16 FROM " + STM007 + " ";
  command->CommandText = sql_stmt;
  command->Execute();

//  sql_stmt = "INSERT INTO IDN_LIST_TMP SELECT DISTINCT IDN, 16 FROM " + DAM103 + " ";
  sql_stmt = "INSERT INTO IDN_LIST_TMP SELECT DISTINCT IDN, 8 FROM " + JAS002_T + " ";
  command->CommandText = sql_stmt;
  command->Execute();

/*  sql_stmt = "INSERT INTO IDN_LIST_TMP SELECT DISTINCT IDN, 32 FROM " + DAM203 + " ";
  command->CommandText = sql_stmt;
  command->Execute();*/

  sql_stmt = "INSERT INTO IDN_LIST SELECT IDN, SUM(HIT) FROM IDN_LIST_TMP GROUP BY IDN;";
  command->CommandText = sql_stmt;
  command->Execute();

try{
  sql_stmt = "CREATE INDEX IDX ON IDN_LIST(IDN);";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 1913)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,6) == "資料表");
}

try{
  sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[IDN_LIST_TMP]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "DROP TABLE IDN_LIST_TMP;";
  command->CommandText = sql_stmt;
  command->Execute();
}
catch(Exception &E){
  if (AnsiString(E.ClassName()) == "EOleException")
    if(command->Connection->Errors->Item[0]->NativeError == 3701)
      command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
}
}
/*
bool error_checking(int gender, int test_cell, double gav, double nav, double lien_value,
                    double principal, bool il, AnsiString msn, AnsiString *error_message)
{
  AnsiString errors="";
  bool error = false;

  if(gav<nav){
    errors+="鑑價值(G值)必須大於鑑價值(N值) ";
    error=true;
  }
  if((test_cell < 1) || ((test_cell > 46) && ((test_cell < 70) || (test_cell > 76)))){
    errors+="產品種類無法辨識 ";
    error=true;
  }
  if(il){
    if(principal < 100000){
      errors+="申請金額必須大於新台幣十萬元 ";
      error = true;
    }
  }
  if(lien_value<0){
    errors+="設定值不得為負數 ";
    error=true;
  }
  if((gender < 0) || (gender > 1)){
    errors+="申請人性別代碼錯誤 ";
    error = true;
  }
  if(error){
    (*error_message) += errors;
    (*error_message) += " 案號:" + msn;
  }
  return error;
}
*/
double krm023_balance(TADOQuery *query, AnsiString idn, int now)
{
  AnsiString sql_stmt="";
/*
  try{
    sql_stmt = "DROP TABLE KRM023_RANGE_TMP";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->ExecSQL();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(query->Connection->Errors->Item[0]->NativeError == 3701)
        query->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }
*/
  sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[KRM023_RANGE_TMP]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             " DROP TABLE KRM023_RANGE_TMP;"
             " CREATE TABLE KRM023_RANGE_TMP "
             "(IDN CHAR(14), ISSUE CHAR(3), MON INT)";
//  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->ExecSQL();

  sql_stmt = "INSERT INTO KRM023_RANGE_TMP(IDN, ISSUE, MON) ";
  sql_stmt += "SELECT IDN, ISSUE, MAX(MON_SINCE) FROM " + KRM023 + " ";
  sql_stmt += "WHERE :now - MON_SINCE <= 2 ";
  sql_stmt += "GROUP BY IDN, ISSUE";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["now"] = now;
  query->ExecSQL();

  sql_stmt = "SELECT SUM(CASE WHEN PAY_CODE IN ('C', 'D', 'E', 'F') THEN PAYMENT_AMT ELSE 0 END) AS BAL, ";
  sql_stmt += "SUM(CONVERT(FLOAT, LIMIT)) AS LINE ";
  sql_stmt += "FROM " + KRM023 + " AS A ";
  sql_stmt += "INNER JOIN KRM023_RANGE_TMP AS B ON A.IDN = B.IDN AND ";
  sql_stmt += "A.ISSUE = B.ISSUE AND A.MON_SINCE = B.MON ";
  sql_stmt += "WHERE PAY_CODE IN ('C', 'D', 'E', 'F') AND ";
  sql_stmt += " A.IDN = :idn";
  sql_stmt = sql_stmt.UpperCase();
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["idn"] = idn;
//  query->Parameters->ParamValues["now"] = now;
  query->Open();
  double balance = 0;
  if(!query->FieldValues["BAL"].IsNull())
    balance = query->FieldValues["BAL"];

/*  sql_stmt = "SELECT ISNULL(MAX(A.AMOUNT),0) AS AMOUNT FROM (";
  sql_stmt += "SELECT MON_SINCE, ISNULL(SUM(PAYMENT_AMT),0) AS AMOUNT FROM " + KRM023;
  sql_stmt +=" WHERE IDN = :idn AND MON_SINCE >= :now AND PAY_CODE IN ";
  sql_stmt += "('C', 'D', 'E', 'F') ";
  sql_stmt += "GROUP BY MON_SINCE) AS A";
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["idn"]= idn;
  query->Parameters->ParamValues["now"]= now - 1;
  query->Open();
  double balance = 0;
  if(!query->FieldByName("AMOUNT")->IsNull)
    balance = query->FieldValues["AMOUNT"];*/
  query->Close();
  return balance;
}

double bam009_balance(TADOQuery *query, AnsiString idn)
{
  AnsiString sql_stmt;
  sql_stmt = "SELECT SUM(CAST(ISNULL(LOAN_AMT,0) AS INT) + CAST(ISNULL(PASS_DUE_AMT,0) AS INT)) AS AMOUNT FROM " + BAM086;
  sql_stmt +=" WHERE IDN = :idn AND (ACCOUNT_CODE2 = '' OR ACCOUNT_CODE2 IS NULL OR ACCOUNT_CODE2 = 'N');";
  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql_stmt);
  query->Parameters->ParamValues["idn"]= idn;
  query->Open();
  double balance = 0;
  if(!query->FieldByName("AMOUNT")->IsNull)
    balance = query->FieldValues["AMOUNT"];
  query->Close();
  return balance;
}


void prepare_JAS002(TADOCommand *command, const AnsiString &ori_table, const AnsiString &dest_table)
{
	AnsiString sql_stmt;

  sql_stmt = "UPDATE " + ori_table + " SET ";
  sql_stmt += "MSN = (CASE WHEN MSN = '' THEN NULL ELSE MSN END), ";
  sql_stmt += "INQUIRY_DATE = (CASE WHEN INQUIRY_DATE = '' THEN NULL ELSE INQUIRY_DATE END), ";
  sql_stmt += "IDN = (CASE WHEN IDN = '' THEN NULL ELSE IDN END), ";
  sql_stmt += "EVER_DELINQUENT = (CASE WHEN EVER_DELINQUENT = '' THEN NULL ELSE EVER_DELINQUENT END), ";
  sql_stmt += "DELINQUENT_DATE = (CASE WHEN DELINQUENT_DATE = '' THEN NULL ELSE DELINQUENT_DATE END), ";
  sql_stmt += "EVER_BAD_CHECK = (CASE WHEN EVER_BAD_CHECK = '' THEN NULL ELSE EVER_BAD_CHECK END), ";
  sql_stmt += "BAD_CHECK_DATE = (CASE WHEN BAD_CHECK_DATE = '' THEN NULL ELSE BAD_CHECK_DATE END), ";
  sql_stmt += "EVER_REJECT = (CASE WHEN EVER_REJECT = '' THEN NULL ELSE EVER_REJECT END), ";
  sql_stmt += "REJECT_DATE = (CASE WHEN REJECT_DATE = '' THEN NULL ELSE REJECT_DATE END), ";
  sql_stmt += "EVER_STOP_CARD = (CASE WHEN EVER_STOP_CARD = '' THEN NULL ELSE EVER_STOP_CARD END), ";
  sql_stmt += "STOP_CARD_DATE = (CASE WHEN STOP_CARD_DATE = '' THEN NULL ELSE STOP_CARD_DATE END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

/*
  sql_stmt = "DROP TABLE " + dest_table + ";";
  sql_stmt = sql_stmt.UpperCase();
  try{
    command->CommandText = sql_stmt;
    command->Execute();
  }
  catch(Exception &E){
    if (AnsiString(E.ClassName()) == "EOleException")
      if(command->Connection->Errors->Item[0]->NativeError == 3701)
        command->Connection->Errors->Clear();
//      if(E.Message.SubString(0,16) == "無法 卸除 資料表");
  }
*/
  sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[" + dest_table + "]')"
             "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
             "   drop table [" + dest_table + "]; "
             "CREATE TABLE " + dest_table + " "
             " (IDN Char(14), TYPE CHAR(1), E_DATE Char(7), "
             "E_MON_SINCE INT);";
//  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + dest_table;
  sql_stmt += " SELECT IDN, 'D', DELINQUENT_DATE, NULL FROM " + ori_table;
  sql_stmt += " WHERE EVER_DELINQUENT = 'Y';";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + dest_table;
  sql_stmt += " SELECT IDN, 'B', BAD_CHECK_DATE, NULL FROM " + ori_table;
  sql_stmt += " WHERE EVER_BAD_CHECK = 'Y';";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + dest_table;
  sql_stmt += " SELECT IDN, 'R', REJECT_DATE, NULL FROM " + ori_table;
  sql_stmt += " WHERE EVER_REJECT = 'Y';";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "INSERT INTO " + dest_table;
  sql_stmt += " SELECT IDN, 'S', STOP_CARD_DATE, NULL FROM " + ori_table;
  sql_stmt += " WHERE EVER_STOP_CARD = 'Y';";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + dest_table;
  sql_stmt += " SET E_DATE = (CASE WHEN (LEN(E_DATE) = 6 OR E_DATE <> '*' OR E_DATE IS NOT NULL) ";
  sql_stmt += "THEN '0' + E_DATE ELSE E_DATE END);";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();

  sql_stmt = "UPDATE " + dest_table;
  sql_stmt += " SET E_MON_SINCE = CONVERT(INT, ISNULL(LEFT(E_DATE,3),0)) * 12 + ";
  sql_stmt += "CONVERT(INT, ISNULL(RIGHT(LEFT(E_DATE,5),2),0));";
  sql_stmt = sql_stmt.UpperCase();
  command->CommandText = sql_stmt;
  command->Execute();
}

//-----------------------------------------------------------
unsigned int in_pdaco_1_00(TADOQuery *query,
                const AnsiString &krm023, const AnsiString &krm001,
                const AnsiString &jas002, const AnsiString &bam085, const AnsiString &stm007,
                const AnsiString &msn, const AnsiString &idno,
                const AnsiString &time_stamp,
                unsigned char flag, int monthly_income, int now)
{
//  flag:
//  1: (0x01) krm023 hit
//  2: (0x02) krm001 hit	
//  3: (0x04) bam086 hit	
//  4: (0x08) jas002 hit	
//  5: (0x10) stm007 hit	
//
//  exclusion flag:
//  1: (0x01) k23_hit = 0
//  2: (0x02) k01_hit = 0
//  3: (0x04) max_bucket_ef > 3, 102- 信用卡一年內曾逾期超過90天 
//  4: (0x08) IND001 = 1
//  5: (0x10) FS044 > 0 || FS334 > 3, 103- 貸款一年內曾逾期超過90天
//  6: (0x20) jas002_defect > 0, 101- 重大信用瑕疵
//  7: (0x40) FS302 > 0          104- 現金卡最近一個月遲繳
//  8: (0x80) cash_card_utilization >= 95%  105- 信用卡動支比例達95%以上
//  9: (0x100) balance of credit and cash card >= 500k,  106- 現金卡與信用卡餘額超過50萬元
// 10: (0x200) fs031 > 5            120- JCIC資料不足且查詢過多
// 11: (0x400) bucket_ef_1k > 0     121- JCIC資料不足且有信用卡逾繳紀錄
// 12: (0x800) k01 with stop_code=3 122,124- JCIC資料不足且有強制停卡紀錄
// 13: (0x1000) fs031 > 3           123- JCIC資料不足且查詢過多
// 14: (0x2000) fs334b_1m > 0       108- 任一貸款最近一個月遲繳
// 15: (0x4000) ms606 > 1M          109- 無擔保貸款總額超過100萬元
// 16: (0x8000) MonIncome*22-ms606 < 0 110- 負債比率過高
// 17: (0x10000) note_flag > 0      111- 聯徵特殊註記
// 18: (0x20000) CREDIT_BLOCK_LIST > 0  112- 京城銀行黑名單


  AnsiString sql_stmt;
  double credit_card_rev_balance_opt = 0;
  double cash_card_balance = 0;
  double credit_card_balance = 0;
  double unsecured_balance = 0;
  int exclusion = 0;
  int count = 0;

  if ((flag & 0x1) == 0) //k23 not hit
    exclusion |= 0x1;
  else {  // k23_hit
// max bucket_ef_1k > 3
    sql_stmt = "SELECT MAX(Bucket_ef_1k) As ef FROM " + krm023 + " ";
    sql_stmt +="WHERE IDN = :idn;";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idn"] = idno;
    query->Open();
    int max_bucket = query->FieldValues["ef"];
    query->Close();
    if (max_bucket > 3)
      exclusion |= 0x4;
    if (max_bucket > 0)
      exclusion |= 0x400;
      
    sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[KRM023_RANGE_TMP]')"
               "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
               "   drop table [KRM023_RANGE_TMP]; "
               "CREATE TABLE KRM023_RANGE_TMP "
               " (IDN CHAR(11), ISSUE CHAR(3), MON INT)";
//    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->ExecSQL();

    sql_stmt = "INSERT INTO KRM023_RANGE_TMP(IDN, ISSUE, MON) ";
    sql_stmt += "SELECT IDN, ISSUE, MAX(MON_SINCE) FROM " + krm023 + " ";
    sql_stmt += "WHERE :now - MON_SINCE <= 3 ";
    sql_stmt += "GROUP BY IDN, ISSUE";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["now"] = now;
    query->ExecSQL();

    sql_stmt = "SELECT SUM(CASE WHEN PAY_CODE IN ('C', 'D', 'E', 'F') THEN ISNULL(PAYMENT_AMT,0)+ISNULL(SPREAD_PAYMENT,0) "
               "                WHEN CASH = 'Y' AND ISNULL(SPREAD_PAYMENT,0)>0 THEN ISNULL(SPREAD_PAYMENT,0) "
               "                ELSE 0 END) AS BAL, ";
    sql_stmt += "FROM " + krm023 + " AS A ";
    sql_stmt += "INNER JOIN KRM023_RANGE_TMP AS B ON A.IDN = B.IDN AND ";
    sql_stmt += "A.ISSUE = B.ISSUE AND A.MON_SINCE = B.MON AND A.IDN = :idn";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idn"] = idno;
    query->Open();
    if(!query->FieldValues["BAL"].IsNull()){
      credit_card_rev_balance_opt = query->FieldValues["BAL"];
      credit_card_rev_balance_opt *= 1000;
    }

    sql_stmt = "SELECT SUM((CASE WHEN PAY_CODE IN ('C', 'D', 'E', 'F') THEN ISNULL(PAYMENT_AMT,0) ELSE 0 END) "
               "            + ISNULL(SPREAD_PAYMENT,0)) AS BAL ";
    sql_stmt += "FROM " + krm023 + " AS A ";
    sql_stmt += "INNER JOIN KRM023_RANGE_TMP AS B ON A.IDN = B.IDN AND ";
    sql_stmt += "A.ISSUE = B.ISSUE AND A.MON_SINCE = B.MON AND A.IDN = :idn";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idn"] = idno;
    query->Open();
    if(!query->FieldValues["BAL"].IsNull()){
      credit_card_balance = query->FieldValues["BAL"];
      credit_card_balance *= 1000;
    }

// IND001 =1
    sql_stmt = "SELECT MIN(MON_SINCE) AS MON FROM " + krm023 + " ";
    sql_stmt += "WHERE IDN = :idn";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idn"] = idno;
    query->Open();
    int early_month = now - query->FieldValues["MON"];
    query->Close();

    sql_stmt = "SELECT MAX(MON_SINCE) AS MON FROM " + krm023 + " ";
    sql_stmt += "WHERE IDN = :idn";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idn"] = idno;
    query->Open();
    int latest_month = now - query->FieldValues["MON"];
    query->Close();
// 20060316 by Eric:the earliest stmt should >=6 and the latest stmt should < 6
    if (early_month < 6 || latest_month >= 6)
       exclusion |= 0x8;
  }

  if ((flag & 0x2) == 0)  // no k01 hit
    exclusion |= 0x2;
  else { // k01 hit
    try{
      sql_stmt = "CREATE TABLE IND_TMP (IDN Char(14), Mon int);";
      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->ExecSQL();
    }
    catch(Exception &E){
      if (static_cast<AnsiString>(E.ClassName()) == "EOleException")
        if(query->Connection->Errors->Item[0]->NativeError == 2714)
          query->Connection->Errors->Clear();
//    if(E.Message.SubString(0, 20) == "資料庫已經有一個名為");
    }
    sql_stmt = "DELETE FROM IND_TMP;";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->ExecSQL();

    // for account is open and primary card holder
    sql_stmt = "INSERT INTO IND_TMP SELECT IDN, MIN(Start_Mon_Since) FROM ";
    sql_stmt += krm001 + " WHERE IDN = :idn and m_s='Y' and end_mon_since > :now GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idn"] = idno;
    query->Parameters->ParamValues["now"] = now;
    query->ExecSQL();

    sql_stmt = "SELECT Mon FROM IND_TMP;";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Open();

    int mons_before = now - query->FieldByName("Mon")->AsInteger;
    query->Close();
    if ((mons_before >= 1) && (mons_before < 7))
      exclusion |= 0x8;

    try{
      sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[IND_TMP]')"
               "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
               " DROP TABLE IND_TMP;";
//      sql_stmt = sql_stmt.UpperCase();
      query->Close();
      query->SQL->Clear();
      query->SQL->Add(sql_stmt);
      query->ExecSQL();
    }
    catch( Exception &E){
      if (AnsiString(E.ClassName()) == "EOleException")
        if(query->Connection->Errors->Item[0]->NativeError == 3701)
          query->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
    }
// with stop_code = 3
    sql_stmt = "SELECT IDN, COUNT(*) AS CNT FROM ";
    sql_stmt += krm001 + " WHERE IDN = :idn and STOP_CODE = '3' GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idn"] = idno;
    query->Open();
    int count;
    if (query->FieldByName("CNT")->IsNull)
       count = 0;
    else
       count = query->FieldValues["CNT"];
    query->Close();
    if (count > 0)
       exclusion |= 0x800;

  } // end of k01 hit

  if (flag & 0x4) { // bam085 hit
/*----FS044----*/
     sql_stmt ="SELECT IDN, SUM(CASE WHEN PASS_DUE_AMT > 0 THEN 1 ELSE 0 END) AS CNT ";
     sql_stmt +="FROM " + bam085 + " ";
     sql_stmt +="WHERE IDN = :idn GROUP BY IDN;";
     sql_stmt = sql_stmt.UpperCase();
     query->Close();
     query->SQL->Clear();
     query->SQL->Add(sql_stmt);
     query->Parameters->ParamValues["idn"] = idno;
     query->Open();
     count = query->FieldValues["CNT"];
     query->Close();
     if (count > 0)
        exclusion |= 0x10;

// FS334 > 3 曾經最高逾期月數
     sql_stmt = " select IDN, max(bucket) AS CNT "
                "    from bam086_bucket "
                " WHERE IDN = :idn GROUP BY IDN;";
     sql_stmt = sql_stmt.UpperCase();
     query->Close();
     query->SQL->Clear();
     query->SQL->Add(sql_stmt);
     query->Parameters->ParamValues["idn"] = idno;
     query->Open();
     count = query->FieldValues["CNT"];
     query->Close();
     if (count > 3)
        exclusion |= 0x10;
// FS302
     sql_stmt = "SELECT IDN, SUM(CASE WHEN ACCOUNT_CODE = 'Y' AND ";
     sql_stmt+="                           ISNULL(LEFT(PAY_CODE_12,1), '0') NOT IN ('0', 'X') ";
     sql_stmt+="                      THEN 1 ELSE 0 END) AS CNT ";
     sql_stmt+="FROM " + bam085 + " ";
     sql_stmt+="WHERE IDN = :idn GROUP BY IDN;";
     sql_stmt = sql_stmt.UpperCase();
     query->Close();
     query->SQL->Clear();
     query->SQL->Add(sql_stmt);
     query->Parameters->ParamValues["idn"] = idno;
     query->Open();
     count = query->FieldValues["CNT"];
     query->Close();
     if (count > 0)
        exclusion |= 0x40;

// FS334_1B 
     sql_stmt = "SELECT IDN, SUM(CASE WHEN ISNULL(LEFT(PAY_CODE_12,1), '0') NOT IN ('0', 'X') ";
     sql_stmt+= "                     THEN 1 ELSE 0 END) AS CNT ";
     sql_stmt+= "FROM " + bam085 + " ";
     sql_stmt+= "WHERE IDN = :idn GROUP BY IDN;";
     sql_stmt = sql_stmt.UpperCase();
     query->Close();
     query->SQL->Clear();
     query->SQL->Add(sql_stmt);
     query->Parameters->ParamValues["idn"] = idno;
     query->Open();
     count = query->FieldValues["CNT"];
     query->Close();
     if (count > 0)
        exclusion |= 0x2000;

// Cash card utilization >= 95%
     sql_stmt = "SELECT IDN, SUM(CASE WHEN ACCOUNT_CODE = 'Y' AND ";
     sql_stmt+="        CAST(ISNULL(CONTRACT_AMT,0) AS FLOAT) * 0.95 <= (CAST(ISNULL(LOAN_AMT,0) AS FLOAT) + CAST(ISNULL(PASS_DUE_AMT,0) AS FLOAT)) AND ";
     sql_stmt+="        ISNULL(CONTRACT_AMT,0) > 0 ";
     sql_stmt+="                      THEN 1 ELSE 0 END) AS CNT ";
     sql_stmt+="FROM " + bam085 + " ";
     sql_stmt+="WHERE IDN = :idn GROUP BY IDN;";
     sql_stmt = sql_stmt.UpperCase();
     query->Close();
     query->SQL->Clear();
     query->SQL->Add(sql_stmt);
     query->Parameters->ParamValues["idn"] = idno;
     query->Open();
     count = query->FieldValues["CNT"];
     query->Close();
     if (count > 0)
        exclusion |= 0x80;

    sql_stmt = "SELECT SUM(CAST(ISNULL(LOAN_AMT, 0) AS INT) + CAST(ISNULL(PASS_DUE_AMT, 0) AS INT)) ";
    sql_stmt += "AS BAL FROM " + bam085 + " ";
    sql_stmt += "WHERE IDN = :idno AND ACCOUNT_CODE = 'Y';";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idno"] = idno;
    query->Open();
    if (!query->FieldValues["BAL"].IsNull()){
      cash_card_balance = query->FieldValues["BAL"];
      cash_card_balance *= 1000;
    }

    sql_stmt = "SELECT SUM(CAST(ISNULL(LOAN_AMT, 0) AS INT) + CAST(ISNULL(PASS_DUE_AMT, 0) AS INT)) ";
    sql_stmt += "AS BAL FROM " + bam085 + " ";
    sql_stmt += "WHERE IDN = :idn AND ((ACCOUNT_CODE2 = '') OR (ACCOUNT_CODE2 IS NULL) OR (ACCOUNT_CODE = 'N'));";
    sql_stmt = sql_stmt.UpperCase();
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idn"] = idno;
    query->Open();
    if (!query->FieldValues["BAL"].IsNull()){
      unsecured_balance = query->FieldValues["BAL"];
      unsecured_balance *= 1000;
    }

    if ((credit_card_rev_balance_opt + cash_card_balance) >= 500000)
       exclusion |= 0x100;
    if ((credit_card_balance + unsecured_balance) >= 1000000)
       exclusion |= 0x4000;
    if ((monthly_income * 22 - credit_card_balance - unsecured_balance) < 0)
       exclusion |= 0x8000;

  }

// jas002_defect
    if(flag & 0x8){ // jas002 hit
		sql_stmt ="SELECT IDN, SUM(CASE WHEN :now - E_MON_SINCE <= 36 THEN 1 ELSE 0 END) AS CNT ";
		sql_stmt +="FROM " + jas002 + " ";
		sql_stmt +="WHERE IDN = :idn GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
	  query->Parameters->ParamValues["now"] = now;
	  query->Parameters->ParamValues["idn"] = idno;
	  query->Open();
	  int count;
    if(query->FieldByName("CNT")->IsNull)
      count = 0;
    else
      count = query->FieldValues["CNT"];
	  query->Close();
	  if(count > 0)
      exclusion |= 0x20;
    }  // end of jas002 hit

// fs031
    if (flag & 0x10){ // stm007 hit
	sql_stmt ="SELECT IDN, COUNT(*) AS CNT ";
  	sql_stmt +="FROM " + stm007 + " ";
	sql_stmt +="WHERE Item_List IS NOT NULL AND  ";
	sql_stmt +="Item_List <> '' AND  ";
  	sql_stmt +="(:now - QUERY_MON_SINCE) <= 12 ";
	sql_stmt +="GROUP BY IDN ";
        sql_stmt = sql_stmt.UpperCase();
  	query->Close();
  	query->SQL->Clear();
  	query->SQL->Add(sql_stmt);
	query->Parameters->ParamValues["now"] = now;
	query->Open();
	int count;
	if (query->FieldByName("CNT")->IsNull)
          count = 0;
        else
          count = query->FieldValues["CNT"];
	query->Close();
	if (count > 5)
           exclusion |= 0x200;
	else if (count > 3)
           exclusion |= 0x1000;
    }  // end of stm007 hit


 try {
    sql_stmt = "SELECT COUNT(*) AS CNT FROM VAM102 WHERE IDN = :idn AND NOTE IS NOT NULL";
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idn"] = idno;
    query->Open();
    if (query->FieldValues["CNT"].IsNull())
       count = 0;
    else
       count = query->FieldValues["CNT"];
    query->Close();
    if (count > 0)
       exclusion |= 0x10000;

    sql_stmt = "SELECT COUNT(*) AS CNT FROM CREDIT_BLOCK_LIST WHERE IDN = :idn ";
    query->Close();
    query->SQL->Clear();
    query->SQL->Add(sql_stmt);
    query->Parameters->ParamValues["idn"] = idno;
    query->Open();
    if (query->FieldValues["CNT"].IsNull())
       count = 0;
    else
       count = query->FieldValues["CNT"];
    query->Close();
    if (count > 0)
       exclusion |= 0x20000;
       
 } catch (Exception &E) {
   throw;
 }
  return exclusion;
}

unsigned char get_data_availability(TADOQuery *query,const AnsiString &idno)
{
	AnsiString sql_stmt;
	sql_stmt = "SELECT HIT FROM IDN_LIST WHERE IDN = :idn;";
	sql_stmt = sql_stmt.UpperCase();
	query->Close();
	query->SQL->Clear();
	query->SQL->Add(sql_stmt);
	query->Parameters->ParamValues["idn"] = idno;
	query->Open();
  unsigned char flag;
  if(query->FieldByName("HIT")->IsNull)
    flag = 0;
  else
	  flag = query->FieldValues["HIT"];
	query->Close();
	return flag;
}

int get_product_type(int test_cell)
{
  int product_type;
  int upper = MAX_CELL - 1;
  int lower = 0;
  int index = (upper + lower) / 2;
  while ((product[index][7] != test_cell) && (lower <= upper)){
    if(product[index][7] > test_cell)
      upper = index - 1;
    else
      lower = index + 1;
    index = (upper + lower) / 2;
  }
  if(lower > upper)
    product_type = -1;
  else
    product_type = product[index][6];
  return product_type;
}

double pdaco_1_00(TADOCommand *command, TADOQuery *query, const AnsiString &case_no,
                const AnsiString &idn, const AnsiString &time_stamp, double gender,
                unsigned char data_flag, const AnsiString &krm023,
                const AnsiString &krm001, const AnsiString &bam085,
                const AnsiString &jas002, const AnsiString &stm007,
                int now, bool with_gender)
{
	AnsiString sql_stmt;
	double risk_score  = 1000;

	try{
		sql_stmt = "CREATE TABLE PDACO_V1_00_CAL (CASE_NO Char(20), IDN Char(14), ";
		sql_stmt += "TIME_STAMP Char(12), FS014_9M int, FS101_9M int, ";
    sql_stmt += "FS016_3M int, FS101_3M int, FS005_3M_1k int, ";
		sql_stmt += "FS059_3M_1K int, FS059_6M_1K int, FS059_9M_1K int, SEX float, ";
		sql_stmt += "FS212_3M_1K float, FS212_6M_1K float, FS031 int, MS117_6M float, ";
		sql_stmt += "FS102_3M int, FS102_6M int, FS102_9M int, FS051 int, ";
		sql_stmt += "APP_LAST_MONTH_BUCKET int, FS205_3M_1K int, ";
		sql_stmt += "INT015_3M float, INT028_9M float, ";
    sql_stmt += "FT059_1K_42 int, FT212_43_1K float, FT102_42 int, ";
		sql_stmt += "FT059_1K_43 int, ";
		sql_stmt += "FT059_1K_42_Q int, FT212_43_1K_Q float, FT102_42_R float, ";
		sql_stmt += "FS205_3M_1K_Q int, FT059_1K_43_R float, ";
		sql_stmt += "INT015_3M_T float, INT028_9M_T float, ";
    sql_stmt += "FT059_1k_42_Q_T int, FT212_43_1K_Q_T float, ";
		sql_stmt += "FT102_42_R_T float, FS031_T int, SEX_T float, APP_LAST_MONTH_BUCKET_T int, ";
		sql_stmt += "FS205_3M_1K_Q_T int, FT059_1K_43_R_T float, ";
		sql_stmt += "FS051_T int, ";
		sql_stmt += "EXCLUSION int, AVAIL int, SCORE float, CUT int, PB1 float, PB2 float);";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
		command->Execute();
  }
  catch(Exception &E){
  if (static_cast<AnsiString>(E.ClassName()) == "EOleException")
    if(query->Connection->Errors->Item[0]->NativeError == 2714)
      query->Connection->Errors->Clear();
//    if(E.Message.SubString(0, 20) == "資料庫已經有一個名為");
  }

	sql_stmt = "INSERT INTO PDACO_V1_00_CAL(CASE_NO, IDN, TIME_STAMP, SEX, AVAIL, EXCLUSION) ";
	sql_stmt += "VALUES(:case_no, :idn, :time_stamp, :sex, :avail, 0);";
  sql_stmt = sql_stmt.UpperCase();
	command->CommandText = sql_stmt;
	command->Parameters->ParamValues["case_no"] = case_no;
	command->Parameters->ParamValues["idn"] = idn;
	command->Parameters->ParamValues["sex"] = gender;
	command->Parameters->ParamValues["time_stamp"] = time_stamp;
	command->Parameters->ParamValues["avail"] = data_flag;
	command->Execute();

	int exclusion = 0;//in_pdaco_1_00(query, krm023, krm001, jas002, bam085, data_flag, now); since this function is used as the module exclusion and is called before
	if(exclusion==0){
//Init variables
		if(data_flag & 0x1){
			sql_stmt = "UPDATE PDACO_V1_00_CAL SET ";
			sql_stmt += "FS016_3M = 0, FS101_3M = 0, FS005_3M_1k = 0, FS014_9M = 0, ";
			sql_stmt += "FS059_3M_1K = 0, FS059_6M_1K = 0, FS059_9M_1K = 0, ";
			sql_stmt += "FS102_3M = 0, FS102_6M = 0, FS102_9M = 0, MS117_6M = 0, FS101_9M = 0 ";
      sql_stmt += "WHERE IDN = :idn;";
      sql_stmt = sql_stmt.UpperCase();
			command->CommandText = sql_stmt;
      command->Parameters->ParamValues["idn"] = idn;
			command->Execute();
		}
		if((data_flag & 0x1) && (data_flag & 0x2)){
			sql_stmt = "UPDATE PDACO_V1_00_CAL SET ";
			sql_stmt += "FS212_3M_1K = 0, FS212_6M_1K = 0, ";
			sql_stmt += "FS205_3M_1K = 0 ";
      sql_stmt += "FROM " + krm001 + " AS A, " + krm023 + " As B ";
      sql_stmt += "WHERE PDACO_V1_00_CAL.IDN = :idn AND ";
      sql_stmt += "PDACO_V1_00_CAL.IDN = A.IDN AND ";
      sql_stmt += "A.IDN = B.IDN AND ";
      sql_stmt += "(CASE WHEN A.ISSUE = '021' AND CARD_BRAND = 'V' THEN 'CTV' ";
      sql_stmt += "WHEN A.ISSUE = '021' AND CARD_BRAND = 'M' THEN 'CTM' ";
      sql_stmt += "WHEN A.ISSUE = '021' AND CARD_BRAND = 'D' THEN 'CTD' ";
      sql_stmt += "WHEN A.ISSUE = 'A82' AND CARD_BRAND = 'A' THEN 'AEA' ";
      sql_stmt += "ELSE A.ISSUE END) = B.ISSUE;";
      sql_stmt = sql_stmt.UpperCase();
			command->CommandText = sql_stmt;
      command->Parameters->ParamValues["idn"] = idn;
			command->Execute();
		}
		if(data_flag & 0x4){
			sql_stmt = "UPDATE PDACO_V1_00_CAL SET ";
			sql_stmt += "FS051 = 0 ";
      sql_stmt += "WHERE IDN = :idn;";
      sql_stmt = sql_stmt.UpperCase();
			command->CommandText = sql_stmt;
      command->Parameters->ParamValues["idn"] = idn;
			command->Execute();
		}
		if(data_flag & 0x8){
			sql_stmt = "UPDATE PDACO_V1_00_CAL SET ";
			sql_stmt += "FS031 = 0 ";
      sql_stmt += "WHERE IDN = :idn;";
      sql_stmt = sql_stmt.UpperCase();
			command->CommandText = sql_stmt;
      command->Parameters->ParamValues["idn"] = idn;
			command->Execute();
		}
//		prepare_2xx_variables(command, query, krm023, krm001, now);


//create temp tables
		try{
  		sql_stmt = "DROP TABLE TMP;";
      sql_stmt = sql_stmt.UpperCase();
  		command->CommandText = sql_stmt;
  		command->Execute();
		}
		catch( Exception &E){
  		if (AnsiString(E.ClassName()) == "EOleException")
    		if(command->Connection->Errors->Item[0]->NativeError == 3701)
      		command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
		}
		try{
  		sql_stmt = "DROP TABLE TMP1;";
      sql_stmt = sql_stmt.UpperCase();
  		command->CommandText = sql_stmt;
  		command->Execute();
		}
		catch( Exception &E){
  		if (AnsiString(E.ClassName()) == "EOleException")
    		if(command->Connection->Errors->Item[0]->NativeError == 3701)
      		command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
		}
		sql_stmt = "CREATE TABLE TMP ";
	  sql_stmt += "(IDN Char(14), Mon INT, V1 FLOAT, V2 FLOAT, V3 FLOAT);";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

	  sql_stmt = "CREATE TABLE TMP1 ";
	  sql_stmt += "(IDN Char(14), Mon INT, V1 FLOAT);";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

/*----FS014----*/
/*---Init temp table----*/
		sql_stmt ="DELETE FROM TMP;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
    sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
/*---Start making FS014---*/
    sql_stmt ="INSERT INTO TMP ";
    sql_stmt +="SELECT IDN, 9, COUNT(DISTINCT ISSUE), NULL, NULL ";
		sql_stmt +=("FROM " + krm023 + " ");
    sql_stmt +="WHERE (:now - Mon_Since)<= 9 AND ";
//    sql_stmt +="(:now1 - Mon_Since) > 0 AND ";
    sql_stmt +="Pay_Code IN ('A', 'B') GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
		command->Execute();

    sql_stmt ="UPDATE PDACO_V1_00_CAL ";
    sql_stmt +="SET FS014_9M = V1 FROM TMP AS A ";
    sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=9;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

/*----FS016----*/
/*---Init temp table----*/
		sql_stmt ="DELETE FROM TMP;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
    sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
/*---Start making FS016---*/
		sql_stmt ="INSERT INTO TMP ";
		sql_stmt +="SELECT IDN, 3, COUNT(DISTINCT ISSUE), NULL, NULL ";
		sql_stmt +=("FROM " + krm023 + " ");
		sql_stmt +="WHERE (:now - Mon_Since)<= 3 AND ";
//		sql_stmt +="(:now1 - Mon_Since) > 0 AND ";
		sql_stmt +="CASH = 'Y' GROUP BY IDN ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
		command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS016_3M = V1 FROM TMP AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=3;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

/*----FS101----*/
/*---Init temp table----*/
    sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
/*---Start making FS101---*/
		sql_stmt ="DECLARE @i INT ";
		sql_stmt +="SET @i = 3 ";
		sql_stmt +="WHILE @i<=9 ";
		sql_stmt +="BEGIN ";
		sql_stmt +="INSERT INTO TMP1 ";
		sql_stmt +="SELECT IDN, @i, COUNT(DISTINCT ISSUE) ";
		sql_stmt +=("FROM " + krm023 + " ");
		sql_stmt +="WHERE (:now - Mon_Since)<= @i ";
//		sql_stmt +="AND (:now1 - Mon_Since) > 0 ";
		sql_stmt +="GROUP BY IDN SET @i = @i + 6 END;";
                sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
	  command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS101_3M = V1 FROM TMP1 AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=3;";
                sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS101_9M = V1 FROM TMP1 AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=9;";
                sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

/*----FS059----*/
/*---Init temp table----*/
    sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
/*---Start making FS059---*/
		sql_stmt ="DECLARE @i INT ";
		sql_stmt +="SET @i = 3 ";
		sql_stmt +="WHILE @i<=9 ";
		sql_stmt +="BEGIN ";
		sql_stmt +="INSERT INTO TMP1 ";
		sql_stmt +="SELECT IDN, @i, COUNT(DISTINCT ISSUE) ";
		sql_stmt +="FROM " + krm023 + " ";
		sql_stmt +="WHERE Bucket_ef_1K >=1 AND ";
		sql_stmt +="(:now - Mon_Since)<= @i ";
//		sql_stmt +="AND (:now1 - Mon_Since) > 0 ";
		sql_stmt +="GROUP BY IDN SET @i = @i + 3 END;";
                sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
	  command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS059_3M_1k = V1 FROM TMP1 AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=3;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS059_6M_1k = V1 FROM TMP1 AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=6;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS059_9M_1k = V1 FROM TMP1 AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=9;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

/*----FS212_1K----*/
/*---Init temp table----*/
		sql_stmt ="DELETE FROM TMP;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
    sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
/*---Start making FS212_1K---*/
		sql_stmt ="DECLARE @I INT ";
		sql_stmt +="SET @I = 1 ";
		sql_stmt +="WHILE @I <= 12 ";
		sql_stmt +="BEGIN ";
		sql_stmt +="INSERT INTO TMP ";
		sql_stmt +="SELECT IDN, @I, COUNT(*), NULL, NULL ";
		sql_stmt +="FROM " + krm023 + "  AS A ";
		sql_stmt +="WHERE Issue IN ";
		sql_stmt +="(SELECT Issue FROM Latest_Line ";
		sql_stmt +="WHERE  (MON_SINCE - MOB + 1) >=  (:now - 6) AND IDN = A.IDN) AND ";
		sql_stmt +="Pay_Code in ('C', 'D', 'E','F') AND ";
		sql_stmt +="Payment_Amt > 1 AND ";
		sql_stmt +="(:now1 - Mon_Since) = @I  ";
		sql_stmt +="GROUP BY IDN ";
		sql_stmt +="SET @I = @I + 1 ";
		sql_stmt +="END ";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
	  command->Parameters->ParamValues["now1"] = now;
	  command->Execute();

		sql_stmt ="DECLARE @I INT ";
		sql_stmt +="SET @I = 3 ";
		sql_stmt +="WHILE @I <= 6 ";
		sql_stmt +="BEGIN ";
		sql_stmt +="INSERT INTO TMP1 ";
		sql_stmt +="SELECT IDN, @I, AVG(V1) ";
		sql_stmt +="FROM TMP ";
		sql_stmt +="WHERE Mon <= @I ";
		sql_stmt +="GROUP BY IDN ";
		sql_stmt +="SET @I = @I + 3 ";
		sql_stmt +="END ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET  ";
		sql_stmt +="FS212_3M_1k = V1 ";
		sql_stmt +="FROM TMP1 AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND A.Mon = 3;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET  ";
		sql_stmt +="FS212_6M_1k = V1 ";
		sql_stmt +="FROM TMP1 AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND A.Mon = 6;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Execute();

/*----FS102----*/
/*---Init temp table----*/
		sql_stmt ="DELETE FROM TMP;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
    sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

		try{
  		sql_stmt = " if exists (select * from dbo.sysobjects where id = object_id(N'[FS_ISSUE]')"
                           "          and OBJECTPROPERTY(id, N'IsUserTable') = 1) "
                           "   drop table [FS_ISSUE]; ";
//                sql_stmt = sql_stmt.UpperCase();
  		command->CommandText = sql_stmt;
  		command->Execute();
		}
		catch( Exception &E){
  		if (AnsiString(E.ClassName()) == "EOleException")
    		if(command->Connection->Errors->Item[0]->NativeError == 3701)
      		command->Connection->Errors->Clear();
//    if(E.Message.SubString(0,16) == "無法 卸除 資料表");
		}

		sql_stmt = "CREATE TABLE FS_ISSUE(IDN Char(14), MON Int, ISSUE Char(3));";
                sql_stmt = sql_stmt.UpperCase();
	        command->CommandText = sql_stmt;
                command->Execute();

/*---Start making FS102---*/
		sql_stmt ="INSERT INTO FS_Issue ";
		sql_stmt +="SELECT IDN, Mon_Since, Issue  ";
		sql_stmt +="FROM " + krm023 + "  ";
		sql_stmt +="WHERE Limit > 100 ";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

		sql_stmt ="DECLARE @i INT ";
		sql_stmt +="SET @i = 3 ";
		sql_stmt +="WHILE @i<=9 ";
		sql_stmt +="BEGIN ";
		sql_stmt +="INSERT INTO TMP ";
		sql_stmt +="SELECT DISTINCT IDN, @i, 0, NULL, NULL ";
		sql_stmt +=("FROM " + krm023 + "  ");
		sql_stmt +="WHERE (:now - Mon_Since)<= @i ";
//		sql_stmt +="AND (:now1 - Mon_Since) > 0 ";
		sql_stmt +="GROUP BY IDN SET @i = @i + 3 END;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
 		command->Execute();

		sql_stmt ="DECLARE @i INT ";
		sql_stmt +="SET @i = 3 ";
		sql_stmt +="WHILE @i<=9 ";
		sql_stmt +="BEGIN ";
		sql_stmt +="INSERT INTO TMP1  ";
		sql_stmt +="SELECT IDN, @i, COUNT(DISTINCT ISSUE) ";
		sql_stmt +="FROM FS_Issue ";
		sql_stmt +="WHERE (:now - Mon)<= @i ";
//		sql_stmt +="AND (:now1 - Mon) > 0 ";
		sql_stmt +="GROUP BY IDN SET @i = @i + 3 END;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
 		command->Execute();

		sql_stmt ="UPDATE TMP ";
		sql_stmt +="SET V1 = A.V1 ";
		sql_stmt +="FROM TMP1 AS A ";
		sql_stmt +="WHERE A.IDN = TMP.IDN AND TMP.MON = A.MON;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS102_3M = V1 FROM TMP AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=3;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET FS102_6M = V1 FROM TMP AS A ";
    sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=6;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
 		command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET ";
		sql_stmt +="FS102_9M = V1 ";
		sql_stmt +="FROM TMP AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=9;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    if(data_flag & 0x10){
/*----FS031----*/
/*---Init temp table----*/
      sql_stmt ="DELETE FROM TMP1;";
      sql_stmt = sql_stmt.UpperCase();
	    command->CommandText = sql_stmt;
	    command->Execute();
/*---Start making FS031---*/
		  sql_stmt ="INSERT INTO TMP1 ";
		  sql_stmt +="SELECT IDN, NULL, COUNT(*) ";
  		sql_stmt +="FROM " + stm007 + " ";
	  	sql_stmt +="WHERE Item_List IS NOT NULL AND  ";
		  sql_stmt +="Item_List <> '' AND  ";
  		sql_stmt +="(:now - QUERY_MON_SINCE) <= 12 ";
	  	sql_stmt +="GROUP BY IDN ";
      sql_stmt = sql_stmt.UpperCase();
		  command->CommandText = sql_stmt;
  	  command->Parameters->ParamValues["now"] = now;
 	  	command->Execute();

  		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
	  	sql_stmt +="SET FS031 = V1 ";
		  sql_stmt +="FROM TMP1 AS A ";
  		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN ";
      sql_stmt = sql_stmt.UpperCase();
	  	command->CommandText = sql_stmt;
 		  command->Execute();
    };
/*----App_Last_Month_Bucket----*/
    sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

/*    sql_stmt ="DELETE FROM TMP;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();*/

/*    sql_stmt = "INSERT INTO TMP(IDN, MON) SELECT IDN, MAX(MON_SINCE) FROM ";
    sql_stmt += krm023 + " GROUP BY IDN";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    sql_stmt = "INSERT INTO TMP1(IDN, V1) SELECT IDN, MAX(BUCKET_EF_1K) ";
    sql_stmt += "FROM " + krm023 + " AS A INNER JOIN TMP AS B ";
    sql_stmt += "ON A.IDN = B.IDN AND A.MON_SINCE = B.MON GROUP BY A.IDN";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute(); */

    sql_stmt = "INSERT INTO TMP1 SELECT IDN, NULL, MAX(BUCKET_EF_1K) ";
    sql_stmt += "FROM " + krm023 + " ";
    sql_stmt += "WHERE :now - Mon_Since <= 1 GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
 		command->Execute();


		sql_stmt = "UPDATE PDACO_V1_00_CAL SET APP_LAST_MONTH_BUCKET = A.V1 ";
		sql_stmt += "FROM TMP1 AS A WHERE PDACO_V1_00_CAL.IDN = A.IDN;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

/*----FS205_1K----*/
    sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

		sql_stmt +="INSERT INTO TMP1 ";
		sql_stmt +="SELECT IDN, 3, COUNT(*) ";
		sql_stmt +="FROM " + krm023 + "  AS A ";
		sql_stmt +="WHERE Issue IN ";
		sql_stmt +="(SELECT Issue FROM Latest_Line ";
		sql_stmt +="WHERE MOB <= 12 AND IDN = A.IDN) AND ";
		sql_stmt +="Pay_Code in ('C', 'D', 'E','F') AND ";
		sql_stmt +="Payment_Amt > 1 AND ";
		sql_stmt +="(:now - Mon_Since) <= 3 ";
//		sql_stmt +="(:now1 - Mon_Since) > 0 ";
		sql_stmt +="GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
 		command->Execute();

		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt +="SET  ";
		sql_stmt +="FS205_3M_1k = V1 ";
		sql_stmt +="FROM TMP1 AS A ";
		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND A.Mon = 3;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
 		command->Execute();

    if(data_flag & 0x4){
/*----FS051----*/
/*---Init temp table----*/
  		sql_stmt ="DELETE FROM TMP1;";
      sql_stmt = sql_stmt.UpperCase();
	    command->CommandText = sql_stmt;
	    command->Execute();
/*---Start making FS051---*/
  		sql_stmt ="INSERT INTO TMP1 ";
	  	sql_stmt +="SELECT IDN, NULL, COUNT(*) ";
		  sql_stmt +="FROM " + bam085 + " ";
  		sql_stmt +="WHERE purpose_code = '4' ";
	  	sql_stmt +="GROUP BY IDN;";
      sql_stmt = sql_stmt.UpperCase();
	    command->CommandText = sql_stmt;
  	  command->Execute();

  		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
	  	sql_stmt +="SET FS051 = V1 ";
		sql_stmt +="FROM TMP1 AS A ";
  		sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN;";
                sql_stmt = sql_stmt.UpperCase();
	    command->CommandText = sql_stmt;
	    command->Execute();
    }
/*----MS117----*/
/*---Init temp table----*/
		sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
/*---Start making MS117---*/
    sql_stmt ="INSERT INTO TMP1 ";
    sql_stmt+="SELECT IDN, 6, MIN(Payment_amt/CONVERT(decimal(13,5),Limit)) ";
    sql_stmt+=("FROM " + krm023 + " ");
    sql_stmt+="WHERE (:now - Mon_Since)<= 6 AND ";
    sql_stmt+="      (:now1 - Mon_Since) > 0 AND ";
    sql_stmt+="       Pay_Code IN ('C', 'D') AND ";
    sql_stmt+="       Limit <> 0 GROUP BY IDN;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
	  command->Parameters->ParamValues["now1"] = now;
 		command->Execute();

    sql_stmt ="UPDATE PDACO_V1_00_CAL ";
    sql_stmt+="SET ";
    sql_stmt+="MS117_6M = V1 FROM TMP1 AS A ";
    sql_stmt+="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=6 ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

/*----FS005----*/
/*---Init temp table----*/
		sql_stmt ="DELETE FROM TMP1;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
		sql_stmt ="DELETE FROM TMP;";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();
/*---Start making FS005---*/
    sql_stmt ="INSERT INTO TMP1 ";
    sql_stmt +="SELECT IDN, Mon_Since, 1 ";
    sql_stmt +=("FROM " + krm023 + " ");
    sql_stmt +="WHERE Bucket_f_1K >=2 ";
    sql_stmt +="GROUP BY IDN, Mon_Since  ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

    sql_stmt ="INSERT INTO TMP ";
    sql_stmt +="SELECT IDN, 3, SUM(V1), NULL, NULL ";
    sql_stmt +="FROM TMP1 ";
    sql_stmt +="WHERE (:now - Mon)<= 3 ";
//    sql_stmt +="      AND (:now1 - Mon) > 0  ";
    sql_stmt +="GROUP BY IDN ";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["now"] = now;
//	  command->Parameters->ParamValues["now1"] = now;
	  command->Execute();

    sql_stmt ="UPDATE PDACO_V1_00_CAL ";
    sql_stmt +="SET ";
    sql_stmt +="FS005_3M_1k = V1 ";
    sql_stmt +="FROM TMP AS A ";
    sql_stmt +="WHERE A.IDN = PDACO_V1_00_CAL.IDN AND Mon=3";
    sql_stmt = sql_stmt.UpperCase();
	  command->CommandText = sql_stmt;
	  command->Execute();

//Trend and interaction variables
		sql_stmt ="UPDATE PDACO_V1_00_CAL ";
		sql_stmt+="SET  ";
		sql_stmt+="INT015_3M = FS016_3M / (CASE WHEN FS101_3M = 0 THEN NULL ELSE CONVERT(FLOAT,FS101_3M) END), ";
    sql_stmt+="INT028_9M = FS014_9M / (CASE WHEN FS101_9M = 0 THEN NULL ELSE CONVERT(FLOAT,FS101_9M) END), ";
		sql_stmt+="FT059_1K_42 = FS059_3M_1K - FS059_9M_1K + FS059_6M_1K, ";
		sql_stmt+="FT212_43_1K = 2 * FS212_3M_1K - FS212_6M_1K, ";
		sql_stmt+="FT102_42 = FS102_3M - FS102_9M + FS102_6M, ";
		sql_stmt+="FT059_1K_43 = 2 * FS059_3M_1K - FS059_6M_1K ";
    sql_stmt += "WHERE IDN = :idn;";
    sql_stmt = sql_stmt.UpperCase();
		command->CommandText = sql_stmt;
    command->Parameters->ParamValues["idn"] = idn;
	  command->Execute();

//Variable transformation phase 1
    sql_stmt="UPDATE PDACO_V1_00_CAL ";
		sql_stmt+="SET  ";
		sql_stmt+="INT015_3M_T =  ";
		sql_stmt+="(CASE WHEN INT015_3M>1 THEN 1 ";
		sql_stmt+="	    WHEN INT015_3M IS NULL THEN 1 ";
		sql_stmt+=" ELSE INT015_3M ";
		sql_stmt+=" END),	 ";
    sql_stmt+="INT028_9M_T = ";
		sql_stmt+="(CASE WHEN INT028_9M IS NULL THEN 0.3 ";
		sql_stmt+="	    WHEN INT028_9M > 0.3 THEN 0.3 ";
		sql_stmt+=" ELSE INT028_9M ";
		sql_stmt+=" END),	 ";
		sql_stmt+="FT059_1K_42_Q = POWER(FT059_1K_42,2), ";
		sql_stmt+="FT212_43_1K_Q = POWER(FT212_43_1K,2), ";
		sql_stmt+="FT102_42_R = POWER((CASE WHEN FT102_42 < 0 THEN NULL ELSE CONVERT(FLOAT,FT102_42) END),0.5), ";
		sql_stmt+="FS031_T = ";
		sql_stmt+="(CASE WHEN FS031 IS NULL THEN -1 ";
		sql_stmt+="	    WHEN FS031>9 THEN 9 ";
		sql_stmt+="	    ELSE FS031 ";
		sql_stmt+=" END), ";
    if(with_gender)
		  sql_stmt+="SEX_T = (CASE WHEN SEX = 1 THEN 1 ELSE 0 END),  ";
    else
		  sql_stmt+="SEX_T = 0.5,  ";
		sql_stmt+="APP_LAST_MONTH_BUCKET_T = ";
		sql_stmt+="(CASE WHEN APP_LAST_MONTH_BUCKET IS NULL THEN 1 ";
		sql_stmt+="	    WHEN APP_LAST_MONTH_BUCKET>2 THEN 2 ";
		sql_stmt+=" ELSE APP_LAST_MONTH_BUCKET ";
		sql_stmt+=" END), ";
		sql_stmt+="FS205_3M_1K_Q = POWER(FS205_3M_1K,2), ";
		sql_stmt+="FT059_1K_43_R= POWER((CASE WHEN FT059_1K_43 < 0 THEN NULL ELSE CONVERT(FLOAT,FT059_1K_43) END),0.5), ";
		sql_stmt+="FS051_T = ";
		sql_stmt+="(CASE WHEN FS051 IS NULL THEN 2 ";
		sql_stmt+="	    WHEN FS051>5 THEN 5 ";
		sql_stmt+="	    ELSE FS051 ";
		sql_stmt+=" END) ";
    sql_stmt += "WHERE IDN = :idn;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["idn"] = idn;
	  command->Execute();
//Variable transformation phase 2
    sql_stmt="UPDATE PDACO_V1_00_CAL ";
    sql_stmt+="SET  ";
    sql_stmt+="FT059_1K_42_Q_T =  ";
    sql_stmt+="(CASE WHEN FT059_1K_42_Q>16 THEN 16 ";
    sql_stmt+=" ELSE FT059_1K_42_Q ";
    sql_stmt+=" END), ";
    sql_stmt+="FT212_43_1K_Q_T = ";
    sql_stmt+="(CASE WHEN FT212_43_1K_Q IS NULL THEN 0 ";
    sql_stmt+="	    ELSE FT212_43_1K_Q ";
    sql_stmt+=" END), ";
    sql_stmt+=" FT102_42_R_T = ";
    sql_stmt+="(CASE WHEN FT102_42_R IS NULL THEN -0.2 ";
    sql_stmt+="	    WHEN FT102_42_R>1.7 THEN 1.7 ";
    sql_stmt+=" ELSE FT102_42_R ";
    sql_stmt+=" END), ";
    sql_stmt+="FS205_3M_1K_Q_T = ";
    sql_stmt+="(CASE WHEN FS205_3M_1K_Q IS NULL THEN 0 ";
    sql_stmt+="	    WHEN FS205_3M_1K_Q>60 THEN 60 ";
    sql_stmt+=" ELSE FS205_3M_1K_Q ";
    sql_stmt+=" END), ";
    sql_stmt+="FT059_1K_43_R_T = ";
    sql_stmt+="(CASE WHEN FT059_1K_43_R IS NULL THEN 1 ";
    sql_stmt+=" ELSE FT059_1K_43_R ";
    sql_stmt+=" END) ";
    sql_stmt += "WHERE IDN = :idn;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["idn"] = idn;
	  command->Execute();
//Model calculation
    sql_stmt="UPDATE PDACO_V1_00_CAL ";
    sql_stmt+="SET  ";
    sql_stmt+="SCORE=0.01474 + ";
    sql_stmt+="INT015_3M_T * 0.04190 + ";
    sql_stmt+="FT059_1K_42_Q_T * 0.00964 + ";
    sql_stmt+="FT212_43_1K_Q_T * 0.01186 + ";
    sql_stmt+="FT102_42_R_T * -0.02683 + ";
    sql_stmt+="FS031_T * 0.00573 + ";
    sql_stmt+="MS117_6M * 0.04292 + ";
    sql_stmt+="FS005_3M_1K * 0.12726 + ";
    sql_stmt+="SEX_T * 0.02654 + ";
    sql_stmt+="App_Last_Month_Bucket_T * 0.05866 + ";
    sql_stmt+="FS205_3M_1K_Q_T * 0.00058973 + ";
    sql_stmt+="INT028_9M_T * -0.08045 + ";
    sql_stmt+="FT059_1K_43_R_T * 0.01873 + ";
    sql_stmt+="FS051_T * 0.00345 ";
    sql_stmt += "WHERE IDN = :idn;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["idn"] = idn;
	  command->Execute();
//Cut
    sql_stmt="UPDATE PDACO_V1_00_CAL ";
    sql_stmt+="SET  ";
    sql_stmt+="CUT =  ";
    sql_stmt+="(CASE  ";
    sql_stmt+="WHEN SCORE <=	-0.03231	THEN 1 ";
    sql_stmt+="WHEN SCORE <=	-0.02275	THEN 2 ";
    sql_stmt+="WHEN SCORE <=	-0.01479	THEN 3 ";
    sql_stmt+="WHEN SCORE <=	-0.00919	THEN 4 ";
    sql_stmt+="WHEN SCORE <=	-0.00438	THEN 5 ";
    sql_stmt+="WHEN SCORE <=	 0.00101	THEN 6 ";
    sql_stmt+="WHEN SCORE <=	 0.00624	THEN 7 ";
    sql_stmt+="WHEN SCORE <=	 0.01245	THEN 8 ";
    sql_stmt+="WHEN SCORE <=	 0.01836	THEN 9 ";
    sql_stmt+="WHEN SCORE <=	 0.02482	THEN 10 ";
    sql_stmt+="WHEN SCORE <=	 0.03219	THEN 11 ";
    sql_stmt+="WHEN SCORE <=	 0.03963	THEN 12 ";
    sql_stmt+="WHEN SCORE <=	 0.04759	THEN 13 ";
    sql_stmt+="WHEN SCORE <=	 0.05585	THEN 14 ";
    sql_stmt+="WHEN SCORE <=	 0.06657	THEN 15 ";
    sql_stmt+="WHEN SCORE <=	 0.07865	THEN 16 ";
    sql_stmt+="WHEN SCORE <=	 0.09435	THEN 17 ";
    sql_stmt+="WHEN SCORE <=	 0.11509	THEN 18 ";
    sql_stmt+="WHEN SCORE <=	 0.15002	THEN 19 ";
    sql_stmt+="WHEN SCORE >	 0.15002	THEN 20 ";
    sql_stmt+="ELSE 0	END) ";
    sql_stmt += "WHERE IDN = :idn;";
    sql_stmt = sql_stmt.UpperCase();
    command->CommandText = sql_stmt;
    command->Parameters->ParamValues["idn"] = idn;
	  command->Execute();


	  sql_stmt = "SELECT SCORE FROM PDACO_V1_00_CAL WHERE IDN = :idn;";
	  sql_stmt = sql_stmt.UpperCase();
	  query->Close();
	  query->SQL->Clear();
	  query->SQL->Add(sql_stmt);
	  query->Parameters->ParamValues["idn"] = idn;
	  query->Open();
	  risk_score = query->FieldValues["SCORE"];
	  query->Close();
	}
	else{
  	sql_stmt = "UPDATE PDACO_V1_00_CAL SET EXCLUSION = :exclusion ";
    sql_stmt += "WHERE IDN = :idn;";
    sql_stmt = sql_stmt.UpperCase();
  	command->CommandText = sql_stmt;
	  command->Parameters->ParamValues["exclusion"] = exclusion;
	  command->Parameters->ParamValues["idn"] = idn;
	  command->Execute();
  }
  return risk_score;
}

char * CurrDateTime ()
{
 time_t timer;
 struct tm *tblock;
 static char buf[20];

 timer = time(NULL);
 tblock = localtime(&timer);
 sprintf (buf, "%04d%02d%02d%02d%02d%02d", tblock->tm_year+1900, tblock->tm_mon+1,
          tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec);
 return (buf);
}


