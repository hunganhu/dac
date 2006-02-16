connect to ESCORE user escorap1 using escorap1;

-- drop table aas001;
-- drop table aps001;
-- drop table bas001;
-- drop table bam086;
-- drop table bam302;
-- drop table bam303;
-- drop table bam304;
-- drop table dam103;
-- drop table dam203;
-- drop table das006;
-- drop table krm001;
-- drop table krm023;
-- drop table stm001;
-- drop table vam102;
-- drop table app_info;


-- create JCIC tables
create table AAS001 (
	CASE_SN		    char(12) not null,
	inquiry_date	    char(10) not null,
	IDN		    char(10) not null,
	ID		    char(10),
	PNAME               Char(40),
	IS_LOST             Char(1),
	NULL2               Char(7),
	IS_FAKE_DATE        Char(1),
	FAKE_DATE           Char(7)
) IN "ESCORETABSP";
create index i_AAS001 on AAS001(CASE_SN, IDN);

COMMENT ON TABLE AAS001 IS '�۵M�H�m�W,�����ҫ_�� (�浧)';
COMMENT ON COLUMN AAS001.CASE_SN	IS '�ӽнs��';
COMMENT ON COLUMN AAS001.inquiry_date	IS 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
COMMENT ON COLUMN AAS001.IDN 		IS '�ӽФH�����Ҹ�';
COMMENT ON COLUMN AAS001.ID 		IS '�����Ҹ�';
COMMENT ON COLUMN AAS001.PNAME		IS '����m�W';
COMMENT ON COLUMN AAS001.IS_LOST	IS '�O�_�������Ҹɵo YN';
COMMENT ON COLUMN AAS001.NULL2		IS '�ť�';
COMMENT ON COLUMN AAS001.IS_FAKE_DATE	IS '�O�_�������ҫ_�� YN';
COMMENT ON COLUMN AAS001.FAKE_DATE	IS '�����ҫ_�Τ��';

create table APS001 (
	CASE_SN		    char(12) not null,
	inquiry_date	    char(10) not null,
	IDN		    char(10) not null,
	RGY_ADDRESS         Char(66),
	ENG_NAME            Char(20),
	BIRTHDAY            Char(7)
) IN "ESCORETABSP";
create index i_aps001 on APS001(CASE_SN, IDN);

COMMENT ON TABLE APS001	IS '�۵M�H�ӤH��� (�浧 J01)';
COMMENT ON COLUMN APS001.CASE_SN	IS '�ӽнs��';
COMMENT ON COLUMN APS001.inquiry_date	IS 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
COMMENT ON COLUMN APS001.IDN 		IS '�ӽФH�����Ҹ�';
COMMENT ON COLUMN APS001.RGY_ADDRESS	IS '���y�a�}';
COMMENT ON COLUMN APS001.ENG_NAME	IS '�^��m�W';
COMMENT ON COLUMN APS001.BIRTHDAY	IS '�X�ͤ��';

create table BAS001 (
	CASE_SN		    char(12) not null,
	inquiry_date	    char(10) not null,
	IDN		    char(10) not null,
	EVER_DELINQUENT     Char(1),
	DATA_YYYMMDD        Char(7)
) IN "ESCORETABSP";
create index i_BAS001 on BAS001(CASE_SN, IDN);

COMMENT ON TABLE BAS001 IS '�«H���`�����]�T�~���O�ʩΤ��~���b�b�^';
COMMENT ON COLUMN BAS001.CASE_SN	IS '�ӽнs��';
COMMENT ON COLUMN BAS001.inquiry_date	IS 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
COMMENT ON COLUMN BAS001.IDN	 	IS '�ӽФH�����Ҹ�';
COMMENT ON COLUMN BAS001.EVER_DELINQUENT	IS '�«H���` YN';
COMMENT ON COLUMN BAS001.DATA_YYYMMDD	IS '�̪�«H���`��� YYYMMDD';


create table BAM086 (
	CASE_SN		    char(12) not null,
	inquiry_date	    char(10) not null,
	IDN		    char(10) not null,
	DATA_YYY            Char(3),
	DATA_MM             Char(2),
	BANK_CODE           Char(7),
	BANK_NAME           Char(40),
	ACCOUNT_CODE        Char(1),
	ACCOUNT_CODE2       Char(1),
	PURPOSE_CODE        Char(1),
	CONTRACT_AMT        char(10),
	LOAN_AMT            char(10),
	PASS_DUE_AMT        char(10),
	PAY_CODE_12         Char(12),
	CO_LOAN             Char(1),
	UN_MARK             Char(1),
	U_YYYMMDD           Char(8),
	U_RATE              char(3),
	IB_MARK             Char(1),
	IAB_BAN             Char(8),
	IAB_NAME            Char(60),
	R_YYYMMDD           Char(7),
	REFUND_AMT          char(10),
	CK_REF              Char(1)
) IN "ESCORETABSP";
create index i_BAM086 on BAM086(CASE_SN, IDN);

COMMENT ON TABLE BAM086 IS '�«H��Ʃ��Ӥ��ٴڬ�����T-��w�O (�h�� B32,B63-B64)';
COMMENT ON COLUMN BAM086.CASE_SN	IS '�ӽнs��';
COMMENT ON COLUMN BAM086.inquiry_date	IS 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
COMMENT ON COLUMN BAM086.IDN	 	IS '�ӽФH�����Ҹ�';
COMMENT ON COLUMN BAM086.DATA_YYY 	IS '��Ʀ~��';
COMMENT ON COLUMN BAM086.DATA_MM 	IS '��Ƥ��';
COMMENT ON COLUMN BAM086.BANK_CODE	IS '��w�N��';
COMMENT ON COLUMN BAM086.BANK_NAME 	IS '��w�W��';
COMMENT ON COLUMN BAM086.ACCOUNT_CODE	IS '�|�p���  (��Ӫ�)';
COMMENT ON COLUMN BAM086.ACCOUNT_CODE2	IS '�|�p��ص��O  S ��ܦ���O';
COMMENT ON COLUMN BAM086.PURPOSE_CODE	IS '�γ~�O  (��Ӫ�)';
COMMENT ON COLUMN BAM086.CONTRACT_AMT	IS '�q�����B (�d��)';
COMMENT ON COLUMN BAM086.LOAN_AMT	IS '��ڥ��O�� (�d��)';
COMMENT ON COLUMN BAM086.PASS_DUE_AMT	IS '�O�����٪��B (�d��)';
COMMENT ON COLUMN BAM086.PAY_CODE_12	IS '�̪�12�Ӥ��ٴڬ���  (��Ӫ�)';
COMMENT ON COLUMN BAM086.CO_LOAN	IS '�@�P�ɴڵ��O  * ��ܦ��@�P�ɴ�';
COMMENT ON COLUMN BAM086.UN_MARK 	IS '����p�U���O  * ��ܬ�����p�U';
COMMENT ON COLUMN BAM086.U_YYYMMDD	IS '�p�U���  �褸YYYYMMDD';
COMMENT ON COLUMN BAM086.U_RATE		IS '�ѶU���';
COMMENT ON COLUMN BAM086.IB_MARK 	IS '����y�V���O  * ��ܬ�����y�V�D�k�H��´';
COMMENT ON COLUMN BAM086.IAB_BAN	IS '�D�k�H��´�νs';
COMMENT ON COLUMN BAM086.IAB_NAME	IS '�D�k�H��´�W��';
COMMENT ON COLUMN BAM086.R_YYYMMDD	IS '�ٴڤ��';
COMMENT ON COLUMN BAM086.REFUND_AMT	IS '�ٴڪ��B (�d��)';
COMMENT ON COLUMN BAM086.CK_REF		IS '�ٴڰO��  Y:�����ٲM N:�����ٲM 1.�ٲM 2.�M�v�n���� 3.�M�v�n��Q�� 4.�ॿ�`';

create table BAM302 (
	CASE_SN		    char(12) not null,
	inquiry_date	    char(10) not null,
	IDN		    char(10) not null,
	DATA_YYY            Char(3),
	DATA_MM             Char(2),
	BANK_CODE           Char(7),
	BANK_NAME           Char(40),
	ACCOUNT_CODE        Char(1),
	ACCOUNT_CODE2       Char(1),
	PURPOSE_CODE        Char(1),
	GID_CODE            Char(1),
	M_ID                Char(10),
	M_ID_NAME           Char(60),
	REL_CODE            Char(2),
	CONTRACT_AMT        char(10),
	LOAN_AMT            char(10),
	PASS_DUE_AMT        char(10)
) IN "ESCORETABSP";
create index i_BAM302 on BAM302(CASE_SN, IDN);


