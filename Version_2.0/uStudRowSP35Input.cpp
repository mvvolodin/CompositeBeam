//---------------------------------------------------------------------------

#pragma hdrstop

#include "uStudRowSP35Input.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

StudSP35 StudRowSP35Input::st_ {u"stud", 1, 1, 1, 1, 1};

StudRowSP35Input::StudRowSP35Input(double x, double x_l, double x_r, int st_num):
	x_(x), x_l_(x_l), x_r_(x_r), st_num_(st_num){}
