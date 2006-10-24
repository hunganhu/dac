//---------------------------------------------------------------------------
#pragma hdrstop

#include "NPV.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void loc::get_product_feature()
{
  int periods = NPV_PERIODS + 1;

  int upper = MAX_CELL - 1;
  int lower = 0;
  int index = (upper + lower) / 2;
  while ((product[index][7] != _test_cell)){
    if(product[index][7] > _test_cell)
      upper = index - 1;
    else
      lower = index + 1;
    index = (upper + lower) / 2;
  }

  double teaser_rate = product[index][0];
  double go_to_rate = product[index][1];
  _mgmt_fee = product[index][4];
  _mgmt_fee_periods = product[index][5];
  _teaser_periods = product[index][2];

  for(int i=0; i<periods; ++i){
    if(i<=_teaser_periods)
      _series[i][3] =  teaser_rate/12.0;
    else
      _series[i][3] = go_to_rate/12.0;
  };
};

loc::loc(int test_cell, double gav, double nav, double existing_mortgage,
         AnsiString zip, double principal, double balance,
         int propensity_decile, double risk,
         AnsiString msn, AnsiString idn, AnsiString time_stamp,
         AnsiString *error_message)
         : _msn(msn), _idn(idn), _time_stamp(time_stamp)
{
    _status = 0;
  try{
    _test_cell = test_cell;
    _gav = gav<=0? 0.000000000000001 : gav / 1000.0;
    _existing_mortgage = existing_mortgage / 1000.0;
    _nav = nav<=0? 0.000000000000001 : nav / 1000.0;
    _principal = principal / 1000.0;
    _outstanding_unsecured_bal = balance;
    _coe = COE / 12.0;
    _cof = COF / 12.0;
    _leverage = LEVERAGE;
    _periods = NPV_PERIODS;
    _commission = COMMISSION;
    _maintenance_cost = Maintenance_cost / 12.0;
    _personal_risk = risk;
    _propensity_decile = propensity_decile;
    _home_equity = _nav - _existing_mortgage;
    _series = new double [NPV_PERIODS+1][TEST+1];
    for(int i=0; i<=NPV_PERIODS; ++i)
      for(int j=0; j<TEST; ++j)
        _series[i][j] = 0;

    get_product_feature();
    get_recovery_ratio(zip);


    _series[0][4] = 1; //probability of stay
    _status += 1;
  }
  catch(bad_alloc){
    (*error_message)+="記憶體不足";
    _status = 0;
  }
};

loc::loc()
{
  _status = 0;
};

loc::~loc()
{
  if (_status != 0)
    delete[] _series;
};

void loc::attrition(double pb, bool voluntary)
{
  double voluntary_close = 0;
  double monthly_pb = pb / 12.0;
  for(int i = 1; i <=_periods; ++i){
    if(voluntary){
      if(i < 120)
        voluntary_close = 0.00004 * i + 0.0063;
      else if(i==120)
        voluntary_close = 0.025;
      else if(i==179)
        voluntary_close = 0.05;
      else if(i==180)
        voluntary_close = 0.5;
      else if(i>180)
        voluntary_close = 0.05;
      else
        voluntary_close = 0.0125 + 0.0002 * (i - 120);
    };
    _series[i][4] = _series[i-1][4] * (1 - voluntary_close - monthly_pb);

    if(i== (_teaser_periods + 1))
      _series[i][4] -= 0.3 * _series[i-1][4];
  };
};

