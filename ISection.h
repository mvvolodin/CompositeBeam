//---------------------------------------------------------------------------

#ifndef ISectionH
#define ISectionH
//---------------------------------------------------------------------------
#include <math.h>
#include <ostream>
#include <istream>
//---------------------------------------------------------------------------
#include "Units.h"
#include "System.hpp"

enum class ProfileGroup{
	B,
	SH,
	K,
	C,
	DB,
	DK,
};

class ISection{

public:
	ISection();
	ISection(String  profile_number,
			ProfileGroup profile_group,
			double b_uf,
			double t_uf,
			double b_lf,
			double t_lf,
			double h_w,
			double t_w,
			double h_st,
			double r,
			double weight,
			double Z_f2_st,
			double Z_f1_st,
			double A_st,
			double I_st,
			double Wf2_st,
			double Wf1_st);
	void save(std::ostream& ostr)const;
	void load(std::istream& istr);
	void set_default_values();
	String get_profile_number()const {return profile_number_;}
	ProfileGroup get_profile_group()const {return profile_group_;}
	double get_b_uf(LengthUnit length_unit=LengthUnit::mm) const {return b_uf_/static_cast<int>(length_unit);}
	double get_t_uf(LengthUnit length_unit=LengthUnit::mm) const {return t_uf_/static_cast<int>(length_unit);}
	double get_b_lf(LengthUnit length_unit=LengthUnit::mm) const {return b_lf_/static_cast<int>(length_unit);}
	double get_t_lf(LengthUnit length_unit=LengthUnit::mm) const {return t_lf_/static_cast<int>(length_unit);}
	double get_h_w(LengthUnit length_unit=LengthUnit::mm) const {return h_w_/static_cast<int>(length_unit);}
	double get_t_w(LengthUnit length_unit=LengthUnit::mm) const {return t_w_/static_cast<int>(length_unit);}
	double get_h_st(LengthUnit length_unit=LengthUnit::mm) const {return h_st_/static_cast<int>(length_unit);}
	double get_r(LengthUnit length_unit=LengthUnit::mm) const {return r_/static_cast<int>(length_unit);}
	double get_weight(LengthUnit length_unit=LengthUnit::mm, LoadUnit load_unit=LoadUnit::N) const {return weight_/static_cast<int>(load_unit)*static_cast<int>(length_unit);}
	double get_Z_f2_st(LengthUnit length_unit=LengthUnit::mm) const {return Z_f2_st_/static_cast<int>(length_unit);}
	double get_Z_f1_st(LengthUnit length_unit=LengthUnit::mm) const {return Z_f1_st_/static_cast<int>(length_unit);}
	double get_A_st(LengthUnit length_unit=LengthUnit::mm) const {return A_st_/std::pow(static_cast<int>(length_unit),2);}
	double get_I_st(LengthUnit length_unit=LengthUnit::mm) const {return I_st_/std::pow(static_cast<int>(length_unit),4);}
	double get_Wf2_st(LengthUnit length_unit=LengthUnit::mm) const {return Wf2_st_/std::pow(static_cast<int>(length_unit),3);}
	double get_Wf1_st(LengthUnit length_unit=LengthUnit::mm) const {return Wf1_st_/std::pow(static_cast<int>(length_unit),3);}
	double get_A_f1_st(LengthUnit length_unit=LengthUnit::mm) const{return b_lf_*t_lf_/std::pow(static_cast<int>(length_unit),2);}
	double get_A_f2_st(LengthUnit length_unit=LengthUnit::mm) const{return b_uf_*t_uf_/std::pow(static_cast<int>(length_unit),2);}
	double get_A_w_st(LengthUnit length_unit=LengthUnit::mm) const{return t_w_*h_w_/std::pow(static_cast<int>(length_unit),2);}

private:
	String profile_number_;
	ProfileGroup profile_group_;
	double b_uf_ = 0.;
	double t_uf_ = 0.;
	double b_lf_ = 0.;
	double t_lf_ = 0.;
	double h_w_ = 0.;
	double t_w_ = 0.;
	double h_st_ = 0.;//������ �����
	double r_ = 0.;//������
	double weight_ = 0.;//�������� ��� �����
	double Z_f2_st_ = 0.;//���������� �� �.�. ��������� ������� �� �������� ����� ����.�����
	double Z_f1_st_ = 0.; //���������� �� �.�. ��������� ������� �� �������� ����� ������ �����
	double A_st_ = 0.;
	double I_st_ = 0.;
	double Wf2_st_ = 0.;//������ ������������� ������� ������� ������� �����
	double Wf1_st_ = 0.;//������ ������������� ������� ������� ������ �����

};
#endif
