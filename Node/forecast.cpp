#include "forecast.h"


using namespace std;
#define FILE		"06-05-16.txt"
#define READ_FILE	"Input\\06-04-16.txt"
#define OUT_FILE	"OutputData\\OUT_06-05-16.txt"

ofstream  OutputFile(OUT_FILE);
typedef struct timeRate
{
	int startHour;
	int endHour;
	int startMinutes;
	int endMinutes;
}TRate;

typedef struct MyWeather
{
	DateTime currentDateTime;

	double tem;
	double hum;
	double presure;

	double presDown;

	double rateInYear;
	double rateInDay;

	double output;

	double Presure24;
	double PercentDecrease24;
	double TotalDecrease;

	string forcast = "NULL";
}Weather;

Weather WeatherArray[24];
float PresDownArray[24];
double prePresure = 0;

typedef struct calRate
{
	uint16_t countHR;			//Count dự đoán Heavy rain
	uint16_t countLR;			//Count dự đoán Little rain
	uint16_t countNR;			//Count dự đoán Not rain
	uint16_t countRainRight;	//Count dự đoán Rain Right
	uint16_t countRight;		//Count dự đoán Right (Kể cả dự đoán không mưa)
	TRate timeRate;				//Chứa thời gian bắt đầu, kết thúc dự đoán, bắt đầu kết thúc mưa thật.
}CalRate;

float percent(float input, float left, float right)
{
	if (left == right)
	{
		return input;
	}
	else if(left < right)
	{
		return (input -left) / (right-left);
	}
	else
	{
		return (input - right) / (left - right);
	}
}

int calculateWeather(Weather &weather, FUZ_SYS_LWF  fm_fuzzy_system_lwf,CalRate* cr)
{
	float tempTotal = 0;
	/*Tính tỉ lệ mưa của tháng trong năm*/
	weather.rateInYear = percentMonth(weather.currentDateTime.month);
	/*Tính tỉ lệ mưa của giờ trong ngày*/
	weather.rateInDay = percentDay(weather.currentDateTime.hour);
	/*Nếu mà áp suất trước đó bằng 0 thì tổng áp suất nó bằng 0*/
	if (prePresure == 0)
	{
		weather.presDown = 0;
	}
	else
	{
		weather.presDown = ((weather.presure - prePresure) / prePresure) * 100000;
	}
	prePresure = weather.presure;
	/*Biến tạm chưa tổng áp suất giảm*/
	tempTotal = pushPresDownArray(weather.presDown);
	weather.TotalDecrease = tempTotal;

	/*Truyền các tham số vào hệ fuzzy và tính ra output
	*Gồm: Nhiệt độ, Độ ẩm, Tổng áp suất giảm liên tiếp trong 2 giờ, tỉ lệ tháng, tỉ lệ giờ
	*/
	weather.output = fuzzy_control_lwf(weather.tem, weather.hum, weather.TotalDecrease, weather.rateInYear, weather.rateInDay,&fm_fuzzy_system_lwf);
	
	/*=========================== PHÂN TÍCH OUTPUT, TÍNH TỈ LỆ ====================*/
	/*Truyền giờ bắt đầu*/
	int gioStart	= cr->timeRate.startHour;
	int gioEnd		= cr->timeRate.endHour;
	/*Truyền giờ dự đoán. Dự đoán trước 4 tiếng*/
	int gioDuDoan	= weather.currentDateTime.hour + 4;
	int phutDuDoan	= weather.currentDateTime.minute;
	/*Scale lại trường hợp nó vượt qua 0 giờ*/
	if (gioDuDoan >= 24)
	{
		gioDuDoan = gioDuDoan - 24;
	}
	/*Ghép giờ phút thành kiểu int để dễ so sánh*/
	int gioPhutDuDoan = gioDuDoan * 100 + phutDuDoan;
	/*************************/
	if (weather.output <= OUT_PUT_VALUE1)
	{
		weather.forcast = "==NOT_RAIN==";
		/*Đếm số lần dự đoán NOT RAIN*/
		cr->countNR++;
		if (gioDuDoan >= 12 && gioDuDoan <= 23)
		{
			if (!(gioPhutDuDoan <= gioEnd && gioPhutDuDoan >=gioStart))
			{
				/*Nếu mà giờ dự đoán nằm nogài khoảng mưa thật*/
				/*Tăng biến đếm đúng lên*/
				cr->countRainRight++;
			}
		}
	}
	else if(weather.output <=OUT_PUT_VALUE2)
	{
		weather.forcast = "LITTLE RAIN";
		/*Đếm số lần dự đoán LITTLE RAIN*/
		cr->countLR++;
		if (gioDuDoan >= 12 && gioDuDoan <= 23)
		{
			if (gioPhutDuDoan <= gioEnd && gioPhutDuDoan >=gioStart)
			{
				/*Nếu mà giờ dự đoán năm trong khoảng mưa thật*/
				/*Tăng biến đếm đúng lên*/
				cr->countRainRight++;
			}
		}
	}
	else
	{
		weather.forcast = "HEAVY RAIN";
		/*Đếm số lần dự đoán HEAVY RAIN*/
		cr->countHR++;
		if (gioDuDoan >= 12 && gioDuDoan <= 23)
		{
			if (gioPhutDuDoan <= gioEnd && gioPhutDuDoan >=gioStart)
			{
				/*Nếu mà giờ dự đoán năm trong khoảng mưa thật*/
				/*Tăng biến đếm đúng lên*/
				cr->countRainRight++;
			}
		}
	}
	return 1;

}

