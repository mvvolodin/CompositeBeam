﻿//---------------------------------------------------------------------------
#pragma hdrstop
#include "uCompSectGeomSP35.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
CompSectGeomSP35::CompSectGeomSP35(Steel const & steel,
	std::unique_ptr<GeneralSteelSection> st_sect,
	ConcreteSP35 const & concrete,
	std::unique_ptr<GeneralConcreteSection> conc_sect,
	ConcStateConsid const state):
		steel_(steel), st_sect_(std::move(st_sect)),
		concrete_(concrete), conc_sect_(std::move(conc_sect))
{
	calculate(state);
}

double CompSectGeomSP35::E_ef_kr()const
{
	double const E_st = steel_.get_E_st();
	double const E_b = concrete_.E_b();
	double const E_rs = conc_sect_ -> rebars().rebar().E_s();

	double const n_b = E_st / E_b;
	double const n_r = E_st / E_rs;

	double const C_b = conc_sect_ -> C_b();
	double const a_l_r = conc_sect_ -> rebars().a_l();
	double const a_u_r = conc_sect_ -> rebars().a_u();
	double const Z_s2_s = st_sect_ -> Z_s2_s();
	double const h_n = conc_sect_ -> h_n();
	double const h_f = conc_sect_ -> h_f();

	double const A_u_r = conc_sect_ -> rebars().A_u_r_per_unit() * conc_sect_ -> b_sl();
	double const A_l_r = conc_sect_ -> rebars().A_l_r_per_unit() * conc_sect_ -> b_sl();
	double const A_s = st_sect_ -> A_s();
	double const A_b = conc_sect_ -> A_b();

	double const I_s = st_sect_ -> I_s();

	double const Z_s_st =
		(A_l_r / n_r * (a_l_r + h_n + Z_s2_s) + A_u_r / n_r * (Z_s2_s + h_n + h_f - a_u_r)) /
		(A_u_r / n_r + A_l_r / n_r + A_s);
	double const Z_s_r_u = Z_s2_s + h_n + h_f - a_u_r;
	double const Z_s_r_l = Z_s2_s + h_n + a_l_r;

	double const A_st = A_s + A_u_r / n_r + A_l_r / n_r;

	double const I_st =
		I_s + A_s * Z_s_st * Z_s_st + A_l_r / n_r * (Z_s_r_l - Z_s_st) * (Z_s_r_l - Z_s_st) +
		A_u_r / n_r * (Z_s_r_u - Z_s_st) * (Z_s_r_u - Z_s_st);

	double const Z_b_st = Z_s2_s + C_b - Z_s_st;

	double const c_n = concrete_.c_n();
    /* TODO 1 -oMV : Сделать gama_f_shr_kr переменной */
	double const gamma_f_shr_kr = 1.1;

	double const phi_kr = gamma_f_shr_kr * E_b * c_n;

	double const nu = A_b / n_b * (1 / A_st + Z_b_st * Z_b_st / I_st);

	return (nu - 0.5 * phi_kr + 1) / ((1 + phi_kr) * nu + 0.5 * phi_kr + 1) * E_b;
}
void CompSectGeomSP35::calculate(ConcStateConsid conc_st_consid)
{
	double const E_b = concrete_.E_b();

	switch (conc_st_consid) {

	case ConcStateConsid::normal:
		calculate(concrete_.E_b());
		break;

	case ConcStateConsid::shrink:
		calculate(concrete_.E_b_shr());
		break;

	case ConcStateConsid::creep:
		calculate(E_ef_kr());
		break;
	}

}

