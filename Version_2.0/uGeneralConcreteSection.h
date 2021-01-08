//---------------------------------------------------------------------------

#ifndef uGeneralConcreteSectionH
#define uGeneralConcreteSectionH

#include <string>
#include <Vcl.Grids.hpp>
#include "uRebar.h"
#include "uWord_Automation.h"

class GeneralConcreteSection{
public:
	GeneralConcreteSection(double const h_f,
						   double const L,
						   double const B_l,
						   double const B_r,
						   double const b_uf,
						   bool const is_end_beam,
						   Rebars const & rebars);
	virtual ~ GeneralConcreteSection();
	double h_f() const; // ��������� ������� �������������� �����
	double b_sl() const; // ��������� ������ ������
	Rebars rebars()const;
	double A_b() const;// ������� ������
	double I_b() const;// ������ ������� ������

	virtual std::u16string slab_type()const = 0;//�������� �����
	virtual double h() const = 0; //������ �������������� �����
	virtual double h_n() const = 0; // ���������� �� �������� ����� ������� ����� �������� �� ���������� ����� ��������� �������������� �����
	virtual double C_b() const = 0; // ����� ������� ������
	virtual double SW(double dens) const = 0;

	void fill_grid(TStringGrid* str_grid)const;
//	void fill_grid_SP266(TStringGrid* str_grid)const;
    void print_SP266(TWord_Automation & report)const;
	void print_input(TWord_Automation & report)const;
	void print_output(TWord_Automation & report)const;

protected:
	const double h_f_;

private:
	const double b_sl_;
	const Rebars rebars_;
	double calc_b_sl(const double L, const double B_l, const double B_r,
					 const double h_f, const double b_uf, const bool end_beam) const;
	double calc_b(const double h_f, const double a,  const double B, const double l)const;
	double calc_b_c(const double h_f, const double a,  const double C, const double l)const;
};




//---------------------------------------------------------------------------
#endif