float pushPresDownArray(float pushValue)
{
	/*push to PresDownArray pushvalue and return total presdown*/
	int array_size = sizeof(PresDownArray) / sizeof(PresDownArray[0]);
	float totalPresDown = 0;
	for (int j = array_size - 1; j > 0; j--)
	{
		PresDownArray[j] = PresDownArray[j - 1];
	}
	PresDownArray[0] = pushValue;
	for (int i = 0; i < 24; i++)
	{
		totalPresDown += PresDownArray[i];
	}
	return totalPresDown;
}

float pushWeatherArray(Weather pushWeather)
{
	/*push to PresDownArray pushvalue and return total presdown*/
	int array_size = sizeof(WeatherArray) / sizeof(WeatherArray[0]);
	for (int j = array_size - 1; j > 0; j--)
	{
		PresDownArray[j] = PresDownArray[j - 1];
	}
	WeatherArray[0] = pushWeather;
	return 1;
}

double percentMonth(double month)
{
	if ((month >= 2) && (month <= 8))
	{
		return 100 - ((month - 2) / (8 - 2)) * 100;
	}
	else
	{
		if (month <= 2)
		{
			month = month + 12;
		}
		return ((month - 8) / (14 - 8)) * 100;
	}
}

double percentDay(double hour)
{
	if ((hour >= 11) && (hour <= 23))
	{
		return 100 *percent(hour, 11, 23);
	}
	else
	{
		if (hour < 11)
		{
			hour = hour + 24;
		}
		return 100*(1-percent(hour, 23, 35));
	}
}

double MIN5(double num1, double num2, double num3, double num4, double num5)
{
	return MIN(num1, MIN(num2, MIN(num3, MIN(num4, num5))));
}

double MAX5(double num1, double num2, double num3, double num4, double num5)
{
	return MAX(num1, MAX(num2, MAX(num3, MAX(num4, num5))));
}

