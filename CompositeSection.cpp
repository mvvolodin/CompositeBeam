// ---------------------------------------------------------------------------

#pragma hdrstop

#include "CompositeSection.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

#include "MathFunction.h"
#include<cmath>
ISection CompositeSection::steel_part___; //�� �����������
TConcretePart* CompositeSection::concrete_part___;  //�� ������������
Steel CompositeSection::i_sect_steel___; //�� �����������

CompositeSection::CompositeSection()
{

}
CompositeSection::CompositeSection(SteelPart    steel_part,
								   TConcretePart* concrete_part)
{

}

CompositeSection::CompositeSection(TGeometry geometry,
									  Steel steel_i_section,
									  TISectionInitialData i_sect_initial_data,
									  TConcretePart* concrete_part)
	:i_sect_steel(steel_i_section),concrete_part(concrete_part)
{

	steel_part=ISection(i_sect_initial_data);


	if (!geometry.is_end_beam())
	{
		double t_sl= concrete_part->get_t_sl();
		double a=steel_part.get_b_uf()/2;
		double B_l=geometry.get_trib_width_left();
		double B_r=geometry.get_trib_width_right();
		double l=geometry.get_span();
		concrete_part->set_b_l(concrete_part->effective_width_calc(t_sl,a,B_l,l));
		concrete_part->set_b_r(concrete_part->effective_width_calc(t_sl,a,B_r,l));
	}
	else
	{
		double t_sl= concrete_part->get_t_sl();
		double a=steel_part.get_b_uf()/2;
		double B_l=geometry.get_trib_width_left();
		double B_r=geometry.get_trib_width_right();
		double l=geometry.get_span();
		concrete_part->set_b_l(concrete_part->effective_width_cantilever_calc(t_sl,a,B_l,l));
		concrete_part->set_b_r(concrete_part->effective_width_calc(t_sl,a,B_r,l));

	}

	concrete_part->calc_area(); //���������� ����������� ������
	concrete_part->calc_inertia();//����������� ����������� ������

	alfa_to_rebar_steel_calc();
	alfa_to_concrete_calc();
	compos_sect_height_calc();
	compos_sect_area_calc();
	dist_conc_to_steel_calc();
	first_moment_of_area_comp_calc();
	dist_steel_to_compos_calc();
	inertia_compos_calc();
	dist_compos_to_upper_fl_calc();
	dist_compos_to_lower_fl_calc();
	dist_conc_to_compos_calc();
	sect_modulus_upper_fl();
	sect_modulus_lower_fl();
	sect_modulus_conc();
}

void CompositeSection::alfa_to_rebar_steel_calc()
{
	double E_st_ = i_sect_steel.get_E_s(); // ������ ��������� �����
	double E_s_ = concrete_part->get_rebar().get_E_s(); // ������ ��������� ��������
	Alfa_s_ = E_st_ / E_s_;
}

void CompositeSection::alfa_to_concrete_calc()
{
	double E_st_=i_sect_steel.get_E_s(); // ������ ��������� �����
	double E_b_=concrete_part->get_E_b();//������ ��������� ������
	Alfa_b_=E_st_/E_b_;
}

void CompositeSection::compos_sect_height_calc()
{
	double h_p = concrete_part->get_h_b(); //������� �������������� �����. ��� ������ ����� �� ������� ����� ������ ������� � ������� ������ ��� ���
	double h_st = steel_part.get_h_st(); // ������ �������� �����
	H_stb_ = h_p + h_st;
}

void CompositeSection::compos_sect_area_calc()
{
	double A_b=concrete_part->get_A_b(); // A_b (concrete_part->get_area()) ���������������?
	double A_st=steel_part.get_A_st();
	A_red_= A_st + A_b/ Alfa_b_;
}

void CompositeSection::dist_conc_to_steel_calc()
{
	double C_b_ =100;// concrete_part->get_C_b(); // ���������� �� �������� ����� ����� �� �.�. �����
	double Z_f2_st=steel_part.get_Z_f2_st();//���������� �� �.�. �� �������� ����� ����.�����
	Z_b_st_=C_b_+Z_f2_st;
}

void CompositeSection::first_moment_of_area_comp_calc()
{
	double A_b = concrete_part->get_A_b();
	S_stb_= A_b*Z_b_st_/Alfa_b_;
}

void CompositeSection::dist_steel_to_compos_calc()
{
	Z_st_red_=S_stb_/A_red_;
}

void CompositeSection::inertia_compos_calc()
{
	double I_st=steel_part.get_I_st();
	double A_st = steel_part.get_A_st();
	double A_b=concrete_part->get_A_b();
	double I_b=concrete_part->get_I_b();
	I_red_=I_st+A_st*pow(Z_st_red_,2)+1/Alfa_b_*I_b+1/Alfa_b_*A_b*pow((Z_b_st_-Z_st_red_),2);
}

void CompositeSection::dist_compos_to_upper_fl_calc()
{
	double Z_f2_st=steel_part.get_Z_f2_st();
	Z_red_f2_=Z_f2_st-Z_st_red_;
}

void CompositeSection::dist_compos_to_lower_fl_calc()
{
	double Z_f1_st=steel_part.get_Z_f1_st();
	Z_red_f1_=Z_f1_st+Z_st_red_;
}

void CompositeSection::dist_conc_to_compos_calc()
{
	Z_b_red_=this->Z_b_st_-Z_st_red_;
}

void CompositeSection::sect_modulus_upper_fl()
{
	W_f2_red_=I_red_/Z_red_f2_;
}

void CompositeSection::sect_modulus_lower_fl()
{
	W_f1_red_=I_red_/Z_red_f1_;
}

void CompositeSection::sect_modulus_conc()
{
	W_b_red_=I_red_/Z_b_red_;
}