COMMENT ON TABLE BAM302 IS '�«H�O�Ҹ��(B31���@�P�Űȸ�� (�h�� B31-B33)';
COMMENT ON COLUMN BAM302.CASE_SN	IS '�ӽнs��';
COMMENT ON COLUMN BAM302.IDN 		IS '�ӽФH�����Ҹ�';
COMMENT ON COLUMN BAM302.inquiry_date	IS 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
COMMENT ON COLUMN BAM302.DATA_YYY	IS '��Ʀ~��';
COMMENT ON COLUMN BAM302.DATA_MM	IS '��Ƥ��';
COMMENT ON COLUMN BAM302.BANK_CODE 	IS '��w�N��';
COMMENT ON COLUMN BAM302.BANK_NAME	IS '��w�W��';
COMMENT ON COLUMN BAM302.ACCOUNT_CODE	IS '�|�p���  (��Ӫ�)';
COMMENT ON COLUMN BAM302.ACCOUNT_CODE2	IS '�|�p���O  S ��ܦ���O';
COMMENT ON COLUMN BAM302.PURPOSE_CODE	IS '�γ~�O  (��Ӫ�)';
COMMENT ON COLUMN BAM302.GID_CODE 	IS '�����N��  (��Ӫ�)';
COMMENT ON COLUMN BAM302.M_ID		IS '�D�ŰȤH�����Ҹ�';
COMMENT ON COLUMN BAM302.M_ID_NAME 	IS '�D�ŰȤH�W��';
COMMENT ON COLUMN BAM302.REL_CODE	IS '���Y�N��  (��Ӫ�)';
COMMENT ON COLUMN BAM302.CONTRACT_AMT	IS '�q�����B (�d��)';
COMMENT ON COLUMN BAM302.LOAN_AMT	IS '��ڥ��O�� (�d��)';
COMMENT ON COLUMN BAM302.PASS_DUE_AMT	IS '�O�����٪��B (�d��)';

create table BAM303 (
	CASE_SN		    char(12) not null,
	inquiry_date	    char(10) not null,
	IDN		    char(10) not null,
	DATA_YYY            Char(3),
	DATA_MM             Char(2),
	BANK_CODE           Char(7),
	BANK_NAME           Char(40),
	ACCOUNT_CODE        Char(1),
	ACCOUNT_CODE2       Char(1),
	PURPOSE_CODE        Char(1),
	GID_CODE            Char(1),
	M_ID                Char(10),
	M_ID_NAME           Char(60),
	REL_CODE            Char(2),
	CONTRACT_AMT        char(10),
	LOAN_AMT            char(10),
	PASS_DUE_AMT        char(10)
) IN "ESCORETABSP";
create index i_BAM303 on BAM303(CASE_SN, IDN);

COMMENT ON TABLE BAM303	IS '�«H�O�Ҹ��(B31���@�P�Űȸ�� (�h�� B31-B33)';
COMMENT ON COLUMN BAM303.CASE_SN	IS '�ӽнs��';
COMMENT ON COLUMN BAM303.IDN	 	IS '�ӽФH�����Ҹ�';
COMMENT ON COLUMN BAM303.inquiry_date	IS 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
COMMENT ON COLUMN BAM303.DATA_YYY 	IS '��Ʀ~��';
COMMENT ON COLUMN BAM303.DATA_MM 	IS '��Ƥ��';
COMMENT ON COLUMN BAM303.BANK_CODE	IS '��w�N��';
COMMENT ON COLUMN BAM303.BANK_NAME	IS '��w�W��';
COMMENT ON COLUMN BAM303.ACCOUNT_CODE	IS '�|�p���  (��Ӫ�)';
COMMENT ON COLUMN BAM303.ACCOUNT_CODE2	IS '�|�p���O  S ��ܦ���O';
COMMENT ON COLUMN BAM303.PURPOSE_CODE	IS '�γ~�O  (��Ӫ�)';
COMMENT ON COLUMN BAM303.GID_CODE	IS '�����N��  (��Ӫ�)';
COMMENT ON COLUMN BAM303.M_ID	IS '�D�ŰȤH�����Ҹ�';
COMMENT ON COLUMN BAM303.M_ID_NAME	IS '�D�ŰȤH�W��';
COMMENT ON COLUMN BAM303.REL_CODE	IS '���Y�N��  (��Ӫ�)';
COMMENT ON COLUMN BAM303.CONTRACT_AMT	IS '�q�����B (�d��)';
COMMENT ON COLUMN BAM303.LOAN_AMT	IS '��ڥ��O�� (�d��)';
COMMENT ON COLUMN BAM303.PASS_DUE_AMT	IS '�O�����٪��B (�d��)';


create table BAM304 (
	CASE_SN		    char(12) not null,
	inquiry_date	    char(10) not null,
	IDN		    char(10) not null,
	DATA_YYY            Char(3),
	DATA_MM             Char(2),
	BANK_CODE           Char(7),
	BANK_NAME           Char(40),
	ACCOUNT_CODE        Char(1),
	ACCOUNT_CODE2       Char(1),
	PURPOSE_CODE        Char(1),
	GID_CODE            Char(1),
	M_ID                Char(10),
	M_ID_NAME           Char(60),
	REL_CODE            Char(2),
	CONTRACT_AMT        char(10),
	LOAN_AMT            char(10),
	PASS_DUE_AMT        char(10)
) IN "ESCORETABSP";
create index i_BAM304 on BAM304(CASE_SN, IDN);

COMMENT ON TABLE BAM304	IS '�«H�O�Ҹ��(B31����L�Űȸ��) (�h�� B31-B33)';
COMMENT ON COLUMN BAM304.CASE_SN	IS '�ӽнs��';
COMMENT ON COLUMN BAM304.IDN	 	IS '�ӽФH�����Ҹ�';
COMMENT ON COLUMN BAM304.inquiry_date	IS 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
COMMENT ON COLUMN BAM304.DATA_YYY	IS '��Ʀ~��';
COMMENT ON COLUMN BAM304.DATA_MM 	IS '��Ƥ��';
COMMENT ON COLUMN BAM304.BANK_CODE	IS '��w�N��';
COMMENT ON COLUMN BAM304.BANK_NAME	IS '��w�W��';
COMMENT ON COLUMN BAM304.ACCOUNT_CODE	IS '�|�p���  (��Ӫ�)';
COMMENT ON COLUMN BAM304.ACCOUNT_CODE2	IS '�|�p���O  S ��ܦ���O';
COMMENT ON COLUMN BAM304.PURPOSE_CODE	IS '�γ~�O  (��Ӫ�)';
COMMENT ON COLUMN BAM304.GID_CODE 	IS '�����N��  (��Ӫ�)';
COMMENT ON COLUMN BAM304.M_ID		IS '�D�ŰȤH�����Ҹ�';
COMMENT ON COLUMN BAM304.M_ID_NAME	IS '�D�ŰȤH�W��';
COMMENT ON COLUMN BAM304.REL_CODE	IS '���Y�N��  (��Ӫ�)';
COMMENT ON COLUMN BAM304.CONTRACT_AMT	IS '�q�����B (�d��)';
COMMENT ON COLUMN BAM304.LOAN_AMT	IS '��ڥ��O�� (�d��)';
COMMENT ON COLUMN BAM304.PASS_DUE_AMT 	IS '�O�����٪��B (�d��)';

create table DAM103 (
	CASE_SN		    char(12) not null,
	inquiry_date	    char(10) not null,
	IDN		    char(10) not null,
	CK_DATE             Char(7),
	ISSUE_BANK          Char(7),
	BANK_NAME           Char(40),
	ACCNT_NO            Char(9),
	CK_TYPE             Char(1),
	CHECK_NO            Char(9),
	AMT                 char(10),
	CANCEL_DATE         Char(7),
	COMPANY_BOUNCE      Char(1),
	RESP_ID             Char(10),
	RESP_NAME           Char(60)
) IN "ESCORETABSP";
create index i_DAM103 on DAM103(CASE_SN, IDN);

