//---------------------------------------------------------------------------

#ifndef main_utilsH
#define main_utilsH

//---------------------------------------------------------------------------
bool request_for_jcic_data(TADOQuery *query, const AnsiString &id,
             AnsiString &query_sn, AnsiString &jcic_inquiry_result, int &error_no);
AnsiString get_store_jcic_data(TADOConnection *ejcic_connection,
                         TADOQuery *ejcic_query, TADOCommand *command,
                         const AnsiString &query_sn, const AnsiString &msn, const AnsiString &idn,
                         const AnsiString &input_time);
AnsiString get_ejcic_inquiry_result(TADOQuery *query, const AnsiString &query_sn,
                             AnsiString &result, AnsiString &result_code);
void prepare_prelimitary_report(TADOCommand *command, const AnsiString &report_gen_time);
void prepare_final_report(TADOCommand *command, const AnsiString &report_gen_time);
bool generate_prelimitary_report(TADOQuery *query, const AnsiString &report_dir,
                     const AnsiString &report_gen_time);
bool generate_final_report(TADOQuery *query, const AnsiString &report_dir,
                     const AnsiString &report_gen_time);
AnsiString get_applicant_name(const AnsiString &msn, TADOQuery *query);
bool check_msn(const AnsiString &msn, TADOQuery *query);
bool check_id(const AnsiString &idn, const AnsiString &msn, TADOQuery *query);
void init_UI_final();
void init_UI();
void get_product_feature(const AnsiString &product_code, TADOQuery *query, double &apr, unsigned int &application_fee);
void get_product_code(const AnsiString &msn, AnsiString &product_code, TADOQuery *query);
bool check_and_cleanup_result(const AnsiString &msn, TADOQuery *query);
bool check_and_cleanup_final_result(const AnsiString &msn, TADOQuery *query);
// test function to get mail date as jcic inquiry date
AnsiString get_jcic_inquriy_date(const AnsiString &msn, TADOQuery *query);

#endif
 