//---------------------------------------------------------------------------

#ifndef CodesH
#define CodesH
//---------------------------------------------------------------------------
#endif

#ifndef ADODBHPP
#include <ADODB.hpp>
#endif

#include "NPV.h"

#pragma alias "@System@@CheckAutoResult$qqrv"="@System@@CheckAutoResult$qqrl"

const AnsiString BAM086 = "BAM086_BASE";
const AnsiString KRM023 = "KRM023_WITH_THREE_BUCKET";
const AnsiString KRM037 = "KRM037_BASE";
const AnsiString KRM001 = "KRM001_BASE";
const AnsiString STM007 = "STM007_BASE";
const AnsiString DAM103 = "DAM103_BASE";
const AnsiString DAM203 = "DAM203_BASE";
const AnsiString JAS002 = "JAS002_BASE";
const AnsiString JAS002_T = "JAS002_TRANSFORMED";

#define DataErrorMsg(x)  ktbMsg[(x)].msg
#define DataErrorCode(x) ktbMsg[(x)].code

typedef struct _output_msg {
	int code;
	char msg[128];
} Output_Msg, *Output_Msg_Ptr;

enum ErrorCodes {
	OUTPUT_CODE_101, 
	OUTPUT_CODE_102, 
	OUTPUT_CODE_103, 
	OUTPUT_CODE_104, 
	OUTPUT_CODE_105, 
	OUTPUT_CODE_106, 
	OUTPUT_CODE_107, 
	OUTPUT_CODE_108, 
	OUTPUT_CODE_109, 
	OUTPUT_CODE_110, 
	OUTPUT_CODE_111,
	OUTPUT_CODE_112, 
	OUTPUT_CODE_120, 
	OUTPUT_CODE_121, 
	OUTPUT_CODE_122, 
	OUTPUT_CODE_123, 
	OUTPUT_CODE_124, 
	OUTPUT_CODE_201, 
	OUTPUT_CODE_202, 
	OUTPUT_CODE_203, 
	OUTPUT_CODE_0, 
	PRESCREEN_PASS, 
	OUTPUT_CODE_204, 
	OUTPUT_CODE_301
};
Output_Msg ktbMsg[] = {
	{101, "模組建議婉拒：重大信用瑕疵 "},
	{102, "模組建議婉拒：信用卡一年內曾逾期超過90天 "},
	{103, "模組建議婉拒：貸款一年內曾逾期超過90天 "},
	{104, "模組建議婉拒：現金卡最近一個月遲繳 "},
	{105, "模組建議婉拒：信用卡動支比例達95%以上 "},
	{106, "模組建議婉拒：現金卡與信用卡餘額超過50萬元 "},
	{107, "模組建議婉拒：個人信用風險過高 "},
	{108, "模組建議婉拒：任一貸款最近一個月遲繳 "},
	{109, "模組建議婉拒：無擔保貸款總額超過100萬元 "},
	{110, "模組建議婉拒：負債比率過高 "},
	{111, "模組建議婉拒：聯徵特殊註記 "},
	{112, "模組建議婉拒：京城銀行黑名單 "},
	{120, "模組建議婉拒：JCIC資料不足且查詢過多 "},
	{121, "模組建議婉拒：JCIC資料不足且有信用卡逾繳紀錄 "},
	{122, "模組建議婉拒：JCIC資料不足且有強制停卡紀錄 "},
	{123, "模組建議婉拒：JCIC資料不足且查詢過多 "},
	{124, "模組建議婉拒：JCIC資料不足且有強制停卡紀錄 "},
	{201, "模組建議人工審核：請詳加核實收入相關資料；模組輸出僅供參考 "},
	{202, "模組建議人工審核：請詳加核實收入相關資料；模組輸出僅供參考 "},
	{203, "模組建議人工審核：請詳加核實收入相關資料；模組輸出僅供參考 "},
	{  0, "模組建議核准 "},
	{  0, "初審通過 "},
	{204, "模組建議婉拒：貢獻度過低 "},
	{301, "模組建議轉個人信貸(抵押品不予承作，但個人信用風險合格 "}
};


extern "C" __declspec(dllexport)
int DAC_SML_NPV(char *idn, char *msn, char *time_stamp, char *ole_db,
                double principal, double apr, int period, double app_fee,
                double gav, double nav, char *zip, double first_lien_value, int monthly_income, char *error);

extern "C" __declspec(dllexport)
int DAC_SML_PRESCREEN(char *idn, char *msn, char *time_stamp, char *ole_db,
                      int monthly_income, char *error);