COMMENT ON TABLE DAM103	IS '�h�� (�h�� D01 D03, ���NDAM102)';
COMMENT ON COLUMN DAM103.CASE_SN	IS '�ӽнs��';
COMMENT ON COLUMN DAM103.inquiry_date	IS 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
COMMENT ON COLUMN DAM103.IDN 		IS '�ӽФH�����Ҹ�';
COMMENT ON COLUMN DAM103.CK_DATE 	IS '�h�����';
COMMENT ON COLUMN DAM103.ISSUE_BANK 	IS '�h����w';
COMMENT ON COLUMN DAM103.BANK_NAME 	IS '��w�W��';
COMMENT ON COLUMN DAM103.ACCNT_NO 	IS '�b��';
COMMENT ON COLUMN DAM103.CK_TYPE 	IS '���ں��� C=�䲼,N=����,R=�ײ�,O=��L';
COMMENT ON COLUMN DAM103.CHECK_NO 	IS '�䲼���X';
COMMENT ON COLUMN DAM103.AMT	 	IS '���B (��)';
COMMENT ON COLUMN DAM103.CANCEL_DATE 	IS '�M�v���O���';
COMMENT ON COLUMN DAM103.COMPANY_BOUNCE	IS '���q�h�� YN';
COMMENT ON COLUMN DAM103.RESP_ID 	IS '���q�νs�έt�d�H�����Ҹ�';
COMMENT ON COLUMN DAM103.RESP_NAME 	IS '���q�W�٩έt�d�H�m�W';

create table DAM203 (
	CASE_SN		    char(12) not null,
	inquiry_date	    char(10) not null,
	IDN		    char(10) not null,
	BEG_DATE            Char(7),
	END_DATE            Char(7),
	RSN                 Char(1),
	SOURCE              Char(1),
	UPD_DATE            Char(7),
	RELIEF              Char(1),
	S_BAN               Char(8),
	S_BAN_NAME          Char(60)
) IN "ESCORETABSP";
create index i_DAM203 on DAM203(CASE_SN, IDN);

COMMENT ON TABLE DAM203 IS '�ک� (�h�� D01 D03, ���NDAM202)';
COMMENT ON COLUMN DAM203.CASE_SN	IS '�ӽнs��';
COMMENT ON COLUMN DAM203.IDN 		IS '�ӽФH�����Ҹ�';
COMMENT ON COLUMN DAM203.inquiry_date	IS 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
COMMENT ON COLUMN DAM203.BEG_DATE	IS '�ک��}�l���';
COMMENT ON COLUMN DAM203.END_DATE	IS '�ک��Ѱ����  ������ΤC�� 0 ��ܤw�Ѱ�';
COMMENT ON COLUMN DAM203.RSN		IS '�ک��Ѱ��z��  1:�M�v�Ѱ� 2:�Ȥ��Ѱ�';
COMMENT ON COLUMN DAM203.SOURCE		IS '��ƨӷ�  1:��ƤH�ӽ� 2:����Ҥ��i';
COMMENT ON COLUMN DAM203.UPD_DATE	IS '��Ƨ�s���';
COMMENT ON COLUMN DAM203.RELIEF		IS '�ک��Ѱ����p  A:�w�Ѱ�';
COMMENT ON COLUMN DAM203.S_BAN		IS '�ӤH����D�k�H��´���νs�]�W��Χt�١^';
COMMENT ON COLUMN DAM203.S_BAN_NAME	IS '�ӤH����D�k�H��´���νs�W��';

create table DAS006 (
	CASE_SN		    char(12) not null,
	inquiry_date	    char(10) not null,
	IDN		    char(10) not null,
	BOUNCE_AMT          char(12),
	BOUNCE_CNT          char(10),
	LATEST_BOUNCE       Char(7),
	WRITEOFF_AMT        char(12),
	WRITEOFF_CNT        char(10),
	LATEST_WRITEOFF     Char(7),
	IDN_BOUNCE_AMT      char(12),
	IDN_BOUNCE_CNT      char(10),
	IDN_LATEST_BOUNCE   Char(7),
	IDN_WRITEOFF_AMT    char(12),
	IDN_WRITEOFF_CNT    char(10),
	IDN_LATEST_WRITEOFF Char(7)
) IN "ESCORETABSP";
create index i_DAS006 on DAS006(CASE_SN, IDN);

COMMENT ON TABLE DAS006 IS '�D��s�a�h���K�n���� (�浧�K�n)';
COMMENT ON COLUMN DAS006.CASE_SN	IS '�ӽнs��';
COMMENT ON COLUMN DAS006.IDN		IS '�ӽФH�����Ҹ�';
COMMENT ON COLUMN DAS006.inquiry_date	IS 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
COMMENT ON COLUMN DAS006.BOUNCE_AMT	IS '�D��h�����M�v���O�`���B (�d��)';
COMMENT ON COLUMN DAS006.BOUNCE_CNT	IS '�D��h�����M�v���O�`�i��';
COMMENT ON COLUMN DAS006.LATEST_BOUNCE	IS '�D��̪�@���h�����';
COMMENT ON COLUMN DAS006.WRITEOFF_AMT	IS '�D��h���w�M�v���O�`���B (�d��)';
COMMENT ON COLUMN DAS006.WRITEOFF_CNT	IS '�D��h���w�M�v���O���`�i��';
COMMENT ON COLUMN DAS006.LATEST_WRITEOFF	IS '�D��̪�@���w�M�v���O���P���';
COMMENT ON COLUMN DAS006.IDN_BOUNCE_AMT	IS '�s�a�h�����M�v���O�`���B (�d��)';
COMMENT ON COLUMN DAS006.IDN_BOUNCE_CNT	IS '�s�a�h�����M�v���O�`�i��';
COMMENT ON COLUMN DAS006.IDN_LATEST_BOUNCE	IS '�s�a�̪�@���h�����';
COMMENT ON COLUMN DAS006.IDN_WRITEOFF_AMT	IS '�s�a�h���w�M�v���O�`���B (�d��)';
COMMENT ON COLUMN DAS006.IDN_WRITEOFF_CNT 	IS '�s�a�h���w�M�v���O���`�i��';
COMMENT ON COLUMN DAS006.IDN_LATEST_WRITEOFF	IS '�s�a�̪�@���w�M�v���O���P���';

create table jas002 (
	CASE_SN		char(12) not null,
	inquiry_date	char(10) not null,
	IDN		char(10) not null,
	ever_delinquent char(1),
	delinquent_date char(7),
	ever_bad_check  char(1),
	bad_check_date  char(7),
	ever_reject     char(1),
	reject_date     char(7),
	ever_stop_card  char(1),
	stop_card_date  char(7)
) IN "ESCORETABSP";
create index i_jas002 on jas002(CASE_SN, IDN);

COMMENT ON TABLE  jas002	IS '�H�β��`���� (B, D, K)';
COMMENT ON COLUMN jas002.CASE_SN	IS '�ӽнs��';
COMMENT ON COLUMN jas002.IDN	 	IS '�ӽФH�����Ҹ�'; 
COMMENT ON COLUMN jas002.inquiry_date	IS 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
COMMENT ON COLUMN jas002.ever_delinquent	IS '�«H���` YN';
COMMENT ON COLUMN jas002.delinquent_date	IS '�̪�«H���`���';
COMMENT ON COLUMN jas002.ever_bad_check	IS '�h�����` YN';
COMMENT ON COLUMN jas002.bad_check_date	IS '�̪�h�����`���';
COMMENT ON COLUMN jas002.ever_reject	IS '�ڵ����ӰO�� YN';
COMMENT ON COLUMN jas002.reject_date	IS '�ڵ����Ӥ��  �ڵ����ӰO��N����즳������Ӥ�ک��w�Ѱ�����������ڵ����Ӥ��';
COMMENT ON COLUMN jas002.ever_stop_card	IS '�j��d���` YN';
COMMENT ON COLUMN jas002.stop_card_date	IS '�̪�j��d���';

create table KRM001 (
	CASE_SN		char(12) not null,
	inquiry_date	char(10) not null,
	IDN		char(10) not null,
	Card_Brand 	char (1),
	Card_Type 	char (1),
	Issue 		char (3),
	Issue_Name 	char (40),
	Start_Date 	char (7),
	Stop_Date 	char (7),
	Stop_Code 	char (1),
	AB_Code 	char (1),
	M_S 		char (1),
	Limit 		char (6),
	LIMIT_TYPE	char (1),
	USAGE_TYPE	char (1),
	SECURE		char (1)
) IN "ESCORETABSP";
create index i_krm001 on KRM001(CASE_SN, IDN);

