//---------------------------------------------------------------------------

#pragma hdrstop

#include "uComposSectCalculatorS35.h"
#include "uBilinearInterp.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


ComposSectCalculatorSP35::ComposSectCalculatorSP35
	(IntForcesCalculator const & intr_frcs_calculator,
	 WorkingConditionsFactors const & work_cond_factors,
	 ComposSectGeomSP35 const & com_sect,
	 double const sigma_bi,
	 double const sigma_ri):
		intr_frcs_calculator_(intr_frcs_calculator),
		work_cond_factors_(work_cond_factors),
		com_sect_(com_sect),
		sigma_bi_(sigma_bi),
		sigma_ri_(sigma_ri){}

SectOutputSP35 ComposSectCalculatorSP35::calculate(int const id, double const x)
{
	double const M_1a = intr_frcs_calculator_.M_1a(x);
	double const M_1b = intr_frcs_calculator_.M_1b(x);
	double const M_2c = intr_frcs_calculator_.M_2c(x);
	double const M_2d = intr_frcs_calculator_.M_2d(x);

	double const M_1 = M_1b;
	double const M_2 = M_2c + M_2d;
	double const M = M_1 + M_2;

	double const Q_1a = intr_frcs_calculator_.Q_1a(x);
	double const Q_1b = intr_frcs_calculator_.Q_1b(x);
	double const Q_2c = intr_frcs_calculator_.Q_2c(x);
	double const Q_2d = intr_frcs_calculator_.Q_2d(x);

	double const Q_1 = Q_1b;
	double const Q_2 = Q_2c + Q_2d;
	double const Q = Q_1 + Q_2;

	double const f_1a = intr_frcs_calculator_.f_1a(x);
	double const f_1b = intr_frcs_calculator_.f_1b(x);
	double const f_2c = intr_frcs_calculator_.f_2c(x);
	double const f_2d = intr_frcs_calculator_.f_2d(x);

	double const W_b_stb = com_sect_.W_b_stb();
	double const n_b = com_sect_.n_b();
	double const n_r = com_sect_.n_r();

	double const sigma_b = M_2 / (n_b * W_b_stb) -  sigma_bi_;
	double const sigma_r = M_2 / (n_r * W_b_stb) +  sigma_ri_;

	double const Z_b_s = com_sect_.Z_b_s();
	double const A_s = com_sect_.A_s();
	double const A_s2 = com_sect_.A_s2();
	double const A_s1 = com_sect_.A_s1();
	double const W_s2_s = com_sect_.W_s2_s();
	double const W_s1_s = com_sect_.W_s1_s();

	double const A_b = com_sect_.A_b();
	double const A_r = com_sect_.A_r();

	double const R_y = com_sect_.R_y();
	double const R_b = com_sect_.R_b();
	double const R_r = com_sect_.R_r();

	double const m = work_cond_factors_.m();
	double const m_b = work_cond_factors_.m_b();

	double const omega = 0;//8.16

	double const fl_ratio = A_s2 / A_s1;
//	double const A_f_min_to_A_w_ratio = A
//	double const A_f_min_plus_A_w_to_A_ratio =

	SP_35_13330_2011_table_9_5::FlangeBendingAndAxialStressSumUp fl = (A_s2 < A_s1)?
		SP_35_13330_2011_table_9_5::FlangeBendingAndAxialStressSumUp::bigger:
		SP_35_13330_2011_table_9_5::FlangeBendingAndAxialStressSumUp::smaller;
	double str_ratio = 0.;
	double str_s2_ratio = 0.;
	double str_s1_ratio = 0.;

	double eta = 0.;
	double eta_s2 = 0.;
	double eta_s1 = 0.;

	double omega_4 = 0;
	double omega_3 = 0;
	double omega_3_s2 = 0;
	double omega_3_s1 = 0;

	double fl_s2_ratio = 0.;
	double fl_s1_ratio = 0.;
	double conc_ratio = 0.;
	double shear_ratio = 0.;
	double i_section_ratio = 0.;

	double N_br = 0.;
	double N_br_R = 0.;
	double N_bR_r = 0.;

	double m_1 = 0.;

	DesignCase const des_case = design_case(sigma_b, sigma_r);

	switch (des_case) {

	case DesignCase::Case_A:
		if((m_1 = 1 + (m_b * R_b - sigma_b) / (m * R_y) * A_b / A_s2) >= 1.2) m_1 = 1.2;

		N_br = A_b * sigma_b + A_r * sigma_r;

		str_ratio = N_br / (A_s * m * R_y);

		eta = SP_35_13330_2011_table_9_5::bilinear_interpolation(fl_ratio, str_ratio, fl);

		//omega = SP_35_13330_2011_table_8_16::bilinear_interpolation();
		omega_3 = 1 + eta * (omega - 1);
		omega_4 = omega_3 / m_1;

		fl_s2_ratio = ((M - Z_b_s * N_br) / (omega_4 * W_s2_s) - N_br / A_s) / (m_1 * m * R_y);
		fl_s1_ratio = ((M - Z_b_s * N_br) / (omega_3 * W_s2_s) + N_br / A_s) / (m * R_y);

		return SectOutputSP35{id, x,
							  M_1a, M_1b, M_2c, M_2d,
							  Q_1a, Q_1b, Q_2c, Q_2d,
							  f_1a, f_1b, f_2c, f_2d,
							  sigma_b, sigma_r,
							  sigma_bi_, sigma_ri_,
							  des_case,
							  fl_s2_ratio,
							  fl_s1_ratio,
							  conc_ratio,
							  shear_ratio,
							  i_section_ratio};


	case DesignCase::Case_B:

		N_br_R = A_b * R_b + A_r * R_r;
		N_bR_r = A_b * R_b + A_r * sigma_r;

		str_s2_ratio = N_br_R / (A_s * m * R_y);
		str_s1_ratio = N_bR_r / (A_s * m * R_y);

		eta_s2 = SP_35_13330_2011_table_9_5::bilinear_interpolation(fl_ratio, str_s2_ratio, fl);
		eta_s1 = SP_35_13330_2011_table_9_5::bilinear_interpolation(fl_ratio, str_s1_ratio, fl);
		//omega = SP_35_13330_2011_table_8_16::bilinear_interpolation();
		omega_3_s2 = 1 + eta_s2 * (omega - 1);
		omega_3_s1 = 1 + eta_s1 * (omega - 1);

		fl_s2_ratio = ((M - Z_b_s * N_br_R) / (omega_3_s2 * W_s2_s) - N_br_R / A_s) / (m * R_y);
		fl_s1_ratio = ((M - Z_b_s * N_bR_r) / (omega_3_s1 * W_s2_s) + N_bR_r / A_s) / (m * R_y);

		return SectOutputSP35{id, x,
							  M_1a, M_1b, M_2c, M_2d,
							  Q_1a, Q_1b, Q_2c, Q_2d,
							  f_1a, f_1b, f_2c, f_2d,
							  sigma_b, sigma_r,
							  sigma_bi_, sigma_ri_,
							  des_case,
							  fl_s2_ratio,
							  fl_s1_ratio,
							  conc_ratio,
							  shear_ratio,
							  i_section_ratio};

	case DesignCase::Case_C:

		N_br_R = A_b * R_b + A_r * R_r;

		str_ratio = N_br_R  / (A_s * m * R_y);

		eta = SP_35_13330_2011_table_9_5::bilinear_interpolation(fl_ratio, str_ratio, fl);

		//omega = SP_35_13330_2011_table_8_16::bilinear_interpolation();
		omega_3 = 1 + eta * (omega - 1);

		fl_s2_ratio = ((M - Z_b_s * N_br_R) / (omega_3 * W_s2_s) - N_br_R / A_s) / (m * R_y);
		fl_s1_ratio = ((M - Z_b_s * N_br_R) / (omega_3 * W_s2_s) + N_br_R / A_s) / (m * R_y);


	   return SectOutputSP35{id, x,
							 M_1a, M_1b, M_2c, M_2d,
							 Q_1a, Q_1b, Q_2c, Q_2d,
							 f_1a, f_1b, f_2c, f_2d,
							 sigma_b, sigma_r,
							 sigma_bi_, sigma_ri_,
							 des_case,
							 fl_s2_ratio,
							 fl_s1_ratio,
							 conc_ratio,
							 shear_ratio,
							 i_section_ratio};
   }

}

double ComposSectCalculatorSP35::N(DesignCase dc, Flange fl)
{

}

DesignCase ComposSectCalculatorSP35::design_case(double const sigma_b, double const sigma_r)
{
	double const E_b = com_sect_.E_b();
	double const I_b = com_sect_.I_b();
	double const I_s = com_sect_.I_s();
	double const E_st = com_sect_.E_st();

	double const R_b = com_sect_.R_b();
	double const R_r = com_sect_.R_r();
	double const W_b_stb = com_sect_.W_b_stb();
	double const n_b = com_sect_.n_b();
	double const n_r = com_sect_.n_r();

	double m_b = work_cond_factors_.m_b();
	double m_r = work_cond_factors_.m_r();

	if (/*(E_b * I_b <= 0.2 * E_st * I_s )*/ 1 &&(sigma_b < m_b * R_b)&&(sigma_r < m_r * R_r))
		return DesignCase::Case_A;

	if ((sigma_b >= m_b * R_b)&&(sigma_r < m_r * R_r))
		return DesignCase::Case_B;

	if ((sigma_b >= m_b * R_b)&&(sigma_r >= m_r * R_r))
		return DesignCase::Case_C;
}