double loc::secured_pb(double line, unsigned int filter)
{
//  double loan_ratio2 = ((_existing_mortgage + line) /_gav )* 100;
  double loan_ratio2 = ((_existing_mortgage + line) /_nav )* 100;
  double loan_ratio2_t;
  if(loan_ratio2 > 200)
  	loan_ratio2_t = 200;
  else if (loan_ratio2 < 100)
  	loan_ratio2_t = 100;
  else
  	loan_ratio2_t = loan_ratio2;

//  double loan_diff2 = ((_existing_mortgage + line) - _gav);
  double loan_diff2 = ((_existing_mortgage + line) - _nav);
  int loan_diff2_1200k= loan_diff2>1200 ? 1 : 0;

  double total_approved_amt=(_existing_mortgage + line);
  double total_approved_amt_nt;
  if (total_approved_amt>9000)
    total_approved_amt_nt=9000;
	else
	  total_approved_amt_nt=total_approved_amt;

	int loan_ratio2_100 = (loan_ratio2>100) ? 1 : 0;

	double app_score= -0.61555 +
	                  loan_ratio2_t * 0.00626 +
                    loan_diff2_1200k * 0.23568 +
                    total_approved_amt_nt * 0.00000587 +
                    loan_ratio2_100 * 0.05567;

  double score;
  double pb = 10000;;
  if(app_score > 0.04784 && (filter == 0x80 || filter == 0)){
  	score =  -0.00170 + _personal_risk * 0.12247 + app_score * 0.21686;
	  if (score <= -0.002254216) pb = 0.0000870644024592;
	  else if (score <= -0.00050731) pb = 0.000193692722589376;
	  else if (score <= 0.0021048076) pb = 0.000297655262147656;
	  else if (score <= 0.0041566377) pb = 0.000417178424294506;
	  else if (score <= 0.0050786692) pb = 0.000520248360300229;
    else if (score <= 0.0055307695) pb = 0.000605503252548778;
    else if (score <= 0.0060615707) pb = 0.000727805449611228;
    else if (score <= 0.006546382) pb = 0.000844679487157763;
    else if (score <= 0.0066395118) pb = 0.000904646020866708;
    else if (score <= 0.0067845982) pb = 0.000966490007447263;
    else if (score <= 0.0069688337) pb = 0.00104274504969745;
    else if (score <= 0.0071914555) pb = 0.00113294155430026;
    else if (score <= 0.0073071934) pb = 0.00123550782110137;
    else if (score <= 0.0075755501) pb = 0.0013468318421912;
    else if (score <= 0.007718263) pb = 0.00145524805789459;
    else if (score <= 0.0078658411) pb = 0.00157623528454038;
    else if (score <= 0.0082168665) pb = 0.00172472719718616;
    else if (score <= 0.0084878042) pb = 0.00187523404482858;
    else if (score <= 0.008696812) pb = 0.0020163151881282;
    else if (score <= 0.0089905977) pb = 0.00216120056371085;
    else if (score <= 0.0091869422) pb = 0.00230834569862786;
    else if (score <= 0.0095722426) pb = 0.00246772213990264;
    else if (score <= 0.0098643554) pb = 0.00265182309205689;
    else if (score <= 0.0102947898) pb = 0.00286506768420182;
    else if (score <= 0.010468706) pb = 0.00308302137895369;
    else if (score <= 0.0107525197) pb = 0.00328934949555068;
    else if (score <= 0.0110744867) pb = 0.00363009373983878;
    else if (score <= 0.0117990196) pb = 0.00416234698088642;
    else if (score <= 0.0126573908) pb = 0.00480135284731245;
    else if (score <= 0.0134294778) pb = 0.00554466145052087;
    else if (score <= 0.014505726) pb = 0.00640850961395341;
    else if (score <= 0.0157531968) pb = 0.0074118713099815;
    else if (score <= 0.0177938305) pb = 0.00857890462035907;
    else if (score <= 0.0241700612) pb = 0.00995329943844833;
    else if (score <= 0.0265895917) pb = 0.0110863356014752;
    else if (score <= 0.0347371483) pb = 0.0118646032483203;
    else if (score <= 0.0395760805) pb = 0.0128135918333278;
    else if (score <= 0.0448282572) pb = 0.0140805324958549;
    else if (score <= 0.0527202661) pb = 0.0159717750918589;
    else if (score <= 0.0650472446) pb = 0.0191258637139272;
    else if (score <= 0.0675659311) pb = 0.0218802690208409;
    else if (score <= 0.0731931599) pb = 0.022906515630648;
    else if (score <= 0.0831266313) pb = 0.0240492126646248;
    else if (score <= 0.0850366378) pb = 0.0253233806144397;
    else if (score <= 0.0899879153) pb = 0.026745989899357;
    else if (score <= 0.0928550313) pb = 0.0283362131269808;
    else if (score <= 0.0938717116) pb = 0.0301157098526485;
    else if (score <= 0.1001386823) pb = 0.0321089480065363;
    else if (score <= 0.1052818675) pb = 0.0343435666900969;
    else if (score <= 0.1250375414) pb = 0.036850785643735;
    else if (score <= 0.1264213006) pb = 0.0396658673642297;
    else if (score <= 0.1264594696) pb = 0.0428286386130065;
    else if (score <= 0.1375026305) pb = 0.0463840789157917;
    else if (score <= 0.1486017668) pb = 0.0465319007171763;
    else if (score <= 0.1877793656) pb = 0.0663755825770675;
    else if (score <= 0.1972354405) pb = 0.102485521847617;
    else pb = 0.20746110669922;
  }
  else{
    score = app_score;
		if (score <= 0.026886) pb = 0.00107500471846493;
    else if (score <= 0.0430285) pb = 0.00121454766287049;
    else if (score <= 0.04567) pb = 0.00168776571927645;
    else if (score <= 0.047431) pb = 0.00194537315880961;
    else if (score <= 0.048605) pb = 0.00219246170922624;
    else if (score <= 0.049779) pb = 0.00244770787079714;
    else if (score <= 0.05154) pb = 0.00274089181639693;
    else if (score <= 0.0524205) pb = 0.00318940610802513;
    else if (score <= 0.055062) pb = 0.0038286587578834;
    else if (score <= 0.056823) pb = 0.00444483581907027;
    else if (score <= 0.05741) pb = 0.00507198147664856;
    else if (score <= 0.059171) pb = 0.00588618642677453;
    else if (score <= 0.060345) pb = 0.00644637046662145;
    else if (score <= 0.062106) pb = 0.00680473090897149;
    else if (score <= 0.06328) pb = 0.0137;
    else if (score <= 0.0944537957) pb = 0.02302831323956;
    else if (score <= 0.1264688197) pb = 0.0250247867259468;
    else if (score <= 0.1486173245) pb = 0.0270096856389131;
    else if (score <= 0.1728627528) pb = 0.0290536632619643;
    else if (score <= 0.1750260385) pb = 0.0305900624782711;
    else if (score <= 0.1811933373) pb = 0.0325732082067571;
    else if (score <= 0.1957243277) pb = 0.0355537062251439;
    else if (score <= 0.2043590099) pb = 0.0380515933131802;
    else if (score <= 0.2160809837) pb = 0.0400818384049177;
    else if (score <= 0.254935) pb = 0.0420708506497511;
    else if (score <= 0.25787) pb = 0.0440813526944908;
    else if (score <= 0.260805) pb = 0.046105578691751;
    else if (score <= 0.26374) pb = 0.0485762943080416;
    else if (score <= 0.269023) pb = 0.0510800144308181;
    else if (score <= 0.272545) pb = 0.0531419781884971;
    else if (score <= 0.2853486693) pb = 0.0556903118450965;
    else if (score <= 0.3217339014) pb = 0.0586733357817978;
    else if (score <= 0.3831913158) pb = 0.0617066623185411;
    else if (score <= 0.4532126772) pb = 0.0646622382850176;
    else if (score <= 0.533192328) pb = 0.0676406357578471;
    else if (score <= 0.5882675709) pb = 0.0702436272069268;
    else if (score <= 0.6263275389) pb = 0.0726909697944507;
    else if (score <= 0.6719456843) pb = 0.0761797989298419;
    else if (score <= 0.7272420576) pb = 0.0807014685541205;
    else if (score <= 0.8066024106) pb = 0.0867395818490491;
    else if (score <= 0.8313990564) pb = 0.0933737011473251;
    else if (score <= 0.8712851701) pb = 0.0992622929024332;
    else if (score <= 0.8980124327) pb = 0.105528116533261;
    else if (score <= 0.9195535979) pb = 0.111570209656598;
    else if (score <= 0.9289064132) pb = 0.116487545551892;
    else if (score <= 0.9350983094) pb = 0.121626366555369;
    else if (score <= 0.9411707748) pb = 0.127444119563389;
    else if (score <= 0.944236) pb = 0.133779950335277;
    else if (score <= 0.946358946) pb = 0.140424076481398;
    else if (score <= 0.94849175) pb = 0.147133085141741;
    else if (score <= 0.9524327826) pb = 0.153881304788229;
    else if (score <= 0.9541523943) pb = 0.160360061946962;
    else if (score <= 0.9566163165) pb = 0.167112942828692;
    else if (score <= 0.9599121443) pb = 0.173829330324587;
    else if (score <= 0.9642685621) pb = 0.180495440119546;
    else if (score <= 0.9707432134) pb = 0.190136136759279;
    else if (score <= 0.9786029453) pb = 0.204261403261604;
    else if (score <= 0.98063) pb = 0.220978835617639;
    else pb = 0.242538478641798;
  };
  return pb;
};

double loc::unsecured_pb(double line)
{
  double pb = 10000;
  if (_personal_risk <= -0.03231) pb= 0.001402016;
  else if (_personal_risk <= -0.02275) pb= 0.002803476;
  else if (_personal_risk <= -0.01479) pb= 0.004205993;
  else if (_personal_risk <=  -0.00919) pb= 0.005610332;
  else if (_personal_risk <= -0.00438) pb= 0.007017814;
  else if (_personal_risk <=  0.00101) pb= 0.008430714;
  else if (_personal_risk <=  0.00624) pb= 0.009852955;
  else if (_personal_risk <=  0.01245) pb= 0.011291301;
  else if (_personal_risk <=  0.01836) pb= 0.012757416;
  else if (_personal_risk <=  0.02482) pb= 0.014271407;
  else if (_personal_risk <=  0.03219) pb= 0.015867943;
  else if (_personal_risk <=  0.03963) pb= 0.017606797;
  else if (_personal_risk <=  0.04759) pb= 0.01959103;
  else if (_personal_risk <=  0.05585) pb= 0.021998328;
  else if (_personal_risk <=  0.06657) pb= 0.025135048;
  else if (_personal_risk <=  0.07865) pb= 0.029529392;
  else if (_personal_risk <=  0.09435) pb= 0.036092056;
  else if (_personal_risk <=  0.11509) pb= 0.0463932;
  else if (_personal_risk <=  0.15002) pb= 0.06314;
  else if (_personal_risk >  0.15002) pb= 0.091;

  double os_unsecured = _outstanding_unsecured_bal + line;
  os_unsecured = os_unsecured > Loan_Base ? os_unsecured : Loan_Base;
  pb = pb * os_unsecured / Loan_Base;
  return pb;
};