COMMENT ON TABLE KRM001	IS '�H�Υd��T (�h�� K01)';
COMMENT ON COLUMN KRM001.CASE_SN	IS '�ӽнs��';
COMMENT ON COLUMN KRM001.IDN	 	IS '�ӽФH�����Ҹ�';
COMMENT ON COLUMN KRM001.inquiry_date	IS 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
COMMENT ON COLUMN KRM001.CARD_BRAND	IS '�d�W�N��  (��Ӫ�)';
COMMENT ON COLUMN KRM001.CARD_TYPE	IS '�г��N��  (��Ӫ�)';
COMMENT ON COLUMN KRM001.ISSUE		IS '�o�d���c�N��';
COMMENT ON COLUMN KRM001.ISSUE_NAME	IS '�o�d���c�W��';
COMMENT ON COLUMN KRM001.START_DATE	IS '�ҥΤ��';
COMMENT ON COLUMN KRM001.STOP_DATE	IS '���Τ��';
COMMENT ON COLUMN KRM001.STOP_CODE	IS '���κ����N��  (��Ӫ�)';
COMMENT ON COLUMN KRM001.AB_CODE 	IS '���έ�]�N��  (��Ӫ�)';
COMMENT ON COLUMN KRM001.M_S		IS '�D���d (YN)';
COMMENT ON COLUMN KRM001.LIMIT		IS '�B�ס]�d���^';
COMMENT ON COLUMN KRM001.LIMIT_TYPE	IS '�O�_��X�B�� (YN)';
COMMENT ON COLUMN KRM001.USAGE_TYPE 	IS '��_�`���ϥ� (YN)';
COMMENT ON COLUMN KRM001.SECURE		IS '��O���O (YN)';


create table KRM023 (
	CASE_SN		char(12) not null,
	inquiry_date	char(10) not null,
	IDN		char(10) not null,
	Yrmon 		char (5),
	Issue 		char (3),
	Issue_Name 	char (40),
	KR_Code 	char (7),
	Limit 		char (5),
	Payment 	char (3),
	Cash 		char (1),
	Pay_code 	char (1)
) IN "ESCORETABSP";
create index i_krm023 on KRM023(CASE_SN, IDN);

COMMENT ON TABLE KRM023	IS '�H�Υd��ú�ڸ�T�]�̪񢰢���^ (�h�� K23)';
COMMENT ON COLUMN KRM023.CASE_SN	IS '�ӽнs��';
COMMENT ON COLUMN KRM023.IDN 		IS '�ӽФH�����Ҹ�';
COMMENT ON COLUMN KRM023.inquiry_date	IS 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
COMMENT ON COLUMN KRM023.YRMON 		IS '��Ʀ~��';
COMMENT ON COLUMN KRM023.ISSUE		IS '�o�d���c�N��';
COMMENT ON COLUMN KRM023.ISSUE_NAME	IS '�o�d���c�W��';
COMMENT ON COLUMN KRM023.KR_CODE	IS '�H�Υd�O';
COMMENT ON COLUMN KRM023.LIMIT 		IS '�֩w�B�� (�d��)';
COMMENT ON COLUMN KRM023.PAYMENT	IS '��ú���B�ŶZ(��)(���+ +HML)';
COMMENT ON COLUMN KRM023.CASH 		IS '�w�ɲ{�����L(YN)';
COMMENT ON COLUMN KRM023.PAY_CODE	IS 'ú�ڪ��p�N��  (��Ӫ�)';


create table KRM037 (
	CASE_SN		char(12) not null,
	inquiry_date	char(10) not null,
	IDN		char(10) not null,
	idn_ban		char(10),
	bill_date	char(7),
	issue		char(3),
	issue_name 	char(24),
	card_type 	char(7),
	perm_limit 	char(7),
	temp_limit 	char(7),
	cash_limit 	char(7),
	payable 	char(3),
	cash_lent 	char(8),
	last_paya 	char(3),
	revol_bal 	char(9),
	pay_stat 	char(1),
	pay_code 	char(1),
	revol_rate 	char(4),
	pre_owed 	char(8),
	debt_code 	char(1),
	close_code 	char(1),
	clear_date 	char(7)
) IN "ESCORETABSP";
create index i_krm037 on krm037(CASE_SN, IDN);

COMMENT ON TABLE KRM037	IS '�H�Υd��b�ڤδ`����v��T�]�̪񢰢���^(�h�� K33,�J�B�צX�p��,��L��r�����ť�,�ƭ����m0)';
COMMENT ON COLUMN KRM037.CASE_SN	IS '�ӽнs��';
COMMENT ON COLUMN KRM037.IDN 		IS '�ӽФH�����Ҹ�';
COMMENT ON COLUMN KRM037.inquiry_date	IS 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
COMMENT ON COLUMN KRM037.idn_ban 	IS 'ú�ڤHID';
COMMENT ON COLUMN KRM037.bill_date	IS '���b��';
COMMENT ON COLUMN KRM037.issue		IS '�o�d���c�N��';
COMMENT ON COLUMN KRM037.issue_name	IS '�o�d���c�W��';
COMMENT ON COLUMN KRM037.card_type	IS '�H�Υd�O';
COMMENT ON COLUMN KRM037.perm_limit	IS '�ä[�B��  ���:�d��';
COMMENT ON COLUMN KRM037.temp_limit	IS '�{���B��  ���:�d��';
COMMENT ON COLUMN KRM037.cash_limit	IS '�w�ɲ{���B��  ���:�d��';
COMMENT ON COLUMN KRM037.payable	IS '�������I�b��(�ŶZ)';
COMMENT ON COLUMN KRM037.cash_lent	IS '�����w�ɲ{��  ���:��';
COMMENT ON COLUMN KRM037.last_paya 	IS '�W�����I�b��(�ŶZ)';
COMMENT ON COLUMN KRM037.revol_bal	IS '�W���`���H��  ���:��';
COMMENT ON COLUMN KRM037.pay_stat	IS '�W��ú�ڪ��p�N��(���B)  (��Ӫ�)';
COMMENT ON COLUMN KRM037.pay_code	IS '�W��ú�ڪ��p�N��(�ɶ�)  (��Ӫ�)';
COMMENT ON COLUMN KRM037.revol_rate	IS '�W���`����vREVOL_BAL/PERM_LIMIT,��T�צܤp���I��2��(�|�ˤ��J),�D�ʤ���';
COMMENT ON COLUMN KRM037.pre_owed	IS '����������v�٫ݥI���B,������N�Ԯ��O�ڪ��B(�榡22�涵)+����������v�ٹw�ɲ{�����B(�榡23�涵),���]�A�����s�W�l�B�N�v(18��)';
COMMENT ON COLUMN KRM037.debt_code 	IS '���v���A���O';
COMMENT ON COLUMN KRM037.close_code	IS '���v���׵��O(��Ӫ�)';
COMMENT ON COLUMN KRM037.clear_date	IS '���}���v���פ��';

create table STM001 (
	CASE_SN		char(12) not null,
	inquiry_date	char(10) not null,
	IDN		char(10) not null,
	Query_Date 	char(7),
	Bank_Code 	char(7),
	Bank_Name 	char(40),
	Item_List 	char(10)
) IN "ESCORETABSP";
create index i_stm001 on STM001(CASE_SN, IDN);

COMMENT ON TABLE STM001 IS '�Q�d�߬��� (�h��)';
COMMENT ON COLUMN STM001.CASE_SN	IS '�ӽнs��';
COMMENT ON COLUMN STM001.IDN		IS '�ӽФH�����Ҹ�';
COMMENT ON COLUMN STM001.inquiry_date	IS 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
COMMENT ON COLUMN STM001.QUERY_DATE	IS '�d�ߤ��';
COMMENT ON COLUMN STM001.BANK_CODE	IS '�d�߳��N��';
COMMENT ON COLUMN STM001.BANK_NAME	IS '�d�߳��W��';
COMMENT ON COLUMN STM001.ITEM_LIST	IS '�d�߶��ئ�C B:�«H D:���H K:�H�Υd��';


create table STM007 (
	CASE_SN		char(12) not null,
	inquiry_date	char(10) not null,
	IDN		char(10) not null,
	query_date 	char(7),
 	bank_code 	char(7),
	bank_name 	char(40),
	item_list 	char(10),
	inq_purpose_1	char(1),
	inq_purpose	char(30)

) IN "ESCORETABSP";
create index i_stm007 on STM007(CASE_SN, IDN);

