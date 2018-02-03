#pragma once
using namespace std;
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <conio.h>
#include <direct.h>
/**--------------------------------------------------------------------------*/
/** Define
/**--------------------------------------------------------------------------*/
#define MAX(A,B)  ((A) > (B) ? (A) : (B))
#define MIN(A,B)  ((A) < (B) ? (A) : (B))
#define PI 3.14159265359

#define TEMP_LEFT 20
#define TEMP_CENTER 25//((TEMP_RIGHT + TEMP_LEFT)/2)
#define TEMP_RIGHT 33
#define TEMP_WITCH 10//((TEMP_RIGHT-TEMP_LEFT)/2)

#define HUM_LEFT 30
#define HUM_RIGHT 70
#define HUM_CENTER 50;//((HUM_RIGHT + HUM_RIGHT)/2)
#define HUM_WITCH 20;//(HUM_RIGHT-HUM_LEFT)/2

#define PRES_LEFT -250
#define PRES_RIGHT 250
#define PRES_WIDTH 500 //PRES_RIGHT*2

#define RATE_YEAR_LEFT 0
#define RATE_YEAR_RIGHT 100
#define RATE_YEAR_WIDTH RATE_YEAR_RIGHT

#define RATE_DAY_LEFT 0
#define RATE_DAY_RIGHT 100
#define RATE_DAY_WIDTH RATE_DAY_RIGHT

#define OUT_PUT_WIDTH 0.25

#define OUT_PUT_VALUE1 0.5
#define OUT_PUT_VALUE2 0.75
/**--------------------------------------------------------------------------*/
/** Typedef and Enum
/**--------------------------------------------------------------------------*/
/**-------------------------------------*/
/** Fuzzy typedef
/**-------------------------------------*/
typedef struct in_mem {
	double width;         /* Input membership function width (1/2 of triangle base).  */
	double *center;       /* Center of each input membership function.                */
	double *dom;          /* Degree of membership for each membership function.       */
} IN_MEM;

typedef struct out_mem {
	double width;         /* Output membership function width (1/2 of triangle base). */
	double *center;       /* Center of each output membership function.               */
	double *dom;
} OUT_MEM;

typedef struct fuz_sys {
	IN_MEM  *emem;        /* Groups all fuzzy system parameters in a single variable. */
	IN_MEM  *edotmem;
	OUT_MEM *outmem;
} FUZ_SYS;

typedef struct fuz_sys_lwf {
	IN_MEM  *tem_mem;        /* Groups all fuzzy system parameters in a single variable. */
	IN_MEM  *hum_mem;
	IN_MEM  *presdown_mem;
	IN_MEM  *rateYear_mem;
	IN_MEM  *rateDay_mem;
	OUT_MEM *out_mem;
} FUZ_SYS_LWF;

/*Others typedef*/
typedef struct MyTime
{
	int day;
	int month;
	int year;

	int hour;
	int minute;

}DateTime;

typedef struct MyWeather
{
	/*Input data*/
	/*Thời gian*/
	DateTime currentDateTime;
	/*Nhiet do, do am, ap suat*/
	double tem;
	double hum;
	double presure;

	double rateInYear;
	double rateInDay;
	/*Huong gio*/
	uint16_t HuongGio;
	/*Suc gio*/
	float SucGioTrungBinh;
	float SucGioLonNhat;
	/*Luong mua*/
	float LuongMua1Gio;
	float LuongMuaTrongNgay;

	double presDown;
	/*Output Data*/
	double output;

	double Presure24;
	double PercentDecrease24;
	double TotalDecrease;

	string forcast = "NULL";
	string TinhHinh = "NULL";
}Weather;
/**-------------------------------------*/
/** Enum
/**-------------------------------------*/
enum OUTPUT_
{
	RT,
	T,
	V,
	C,
	RC
};
/**--------------------------------------------------------------------------*/
/** Functions Prototypes
/**--------------------------------------------------------------------------*/
/**-------------------------------------*/
/** External function
/**-------------------------------------*/
void fuzzy_init_lwf(FUZ_SYS_LWF *fuzzy_system_lwf);

void khoi_tao_weather_rong(Weather InitWeather);

int calculateWeather(Weather &weather, FUZ_SYS_LWF  fm_fuzzy_system_lwf);

/**-------------------------------------*/
/** Internal function
/**-------------------------------------*/
double leftall(double u, double w, double c);

double rightall(double u, double w, double c);

double triangle(double u, double w, double c);

float percent(float input, float left, float right);

double percentMonth(double month);

double percentDay(double hour);

double MIN5(double num1, double num2, double num3, double num4, double num5);

double MAX5(double num1, double num2, double num3, double num4, double num5);

int fuzzyify_two(double u, IN_MEM *mem);

int fuzzyify_three(double u, IN_MEM *mem);

double fuzzy_control_lwf(double tem, double hum, double presDown,
	double rateYear, double rateDay, FUZ_SYS_LWF *fuzzy_system_lwf);

double inf_defuzz_lwf2(IN_MEM *tem_mem, IN_MEM *hum_mem, IN_MEM *presDown_mem,
	IN_MEM *rateYear_mem, IN_MEM *rateDay_mem, OUT_MEM *out_mem);

void fineMaxMin(const IN_MEM *tem_mem, const IN_MEM *hum_mem, const IN_MEM *presDown_mem,
	const IN_MEM *rateYear_mem, const IN_MEM *rateDay_mem, OUT_MEM *out_mem);

double inf_defuzz_lwf2(IN_MEM *tem_mem, IN_MEM *hum_mem, IN_MEM *presDown_mem,
	IN_MEM *rateYear_mem, IN_MEM *rateDay_mem, OUT_MEM *out_mem);

void match_lwf(const IN_MEM *tem_mem, const IN_MEM *hum_mem, const IN_MEM *presdown_mem,
	const IN_MEM *rateYear_mem, const IN_MEM *rateDay_mem, int *pos);

float pushPresDownArray(float pushValue);