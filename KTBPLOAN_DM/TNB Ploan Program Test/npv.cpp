#include "npv.h"

ploan::ploan(unsigned int loan_amount, double apr, unsigned int terms,
             unsigned int application_fee,
             double ms093, double ms094, double ms105, double ms047,
             double ms080, double wi006_9m, double wi008_3m, double wi008_9m,
             double wi002_12m, double wi004_12m, double pdaco_score,
             bool ms093_isnull, bool ms094_isnull, bool ms105_isnull, bool ms047_isnull,
             bool ms080_isnull, bool wi006_9m_isnull, bool wi008_3m_isnull,
             bool wi008_9m_isnull, bool wi002_12m_isnull, bool wi004_12m_isnull)
{
	_loan_amount = loan_amount;
  _terms = terms;
  _application_fee = application_fee;
  _ms093 = ms093;
  _ms094 = ms094;
  _ms105 = ms105;
  _ms047 = ms047;
  _ms080 = ms080;
  _wi006_9m = wi006_9m;
  _wi008_3m = wi008_3m;
  _wi008_9m = wi008_9m;
  _wi002_12m = wi002_12m;
  _wi004_12m = wi004_12m;
  _ms093_isnull = ms093_isnull;
  _ms094_isnull = ms094_isnull;
  _ms105_isnull = ms105_isnull;
  _ms047_isnull = ms047_isnull;
  _ms080_isnull = ms080_isnull;
  _wi006_9m_isnull = wi006_9m_isnull;
  _wi008_3m_isnull = wi008_3m_isnull;
  _wi008_9m_isnull = wi008_9m_isnull;
  _wi002_12m_isnull = wi002_12m_isnull;
  _wi004_12m_isnull = wi004_12m_isnull;

  _year_for_pd = ((_terms / 12.0) + 0.5);

  _pdaco_score = pdaco_score;
  _monthly_rate = apr / 12.0;
  _apr = apr;
  _npv = new double [_terms+4][14];
  _located = true;

  _npv[0][0] = 0;  //interest revenue
  _npv[0][1] = 0;  //principal payment
  _npv[0][2] = _loan_amount;  //remaining principal
  _npv[0][3] = _monthly_rate;  //interest rate
  _npv[0][4] = 1;  //probability of stay; i.e. account attrition
  _npv[0][5] = 0;  //credit loss
  _npv[0][6] = 0;  //pre-collection cost
  _npv[0][8] = 0;  //interest cost
  _npv[0][9] = 0;  //collection cost
  _npv[0][7] = 0;  //net value


  _npv[0][10] = 0; //late fee

  _npv[0][11] = 1; //% of balance remaining; i.e. balance attrition, not used
  _npv[0][12] = 0; //delta working capital
  _npv[0][13] = 0; //operating expense

//  int interest_terms = static_cast<int>(interest_scheme.size());
  for(unsigned int i = 0; i<= _terms+3; ++i){
    _npv[i][3] = _monthly_rate;
//    if(i<interest_terms)
//      _npv[i][3] = interest_scheme[i];
//    else if(i<=NPV_PERIODS)
//      _npv[i][3] = _monthly_regular_apr;
//    else
//      _npv[i][3] = 0;
  };
};

ploan::~ploan()
{
  if(_located)
    delete[]_npv;
};

