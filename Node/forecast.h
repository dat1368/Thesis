#pragma once
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <string>
#include <conio.h>
#include <vector>
#include <regex>
#include <fstream>
#include <sstream>
#include <direct.h>


#define IS_TEST_MODE 1

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

/************************************************************************************/
enum OUTPUT_
{
	RT,
	T,
	V,
	C,
	RC
};
//int LWF_Fuzzy[72] = { RC,RC,V,T,C,C,T,
//T,RC,RC,V,T,C,C,T,T,
//RC,RC,V,T,C,RC,T,T,RC
//,RC,V,V,V,V,RT,RT,RC,RC,
//V,V,V,V,RT,RT,RC,RC,V,V,
//V,V,RT,RT,RC,RC,RT,RT,C,
//V,RT,RT,RC,RC,T,RT,C,V,RT
//,RT,RC,RC,T,RT,C,V,RT,RT
//};
//int LWF_Fuzzy[72] = {
//V, V, RT, RT, V, V, T, T, C, V, T, RT, RC, C, V, T, RC, C, V, T, RC, RC, V, V, V, V, RT, RT, C, V, T, RT, C, V, T, RT, RC, C, V, T, RC, C, V, T, RC, RC, V, V, V, V, RT, RT, C, V, T, RT, C, V, T, RT, RC, C, V, T, C, C, V, V, RC, RC, V, V
//};

/*Luat mo lap vao ngay chu nhat, tai hoi quan cafe*/
/*Rule 4*/
int LWF_Fuzzy[72] = {
	/*Rule 4*/
	//RC,C,C,V,C,V,V,T,RC,C,V,T,C,V,V,T,RC,C,C,V,C,V,V,T,C,V,V,T,V,T,T,RT,RC,C,C,V,V,T,T,RT,RC,C,C,V,C,V,V,T,C,V,V,T,V,T,T,RT,C,V,V,T,C,V,T,RT,C,V,V,T,V,T,T,RT
	//RULE 5 => Toàn là không mưa
	//C,V,RT,RT,V,T,V,T,C,V,V,T,V,T,T,RT,V,T,V,T,V,T,T,RT,RC,C,RT,RT,C,V,V,T,RC,C,V,T,C,V,T,RT,C,V,V,T,V,T,T,RT,RC,C,RT,RT,C,V,RT,RT,RC,C,RT,RT,C,V,V,T,C,V,RT,RT,V,T,V,T
	/*RULE 6 => Toàn là mưa*/
	RC,C,C,V,C,V,V,T,RC,C,V,T,C,V,V,T,RC,RC,C,V,C,V,V,T,RC,C,V,T,V,T,T,RT,RC,C,C,V,V,T,T,RT,RC,RC,C,V,C,V,V,T,V,V,V,RT,T,T,T,RT,C,C,V,T,C,V,T,RT,C,C,V,T,V,T,T,RT
};
/*Rule 5*/
//int LWF_Fuzzy[72] = {
//	C,V,RT,RT,
//	V,T,V,T,
//	C,V,V,T,
//	V,T,T,RT,
//	V,T,V,T,
//	V,T,T,RT,
//	RC,C,RT,RT,
//	C,V,V,T,
//	RC,C,V,T,
//	C,V,T,RT,
//	C,V,V,T,
//	V,T,T,RT,
//	RC,C,RT,RT,
//	C,V,RT,RT,
//	RC,C,RT,RT,
//	C,V,V,T,
//	C,V,RT,RT,
//	V,T,V,T
//};
/*Rule 1*/
//int LWF_Fuzzy[72] = {
//	RC,RC,V,T,
//	RC,C,T,T,
//	RC,RC,V,T,
//	RC,C,T,T,
//	RC,RC,V,T,
//	RC,RC,T,T,
//
//	RC,RC,V,V,
//	V,V,RT,RT,
//	RC,RC,V,V,
//	V,V,RT,RT,
//	RC,RC,V,V,
//	V,V,RT,RT,
//
//	C,C,RT,RT,
//	C,V,RT,RT,
//	C,C,T,RT,
//	C,V,RT,RT,
//	C,C,T,RT,
//	C,V,RT,RT
//};


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

typedef struct MyTime
{
	int day;
	int month;
	int year;

	int hour;
	int minute;

}DateTime;

/************************************************************************************/

/* Function Prototypes: */

void fuzzy_init(FUZ_SYS *fuzzy_system);
void fuzzy_init_lwf(FUZ_SYS_LWF *fuzzy_system_lwf);

void fuzzy_free(FUZ_SYS *fuzzy_system);
double fuzzy_control(double e, double edot, FUZ_SYS *fuzzy_system);
double fuzzy_control_lwf
(
	double tem,
	double hum,
	double presDown,
	double rateYear,
	double rateDay,
	FUZ_SYS_LWF *fuzzy_system_lwf
	);

void fuzzyify(double u, IN_MEM *mem);
int fuzzyify_three(double u, IN_MEM *mem);
int fuzzyify_two(double u, IN_MEM *mem);

double leftall(double u, double w, double c);
double rightall(double u, double w, double c);
double triangle(double u, double w, double c);

void match(const IN_MEM *emem, const IN_MEM *edotmem, int *pos);
void match_lwf(
	const IN_MEM *tem_mem,
	const IN_MEM *hum_mem,
	const IN_MEM *presdown_mem,
	const IN_MEM *rateYear_mem,
	const IN_MEM *rateDay_mem,
	int *pos);


double inf_defuzz(IN_MEM *emem, IN_MEM *edotmem, OUT_MEM *outmem, int *pos);
double inf_defuzz_lwf(
	IN_MEM *tem_mem,
	IN_MEM *hum_mem,
	IN_MEM *presDown_mem,
	IN_MEM *rateYear_mem,
	IN_MEM *rateDay_mem,
	OUT_MEM *out_mem,
	int *pos);
double inf_defuzz_lwf2(IN_MEM *tem_mem, IN_MEM *hum_mem, IN_MEM *presDown_mem, IN_MEM *rateYear_mem, IN_MEM *rateDay_mem, OUT_MEM *out_mem);
double fuzzy_control_lwf(double tem, double hum, double presDown,
	double rateYear, double rateDay, FUZ_SYS_LWF *fuzzy_system_lwf);
/************************************************************************************/

double percentMonth(double month);

double percentDay(double hour);

double MIN5(double num1, double num2, double num3, double num4, double num5);

double MAX5(double num1, double num2, double num3, double num4, double num5);

float pushPresDownArray(float pushValue);
