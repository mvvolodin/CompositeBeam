//---------------------------------------------------------------------------

#ifndef ISectionH
#define ISectionH
//---------------------------------------------------------------------------

#include <math.h>
#include "ISectionInitialDataUnit.h"
#include "Units.h"

class ISection{
private:
	String profile_number_;
	double b_uf_;
	double t_uf_;
	double b_lf_;
	double t_lf_;
	double h_w_;
	double t_w_;
	double h_st_;//������ �����
	double r_;//������
    double weight_;//�������� ��� �����
	double Z_f2_st_;//���������� �� �.�. ��������� ������� �� �������� ����� ����.�����
	double Z_f1_st_; //���������� �� �.�. ��������� ������� �� �������� ����� ������ �����
	double A_st_;
	double I_st_;
	double Wf2_st_;//������ ������������� ������� ������� ������� �����
	double Wf1_st_;//������ ������������� ������� ������� ������ �����

public:
	ISection();
	ISection(TISectionInitialData i_section_initial_data);
	inline String get_profile_number() const {return profile_number_;}
	inline double get_b_uf(LengthUnit length_unit=LengthUnit::mm) const {return b_uf_/static_cast<int>(length_unit);}
	inline double get_t_uf(LengthUnit length_unit=LengthUnit::mm) const {return t_uf_/static_cast<int>(length_unit);}
	inline double get_b_lf(LengthUnit length_unit=LengthUnit::mm) const {return b_lf_/static_cast<int>(length_unit);}
	inline double get_t_lf(LengthUnit length_unit=LengthUnit::mm) const {return t_lf_/static_cast<int>(length_unit);}
	inline double get_h_w(LengthUnit length_unit=LengthUnit::mm) const {return h_w_/static_cast<int>(length_unit);}
	inline double get_t_w(LengthUnit length_unit=LengthUnit::mm) const {return t_w_/static_cast<int>(length_unit);}
	inline double get_h_st(LengthUnit length_unit=LengthUnit::mm) const {return h_st_/static_cast<int>(length_unit);}
	inline double get_r(LengthUnit length_unit=LengthUnit::mm) const {return r_/static_cast<int>(length_unit);}
	inline double get_weight(LengthUnit length_unit=LengthUnit::mm, LoadUnit load_unit=LoadUnit::N) const {return weight_/static_cast<int>(load_unit)*static_cast<int>(length_unit);}
	inline double get_Z_f2_st(LengthUnit length_unit=LengthUnit::mm) const {return Z_f2_st_/static_cast<int>(length_unit);}
	inline double get_Z_f1_st(LengthUnit length_unit=LengthUnit::mm) const {return Z_f1_st_/static_cast<int>(length_unit);}
	inline double get_A_st(LengthUnit length_unit=LengthUnit::mm) const {return A_st_/std::pow(static_cast<int>(length_unit),2);}
	inline double get_I_st(LengthUnit length_unit=LengthUnit::mm) const {return I_st_/std::pow(static_cast<int>(length_unit),4);}
	inline double get_Wf2_st(LengthUnit length_unit=LengthUnit::mm) const {return Wf2_st_/std::pow(static_cast<int>(length_unit),3);}
	inline double get_Wf1_st(LengthUnit length_unit=LengthUnit::mm) const {return Wf1_st_/std::pow(static_cast<int>(length_unit),3);}
	inline  double get_A_f2_st(LengthUnit length_unit=LengthUnit::mm) const{return b_uf_*t_uf_/std::pow(static_cast<int>(length_unit),2);}
};
#endif
