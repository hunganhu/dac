#ifndef economicH
#define economicH
//---------------------------------------------------------------------------
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
const int SalesPointCost[] = {0, 600, 600, 600, 600, 600, 600, 600, 600, 0}; // Channel 1-9 

// 主管手續獎金, Add 0 for dummy channel 0				  
const int HeadFeeBonus [2][10] = {{0, 160, 160, 160, 160, 0, 0, 160, 0, 0},  // GX, Channel 1-9
                                  {0,  96,  96,  96,  96, 0, 0,  96, 0, 0}}; // KHJ, Channel 1-9				  

// 業務手續獎金, Add 0 for dummy channel 0				  
const int SalesFeeBonus [2][10] = {{0, 3500, 3500, 0, 0, 0, 0, 3500, 0, 0},  // GX, Channel 1-9
                                   {0, 1000, 1000, 0, 0, 0, 0, 1000, 0, 0}}; // KHJ, Channel 1-9				  

// 主管手獎減免, Add 0 for dummy channel 0				  
const int HeadBonusDiscount [2][10] = {{0, 1, 1, 1, 1, 0, 0, 1, 0, 0},  // GX, Channel 1-9
                                       {0, 1, 1, 1, 1, 0, 0, 1, 0, 0}}; // KHJ, Channel 1-9				  

// 業務手獎減免, Add 0 for dummy channel 0				  
int SalesBonusDiscount [2][10] = {{0, 1, 1, 0, 0, 0, 0, 1, 0, 0},  // GX, Channel 1-9
                                  {0, 1, 1, 0, 0, 0, 0, 1, 0, 0}}; // KHJ, Channel 1-9				  