void ploan::probability_of_stay()
{
	_npv[0][4] = 1;
  double base31[36] =
  {0.0066970353615938, 0.0066970353615938, 0.0066970353615938, 0.0066970353615938, 0.0066970353615938, 0.0066970353615938,
   0.0066970353615938, 0.0066970353615938, 0.0066970353615938, 0.0066970353615938, 0.0066970353615938, 0.0305810397553517,
   0.0366972477064220, 0.0275229357798165, 0.0275229357798165, 0.0220183486238532, 0.0220183486238532, 0.0220183486238532,
   0.0220183486238532, 0.0220183486238532, 0.0220183486238532, 0.0220183486238532, 0.0220183486238532, 0.0220183486238532,
   0.0220183486238532, 0.0220183486238532, 0.0220183486238532, 0.0220183486238532, 0.0220183486238532, 0.0220183486238532,
   0.0220183486238532, 0.0220183486238532, 0.0220183486238532, 0.0220183486238532, 0.0220183486238532, 0.0220183486238532};
  double base32[36] =
  {0.0076531503387259, 0.0076531503387259, 0.0076531503387259, 0.0076531503387259, 0.0076531503387259, 0.0076531503387259,
   0.0076531503387259, 0.0076531503387259, 0.0076531503387259, 0.0076531503387259, 0.0076531503387259, 0.0469483568075117,
   0.0563380281690141, 0.0422535211267606, 0.0422535211267606, 0.0338028169014084, 0.0338028169014084, 0.0338028169014084,
   0.0338028169014084, 0.0338028169014084, 0.0338028169014084, 0.0338028169014084, 0.0338028169014084, 0.0338028169014084,
   0.0338028169014084, 0.0338028169014084, 0.0338028169014084, 0.0338028169014084, 0.0338028169014084, 0.0338028169014084,
   0.0338028169014084, 0.0338028169014084, 0.0338028169014084, 0.0338028169014084, 0.0338028169014084, 0.0338028169014084};

  double base51[60]=
  {0.0050520442137829, 0.0050520442137829, 0.0050520442137829, 0.0050520442137829, 0.0050520442137829, 0.0050520442137829,
   0.0050520442137829, 0.0050520442137829, 0.0050520442137829, 0.0050520442137829, 0.0050520442137829, 0.0206611570247934,
   0.0247933884297521, 0.0185950413223140, 0.0185950413223140, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512,
   0.0148760330578512, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512,
   0.0148760330578512, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512,
   0.0148760330578512, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512,
   0.0148760330578512, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512,
   0.0148760330578512, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512,
   0.0148760330578512, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512,
   0.0148760330578512, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512, 0.0148760330578512};
  double base52[60] =
  {0.0049858943709634, 0.0049858943709634, 0.0049858943709634, 0.0049858943709634, 0.0049858943709634, 0.0049858943709634,
   0.0049858943709634, 0.0049858943709634, 0.0049858943709634, 0.0049858943709634, 0.0049858943709634, 0.0431034482758621,
   0.0517241379310345, 0.0387931034482759, 0.0387931034482759, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207,
   0.0310344827586207, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207,
   0.0310344827586207, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207,
   0.0310344827586207, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207,
   0.0310344827586207, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207,
   0.0310344827586207, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207,
   0.0310344827586207, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207,
   0.0310344827586207, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207, 0.0310344827586207};
  double base71[120] =
  {0.00390897358644327, 0.00390897358644327, 0.00390897358644327, 0.00390897358644327, 0.00390897358644327, 0.00390897358644327,
   0.00390897358644327, 0.00390897358644327, 0.00390897358644327, 0.00390897358644327, 0.00390897358644327, 0.01922508133688260,
   0.02307009760425910, 0.01730257320319430, 0.01730257320319430, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550,
   0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550,
   0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550,
   0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550,
   0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550,
   0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550,
   0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550,
   0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550,
   0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550,
   0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550,
   0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550,
   0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550,
   0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550,
   0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550,
   0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550,
   0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550,
   0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550,
   0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550, 0.01384205856255550};
  double base72[120] =
  {0.0055139496437332, 0.0055139496437332, 0.0055139496437332, 0.0055139496437332, 0.0055139496437332, 0.0055139496437332,
   0.0055139496437332, 0.0055139496437332, 0.0055139496437332, 0.0055139496437332, 0.0055139496437332, 0.0197368421052632,
   0.0236842105263158, 0.0177631578947368, 0.0177631578947368, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895,
   0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895,
   0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895,
   0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895,
   0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895,
   0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895,
   0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895,
   0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895,
   0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895,
   0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895,
   0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895,
   0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895,
   0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895,
   0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895,
   0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895,
   0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895,
   0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895,
   0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895, 0.0142105263157895};
  double involunatory_attrition, volunatory_attrition;
	for(unsigned int i = 0; i<=_terms+3; ++i){
    if(i==0)
      _npv[i][4] = 1;
    else{
      involunatory_attrition = _npv[i-1][4] * ATTRITION_ADJ * assign_pd(i);
      if(_terms < 48){
        if(_apr > 0.09)
          volunatory_attrition = base32[i-1];
        else
          volunatory_attrition = base31[i-1];
      }
      else if(_terms < 84){
        if(_apr > 0.09)
          volunatory_attrition = base52[i-1];
        else
          volunatory_attrition = base51[i-1];
      }
      else{
        if(_apr > 0.09)
          volunatory_attrition = base72[i-1];
        else
          volunatory_attrition = base71[i-1];
      };
      volunatory_attrition *=  (_npv[i-1][4] * ATTRITION_ADJ);
      if(i <= _terms)
        _npv[i][4] = _npv[i-1][4] - involunatory_attrition - volunatory_attrition;
      else
        _npv[i][4] = _npv[i-1][4];
    };
  };
};