void loc::propensity(double line, int p_decile)
{
  int line_segment = (line / 200) + 1;
  unsigned long line_int = line;
  if((line_int % 200 == 0) && (line == line_int))
    --line_segment;
  if(line_segment > 25)
  	line_segment = 25;

  double utilization_slope = 0;
  if(p_decile <= 7){
    switch(line_segment){
      case 1:
        utilization_slope = 0.804205;
        break;
      case 2:
        utilization_slope = 0.712757032;
        break;
      case 3:
        utilization_slope = 0.646003838;
        break;
      case 4:
        utilization_slope = 0.59478741;
        break;
      case 5:
        utilization_slope = 0.553572386;
        break;
      case 6:
        utilization_slope = 0.518953323;
        break;
      case 7:
        utilization_slope = 0.488794633;
        break;
      case 8:
        utilization_slope = 0.461729447;
        break;
      case 9:
        utilization_slope = 0.436864;
        break;
      case 10:
        utilization_slope = 0.413600887;
        break;
      case 11:
        utilization_slope = 0.391531839;
        break;
      case 12:
        utilization_slope = 0.370371653;
        break;
      case 13:
        utilization_slope = 0.349916805;
        break;
      case 14:
        utilization_slope = 0.330019067;
        break;
      case 15:
        utilization_slope = 0.310568394;
        break;
      case 16:
        utilization_slope = 0.291481614;
        break;
      case 17:
        utilization_slope = 0.274335637;
        break;
      case 18:
        utilization_slope = 0.259094768;
        break;
      case 19:
        utilization_slope = 0.245458201;
        break;
      case 20:
        utilization_slope = 0.233185291;
        break;
      case 21:
        utilization_slope = 0.22208123;
        break;
      case 22:
        utilization_slope = 0.211986628;
        break;
      case 23:
        utilization_slope = 0.202769818;
        break;
      case 24:
        utilization_slope = 0.194321076;
        break;
      case 25:
        utilization_slope = 0.186548233;
        break;
    };
  }
  else if(p_decile <=9){
    switch(line_segment){
      case 1:
        utilization_slope = 0.848448333;
        break;
      case 2:
        utilization_slope = 0.791660812;
        break;
      case 3:
        utilization_slope = 0.736041088;
        break;
      case 4:
        utilization_slope = 0.688383212;
        break;
      case 5:
        utilization_slope = 0.64758659;
        break;
      case 6:
        utilization_slope = 0.612096337;
        break;
      case 7:
        utilization_slope = 0.580643564;
        break;
      case 8:
        utilization_slope = 0.552275398;
        break;
      case 9:
        utilization_slope = 0.526288032;
        break;
      case 10:
        utilization_slope = 0.502160564;
        break;
      case 11:
        utilization_slope = 0.479504183;
        break;
      case 12:
        utilization_slope = 0.458025521;
        break;
      case 13:
        utilization_slope = 0.43750066;
        break;
      case 14:
        utilization_slope = 0.417756724;
        break;
      case 15:
        utilization_slope = 0.398658742;
        break;
      case 16:
        utilization_slope = 0.380100205;
        break;
      case 17:
        utilization_slope = 0.3631205;
        break;
      case 18:
        utilization_slope = 0.34774519;
        break;
      case 19:
        utilization_slope = 0.333749096;
        break;
      case 20:
        utilization_slope = 0.320948063;
        break;
      case 21:
        utilization_slope = 0.30918992;
        break;
      case 22:
        utilization_slope = 0.298347752;
        break;
      case 23:
        utilization_slope = 0.288314803;
        break;
      case 24:
        utilization_slope = 0.27900059;
        break;
      case 25:
        utilization_slope = 0.270327877;
        break;
    };
  }
  else if(p_decile ==10){
    switch(line_segment){
      case 1:
        utilization_slope = 0.866145667;
        break;
      case 2:
        utilization_slope = 0.826394402;
        break;
      case 3:
        utilization_slope = 0.787421668;
        break;
      case 4:
        utilization_slope = 0.753126737;
        break;
      case 5:
        utilization_slope = 0.719092916;
        break;
      case 6:
        utilization_slope = 0.68689373;
        break;
      case 7:
        utilization_slope = 0.656749654;
        break;
      case 8:
        utilization_slope = 0.628514938;
        break;
      case 9:
        utilization_slope = 0.601957726;
        break;
      case 10:
        utilization_slope = 0.576846275;
        break;
      case 11:
        utilization_slope = 0.552974115;
        break;
      case 12:
        utilization_slope = 0.530164764;
        break;
      case 13:
        utilization_slope = 0.508269714;
        break;
      case 14:
        utilization_slope = 0.48716458;
        break;
      case 15:
        utilization_slope = 0.466745119;
        break;
      case 16:
        utilization_slope = 0.446923701;
        break;
      case 17:
        utilization_slope = 0.428314141;
        break;
      case 18:
        utilization_slope = 0.411261577;
        break;
      case 19:
        utilization_slope = 0.39571315;
        break;
      case 20:
        utilization_slope = 0.381468826;
        break;
      case 21:
        utilization_slope = 0.368363369;
        break;
      case 22:
        utilization_slope = 0.356258983;
        break;
      case 23:
        utilization_slope = 0.345039763;
        break;
      case 24:
        utilization_slope = 0.334607453;
        break;
      case 25:
        utilization_slope = 0.324878164;
        break;
    };
  };
  double revolver_ratio = 0.0268 * p_decile + 0.711;

  double revolver_slope;
  for(int i = 1; i<=NPV_PERIODS; ++i){
  	switch (i){
  		case 1:
  			revolver_slope = 0.857221542227662;
  			break;
  		case 2:
  			revolver_slope = 0.885174418604651;
  			break;
  		case 3:
  			revolver_slope = 0.951232211037834;
  			break;
  		case 4:
  			revolver_slope = 0.994186046511628;
  			break;
  		default:
  			revolver_slope = 1;
  			break;
  	};
    double usage = revolver_ratio * utilization_slope * revolver_slope * line;
    _series[i][2] = usage;
  };
};

void loc::interest_revenue()
{
  for(int i=1; i<=_periods;++i)
    _series[i][0] = _series[i][3] * _series[i][2];
};

void loc::late_fee(double pb)
{
  double late = pb / 12.0 * 2.0;

  for(int i=3; i<=_periods; ++i)
    _series[i][5] = _series[i-1][0] * _series[i-1][3] * LATE_FEE * late;
};

void loc::interest_cost()
{
  for(int i=1; i<=_periods;++i)
    _series[i][8] = _cof * _series[i][2] * _leverage;
};

void loc::late_cost(double pb)
{
  double late = pb / 12.0 * 2.0;
  for(int i=3; i<=_periods;++i)
    _series[i][1] = M2_3_EXPENSE * late;
};

void loc::collection_cost(double pb, double line, bool secured)
{
  double discounting_factor = 1 / pow(1+_coe, Time_to_recover);
  double expected_recovery = (secured ?
  ((_nav * _recovery_ratio) - _existing_mortgage) :
  line * Default_recovery) * discounting_factor;
  expected_recovery = expected_recovery > 0 ? expected_recovery : 0;
  double wacc = 1.5 * (_cof * _leverage + _coe * (1 - _leverage));
  double monthly_pb = pb/12.0;
  for(int i=5; i<=_periods; ++i)
    _series[i][9] = monthly_pb *
                   (M4P_EXPENSE + (line * Legal_fee_rate) +
                    (wacc * expected_recovery));
};

