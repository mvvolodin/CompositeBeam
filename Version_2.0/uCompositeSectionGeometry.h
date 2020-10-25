//---------------------------------------------------------------------------

#ifndef uCompositeSectionGeometryH
#define uCompositeSectionGeometryH

#include <ostream>
#include <memory>

#include "uSteelPart.h"
#include "uConcretePart.h"
#include "uRebar.h"
#include "uGeneralConcreteSection.h"
#include "uGeneralSteelSection.h"

class ComposSectGeomSP35{
	enum class NA_Location{
		CONCRETE,
		UPPER_FLANGE,
		WEB,
		NO_SOLUTION
	};
	struct NeutralAxis{
		NA_Location na_location_;
		double x_na_; //���������� �� �������� ����� ������������������� ����� �� ����������� ���
	};
private:
	const Steel steel_; // �������� ��������, �� ��� ������, �������� ����������� �������
	const std::unique_ptr<const GeneralSteelSection> st_sect_;
	const Concrete concrete_;
	const std::unique_ptr<const GeneralConcreteSection> conc_sect_;

	void calculate();

	double alfa_s();
	double alfa_b();
	double H_red();
	double A_red();
	double Z_b_st();
	double Z_st_r_u();
	double Z_st_r_l();
	double S_red();
	double Z_st_red();
	double I_red();




	double n_r_= 0.;//����������� ���������� � �����
	double n_b_= 0.;//����������� ���������� � ������
	double H_stb_= 0.; //������ ������������ �������
	double A_stb_= 0.; //������� �������������������� �������
	double Z_st_r_u_= 0.;//���������� ����� �.�. �������� ����� � ������� ���������
	double Z_st_r_l_= 0.;//���������� ����� �.�. �������� ����� � ������ ���������
	double Z_b_st_= 0.; //���������� ����� �������� ������� ����� � �������� �����
	double S_stb_= 0.;//����������� ������ ������� �������������������� ������� ������������ �.�. ��������� �������
	double I_stb_= 0.;  //������ ������� �������������������� ������� ����������� � �������
	double Z_s_stb_= 0.;//���������� ����� ������� ������� ��������� � �������������������� �������
	double Z_red_f2_= 0.;//���������� ����� �.�. �������������������� ������� � �������� ������ ������� �����
	double Z_red_f1_= 0.; //���������� ����� �.�. �������������������� ������� � �������� ������ ������ �����
	double Z_b_stb_= 0.;  //���������� ����� �.�. ��������� � �������������������� �������
	double Z_red_r_u_= 0.;//���������� ����� �.�. �������������������� ������� � ������� ���������
	double Z_red_r_l_= 0.;//���������� ����� �.�. �������������������� ������� � ������ ���������
	double W_f2_red_= 0.; //������ ������������� �������������������� ������� ��� ������� �����
	double W_f1_red_= 0.; //������ ������������� �������������������� ������� ��� ������ �����
	double W_b_stb_= 0.;//������ ������������� �������������������� ������� ��� �.�. �������������� �����
	double W_b_st_ = 0.;//�������� ������ ������������� �� ������ ����� ������� ������� ������

	double M_Rd_ = 0.;
public:
	ComposSectGeomSP35(const Steel& steel,
							  std::unique_ptr<const GeneralSteelSection> st_sect,
							  const Concrete& concrete,
							  std::unique_ptr<const GeneralConcreteSection> conc_sect);
	double E_b()const;
	double I_b()const;
	double E_st()const;
	double I_s()const;
	double W_b_stb()const;
	double n_b()const;
	double n_r()const;
    double R_y()const;
	double R_b()const;
	double R_r()const;
	double Z_b_s()const;
	double W_s2_s()const;
	double W_s1_s()const;
	double A_s2()const;
    double A_s1()const;
	double A_s()const;
	double A_b()const;
	double A_r()const;




};

class CompositeSectionGeometry{
	enum class NA_Location{
		CONCRETE,
	  //	BTW_CONCRETE_AND_UPPER_FLANGE,
		UPPER_FLANGE,
		WEB,
		NO_SOLUTION
	};
	struct NeutralAxis{
		NA_Location na_location_;
		double x_na_; //���������� �� �������� ����� ������������������� ����� �� ����������� ���
	};
private:

	SteelPart steel_part_;
	ConcretePart concrete_part_;

	void calculate();
	bool composite_section_calculated = false;

