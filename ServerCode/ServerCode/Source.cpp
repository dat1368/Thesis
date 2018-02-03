#include "Header.h"
/**--------------------------------------------------------------------------*/
/** Global variables declare
/**--------------------------------------------------------------------------*/
int LWF_Fuzzy[72] = {
	RC,C,C,V,C,
	V,V,T,RC,C,
	V,T,C,V,V,
	T,RC,RC,C,
	V,C,V,V,T,
	RC,C,V,T,V,
	T,T,RT,RC,
	C,C,V,V,T,
	T,RT,RC,RC,C,
	V,C,V,V,T,
	V,V,V,RT,T,
	T,T,RT,C,C,
	V,T,C,V,T,
	RT,C,C,V,T,
	V,T,T,RT
};

double prePresure = 0;

Weather WeatherArray[24];

float PresDownArray[24];
/**--------------------------------------------------------------------------*/
/** Functions define
/**--------------------------------------------------------------------------*/
/**-------------------------------------*/
/** main
/**-------------------------------------*/
int main()
{
	float final_output = 0;
	/*Lưu thời tiết hiện tại*/
	Weather currentWeather;
	/*Đối tượng để tính toán fuzzy*/
	FUZ_SYS_LWF fuzzy_system_lwf;
	/*Khởi tạo vùng nhớ cho đối tượng Fuzzy System*/
	fuzzy_init_lwf(&fuzzy_system_lwf);
	/*Truyền giá trị cho currenWeather*/

	/*Thiết lập vòng loop ở đây*/
	/*Khi có tín hiệu tới -> Xóa 0 các giá trị*/
	khoi_tao_weather_rong(currentWeather);
	/*Nạp giá trị mới vào*/
	currentWeather.currentDateTime.hour = 7;
	currentWeather.currentDateTime.month = 12;
	currentWeather.tem = 29;
	currentWeather.hum = 26.8;
	currentWeather.presure = 1001.06 * 100;
	currentWeather.HuongGio = 270;
	currentWeather.SucGioLonNhat = 1;
	/*Tính toán giá trị và bỏ vào currentweather*/
	calculateWeather(currentWeather, fuzzy_system_lwf);
	final_output = currentWeather.output;
	cout << "output:" << final_output;
	system("pause");
}

