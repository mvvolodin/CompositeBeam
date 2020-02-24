//---------------------------------------------------------------------------

#ifndef CompositeSectionH
#define CompositeSectionH
#include "ISectionInitialDataUnit.h"
#include "ISection.h"
#include "ConcretePartUnit.h"
#include "Steel.h"
#include "Rebar.h"
#include "SteelPart.h"

class CompositeSection{
private:

	SteelPart steel_part_;
	TConcretePart* concrete_part_;

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

public:
	CompositeSection();
	CompositeSection(SteelPart    steel_part,
					 TConcretePart* concrete_part,
					 TGeometry geometry);

	SteelPart get_steel_part()const {return steel_part_;}
	ISection get_I_section()const {return steel_part_.get_I_section();}
	TConcretePart* get_concrete_part()const {return concrete_part_;}
	Steel get_steel_grade()const {return steel_part_.get_I_steel();}
	double get_alfa_b() const {return alfa_b_;}
	double get_alfa_s() const {return alfa_s_;}
	double get_A_red(LengthUnit length_unit=LengthUnit::mm) const {return A_red_/std::pow(static_cast<int>(length_unit),2);}
	double get_I_red(LengthUnit length_unit=LengthUnit::mm) const {return I_red_/std::pow(static_cast<int>(length_unit),4);}
	double get_W_b_red(LengthUnit length_unit=LengthUnit::mm) const {return W_b_red_/std::pow(static_cast<int>(length_unit),3);}
	double get_W_f2_red(LengthUnit length_unit=LengthUnit::mm) const {return W_f2_red_/std::pow(static_cast<int>(length_unit),3);}
	double get_W_f1_red(LengthUnit length_unit=LengthUnit::mm) const {return W_f1_red_/std::pow(static_cast<int>(length_unit),3);}
	double get_Z_b_red(LengthUnit length_unit=LengthUnit::mm) const {return Z_b_red_/static_cast<int>(length_unit);}
	double get_Z_f2_red(LengthUnit length_unit=LengthUnit::mm) const {return Z_red_f2_/static_cast<int>(length_unit);}
	double get_Z_f1_red(LengthUnit length_unit=LengthUnit::mm) const {return Z_red_f1_/static_cast<int>(length_unit);}
	double get_Z_b_st(LengthUnit length_unit=LengthUnit::mm) const {return Z_b_st_/static_cast<int>(length_unit);}
} ;
//---------------------------------------------------------------------------
#endif