void ploan::interest_cost(unsigned int i)
{
  if(i>0 && i <= _terms)
    _npv[i][8] = LEVERAGE_RATIO * COF/12.0 * _npv[i-1][2] * _npv[i-1][4];
  else
    _npv[i][8] = 0;
};

double ploan::discount()
{
  double npv = 0;
  for(unsigned int i=0; i<=_terms+3; ++i)
    npv += _npv[i][7] / pow(1 + ROE / 12.0, i);
  return npv;
};

void ploan::pre_collection_cost(unsigned int i)
{
  if(((i<2) ||(_npv[i-2][2] == 0)) || (i > (_terms + 1)))
    _npv[i][6] = 0;
  else{
    double M1_per_open = _monthly_pb * M1_TO_M7_RATIO;
    double grace_per_open = _monthly_pb * M1_TO_M7_RATIO * M1_TO_M7_RATIO;
    _npv[i][6] = _npv[i-2][4] * (M1_per_open * PRE_COLLECTION_PHONE_COST +  grace_per_open * SMS_COST);
  }
};

void ploan::collection_cost(unsigned int i)
{
  if(((i<4) ||(_npv[i-2][2] == 0)) || (i > (_terms + 3)))
    _npv[i][9] = 0;
  else{
    double M1_fix_cost = EXECUTION_ORDER_COST + PROPERTY_INQUIRY_COST + AUDITION_COST + LEGAL_PERSONNEL;
    _npv[i][9] = _npv[i-4][4] * _monthly_pb * (M1_fix_cost + _npv[i-4][2] * EXECUTION_COST_RATE);
  }
};

void ploan::late_fee(unsigned int i)
{
  if(i > (_terms + 1))
    _npv[i][10] = 0;
  else{
    double involunatory_attrition = _npv[i-1][4] * ATTRITION_ADJ * assign_pd(i);
    double involunatory_attrition_M1 = involunatory_attrition * M1_TO_M7_RATIO;
    _npv[i][10] = _npv[i-1][2] * _npv[i][3] *
       ((involunatory_attrition_M1 * RECOVERY_RATIO_4D * DAYS_LATE * 12 / 365 * LATE_FEE_RATE) +
        (involunatory_attrition * RECOVERY_RATIO * DAYS_LATE_M7 * 12 / 365 * LATE_FEE_RATE_M7));
  };
};

void ploan::credit_loss(unsigned int i)
{
  if(i>0 && i < (_terms +1)){
    double involunatory_attrition = _npv[i-1][4] * ATTRITION_ADJ * assign_pd(i);
    double recovery_discount_ratio = 1/ pow((1+ROE/12.0), LEGAL_ACTION_PERIOD);
    _npv[i][5] = _npv[i-1][2] * involunatory_attrition * (1 - recovery_discount_ratio * (1 - EAD));
  }
  else
    _npv[i][5] = 0;
};

