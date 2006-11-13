#include "npv.h"

bt::bt(int bt_counts, int total_bt_amount, int bt_fee_type,
       double monthly_bt_fee, int months_charging_bt_fee, int bt_period,
       double pb, const vector<double> &interest_scheme, double regular_apr)
{
  _bt_counts = bt_counts;
	_total_bt_amount = total_bt_amount;
  _monthly_fee_type = bt_fee_type;
  _monthly_fee = monthly_bt_fee;
  _months_charging_fee = months_charging_bt_fee;
  _bt_period = bt_period;
  _monthly_pb = pb / 12.0;
  _monthly_regular_apr = regular_apr / 12.0;

  _npv[0][0] = 0;  //interest revenue
  _npv[0][1] = 0;  //mgmt fee
  _npv[0][2] = 0;  //remaining balabnce
  _npv[0][3] = 0;  //interest rate
  _npv[0][4] = 1;  //probability of stay; i.e. account attrition
  _npv[0][5] = 0;  //credit loss
  _npv[0][6] = 0;  //pre-collection cost
  _npv[0][7] = 0;  //net value
  _npv[0][8] = 0;  //interest cost
  _npv[0][9] = 0;  //collection cost
  _npv[0][10] = 0; //final collection cost
  _npv[0][11] = 1; //% of balance remaining; i.e. balance attrition
  _npv[0][12] = 0; //delta working capital
  _npv[0][13] = 0; //operating expense

  int interest_terms = static_cast<int>(interest_scheme.size());
  for(int i=0; i< NPV_PERIODS + 10; ++i){
    if(i<interest_terms)
      _npv[i][3] = interest_scheme[i];
    else if(i<=NPV_PERIODS)
      _npv[i][3] = _monthly_regular_apr;
    else
      _npv[i][3] = 0;
  };
};

void bt::remaining_balance_percentage()
{
	double good_balance_attrition[7]={
	0,
  0.0407654167932413,
  0.0441244265476468,
  0.0469019450711537,
  0.0487361195528268,
  0.0614458962808867,
  0.07};

  _npv[0][11] = 1;
	for(int i=1; i<NPV_PERIODS+10; ++i){
    if(i<NPV_PERIODS+1){
      double good_attrition = i > 5 ? good_balance_attrition[6] : good_balance_attrition[i];
      _npv[i][11] = _npv[i-1][11] * (1 - good_attrition - _monthly_pb);
    }
    else
      _npv[i][11] = 0;
  };
};

void bt::probability_of_stay()
{
	_npv[0][4] = 1;
	for(int i = 1; i<NPV_PERIODS+10; ++i){
    if(i<NPV_PERIODS+1){
      double good_attrition = i > _bt_period ? OUT_OF_TEASER_VOLUTARY_ATTRITION : IN_TEASER_VOLUTARY_ATTRITION;
      _npv[i][4] = _npv[i-1][4] * (1 - good_attrition - _monthly_pb);
    }
    else
      _npv[i][4] = 0;
  };
};

void bt::remaining_balance(int i)
{
  if(i > NPV_PERIODS)
	  _npv[i][2] = 0;
  else
		_npv[i][2] = _total_bt_amount * _npv[i][11];
};


void bt::interest_revenue(int i)
{
  if((i == 0) ||(i > NPV_PERIODS))
	  _npv[i][0] = 0;
  else
		_npv[i][0] = _npv[i-1][2] * _npv[i][3];
};

void bt::interest_cost(int i)
{
  if((i == 0) || (i > NPV_PERIODS))
    _npv[i][8] = 0;
  else
    _npv[i][8] = LEVERAGE_RATIO * COF/12.0 * _npv[i-1][2];
};

double bt::discount()
{
  double npv = 0;
  for(int i=0; i<=NPV_PERIODS + 9; ++i)
    npv += _npv[i][7] / pow(1 + ROE / 12.0, i);
  return npv;
};

void bt::pre_collection_cost(int i)
{
  if((i < 2) || (i > (NPV_PERIODS + 1)))
    _npv[i][6] = 0;
  else
    _npv[i][6] = DPD_6_90_COST * _npv[i-2][4] * _monthly_pb * DPD_6_90_ROLL_BACK_RATE;
};

void bt::collection_cost(int i)
{
  if((i < 4) || (i > (NPV_PERIODS + 3)))
    _npv[i][9] = 0;
  else{
    int court_order_cost = DPD_COURT_ORDER_COST;
    if((_npv[i-4][2] / _npv[i-4][4]) < 3000)
      court_order_cost = 0;
    double in_house =  (1 - DPD_90_270_OUTSOURCING_RATE) * (DPD_90_270_COST_INHOUSE + DPD_COURT_FORCED_EXEC_COST * DPD_COURT_FORCED_EXEC_RATE);
    double out_source = DPD_90_270_OUTSOURCING_RATE * DPD_90_270_RECOVERY_RATE_OUTSOURCING * (_npv[i-4][2] / _npv[i-4][4]) * DPD_90_270_COST_OUTSOURCING;
    _npv[i][9] = _npv[i-4][4] * _monthly_pb * (in_house + out_source + court_order_cost);
  };
};

void bt::final_collection_cost(int i)
{
  if((i < 10) || (i > (NPV_PERIODS + 9)))
    _npv[i][10] = 0;
  else{
    _npv[i][10] = _npv[i-10][4] * (_monthly_pb * DPD_270P_ROLL_RATE) * (_npv[i-10][2] / _npv[i-10][4]) * DPD_270P_RECOVERY_RATE * DPD_270P_COST;
  }
};