	double alfa_s_= 0.;//����������� ���������� � �����
	double alfa_b_= 0.;//����������� ���������� � ������
	double H_red_= 0.; //������ ������������ �������
	double A_red_= 0.; //������� �������������������� �������
	double Z_st_r_u_= 0.;//���������� ����� �.�. �������� ����� � ������� ���������
	double Z_st_r_l_= 0.;//���������� ����� �.�. �������� ����� � ������ ���������
	double Z_b_st_= 0.; //���������� ����� �������� ������� ����� � �������� �����
	double S_red_= 0.;//����������� ������ ������� �������������������� ������� ������������ �.�. ��������� �������
	double I_red_= 0.;  //������ ������� �������������������� ������� ����������� � �������
	double Z_st_red_= 0.;//���������� ����� ������� ������� ��������� � �������������������� �������
	double Z_red_f2_= 0.;//���������� ����� �.�. �������������������� ������� � �������� ������ ������� �����
	double Z_red_f1_= 0.; //���������� ����� �.�. �������������������� ������� � �������� ������ ������ �����
	double Z_b_red_= 0.;  //���������� ����� �.�. ��������� � �������������������� �������
	double Z_red_r_u_= 0.;//���������� ����� �.�. �������������������� ������� � ������� ���������
	double Z_red_r_l_= 0.;//���������� ����� �.�. �������������������� ������� � ������ ���������
	double W_f2_red_= 0.; //������ ������������� �������������������� ������� ��� ������� �����
	double W_f1_red_= 0.; //������ ������������� �������������������� ������� ��� ������ �����
	double W_b_red_= 0.;//������ ������������� �������������������� ������� ��� �.�. �������������� �����
	double W_b_st_ = 0.;//�������� ������ ������������� �� ������ ����� ������� ������� ������

	double M_Rd_ = 0.;

	void alfa_to_rebar_steel_calc();
	void alfa_to_concrete_calc();
	void compos_sect_height_calc();
	void compos_sect_area_calc();
	void dist_conc_to_steel_calc();
	void first_moment_of_area_comp_calc();
	void dist_steel_to_compos_calc();
	void inertia_compos_calc();
	void dist_compos_to_upper_fl_calc();
	void dist_compos_to_lower_fl_calc();
	void dist_conc_to_compos_calc();
	void sect_modulus_upper_fl();
	void sect_modulus_lower_fl();
	void sect_modulus_conc();
	void fictitious_modulus();

	void calc_rigid_plastic_moment();
	NeutralAxis calc_neutral_axis();

public:
	CompositeSectionGeometry();
	CompositeSectionGeometry(SteelPart    steel_part,
							 ConcretePart concrete_part);
	void set_default_values();
	void save(std::ostream& ostr)const;
	void load(std::istream& istr);

	SteelPart& get_steel_part(){return steel_part_;}
	const SteelPart& get_steel_part()const {return steel_part_;}
	const ConcretePart& get_concrete_part() {return concrete_part_;}

	void set_phi_b_cr(double phi_b_cr);
	void set_b_sl(double b_sl){concrete_part_.set_b_sl(b_sl);}

	double get_alfa_b();
	double get_alfa_s();
	double get_A_red(LengthUnit length_unit=LengthUnit::mm);
	double get_I_red(LengthUnit length_unit=LengthUnit::mm);
	double get_W_b_red(LengthUnit length_unit=LengthUnit::mm);
	double get_W_f2_red(LengthUnit length_unit=LengthUnit::mm);
	double get_W_f1_red(LengthUnit length_unit=LengthUnit::mm);
	double get_Z_b_red(LengthUnit length_unit=LengthUnit::mm);
	double get_Z_st_red(LengthUnit length_unit=LengthUnit::mm);
	double get_Z_f2_red(LengthUnit length_unit=LengthUnit::mm);
	double get_Z_f1_red(LengthUnit length_unit=LengthUnit::mm);
	double get_Z_b_st(LengthUnit length_unit=LengthUnit::mm);
	double get_W_b_st(LengthUnit length_unit=LengthUnit::mm);

	double get_b_s()const {return concrete_part_.get_rebar().get_b_s();}
	double get_M_Rd(LoadUnit load_unit=LoadUnit::N, LengthUnit length_unit=LengthUnit::mm);
	double get_Q_Rd()const;

	double get_E_st_I_red()const;
	double get_E_st_I_st()const;


} ;
//---------------------------------------------------------------------------
#endif


