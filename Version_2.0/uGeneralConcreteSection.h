//---------------------------------------------------------------------------

#ifndef uGeneralConcreteSectionH
#define uGeneralConcreteSectionH

#include "uRebar.h"

class GeneralConcreteSection{
public:
	virtual ~ GeneralConcreteSection();
	virtual double h_f() const  = 0; // ��������� ������� �������������� �����
	virtual double h_n() const = 0; // ���������� �� �������� ����� ������� ����� �������� �� ���������� ����� ��������� �������������� �����
	virtual double h_b() const = 0;
	virtual double b_sl() const = 0; // ��������� ������ ������
	virtual const Rebar& rebar()const = 0;
	virtual double a_u() const = 0;// ���������� �� ������� ����� ��������� �������������� ����� �� ����� ��������
	virtual double a_l() const = 0;// ���������� �� ������ ����� ��������� �������������� ����� �� ������ ��������
	virtual double num_rebar_rows() const = 0;
	virtual double C_b() const = 0; // ����� ������� ������
	virtual double A_b() const = 0;// ������� ������
	virtual double I_b() const = 0;// ������ ������� ������
};




//---------------------------------------------------------------------------
#endif
