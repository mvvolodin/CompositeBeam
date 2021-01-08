﻿//---------------------------------------------------------------------------
#ifndef uComposSectGeomSP35H
#define uComposSectGeomSP35H
//---------------------------------------------------------------------------
#include <memory>
#include <Vcl.Grids.hpp>
#include "uGeneralConcreteSection.h"
#include "uGeneralSteelSection.h"
#include "uConcreteSP35.h"
#include "uSteel.h"
#include "uWord_Automation.h"
//---------------------------------------------------------------------------
class ComposSectGeomSP35{

public:

	enum class ConcStateConsid{
		normal,
		shrink,
		creep,
	};
	ComposSectGeomSP35() = default;

	ComposSectGeomSP35(Steel const & steel,
					   GeneralSteelSection const * const st_sect,
					   ConcreteSP35 const & concrete,
					   GeneralConcreteSection const * const conc_sect,
					   ConcStateConsid const conc_st_consid);

	double E_rs()const;
	double E_b()const;
	double E_b_shr()const;
	double E_st()const;

	double n_b()const;
	double n_r()const;

	double R_y()const;
	double R_b()const;
	double R_r()const;

	double eps_b_lim() const;

	double A_s()const;
	double A_s2()const;
	double A_s1()const;
    double A_w()const;
	double W_s1_s()const;
	double W_s2_s()const;
	double I_s()const;

	double A_st()const;
	double S_st()const;
    double I_st()const;

	double b_sl()const;
	double A_b()const;
	double I_b()const;

	double h_stb()const;
	double Z_b_s()const;
	double Z_b_stb()const;
    double Z_b_st()const;
	double Z_s_stb()const;
	double Z_r_stb()const;
	double A_stb()const;
	double W_b_stb()const;
    double W_b_s()const;
	double I_stb()const;
	double eps_shr()const;

	double A_r()const;

	void print(TWord_Automation & report)const;
	void fill_grid(TStringGrid* str_grid)const;
	GeneralSteelSection const * const st_sect()const;
	GeneralConcreteSection const * const conc_sect()const;

#ifndef NDEBUG
	void print_data_to_logger(TFormLogger const & log)const;
#endif

private:

	Steel const & steel_;
	GeneralSteelSection const * const st_sect_;
	ConcreteSP35 const & concrete_;
	GeneralConcreteSection const * const conc_sect_;

	void calculate(double const E_b);
	void calculate(ConcStateConsid conc_st_consid);

	double E_ef_kr()const;

	double n_r_;//коэффициент приведения к стали
	double n_b_;//коэффициент приведения к бетону

	double A_st_;//Площадь стальной части сечения включая арматуру
	double S_st_;//Статический момент стальной части сечения включая арматуру
	double I_st_;//Момент инерции стальной части сечения включая арматуру

	double h_stb_; //высота композитного сечения
	double A_stb_; //площадь сталежелезобетонного сечения
	double S_stb_;//Статический момент инерции сталежелезобетонного сечения относительно Ц.Т. стального сечения
	double I_stb_;  //Момент инерции сталежелезобетонного сечения приведённого к металлу
	double W_b_stb_;//Момент сопротивления сталежелезобетонного сечения для Ц.Т. железобетонной плиты

	double W_b_s_;//Условный момент сопротивления на уровне центра тяжести сечения бетона

	double Z_s_r_u_;//расстояние между Ц.Т. стальной балки и верхней арматурой
	double Z_s_r_l_;//расстояние между Ц.Т. стальной балки и нижней арматурой
	double Z_b_s_; //Расстояние между центрами тяжести плиты и стальной балки

	double Z_s_stb_;//Расстояние между Ц.Т. сечений стальной и сталежелезобетонной балок
	double Z_b_st_;//Расстояние между Ц.Т.бетонного сечения и стальной части включая арматуру
	double Z_s_st_; //Расстояние между Ц.Т. сечений стальной балки и стальной части включая арматуру
	double Z_st_stb_;//Расстояние между Ц.Т. стальной части сечения включая арматуру и сталежелезобетонного сечения
	double Z_b_stb_;  //Расстояние между Ц.Т. бетонного и сталежелезобетонного сечения
	double Z_r_u_stb_;//Расстояние между Ц.Т. сталежелезобетонного сечения и верхней арматурой
	double Z_r_l_stb_;//Расстояние между Ц.Т. сталежелезобетонного сечения и нижней арматурой


};

//---------------------------------------------------------------------------
#endif