void ploan::delta_working_capital(unsigned int i)
{
  credit_loss(i);
  if(i == 0)
    _npv[i][12] = _loan_amount * (LEVERAGE_RATIO - 1);
  else if (i < _terms){
    _npv[i][12] = (_npv[i-1][2] * _npv[i-1][4] - _npv[i][2] * _npv[i][4]) * (1 - LEVERAGE_RATIO) - _npv[i][5];
  }
  else if (i == _terms){
    _npv[i][12] = (_npv[i-1][2]) * _npv[i-1][4] * (1 - LEVERAGE_RATIO) - _npv[i][5];
  }
  else
    _npv[i][12] = 0;
};

void ploan::mgmt_fee(unsigned int i)
{
/*  if((i <= _months_charging_fee) && (i > 0)){
    if(_monthly_fee_type == 1)
      _npv[i][1] = _monthly_fee * _bt_counts;
    else
      _npv[i][1] = _monthly_fee * _npv[i-1][2];
  }
  else if((_months_charging_fee == 1) && (i==0))
    _npv[0][1] = _monthly_fee * _bt_counts;
  else      */
//    _npv[i][1] = 0;
};

void ploan::operation_expense(unsigned int i)
{
  if(i > 0 && (i < (_terms + 1)))
    _npv[i][13] = ACCT_MGMT_FEE * _npv[i-1][4];
  else
    _npv[i][13] = 0;
};

double ploan::npv(double &pb)
{
//initial term cost


//  pb = calculate_pb();

  _monthly_pb =  pb/ 12.0;
  probability_of_stay();
  annunity();
//monthly revenues

  for(unsigned int i = 0; i <= _terms+3; ++i){

     interest_cost(i);
     pre_collection_cost(i);
     collection_cost(i);
     late_fee(i);
     delta_working_capital(i);
     operation_expense(i);

    _npv[i][7] =  _npv[i][0] + _npv[i][12] - _npv[i][6] -
                  _npv[i][8] - _npv[i][9] + _npv[i][10] - _npv[i][13];
    if(i==0)
     _npv[i][7] = _application_fee + _npv[i][12];
  };
  double npv_value = discount();
//  bool dump = dump_series(10, _terms+3);
  return npv_value;
};


bool ploan::dump_series(int type, int period)
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