void loc::credit_loss(double pb, double line, bool secured)//6
{
  double monthly_pb = pb / 12.0;
  double discounting_factor = 1 / pow(1+_coe, Time_to_recover);
  double expected_recovery = (secured ?
  ((_nav * _recovery_ratio) - _existing_mortgage) :
  line * Default_recovery) * discounting_factor;
  expected_recovery = expected_recovery > 0 ? expected_recovery : 0;
  for(int i=1; i<=_periods; ++i){
    _series[i][6] = monthly_pb *
      ((line > expected_recovery) ? line - expected_recovery : 0);
  }
};

double loc::discount(int periods, double factor)
{
  double npv = 0;
  for(int i=0; i<=periods; ++i)
    npv += _series[i][7] / pow(1 + factor, i);
  return npv;
};

double loc::max_npv_line(int trails, double &npv_value, double &pb, double npv[][3], unsigned int filter)
{
  double max_npv = npv[0][1];
  double max_line = npv[0][0];
  double max_npv_pb = npv[0][2];
  bool over_limit = false;
  double line_ceiling;
  if(filter == 0)
    line_ceiling = Max_Granting_Ratio* _nav;
  else if(filter == 0x80)
    line_ceiling = Max_Granting_Ratio_CASHCARD * _nav;
  else
    line_ceiling = Max_Granting_Ratio_DATA * _nav;

  for(int i=0; i<trails;++i){
    double line = npv[i][0];
    double total_exposure = _existing_mortgage + line;
    if(total_exposure > line_ceiling){
      if(i==0)
        over_limit = true;
      break;
    }
    if(max_npv < npv[i][1]){
      max_npv = npv[i][1];
      max_line = npv[i][0];
      max_npv_pb = npv[i][2];
    }
  };
  if(max_npv < 0 || over_limit){
    max_npv = 0;
    max_line = 0;
    max_npv_pb = 0;
  };
  npv_value = max_npv;
  pb = max_npv_pb;
  return max_line;
}

double loc::npv(bool secured, double &max_npv_value, double &pb_max_npv, unsigned int filter, double secured_line)
{
  bool out = false;
  int cap;
  if(!secured && filter != 0)
    out = true;
  if(secured && (filter != 0 && filter != 0x80))
    out = true;
  if(out){
    max_npv_value = 0;
    pb_max_npv = 0;
    return 0;
  };
  int upper = secured ? MAX_LENDING_SECURED : MAX_LENDING_UNSECURED;
  if(!secured){
  	upper = (upper > secured_line) ? secured_line : upper;

    if(_personal_risk > 0.04759){
      if (_principal >= 100)
        cap = _principal < 500 ? _principal : 500;
      else
        cap = 500;
      upper = (upper > cap) ? cap : upper;
    }
  }
  int trails = (upper - MIN_LENDING_AMOUNT) / 10.0 + 1;

  double (*npv)[3] = new double [trails][3];
  for(int i = 0; i < trails; ++i){
  	npv[i][0] = 0;
  	npv[i][1] = 0;
  	npv[i][2] = 0;
  }
//0 for line assigned, 1 for NPV

  for(int i = 0; i < trails; ++i){
    double line = MIN_LENDING_AMOUNT + i * 10;
    double pb;
    if(secured){
      _series[0][7] = (line * (_commission + Set_lien_fee) + 2.25)*(-1); //對保2.25 + 代書 3 = 5.25; 代書 3 與set_lien_fee因向申貸人收費而打平
      pb = secured_pb(line, filter);
    }
    else{
      _series[0][7] = (line * _commission /*+ 3*/)*(-1);  //代書 3
      pb = unsecured_pb(line);
    }
    propensity(line, _propensity_decile);//2
    interest_revenue();//0
    attrition(pb);//4
    late_fee(pb);//5
    interest_cost();//8
    late_cost(pb);//1
    collection_cost(pb, line, secured);//9
    credit_loss(pb, line, secured);//6

    int last_month=1;
    double mgmt_fee = _mgmt_fee;
    double extension_cost;
    for(int k=1; k<=_periods; ++k){ //k is for cash flow schedule
      if(k>_mgmt_fee_periods) mgmt_fee = 0;
      extension_cost = (k%12==0)? Extension_expense : 0;
      _series[k][10] =  (_leverage - 1) *
                      (_series[k][2] * _series[k][4] -
                      _series[k-1][2] * _series[k-1][4]) -
                      _series[k][6] * _series[k-1][4];
      _series[k][7] = _series[k][4] *
                     (_series[k][0] + mgmt_fee - _series[k][8]
                      - _series[k][1] - _series[k][9] - extension_cost)
                      + (_series[k][5] - _maintenance_cost) * _series[k-1][4]
                      + _series[k][10];

      last_month = k;
      if(_series[k][2] == 0)
        break;
    }//end of for k

    if(_series[last_month][2] != 0){
      _series[last_month][2] = 0;
      _series[last_month][10] = (_leverage - 1) *
                      (_series[last_month][2] * _series[last_month][4] -
                      _series[last_month-1][2] * _series[last_month-1][4]) -
                      _series[last_month][6] * _series[last_month-1][4];
      _series[last_month][7] = _series[last_month][4] *
                     (_series[last_month][0] + mgmt_fee - _series[last_month][8]
                      - _series[last_month][1] - _series[last_month][9] - extension_cost)
                      + (_series[last_month][5] - _maintenance_cost) * _series[last_month-1][4]
                      + _series[last_month][10];

    }//The net cash flow for each period is ready
    npv[i][0] = line;
    npv[i][1]= discount(last_month, _coe);
    npv[i][2] = pb;
//    if((_msn == "0005-0067822-CC-07-2") && (secured == false))
//      dump_series(10, 60);
//    if((line == 600) && (_msn == "0005-0479775-CC-07-2") && (secured == false))
//      dump_series(10, 60);
  }//end if i
  double npv_value = 0;
  double pb_value = 0;
//  if(secured == true)
//    dump_npv(trails,npv);

  double optimal_line = 0;
  if(trails > 0)
    optimal_line = max_npv_line(trails, npv_value, pb_value, npv, filter);

  max_npv_value = npv_value;
  pb_max_npv = pb_value;
  delete[] npv;
  return optimal_line;
};

//---------------------------------------------
int il::get_periods()
{
  int upper = MAX_CELL - 1;
  int lower = 0;
  int index = (upper + lower) / 2;
  while ((product[index][7] != _test_cell)){
    if(product[index][7] > _test_cell)
      upper = index - 1;
    else
      lower = index + 1;
    index = (upper + lower) / 2;
  }
  _periods = product[index][3];
  return index;
}

void il::get_product_feature(int index)
{
  int periods;         

  double apr = product[index][1];
  _mgmt_fee = product[index][4];
  _mgmt_fee_periods = product[index][5];

  periods = _periods + 1;
  for(int i=0; i<periods; ++i)
    _series[i][3] = apr/12.0;
};

//--------------------------------------------------------------------------------------------------------------
void il::set_apr(double apr)
{
  int periods;         

  _mgmt_fee = 0;
  _mgmt_fee_periods = 0;

  periods = _periods + 1;
  for(int i=0; i<periods; ++i)
    _series[i][3] = apr/12.0;
};
//--------------------------------------------------------------------------------------------------------------
/*void il::get_propensity_parameters(int propensity_decile)
{
  switch (propensity_decile){
    case 0:
      _zero_taken_multiple = 1.21372287581699;
      break;
    case 1:
      _zero_taken_multiple = 1.11348538011696;
      break;
    case 2:
      _zero_taken_multiple = 1.11348538011696;
      break;
    case 3:
      _zero_taken_multiple = 1.11348538011696;
      break;
    case 4:
      _zero_taken_multiple = 1.21372287581699;
      break;
    case 5:
      _zero_taken_multiple = 1.21372287581699;
      break;
    case 6:
      _zero_taken_multiple = 1.21372287581699;
      break;
    case 7:
      _zero_taken_multiple = 1.21372287581699;
      break;
    case 8:
      _zero_taken_multiple = 1.35;
      break;
    case 9:
      _zero_taken_multiple = 1.45;
      break;
    case 10:
      _zero_taken_multiple = 1.86;
      break;
  }
};*/