void bt::credit_loss(int i)
{
  if((i==0) || (i > NPV_PERIODS))
    _npv[i][5] = 0;
  else{
    double recovery_collection;
//    if(i==1)
      recovery_collection = _npv[i-1][2] * _monthly_pb * (((DPD_90_270_OUTSOURCING_RATE * DPD_90_270_RECOVERY_RATE_OUTSOURCING + (1 - DPD_90_270_OUTSOURCING_RATE) * DPD_90_270_RECOVERY_DATE_INHOUSE)) / pow((1+ROE), (DPD_90_270_AVERAGE_MONTH / 12.0)));
//    else
//        recovery_collection = _npv[i-1][2] * _monthly_pb * (((DPD_90_270_OUTSOURCING_RATE * (1 - DPD_90_270_COST_OUTSOURCING) * DPD_90_270_RECOVERY_RATE_OUTSOURCING + (1 - DPD_90_270_OUTSOURCING_RATE) * DPD_90_270_RECOVERY_DATE_INHOUSE)) / pow((1+ROE), (DPD_90_270_AVERAGE_MONTH / 12.0)));
    double recovery_final = (_npv[i-1][2] * _monthly_pb * DPD_270P_ROLL_RATE * DPD_270P_RECOVERY_RATE) / pow((1+ROE), (DPD_270P_AVERAGE_MONTH / 12.0));
    _npv[i][5] =  (_npv[i-1][2] * _monthly_pb) - recovery_collection - recovery_final;
  };
};

void bt::delta_working_capital(int i)
{
  credit_loss(i);
  if(i == 0)
    _npv[i][12] = _total_bt_amount * (LEVERAGE_RATIO - 1);
  else if (i < NPV_PERIODS){
    _npv[i][12] = (_npv[i-1][2] - _npv[i][2]) * (1 - LEVERAGE_RATIO) - _npv[i][5];
  }
  else if (i == NPV_PERIODS){
    _npv[i][12] = (_npv[i-1][2]) * (1 - LEVERAGE_RATIO) - _npv[i][5];
  }
  else
    _npv[i][12] = 0;
};

void bt::mgmt_fee(int i)
{
  if((i <= _months_charging_fee) && (i > 0)){
    if(_monthly_fee_type == 1)
      _npv[i][1] = _monthly_fee * _bt_counts;
    else
      _npv[i][1] = _monthly_fee * _npv[i-1][2];
  }
  else if((_months_charging_fee == 1) && (i==0))
    _npv[0][1] = _monthly_fee * _bt_counts;
  else
    _npv[i][1] = 0;
};

void bt::operation_expense(int i)
{
  if((i==0)||(i>NPV_PERIODS))
    _npv[i][13] = 0;
  else
    _npv[i][13] = MONTHLY_OPERATING_EXPENSE * _npv[i-1][4];
};

double bt::npv()
{
//initial term cost
  double setup_cost = 50;
  double commission = COMMISSION_RATE * _total_bt_amount;
  remaining_balance_percentage();
  probability_of_stay();

//monthly revenues
  for(int i = 0; i < NPV_PERIODS + 10; ++i){
     remaining_balance(i);
     interest_revenue(i);
     interest_cost(i);
     pre_collection_cost(i);
     collection_cost(i);
     final_collection_cost(i);
     delta_working_capital(i);
     mgmt_fee(i);
     operation_expense(i);

    _npv[i][7] =  _npv[i][0] + _npv[i][1] + _npv[i][12] - _npv[i][6] -
                  _npv[i][8] - _npv[i][9] - _npv[i][10] - _npv[i][13];
    if(i==0)
     _npv[i][7] = _npv[i][7] - setup_cost - commission;
  };
  double npv_value = discount();
//  bool dump = dump_series(10, NPV_PERIODS+9);
  return npv_value;
};


bool bt::dump_series(int type, int period)
{
  ofstream output("npv_dumping.csv", ios_base::app);
  if(!output){
    cerr << "Can not open attrition.csv for output.\n";
    return false;
  };
//  output << _case_sn;
//  output << endl;
  output.precision(15);
  switch (type){
    case 2: //outstanding principle (i.e. remaining principle)
      for(int i=0; i<=period; ++i){
       output << _npv[i][2] << ",";
       if(_npv[i][2]==0)
         break;
      };
    break;
    case 4: //attrition (i.e. probability of stay)
      for(int i=0; i<=period; ++i){
       output << _npv[i][4] << ",";
       if(_npv[i][2]==0)
         break;
      };
    break;
    case 6: //credit loss
      for(int i=0; i<=period; ++i){
       output << _npv[i][6] << ",";
       if(_npv[i][2]==0)
         break;
      };
    break;
    case 7: //credit loss
      for(int i=0; i<=period; ++i){
       output << _npv[i][7] << ",";
       if(_npv[i][2]==0)
         break;
      };
    break;
    case 10: //everything
       for(int j=0; j < 14; ++j){
         for(int i=0; i<=period; ++i){
           output << _npv[i][j] << ",";
//           if(_npv[i][2]==0)
//             break;
         }
         output<<endl;
       }
    break;
  }
  output << endl;
  output.close();
  return true;
};
