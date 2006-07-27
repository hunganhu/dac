#ifndef ADODBHPP
#include <ADODB.hpp>
#endif

#include <vector>

#include "cc_err.h"

#pragma alias "@System@@CheckAutoResult$qqrv"="@System@@CheckAutoResult$qqrl"
#define INCOME_CAP 125000  //NT$1.5MM annaully
#define APPROVED_AMT_CAP 500000  
using namespace std;

const AnsiString BAM086 = "BAM086_BASE";
const AnsiString KRM023 = "KRM023_WITH_THREE_BUCKET";
const AnsiString KRM037 = "KRM037_BASE";
const AnsiString KRM001 = "KRM001_BASE";
const AnsiString STM001 = "STM001_BASE";
const AnsiString JAS002 = "JAS002_BASE";
const AnsiString JAS002_T = "JAS002_TRANSFORMED";



//extern "C" __declspec(dllexport)
int TNB_Ploan_AM_Campaign(char *msno, char *jcic_inquiry_date, char *app_input_time,
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
                const AnsiString &msn, const AnsiString &jcic_date,
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

unsigned int get_data_availability(TADOQuery *query,const AnsiString &msn);

unsigned int in_pdaco(TADOQuery *query,
                const AnsiString &krm023, const AnsiString &krm001,
                const AnsiString &jas002, const AnsiString &bam085,
                const AnsiString &msn,
                unsigned int flag, int now,
                unsigned int &unsecured_balance_opt, unsigned int &unsecured_balance_per,
                unsigned int &max_line, unsigned int &unsecured_balance_dac);
void get_product_parameters(TADOQuery *query,
                            const AnsiString &msn, const AnsiString &input_time,
                            double &apr, unsigned int &loan_amount,
                            unsigned int &terms, unsigned int &application_fee);
double pdaco_1_00(TADOQuery *query, TADOCommand *command,
                  const AnsiString &krm023, const AnsiString &krm001,
                  const AnsiString &jas002, const AnsiString &bam085,
                  const AnsiString &stm001, const AnsiString &msn,
                  int gender, int now, unsigned int exclusion, unsigned int data_flag);
void cpdaco_v1_00_preparation(TADOQuery *query, TADOCommand *command,
                    const AnsiString &krm023, const AnsiString &krm001,
                    const AnsiString &jas002, const AnsiString &bam085,
                    const AnsiString &stm001,
                    const AnsiString &msn, int gender,
                    int now, unsigned int exclusion, unsigned int data_flag,
                    double &ms093, double &ms094, double &ms105, double &ms047,
                    double &ms080, double &wi006_9m, double &wi008_3m,
                    double &wi008_9m, double &wi002_12m, double &wi004_12m,
                    double &pdaco_score,
                    bool &ms093_isnull, bool &ms094_isnull, bool &ms105_isnull,
                    bool &ms047_isnull, bool &ms080_isnull,
                    bool &wi006_9m_isnull, bool &wi008_3m_isnull,
                    bool &wi008_9m_isnull, bool &wi002_12m_isnull,
                    bool &wi004_12m_isnull);
void ci_variables(TADOQuery *query, TADOCommand *command, int now,
                  const AnsiString &krm023, const AnsiString &bam085,
                  const AnsiString &msn, unsigned int data_flag,
                  double &ms093, double &ms094, double &ms105, double &ms047,
                  double &ms080, double &wi006_9m, double &wi008_3m,
                  double &wi008_9m, double &wi002_12m, double &wi004_12m,
                  bool &ms093_isnull, bool &ms094_isnull, bool &ms105_isnull, bool &ms047_isnull,
                  bool &ms080_isnull, bool &wi006_9m_isnull, bool &wi008_3m_isnull,
                  bool &wi008_9m_isnull, bool &wi002_12m_isnull, bool &wi004_12m_isnull);
double calculate_pb(unsigned int amount, double apr, unsigned int terms,
                    double ms093, double ms094, double ms105, double ms047,
                    double ms080, double wi006_9m, double wi008_3m,
                    double wi008_9m, double wi002_12m, double wi004_12m,
                    double pdaco_score);
void prescreen_out(unsigned int filter_flag, unsigned int insufficiency_flag,
                   AnsiString idno, AnsiString input_time);

unsigned int data_insufficiency(TADOQuery *query,
                                unsigned int data_flag,
                                unsigned int filter_flag,
                                const AnsiString &krm023,
                                const AnsiString &krm001,
                                const AnsiString &stm001, int now,
                                const AnsiString &msn, const AnsiString &input_time);
                                
void store_result(TADOCommand *command,
                  const AnsiString &idno,
                  const AnsiString &input_time, int result_code,
                  const AnsiString &result_message,
                  unsigned int application_amount,
                  unsigned int unsecured_balance, int approved_amount_dac,
                  int approved_amount_fsc, unsigned int unsecured_balance_dac,
                  unsigned int monthly_income, int npv, double pb,
                  const AnsiString &note,
                  const AnsiString &version, bool normal);

double get_pb(TADOQuery *query, const AnsiString &msn);

void clean_up(TADOCommand *command);


void store_ci_score(TADOCommand *command, const AnsiString &msn,
                    const AnsiString &input_time, double ci_score);

double get_pdaco_score(TADOQuery *query, const AnsiString &msn);
double get_pdaco_1_00_pb(TADOQuery *query, const AnsiString &msn);
unsigned int check_credit_card_block(TADOQuery *query, const AnsiString &msn);
unsigned int get_income(TADOQuery *query,
                        const AnsiString &idn, const AnsiString &input_time);
void vam102_message(TADOQuery *query, const AnsiString &msn, const AnsiString &jcic_date, const AnsiString &input_time, AnsiString &note);

void store_data_insufficiency(TADOQuery *query,
                              const AnsiString &msn, const AnsiString &input_time,
                              unsigned int data_flag,
                              unsigned int delinquent_cards,
                              unsigned int inquiries,
                              unsigned int stop_cards);