il::il(double apr, int period, double gav, double nav, double existing_mortgage,
       AnsiString zip, double principal, double app_fee, double balance,
       double risk, AnsiString msn, AnsiString idn, AnsiString time_stamp,
       AnsiString *error_message)
       : _msn(msn), _idn(idn), _time_stamp(time_stamp)
{
  _status = 0;
  try{
//    _test_cell = test_cell;
    _gav = gav<=0? 0.000000000000001 : gav / 1000.0;
    _existing_mortgage = existing_mortgage / 1000.0;
    _nav = nav<=0? 0.000000000000001 : nav / 1000.0;
    _principal = principal / 1000.0;
    _app_fee = app_fee /1000.0;
    _coe = COE / 12.0;
    _cof = COF / 12.0;

    _outstanding_unsecured_bal = balance;
    _leverage = LEVERAGE;
    _commission = COMMISSION;
    _maintenance_cost = Maintenance_cost / 12.0;
    _personal_risk = risk;
    _home_equity = _nav - _existing_mortgage;
//    get_propensity_parameters(propensity_decile);
//    int product_cell_index = get_periods();
    _periods = period;

    _series = new double [_periods+1][TEST+1];
    for(int i = 0; i<=_periods; ++i){
      for(int j = 0; j<=TEST; ++j)
        _series[i][j] = 0;
    }
//    get_product_feature(product_cell_index);
    set_apr(apr);
    get_recovery_ratio(zip);

    _series[0][4] = 1; //probability of stay
    _status += 1;
  }
  catch(bad_alloc){
    (*error_message)+="記憶體不足";
    _status = 0;
  };
};

il::il()
{
  _status = 0;
}

il::~il()
{
  if (_status != 0)
    delete[] _series;
};

/*double il::effective_amount(double offering)
{
  double offering_ratio = offering / _home_equity;
  if(offering_ratio > _zero_taken_multiple)
     return _zero_taken_multiple * _home_equity * 1 / 2.0;
  else
    return (1 + (_zero_taken_multiple - offering_ratio) / _zero_taken_multiple) *
            offering_ratio / 2.0 * _home_equity;
};*/