void fuzzy_init_lwf(FUZ_SYS_LWF *fuzzy_system_lwf) {
	/* Allocate memory for membership functions. */
	if (!(fuzzy_system_lwf->tem_mem = (IN_MEM *)malloc(sizeof(IN_MEM)))) {
		printf("Error allocating memory.\n");
		exit(1);
	}
	if (!(fuzzy_system_lwf->hum_mem = (IN_MEM *)malloc(sizeof(IN_MEM)))) {
		printf("Error allocating memory.\n");
		exit(1);
	}
	if (!(fuzzy_system_lwf->presdown_mem = (IN_MEM *)malloc(sizeof(IN_MEM)))) {
		printf("Error allocating memory.\n");
		exit(1);
	}
	if (!(fuzzy_system_lwf->rateDay_mem = (IN_MEM *)malloc(sizeof(IN_MEM)))) {
		printf("Error allocating memory.\n");
		exit(1);
	}
	if (!(fuzzy_system_lwf->rateYear_mem = (IN_MEM *)malloc(sizeof(IN_MEM)))) {
		printf("Error allocating memory.\n");
		exit(1);
	}
	if (!(fuzzy_system_lwf->out_mem = (OUT_MEM *)malloc(sizeof(OUT_MEM)))) {
		printf("Error allocating memory.\n");
		exit(1);
	}
	if (!(fuzzy_system_lwf->tem_mem->center = (double *)malloc(3 * sizeof(double)))) {
		printf("Error allocating memory.\n");
		exit(1);
	}
	if (!(fuzzy_system_lwf->tem_mem->dom = (double *)malloc(3 * sizeof(double)))) {
		printf("Error allocating memory.\n");
		exit(1);
	}
	if (!(fuzzy_system_lwf->hum_mem->center = (double *)malloc(3 * sizeof(double)))) {
		printf("Error allocating memory.\n");
		exit(1);
	}
	if (!(fuzzy_system_lwf->hum_mem->dom = (double *)malloc(3 * sizeof(double)))) {
		printf("Error allocating memory.\n");

	}
	if (!(fuzzy_system_lwf->presdown_mem->center = (double *)malloc(2 * sizeof(double)))) {
		printf("Error allocating memory.\n");
		exit(1);
	}
	if (!(fuzzy_system_lwf->presdown_mem->dom = (double *)malloc(2 * sizeof(double)))) {
		printf("Error allocating memory.\n");
	}

	if (!(fuzzy_system_lwf->rateYear_mem->center = (double *)malloc(2 * sizeof(double)))) {
		printf("Error allocating memory.\n");
		exit(1);
	}
	if (!(fuzzy_system_lwf->rateYear_mem->dom = (double *)malloc(2 * sizeof(double)))) {
		printf("Error allocating memory.\n");
	}
	if (!(fuzzy_system_lwf->rateDay_mem->center = (double *)malloc(2 * sizeof(double)))) {
		printf("Error allocating memory.\n");
		exit(1);
	}
	if (!(fuzzy_system_lwf->rateDay_mem->dom = (double *)malloc(2 * sizeof(double)))) {
		printf("Error allocating memory.\n");
	}

	if (!(fuzzy_system_lwf->out_mem->center = (double *)malloc(5 * sizeof(double)))) {
		printf("Error allocating memory.\n");
		exit(1);
	}
	if (!(fuzzy_system_lwf->out_mem->dom = (double *)malloc(5 * sizeof(double)))) {
		printf("Error allocating memory.\n");
		exit(1);
	}
	/* Initialize for local weather forecast. */
	fuzzy_system_lwf->tem_mem->width = TEMP_WITCH;
	fuzzy_system_lwf->hum_mem->width = HUM_WITCH;
	fuzzy_system_lwf->presdown_mem->width = PRES_WIDTH;
	fuzzy_system_lwf->rateYear_mem->width = RATE_YEAR_WIDTH;
	fuzzy_system_lwf->rateDay_mem->width = RATE_DAY_WIDTH;
	fuzzy_system_lwf->out_mem->width = OUT_PUT_WIDTH;

	fuzzy_system_lwf->tem_mem->center[0] = TEMP_LEFT;
	fuzzy_system_lwf->tem_mem->center[1] = TEMP_CENTER;
	fuzzy_system_lwf->tem_mem->center[2] = TEMP_RIGHT;

	fuzzy_system_lwf->hum_mem->center[0] = HUM_LEFT;
	fuzzy_system_lwf->hum_mem->center[1] = HUM_CENTER;
	fuzzy_system_lwf->hum_mem->center[2] = HUM_RIGHT;

	fuzzy_system_lwf->presdown_mem->center[0] = PRES_LEFT;
	fuzzy_system_lwf->presdown_mem->center[1] = PRES_RIGHT;

	fuzzy_system_lwf->rateYear_mem->center[0] = RATE_YEAR_LEFT;
	fuzzy_system_lwf->rateYear_mem->center[1] = RATE_YEAR_RIGHT;

	fuzzy_system_lwf->rateDay_mem->center[0] = RATE_DAY_LEFT;
	fuzzy_system_lwf->rateDay_mem->center[1] = RATE_DAY_RIGHT;

	for (int i = 0; i < 5; i++)
	{
		fuzzy_system_lwf->out_mem->center[i] = i*OUT_PUT_WIDTH;
		fuzzy_system_lwf->out_mem->dom[i] = 0;
		/*std::cout << "outmem center :" << fuzzy_system_lwf->out_mem->center[i]<<std::endl;*/
	}
}

