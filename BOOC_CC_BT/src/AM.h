#ifndef ADODBHPP
#include <ADODB.hpp>
#endif

#include <vector>

#include "cc_err.h"

#pragma alias "@System@@CheckAutoResult$qqrv"="@System@@CheckAutoResult$qqrl"
using namespace std;

const AnsiString BAM086 = "BAM086_BASE";
const AnsiString KRM023 = "KRM023_WITH_THREE_BUCKET";
const AnsiString KRM034 = "KRM034_BASE";
const AnsiString KRM037 = "KRM037_BASE";
const AnsiString KRM040 = "KRM040_BASE";
const AnsiString KRM001 = "KRM001_BASE";
const AnsiString STM001 = "STM001_BASE";
const AnsiString JAS002 = "JAS002_BASE";
const AnsiString JAS002_T = "JAS002_TRANSFORMED";
const int UPPER = 150000; //The upper limit for BT, above it, the module output is for referece only


extern "C" __declspec(dllexport)
    int BOOC_CC_BT_AM(char *app_sn, char *idn, /*char *jcic_data_date,*/
                      int product_type, char *bank_code, char *ole_db_str,
                      char *error);


void connect(TADOConnection *connection, const AnsiString &connection_string="",
             int connection_timeout = 30,
             int command_timeout = 300);
void initialize_ado_objects(TADOConnection *connection, TADOQuery *query,
                            TADOQuery * query2, TADOCommand *command,
                            int command_timeout);
int yrmon_to_mon(const AnsiString &inquiry_month="",
                 bool time_lock=false, const AnsiString &lock="",
                 bool archive=false, bool use_krm037=true);
void copy_table(TADOCommand *command,
                const AnsiString &source_table, const AnsiString &destination_table,
                const AnsiString &app_sn, const AnsiString &idno);
void prepare_KRM023(TADOCommand *command, const AnsiString &table, int now);
void merge_prepare_KRM023_KRM037(TADOCommand *command, const AnsiString &krm023,
                                 const AnsiString &krm037, int now);
void merge_prepare_KRM023_KRM040(TADOCommand *command, const AnsiString &krm023,
                                 const AnsiString &krm040, int now);
void merge_prepare_KRM023_KRM034(TADOCommand *command, const AnsiString &krm023,
                                 const AnsiString &krm034, int now);
void prepare_KRM001(TADOCommand *command, const AnsiString &table, int now);
void build_bam_bucket(TADOCommand *command, const AnsiString &bam085, int now);
void prepare_BAM086(TADOCommand *command, const AnsiString &table, int now);
void prepare_STM001(TADOCommand *command, const AnsiString &table,
                    const AnsiString &bank_atm_code,
                    const AnsiString &jcic_inquiry_date);
void prepare_JAS002(TADOCommand *command, const AnsiString &ori_table, const AnsiString &dest_table);
void get_idn_list(TADOCommand *command, AnsiString KRM001, AnsiString KRM023,
                  AnsiString BAM086, AnsiString STM001, AnsiString JAS002);
void create_common_working_tables(TADOCommand * command);
void prepare_2xx_infra(TADOCommand *command, int now);
void create_propensity_tables(TADOCommand *command);
void load_and_initialize_propensity(TADOCommand *command, const AnsiString &app_sn, unsigned int exclusion);
double propensity(TADOCommand *command, TADOQuery *query, const AnsiString &app_sn,
                  int now, unsigned int exclusion, unsigned int data_flag);
int propensity_cut(TADOQuery *query, const AnsiString &app_sn);
unsigned int get_data_availability(TADOQuery *query,const AnsiString &app_sn);
void get_prescreen_parameters(TADOQuery *query,
                              const AnsiString &case_sn, const AnsiString &idno,
                              double &cash_card_util_cap, double &credit_card_util_cap, int &cc_balance_cap);
unsigned int in_daco41(TADOQuery *query,
                const AnsiString &krm023, const AnsiString &krm001,
                const AnsiString &jas002, const AnsiString &bam085,
                const AnsiString &case_sn,
                unsigned int flag, int now,
                unsigned int &unsecured_amount_opt, unsigned int &unsecured_amount_per,
                unsigned int &max_line, bool &p1,
                double cash_card_util_cap, double credit_card_util_cap, int cc_balance_cap);

double daco41(TADOQuery *query, TADOCommand *command,
              const AnsiString &krm023, const AnsiString &krm001,
              const AnsiString &jas002, const AnsiString &bam085,
              const AnsiString &stm001, const AnsiString &app_sn,
              int gender, int now, unsigned int exclusion, unsigned int data_flag);

double pdaco_1_00(TADOQuery *query, TADOCommand *command,
                  const AnsiString &krm023, const AnsiString &krm001,
                  const AnsiString &jas002, const AnsiString &bam085,
                  const AnsiString &stm001, const AnsiString &app_sn,
                  int gender, int now, unsigned int exclusion, unsigned int data_flag);

double tf_a2(TADOQuery *query, TADOCommand *command,
             const AnsiString &krm023, const AnsiString &krm001,
             const AnsiString &bam085, const AnsiString &stm001,
             const AnsiString &app_sn, int gender, int now,
             unsigned int exclusion, unsigned int data_flag);

void get_product_parameters(TADOQuery *query, const AnsiString &case_sn,
                            const AnsiString &idno, int product_type,
                            AnsiString &app_date, double &regular_apr,
                            int &bt_counts, int &total_bt_amount,
                            int &bt_fee_type, double &monthly_bt_fee,
                            int &months_charging_bt_fee, int &bt_period,
                            int &gender, AnsiString &card_type,
                            double cash_card_util_cap,
                            double credit_card_util_cap,
                            int cc_balance_cap);

void rates_to_vector(TADOQuery *query, const AnsiString &case_no,
                     const AnsiString &idno,
                     vector<double>&interest_scheme, double regular_apr);

void check_error(double regular_apr, int bt_counts, int total_bt_amount,
                 int bt_fee_type, double monthly_bt_fee,
                 int months_charging_bt_fee, int bt_period,
                 int gender, int product_type, const AnsiString &card_type,
                 vector<double> &interest_scheme,
                 AnsiString app_no, AnsiString idno, AnsiString app_date,
                 double cash_card_util_cap, double credit_card_util_cap, int cc_balance_cap);
                 
void prescreen_out(unsigned int filter_flag, AnsiString app_no, AnsiString idno,
                   AnsiString app_date, int product_type, int amount,
                   double cash_card_util_cap, double credit_card_util_cap,
                   int cc_balance_cap);

bool check_tf_a2_krm023(TADOQuery *query, const AnsiString &KRM023, const int now);

double bt_pb(double prisk_score, double trisk_score, bool tf_a2);

int credit_card_line(TADOQuery *query,
                     double drisk_score, double propensity_score,
                     unsigned int highest_line,
                     const AnsiString &card_type,
                     int &risk_cut, int &propensity_cut,
                     double &pb, double &ev, double &dollar_bad, bool p1);

void store_result(TADOCommand *command,
                  const AnsiString &case_sn, const AnsiString &idno,
                  const AnsiString &app_date, int product_type, int result_code,
                  const AnsiString &result_message, int optimal_line,
                  int risk_cut, int propensity_cut,
                  double ev, double pb, double dollar_bad, bool normal);

void clean_up(TADOCommand *command);


void get_score(TADOQuery *query, double &pscore, double &tfscore, bool &tf, const AnsiString &case_sn);

double get_dollar_bad(TADOQuery *query, unsigned int risk_twentile,
                      unsigned int propensity_twentile, unsigned int line_index);