double il::secured_pb(double loan_amount, unsigned int filter)
{
//  double loan_ratio2 = ((_existing_mortgage + loan_amount) /_gav )* 100;
  double loan_ratio2 = ((_existing_mortgage + loan_amount) /_nav )* 100;
  double loan_ratio2_t;
  if(loan_ratio2 > 200)
  	loan_ratio2_t = 200;
  else if (loan_ratio2 < 100)
  	loan_ratio2_t = 100;
  else
  	loan_ratio2_t = loan_ratio2;

//  double loan_diff2 = ((_existing_mortgage + loan_amount) - _gav);
  double loan_diff2 = ((_existing_mortgage + loan_amount) - _nav);
  int loan_diff2_1200k= loan_diff2>1200 ? 1 : 0;
  
  double total_approved_amt=(_existing_mortgage + loan_amount);
  double total_approved_amt_nt;
  if (total_approved_amt>9000) 
    total_approved_amt_nt=9000;
	else 
	  total_approved_amt_nt=total_approved_amt;
	  
	int loan_ratio2_100 = (loan_ratio2>100) ? 1 : 0;
	
	double app_score= -0.61555 + 
	                  loan_ratio2_t * 0.00626 +
                    loan_diff2_1200k * 0.23568 +
                    total_approved_amt_nt * 0.00000587 + 
                    loan_ratio2_100 * 0.05567;
  
  double score;
  double pb;
  if(app_score > 0.04784 && (filter == 0x80 || filter == 0)){
  	score =  -0.00170 + _personal_risk * 0.12247 + app_score * 0.21686; 
	  if (score <= -0.002254216) pb = 0.0000870644024592;
	  else if (score <= -0.00050731) pb = 0.000193692722589376;
	  else if (score <= 0.0021048076) pb = 0.000297655262147656;
	  else if (score <= 0.0041566377) pb = 0.000417178424294506;
	  else if (score <= 0.0050786692) pb = 0.000520248360300229;
    else if (score <= 0.0055307695) pb = 0.000605503252548778;
    else if (score <= 0.0060615707) pb = 0.000727805449611228;
    else if (score <= 0.006546382) pb = 0.000844679487157763;
    else if (score <= 0.0066395118) pb = 0.000904646020866708;
    else if (score <= 0.0067845982) pb = 0.000966490007447263;
    else if (score <= 0.0069688337) pb = 0.00104274504969745;
    else if (score <= 0.0071914555) pb = 0.00113294155430026;
    else if (score <= 0.0073071934) pb = 0.00123550782110137;
    else if (score <= 0.0075755501) pb = 0.0013468318421912;
    else if (score <= 0.007718263) pb = 0.00145524805789459;
    else if (score <= 0.0078658411) pb = 0.00157623528454038;
    else if (score <= 0.0082168665) pb = 0.00172472719718616;
    else if (score <= 0.0084878042) pb = 0.00187523404482858;
    else if (score <= 0.008696812) pb = 0.0020163151881282;
    else if (score <= 0.0089905977) pb = 0.00216120056371085;
    else if (score <= 0.0091869422) pb = 0.00230834569862786;
    else if (score <= 0.0095722426) pb = 0.00246772213990264;
    else if (score <= 0.0098643554) pb = 0.00265182309205689;
    else if (score <= 0.0102947898) pb = 0.00286506768420182;
    else if (score <= 0.010468706) pb = 0.00308302137895369;
    else if (score <= 0.0107525197) pb = 0.00328934949555068;
    else if (score <= 0.0110744867) pb = 0.00363009373983878;
    else if (score <= 0.0117990196) pb = 0.00416234698088642;
    else if (score <= 0.0126573908) pb = 0.00480135284731245;
    else if (score <= 0.0134294778) pb = 0.00554466145052087;
    else if (score <= 0.014505726) pb = 0.00640850961395341;
    else if (score <= 0.0157531968) pb = 0.0074118713099815;
    else if (score <= 0.0177938305) pb = 0.00857890462035907;
    else if (score <= 0.0241700612) pb = 0.00995329943844833;
    else if (score <= 0.0265895917) pb = 0.0110863356014752;
    else if (score <= 0.0347371483) pb = 0.0118646032483203;
    else if (score <= 0.0395760805) pb = 0.0128135918333278;
    else if (score <= 0.0448282572) pb = 0.0140805324958549;
    else if (score <= 0.0527202661) pb = 0.0159717750918589;
    else if (score <= 0.0650472446) pb = 0.0191258637139272;
    else if (score <= 0.0675659311) pb = 0.0218802690208409;
    else if (score <= 0.0731931599) pb = 0.022906515630648;
    else if (score <= 0.0831266313) pb = 0.0240492126646248;
    else if (score <= 0.0850366378) pb = 0.0253233806144397;
    else if (score <= 0.0899879153) pb = 0.026745989899357;
    else if (score <= 0.0928550313) pb = 0.0283362131269808;
    else if (score <= 0.0938717116) pb = 0.0301157098526485;
    else if (score <= 0.1001386823) pb = 0.0321089480065363;
    else if (score <= 0.1052818675) pb = 0.0343435666900969;
    else if (score <= 0.1250375414) pb = 0.036850785643735;
    else if (score <= 0.1264213006) pb = 0.0396658673642297;
    else if (score <= 0.1264594696) pb = 0.0428286386130065;
    else if (score <= 0.1375026305) pb = 0.0463840789157917;
    else if (score <= 0.1486017668) pb = 0.0465319007171763;
    else if (score <= 0.1877793656) pb = 0.0663755825770675;
    else if (score <= 0.1972354405) pb = 0.102485521847617;
    else pb = 0.20746110669922;  	  
  }	
  else{	
    score = app_score;
		if (score <= 0.026886) pb = 0.00107500471846493;
    else if (score <= 0.0430285) pb = 0.00121454766287049;
    else if (score <= 0.04567) pb = 0.00168776571927645;
    else if (score <= 0.047431) pb = 0.00194537315880961;
    else if (score <= 0.048605) pb = 0.00219246170922624;
    else if (score <= 0.049779) pb = 0.00244770787079714;
    else if (score <= 0.05154) pb = 0.00274089181639693;
    else if (score <= 0.0524205) pb = 0.00318940610802513;
    else if (score <= 0.055062) pb = 0.0038286587578834;
    else if (score <= 0.056823) pb = 0.00444483581907027;
    else if (score <= 0.05741) pb = 0.00507198147664856;
    else if (score <= 0.059171) pb = 0.00588618642677453;
    else if (score <= 0.060345) pb = 0.00644637046662145;
    else if (score <= 0.062106) pb = 0.00680473090897149;
    else if (score <= 0.06328) pb = 0.0137;
    else if (score <= 0.0944537957) pb = 0.02302831323956;
    else if (score <= 0.1264688197) pb = 0.0250247867259468;
    else if (score <= 0.1486173245) pb = 0.0270096856389131;
    else if (score <= 0.1728627528) pb = 0.0290536632619643;
    else if (score <= 0.1750260385) pb = 0.0305900624782711;
    else if (score <= 0.1811933373) pb = 0.0325732082067571;
    else if (score <= 0.1957243277) pb = 0.0355537062251439;
    else if (score <= 0.2043590099) pb = 0.0380515933131802;
    else if (score <= 0.2160809837) pb = 0.0400818384049177;
    else if (score <= 0.254935) pb = 0.0420708506497511;
    else if (score <= 0.25787) pb = 0.0440813526944908;
    else if (score <= 0.260805) pb = 0.046105578691751;
    else if (score <= 0.26374) pb = 0.0485762943080416;
    else if (score <= 0.269023) pb = 0.0510800144308181;
    else if (score <= 0.272545) pb = 0.0531419781884971;
    else if (score <= 0.2853486693) pb = 0.0556903118450965;
    else if (score <= 0.3217339014) pb = 0.0586733357817978;
    else if (score <= 0.3831913158) pb = 0.0617066623185411;
    else if (score <= 0.4532126772) pb = 0.0646622382850176;
    else if (score <= 0.533192328) pb = 0.0676406357578471;
    else if (score <= 0.5882675709) pb = 0.0702436272069268;
    else if (score <= 0.6263275389) pb = 0.0726909697944507;
    else if (score <= 0.6719456843) pb = 0.0761797989298419;
    else if (score <= 0.7272420576) pb = 0.0807014685541205;
    else if (score <= 0.8066024106) pb = 0.0867395818490491;
    else if (score <= 0.8313990564) pb = 0.0933737011473251;
    else if (score <= 0.8712851701) pb = 0.0992622929024332;
    else if (score <= 0.8980124327) pb = 0.105528116533261;
    else if (score <= 0.9195535979) pb = 0.111570209656598;
    else if (score <= 0.9289064132) pb = 0.116487545551892;
    else if (score <= 0.9350983094) pb = 0.121626366555369;
    else if (score <= 0.9411707748) pb = 0.127444119563389;
    else if (score <= 0.944236) pb = 0.133779950335277;
    else if (score <= 0.946358946) pb = 0.140424076481398;
    else if (score <= 0.94849175) pb = 0.147133085141741;
    else if (score <= 0.9524327826) pb = 0.153881304788229;
    else if (score <= 0.9541523943) pb = 0.160360061946962;
    else if (score <= 0.9566163165) pb = 0.167112942828692;
    else if (score <= 0.9599121443) pb = 0.173829330324587;
    else if (score <= 0.9642685621) pb = 0.180495440119546;
    else if (score <= 0.9707432134) pb = 0.190136136759279;
    else if (score <= 0.9786029453) pb = 0.204261403261604;
    else if (score <= 0.98063) pb = 0.220978835617639;
    else pb = 0.242538478641798;
  };  
  return pb;
};

double il::unsecured_pb(double loan_amount)
{
  double pb = 10000;
  if (_personal_risk <= -0.03231) pb= 0.001402016;
  else if (_personal_risk <= -0.02275) pb= 0.002803476;
  else if (_personal_risk <= -0.01479) pb= 0.004205993;
  else if (_personal_risk <=  -0.00919) pb= 0.005610332;
  else if (_personal_risk <= -0.00438) pb= 0.007017814;
  else if (_personal_risk <=  0.00101) pb= 0.008430714;
  else if (_personal_risk <=  0.00624) pb= 0.009852955;
  else if (_personal_risk <=  0.01245) pb= 0.011291301;
  else if (_personal_risk <=  0.01836) pb= 0.012757416;
  else if (_personal_risk <=  0.02482) pb= 0.014271407;
  else if (_personal_risk <=  0.03219) pb= 0.015867943;
  else if (_personal_risk <=  0.03963) pb= 0.017606797;
  else if (_personal_risk <=  0.04759) pb= 0.01959103;
  else if (_personal_risk <=  0.05585) pb= 0.021998328;
  else if (_personal_risk <=  0.06657) pb= 0.025135048;
  else if (_personal_risk <=  0.07865) pb= 0.029529392;
  else if (_personal_risk <=  0.09435) pb= 0.036092056;
  else if (_personal_risk <=  0.11509) pb= 0.0463932;
  else if (_personal_risk <=  0.15002) pb= 0.06314;
  else if (_personal_risk >  0.15002) pb= 0.091;

  double os_unsecured = _outstanding_unsecured_bal + loan_amount;
  os_unsecured = os_unsecured > Loan_Base ? os_unsecured : Loan_Base;
  pb = pb * os_unsecured / Loan_Base;
  return pb;
};

void il::annunity(double init_principal, int index)
{

  _series[0][index] = init_principal;
  double compound = pow((1+_series[0][3]), _periods);
  double monthly_repayment_ratio = (compound * _series[0][3]) /
                                  (compound -1);
  double monthly_repayment = init_principal * monthly_repayment_ratio;
  double principal_repayment;

  for(int i=1; i<=_periods; ++i){
    if(_series[i][3] != _series[i-1][3]){
      int remaining_periods = _periods - i + 1;
      compound = pow((1+_series[i][3]), remaining_periods);
      monthly_repayment_ratio = (compound * _series[i][3]) /
                                (compound -1);
      monthly_repayment = _series[i][index] * monthly_repayment_ratio;
    };
    _series[i][0] = (_series[i][3]) * _series[i-1][index]; //interest repayment
    principal_repayment = monthly_repayment - _series[i][0];

    _series[i][1] = principal_repayment;
    _series[i][index] = _series[i-1][index] - _series[i][1];
    if(_series[i][index]<=0){
      _series[i][1] += _series[i][index];
      _series[i][index]=0;
      break;
    }
  };
    _series[_periods][1] += _series[_periods][index];
    _series[_periods][index]=0;
};

