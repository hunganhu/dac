bcp "select Code, Description from Fubon_Ploan_Test_2005.dbo.rd_card_type_code" queryout c:\fuhwa\fubon\import\rd_card_type_code.csv -c -t , -U jcic -P jcic -S giza
bcp "select Code, Description from Fubon_Ploan_Test_2005.dbo.rd_credit_card_product_id" queryout c:\fuhwa\fubon\import\rd_credit_card_product_id.csv -c -t , -U jcic -P jcic -S giza

bcp "select customer_id from Fubon_Ploan_Test_2005.dbo.bu_customer_bank_bond" queryout c:\fuhwa\fubon\import\bu_customer_bank_bond.csv -c -t , -U jcic -P jcic -S giza
bcp "select customer_id from Fubon_Ploan_Test_2005.dbo.bu_customer_bank_fund" queryout c:\fuhwa\fubon\import\bu_customer_bank_fund.csv -c -t , -U jcic -P jcic -S giza
bcp "select customer_id from Fubon_Ploan_Test_2005.dbo.bu_customer_futures" queryout c:\fuhwa\fubon\import\bu_customer_futures.csv -c -t , -U jcic -P jcic -S giza
bcp "select customer_id from Fubon_Ploan_Test_2005.dbo.bu_customer_insurance_group" queryout c:\fuhwa\fubon\import\bu_customer_insurance_group.csv -c -t , -U jcic -P jcic -S giza
bcp "select customer_id from Fubon_Ploan_Test_2005.dbo.bu_customer_insurance_life" queryout c:\fuhwa\fubon\import\bu_customer_insurance_life.csv -c -t , -U jcic -P jcic -S giza
bcp "select customer_id from Fubon_Ploan_Test_2005.dbo.bu_customer_investment_trust" queryout c:\fuhwa\fubon\import\bu_customer_investment_trust.csv -c -t , -U jcic -P jcic -S giza
bcp "select customer_id from Fubon_Ploan_Test_2005.dbo.bu_customer_property_insure" queryout c:\fuhwa\fubon\import\bu_customer_property_insure.csv -c -t , -U jcic -P jcic -S giza
bcp "select customer_id from Fubon_Ploan_Test_2005.dbo.bu_customer_security_bond" queryout c:\fuhwa\fubon\import\bu_customer_security_bond.csv -c -t , -U jcic -P jcic -S giza
bcp "select customer_id from Fubon_Ploan_Test_2005.dbo.bu_customer_security_brokage" queryout c:\fuhwa\fubon\import\bu_customer_security_brokage.csv -c -t , -U jcic -P jcic -S giza
bcp "select Acct_Status_Code, Card_Cancel_Date, Card_Issue_Date, Card_Type_Code, Primary_Cardholder_ID, Product_ID from Fubon_Ploan_Test_2005.dbo.cc_acct_credit_card" queryout c:\fuhwa\fubon\import\cc_acct_credit_card.csv -c -t , -U jcic -P jcic -S giza
bcp "select Billing_Close_Date, Last_Payment_Amt, Monthly_Limit_Amt, Primary_Cardholder_ID, Revolving_Interest_Amt, This_Term_Expenditure_Amt, This_Term_Min_Payment, This_Term_Total_Amt_Receivable, This_Term_Cash_Advance_Amt from Fubon_Ploan_Test_2005.dbo.cc_credit_card_statements" queryout c:\fuhwa\fubon\import\cc_credit_card_statements.csv -c -t , -U jcic -P jcic -S giza
bcp "select DOB, Education_Code, Gender_Code, Party_ID from Fubon_Ploan_Test_2005.dbo.cc_party_bank_credit_card" queryout c:\fuhwa\fubon\import\cc_party_bank_credit_card.csv -c -t , -U jcic -P jcic -S giza
bcp "select * from Fubon_Ploan_Test_2005.dbo.Fubon_response_score" queryout c:\fuhwa\fubon\result\Fubon_response_score.csv -c -t , -U jcic -P jcic -S giza