/************************************************************************************/
void fuzzy_free(FUZ_SYS *fuzzy_system) {

	/* Free memory allocated in fuzzy_init(). */
	free(fuzzy_system->emem->center);
	free(fuzzy_system->emem->dom);
	free(fuzzy_system->edotmem->center);
	free(fuzzy_system->edotmem->dom);
	free(fuzzy_system->outmem->center);
	free(fuzzy_system->emem);
	free(fuzzy_system->edotmem);
	free(fuzzy_system->outmem);
}

/************************************************************************************/

double fuzzy_control_lwf(double tem,double hum,double presDown,
	                     double rateYear,double rateDay,FUZ_SYS_LWF *fuzzy_system_lwf)
{
	/* Given crisp inputs e and edot, determine the crisp output u. */

	int pos[5];

	fuzzyify_three(tem, fuzzy_system_lwf->tem_mem);
	fuzzyify_three(hum, fuzzy_system_lwf->hum_mem);
	fuzzyify_two(presDown, fuzzy_system_lwf->presdown_mem);
	fuzzyify_two(rateYear, fuzzy_system_lwf->rateYear_mem);
	fuzzyify_two(rateDay, fuzzy_system_lwf->rateDay_mem);

	match_lwf(
		fuzzy_system_lwf->tem_mem,
		fuzzy_system_lwf->hum_mem,
		fuzzy_system_lwf->presdown_mem,
		fuzzy_system_lwf->rateYear_mem,
		fuzzy_system_lwf->rateDay_mem,
		pos
		);

	return inf_defuzz_lwf2(
		fuzzy_system_lwf->tem_mem,
		fuzzy_system_lwf->hum_mem,
		fuzzy_system_lwf->presdown_mem,
		fuzzy_system_lwf->rateYear_mem,
		fuzzy_system_lwf->rateDay_mem,
		fuzzy_system_lwf->out_mem
		);
}

/************************************************************************************/

/************************************************************************************/

int fuzzyify_three(double u, IN_MEM *mem) {

	/* Assumes 3 membership functions, with first and last membership
	functions leftall and rightall respectively.  Center membership functions are
	triangular. */
	int i;
	mem->dom[0] = leftall(u, mem->width, mem->center[0]);
	mem->dom[1] = triangle(u, mem->width, mem->center[1]);
	mem->dom[2] = rightall(u, mem->width, mem->center[2]);
	return 1;
}

/************************************************************************************/