double ploan::calculate_pb()
{
  double ln_35_047, ln_35_080, ln_35_006_9m, ln_35_008_3m, ln_35_008_9m, ln_35_002_12m, ln_35_004_12m;

  double ln_35_004_12m_q, ln_35_004_12m_q_t1, ln_35_004_12m_q_t4;
  double ln_35_002_12m_t1, ln_35_002_12m_t3, ln_35_080_t, wi006_9mb;
  double ln_35_006_9mb_t1, ln_35_047_t, ln_35_008_9m_r, ln_35_008_9m_r_t2;
  double ln_35_006_9mb, ln_35_006_9mb_t4, pb;

  double compound = pow((1+_monthly_rate), _terms);
  double monthly_repayment_ratio = (compound * (_monthly_rate)) /
                                  (compound -1);
  double monthly_repayment = _loan_amount * monthly_repayment_ratio;

  bool  ln_35_047_isnull = false;
  bool ln_35_080_isnull =  false;
  bool ln_35_006_9m_isnull =  false;
  bool ln_35_008_3m_isnull =  false;
  bool ln_35_008_9m_isnull =  false;
  bool ln_35_002_12m_isnull =  false;
  bool ln_35_004_12m_isnull =  false;
  bool ln_35_006_9mb_t1_isnull = false;
  bool ln35_008_9m_r_isnull = false;
  bool ln35_008_3m_isnull = false;

  if(_ms047 == 0 || _ms047_isnull)
    ln_35_047_isnull = true;
  else
    ln_35_047=(monthly_repayment+(_ms093+(_ms094+_ms105)*0.35)*1000) / _ms047;

  if(_ms080 == 0 || _ms080_isnull)
    ln_35_080_isnull = true;
  else
    ln_35_080=(monthly_repayment+(_ms093+(_ms094+_ms105)*0.35)*1000) / _ms080;

  if(_wi006_9m == 0 || _wi006_9m_isnull)
    ln_35_006_9m_isnull = true;
  else
    ln_35_006_9m=(monthly_repayment+(_ms093+(_ms094+_ms105)*0.35)*1000) / _wi006_9m;

  if(_wi008_3m == 0 || _wi008_3m_isnull)
    ln35_008_3m_isnull = true;
  else
    ln_35_008_3m=(monthly_repayment+(_ms093+(_ms094+_ms105)*0.35)*1000) / _wi008_3m;

  if(_wi008_9m == 0 || _wi008_9m_isnull)
    ln_35_008_9m_isnull = true;
  else
    ln_35_008_9m=(monthly_repayment+(_ms093+(_ms094+_ms105)*0.35)*1000) / _wi008_9m;

  if(_wi002_12m == 0 || _wi002_12m_isnull)
    ln_35_002_12m_isnull = true;
  else
    ln_35_002_12m=(monthly_repayment+(_ms093+(_ms094+_ms105)*0.35)*1000)/ _wi002_12m;

  if(_wi004_12m == 0 || _wi004_12m_isnull)
    ln_35_004_12m_isnull = true;
  else
    ln_35_004_12m=(monthly_repayment+(_ms093+(_ms094+_ms105)*0.35)*1000)/ _wi004_12m;

  if(ln_35_004_12m_isnull)
    ln_35_004_12m_q_t4 = 300;
  else{
    ln_35_004_12m_q = pow(ln_35_004_12m, 2);
    ln_35_004_12m_q_t1 = ln_35_004_12m_q/1000;
    if (ln_35_004_12m_q_t1>300)
      ln_35_004_12m_q_t4=300;
  	else
	    ln_35_004_12m_q_t4=ln_35_004_12m_q_t1;
  };

  if(ln_35_002_12m_isnull)
    ln_35_002_12m_t3=200;
  else{
    ln_35_002_12m_t1=ln_35_002_12m/1000;
	  if(ln_35_002_12m_t1>200)
	    ln_35_002_12m_t3=200;
	  else
	    ln_35_002_12m_t3=ln_35_002_12m_t1;
  };

  if(ln_35_080_isnull)
    ln_35_080_t=700;
	else if(ln_35_080<0)
	  ln_35_080_t=700;
	else if(ln_35_080>10000)
	  ln_35_080_t=10000;
	else
	  ln_35_080_t=ln_35_080;

  if((_wi006_9m==0) && ((_wi008_3m>0) && (!_wi008_3m_isnull)))
    wi006_9mb=13;
	else
	  wi006_9mb=_wi006_9m;

   if((_wi006_9m_isnull) || (wi006_9mb == 0))
     ln_35_006_9mb_t1_isnull = true;
   else{
     ln_35_006_9mb=(monthly_repayment+(_ms093+(_ms094+_ms105)*0.35)*1000) / wi006_9mb;
     ln_35_006_9mb_t1=ln_35_006_9mb/1000;
   };
  if(ln_35_006_9mb_t1_isnull)
    ln_35_006_9mb_t4=1;
	else
	  ln_35_006_9mb_t4=0;

  if(ln_35_047_isnull)
    ln_35_047_t=260;
	else if(ln_35_047>260)
	  ln_35_047_t=260;
	else
	  ln_35_047_t=ln_35_047;

  if(ln_35_008_9m < 0 || ln_35_008_9m_isnull)
    ln35_008_9m_r_isnull = true;
  else
    ln_35_008_9m_r= pow(ln_35_008_9m,0.5);

  if(ln35_008_9m_r_isnull)
    ln_35_008_9m_r_t2=1;
	else
	  ln_35_008_9m_r_t2=0;

  _ci_score	=	-0.0376	+
              ln_35_004_12m_q_t4	*	0.00032495	+
              ln_35_002_12m_t3	*	0.00025644	+
              ln_35_080_t	*	0.00000761	+
              ln_35_006_9mb_t4	*	0.03049	+
              ln_35_047_t	*	0.00011161	+
              ln_35_008_9m_r_t2	*	0.01753	;
  _cpdaco_score = 0.00742	+
                  _ci_score * _pdaco_score *	7.73177 +
                  _pdaco_score	*	0.28397;


  pb = assign_pb();
	return pb;
};