// 產品	通路 利率, Add 0 for dummy channel 0
const double VariableCommission [2][10][11] = {
   {{ 0, 0,   0, 0,   0, 0,   0, 0, 0, 0, 0}, //GX, Channel 0, [20.0%, 19.5%, 19.0%, 18.5%, 18.0%, 17.5%, 17.0%, 16.5%, 16.0%, 15.5%, 15.0%]
   {3.5, 3, 2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0}, //GX, Channel 1, [20.0%, 19.5%, 19.0%, 18.5%, 18.0%, 17.5%, 17.0%, 16.5%, 16.0%, 15.5%, 15.0%]
   {3.5, 3, 2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0}, //GX, Channel 2, [20.0%, 19.5%, 19.0%, 18.5%, 18.0%, 17.5%, 17.0%, 16.5%, 16.0%, 15.5%, 15.0%]
   {3.5, 3, 2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0}, //GX, Channel 3, [20.0%, 19.5%, 19.0%, 18.5%, 18.0%, 17.5%, 17.0%, 16.5%, 16.0%, 15.5%, 15.0%]
   {3.5, 3, 2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0}, //GX, Channel 4, [20.0%, 19.5%, 19.0%, 18.5%, 18.0%, 17.5%, 17.0%, 16.5%, 16.0%, 15.5%, 15.0%]
   {3.5, 3, 2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0}, //GX, Channel 5, [20.0%, 19.5%, 19.0%, 18.5%, 18.0%, 17.5%, 17.0%, 16.5%, 16.0%, 15.5%, 15.0%]
   {3.5, 3, 2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0}, //GX, Channel 6, [20.0%, 19.5%, 19.0%, 18.5%, 18.0%, 17.5%, 17.0%, 16.5%, 16.0%, 15.5%, 15.0%]
   {3.5, 3, 2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0}, //GX, Channel 7, [20.0%, 19.5%, 19.0%, 18.5%, 18.0%, 17.5%, 17.0%, 16.5%, 16.0%, 15.5%, 15.0%]
   {3.5, 3, 2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0}, //GX, Channel 8, [20.0%, 19.5%, 19.0%, 18.5%, 18.0%, 17.5%, 17.0%, 16.5%, 16.0%, 15.5%, 15.0%]
   {3.5, 3, 2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0}},//GX, Channel 9, [20.0%, 19.5%, 19.0%, 18.5%, 18.0%, 17.5%, 17.0%, 16.5%, 16.0%, 15.5%, 15.0%]
   {{ 0, 0,   0, 0,   0, 0, 0, 0, 0, 0, 0},//KHJ, Channel 0, [20.0%, 19.5%, 19.0%, 18.5%, 18.0%, 17.5%, 17.0%, 16.5%, 16.0%, 15.5%, 15.0%]
   {2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0, 0, 0},//KHJ, Channel 1, [20.0%, 19.5%, 19.0%, 18.5%, 18.0%, 17.5%, 17.0%, 16.5%, 16.0%, 15.5%, 15.0%]
   {2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0, 0, 0},//KHJ, Channel 2, [20.0%, 19.5%, 19.0%, 18.5%, 18.0%, 17.5%, 17.0%, 16.5%, 16.0%, 15.5%, 15.0%]
   {2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0, 0, 0},//KHJ, Channel 3, [20.0%, 19.5%, 19.0%, 18.5%, 18.0%, 17.5%, 17.0%, 16.5%, 16.0%, 15.5%, 15.0%]
   {2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0, 0, 0},//KHJ, Channel 4, [20.0%, 19.5%, 19.0%, 18.5%, 18.0%, 17.5%, 17.0%, 16.5%, 16.0%, 15.5%, 15.0%]
   {2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0, 0, 0},//KHJ, Channel 5, [20.0%, 19.5%, 19.0%, 18.5%, 18.0%, 17.5%, 17.0%, 16.5%, 16.0%, 15.5%, 15.0%]
   {2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0, 0, 0},//KHJ, Channel 6, [20.0%, 19.5%, 19.0%, 18.5%, 18.0%, 17.5%, 17.0%, 16.5%, 16.0%, 15.5%, 15.0%]
   {2.5, 2, 1.5, 1, 0.5, 0, 0, 0, 0, 0, 0},//KHJ, Channel 7, [20.0%, 19.5%, 19.0%, 18.5%, 18.0%, 17.5%, 17.0%, 16.5%, 16.0%, 15.5%, 15.0%]
   {  0, 0,   0, 0,   0, 0, 0, 0, 0, 0, 0},//KHJ, Channel 8, [20.0%, 19.5%, 19.0%, 18.5%, 18.0%, 17.5%, 17.0%, 16.5%, 16.0%, 15.5%, 15.0%]
   {  0, 0,   0, 0,   0, 0, 0, 0, 0, 0, 0}}//KHJ, Channel 9, [20.0%, 19.5%, 19.0%, 18.5%, 18.0%, 17.5%, 17.0%, 16.5%, 16.0%, 15.5%, 15.0%]
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

// PB Assignment for GX

const double demo_PB[] = {
   0.070500713,  //twentile 0
   0.07567789,   //twentile 1 
   0.080793804,  //twentile 2 
   0.08369692,   //twentile 3 
   0.086490683,  //twentile 4 
   0.092223057,  //twentile 5
   0.096807994,  //twentile 6 
   0.108087593,  //twentile 7 
   0.121242202,  //twentile 8 
   0.127603141,  //twentile 9 
   0.140645667,  //twentile 10 
   0.199175101,  //twentile 11 
   0.297444698,  //twentile 12
   0.363314142,  //twentile 13
   0.469618598,  //twentile 14
                  };
                  
const double b2_PB[][14] = {                  
   {0.112750977, 0.12005079,  0.121878848, 0.123707168, 0.125535622, 0.127364148, 0.12919271,  0.131021291, 0.132849882, 0.134678478, 0.136507077, 0.138335677, 0.140164278, 0.14199288},  // 0                   
   {0.113894483, 0.121984103, 0.124040301, 0.12610077,  0.128163816, 0.130228419, 0.132293964, 0.134360078, 0.136426538, 0.138493206, 0.140560001, 0.142626873, 0.144693792, 0.146760739}, // 1 
   {0.115465371, 0.124023944, 0.126275622, 0.128545335, 0.130827022, 0.133116659, 0.135411576, 0.137710001, 0.140010756, 0.14231306,  0.144616395, 0.146920414, 0.149224889, 0.151529666}, // 2 
   {0.117694768, 0.126304491, 0.128693576, 0.131127518, 0.133593307, 0.136081709, 0.138586165, 0.141102021, 0.14362597,  0.146155665, 0.148689441, 0.151226113, 0.153764843, 0.156305033}, // 3 
   {0.124981108, 0.132336639, 0.134747802, 0.137293698, 0.139944598, 0.142677321, 0.145473789, 0.148319909, 0.151204697, 0.154119595, 0.157057935, 0.160014523, 0.162985314, 0.165967158}, // 4 
   {0.130441711, 0.136556252, 0.13883202,  0.141298101, 0.143917788, 0.146661425, 0.149505056, 0.152429338, 0.155418655, 0.158460405, 0.161544418, 0.164662492, 0.16780801,  0.170975638}, // 5 
   {0.13737239,  0.141947243, 0.143992501, 0.146283284, 0.148778314, 0.151443202, 0.154249311, 0.157172803, 0.160193838, 0.16329591,  0.166465291, 0.169690567, 0.172962249, 0.176272451}, // 6 
   {0.145960819, 0.148826244, 0.150561269, 0.152590315, 0.154870545, 0.157365298, 0.160043206, 0.162877441, 0.165845068, 0.168926487, 0.172104951, 0.17536616,  0.178697902, 0.182089754}, // 7 
   {0.156390681, 0.15754001,  0.158912225, 0.160614117, 0.162604462, 0.164847101, 0.167310333, 0.169966377, 0.172790897, 0.175762587, 0.178862797, 0.182075209, 0.185385552, 0.188781347}, // 8 
   {0.165402964, 0.165478567, 0.166462604, 0.167897864, 0.169640329, 0.171655912, 0.173914211, 0.176388128, 0.179053518, 0.181888882, 0.184875083, 0.187995093, 0.191233773, 0.194577661}, // 9 
   {0.172029206, 0.172029206, 0.172258126, 0.173494741, 0.175066782, 0.176920543, 0.179027246, 0.181360963, 0.183898345, 0.186618379, 0.189502159, 0.192532689, 0.195694691, 0.198974443}, // 10 
   {0.17919542,  0.17919542,  0.17919542,  0.179764696, 0.181168496, 0.182858147, 0.184807003, 0.1869908,   0.189387462, 0.191976912, 0.194740902, 0.197662853, 0.200727713, 0.20392182 }, // 11 
   {0.186922254, 0.186922254, 0.186922254, 0.186944413, 0.188018825, 0.18954885,  0.191339879, 0.193369756, 0.19561811,  0.198066227, 0.200696922, 0.203494428, 0.206444281, 0.209533225}, // 12 
   {0.195230215, 0.195230215, 0.195230215, 0.195230215, 0.195695443, 0.197078274, 0.198719053, 0.200598091, 0.2026971,   0.204999107, 0.207488371, 0.210150299, 0.212971376, 0.21593909 }, // 13
   {0.204139669, 0.204139669, 0.204139669, 0.204139669, 0.204280404, 0.20553767,  0.2070447,   0.208784545, 0.210741298, 0.212900043, 0.215246806, 0.217768501, 0.220452887, 0.223288518}, // 14
   {0.213670854, 0.213670854, 0.213670854, 0.213670854, 0.213860196, 0.215024033, 0.216424204, 0.218046668, 0.219878107, 0.221905897, 0.224118087, 0.226503368, 0.229051054, 0.231751051}, // 15 
   {0.223843883, 0.223843883, 0.223843883, 0.223866068, 0.224525793, 0.225640221, 0.226972366, 0.228511154, 0.230245958, 0.232166588, 0.234263285, 0.236526716, 0.238947962, 0.241518509}, // 16 
   {0.23467875,  0.234682987, 0.234852905, 0.235452907, 0.236372715, 0.237495076, 0.238811617, 0.240314174, 0.241994801, 0.243845778, 0.245859615, 0.248029058, 0.250347091, 0.252806937}, // 17
   {0.246195332, 0.246945708, 0.247616695, 0.248470304, 0.249501073, 0.250703533, 0.25207222,  0.253601704, 0.255286597, 0.257121576, 0.259101389, 0.261220866, 0.263474934, 0.265858619}, // 18
   {0.258413399, 0.260738402, 0.26168874,  0.26278203,  0.264015624, 0.265386737, 0.266892476, 0.268529856, 0.270295817, 0.272187246, 0.274200987, 0.276333859, 0.278582663, 0.280944198}, // 19
   {0.271352615, 0.275696127, 0.277037316, 0.278480674, 0.280025815, 0.281672154, 0.283418926, 0.28526521,  0.287209939, 0.289251921, 0.291389851, 0.293622326, 0.295947857, 0.298364879}, // 20 
   {0.285032541, 0.291882931, 0.293741633, 0.295662177, 0.297645838, 0.299693675, 0.30180655,  0.30398514,  0.306229952, 0.308541334, 0.310919491, 0.313364494, 0.315876291, 0.318454719}, // 21
   {0.29947264,  0.309364242, 0.311883148, 0.314425852, 0.316994672, 0.319591731, 0.322218968, 0.324878144, 0.327570857, 0.330298549, 0.333062516, 0.335863917, 0.338703781, 0.341583014}, // 22 
   {0.314692282, 0.328206857, 0.33154555,  0.334874385, 0.33819613,  0.341513393, 0.344828634, 0.348144169, 0.351462178, 0.354784709, 0.358113686, 0.361450913, 0.364798081, 0.368156774}, // 23
   {0.330710746, 0.348478925, 0.352814755, 0.357113856, 0.361378906, 0.365612477, 0.369817034, 0.373994941, 0.378148466, 0.382279782, 0.386390969, 0.390484024, 0.394560854, 0.398623288}, // 24 
   {0.34754722,  0.370249938, 0.375778899, 0.381253737, 0.386676618, 0.392049651, 0.397374879, 0.402654291, 0.407889815, 0.413083324, 0.418236637, 0.423351518, 0.428429682, 0.433472791}, // 25
   {0.365220809, 0.393590716, 0.400528334, 0.407406911, 0.414227851, 0.420992529, 0.427702297, 0.434358478, 0.44096237,  0.447515246, 0.454018355, 0.460472923, 0.466880149, 0.47324121 }, // 26
   {0.383750536, 0.418573398, 0.427155614, 0.435689677, 0.444176198, 0.45261578,  0.461009023, 0.469356517, 0.477658846, 0.485916588, 0.494130314, 0.502300589, 0.510427972, 0.518513016}, // 27
   {0.402415509, 0.444240533, 0.454647662, 0.465035331, 0.475403635, 0.485752666, 0.49608252,  0.506393289, 0.516685065, 0.526957941, 0.537212008, 0.547447358, 0.557664083, 0.567862271}  // 28
   };


const double b1_PB[] = {
   0.031130021, // 0
   0.058291646, // 1
   0.085505617, // 2
   0.0,         // 3 missing
   0.119389694, // 4
   0.137359645, // 5
   0.156121074, // 6
   0.181768755, // 7
   0.201005969, // 8
   0.213969007, // 9
   0.230702779, // 10
   0.248090634, // 11
   0.260996253, // 12
   0.274342634, // 13
   0.289082445, // 14
   0.303299903, // 15
   0.317342423, // 16
   0.330608749, // 17
   0.343733214, // 18
   0.35260419   // 19
   };

const double a2_PB[][18] = {                  
   {0.022447037, 0.021734794, 0.021378672, 0.02102255,  0.020666429, 0.020310307, 0.019954185, 0.019598064, 0.019241942, 0.018885821, 0.018529699, 0.018173577, 0.017817456, 0.017461334, 0.017105213, 0.016749091, 0.016392969, 0.016036848}, // 0
   {0.027943629, 0.028109368, 0.028192238, 0.02827511,  0.028357982, 0.028440854, 0.028523727, 0.0286066,   0.028689474, 0.028772347, 0.028855221, 0.028938095, 0.029020969, 0.029103843, 0.029186718, 0.029269592, 0.029352466, 0.029435341}, // 1
   {0.033440712, 0.034484295, 0.035006104, 0.035527923, 0.036049749, 0.036571583, 0.037093423, 0.037615267, 0.038137115, 0.038658967, 0.039180822, 0.039702679, 0.040224539, 0.0407464,   0.041268263, 0.041790127, 0.042311992, 0.042833858}, // 2
   {0.038940768, 0.04086152,  0.041821988, 0.042782508, 0.043743072, 0.044703676, 0.045664313, 0.04662498,  0.047585673, 0.048546389, 0.049507124, 0.050467877, 0.051428645, 0.052389425, 0.053350217, 0.054311019, 0.055271829, 0.056232647}, // 3
   {0.044451724, 0.047247656, 0.04864592,  0.050044356, 0.051442945, 0.052841671, 0.05424052,  0.055639479, 0.057038537, 0.058437682, 0.059836906, 0.061236199, 0.062635555, 0.064034967, 0.065434428, 0.066833934, 0.068233479, 0.06963306},  // 4
   {0.04999246,  0.053659257, 0.055493371, 0.057327908, 0.059162833, 0.060998112, 0.062833714, 0.064669612, 0.066505778, 0.068342191, 0.070178827, 0.072015667, 0.073852693, 0.075689887, 0.077527236, 0.079364725, 0.081202341, 0.083040073}, // 5
   {0.055600572, 0.060130657, 0.062397089, 0.064664362, 0.06693242,  0.069201208, 0.071470675, 0.073740772, 0.076011456, 0.078282683, 0.080554415, 0.082826616, 0.08509925,  0.087372286, 0.089645695, 0.091919448, 0.09419352,  0.096467887}, // 6
   {0.061342488, 0.06672465,  0.069417998, 0.072112753, 0.074808841, 0.07750619,  0.080204731, 0.082904398, 0.085605128, 0.088306861, 0.09100954,  0.09371311,  0.096417519, 0.099122718, 0.10182866,  0.1045353,   0.107242597, 0.109950512}, // 7
   {0.067326068, 0.073546235, 0.076659487, 0.079774749, 0.082891947, 0.086011007, 0.089131854, 0.092254417, 0.095378625, 0.09850441,  0.101631703, 0.10476044,  0.107890557, 0.111021991, 0.114154684, 0.117288578, 0.120423616, 0.123559745}, // 8
   {0.073715733, 0.080759673, 0.084285391, 0.087813548, 0.091344095, 0.094876981, 0.098412152, 0.101949552, 0.105489125, 0.109030814, 0.112574558, 0.116120301, 0.119667982, 0.123217543, 0.126768924, 0.130322068, 0.133876917, 0.137433414}, // 9
   {0.078436928, 0.086041747, 0.089847915, 0.093656583, 0.097467736, 0.101281358, 0.105097424, 0.108915906, 0.112736772, 0.116559989, 0.120385518, 0.124213319, 0.128043349, 0.131875564, 0.135709919, 0.139546367, 0.143384859, 0.147225348}, // 10
   {0.081191455, 0.089101866, 0.093060602, 0.097021723, 0.100985241, 0.104951163, 0.10891949,  0.112890216, 0.11686333,  0.120838816, 0.124816654, 0.128796821, 0.13277929,  0.136764031, 0.140751012, 0.144740198, 0.148731553, 0.152725037}, // 11
   {0.08408584,  0.092302607, 0.0964141,   0.100527741, 0.104643573, 0.108761631, 0.112881944, 0.117004532, 0.12112941,  0.125256584, 0.129386057, 0.133517827, 0.137651887, 0.141788226, 0.145926827, 0.150067674, 0.154210744, 0.158356014}, // 12
   {0.087142169, 0.095667313, 0.099932359, 0.104199174, 0.108467837, 0.112738416, 0.117010969, 0.121285547, 0.125562192, 0.12984094,  0.134121819, 0.138404851, 0.142690054, 0.146977438, 0.151267009, 0.155558771, 0.15985272,  0.164148851}, // 13
   {0.090385091, 0.099222219, 0.103642383, 0.108063785, 0.112486538, 0.116910744, 0.121336497, 0.125763878, 0.130192963, 0.134623815, 0.139056495, 0.143491051, 0.147927528, 0.152365963, 0.156806388, 0.161248829, 0.165693306, 0.170139837}, // 14
   {0.093842014, 0.102996688, 0.107574503, 0.112152857, 0.116731899, 0.121311769, 0.125892592, 0.130474488, 0.135057561, 0.139641912, 0.144227629, 0.148814793, 0.153403477, 0.157993749, 0.162585668, 0.167179286, 0.171774651, 0.176371805}, // 15
   {0.097543314, 0.107023475, 0.111762656, 0.116501506, 0.121240211, 0.125978943, 0.130717865, 0.135457128, 0.140196872, 0.14493723,  0.149678323, 0.154420263, 0.159163156, 0.163907098, 0.168652179, 0.173398481, 0.178146079, 0.182895042}, // 16
   {0.101522546, 0.111338986, 0.116244684, 0.121149012, 0.126052189, 0.130954421, 0.135855902, 0.140756817, 0.145657339, 0.15055763,  0.155457845, 0.160358128, 0.165258615, 0.170159433, 0.175060702, 0.179962533, 0.184865032, 0.189768296}, // 17
   {0.105816666, 0.115983562, 0.121062642, 0.126139159, 0.131213359, 0.136285477, 0.141355736, 0.14642435,  0.15149152,  0.156557439, 0.16162229,  0.166686246, 0.171749474, 0.176812128, 0.181874358, 0.186936305, 0.191998101, 0.197059872}, // 18
   {0.110466264, 0.121001768, 0.126263127, 0.1315206,   0.136774455, 0.142024949, 0.147272328, 0.152516829, 0.157758677, 0.16299809,  0.168235275, 0.17347043,  0.178703747, 0.183935407, 0.189165584, 0.194394445, 0.199622149, 0.204848847}, // 19
   {0.115515796, 0.126442694, 0.131897613, 0.137347237, 0.142791846, 0.148231711, 0.153667093, 0.159098244, 0.164525408, 0.169948817, 0.175368698, 0.180785269, 0.186198738, 0.191609309, 0.197017175, 0.202422523, 0.207825534, 0.213226381}, // 20
   {0.121013838, 0.132360275, 0.138022811, 0.143678616, 0.149327975, 0.15497116,  0.16060844,  0.166240071, 0.171866305, 0.177487383, 0.18310354,  0.188715006, 0.194322,    0.199924736, 0.205523422, 0.211118258, 0.21670944,  0.222297154}, // 21
   {0.127013334, 0.138813617, 0.144701034, 0.150580346, 0.156451827, 0.162315742, 0.168172353, 0.174021912, 0.179864667, 0.185700858, 0.191530718, 0.197354477, 0.203172356, 0.208984571, 0.214791333, 0.220592848, 0.226389314, 0.232180926}, // 22
   {0.133571863, 0.145867334, 0.152000586, 0.158124528, 0.164239413, 0.170345488, 0.176442995, 0.182532171, 0.188613246, 0.194686448, 0.200751998, 0.206810112, 0.212861004, 0.218904881, 0.224941945, 0.230972395, 0.236996425, 0.243014225}, // 23
   {0.140751911, 0.153591906, 0.159996157, 0.16639021,  0.172774282, 0.179148591, 0.185513348, 0.191868761, 0.198215033, 0.204552367, 0.210880959, 0.217201003, 0.223512689, 0.229816204, 0.236111732, 0.242399452, 0.248679543, 0.254952178}, // 24
   {0.148621147, 0.162064044, 0.168769246, 0.175463855, 0.182148048, 0.188821999, 0.195485879, 0.202139856, 0.208784097, 0.215418766, 0.222044025, 0.228660033, 0.235266949, 0.241864926, 0.248454118, 0.255034676, 0.261606748, 0.268170481}, // 25
   {0.157252718, 0.171367066, 0.178408585, 0.185439837, 0.192460947, 0.199472042, 0.206473244, 0.213464674, 0.220446455, 0.227418705, 0.234381543, 0.241335085, 0.248279448, 0.255214745, 0.262141091, 0.269058597, 0.275967374, 0.282867532}, // 26
   {0.166725538, 0.181591294, 0.189010594, 0.196420942, 0.203822414, 0.211215083, 0.218599022, 0.225974305, 0.233341004, 0.240699191, 0.248048938, 0.255390314, 0.262723392, 0.270048239, 0.277364927, 0.284673523, 0.291974096, 0.299266713}, // 27
   {0.177124601, 0.192834454, 0.200679841, 0.208518907, 0.216351683, 0.224178199, 0.231998486, 0.239812572, 0.247620487, 0.25542226,  0.263217922, 0.271007501, 0.278791026, 0.286568526, 0.29434003,  0.302105567, 0.309865165, 0.317618852}, // 28
   {0.187760998, 0.204355713, 0.212649544, 0.220941029, 0.229230174, 0.237516982, 0.245801458, 0.254083606, 0.262363431, 0.270640936, 0.278916127, 0.287189007, 0.295459581, 0.303727853, 0.311993828, 0.320257509, 0.328518901, 0.336778009}, // 29
};























#endif