COMMENT ON TABLE STM007 IS '�̪�T�Ӥ뤺�DZ�����~�Q�d�߬��� (�h��)';
COMMENT ON COLUMN STM007.CASE_SN	IS '�ӽнs��';
COMMENT ON COLUMN STM007.IDN		IS '�ӽФH�����Ҹ�';
COMMENT ON COLUMN STM007.inquiry_date	IS 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
COMMENT ON COLUMN STM007.query_date	IS '�d�ߤ��';
COMMENT ON COLUMN STM007.bank_code	IS '�d�߳��N��';
COMMENT ON COLUMN STM007.bank_name	IS '�d�߳��W��';
COMMENT ON COLUMN STM007.item_list	IS '�d�߶��ئ�C B:�«H D:���H K:�H�Υd��';
COMMENT ON COLUMN STM007.inq_purpose_1	IS '�d�߲z�ѽX';
COMMENT ON COLUMN STM007.inq_purpose 	IS '�d�߲z�ѽX(�������)1:�s�~�ȥӽ�2:��~�ȩ���3:�s�~�ȥӽФέ�~�ȩ���';

create table VAM102 (
	CASE_SN		char(12) not null,
	inquiry_date	char(10) not null,
	IDN		char(10) not null,
	IDN_BAN 	char(10),
	DATA_DATE 	char(7),
	MAINCODE 	char(1),
	MAINNOTE 	char(36),
	SUBCODE 	char(1),
	SUBNOTE 	char(60),
	NOTES	 	char(100)
) IN "ESCORETABSP";
create index i_vam102 on VAM102(CASE_SN, IDN);

COMMENT ON TABLE VAM102 IS '�ɥR/���O��T';
COMMENT ON COLUMN VAM102.CASE_SN	IS '�ӽнs��';
COMMENT ON COLUMN VAM102.IDN		IS '�ӽФH�����Ҹ�';
COMMENT ON COLUMN VAM102.inquiry_date	IS 'JCIC ��Ƭd�ߤ��(yyy/mm/dd, yyy������~)';
COMMENT ON COLUMN VAM102.IDN_BAN	IS '�νs/�����Ҹ�';
COMMENT ON COLUMN VAM102.DATA_DATE	IS '�T���n�����';
COMMENT ON COLUMN VAM102.MAINCODE	IS '�T�������j���N�X  (��Ӫ�)';
COMMENT ON COLUMN VAM102.MAINNOTE	IS '�T�������j��';
COMMENT ON COLUMN VAM102.SUBCODE	IS '�T�������Ӷ��N�X  (��Ӫ�)';
COMMENT ON COLUMN VAM102.SUBNOTE	IS '�T�������Ӷ�';
COMMENT ON COLUMN VAM102.NOTES		IS '�T�����e';

-- create INPUT tables
create table app_info (
	Case_SN		char(12) not null,
	system_date	char(7) not null,
	system_time	char(6) not null,
	Applicant_ID	char(10) not null,
	applicant_name	varchar(30) not null,
	Guarantor_ID	char(10),
	Loan_purpose	int not null,
	education	int not null,
	Birthday	char(7) not null,
	Marriage_status	int not null,
	Annual_Income	int not null,
	Home_ownership	int not null,
	Year_at_current_address	int not null,
	Car_brand	varchar(20),
	Car_year	int,
	Car_cc		int,
	Company_name	varchar(60) not null,
	Job_title	varchar(40) not null,
	Year_at_current_job	int not null,
	Product_type_code	int not null,
	App_amt		int not null,
	Term_1		int not null,
	Term_2		int,
	Term_3		int,
	APR_1		decimal(7,4) not null,
	APR_2		decimal(7,4),
	APR_3		decimal(7,4),
	grace_period	int not null,
	Fee_1		int,
	Fee_2		decimal(7,4),
	system_date_final	char(7),
	system_time_final	char(6),
	Approval_final	int,
	Account_No	varchar(20),
	Open_date	char(7),
	Loan_amt	int,
	Loan_APR	decimal(7,4),
	Loan_term	int,
	Fee_1_final	int,
	Fee_2_final	decimal(7,4),
	NPV_final	int,
	CI_key		char(10),
	CI_name		varchar(30),
	CI_branch	varchar(20),
	branch		varchar(20) not null,
	agent		varchar(30) not null,
	errcode		char(4),
	agent_id	varchar(10) not null
) IN "ESCORETABSP";
alter table app_info add constraint p_app_info primary key (CASE_SN);

COMMENT ON TABLE app_info	IS 'DAC�ӤH�H�U���';
COMMENT ON COLUMN app_info.CASE_SN	IS '�ӽнs��(���i����)';
COMMENT ON COLUMN app_info.system_date	IS '�ӽп�J�t�Τ��';
COMMENT ON COLUMN app_info.system_time	IS '�ӽп�J�t�ήɶ�';
COMMENT ON COLUMN app_info.Applicant_ID	IS '�ӽФH�����Ҹ�';
COMMENT ON COLUMN app_info.applicant_name	IS '�ӽФH�m�W';
COMMENT ON COLUMN app_info.Guarantor_ID	IS '�O�ҤH�����Ҹ�';
COMMENT ON COLUMN app_info.Loan_purpose	IS '����γ~';
COMMENT ON COLUMN app_info.education	IS '�Ш|�{��';
COMMENT ON COLUMN app_info.Birthday	IS '�ӽФH�ͤ�';
COMMENT ON COLUMN app_info.Marriage_status	IS '�ӽФH�B�ê��A';
COMMENT ON COLUMN app_info.Annual_Income	IS '�ӽФH�~�ұo���J';
COMMENT ON COLUMN app_info.Home_ownership	IS '�ӽФH�{��ЫΪ��p';
COMMENT ON COLUMN app_info.Year_at_current_address	IS '�ӽФH�{�}�w�~��~��';
COMMENT ON COLUMN app_info.Car_brand	IS '�ӽФH���y���(�t�P)';
COMMENT ON COLUMN app_info.Car_year	IS '�ӽФH���y���(�~��)';
COMMENT ON COLUMN app_info.Car_cc	IS '�ӽФH���y���(�Ʈ�q)';
COMMENT ON COLUMN app_info.Company_name	IS '�ӽФH��¾���q';
COMMENT ON COLUMN app_info.Job_title	IS '�ӽФH¾��';
COMMENT ON COLUMN app_info.Year_at_current_job	IS '�ӽФH�{¾�A�Ȧ~��';
COMMENT ON COLUMN app_info.Product_type_code	IS '���~�����X';
COMMENT ON COLUMN app_info.App_amt	IS '�ӶU���B';
COMMENT ON COLUMN app_info.Term_1	IS '����(�Ĥ@��)';
COMMENT ON COLUMN app_info.Term_2	IS '����(�ĤG��)';
COMMENT ON COLUMN app_info.Term_3	IS '����(�ĤT��)';
COMMENT ON COLUMN app_info.APR_1	IS '�Q�v(�Ĥ@��)';
COMMENT ON COLUMN app_info.APR_2	IS '�Q�v(�ĤG��)';
COMMENT ON COLUMN app_info.APR_3	IS '�Q�v(�ĤT��)';
COMMENT ON COLUMN app_info.grace_period	IS '�e����';
COMMENT ON COLUMN app_info.Fee_1	IS '�}��O(�T�w���B����)';
COMMENT ON COLUMN app_info.Fee_2	IS '�}��O(�ʤ��񳡤�)';
COMMENT ON COLUMN app_info.system_date_final	IS '���׿�J�t�Τ��yyymmdd';
COMMENT ON COLUMN app_info.system_time_final	IS '���׿�J�t�ήɶ�hhmiss';
COMMENT ON COLUMN app_info.Approval_final	IS '�̲׭��/���ڨM�w';
COMMENT ON COLUMN app_info.Account_No	IS '���ڱb��';
COMMENT ON COLUMN app_info.Open_date	IS '���ڤ��';
COMMENT ON COLUMN app_info.Loan_amt	IS '���ڪ��B';
COMMENT ON COLUMN app_info.Loan_APR	IS '���ڧQ�v';
COMMENT ON COLUMN app_info.Loan_term	IS '�̲׶U�ڴ���';
COMMENT ON COLUMN app_info.Fee_1_final	IS '�ꦬ�}��O(�T�w���B����)';
COMMENT ON COLUMN app_info.Fee_2_final	IS '�ꦬ�}��O(�ʤ��񳡤�)';
COMMENT ON COLUMN app_info.NPV_final	IS '�w���g�ٻ���(�̹꼷���B����)';
COMMENT ON COLUMN app_info.CI_key	IS '�~�ȭ������Ҹ�';
COMMENT ON COLUMN app_info.CI_name	IS '�~�ȤH���m�W';
COMMENT ON COLUMN app_info.CI_branch	IS '�~�ȤH�����ݤ���';
COMMENT ON COLUMN app_info.branch	IS '�g�����';
COMMENT ON COLUMN app_info.agent	IS '�g�����m�W';
COMMENT ON COLUMN app_info.agent_id	IS '�g����ID';
COMMENT ON COLUMN app_info.errcode	IS '�ץ�d�ߵ��G';