double ploan::assign_pb()
{
  double pb;
  if(_cpdaco_score <= -0.00621522) pb = 0.00456270448895028;
  else if(_cpdaco_score <= -0.001009662) pb = 0.00614059272559853;
  else if(_cpdaco_score <= 0.0016614313) pb = 0.0077276017034991;
  else if(_cpdaco_score <= 0.0036727668) pb = 0.00931461068140074;
  else if(_cpdaco_score <= 0.0054064523) pb = 0.0109016196593226;
  else if(_cpdaco_score <= 0.0069567309) pb = 0.0124886286374571;
  else if(_cpdaco_score <= 0.0086160987) pb = 0.0140756376170916;
  else if(_cpdaco_score <= 0.0100599299) pb = 0.0156626466046451;
  else if(_cpdaco_score <= 0.0118217596) pb = 0.0172405348840249;
  else if(_cpdaco_score <= 0.0141499981) pb = 0.0188184232819514;
  else if(_cpdaco_score <= 0.01637437) pb = 0.0204054327954711;
  else if(_cpdaco_score <= 0.0182075362) pb = 0.0219924433620352;
  else if(_cpdaco_score <= 0.0200870556) pb = 0.0235794566365131;
  else if(_cpdaco_score <= 0.0225567537) pb = 0.0251664763627067;
  else if(_cpdaco_score <= 0.0251618186) pb = 0.0267535104921789;
  else if(_cpdaco_score <= 0.0273908372) pb = 0.0283405750227193;
  else if(_cpdaco_score <= 0.0305383207) pb = 0.0299185788778192;
  else if(_cpdaco_score <= 0.0333893488) pb = 0.0314966988130079;
  else if(_cpdaco_score <= 0.0369613512) pb = 0.0330841569850159;
  else if(_cpdaco_score <= 0.0403064067) pb = 0.0346720036342812;
  else if(_cpdaco_score <= 0.0443997372) pb = 0.036260525956462;
  else if(_cpdaco_score <= 0.0479082086) pb = 0.0378501917137932;
  else if(_cpdaco_score <= 0.0515019682) pb = 0.0394417450576612;
  else if(_cpdaco_score <= 0.0558887298) pb = 0.0410363448184341;
  else if(_cpdaco_score <= 0.0598270422) pb = 0.0426265434415847;
  else if(_cpdaco_score <= 0.0644071946) pb = 0.0442241220633064;
  else if(_cpdaco_score <= 0.0685388201) pb = 0.0458422388084545;
  else if(_cpdaco_score <= 0.073968267) pb = 0.0474773614892305;
  else if(_cpdaco_score <= 0.077801493) pb = 0.049137619573412;
  else if(_cpdaco_score <= 0.0825236976) pb = 0.0508345162169793;
  else if(_cpdaco_score <= 0.0881492006) pb = 0.052584132354534;
  else if(_cpdaco_score <= 0.0933302255) pb = 0.0544086958938513;
  else if(_cpdaco_score <= 0.0991208145) pb = 0.0563386085548245;
  else if(_cpdaco_score <= 0.1047230787) pb = 0.0584024027519682;
  else if(_cpdaco_score <= 0.1106139114) pb = 0.0606656488370072;
  else if(_cpdaco_score <= 0.1171448678) pb = 0.0632154169381179;
  else if(_cpdaco_score <= 0.1243742454) pb = 0.066136073114141;
  else if(_cpdaco_score <= 0.1316462697) pb = 0.0695540945881063;
  else if(_cpdaco_score <= 0.1387171246) pb = 0.0736338309373768;
  else if(_cpdaco_score <= 0.146798664) pb = 0.0785873619431643;
  else if(_cpdaco_score <= 0.1570618205) pb = 0.084686557319047;
  else if(_cpdaco_score <= 0.169866044) pb = 0.0922272547606849;
  else if(_cpdaco_score <= 0.1816242536) pb = 0.101677154775113;
  else if(_cpdaco_score <= 0.1949277451) pb = 0.113649567310443;
  else if(_cpdaco_score <= 0.2112718793) pb = 0.128787542254837;
  else if(_cpdaco_score <= 0.2292424804) pb = 0.147950603353279;
  else if(_cpdaco_score <= 0.2537719927) pb = 0.172204431771322;
  else if(_cpdaco_score <= 0.2904674278) pb = 0.202864092837363;
  else if(_cpdaco_score <= 0.3577100549) pb = 0.241545082637999;
  else if(_cpdaco_score <= 1.3074021536) pb = 0.290223394278218;
  return pb;
};