int fuzzyify_two(double u, IN_MEM *mem) {

	/* Assumes 2 membership functions, leftall and rightall respectively. */
	mem->dom[0] = leftall(u, mem->width, mem->center[0]);
	mem->dom[1] = rightall(u, mem->width, mem->center[1]);
	return 1;
}

/************************************************************************************/

double leftall(double u, double w, double c)

/* Determine degree of membership for a leftall membership function.
NOTE:  u is input, c is mem. fun. center, and w is mem. fun. width. */

{
	if (u < c)
		return 1.0;
	else
		return MAX(0, (1 - (u - c) / w));
}

/************************************************************************************/

double rightall(double u, double w, double c)
/* Determine degree of membership for a RIGHTALL membership function
NOTE:  u is input, c is mem. fun. center, and w is mem. fun. width. */

{
	if (u >= c)
		return 1.0;
	else
		return MAX(0, (1 - (c - u) / w));
}

/************************************************************************************/

double triangle(double u, double w, double c)

/* Determine degree of membership for a TRIANGLE membership function
NOTE:  u is input, c is mem. fun. center, and w is mem. fun. width. */

{
	if (u >= c)
		return MAX(0, (1 - (u - c) / w));
	else
		return MAX(0, (1 - (c - u) / w));
}

/************************************************************************************/

void match(const IN_MEM *emem, const IN_MEM *edotmem, int *pos) {

	/* For each universe of discourse, determine the index of the first membership function
	with a non-zero degree (i.e. match the rules to the current inputs to find which rules
	are on).  These indices are used to determine which four rules to evaluate.  (NOTE:
	A 2 input sytem with no more than 50% overlap for input membership functions only
	requires the evaluation of at most 4 rules.) */

	int i;

	for (i = 0; i<5; i++) {
		if (emem->dom[i] != 0) {
			pos[0] = i;
			break;
		}
	}
	for (i = 0; i<5; i++) {
		if (edotmem->dom[i] != 0) {
			pos[1] = i;
			break;
		}
	}
}

void match_lwf(
	const IN_MEM *tem_mem,
	const IN_MEM *hum_mem,
	const IN_MEM *presdown_mem,
	const IN_MEM *rateYear_mem,
	const IN_MEM *rateDay_mem,
	int *pos)
{

	/* For each universe of discourse, determine the index of the first membership function
	with a non-zero degree (i.e. match the rules to the current inputs to find which rules
	are on).  These indices are used to determine which four rules to evaluate.  (NOTE:
	A 2 input sytem with no more than 50% overlap for input membership functions only
	requires the evaluation of at most 4 rules.) */

	int i;

	for (i = 0; i<3; i++) {
		if (tem_mem->dom[i] != 0) {
			pos[0] = i;
			break;
		}
	}

	for (i = 0; i<3; i++) {
		if (hum_mem->dom[i] != 0) {
			pos[1] = i;
			break;
		}
	}

	for (i = 0; i<2; i++) {
		if (presdown_mem->dom[i] != 0) {
			pos[2] = i;
			break;
		}
	}

	for (i = 0; i<2; i++) {
		if (rateYear_mem->dom[i] != 0) {
			pos[3] = i;
			break;
		}
	}

	for (i = 0; i<2; i++) {
		if (rateDay_mem->dom[i] != 0) {
			pos[4] = i;
			break;
		}
	}
}
/************************************************************************************/
/*Fuzzy using Midemi method, It unwork in this project*/

/************************************************************************************/
/*defuzzi using Sugeno*/

#if 1
int showWeather(Weather needShow)
{
	cout << "Day:" << needShow.currentDateTime.day << endl;
	cout << "Month:" << needShow.currentDateTime.month << endl;
	cout << "Year:" << needShow.currentDateTime.year << endl;
	cout << "Hour:" << needShow.currentDateTime.hour << endl;
	cout << "Minute:" << needShow.currentDateTime.minute << endl;
	cout << "Temp:" << needShow.tem << endl;
	cout << "Humi:" << needShow.hum << endl;
	cout << "presure:" << needShow.presure << endl;

	cout << "rate in Year:" << needShow.rateInYear << endl;
	cout << "rate in Day: " << needShow.rateInDay << endl;

	cout << "presDOWN::" << needShow.presDown << endl;

	cout << "==OUTPUT==" << needShow.output <<"==OUTPUT==" << endl<<endl;
	return 1;
}

