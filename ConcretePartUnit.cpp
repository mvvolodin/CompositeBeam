
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
TConcretePart::TConcretePart(Concrete concrete, Rebar rebar)
	:concrete_(concrete),rebar_(rebar)
{
}
TConcretePart::TConcretePart(Concrete concrete, Rebar rebar, double t_sl)
	:concrete_(concrete), rebar_(rebar), t_sl_(t_sl)

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
	if (l>=4*b)
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
	 I_b_=(b_l_+b_r_)*std::pow(t_sl_,3);
}

//---------------------------------------------------------------------------

TCorrugatedSlab::TCorrugatedSlab(Concrete concrete, Rebar rebar)
	:TConcretePart(concrete, rebar)
{
}

void TCorrugatedSlab::calc_area()
{
}

void TCorrugatedSlab::calc_inertia()
{
}