void ploan::annunity()
{
  double compound = pow((1+_npv[0][3]), _terms);
  double monthly_repayment_ratio = (compound * _npv[0][3]) /
                                  (compound -1);
  double monthly_repayment = _loan_amount * monthly_repayment_ratio;
  double principal_repayment;

  for(unsigned int i=1; i<=_terms; ++i){
    _npv[i][0] = _npv[i][3] * _npv[i-1][2]; //interest repayment
    principal_repayment = monthly_repayment - _npv[i][0];
    _npv[i][0] *= _npv[i][4];
    _npv[i][1] = principal_repayment;
    _npv[i][2] = _npv[i-1][2] - _npv[i][1];
    if(_npv[i][2]<=0){
      _npv[i][1] += _npv[i][2];
      _npv[i][2]=0;
      break;
    };
  };
  _npv[_terms][1] += _npv[_terms][2];
  _npv[_terms][2] =0;

  for(unsigned int i = _terms + 1; i < (_terms + 4); ++i){
    _npv[i][0] = 0;
    _npv[i][1] = 0;
    _npv[i][2] = 0;
  };
};

double ploan::assign_pd(unsigned int i)
{
  double return_value;
  switch(_year_for_pd){
    case 0:
      if(i<7)
       return_value = 2 * _monthly_pb * _terms / (_terms + 1) * (i /_terms);
      else
        return_value = 0;
      break;
    case 1:
      if(i < 8)
        return_value = _monthly_pb * 12 / 7.85 * i / 7;
      else
        return_value = _monthly_pb * 12 / 7.85 * 0.77;
      break;
    case 2:
      if(i < 9)
        return_value = _monthly_pb * 12 / 7.58 * i / 8;
      else
        return_value = _monthly_pb * 12 / 7.58 * 0.77;
      break;
    case 3:
      if(i < 10)
        return_value = _monthly_pb * 12 / 7.31 * i / 9;
      else
        return_value = _monthly_pb * 12 / 7.31 * 0.77;
      break;
    case 4:
      if(i <11)
        return_value = _monthly_pb * 12 / 7.04 * i / 10;
      else
        return_value = _monthly_pb * 12 / 7.04 * 0.77;
      break;
    case 5:
      if(i < 12)
        return_value = _monthly_pb * 12 / 6.77 * i / 11;
      else
        return_value = _monthly_pb * 12 / 6.77 * 0.77;
      break;
    case 6:
      if(i < 13)
        return_value = _monthly_pb * 12 / 6.5 * i / 12;
      else
        return_value = _monthly_pb * 12 / 6.5 * 0.77;
      break;
    default:
      if(i < 13)
        return_value = _monthly_pb * 12 / 6.5 * i / 12;
      else
        return_value = _monthly_pb * 12 / 6.5 * 0.77;
      break;
  };
  return return_value;
};