int showForecast(Weather _weather)
{
	
	int temp = 2;
	int temp2 = 0;
	if (_weather.output <= 0.65)
	{
		temp = 2;
	}
	else if (_weather.output < 0.7)
	{
		temp = 1;
	}
	else
	{
		temp = 0;
	}
	temp2 = _weather.currentDateTime.hour + 4 ;
	if (temp2 >= 24)
	{
		temp2 = temp2 - 24;
	}
	cout << temp2 << ":";
	cout << _weather.currentDateTime.minute << "=\t" << temp;
	cout << "(" << roundf((_weather.output * 100)*100) / 100<< ")";
	cout << "\t\t" << _weather.forcast;
	cout << endl;


	//f << temp2 << ":";
	//f << _weather.currentDateTime.minute << "=\t" << temp;
	//f << "(" << roundf(_weather.output * 100) * 10 / 10 << ")";
	OutputFile << roundf(_weather.output * 100) * 100 / 100;
	//f << "\t\t" << _weather.forcast;
	OutputFile << endl;
	return 1;

}

Weather parseLineData(string dataLine)
{
	//string tempString[7];
	Weather tempWeather;
	tempWeather.tem = 24;
	int i = 0;
	string tempArray[8];

	string _paragraph = dataLine;							/*Where store a converted string*/
	regex breakString = regex("\:");
	sregex_token_iterator endOfSequence;
	sregex_token_iterator splitData(_paragraph.begin(), _paragraph.end(), breakString, -1);
	while (splitData != endOfSequence)
	{

		string TempOfWord = *splitData++;	/*content a line*/
		tempArray[i] = TempOfWord;
		i++;
		//cout << TempOfWord << endl;
	}
	tempWeather.currentDateTime.day = atoi(tempArray[0].c_str());
	tempWeather.currentDateTime.month = atoi(tempArray[1].c_str());
	tempWeather.currentDateTime.year = atoi(tempArray[2].c_str());
	tempWeather.currentDateTime.hour = atoi(tempArray[3].c_str());
	tempWeather.currentDateTime.minute = atoi(tempArray[4].c_str());

	tempWeather.tem = atoi(tempArray[5].c_str());
	tempWeather.hum = atoi(tempArray[6].c_str());
	tempWeather.presure = atoi(tempArray[7].c_str());

	return tempWeather;
}



vector<string> readDataFile(string _data_file)
{
	/*Global variable*/
	fstream Data(_data_file);					/*fsteam to read dicrectory*/
	vector<string> _ListOfData;					/*List of dictionary that this function return*/
	stringstream streamGetString;				/*Where store a paragraph to character by character*/
	string _paragraph;							/*Where store a converted string*/

												/*Function*/
	streamGetString << Data.rdbuf();			/*Get paragraph*/
	_paragraph = streamGetString.str();			/*Store paragraph to _paragraph*/
	regex breakString = regex("\\n+");
	sregex_token_iterator endOfSequence;
	sregex_token_iterator splitData(_paragraph.begin(), _paragraph.end(), breakString, -1);

	if (Data.is_open())
	{
		cout << "Close file" << endl;
		Data.close();
	}

	while (splitData != endOfSequence)
	{
		string TempOfWord = *splitData++;	/*content a line*/
		//cout << TempOfWord;
		_ListOfData.push_back(TempOfWord);
											
	}

	return _ListOfData;
}
#endif