-- create OUTPUT tables
create table app_result (
	Case_SN		char(12) not null,
	system_time	char(13),
	Applicant_PB	decimal(7,4),
	Guarantor_PB	decimal(7,4),
	Applicant_PB_adj	decimal(7,4),
	NPV		int,
	PS_code		int,
	PS_msg		varchar(256),
	PS_code_g	int,
	PS_msg_g	varchar(256),
	Guarantor_pass	int,
	Fund_deposit_pct	decimal(7,4),
	Fund_bank_pct	decimal(7,4),
	Fund_self_pct	decimal(7,4),
	Fund_free_pct	decimal(7,4),
	COF_deposit	decimal(7,4),
	COF_bank	decimal(7,4),
	ROE		decimal(7,4),
	Commission	decimal(7,4),
	Approval_code	int,
	Approval_msg	varchar(512),
	Lowest_Rate_1	decimal(7,4),
	Lowest_Rate_2	decimal(7,4),
	Lowest_Rate_3	decimal(7,4)
) IN "ESCORETABSP";
alter table app_result add constraint p_app_result primary key (CASE_SN);

COMMENT ON TABLE app_result	IS 'DAC�ӤH�H�U�Ҳյ��G���';
COMMENT ON COLUMN app_result.CASE_SN	IS '�ӽнs��(���i����)';
COMMENT ON COLUMN app_result.system_time	IS '�Ҳհ���t�ήɶ�';
COMMENT ON COLUMN app_result.Applicant_PB	IS '�ӽФH�w���a�b���v';
COMMENT ON COLUMN app_result.Guarantor_PB	IS '�O�ҤH�w���a�b���v';
COMMENT ON COLUMN app_result.Applicant_PB_adj	IS '�ӽФH�w���a�b���v(�վ��)';
COMMENT ON COLUMN app_result.NPV	IS '�w���g�ٻ���';
COMMENT ON COLUMN app_result.PS_code	IS '�ӽФH�w���z�ﵲ�G�X';
COMMENT ON COLUMN app_result.PS_msg	IS '�ӽФH�w���z�ﵲ�G�T��';
COMMENT ON COLUMN app_result.PS_code_g	IS '�O�ҤH�w���z�ﵲ�G�X';
COMMENT ON COLUMN app_result.PS_msg_g	IS '�O�ҤH�w���z�ﵲ�G�T��';
COMMENT ON COLUMN app_result.Guarantor_pass	IS '�O�ҤH�ŦX�H�μз�';
COMMENT ON COLUMN app_result.Fund_deposit_pct	IS '����ӷ��Ӧۦs�ڦʤ���';
COMMENT ON COLUMN app_result.Fund_bank_pct	IS '����ӷ��ӦۦP�~��ڦʤ���';
COMMENT ON COLUMN app_result.Fund_self_pct	IS '����ӷ��ۦ��ʤ���';
COMMENT ON COLUMN app_result.Fund_free_pct	IS '����ӷ��L���ʤ���';
COMMENT ON COLUMN app_result.COF_deposit	IS '�s�ڥ����Q�v';
COMMENT ON COLUMN app_result.COF_bank	IS '�P�~��ڧQ�v';
COMMENT ON COLUMN app_result.ROE	IS 'ROE';
COMMENT ON COLUMN app_result.Commission	IS '�~�ȼ���(% of loan Amt)';
COMMENT ON COLUMN app_result.Approval_code	IS '�Ҳխ���ĳ�X';
COMMENT ON COLUMN app_result.Approval_msg	IS '�Ҳխ���ĳ�T��';
COMMENT ON COLUMN app_result.Lowest_Rate_1	IS '�̧C�i�ӧ@�Q�v(�Ĥ@��)';
COMMENT ON COLUMN app_result.Lowest_Rate_2	IS '�̧C�i�ӧ@�Q�v(�ĤG��)';
COMMENT ON COLUMN app_result.Lowest_Rate_3	IS '�̧C�i�ӧ@�Q�v(�ĤT��)';

create table COUNTER (
	SEQ_TYPE	char(01) not null,
	SEQ_NO		numeric(10) not null
) IN "ESCORETABSP";
COMMENT ON TABLE  COUNTER		IS '�Ǹ���ƪ�';
COMMENT ON COLUMN COUNTER.SEQ_TYPE	IS '�Ǹ�����, W:WEB';
COMMENT ON COLUMN COUNTER.SEQ_NO	IS '�Ǹ���';


-- create FINANCIAL tables
create table TCB_FIN_INFO (
	Fund_deposit_pct	decimal(7,4),
	Fund_bank_pct	decimal(7,4),
	Fund_self_pct	decimal(7,4),
	Fund_free_pct	decimal(7,4),
	COF_deposit	decimal(7,4),
	COF_bank	decimal(7,4),
	ROE		decimal(7,4),
	Commission	decimal(7,4)
) IN "ESCORETABSP";

insert into TCB_FIN_INFO (Fund_deposit_pct, Fund_bank_pct, Fund_self_pct,
                             Fund_free_pct, COF_deposit, COF_bank, ROE, Commission)
      values (80.0, 14.0, 3.0, 3.0, 1.26, 1.38, 1.68, 0.0);
CREATE TRIGGER TCB_FIN_UPDATE
  AFTER UPDATE ON TCB_FIN_INFO
  REFERENCING OLD AS O
  FOR EACH ROW MODE DB2SQL
  INSERT INTO TCB_FIN_INFO_LOG
    (SYSTEM_TIME, USER_ID, FUND_DEPOSIT_PCT, FUND_BANK_PCT, FUND_SELF_PCT, FUND_FREE_PCT,
     COF_DEPOSIT, COF_BANK, ROE, COMMISSION)
  VALUES (CURRENT TIMESTAMP, USER, O.FUND_DEPOSIT_PCT, O.FUND_BANK_PCT, O.FUND_SELF_PCT, O.FUND_FREE_PCT,
    O.COF_DEPOSIT, O.COF_BANK, O.ROE, O.COMMISSION);


COMMENT ON TABLE TCB_FIN_INFO	IS 'DAC�ӤH�H�U�Ҳհ]�ȸ�T';
COMMENT ON COLUMN TCB_FIN_INFO.Fund_deposit_pct	IS '����ӷ��Ӧۦs�ڦʤ���';
COMMENT ON COLUMN TCB_FIN_INFO.Fund_bank_pct	IS '����ӷ��ӦۦP�~��ڦʤ���';
COMMENT ON COLUMN TCB_FIN_INFO.Fund_self_pct	IS '����ӷ��ۦ��ʤ���';
COMMENT ON COLUMN TCB_FIN_INFO.Fund_free_pct	IS '����ӷ��L���ʤ���';
COMMENT ON COLUMN TCB_FIN_INFO.COF_deposit	IS '�s�ڥ����Q�v';
COMMENT ON COLUMN TCB_FIN_INFO.COF_bank		IS '�P�~��ڧQ�v';
COMMENT ON COLUMN TCB_FIN_INFO.ROE		IS 'ROE';
COMMENT ON COLUMN TCB_FIN_INFO.Commission	IS '�~�ȼ���';


create table TCB_FIN_INFO_LOG (
	user_id		varchar(16),
	system_time	varchar(30),
	Fund_deposit_pct	decimal(7,4),
	Fund_bank_pct	decimal(7,4),
	Fund_self_pct	decimal(7,4),
	Fund_free_pct	decimal(7,4),
	COF_deposit	decimal(7,4),
	COF_bank	decimal(7,4),
	ROE		decimal(7,4),
	Commission	decimal(7,4)
) IN "ESCORETABSP";