/**-------------------------------------*/
/** External function
/**-------------------------------------*/
void fuzzy_init_lwf(FUZ_SYS_LWF *fuzzy_system_lwf) {
	/* Allocate memory for membership functions. */
	if (!(fuzzy_system_lwf->tem_mem = (IN_MEM *)malloc(sizeof(IN_MEM)))) {
		exit(1);
	}
	if (!(fuzzy_system_lwf->hum_mem = (IN_MEM *)malloc(sizeof(IN_MEM)))) {
		exit(1);
	}
	if (!(fuzzy_system_lwf->presdown_mem = (IN_MEM *)malloc(sizeof(IN_MEM)))) {
		exit(1);
	}
	if (!(fuzzy_system_lwf->rateDay_mem = (IN_MEM *)malloc(sizeof(IN_MEM)))) {
		exit(1);
	}
	if (!(fuzzy_system_lwf->rateYear_mem = (IN_MEM *)malloc(sizeof(IN_MEM)))) {
		exit(1);
	}
	if (!(fuzzy_system_lwf->out_mem = (OUT_MEM *)malloc(sizeof(OUT_MEM)))) {
		exit(1);
	}
	if (!(fuzzy_system_lwf->tem_mem->center = (double *)malloc(3 * sizeof(double)))) {
		exit(1);
	}
	if (!(fuzzy_system_lwf->tem_mem->dom = (double *)malloc(3 * sizeof(double)))) {
		exit(1);
	}
	if (!(fuzzy_system_lwf->hum_mem->center = (double *)malloc(3 * sizeof(double)))) {
		exit(1);
	}
	if (!(fuzzy_system_lwf->hum_mem->dom = (double *)malloc(3 * sizeof(double)))) {

	}
	if (!(fuzzy_system_lwf->presdown_mem->center = (double *)malloc(2 * sizeof(double)))) {
		exit(1);
	}
	if (!(fuzzy_system_lwf->presdown_mem->dom = (double *)malloc(2 * sizeof(double)))) {
	}

	if (!(fuzzy_system_lwf->rateYear_mem->center = (double *)malloc(2 * sizeof(double)))) {
		exit(1);
	}
	if (!(fuzzy_system_lwf->rateYear_mem->dom = (double *)malloc(2 * sizeof(double)))) {
	}
	if (!(fuzzy_system_lwf->rateDay_mem->center = (double *)malloc(2 * sizeof(double)))) {
		exit(1);
	}
	if (!(fuzzy_system_lwf->rateDay_mem->dom = (double *)malloc(2 * sizeof(double)))) {
		exit(1);
	}

	if (!(fuzzy_system_lwf->out_mem->center = (double *)malloc(5 * sizeof(double)))) {
		exit(1);
	}
	if (!(fuzzy_system_lwf->out_mem->dom = (double *)malloc(5 * sizeof(double)))) {
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

void khoi_tao_weather_rong(Weather InitWeather)
{
	InitWeather.currentDateTime.day = 0;
	InitWeather.currentDateTime.month = 0;
	InitWeather.currentDateTime.year = 0;
	InitWeather.currentDateTime.hour = 0;
	InitWeather.currentDateTime.minute = 0;

	InitWeather.tem = 0;
	InitWeather.hum = 0;
	InitWeather.presure = 0;
	InitWeather.HuongGio = 0;
	InitWeather.LuongMua1Gio = 0;
	InitWeather.LuongMuaTrongNgay = 0;
	InitWeather.output = 0;
	InitWeather.PercentDecrease24 = 0;
	InitWeather.presDown = 0;
	InitWeather.Presure24 = 0;
	InitWeather.rateInDay = 0;
	InitWeather.rateInYear = 0;
	InitWeather.SucGioLonNhat = 0;
	InitWeather.SucGioTrungBinh = 0;
	InitWeather.TotalDecrease = 0;
}

int calculateWeather(Weather &weather, FUZ_SYS_LWF  fm_fuzzy_system_lwf)
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
	weather.output = fuzzy_control_lwf(weather.tem, weather.hum, weather.TotalDecrease, weather.rateInYear, weather.rateInDay, &fm_fuzzy_system_lwf);
	weather.output = weather.output * 10;
	switch (weather.HuongGio)
	{
	case 0:
		/*do nothing*/
		break;
	case 45:
		weather.output = weather.output + 0.5;
		break;
	case 90:
		weather.output = weather.output + 1;
		break;
	case 135:
		weather.output = weather.output + 0.5;
		break;
	case 180:
		/*do nothing*/
		break;
	case 225:
		weather.output = weather.output - 0.5;
		break;
	case 270:
		weather.output = weather.output - 1;
		break;
	case 315:
		weather.output = weather.output - 0.5;
		break;
	default:
		/*do nothing*/
		break;
	}
	weather.output = weather.output + weather.SucGioLonNhat;
	return 1;
}

/**-------------------------------------*/
/** Internal function
/**-------------------------------------*/
double leftall(double u, double w, double c)

/* Determine degree of membership for a leftall membership function.
NOTE:  u is input, c is mem. fun. center, and w is mem. fun. width. */

{
	if (u < c)
		return 1.0;
	else
		return MAX(0, (1 - (u - c) / w));
}

double rightall(double u, double w, double c)
/* Determine degree of membership for a RIGHTALL membership function
NOTE:  u is input, c is mem. fun. center, and w is mem. fun. width. */

{
	if (u >= c)
		return 1.0;
	else
		return MAX(0, (1 - (c - u) / w));
}

double triangle(double u, double w, double c)

/* Determine degree of membership for a TRIANGLE membership function
NOTE:  u is input, c is mem. fun. center, and w is mem. fun. width. */

{
	if (u >= c)
		return MAX(0, (1 - (u - c) / w));
	else
		return MAX(0, (1 - (c - u) / w));
}

float percent(float input, float left, float right)
{
	if (left == right)
	{
		return input;
	}
	else if (left < right)
	{
		return (input - left) / (right - left);
	}
	else
	{
		return (input - right) / (left - right);
	}
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
		return 100 * percent(hour, 11, 23);
	}
	else
	{
		if (hour < 11)
		{
			hour = hour + 24;
		}
		return 100 * (1 - percent(hour, 23, 35));
	}
}

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

int fuzzyify_two(double u, IN_MEM *mem) {

	/* Assumes 2 membership functions, leftall and rightall respectively. */
	mem->dom[0] = leftall(u, mem->width, mem->center[0]);
	mem->dom[1] = rightall(u, mem->width, mem->center[1]);
	return 1;
}

double fuzzy_control_lwf(double tem, double hum, double presDown,
	double rateYear, double rateDay, FUZ_SYS_LWF *fuzzy_system_lwf)
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

void fineMaxMin(const IN_MEM *tem_mem, const IN_MEM *hum_mem, const IN_MEM *presDown_mem,
	const IN_MEM *rateYear_mem, const IN_MEM *rateDay_mem, OUT_MEM *out_mem)
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

double inf_defuzz_lwf2(IN_MEM *tem_mem, IN_MEM *hum_mem, IN_MEM *presDown_mem,
	IN_MEM *rateYear_mem, IN_MEM *rateDay_mem, OUT_MEM *out_mem)
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

void match_lwf(const IN_MEM *tem_mem, const IN_MEM *hum_mem, const IN_MEM *presdown_mem,
	const IN_MEM *rateYear_mem, const IN_MEM *rateDay_mem, int *pos)
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

double MIN5(double num1, double num2, double num3, double num4, double num5)
{
	return MIN(num1, MIN(num2, MIN(num3, MIN(num4, num5))));
}

double MAX5(double num1, double num2, double num3, double num4, double num5)
{
	return MAX(num1, MAX(num2, MAX(num3, MAX(num4, num5))));
}