#ifndef VCL_H
#include <vcl.h>
#endif

#include <ADODB.hpp>

#ifndef COMMON_H
#define COMMON_H
#endif

#pragma alias "@System@@CheckAutoResult$qqrv"="@System@@CheckAutoResult$qqrl"

bool request_for_jcic_data(TADOQuery *query, const AnsiString &id, AnsiString &query_sn, AnsiString &jcic_inquiry_result, int &error_no);
AnsiString get_ejcic_inquiry_result(TADOQuery *query, const AnsiString &query_sn, AnsiString &result, AnsiString &result_code);
AnsiString get_store_jcic_data(TADOConnection *ejcic_connection,
                               TADOQuery *ejcic_query, TADOCommand *command,
                               const AnsiString &query_sn, const AnsiString &idn,
                               const AnsiString &msn);
AnsiString generate_case_sn(TADOQuery *query);
AnsiString fill_in(const AnsiString &source, int digits);
