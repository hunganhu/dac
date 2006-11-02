#ifndef ADODBHPP
#include <ADODB.hpp>
#endif

#include <vector>

#include "cc_err.h"

#pragma alias "@System@@CheckAutoResult$qqrv"="@System@@CheckAutoResult$qqrl"
using namespace std;

const AnsiString BAM086 = "BAM086_BASE";
const AnsiString KRM023 = "KRM023_WITH_THREE_BUCKET";
const AnsiString KRM037 = "KRM037_BASE";
const AnsiString KRM034 = "KRM034_BASE";
const AnsiString KRM001 = "KRM001_BASE";
const AnsiString STM001 = "STM001_BASE";
const AnsiString JAS002 = "JAS002_BASE";
const AnsiString JAS002_T = "JAS002_TRANSFORMED";


extern "C" __declspec(dllexport)
int TNB_CC_AM(char *idn, char *jcic_inquiry_date, char *app_input_time,
              char *bank_code, char *ole_db_str, int gender, char *error);

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
                const AnsiString &idno, const AnsiString &jcic_date,
                const AnsiString &input_time);
void prepare_KRM023(TADOCommand *command, const AnsiString &table, int now);
void merge_prepare_KRM023_KRM037(TADOCommand *command, const AnsiString &krm023,
                                 const AnsiString &krm037, int now);
void merge_prepare_KRM023_KRM034(TADOCommand *command, const AnsiString &krm023,
                                 const AnsiString &krm034, int now);
void prepare_KRM001(TADOCommand *command, const AnsiString &table, int now);
void build_bam_bucket(TADOCommand *command, const AnsiString &bam085, int now);
void prepare_BAM086(TADOCommand *command, const AnsiString &table, int now);
void prepare_STM001(TADOCommand *command, const AnsiString &table,
                    const AnsiString &bank_atm_code);
void prepare_JAS002(TADOCommand *command, const AnsiString &ori_table, const AnsiString &dest_table);
void get_idn_list(TADOCommand *command, AnsiString KRM001, AnsiString KRM023,
                  AnsiString BAM086, AnsiString STM001, AnsiString JAS002);
void create_common_working_tables(TADOCommand * command);
void prepare_2xx_infra(TADOCommand *command, int now);
void create_propensity_tables(TADOCommand *command);
void load_and_initialize_propensity(TADOCommand *command, const AnsiString &idno, unsigned int exclusion);
double propensity(TADOCommand *command, TADOQuery *query, const AnsiString &idno,
                  int now, unsigned int exclusion, unsigned int data_flag);
int propensity_cut(TADOQuery *query, const AnsiString &idno);
unsigned int get_data_availability(TADOQuery *query,const AnsiString &idno);
unsigned int in_daco41(TADOQuery *query,
                const AnsiString &krm023, const AnsiString &krm001,
                const AnsiString &jas002, const AnsiString &bam085,
                const AnsiString &stm007,
                const AnsiString &idno,
                unsigned int flag, int now,
                unsigned int &unsecured_balance_opt, unsigned int &unsecured_balance_per,
                unsigned int &max_line, bool &p1);

double daco41(TADOQuery *query, TADOCommand *command,
              const AnsiString &krm023, const AnsiString &krm001,
              const AnsiString &jas002, const AnsiString &bam085,
              const AnsiString &stm001, const AnsiString &idno,
              int gender, int now, unsigned int exclusion, unsigned int data_flag);

void prescreen_out(unsigned int filter_flag, AnsiString idno, AnsiString input_time);

int credit_card_line(TADOQuery *query, double drisk_score, double propensity_score,
                     unsigned int highest_line,
                     int &risk_cut, int &propensity_cut,
                     double &pb, double &dollar_bad, bool p1);
double get_dollar_bad(TADOQuery *query, unsigned int risk_twentile,
                      unsigned int propensity_twentile, unsigned int line_index);
                     

void store_result(TADOCommand *command,
                  const AnsiString &idno,
                  const AnsiString &input_time, int result_code,
                  const AnsiString &result_message, int optimal_line,
                  unsigned int unsecured_balance,
                  double pb, int risk_cut, int propensity_cut, double dollar_bad,  
                  const AnsiString &note, const AnsiString &version, bool normal);
void vam102_message(TADOQuery *query, const AnsiString &idn,
                    const AnsiString &jcic_date,
                    const AnsiString &input_time, AnsiString &note);
void clean_up(TADOCommand *command);
void data_insufficiency_model(TADOQuery *query, TADOCommand *command,
                              const AnsiString &krm023, const AnsiString &stm001,
                              const AnsiString &bam086, const AnsiString idn,
                              int now, unsigned int data_flag,
                              double &pb, int &line, double &dollar_bad);

void gg(TADOQuery *query, TADOCommand *command,
        const AnsiString &krm023, const AnsiString &stm001,
        const AnsiString &idn, int now,
        double &pb, int &line, double &dollar_bad);
void lg(TADOQuery *query, TADOCommand *command,
        const AnsiString &bam086, const AnsiString &stm001,
        const AnsiString &idn, int now,
        double &pb, int &line, double &dollar_bad);

