#ifndef economicH
#define economicH
//-------------------------------------------------------------------------------------------
//Economic model constants
const double LEVERAGE_RATIO = 0.9;     // 舉債比率（％）
const double COMMISSION_RATE = 0.0015; // 佣金比率（％）
const double FEE_COMMISSION_RATE = 0.1;// 開辦費佣金比率（％）
const double ALLOGRAPH_FEE = 0.0;      // 代書費
const double LAND_ADM_FEE = 0.0;       // 地政規費
const double DUEI_BAO_FEE = 2.25;      // 對保
const double SETUP_FEE = 3.0;          // 開辦費
const double COF = 0.01;               // 資金成本
const double ROE = 0.1;                // 股東權益報酬
const double ACCT_MGMT_COST = 0.08;    // Account Management Cost
const double LATE_PENALTY_RATIO = 1.2; // mutiple of interest rate applied
const double M2_3_EXPENSE = 5.25;      // NT$5.25K
const double M4P_EXPENSE = 22.5;       // NT$22500 per account per month
const double TIME_TO_RECOVER = 12;     // months
const double LEGAL_FEE_RATE = 0.008;   // percentage of balance, taken from Teresa's model, for collection

#endif