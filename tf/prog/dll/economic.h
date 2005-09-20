#ifndef economicH
#define economicH
//-------------------------------------------------------------------------------------------
//Economic model constants
const double leverage_ratio = 1.00; //舉債比率（％）
const double recovery_ratio = 0.011;// Net recovery ratio (M6 之後四個月),壞帳回收百分比, (資金損失 % (EAD) )
const double D1_WRITEOFF = 14.259238730; //D1 to W/O ratio
const double D1_FIX_COST = 337.36; //D1 Fixed cost
const double LATE_PENALTY_RATIO = 0.1; //Late penalty ratio on unpaid interest
const double LATE_30D_RATIO = 0.203896985; // tatio of 30 days late / original

// GX product information
const double GX_APP_FEE_RECEIVABLE = 5000.0;
const double GX_TRANSFER_BONUS = 3000.0;
const double GX_OUT_SOURCE_BONUS = 740.0;
const double GX_OUT_SOURCE_RATE = 0.04;

// KHJ product information
const double KHJ_APP_FEE_RECEIVABLE = 2000.0;
const double KHJ_TRANSFER_BONUS = 3000.0;
const double KHJ_OUT_SOURCE_BONUS = 740.0;
const double KHJ_OUT_SOURCE_RATE = 0.04;

//-------------------------------------------------------------------------------------------
// Information to calculation commission
enum Product { GX,   // 0: 國民信貸
	       KHJ}; // 1: 卡好借

const char *ChannelDescription [] = { "",        // Dummy channel 0
				  "Direct sales",  // Channel 1
				  "Channel",       // Channel 2
				  "行員轉介DS",    // Channel 3
				  "行員轉介電銷",  // Channel 4
				  "行員轉介貸款店",// Channel 5
				  "TM - CRM",      // Channel 6
				  "TM - Others",   // Channel 7
				  "貸款店",        // Channel 8
				  "委外" };        // Channel 9

// 點數成本, Add 0 for dummy channel 0
const int SalesPointCost[2][10] = {{0, 1503, 1503, 1503, 1844, 2709, 1844, 1844, 2709, 0}, // GX Channel 1-9
                                   {0, 1503, 1503, 1503, 1844, 2709, 1844, 1844, 2709, 0}};// KHJ Channel 1-9
// 主管手續獎金, Add 0 for dummy channel 0
const int HeadFeeBonus [2][10] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // GX, Channel 1-9
                                  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}; // KHJ, Channel 1-9

// 業務手續獎金, Add 0 for dummy channel 0
const int SalesFeeBonus [2][10] = {{0, 2700, 2700, 540, 540, 540, 540, 2700, 540, 0},  // GX, Channel 1-9
                                   {0, 1080, 1080, 0, 0, 0, 0,  1080, 0, 0}}; // KHJ, Channel 1-9

// 主管手獎減免, Add 0 for dummy channel 0
const int HeadBonusDiscount [2][10] = {{0, 1, 1, 0, 0, 0, 0, 1, 0, 0},  // GX, Channel 1-9
                                       {0, 1, 1, 0, 0, 0, 0, 1, 0, 0}}; // KHJ, Channel 1-9

// 業務手獎減免, Add 0 for dummy channel 0
int SalesBonusDiscount [2][10] = {{0, 1, 1, 0, 0, 0, 0, 1, 0, 0},  // GX, Channel 1-9
                                  {0, 1, 1, 0, 0, 0, 0, 1, 0, 0}}; // KHJ, Channel 1-9

// 產品	通路 利率, Add 0 for dummy channel 0
const double VariableCommission [2][10][11] = {
   {{ 0, 0,   0, 0,   0, 0,   0, 0, 0, 0, 0}, //GX, Channel 0, [20.0% to 15.0% by 0.5%]
   {3.5, 3, 2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0}, //GX, Channel 1, [20.0% to 15.0% by 0.5%]
   {3.5, 3, 2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0}, //GX, Channel 2, [20.0% to 15.0% by 0.5%]
   {3.5, 3, 2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0}, //GX, Channel 3, [20.0% to 15.0% by 0.5%]
   {3.5, 3, 2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0}, //GX, Channel 4, [20.0% to 15.0% by 0.5%]
   {3.5, 3, 2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0}, //GX, Channel 5, [20.0% to 15.0% by 0.5%]
   {3.5, 3, 2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0}, //GX, Channel 6, [20.0% to 15.0% by 0.5%]
   {3.5, 3, 2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0}, //GX, Channel 7, [20.0% to 15.0% by 0.5%]
   {3.5, 3, 2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0}, //GX, Channel 8, [20.0% to 15.0% by 0.5%]
   {3.5, 3, 2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0}},//GX, Channel 9, [20.0% to 15.0% by 0.5%]
   {{ 0, 0,   0, 0,   0, 0, 0, 0, 0, 0, 0},//KHJ, Channel 0, [20.0% to 15.0% by 0.5%]
   {2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0, 0, 0},//KHJ, Channel 1, [20.0% to 15.0% by 0.5%]
   {2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0, 0, 0},//KHJ, Channel 2, [20.0% to 15.0% by 0.5%]
   {2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0, 0, 0},//KHJ, Channel 3, [20.0% to 15.0% by 0.5%]
   {2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0, 0, 0},//KHJ, Channel 4, [20.0% to 15.0% by 0.5%]
   {2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0, 0, 0},//KHJ, Channel 5, [20.0% to 15.0% by 0.5%]
   {2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0, 0, 0},//KHJ, Channel 6, [20.0% to 15.0% by 0.5%]
   {2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0, 0, 0},//KHJ, Channel 7, [20.0% to 15.0% by 0.5%]
   {2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0, 0, 0},//KHJ, Channel 8, [20.0% to 15.0% by 0.5%]
   {  0, 0,   0, 0,   0, 0, 0, 0, 0, 0, 0}}//KHJ, Channel 9, [20.0% to 15.0% by 0.5%]
};
/*
 GX Lines:
 150,000: line <= 150,000 then GX_line = 0;
 200,000: line <= 200,000 then GX_line = 1;
 250,000: line <= 250,000 then GX_line = 2;
 300,000: line <= 300,000 then GX_line = 3;
 350,000: line <= 350,000 then GX_line = 4;
 400,000: line <= 400,000 then GX_line = 5;
 450,000: line <= 450,000 then GX_line = 6;
 500,000: line <= 500,000 then GX_line = 7;
 550,000: line <= 550,000 then GX_line = 8;
 600,000: line <= 600,000 then GX_line = 9;
 APR:
 20.0%: apr_group = 0;
 19.5%: apr_group = 1;
 19.0%: apr_group = 2;
 18.5%: apr_group = 3;
 18.0%: apr_group = 4;
 17.5%: apr_group = 5;
 17.0%: apr_group = 6;
 16.5%: apr_group = 7;
 16.0%: apr_group = 8;
 15.5%: apr_group = 9;
 15.0%: apr_group = 10;
*/

