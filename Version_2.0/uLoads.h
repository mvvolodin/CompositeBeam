﻿//---------------------------------------------------------------------------

#ifndef uLoadsH
#define uLoadsH
//---------------------------------------------------------------------------
#include <cmath>
#include <ostream>
#include <istream>
#include <cassert>
#include <map>
#include "uUnits_new.h"
#include "uWord_Automation.h"

class Loads {
public:
	Loads() = default;
	Loads(double SW_steel_beam,
		  double SW_corrugated_sheets,
		  double SW_add_concrete,
		  double DL_I,
		  double DL_II,
		  double LL,
		  double gamma_f_SW,
		  double gamma_f_concrete_SW,
		  double gamma_f_add_concrete_SW,
		  double gamma_f_DL_I,
		  double gamma_f_DL_II,
		  double gamma_f_LL,
		  double sheeting_continuity_coefficient,
          double fact_quasi_perm_load_,
		  double sigma_bi,
		  double sigma_si);
	Loads(double SW_steel_beam,
		  double SW_corrugated_sheets,
		  double SW_conc,
		  double SW_add_concrete,
		  double DL_I,
		  double DL_II,
		  double LL,
		  double gamma_f_SW,
		  double gamma_f_concrete_SW,
		  double gamma_f_add_concrete_SW,
		  double gamma_f_DL_I,
		  double gamma_f_DL_II,
		  double gamma_f_LL,
		  double sheeting_continuity_coefficient,
		  double fact_quasi_perm_load,
		  double sigma_bi,
		  double sigma_si,
		  double B);
	void set_default_values();
	void set_data(double SW_steel_beam, double SW_corrugated_sheets, double SW_concrete, double B);
    void set_B(double B){B_ = B;}
	void calculate_loads(double SW_concrete);
	void save(std::ostream& ostr) const;
	void load(std::istream& istr);
  	bool get_status()const{return fully_initialized_;}

	double LCC_1a_des()const;
	double LCC_1b_des()const;
	double LCC_2d_des()const;
	double LCC_2d_DL_des()const;

	double LCC_1a()const;
	double LCC_1b()const;
	double LCC_2d()const;
    double LCC_2d_DL()const;

	double get_self_weight(LoadUnit load_unit=LoadUnit::N, LengthUnit length_unit=LengthUnit::mm) const
		{return SW_steel_beam_/static_cast<int>(load_unit)*static_cast<int>(length_unit);}
	double get_self_weight_sheets(LoadUnit load_unit=LoadUnit::N, LengthUnit length_unit=LengthUnit::mm) const
		 {return SW_corrugated_sheets_/static_cast<int>(load_unit)*std::pow(static_cast<int>(length_unit),2);}
	double get_SW_concrete(LoadUnit load_unit=LoadUnit::N, LengthUnit length_unit=LengthUnit::mm) const
		 {return SW_concrete_/static_cast<int>(load_unit)*std::pow(static_cast<int>(length_unit),2);}
	double get_SW_add_concrete(LoadUnit load_unit=LoadUnit::N, LengthUnit length_unit=LengthUnit::mm) const
		 {return SW_add_concrete_/static_cast<int>(load_unit)*std::pow(static_cast<int>(length_unit),2);}
	double get_dead_load_first_stage(LoadUnit load_unit=LoadUnit::N, LengthUnit length_unit=LengthUnit::mm) const
		{return DL_I_/static_cast<int>(load_unit)*std::pow(static_cast<int>(length_unit),2);}
	double get_dead_load_second_stage(LoadUnit load_unit=LoadUnit::N, LengthUnit length_unit=LengthUnit::mm) const
		{return DL_II_/static_cast<int>(load_unit)*std::pow(static_cast<int>(length_unit),2);}
	double get_live_load(LoadUnit load_unit=LoadUnit::N, LengthUnit length_unit=LengthUnit::mm) const
		{return LL_/static_cast<int>(load_unit)*std::pow(static_cast<int>(length_unit),2);}

	double get_gamma_f_st_SW() const {return gamma_f_st_SW_;}
	double get_gamma_f_concrete_SW()const {return gamma_f_concrete_SW_;}
	double get_gamma_f_add_concrete_SW()const {return gamma_f_add_concrete_SW_;}

	double get_sigma_bi()const{return sigma_bi_;}
	double get_sigma_si()const{return sigma_si_;}

	double get_gamma_f_DL_I() const {return gamma_f_DL_I_;}
	double get_gamma_f_DL_II()const{return gamma_f_DL_II_;}
	double get_gamma_f_LL()const {return gamma_f_LL_;}
	double fact_quasi_perm_load()const {return fact_quasi_perm_load_;}

	double get_sheeting_continuity_coefficient(){return sheeting_continuity_coefficient_;}

	double SW_steel_beam()const{return SW_steel_beam_;}
	double SW_concrete()const{return SW_concrete_;}

	void print_SP266(TWord_Automation & report)const;
	void print_SP35(TWord_Automation & report)const;

private:
	double SW_steel_beam_;
	double SW_corrugated_sheets_;
	double SW_concrete_;
	double SW_add_concrete_;
	double DL_I_;
	double DL_II_;
	double LL_;

	double sigma_bi_;
	double sigma_si_;

	double gamma_f_st_SW_;
	double gamma_f_concrete_SW_;
	double gamma_f_add_concrete_SW_;
	double gamma_f_DL_I_;
	double gamma_f_DL_II_;
	double gamma_f_LL_;

	double sheeting_continuity_coefficient_;
	double fact_quasi_perm_load_;

	double B_;//грузовая площадь

	bool fully_initialized_ = true;
};
#endif


