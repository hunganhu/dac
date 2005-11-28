/****************************************************************************
** Licensed Materials - Property of DAC
**
** (C) COPYRIGHT Decision Analytics Consulting 2004
** All Rights Reserved.
**
*****************************************************************************
**
** SOURCE FILE NAME: dacploan.c
**
**
*****************************************************************************/

#include <jni.h>
#include "Dac.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "dac_pl_cal.h"

jclass Class_java_lang_String;
jmethodID MID_String_init;
jmethodID MID_String_getBytes;
char * CurrTime ();

JNIEXPORT jstring JNICALL Java_Dac_dac_1pl_1cal
  (JNIEnv *env, jobject obj, jstring case_sn, jstring alias, jstring uid, jstring upw)
{
     char c_case_sn[32], c_alias[32], c_uid[32], c_upw[32], c_error_msg[512];
     int rc = 0;
     int index = 0;
     FILE *fp;
     const char *caseSN;
     const char *dbName;
     const char *userID;
     const char *userPW;
     char errMsg[512];
     jstring result;
     jbyteArray bytes;
     /* get case_sn */
     caseSN = (*env)->GetStringUTFChars(env, case_sn, NULL);
     if (caseSN == NULL) {
         return NULL; /* OutOfMemoryError already thrown */
     }
     strcpy(c_case_sn, caseSN);
     (*env)->ReleaseStringUTFChars(env, case_sn, caseSN);

     /* get alias */
     dbName = (*env)->GetStringUTFChars(env, alias, NULL);
     if (dbName == NULL) {
         return NULL; /* OutOfMemoryError already thrown */
     }
     strcpy(c_alias, dbName);
     (*env)->ReleaseStringUTFChars(env, alias, dbName);

     /* get uid */
     userID = (*env)->GetStringUTFChars(env, uid, NULL);
     if (userID == NULL) {
         return NULL; /* OutOfMemoryError already thrown */
     }
     strcpy(c_uid, userID);
     (*env)->ReleaseStringUTFChars(env, uid, userID);

     /* get upw */
     userPW = (*env)->GetStringUTFChars(env, upw, NULL);
     if (userPW == NULL) {
         return NULL; /* OutOfMemoryError already thrown */
     }
     strcpy(c_upw, userPW);
     (*env)->ReleaseStringUTFChars(env, upw, userPW);
     errMsg[0] = '\0';

     printf ("C: Case_sn=%s\n", c_case_sn);
     printf ("C: Alias = %s\n", c_alias);
     printf ("C: User ID=%s\n", c_uid);
     printf ("C: User PW=%s\n", c_upw);

     rc = dac_pl_cal(c_case_sn, c_alias, c_uid, c_upw, errMsg);
/*
	fp = fopen("case_list", "r");
	while (fgets(c_case_sn, sizeof(c_case_sn), fp) != NULL) {
	   c_case_sn[12] = '\0';
           index++;
           printf("%s: %d-%s...\n", CurrTime(), index, c_case_sn);
           rc = dac_pl_cal(c_case_sn, c_alias, c_uid, c_upw, errMsg);
           if (rc != 0)
              printf("%s:(%d)%s.\n", CurrTime() , rc, errMsg);
	}
*/
     sprintf (c_error_msg, "%04d%s", -rc, errMsg);

  Class_java_lang_String = (*env)->FindClass(env, "java/lang/String");
  MID_String_init = (*env)->GetMethodID(env, Class_java_lang_String,
				"<init>", "([B)V");
  MID_String_getBytes = (*env)->GetMethodID(env, Class_java_lang_String,
				"getBytes", "()[B");

     if ((*env)->EnsureLocalCapacity(env, 2) < 0) {
        return NULL;
     }
     int len = strlen(c_error_msg);
     bytes = (*env)->NewByteArray(env, len);
     if (bytes != NULL) {
        (*env)->SetByteArrayRegion(env, bytes, 0, len, (jbyte *) c_error_msg);
        result = (*env)->NewObject(env, Class_java_lang_String, MID_String_init, bytes);
        (*env)->DeleteLocalRef(env,bytes);
        return result;
     }
     return NULL;

}
char * CurrTime ()
{
 time_t timer;
 struct tm *tblock;
 static char buf[20];

 timer = time(NULL);
 tblock = localtime(&timer);
 sprintf (buf, "%04d/%02d/%02d %02d:%02d:%02d", tblock->tm_year+1900, tblock->tm_mon+1,
          tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec);
 return (buf);
}