void CompSectGeomSP35::calculate(double const E_b)
{

	double const E_st = steel_.get_E_st();
	double const E_rs = conc_sect_ -> rebars().rebar().E_s();

	n_b_ = E_st / E_b;
	n_r_ = E_st / E_rs;

	double const A_s = st_sect_ -> A_s();
	double const A_b = conc_sect_ -> A_b();
	double const A_u_r = conc_sect_ -> rebars().A_u_r_per_unit() * conc_sect_ -> b_sl();
	double const A_l_r = conc_sect_ -> rebars().A_l_r_per_unit() * conc_sect_ -> b_sl();

	A_stb_= A_s + A_b / n_b_ + A_u_r / n_r_ + A_l_r / n_r_;

	double const h_s = st_sect_ -> h_s();
	double const h_n = conc_sect_ -> h_n();
	double const h_f = conc_sect_ -> h_f();
	double const a_l_r = conc_sect_ -> rebars().a_l();
	double const a_u_r = conc_sect_ -> rebars().a_u();
	double const Z_s2_s = st_sect_ -> Z_s2_s();
	double const C_b = conc_sect_ -> C_b();

	h_stb_ = h_s + h_n + h_f;

	Z_b_s_ = C_b + Z_s2_s;
	Z_s_r_u_ = Z_s2_s + h_n + h_f - a_u_r;
	Z_s_r_l_ = Z_s2_s + h_n + a_l_r;

	S_stb_ = A_b * Z_b_s_ / n_b_ + A_u_r * Z_s_r_u_ / n_r_  + A_l_r * Z_s_r_l_ / n_r_;

	Z_s_stb_ = S_stb_/A_stb_;
	Z_b_stb_ = Z_b_s_ - Z_s_stb_;
	Z_r_u_stb_ = Z_b_stb_ - C_b + h_n + h_f - a_u_r;
	Z_r_l_stb_ = Z_b_stb_ - C_b + a_l_r;

	double const I_s = st_sect_ -> I_s();
	double const I_b = conc_sect_ -> I_b();

	I_stb_ = I_s + A_s * Z_s_stb_ * Z_s_stb_ +
			 I_b / n_b_ + A_b * Z_b_stb_ * Z_b_stb_ / n_b_ +
			 A_u_r * Z_r_u_stb_ * Z_r_u_stb_ / n_r_  +
			 A_l_r * Z_r_l_stb_ * Z_r_l_stb_ / n_r_ ;

	W_b_stb_ = I_stb_ / Z_b_stb_;

	W_b_s_ = I_s / Z_b_s_;

}
double CompSectGeomSP35::E_b()const
{
	return concrete_.E_b();
}
double CompSectGeomSP35::E_b_shr()const
{
	return concrete_.E_b_shr();
}
double CompSectGeomSP35::I_b()const
{
	return conc_sect_ -> I_b();
}
double CompSectGeomSP35::E_st()const
{
	return steel_.get_E_st();
}
double CompSectGeomSP35::I_s()const
{
	return st_sect_ -> I_s();
}
double CompSectGeomSP35::W_b_stb()const
{
	return W_b_stb_;
}
double CompSectGeomSP35::W_b_s()const
{
	return W_b_s_;
}
double CompSectGeomSP35::n_b()const
{
	return n_b_;
}
double CompSectGeomSP35::n_r()const
{
	return n_r_;
}
double CompSectGeomSP35::R_b()const
{
	return concrete_.R_b();
}

double CompSectGeomSP35::R_r()const
{
	return conc_sect_ -> rebars().rebar().R_s();
}
double CompSectGeomSP35::eps_b_lim() const
{
	return concrete_.eps_b_lim();
}
double CompSectGeomSP35::h_stb()const
{
	return h_stb_;
}
double CompSectGeomSP35::Z_b_s()const
{
	return Z_b_s_;
}
double CompSectGeomSP35::W_s2_s()const
{
	return st_sect_ -> W_s2_s();
}
double CompSectGeomSP35::W_s1_s()const
{
	return st_sect_ -> W_s1_s();
}
double CompSectGeomSP35::A_w()const
{
	return st_sect_ -> A_w();
}
double CompSectGeomSP35::A_s2()const
{
	return st_sect_ -> A_s2_s();
}
double CompSectGeomSP35::A_s1()const
{
	return st_sect_ -> A_s1_s();
}
double CompSectGeomSP35::b_sl()const
{
    return conc_sect_ -> b_sl();
}
double CompSectGeomSP35::A_s()const
{
	return st_sect_ -> A_s() ;
}
double CompSectGeomSP35::A_b()const
{
	return conc_sect_ -> A_b();
}
double CompSectGeomSP35::A_r()const
{
	return conc_sect_ -> rebars().A_u_r_per_unit() * conc_sect_ -> b_sl() +
		   conc_sect_ -> rebars().A_l_r_per_unit() * conc_sect_ -> b_sl();
}

double CompSectGeomSP35::E_rs()const
{
	return conc_sect_ -> rebars().rebar().E_rs();
}
double CompSectGeomSP35::R_y()const
{
	return steel_.get_R_y();
}

double CompSectGeomSP35::A_st()const
{
	double const E_st = steel_.get_E_st();
	double const E_rs = conc_sect_ -> rebars().rebar().E_s();
	double const n_r = E_st / E_rs;

	double const A_s = st_sect_ -> A_s();
	double const A_u_r = conc_sect_ -> rebars().A_u_r_per_unit() * conc_sect_ -> b_sl();
	double const A_l_r = conc_sect_ -> rebars().A_l_r_per_unit() * conc_sect_ -> b_sl();

	return  A_s + A_u_r / n_r + A_l_r / n_r;
}