extern "C" __declspec(dllexport)
int TEST_SML_NPV(char *idn, char *msn, char *time_stamp, char *ole_db,
                double principal, double apr, int period, double app_fee,
                double gav, double nav, char *zip, double first_lien_value,
                double balance, double risk_score, int filter_flag, char *error);

void prepare_2xx_infra(TADOCommand *command, int now);

void capture_propensity_result(TADOCommand *command);

void store_input(TADOCommand *command, AnsiString idno, AnsiString msn_no, AnsiString time_stamp_no,
                 double gender, double principal, int test_cell, double gav,
                 double nav, AnsiString zip, double first_lien_value, int sc);
void store_result_test(TADOCommand * command, AnsiString idno, AnsiString msn_no, AnsiString time_stamp_no,
                       int test_cell, double secured_amount, double unsecured_amount,
                       double risk_score, int propensity_decile, double principal,
                       double lending_ratio, bool bsp_exclusion, double secured_npv,
                       double unsecured_npv, double secured_pb, double unsecured_pb,
                       int decline_reason, int sc, double balance);
void store_result(TADOCommand * command, AnsiString idno, AnsiString msn_no, AnsiString time_stamp_no,
                       int test_cell, double secured_amount, double unsecured_amount,
                       double risk_score, int propensity_decile, double principal,
                       double lending_ratio, bool bsp_exclusion, double secured_npv,
                       double unsecured_npv, double secured_pb, double unsecured_pb,
                       int decline_reason, int sc, double balance);
//void get_idn_list(TADOCommand *command, AnsiString KRM001, AnsiString KRM023,
//                  AnsiString BAM086, AnsiString STM007, AnsiString DAM103,
//                  AnsiString DAM203);
void get_idn_list(TADOCommand *command, AnsiString KRM001, AnsiString KRM023,
                  AnsiString BAM086, AnsiString STM007, AnsiString JAS002);
//bool error_checking(int gender, int test_cell, double gav, double nav, double lien_value,
//                    double principal, bool il, AnsiString msn, AnsiString *error_message);

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
                 bool archive=false, bool use_krm037=true);
void prepare_KRM023(TADOCommand *command, const AnsiString &table, int now);
void merge_prepare_KRM023_KRM037(TADOCommand *command, const AnsiString &krm023,
                                 const AnsiString &krm037, int now);
void merge_prepare_KRM023_KRM034(TADOCommand *command, const AnsiString &krm023,
                                 const AnsiString &krm034, int now);
void prepare_KRM001(TADOCommand *command, const AnsiString &table, int now);
void prepare_BAM086(TADOCommand *command, const AnsiString &table);
void prepare_STM007(TADOCommand *command, const AnsiString &table);
void prepare_DAM103(TADOCommand *command, const AnsiString &table);
void prepare_DAM203(TADOCommand *command, const AnsiString &table);
void prepare_JAS002(TADOCommand *command, const AnsiString &ori_table, const AnsiString &dest_table);
void create_common_working_tables(TADOCommand * command);
double propensity(TADOCommand *command, TADOQuery *query, const AnsiString &idn, int now);
void create_propensity_tables(TADOCommand *command);
int propensity_cut(TADOQuery *query, const AnsiString &idno);
void load_and_initialize_propensity(TADOCommand *command, const AnsiString &idn);
int get_product_type(int test_cell);

void write_premier_result(TADOCommand *command, AnsiString msn_no, AnsiString time_stamp_no,
                 double rscore, int code, AnsiString msg);
void write_final_result(TADOCommand *command, AnsiString msn_no, double pb, int npv, 
                 int optimal_amount, int code, AnsiString msg);
void write_final_result_fail(TADOCommand *command, AnsiString msn_no, 
                 int code, AnsiString msg);
char * CurrDateTime ();
                 

//-------------------------------------
unsigned int in_pdaco_1_00(TADOQuery *query,
                const AnsiString &krm023, const AnsiString &krm001,
                const AnsiString &jas002, const AnsiString &bam085, const AnsiString &stm007,
                const AnsiString &msn, const AnsiString &idno,
                const AnsiString &time_stamp,
                unsigned char flag, int monthly_income, int now);
unsigned char get_data_availability(TADOQuery *query,const AnsiString &idno); 
double pdaco_1_00(TADOCommand *command, TADOQuery *query, const AnsiString &case_no,
                const AnsiString &idn, const AnsiString &time_stamp, double gender, 
                unsigned char data_flag, const AnsiString &krm023, 
                const AnsiString &krm001, const AnsiString &bam085,
                const AnsiString &jas002, const AnsiString &stm007, 
                int now, bool with_gender);


