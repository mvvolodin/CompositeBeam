// ---------------------------------------------------------------------------

#ifndef ConcretePartUnitH
#define ConcretePartUnitH
#include "GeometryUnit.h"
#include "Concrete.h"
#include "Rebar.h"
#include "Units.h"

// ---------------------------------------------------------------------------

class TConcretePart {

private:
	String slab_type_;
	Concrete concrete_;
	Rebar rebar_;

protected:
	// ������ ���� ������ ���� ����� � ������� ��������, ������� ������������ ��������� protected

	double b_l_; // ����������� ������ �����
	double b_r_; // ����������� ������ ������
	double h_b_;// ������� �������������� �����. ��� ������ ����� �� ������� ����� ������ ������� � ������� ������ ��� ���
	double C_b_;
	// ���������� �� ������ ����� �������������� ����� �� � ����� �������
	double t_sl_; // ��������� ������� �������������� �����
	double A_b_; // ������� ������������
	double I_b_;
	// ������ ������� �������������� ����� //!����! ���� ������ ��� ���������. �������� ���. �������, ������ ���� � ������. �� ���������� ������ ������������

public:
	double effective_width_calc(double t_sl, double a, double B, double l);
	double effective_width_cantilever_calc(double t_slc, double a, double �,
		double l);
	virtual void calc_h_b() = 0;//������ ����� (��� ����������� � ����� ������ ����������� � ������� ������ ��� ������������)
	virtual void calc_C_b() = 0; // pure virtual
	virtual void calc_area() = 0; // pure virtual
	virtual void calc_inertia() = 0; // pure virtual

	// !����! ����������� ���������� ��������.

	TConcretePart();
	TConcretePart(String slab_type, Concrete concrete, Rebar rebar);
	TConcretePart(Concrete concrete, Rebar rebar, double t_sl);


	inline String get_slab_type() const {return slab_type_;}
	inline Concrete get_concrete() const {return concrete_;}
	inline double get_R_bn() const {return concrete_.get_R_bn();}
	inline double get_R_btn() const {return concrete_.get_R_btn();}
	inline double get_E_b() const {return concrete_.get_E_b();}
	inline Rebar get_rebar() const {return rebar_;}
	inline double get_C_b(LengthUnit length_unit=LengthUnit::mm) const {return C_b_/static_cast<int>(length_unit);}
	inline void set_b_l(double b_l) {b_l_ = b_l;}
	inline void set_b_r(double b_r) {b_r_ = b_r;}
	inline double get_h_b(LengthUnit length_unit=LengthUnit::mm) const {return h_b_/static_cast<int>(length_unit);}
	inline double get_t_sl(LengthUnit length_unit=LengthUnit::mm) const {return t_sl_/static_cast<int>(length_unit);}
	inline double get_b_l(LengthUnit length_unit=LengthUnit::mm) const {return b_l_/static_cast<int>(length_unit);} //����� ���������� b_sl � ���� ������� ��� �������������. ������� ����� ��������� ���� � ��������
	inline double get_b_r(LengthUnit length_unit=LengthUnit::mm) const {return b_r_/static_cast<int>(length_unit);} //����� ���������� b_sl � ���� ������� ��� �������������. ������� ����� ��������� ���� � ��������
	inline double get_b_sl(LengthUnit length_unit=LengthUnit::mm) const {return (b_r_+ b_l_)/static_cast<int>(length_unit);}
	inline double get_A_b(LengthUnit length_unit=LengthUnit::mm) const {return A_b_/std::pow(static_cast<int>(length_unit),2);}
	inline double get_I_b(LengthUnit length_unit=LengthUnit::mm) const {return I_b_/std::pow(static_cast<int>(length_unit),4);}
};

class TFlatSlab : public TConcretePart {

private:
	virtual void calc_area() override;
	virtual void calc_inertia() override;
	virtual void calc_h_b() override;
	virtual void calc_C_b() override;

public:
	TFlatSlab(Concrete concrete, Rebar rebar, double t_sl);

};

class TCorrugatedSlab : public TConcretePart{

private:
	typedef std::map<String, int> CorrugatedSheetsNamedList;

	CorrugatedSheetsNamedList corrugated_sheets_={{L"�60-845",60},
												 {L"�75-750",75},
												 {L"�114-750",114},
												 {L"�153-850",153},
												 {L"���200",200}};
	int h_p_; // ������ �������
	double h_f_; // ������� ������������ ��� ��������

	double h_b_;//����������
	virtual void calc_h_b() override;
	virtual void calc_C_b() override;
	virtual void calc_area() override;
	virtual void calc_inertia() override;

public:
	TCorrugatedSlab(String slab_type, Concrete concrete, Rebar rebar, double h_f);
};

#endif
