#ifndef economicH
#define economicH
//-------------------------------------------------------------------------------------------
//Economic model constants
const double LEVERAGE_RATIO = 0.9;     // �|�Ť�v�]�H�^
const double COMMISSION_RATE = 0.0015; // ������v�]�H�^
const double FEE_COMMISSION_RATE = 0.1;// �}��O������v�]�H�^
const double ALLOGRAPH_FEE = 0.0;      // �N�ѶO
const double LAND_ADM_FEE = 0.0;       // �a�F�W�O
const double DUEI_BAO_FEE = 2.25;      // ��O
const double SETUP_FEE = 3.0;          // �}��O
const double COF = 0.01;               // �������
const double ROE = 0.1;                // �ѪF�v�q���S
const double ACCT_MGMT_COST = 0.08;    // Account Management Cost
const double LATE_PENALTY_RATIO = 1.2; // mutiple of interest rate applied
const double M2_3_EXPENSE = 5.25;      // NT$5.25K
const double M4P_EXPENSE = 22.5;       // NT$22500 per account per month
const double TIME_TO_RECOVER = 12;     // months
const double LEGAL_FEE_RATE = 0.008;   // percentage of balance, taken from Teresa's model, for collection

#endif