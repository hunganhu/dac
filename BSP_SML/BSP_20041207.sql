USE BSP_T
GO

DELETE FROM RESULT
DELETE FROM RESULT_R
DELETE FROM APP_INFO
DELETE FROM Applicants
INSERT INTO Applicants SELECT * FROM Applicants_tmp

SELECT * INTO RESULT_M3_REL_Beta2 FROM RESULT

SELECT COUNT(*) FROM RESULT
SELECT DECLINE_REASON, COUNT(*)
FROM RESULT
GROUP BY DECLINE_REASON
ORDER BY DECLINE_REASON

SELECT A.MSN, A.DECLINE_REASON, B.DECLINE_REASON
FROM RESULT_M4_NEW2_REL AS A INNER JOIN RESULT AS B
ON A.MSN = B.MSN
WHERE A.DECLINE_REASON != B.DECLINE_REASON AND
      A.DECLINE_REASON != 2

SELECT A.MSN, A.HIGHEST_1, B.HIGHEST_1, A.HIGHEST_2, B.HIGHEST_2, A.SECURED_NPV, B.SECURED_NPV, A.UNSECURED_NPV, B.UNSECURED_NPV, A.RESERVED4, B.RESERVED4, A.UNSECURED_PB, B.UNSECURED_PB
FROM RESULT_M4_NEW_REL AS A INNER JOIN RESULT AS B
ON A.MSN = B.MSN
WHERE A.DECLINE_REASON = B.DECLINE_REASON AND
      ((A.HIGHEST_1 != B.HIGHEST_1) OR
       (A.HIGHEST_2 != B.HIGHEST_2) OR
       (A.SECURED_NPV != B.SECURED_NPV) OR
       (A.UNSECURED_NPV != B.UNSECURED_NPV) OR
       (A.RESERVED4 != B.RESERVED4) OR
       (A.UNSECURED_PB != B.UNSECURED_PB))

SELECT A.MSN, A.DECLINE_REASON, B.DECLINE_REASON
FROM RESULT_M3_NEW_REL AS A INNER JOIN RESULT_M3_NEW2_REL AS B
ON A.MSN = B.MSN
WHERE A.DECLINE_REASON != B.DECLINE_REASON
/*142*/

SELECT A.MSN, A.DECLINE_REASON, B.DECLINE_REASON
FROM RESULT_M4_NEW_REL AS A INNER JOIN RESULT_M4_NEW2_REL AS B
ON A.MSN = B.MSN
WHERE A.DECLINE_REASON != B.DECLINE_REASON
/*142*/


SELECT MSN, HIGHEST_1, HIGHEST_2, SECURED_NPV, UNSECURED_NPV, RESERVED4, UNSECURED_PB, DECLINE_REASON
FROM RESULT_M3_NEW2_REL

SELECT MSN, HIGHEST_1, HIGHEST_2, SECURED_NPV, UNSECURED_NPV, RESERVED4, UNSECURED_PB, DECLINE_REASON
FROM RESULT_M4_NEW2_REL

SELECT MSN, HIGHEST_1, HIGHEST_2, SECURED_NPV, UNSECURED_NPV, RESERVED4, UNSECURED_PB, DECLINE_REASON
FROM RESULT_V1_4_REL


SELECT * FROM RESULT