double CompSectGeomSP35::S_st()const  //в норме обозначение S_shr
{
	double const E_st = steel_.get_E_st();
	double const E_rs = conc_sect_ -> rebars().rebar().E_s();
	double const n_r = E_st / E_rs;

	double const Z_s2_s = st_sect_ -> Z_s2_s();
	double const h_n = conc_sect_ -> h_n();
	double const h_f = conc_sect_ -> h_f();
	double const a_l_r = conc_sect_ -> rebars().a_l();
	double const a_u_r = conc_sect_ -> rebars().a_u();

	double const A_s = st_sect_ -> A_s();
	double const A_u_r = conc_sect_ -> rebars().A_u_r_per_unit() * conc_sect_ -> b_sl();
	double const A_l_r = conc_sect_ -> rebars().A_l_r_per_unit() * conc_sect_ -> b_sl();
	double const Z_st_stb = Z_s_stb_ -
		(A_l_r / n_r * (a_l_r + h_n + Z_s2_s) + A_u_r / n_r * (Z_s2_s + h_n + h_f - a_u_r)) /
		(A_u_r / n_r + A_l_r / n_r + A_s);

	return A_st() * Z_st_stb;
}
double CompSectGeomSP35::I_st()const
{
	return I_st_;
}

double CompSectGeomSP35::A_stb()const
{
	return A_stb_;
}
double CompSectGeomSP35::I_stb()const
{
	return I_stb_;
}
double CompSectGeomSP35::Z_s_stb()const
{
	return Z_s_stb_;
}
double CompSectGeomSP35::Z_b_st()const
{
	return Z_b_st_;
}
double CompSectGeomSP35::Z_b_stb()const
{
	return Z_b_stb_;
}
double CompSectGeomSP35::Z_r_stb()const
{
	return (Z_r_l_stb_ + Z_r_u_stb_) / 2;
}
double CompSectGeomSP35::eps_shr()const
{
	return concrete_.eps_shr();
}

void CompSectGeomSP35::print(TWord_Automation & report)const
{
	st_sect_ -> print(report);
	steel_.print_SP266(report);
	conc_sect_ -> print_SP266(report);
	concrete_.print(report);

	report.PasteTextPattern(area_to_str(A_stb_),"%A_stb%");
	report.PasteTextPattern(inertia_to_str(I_stb_),"%I_stb%");
	report.PasteTextPattern(sect_modul_to_str(W_b_stb_),"%W_b_stb%");
	report.PasteTextPattern(length_to_str(Z_b_stb_, LengthUnit::cm),"%Z_b_stb%");
	report.PasteTextPattern(length_to_str(Z_s_stb_, LengthUnit::cm),"%Z_s_stb%");
	report.PasteTextPattern(length_to_str(Z_b_s_, LengthUnit::cm),"%Z_b_s%");
}
#ifndef NDEBUG
void CompSectGeomSP35::print_data_to_logger(TFormLogger const & log)const
{
	log.add_heading(L"Геометрические характеристики композитного сечения");
	log.print_double(L"n_r = ", n_r_, L" ");
	log.print_double(L"n_b = ", n_b_, L" ");
	log.print_double(L"H_stb = ", h_stb_, L" мм");
	log.print_double(L"A_stb = ", A_stb_, L" мм2");
	log.print_double(L"S_stb = ", S_stb_, L" мм3");
	log.print_double(L"I_stb = ", I_stb_, L" мм4");
	log.print_double(L"Z_s_stb = ", Z_s_stb_, L" мм");
	log.print_double(L"Z_b_stb = ", Z_b_stb_, L" мм");
}
#endif
void CompSectGeomSP35::fill_steel_sect_grid(TStringGrid* str_grid)const
{
	st_sect_ -> fill_grid(str_grid);
}
void CompSectGeomSP35::fill_conc_sect_grid(TStringGrid* str_grid)const
{
	conc_sect_ -> fill_grid(str_grid);
}
void CompSectGeomSP35::fill_comp_sect_grid(TStringGrid* str_grid)const
{
	str_grid -> Cells [1][1] = FloatToStrF(A_stb_, ffFixed, 15, 0);
	str_grid -> Cells [1][2] = FloatToStrF(I_stb_, ffFixed, 15, 0);
	str_grid -> Cells [1][3] = FloatToStrF(W_b_stb_, ffFixed, 15, 0);
	str_grid -> Cells [1][4] = FloatToStrF(Z_b_stb_, ffFixed, 15, 0);
	str_grid -> Cells [1][5] = FloatToStrF(Z_s_stb_, ffFixed, 15, 0);
	str_grid -> Cells [1][6] = FloatToStrF(Z_b_s_, ffFixed, 15, 0);
}
//---------------------------------------------------------------------------