void il::attrition(double pb, bool voluntary)
{
  double voluntary_close = 0;
  double monthly_pb = pb / 12.0;
  for(int i = 1; i <=_periods; ++i){
    if(voluntary){
      if(_periods == 36){
        if(i < 13)
          voluntary_close = 0.016;
        else
          voluntary_close = 0.021;
      }
      else{
        if(i < 13)
          voluntary_close = 0.007;
        else
          voluntary_close = 0.015;
      }
    };
    _series[i][4] = _series[i-1][4] * (1 - voluntary_close - monthly_pb);
  }
};

//This function must execute after the completion of payment schedule
void il::late_fee(double pb, double init_principal)
{
  double late = pb / 12.0 * 2.0;
  double compound = pow((1+_series[0][3]), _periods);
  double monthly_repayment_ratio = (compound * _series[0][3]) /
                                  (compound -1);
  double monthly_repayment = init_principal * monthly_repayment_ratio;
  for(int i=3; i<=_periods; ++i)
    _series[i][5] = monthly_repayment * _series[i-1][3] * LATE_FEE * late;
};

void il::interest_cost()
{
  for(int i=1; i<=_periods;++i)
    _series[i][8] = _cof * _series[i][2] * _leverage;
};

void il::late_cost(double pb)
{
  double late = pb / 12.0 * 2.0;
  for(int i=3; i<=_periods;++i)
    _series[i][10] = M2_3_EXPENSE * late;
};

void il::collection_cost(double pb, bool secured)
{
  double discounting_factor = 1 / pow(1+_coe, Time_to_recover);
  double expected_recovery = ((_nav * _recovery_ratio) - _existing_mortgage) * discounting_factor;
  expected_recovery = expected_recovery > 0 ? expected_recovery : 0;
  if(!secured)
    expected_recovery = 0;
  double wacc = 1.5 * (_cof * _leverage + _coe * (1 - _leverage));
  double monthly_pb = pb/12.0;
  for(int i=5; i<_periods; ++i)
    _series[i][9] = monthly_pb *
                   (M4P_EXPENSE + (_series[i][2] * Legal_fee_rate) +
                    (wacc * expected_recovery));
  _series[_periods][9] = 0;
};

void il::credit_loss(double pb, bool secured)//6
{
  double monthly_pb = pb / 12.0;
  double discounting_factor = 1 / pow(1+_coe, Time_to_recover);
  double expected_recovery = ((_nav * _recovery_ratio) - _existing_mortgage) * discounting_factor;
  expected_recovery = expected_recovery > 0 ? expected_recovery : 0;
  if(secured){
    for(int i=1; i<=_periods; ++i)
      _series[i][6] = monthly_pb *
        ((_series[i-1][2] > expected_recovery) ? (_series[i-1][2] - expected_recovery) : 0);
  }
  else{
    for(int i=1; i<=_periods; ++i)
      _series[i][6] = monthly_pb * (_series[i-1][2] - 0) * (1 - Default_recovery);
  }
};

double il::discount(int periods, double factor)
{
  double npv = 0;
  for(int i=0; i<=periods; ++i)
    npv += _series[i][7] / pow(1 + factor, i);
  return npv;
};

double il::npv(bool secured, double &max_npv_value, double &pb_max_npv, unsigned int filter, double secured_amount)
{
  if((!secured) && (filter != 0)){
    max_npv_value = 0;
    pb_max_npv = 0;
    return 0;
  };
  int cap;
  double upper = secured ? MAX_LENDING_SECURED : MAX_LENDING_UNSECURED;
  double upper_bound = min(MAX_LENDING_SECURED, (Max_Granting_Ratio * _nav - _existing_mortgage));
  if(!secured){
  	upper = (upper > secured_amount) ? secured_amount : upper;
    if(_personal_risk > 0.04759){
      if (_principal >= 100)
        cap = _principal < 500 ? _principal : 500;
      else
        cap = 500;
      upper = (upper > cap) ? cap : upper;
    }
  } else  // secured
     upper = upper_bound;

  int trails = (upper - MIN_LENDING_AMOUNT) / 10.0 + 1;
  double (*npv)[3] = new double [trails][3];
  for(int i = 0; i < trails; ++i){
  	npv[i][0] = 0;
  	npv[i][1] = 0;
  	npv[i][2] = 0;
  }


  for(int i = 0; i < trails; ++i){
    double loan_amount = MIN_LENDING_AMOUNT + i * 10;
    double taken_amount = loan_amount;
//    double taken_amount = effective_amount(loan_amount);
    double pb;
    if(secured){
      _series[0][7] = _app_fee + (taken_amount * (_commission + Set_lien_fee + (1 - _leverage)) + 2.25 ) * (-1); //對保2.25 + 代書 3 = 5.25; 代書 3 與set_lien_fee因向申貸人收費而打平
      pb = secured_pb(taken_amount, filter);
    }
    else{
      _series[0][7] = (taken_amount * (_commission + (1-_leverage)) /*+ 3*/) * (-1); //代書 3
      pb = unsecured_pb(taken_amount);
    }
    _series[0][2] = taken_amount;

//    pbs[i] = pb;

    attrition(pb);   //4

//  annunity(loan_amount,11);
    annunity(taken_amount,2); //0, 1, 2; using 3
    late_fee(pb, loan_amount); //5
    interest_cost(); //8
    late_cost(pb); //10
    collection_cost(pb, secured); //9
    credit_loss(pb, secured); //6, using 2

    int last_month=1;
    double mgmt_fee = _mgmt_fee;
    for(int k=1; k<=_periods; ++k){ //k is for cash flow schedule
      if(k>_mgmt_fee_periods)
        mgmt_fee = 0;
      _series[k][7] = _series[k][4] *
                     (_series[k][0] + _series[k][5] + mgmt_fee -
                      _series[k][8] - _series[k][9] - _maintenance_cost -
                      _series[k][10]) + (_leverage - 1) *
                      (_series[k][4] * _series[k][2] -
                       _series[k-1][4] * _series[k-1][2]) -
                      _series[k][6] * _series[k-1][4];

      last_month = k;
      if(_series[k][2] == 0)
        break;
    }//end of for k

    if(_series[last_month][2] != 0){
      _series[last_month][1] += _series[last_month][2];
      _series[last_month][2] = 0;
      _series[last_month][7] = _series[last_month][4] *
                     (_series[last_month][0] + _series[last_month][5] + mgmt_fee -
                      _series[last_month][8] - _series[last_month][9] - _maintenance_cost -
                      _series[last_month][10]) + ( _leverage - 1) *
                      (_series[last_month][4] * _series[last_month][2] -
                       _series[last_month-1][4] * _series[last_month-1][2]) -
                      _series[last_month][6] * _series[last_month-1][4];
    }//The net cash flow for each period is ready
    npv[i][0] = taken_amount;
    npv[i][1]= discount(last_month, _coe);
    npv[i][2] = pb;
//    if((_msn == "0001-1033763-BC-24-4") && (loan_amount == 330) && (secured == false))
//      dump_series(10, _periods);
//    if((_msn == "0004-0257984-CC-30-1") && (loan_amount == 100) && (secured == false))
//      dump_series(10, _periods);
//    if((_msn == "0003-0148812-CC-29-2") && (loan_amount == 100) && (secured == false))
//      dump_series(10, _periods);
  }//end if i
  double npv_value = 0;
  double pb_value = 0;
  double optimal_amount = 0;;
  if(trails > 0)
    optimal_amount = max_npv_amount(trails, npv_value, pb_value, npv, filter);
//  if((secured == false) /*&& (_msn == "0003-0242533-CC-29-2")*/)
//    dump_npv(trails,npv);

  max_npv_value = npv_value;
  pb_max_npv = pb_value;
  delete[] npv;
  return optimal_amount;
};

