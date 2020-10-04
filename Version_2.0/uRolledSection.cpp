//---------------------------------------------------------------------------

#pragma hdrstop

#include "uRolledSection.h"
#include <iostream>
//---------------------------------------------------------------------------
#pragma package(smart_init)

RolledSection::RolledSection(double b_f1, double t_f1,
							 double b_f2, double t_f2,
							 double h_w, double t_w,
							 double A, double I):
								 b_f1_(b_f1),
								 t_f1_(t_f1),
								 b_f2_(b_f1),
								 t_f2_(t_f1),
								 h_w_(h_w),
								 t_w_(t_w),
								 A_(A),
								 I_(I){}

double RolledSection::b_f1()const
{
	return b_f1_;
}
double RolledSection::t_f1()const
{
	return t_f1_;
}
double RolledSection::b_f2()const
{
	return b_f2_;
}
double RolledSection::t_f2()const
{
	return t_f2_;
}
double RolledSection::h_w()const
{
	return h_w_;
}
double RolledSection::t_w()const
{
	return t_w_;
}

double RolledSection::C_st()const
{
	return C_;
}

double RolledSection::A_st()const
{
	return A_;
}

double RolledSection::I_st()const
{
	return I_;
}