/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2005
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: utilemb.h
**
** SAMPLE: Error-checking utility header file for utilemb.sqC 
**
**         This is the header file for the utilemb.sqC error-checking utility
**         file. The utilemb.sqC file is compiled and linked in as an object 
**         module with embedded SQL sample programs by the supplied makefile 
**         and build files.
**
** Macro defined:
**         EMB_SQL_CHECK(MSG_STR)
**
** Class declared:
**         DbEmb
**
****************************************************************************/

#ifndef UTILEMB_H
#define UTILEMB_H

#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_UID_LENGTH 18
#define MAX_PWD_LENGTH 30

#ifndef max
#define max( A, B ) ( ( A ) > ( B ) ? ( A ) : ( B ) )
#endif
#ifndef min
#define min( A, B ) ( ( A ) > ( B ) ? ( B ) : ( A ) )
#endif

#define LOBLENGTH 29

#define USERID_SZ 128
#define PSWD_SZ 14

#if (defined(DB2NT))
#define PATH_SEP "\\"
#else //UNIX
#define PATH_SEP "/"
#endif

#ifdef DEBUG
#define debug 1
#else
#define debug 0
#endif

// classes and methods
class Db
{
  public:
    void setDb(char *, char *, char *);
    char *getAlias();
    char *getUser();
    char *getPswd();
  protected:
    char alias[SQL_ALIAS_SZ + 1];
    char user[USERID_SZ + 1];
    char pswd[PSWD_SZ + 1];
};

class SqlInfo
{
  public:
    static void SqlInfoPrint(struct sqlca *, int, char *);
};

// macro for embedded SQL checking
#define Debug( MSG_STR )                              \
if (debug) cout << CurrDateTime() << ": " << MSG_STR << endl;    

#define Info                              \
if (debug) printf 

#define EMB_SQL_CHECK( MSG_STR )                              \
SqlInfo::SqlInfoPrint(&sqlca, __LINE__, __FILE__ ); \
if (debug) cout << CurrDateTime() << ": " << MSG_STR << endl;     \
if ( sqlca.sqlcode < 0 )                                      \
{                                                             \
  DbEmb::TransRollback();                                     \
  return 1;                                                   \
}

// utility class for embedded SQL checking
class DbEmb: public Db
{
  public:
    static void TransRollback();
    int Connect();
    int Disconnect();
};
char * CurrDateTime ();
 
#ifdef __cplusplus
}
#endif

#endif // UTILEMB_H

