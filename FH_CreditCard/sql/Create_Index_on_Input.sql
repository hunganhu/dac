/* Create_Index_u_statementMonth_idn_on_Input */
 create unique index u_statementMonth_idn on source (statement_month, idn);
/* Create_Index_i_statementMonth_on_Input */
 create index i_statementMonth on source (statement_month);
/* Create_Index_i_idn_on_Input */
 create index i_idn on source (idn);
