//---------------------------------------------------------------------------

#ifndef uSlabConcreteSectionH
#define uSlabConcreteSectionH

#include "uGeneralConcreteSection.h"
#include "uRebar.h"

class SlabConcreteSection: public GeneralConcreteSection{
public:
	SlabConcreteSection(double h_f,
						double h_n,
                        double b_sl,
						Rebar& rebar,
						double b,
						double a_u,
						double a_l);
	double h_f() const override; // ��������� ������� �������������� �����
	double h_n() const override; // ���������� �� �������� ����� ������� ����� �������� �� ���������� ����� ��������� �������������� �����
	double h() const override;
	double b_sl() const override; // ��������� ������ ������
	const Rebar& rebar()const override;
	double a_u() const override;// ���������� �� ������� ����� ��������� �������������� ����� �� ����� ��������
	double a_l() const override;// ���������� �� ������ ����� ��������� �������������� ����� �� ������ ��������
	double C_b() const override; // ����� ������� ������
	double A_b() const override;// ������� ������
	double I_b() const override;// ������ ������� ������

private:
	const double h_f_;
	const double h_n_;
	const double b_sl_;

	const Rebar rebar_;
	const double b_;
	const double a_u_;
	const double a_l_;

};
//---------------------------------------------------------------------------
#endif