bool loc::dump(double loan_amount, double npv, bool secured)
{
  ofstream output("npv_testing.csv", ios_base::app);
  if(!output){
    cerr << "Can not open npv_testing.csv for output.\n";
    return false;
  };
  int secure = secured ? 1 : 0;
  output.precision(15);
  output << _msn << "," << npv << "," << loan_amount << "," << secure;
  output << endl;
  output.close();
  return true;
};

bool il::dump(double loan_amount, double npv, bool secured)
{
  ofstream output("npv_testing.csv", ios_base::app);
  if(!output){
    cerr << "Can not open npv_testing.csv for output.\n";
    return false;
  };
  int secure = secured ? 1 : 0;
  output.precision(15);
  output << _msn << "," << npv << "," << loan_amount << "," << secure;
  output << endl;
  output.close();
  return true;
};

double il::max_npv_amount(int trails, double &npv_value, double &pb, double npv[][3], unsigned int filter)
{
  double max_npv = npv[0][1];
  double max_amount = npv[0][0];
  double max_npv_pb = npv[0][2];
  bool over_limit = false;
  double amount_ceiling;
//  if(filter == 0)
//    amount_ceiling = Max_Granting_Ratio* _nav;
//  else if(filter == 0x80)
//    amount_ceiling = Max_Granting_Ratio_CASHCARD * _nav;
//  else
//    amount_ceiling = Max_Granting_Ratio_DATA * _nav;

  for(int i=0; i<trails;++i){
//    double amount = npv[i][0];
//    double total_exposure = amount + _existing_mortgage;
//    if(total_exposure > amount_ceiling){
//      if(i==0)
//        over_limit = true;
//      break;
//    }
    if(max_npv < npv[i][1]){
      max_npv = npv[i][1];
      max_amount = npv[i][0];
      max_npv_pb = npv[i][2];
    }
  };
  if(max_npv <0 || over_limit){
    max_npv = 0;
    max_npv_pb = 0;
    max_amount = 0;
  };
  npv_value = max_npv;
  pb = max_npv_pb;
  return max_amount;
};

bool il::dump_npv(int trails, double npv[][3])
{
  ofstream output("npv_values.csv", ios_base::app);
  if(!output){
    cerr << "Can not open attrition.csv for output.\n";
    return false;
  };
  output << _idn;
  output << endl;
  output.precision(15);
  for(int i=0; i<trails; ++i){
   output << npv[i][0] << ",";
   if((i%254)==0 && (i>0))
    output <<endl;
  }
  output <<endl;
  for(int i=0; i<trails; ++i){
   output << npv[i][1] << ",";
   if((i%254)==0 && (i>0))
    output <<endl;
  }
  output <<endl;  
  for(int i=0; i<trails; ++i){
   output << npv[i][2] << ",";
   if((i%254)==0 && (i>0))
    output <<endl;
  }
  output << endl;
  output.close();
  return true;
};


bool il::dump_series(int type, int period)
{
  ofstream output("npv_dumping.csv", ios_base::app);
  if(!output){
    cerr << "Can not open attrition.csv for output.\n";
    return false;
  };
  output << _idn;
  output << endl;
  output.precision(15);
  switch (type){
    case 2: //outstanding principle (i.e. remaining principle)
      for(int i=0; i<=_periods; ++i){
       output << _series[i][2] << ",";
       if(_series[i][2]==0)
         break;
      };
    break;
    case 4: //attrition (i.e. probability of stay)
      for(int i=0; i<=_periods; ++i){
       output << _series[i][4] << ",";
       if(_series[i][2]==0)
         break;
      };
    break;
    case 6: //credit loss
      for(int i=0; i<=_periods; ++i){
       output << _series[i][6] << ",";
       if(_series[i][2]==0)
         break;
      };
    break;
    case 7: //credit loss
      for(int i=0; i<=_periods; ++i){
       output << _series[i][7] << ",";
       if(_series[i][2]==0)
         break;
      };
    break;
    case 10: //everything
       for(int j=0; j <= 10; ++j){
         for(int i=0; i<=period; ++i){
           output << _series[i][j] << ",";
           if(_series[i][2]==0)
             break;
         }
         output<<endl;
       }
    break;
  }
  output << endl;
  output.close();
  return true;
};

bool il::dump_pb(int trails,double *pbs)
{
  ofstream output("pb.csv", ios_base::app);
  if(!output){
    cerr << "Can not open attrition.csv for output.\n";
    return false;
  };
  output << _idn;
  output << endl;
  output.precision(15);
  for(int i=0; i<trails; ++i){
   output << pbs[i] << ",";
   if((i%254)==0 && (i>0))
    output <<endl;
  }
  output <<endl;
  output.close();
  return true;
}

bool loc::dump_npv(int trails,double npv[][3])
{
  ofstream output("npv_values.csv", ios_base::app);
  if(!output){
    cerr << "Can not open attrition.csv for output.\n";
    return false;
  };
  output << _idn;
  output << endl;
  output.precision(15);
  for(int i=0; i<trails; ++i){
   output << npv[i][0] << ",";
   if((i%254)==0 && (i>0))
    output <<endl;
  }
  output <<endl;
  for(int i=0; i<trails; ++i){
   output << npv[i][1] << ",";
   if((i%254)==0 && (i>0))
    output <<endl;
  }
  output <<endl;  
  for(int i=0; i<trails; ++i){
   output << npv[i][2] << ",";
   if((i%254)==0 && (i>0))
    output <<endl;
  }
  output <<endl;
  output.close();
  return true;
};

bool loc::dump_series(int type, int period)
{
  ofstream output("npv_dumping.csv", ios_base::app);
  if(!output){
    cerr << "Can not open attrition.csv for output.\n";
    return false;
  };
  output << _idn;
  output << endl;
  output.precision(15);
  switch (type){
    case 2: //outstanding principle (i.e. remaining principle)
      for(int i=0; i<=_periods; ++i){
       output << _series[i][2] << ",";
//       if(_series[i][2]==0)
//         break;
      };
    break;
    case 4: //attrition (i.e. probability of stay)
      for(int i=0; i<=_periods; ++i){
       output << _series[i][4] << ",";
//       if(_series[i][2]==0)
//         break;
      };
    break;
    case 6: //credit loss
      for(int i=0; i<=_periods; ++i){
       output << _series[i][6] << ",";
//       if(_series[i][2]==0)
//         break;
      };
    break;
    case 7: //credit loss
      for(int i=0; i<=_periods; ++i){
       output << _series[i][7] << ",";
//       if(_series[i][2]==0)
//         break;
      };
    break;
    case 10: //everything
       for(int j=0; j <= 10; ++j){
         for(int i=0; i<=period; ++i){
           output << _series[i][j] << ",";
//           if(_series[i][2]==0)
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


