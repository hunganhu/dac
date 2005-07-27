USE BSP_SML_20040830
GO

SELECT A.MSN, 
       LEFT(LTRIM(A.TIME_STAMP),8) AS APP_DATE, 
       A.TEST_CELL, 
       (CASE WHEN A.IDN = A.IDN1 THEN 1 ELSE 0 END) AS APP_OWNER, 
       NAV, 
       GAV, 
       PRINCIPAL AS APP_AMOUNT, 
       FIRST_LIEN_VALUE,
       RESERVED2 AS FLAG,
       (CASE 
          WHEN RESERVED2 IN (1, 5) THEN 'LU'
          WHEN RESERVED2 IN (2, 6) THEN 'HU'
          ELSE 'NONE'
        END) AS RISK_GRP,      
/*       RISK_CUT, */
/*       PROPENSITY_DECILE, */
       HIGHEST_1 AS SECURED, 
       HIGHEST_2 AS UNSECURED,        
       (CASE
          WHEN RESERVED2=0 THEN 'NONE'
          WHEN RESERVED2>4 THEN '100%'
          ELSE '80%'
        END) AS LINE_GRP,   
       FIRST_LIEN_VALUE / (CASE WHEN NAV=0 THEN NULL ELSE CONVERT(FLOAT,NAV) END)/1.2 AS LENDING_RATIO,
       REVIEWED,
       SECURED_NPV,
       UNSECURED_NPV,
       RISK_SCORE,
       DECLINE_REASON 
FROM (APP_INFO_200405 AS A 
INNER JOIN RESULT_200405 AS B 
    ON A.MSN = B.MSN 
   AND A.TIME_STAMP = B.TIME_STAMP)  
ORDER BY A.MSN
GO

SELECT A.MSN, 
       LEFT(LTRIM(A.TIME_STAMP),8) AS APP_DATE, 
       A.TEST_CELL, 
       (CASE WHEN A.IDN = A.IDN1 THEN 1 ELSE 0 END) AS APP_OWNER, 
       NAV, 
       GAV, 
       PRINCIPAL AS APP_AMOUNT, 
       FIRST_LIEN_VALUE,
       RESERVED2 AS FLAG,
       (CASE 
          WHEN RESERVED2 IN (1, 5) THEN 'LU'
          WHEN RESERVED2 IN (2, 6) THEN 'HU'
          ELSE 'NONE'
        END) AS RISK_GRP,      
/*       RISK_CUT, */
/*       PROPENSITY_DECILE, */
       HIGHEST_1 AS SECURED, 
       HIGHEST_2 AS UNSECURED,        
       (CASE
          WHEN RESERVED2=0 THEN 'NONE'
          WHEN RESERVED2>4 THEN '100%'
          ELSE '80%'
        END) AS LINE_GRP,   
       FIRST_LIEN_VALUE / (CASE WHEN NAV=0 THEN NULL ELSE CONVERT(FLOAT,NAV) END)/1.2 AS LENDING_RATIO,
       REVIEWED,
       SECURED_NPV,
       UNSECURED_NPV,
       RISK_SCORE,
       DECLINE_REASON 
FROM (APP_INFO_200406 AS A 
INNER JOIN RESULT_200406 AS B 
    ON A.MSN = B.MSN 
   AND A.TIME_STAMP = B.TIME_STAMP)  
ORDER BY A.MSN
GO

SELECT A.MSN, 
       LEFT(LTRIM(A.TIME_STAMP),8) AS APP_DATE, 
       A.TEST_CELL, 
       (CASE WHEN A.IDN = A.IDN1 THEN 1 ELSE 0 END) AS APP_OWNER, 
       NAV, 
       GAV, 
       PRINCIPAL AS APP_AMOUNT, 
       FIRST_LIEN_VALUE,
       RESERVED2 AS FLAG,
       (CASE 
          WHEN RESERVED2 IN (1, 5) THEN 'LU'
          WHEN RESERVED2 IN (2, 6) THEN 'HU'
          ELSE 'NONE'
        END) AS RISK_GRP,      
/*       RISK_CUT, */
/*       PROPENSITY_DECILE, */
       HIGHEST_1 AS SECURED, 
       HIGHEST_2 AS UNSECURED,        
       (CASE
          WHEN RESERVED2=0 THEN 'NONE'
          WHEN RESERVED2>4 THEN '100%'
          ELSE '80%'
        END) AS LINE_GRP,   
       FIRST_LIEN_VALUE / (CASE WHEN NAV=0 THEN NULL ELSE CONVERT(FLOAT,NAV) END)/1.2 AS LENDING_RATIO,
       REVIEWED,
       SECURED_NPV,
       UNSECURED_NPV,
       RISK_SCORE,
       DECLINE_REASON 
FROM (APP_INFO_200407 AS A 
INNER JOIN RESULT_200407 AS B 
    ON A.MSN = B.MSN 
   AND A.TIME_STAMP = B.TIME_STAMP)  
ORDER BY A.MSN
GO

SELECT A.MSN, 
       LEFT(LTRIM(A.TIME_STAMP),8) AS APP_DATE, 
       A.TEST_CELL, 
       (CASE WHEN A.IDN = A.IDN1 THEN 1 ELSE 0 END) AS APP_OWNER, 
       NAV, 
       GAV, 
       PRINCIPAL AS APP_AMOUNT, 
       FIRST_LIEN_VALUE,
       RESERVED2 AS FLAG,
       (CASE 
          WHEN RESERVED2 IN (1, 5) THEN 'LU'
          WHEN RESERVED2 IN (2, 6) THEN 'HU'
          ELSE 'NONE'
        END) AS RISK_GRP,      
/*       RISK_CUT, */
/*       PROPENSITY_DECILE, */
       HIGHEST_1 AS SECURED, 
       HIGHEST_2 AS UNSECURED,        
       (CASE
          WHEN RESERVED2=0 THEN 'NONE'
          WHEN RESERVED2>4 THEN '100%'
          ELSE '80%'
        END) AS LINE_GRP,   
       FIRST_LIEN_VALUE / (CASE WHEN NAV=0 THEN NULL ELSE CONVERT(FLOAT,NAV) END)/1.2 AS LENDING_RATIO,
       REVIEWED,
       SECURED_NPV,
       UNSECURED_NPV,
       RISK_SCORE,
       DECLINE_REASON 
FROM (APP_INFO_200408 AS A 
INNER JOIN RESULT_200408 AS B 
    ON A.MSN = B.MSN 
   AND A.TIME_STAMP = B.TIME_STAMP)  
ORDER BY A.MSN
GO