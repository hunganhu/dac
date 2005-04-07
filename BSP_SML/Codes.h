
#ifndef ADODBHPP
#include <ADODB.hpp>
#endif

#include "NPV.h"

#pragma alias "@System@@CheckAutoResult$qqrv"="@System@@CheckAutoResult$qqrl"

const AnsiString BAM086 = "BAM086_BASE";
const AnsiString KRM023 = "KRM023_WITH_THREE_BUCKET";
const AnsiString KRM001 = "KRM001_BASE";
const AnsiString STM001 = "STM001_BASE";
const AnsiString DAM103 = "DAM103_BASE";
const AnsiString DAM203 = "DAM203_BASE";
const AnsiString JAS002 = "JAS002_BASE";
const AnsiString JAS002_T = "JAS002_TRANSFORMED";



extern "C" __declspec(dllexport)int DAC_SML_NPV(char *idn, char *idn1, char *idn2,
  char *msn, char *time_stamp, char *ole_db, int gender, int sc, double principal, int test_cell,
  double gav, double nav, char *zip, double first_lien_value,
  char *error);


void prepare_2xx_infra(TADOCommand *command, int now);

void capture_propensity_result(TADOCommand *command);

void store_input(TADOCommand *command, AnsiString idno,AnsiString idno1,
                 AnsiString idno2, AnsiString msn_no, AnsiString time_stamp_no,
                 double gender, double principal, int test_cell, double gav,
                 double nav, AnsiString zip, double first_lien_value, int sc);
void store_result_test(TADOCommand * command, AnsiString idno, AnsiString idno1,
                       AnsiString idno2, AnsiString msn_no, AnsiString time_stamp_no,
                       int test_cell, double secured_amount, double unsecured_amount,
                       double risk_score, int propensity_decile, double principal,
                       double lending_ratio, bool bsp_exclusion, double secured_npv,
                       double unsecured_npv, double secured_pb, double unsecured_pb,
                       int decline_reason, int sc, double balance);
void store_result(TADOCommand * command, AnsiString idno, AnsiString idno1,
                       AnsiString idno2, AnsiString msn_no, AnsiString time_stamp_no,
                       int test_cell, double secured_amount, double unsecured_amount,
                       double risk_score, int propensity_decile, double principal,
                       double lending_ratio, bool bsp_exclusion, double secured_npv,
                       double unsecured_npv, double secured_pb, double unsecured_pb,
                       int decline_reason, int sc, double balance);
//void get_idn_list(TADOCommand *command, AnsiString KRM001, AnsiString KRM023,
//                  AnsiString BAM086, AnsiString STM001, AnsiString DAM103,
//                  AnsiString DAM203);
void get_idn_list(TADOCommand *command, AnsiString KRM001, AnsiString KRM023,
                  AnsiString BAM086, AnsiString STM001, AnsiString JAS002);
bool error_checking(int gender, int test_cell, double gav, double nav, double lien_value,
                    double principal, AnsiString msn, AnsiString *error_message);

double krm023_balance(TADOQuery *query, AnsiString idn, int now);
double bam009_balance(TADOQuery *query, AnsiString idn);

void clean_up(TADOCommand *command);
//------------------------------------
void initialize_ado_objects(TADOConnection *connection, TADOQuery *query,
                            TADOQuery * query2, TADOCommand *command,
                            int command_timeout = 300);
void connect(TADOConnection *connection, const AnsiString &connection_string="",
             int connection_timeout = 30,
             int command_timeout = 300);                            
void copy_table(TADOCommand *command,
                const AnsiString &source_table, const AnsiString &destination_table,
                const AnsiString &msn, const AnsiString &time_stamp);
int yrmon_to_mon(const AnsiString &inquiry_month="", 
                 bool time_lock=false, const AnsiString &lock="",
                 bool archive=false);
void prepare_KRM023(TADOCommand *command, const AnsiString &table, int now);
void prepare_KRM001(TADOCommand *command, const AnsiString &table, int now);
void prepare_BAM086(TADOCommand *command, const AnsiString &table);
void prepare_STM001(TADOCommand *command, const AnsiString &table);
void prepare_DAM103(TADOCommand *command, const AnsiString &table);
void prepare_DAM203(TADOCommand *command, const AnsiString &table);
void prepare_JAS002(TADOCommand *command, const AnsiString &ori_table, const AnsiString &dest_table);
void create_common_working_tables(TADOCommand * command);
double propensity(TADOCommand *command, TADOQuery *query, const AnsiString &idn, int now);
void create_propensity_tables(TADOCommand *command);
int propensity_cut(TADOQuery *query, const AnsiString &idno);
void load_and_initialize_propensity(TADOCommand *command, const AnsiString &idn);
//-------------------------------------
unsigned int in_pdaco_1_00(TADOQuery *query,
                const AnsiString &krm023, const AnsiString &krm001,
                const AnsiString &jas002, const AnsiString &bam085,
                const AnsiString &msn, const AnsiString &idno,
                const AnsiString &time_stamp,
                unsigned char flag, int now);
unsigned char get_data_availability(TADOQuery *query,const AnsiString &idno); 
double pdaco_1_00(TADOCommand *command, TADOQuery *query, const AnsiString &case_no,
                const AnsiString &idn, const AnsiString &time_stamp, double gender, 
                unsigned char data_flag, const AnsiString &krm023, 
                const AnsiString &krm001, const AnsiString &bam085,
                const AnsiString &jas002, const AnsiString &stm001, 
                int now, bool with_gender);


