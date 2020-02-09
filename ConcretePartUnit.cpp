
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
TConcretePart::TConcretePart(String slab_type, Concrete concrete, Rebar rebar)
	:slab_type_(slab_type),concrete_(concrete),rebar_(rebar)
{
}
TConcretePart::TConcretePart(Concrete concrete, Rebar rebar, double t_sl)
	:concrete_(concrete), rebar_(rebar), t_sl_(t_sl),slab_type_(L"������� �����")

{
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
TFlatSlab::TFlatSlab(Concrete concrete, Rebar rebar, double t_sl)
	:TConcretePart(concrete, rebar, t_sl)
{
}

void TFlatSlab::calc_area()
{
	 A_b_=(b_l_+b_r_)*t_sl_;
}

void TFlatSlab::calc_inertia()
{
	 I_b_=(b_l_+b_r_)*std::pow(t_sl_,3)/12;
}

//---------------------------------------------------------------------------

TCorrugatedSlab::TCorrugatedSlab(String slab_type, Concrete concrete, Rebar rebar, double h_f) //h_f ������ ������ ��� ��������
	:TConcretePart(slab_type, concrete, rebar),h_f_(h_f)
{
   h_p_=corrugated_sheets_.find(slab_type)->second;
}

void TCorrugatedSlab::calc_area()
{
}

void TCorrugatedSlab::calc_inertia()
{
}