// Risk management fee for product GX and KHJ
const int GX_RiskMgmtFee [][2] = {
    {7500, 12000}, // low risk, high risk
    {9000, 14400},
    {11500, 18400},
    {14000, 22400},
    {16500, 26400},
    {19000, 30400},
    {21500, 34400},
    {24000, 38400},
    {26500, 42400},
    {29000, 43500}};

/* KHJ Lines:
 100,000: line <= 100,000 then KHJ_line = 0;
 120,000: line <= 120,000 then KHJ_line = 1;
 150,000: line <= 150,000 then KHJ_line = 2;
*/
const int KHJ_RiskMgmtFee [] = {100000, 120000, 150000};

//-------------------------------------------------------------------------------------------

const double vol_attrition_open[2][61] = {
{0.0, 0.0025, 0.005, 0.0075, 0.01, 0.0125, 0.015, 0.02, 0.023, 0.026, 0.029, 0.032, 0.035,// GX Year 1
     0.04,   0.036, 0.032, 0.024, 0.024, 0.024, 0.024, 0.024, 0.024, 0.024, 0.024, 0.024, // GX Year 2
     0.024,  0.024, 0.024, 0.024, 0.024, 0.024, 0.024, 0.024, 0.024, 0.024, 0.024, 0.024, // GX Year 3
     0.024,  0.024, 0.024, 0.024, 0.024, 0.024, 0.024, 0.024, 0.024, 0.024, 0.024, 0.024, // GX Year 4
     0.024,  0.024, 0.024, 0.024, 0.024, 0.024, 0.024, 0.024, 0.024, 0.024, 0.024, 0.0},  // GX Year 5
{0.0, 0.0, 0.001706485, 0.003418803, 0.006861063, 0.008638065, 0.0092, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01,// KHJ Year 1
     0.01 , 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, // KHJ Year 2
     0.01 , 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, // KHJ Year 3
     0.01 , 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, // KHJ Year 4
     0.01 , 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.0}  // KHJ Year 5
};

//-------------------------------------------------------------------------------------------
// Information to calculation PB
// score card A2
const double AMOUNT_3 = 300000;
const double AMOUNT_2 = 200000;
const double AMOUNT_1 = 100000;
const double INFLAT_3 = 15;
const double INFLAT_2 = 3;
const double INFLAT_1 = 1;
const double TRIVIAL_NUM = 0.000001;


const double MIN_LN001_12M_R = 22.0969;
const double MAX_LN001_12M_R = 556.1794;
const double MIN_PARTIAL_SCORE = -0.084903;
const double MAX_PARTIAL_SCORE = 0.723525;
const double a_a2[4] = {0.0,    0.0,     0.39780, 1.31178};
const double b_a2[4] = {0.0,    0.60208, 0.0,     1.01566};
const double c_a2[4] = {0.0,    0.0,     0.00012, 9.42811};
const double d_a2[4] = {12.683, 1.00048, 2.33763, 1.0541};


// score card B2
const double MIN_MPR = 0.0;
const double MAX_MPR = 282.02904106;
const double MIN_BRMP_SCORE = 0.0381;
const double MAX_BRMP_SCORE = 0.67827;
const double a_b2[4] = { 0.0,       0.0, 0.077553, 0.159439};
const double b_b2[4] = { 0.0,       0.0, 0.577976, 0.063206};
const double c_b2[4] = { 0.0,       0.0, 0.0,     10.2859  };
const double d_b2[4] = { 0.0081885, 0.0, 0.0,      9.362   };




















#endif