COMMENT ON TABLE TCB_FIN_INFO_LOG	IS 'DAC�ӤH�H�U�Ҳհ]�ȸ�T�ק����';
COMMENT ON COLUMN TCB_FIN_INFO_LOG.user_id		IS '�ϥΪ�ID';
COMMENT ON COLUMN TCB_FIN_INFO_LOG.system_time	IS '����Ƥ��t�ήɶ�';
COMMENT ON COLUMN TCB_FIN_INFO_LOG.Fund_deposit_pct	IS '����ӷ��Ӧۦs�ڦʤ���';
COMMENT ON COLUMN TCB_FIN_INFO_LOG.Fund_bank_pct	IS '����ӷ��ӦۦP�~��ڦʤ���';
COMMENT ON COLUMN TCB_FIN_INFO_LOG.Fund_self_pct	IS '����ӷ��ۦ��ʤ���';
COMMENT ON COLUMN TCB_FIN_INFO_LOG.Fund_free_pct	IS '����ӷ��L���ʤ���';
COMMENT ON COLUMN TCB_FIN_INFO_LOG.COF_deposit	IS '�s�ڥ����Q�v';
COMMENT ON COLUMN TCB_FIN_INFO_LOG.COF_bank		IS '�P�~��ڧQ�v';
COMMENT ON COLUMN TCB_FIN_INFO_LOG.ROE		IS 'ROE';
COMMENT ON COLUMN TCB_FIN_INFO_LOG.Commission	IS '�~�ȼ���';

-- Trigger for table TCB_FIN_INFO_LOG need to be added here


-- Create Permanent Working Tables
 create table bam086_dedup (
 	case_sn		char(12),
 	IDN		char(11),
	inquiry_date	char(10),
 	data_yyy	char(3),
 	data_mm		char(2),
 	bank_code	char(7),
 	bank_name	char(40),
 	account_code	char(1),
 	account_code2	char(1),
 	purpose_code	char(1),
 	contract_amt	numeric(10),
 	loan_amt	numeric(10),
 	pass_due_amt	numeric(10),
 	pay_code_12	char(12),
 	co_loan		char(1),
	un_mark         char(1),
	u_yyymmdd       char(8),
	u_rate          numeric(3),
	ib_mark         char(1),
	iab_ban         char(8),
	iab_name        char(60),
	r_yyymmdd       char(7),
	refund_amt      numeric(10),
	ck_ref          char(1),
 	bank_code2	char(3),
	mon_since	int,
	cycle		int,
	now		int,
 	cnt		int
 ) IN "ESCORETABSP";
 create index i_bam086_dedup on bam086_dedup(CASE_SN, IDN);

 create table bam086_bucket (
 	case_sn		char(12),
 	idn		char(11),
	bank_code	char(7),
	mon_since	int,
	bucket		float
 ) IN "ESCORETABSP";
 create index i_bam086_bucket on bam086_bucket(CASE_SN, IDN);

 create table krm001_dedup (
 	case_sn		char(12),
 	idn		char(11),
	inquiry_date	char(10),
 	card_brand	char(1),
 	card_type	char(1),
 	issue		char(3),
 	issue_name	char(40),
 	start_date	char(7),
 	stop_date	char(7),
 	stop_code	char(1),
 	ab_code		char(1),
 	m_s		char(1),
 	limit		char(6),
	limit_type	char(1),
	usage_type	char(1),
	secure		char(1),
 	start_mon_since int,
 	end_mon_since	int,
	now		int,
 	cnt		int
 ) IN "ESCORETABSP";
 create index i_krm001_dedup on krm001_dedup(CASE_SN, IDN);

 create table krm023_dedup (
 	case_sn		char(12),
 	idn		char(11),
	inquiry_date	char(10),
 	yrmon		char(5),
 	issue		char(3),
 	issue_name	char(40),
 	kr_code		char(7),
 	limit		char(5),
 	payment		char(3),
 	cash		char(1),
 	pay_code	char(1),
 	mon_since	int,
 	payment_amt	float,
 	bucket_def_1k	int default 0,
 	bucket_ef_1k	int default 0,
 	bucket_f_1k	int default 0,
	now		int,
	curr_inqmon	int,
 	cnt		int
 ) IN "ESCORETABSP";
 create index i_krm023_dedup on krm023_dedup(CASE_SN, IDN, issue, mon_since);

 create table KRM037_dedup (
 	CASE_SN		char(12) not null,
 	inquiry_date	char(10) not null,
 	IDN		char(10) not null,
 	idn_ban		char(10),
 	bill_date	char(7),
 	issue		char(3),
 	issue_name 	char(24),
 	card_type 	char(7),
 	perm_limit 	char(7),
 	temp_limit 	char(7),
 	cash_limit 	char(7),
 	payable 	char(3),
 	cash_lent 	char(8),
 	last_paya 	char(3),
 	revol_bal 	char(9),
 	pay_stat 	char(1),
 	pay_code 	char(1),
 	revol_rate 	char(4),
 	pre_owed 	char(8),
 	debt_code 	char(1),
 	close_code 	char(1),
 	clear_date 	char(7),
 	mon_since	int,
 	payment_amt	float,
 	bucket_def_1k	int default 0,
 	bucket_ef_1k	int default 0,
 	bucket_f_1k	int default 0,
	now		int,
	curr_inqmon	int,
 	cnt		int
 ) IN "ESCORETABSP";
 create index i_krm037_dedup on krm037_dedup(CASE_SN, IDN);
 
 create table stm001_dedup (
 	case_sn		char(12),
 	idn		char(11),
	inquiry_date	char(10),
 	query_date	char(7),
 	bank_code	char(7),
 	bank_name	char(40),
 	item_list	char(10),
 	query_mon_since int,
	now		int,
 	cnt		int
 ) IN "ESCORETABSP";
 create index i_stm001_dedup on stm001_dedup(CASE_SN, IDN);

