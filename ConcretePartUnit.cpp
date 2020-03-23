
    //---------------------------------------------------------------------------

    #pragma hdrstop

	#include<cmath>
	#include "ConcretePartUnit.h"
	#include "MathFunction.h"
	//---------------------------------------------------------------------------
	#pragma package(smart_init)

//---------------------------------------------------------------------------
TConcretePart::TConcretePart()
{
}
TConcretePart::TConcretePart(String slab_type, SlabType slab_type_enum, Concrete concrete, Rebar rebar, double h_f, TGeometry geometry, double b_uf)
	:slab_type_(slab_type), slab_type_enum_(slab_type_enum),concrete_(concrete),rebar_(rebar), h_f_(h_f)
{
		double a=b_uf/2;
		double B_l=geometry.get_trib_width_left();
		double B_r=geometry.get_trib_width_right();
		double l=geometry.get_span();

	if (!geometry.is_end_beam())
	{
		b_l_ = effective_width_calc(h_f_,a,B_l,l);
		b_r_ = effective_width_calc(h_f_,a,B_r,l);
	}
	else
	{
		b_l_ = effective_width_cantilever_calc(h_f_,a,B_l,l);
		b_r_ = effective_width_calc(h_f_,a,B_r,l);
	}
}
//-----------------------------------------------------------------------------
//���������� ������ ������ �������� �� ���������
//-----------------------------------------------------------------------------
void TConcretePart::set_default_values()
{
	slab_type_ = L"������� �����";
	slab_type_enum_ = SlabType::FLAT;
    h_f_ = 200;
	concrete_.set_default_values();
	rebar_.set_default_values();
}
//---------------------------------------------------------------------------
//��������� ������ ............. � �������� ����
//---------------------------------------------------------------------------
void TConcretePart::save(std::ostream& ostr) const
{
//	geometry_.save_geometry(ostr);
//	loads_.save_loads(ostr);
//	working_conditions_factors_.save_working_conditions_factors(ostr);
////	composite_section_.save_composite_section(ostr);
//	studs_.save_studs(ostr);

}
//---------------------------------------------------------------------------
//��������� ������ ........... �� ��������� �����
//---------------------------------------------------------------------------
void TConcretePart::load(std::istream& istr)
{

//	geometry_.load_geometry(istr);
//	loads_.load_loads(istr);
//	working_conditions_factors_.load_working_conditions_factors(istr);
////	composite_section_.load_composite_section(istr);
//	studs_.load_studs(istr);

}

//---------------------------------------------------------------------------
//����������� ���������� ����� �����
//������������ ��������� (� ������� �������������� � �������):
//  *������� �����,
//  *�������� ������ ��������  �.� ����� � ��������� �����
//	*���������� ����� ����� �����
//	*����� �����
//---------------------------------------------------------------------------
double TConcretePart::effective_width_calc(double t_sl, double a,  double B, double l)
{
	double b=0;
	if (l>=4*B)
		b=B/2;
	else
		b=a+6*t_sl;
	return clamp(b, l/8, B/2 );
}
//---------------------------------------------------------------------------
//����������� ���������� ����� ����� � ������ �������
//������������ ��������� (� ������� �������������� � �������):
//  *������� ����� �������,
//  *�������� ������ ��������  �.� ����� � ��������� �����
//	*����� ����������� ����� �����
//	*����� �����
//---------------------------------------------------------------------------
double TConcretePart::effective_width_cantilever_calc(double t_slc, double a,  double C, double l)
{
	double bc=0;
	if (l>=12*C)
		bc=C;
	else
		bc=a+6*t_slc;
	return clamp(bc, l/12, C );
}
TFlatSlab::TFlatSlab(Concrete concrete, Rebar rebar, double h_f, TGeometry geometry, double b_uf)
	:TConcretePart(L"������� �����", SlabType::FLAT, concrete, rebar, h_f, geometry, b_uf)
{
	h_b_=h_f;
	C_b_=h_b_/2.;
	A_b_=(b_l_+b_r_)*h_f;
	I_b_=(b_l_+b_r_)*std::pow(h_f,3)/12;
}

//---------------------------------------------------------------------------

TCorrugatedSlab::TCorrugatedSlab(String slab_type, Concrete concrete, Rebar rebar, double h_f, TGeometry geometry, double b_uf)
	:TConcretePart(slab_type, SlabType::CORRUGATED, concrete, rebar, h_f, geometry, b_uf)
{
   corrugated_sheet_ = corrugated_sheets_map[slab_type];
   h_b_=corrugated_sheet_.get_height()+h_f;
   C_b_=corrugated_sheet_.get_height()+h_f/2.;
   A_b_=(b_l_+b_r_)*h_f;
   I_b_=(b_l_+b_r_)*std::pow(h_f,3)/12;
}

