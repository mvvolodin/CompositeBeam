//---------------------------------------------------------------------------

#ifndef uGeneralConcreteSectionH
#define uGeneralConcreteSectionH

#include <string>
#include <Vcl.Grids.hpp>
#include "uRebar.h"
#include "uWord_Automation.h"
#include "Logger.h"

class GeneralConcreteSection{
public:
	GeneralConcreteSection(double const des_height,
						   double const L,
						   double const B_l,
						   double const B_r,
						   double const b_uf,
						   bool const is_end_beam,
						   Rebars const & rebars);
	virtual ~ GeneralConcreteSection();
	double des_height() const; // ��������� ������� �������������� �����
	double des_width() const; // ��������� ������ ������
	Rebars rebars()const;
	double area() const;// ������� ������
	double inertia() const;// ������ ������� ������
	double rebars_area()const;

	virtual std::u16string slab_type()const = 0;//�������� �����
	virtual double h() const = 0; //������ �������������� �����
	virtual double h_n() const = 0; // ���������� �� �������� ����� ������� ����� �������� �� ���������� ����� ��������� �������������� �����
	virtual double C_b() const = 0; // ����� ������� ������
	virtual double SW(double dens) const = 0;

	void fill_grid(TStringGrid* str_grid)const;
	void print(TWord_Automation & report)const;
	void print_input(TWord_Automation & report)const;
	void print_output(TWord_Automation & report)const;
	#ifdef DEBUG_CONC_SECT
	virtual void log()const;
	#endif

protected:
	const double des_height_;

private:
	const double des_width_;
	const Rebars rebars_;
	double calc_b_sl(const double L, const double B_l, const double B_r,
					 const double h_f, const double b_uf, const bool end_beam) const;
	double calc_b(const double h_f, const double a,  const double B, const double l)const;
	double calc_b_c(const double h_f, const double a,  const double C, const double l)const;
};




//---------------------------------------------------------------------------
#endif