create table stm007_dedup (
	CASE_SN		char(12) not null,
	IDN		char(10) not null,
	inquiry_date	char(10) not null,
	query_date 	char(7),
 	bank_code 	char(7),
	bank_name 	char(40),
	item_list 	char(10),
	inq_purpose_1	char(1),
	inq_purpose	char(30),
 	query_mon_since int,
	now		int,
 	cnt		int
) IN "ESCORETABSP";
create index i_stm007_dedup on stm007_dedup(CASE_SN, IDN);

 create table jas002_t (
 	case_sn		char(12),
 	idn		char(11),
	inquiry_date	char(10),
 	reason		char(1),
 	date_happen	char(7),
 	mon_since	int
 ) IN "ESCORETABSP";
 create index i_jas002_t on jas002_t(CASE_SN, IDN);

 create table jas002_t_dedup (
 	case_sn		char(12),
 	idn		char(11),
	inquiry_date	char(10),
 	reason		char(1),
 	date_happen	char(7),
 	mon_since	int,
	now		int,
 	cnt		int
 ) IN "ESCORETABSP";
 create index i_jas002_t_dedup on jas002_t_dedup(CASE_SN, IDN);

 create table pdaco_cal (
	case_sn		char(12),
	idn		char(11),
	inquiry_date	char(10),
	now		int,
	age		int,
	jas002_defect	int default 0,
	krm001_hit	int default 0,
	krm023_hit	int default 0,
	max_bucket	int default 0,
	fs044		int default 0,
	fs334		int default 0,
	cash_max_bucket	int default 0,
	cash_utilization int default 0,
	ind001		int default 0,
	revolving_amt	int default 0,
	fs102_3m	decimal(16,8),
	fs102_6m	decimal(16,8),
	fs102_9m	decimal(16,8),
	ft102_42	decimal(16,8),
	ft102_42_r	decimal(16,8),
	ft102_42_r_tran	decimal(16,8),
	fs031		decimal(16,8),
	fs031_tran	decimal(16,8),
	fs005_1k_3m	decimal(16,8),
	fs005_1k_3m_q	decimal(16,8),
	fs005_1k_3m_q_tran	decimal(16,8),
	fs059_1k_3m	decimal(16,8),
	fs059_1k_6m	decimal(16,8),
	fs059_1k_9m	decimal(16,8),
	ft059_1k_43	decimal(16,8),
	ft059_1k_43_r	decimal(16,8),
	ft059_1k_43_r_tran	decimal(16,8),
	ft059_1k_42	decimal(16,8),
	ft059_1k_42_q	decimal(16,8),
	ft059_1k_42_q_tran	decimal(16,8),
	fs051		decimal(16,8),
	fs051_tran	decimal(16,8),
	fs016_3m	decimal(16,8),
	fs101_3m	decimal(16,8),
	int015_3	decimal(16,8),
	int015_3_tran	decimal(16,8),
	fs205_1k_3m	decimal(16,8),
	fs205_1k_3m_q	decimal(16,8),
	fs205_1k_3m_q_tran	decimal(16,8),
	fs212_1k_3m	decimal(16,8),
	fs212_1k_6m	decimal(16,8),
	ft212_1k_43	decimal(16,8),
	ft212_1k_43_q	decimal(16,8),
	ft212_1k_43_q_tran	decimal(16,8),
	ms117_6m	decimal(16,8),
	fs014_9m	decimal(16,8),
	fs101_9m	decimal(16,8),
	int028_9	decimal(16,8),
	int028_9_tran	decimal(16,8),
	sex		int,
	sex_tran	int,
	app_last_month_bucket	int,
	app_last_month_bucket_tran	int,
	pdaco_score	decimal(16,8),
	pdaco_twen	decimal(16,8),
	pb_in		decimal(16,8),
	npv		decimal(16,2),
	ps_code		int,
	ps_msg		varchar(128),
	cut		decimal(16,8),
	loan_amount	decimal(16,8),
	apr		decimal(16,8),
	term		decimal(16,8),
	amortization_rate	decimal(16,8),
	monthly_payment	decimal(16,8),
	ms047		decimal(16,8),
	ms080		decimal(16,8),
	ms093		decimal(16,8),
	ms094		decimal(16,8),
	ms105		decimal(16,8),
	wi006_9m	decimal(16,8),
	wi006_9mb	decimal(16,8),
	wi008_3m	decimal(16,8),
	wi008_9m	decimal(16,8),
	wi002_12m	decimal(16,8),
	wi004_12m	decimal(16,8),
	ln_35_047	decimal(16,8),
	ln_35_080	decimal(16,8),
	ln_35_006_9mb	decimal(16,8),
	ln_35_008_9m	decimal(16,8),
	ln_35_002_12m	decimal(16,8),
	ln_35_004_12m	decimal(16,8),
	ln_35_004_12m_q	decimal(16,8),
	ln_35_004_12m_q_t1	decimal(16,8),
	ln_35_004_12m_q_t4	decimal(16,8),
	ln_35_002_12m_t1	decimal(16,8),
	ln_35_002_12m_t3	decimal(16,8),
	ln_35_080_t	decimal(16,8),
	ln_35_006_9mb_t1	decimal(16,8),
	ln_35_006_9mb_t4	decimal(16,8),
	ln_35_047_t	decimal(16,8),
	ln_35_080_9m_r	decimal(16,8),
	ln_35_080_9m_r_t2	decimal(16,8),
	ci_score	decimal(16,8),
	cpdaco_score	decimal(16,8),
	reserve1	float,
	reserve2	float,
	reserve3	float,
	reserve4	float,
	reserve5	float,
	reserve6	float,
	reserve7	float,
	reserve8	float,
	reserve9	varchar(20),
	reserve10	varchar(20)
	
 ) IN "ESCORETABSP";
 create index i_pdaco_cal on pdaco_cal(CASE_SN, IDN);

 create table t1 (
    case_sn   char(12),
    mon	      int,
    deliquent_line float
 ) IN "ESCORETABSP";
 create index i_t1 on t1(CASE_SN);

 create table open_card (
    case_sn	char(12),
    issue	char(3),
    now		int,
    mon		int
 ) IN "ESCORETABSP";
 create index i_open_card on open_card(CASE_SN);

 create table open_line (
    case_sn	char(12),
    issue	char(3),
    mon		int,
    now		int,
    cards	int,
    bucket	int
 ) IN "ESCORETABSP";
 create index i_open_line on open_line(CASE_SN);


 create table latest_stmt_mon (
    case_sn	char(12),
    issue	char(3),
    mon		int
 ) IN "ESCORETABSP";
 create index i_latest_stmt_mon on latest_stmt_mon(CASE_SN);


 create table latest_line (
    case_sn	char(12),
    issue	char(3),
    mon		int,
    mob		int
 ) IN "ESCORETABSP";
 create index i_latest_line on latest_line(CASE_SN);

 create table pdaco_result  (
	case_sn		char(12),
	idn		char(11),
	inquiry_date	char(10),
	now		int,
	age		int,
	jas002_defect	int default 0,
	krm001_hit	int default 0,
	krm023_hit	int default 0,
	max_bucket	int default 0,
	fs044		int default 0,
	fs334		int default 0,
	cash_max_bucket	int default 0,
	cash_utilization int default 0,
	ind001		int default 0,
	revolving_amt	int default 0,
	fs102_3m	decimal(16,8),
	fs102_6m	decimal(16,8),
	fs102_9m	decimal(16,8),
	ft102_42	decimal(16,8),
	ft102_42_r	decimal(16,8),
	ft102_42_r_tran	decimal(16,8),
	fs031		decimal(16,8),
	fs031_tran	decimal(16,8),
	fs005_1k_3m	decimal(16,8),
	fs005_1k_3m_q	decimal(16,8),
	fs005_1k_3m_q_tran	decimal(16,8),
	fs059_1k_3m	decimal(16,8),
	fs059_1k_6m	decimal(16,8),
	fs059_1k_9m	decimal(16,8),
	ft059_1k_43	decimal(16,8),
	ft059_1k_43_r	decimal(16,8),
	ft059_1k_43_r_tran	decimal(16,8),
	ft059_1k_42	decimal(16,8),
	ft059_1k_42_q	decimal(16,8),
	ft059_1k_42_q_tran	decimal(16,8),
	fs051		decimal(16,8),
	fs051_tran	decimal(16,8),
	fs016_3m	decimal(16,8),
	fs101_3m	decimal(16,8),
	int015_3	decimal(16,8),
	int015_3_tran	decimal(16,8),
	fs205_1k_3m	decimal(16,8),
	fs205_1k_3m_q	decimal(16,8),
	fs205_1k_3m_q_tran	decimal(16,8),
	fs212_1k_3m	decimal(16,8),
	fs212_1k_6m	decimal(16,8),
	ft212_1k_43	decimal(16,8),
	ft212_1k_43_q	decimal(16,8),
	ft212_1k_43_q_tran	decimal(16,8),
	ms117_6m	decimal(16,8),
	fs014_9m	decimal(16,8),
	fs101_9m	decimal(16,8),
	int028_9	decimal(16,8),
	int028_9_tran	decimal(16,8),
	sex		int,
	sex_tran	int,
	app_last_month_bucket	int,
	app_last_month_bucket_tran	int,
	pdaco_score	decimal(16,8),
	pdaco_twen	decimal(16,8),
	pb_in		decimal(16,8),
	npv		decimal(16,2),
	ps_code		int,
	ps_msg		varchar(128),
	cut		decimal(16,8),
	loan_amount	decimal(16,8),
	apr		decimal(16,8),
	term		decimal(16,8),
	amortization_rate	decimal(16,8),
	monthly_payment	decimal(16,8),
	ms047		decimal(16,8),
	ms080		decimal(16,8),
	ms093		decimal(16,8),
	ms094		decimal(16,8),
	ms105		decimal(16,8),
	wi006_9m	decimal(16,8),
	wi006_9mb	decimal(16,8),
	wi008_3m	decimal(16,8),
	wi008_9m	decimal(16,8),
	wi002_12m	decimal(16,8),
	wi004_12m	decimal(16,8),
	ln_35_047	decimal(16,8),
	ln_35_080	decimal(16,8),
	ln_35_006_9mb	decimal(16,8),
	ln_35_008_9m	decimal(16,8),
	ln_35_002_12m	decimal(16,8),
	ln_35_004_12m	decimal(16,8),
	ln_35_004_12m_q	decimal(16,8),
	ln_35_004_12m_q_t1	decimal(16,8),
	ln_35_004_12m_q_t4	decimal(16,8),
	ln_35_002_12m_t1	decimal(16,8),
	ln_35_002_12m_t3	decimal(16,8),
	ln_35_080_t	decimal(16,8),
	ln_35_006_9mb_t1	decimal(16,8),
	ln_35_006_9mb_t4	decimal(16,8),
	ln_35_047_t	decimal(16,8),
	ln_35_080_9m_r	decimal(16,8),
	ln_35_080_9m_r_t2	decimal(16,8),
	ci_score	decimal(16,8),
	cpdaco_score	decimal(16,8),
	reserve1	float,
	reserve2	float,
	reserve3	float,
	reserve4	float,
	reserve5	float,
	reserve6	float,
	reserve7	float,
	reserve8	float,
	reserve9	varchar(20),
	reserve10	varchar(20)
 ) IN "ESCORETABSP";