#if 1
void fineMaxMin(
	const IN_MEM *tem_mem,
	const IN_MEM *hum_mem,
	const IN_MEM *presDown_mem,
	const IN_MEM *rateYear_mem,
	const IN_MEM *rateDay_mem,
	OUT_MEM *out_mem)
{
	double tmp = 0;
	int index = 0;// chay tu 0->80 
	double outDOM[5] = { 0, 0, 0, 0, 0 };
	int uFuzzy[72];
	for (int i = 0; i < 72; i++)
	{
		uFuzzy[i] = LWF_Fuzzy[i];
	}
	//             
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			for (int m = 0; m < 2; m++)
				for (int n = 0; n < 2; n++)
					for (int k = 0; k < 2; k++)
					{
						if ((tem_mem->dom[i] != 0.0)
							&& (hum_mem->dom[j] != 0.0)
							&& (presDown_mem->dom[m] != 0.0)
							&& (rateYear_mem->dom[n] != 0.0)
							&& (rateDay_mem->dom[k] != 0.0))
						{

							tmp = MIN5(tem_mem->dom[i],
								hum_mem->dom[j],
								presDown_mem->dom[m],
								rateYear_mem->dom[n],
								rateDay_mem->dom[k]);
							
							outDOM[uFuzzy[index]] = MAX(outDOM[uFuzzy[index]], tmp);
							//std::cout << "DOM::::::::::" << tmp << std::endl;
						}
						index++;
					}

	}// End loop for  

	for (int i = 0; i<5; i++) {
		out_mem->dom[i] = outDOM[i];
	}
}

double inf_defuzz_lwf2(IN_MEM *tem_mem, IN_MEM *hum_mem, IN_MEM *presDown_mem, IN_MEM *rateYear_mem, IN_MEM *rateDay_mem, OUT_MEM *out_mem)
{
	fineMaxMin(tem_mem, hum_mem, presDown_mem, rateYear_mem, rateDay_mem, out_mem);

	//Processing defuzzification 
	double WAtot = 0, Atot = 0;
	for (int i = 0; i < 5; i++) {
		WAtot += out_mem->center[i] * out_mem->dom[i];
		Atot += out_mem->dom[i];
	}
	return WAtot / Atot;
}
#endif

#if 1
int main()
{
	vector<string> vectorOfData;
	Weather currentWeather;
	FUZ_SYS_LWF fuzzy_system_lwf;
	fuzzy_init_lwf(&fuzzy_system_lwf);
	CalRate* cr_ =new CalRate;

	cr_->countHR = 0;
	cr_->countLR = 0;
	cr_->countNR = 0;
	cr_->countRainRight = 0;

	cr_->timeRate.startHour = 1520;
	//cr_->timeRate.startMinutes = 15;
	cr_->timeRate.endHour = 1940;
	//currentWeather.timeRate.endMinutes = 15;
	vectorOfData = readDataFile(READ_FILE);

	for (int i = 0; i < vectorOfData.size(); i++)
	{
		currentWeather= parseLineData(vectorOfData[i]);
		calculateWeather(currentWeather,fuzzy_system_lwf,cr_);
		showForecast(currentWeather);
	}
	cout <<"TONG SO LAN DU DOAN DUNG:\t"<< cr_->countRainRight << endl;
	
	/*Số lần dự đoán mưa = count heavy rain + count little rain*/
	float temp = cr_->countHR + cr_->countLR;
	cout <<"TONG SO LAN DU DOAN MUA:\t" << temp << endl;
	/*Tỉ lệ 1=  số lần dự đoán mưa đúng / số lần dự đoán mưa */
	temp = cr_->countRainRight / temp;


	/*Tỉ lệ 1 =  số lần dự đoán đúng / số lần dự đoán (288 lần một ngày)*/
	/*Số lần dự đoán đúng*/
	cout <<"SO LAN DUNG/SO LAN MUA:\t" << temp;
	system("pause");
}
#endif
/*****************************************************************************/
