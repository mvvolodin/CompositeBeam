//---------------------------------------------------------------------------

#ifndef uGeneralConcreteSectionH
#define uGeneralConcreteSectionH

#include "uRebar.h"


class GeneralConcreteSection{
public:
	GeneralConcreteSection(const double h_f,
						   const Rebars& rebars,
						   const double b,
						   const double a_u,
						   const double a_l,
						   const double L,
						   const double B_l,
						   const double B_r,
						   const double b_uf,
						   const bool end_beam);
	virtual ~ GeneralConcreteSection();
	double h_f() const; // ��������� ������� �������������� �����
	double b_sl() const; // ��������� ������ ������
	Rebars rebars()const;
	double a_u() const;// ���������� �� ������� ����� ��������� �������������� ����� �� ����� ��������
	double a_l() const;// ���������� �� ������ ����� ��������� �������������� ����� �� ������ ��������
	double num_rebar_rows() const;
	double A_b() const;// ������� ������
	double I_b() const;// ������ ������� ������

	virtual double h() const = 0; //������ �������������� �����
	virtual double h_n() const = 0; // ���������� �� �������� ����� ������� ����� �������� �� ���������� ����� ��������� �������������� �����
	virtual double C_b() const = 0; // ����� ������� ������

protected:
	const double h_f_;

private:
	const double b_sl_;
	const Rebars rebars_;
	const double b_;
	const double a_u_;
	const double a_l_;
	double calc_b_sl(const double L, const double B_l, const double B_r,
					 const double h_f, const double b_uf, const bool end_beam) const;
	double calc_b(const double h_f, const double a,  const double B, const double l)const;
	double calc_b_c(const double h_f, const double a,  const double C, const double l)const;
};




//---------------------------------------------------------------------------
#endif
