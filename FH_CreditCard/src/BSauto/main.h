//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
#include <iostream>
#include <fstream>

#define CONNECTION_STRING  "FILE NAME=./AdoConnect.udl"
#define DEBUG  if (Debug) fprintf
//---------------------------------------------------------------------------

/* List of constants that identify return messages */
typedef enum return_types_
  {SUCCESS,
   MISSING_CONTROL,
   MISSING_STATEMENT,
   MISSING_ACCOUNT,
   UNMATCHED_STMT_CONTROL,
   UNMATCHED_ACCT_CONTROL,
   UNMATCHED_STMT_UPLOAD,
   UNMATCHED_ACCT_UPLOAD,
   UNMATCHED_ACCT_CYCLE_UPLOAD,
   UNMATCHED_STMT_TRANSFER,
   UNMATCHED_ACCT_TRANSFER,
   SQL_RUNTIME_ERROR
  } Return_Type;
/* List of return messages */
char *return_msgs [] = {
   "Success.",                       // return code = 0
   "Control file does not exist.",   // return code = 1
   "Statement file does not exist.", // return code = 2
   "Account file does not exist.",   // return code = 3
   "Statement - number of records mismatch between text file and control file.", // return code = 4
   "Account - number of records mismatch between text file and control file.",   // return code = 5
   "Statement - number of records mismatch between text file and temp table.",  // return code = 6
   "Account - number of records mismatch between text file and temp table.",    // return code = 7
   "Account - number of records mismatch between text and cycle date loaded.",  // return code = 8
   "Statement - number of records mismatch between temp and production table.", // return code = 9
   "Account - number of records mismatch between temp and production table."    // return code = 10
};

int write_result(char *bs_home, char *cycle_date, char *create_date);
void write_log_table(TADOHandler *dbhandle, char *cycle_date, char *start,
                     char *end, int retcode, char *msg);


#endif
