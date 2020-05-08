// ---------------------------------------------------------------------------
#ifndef ConcretePartUnitH
#define ConcretePartUnitH
// ---------------------------------------------------------------------------
#include <ostream>
#include <istream>
// ---------------------------------------------------------------------------
#include "uGeometry.h"
#include "uConcrete.h"
#include "uRebar.h"
#include "uCorrugatedSheet.h"
#include "Units.h"
// ---------------------------------------------------------------------------
enum class SlabType{
	FLAT,
	CORRUGATED,
};
// ---------------------------------------------------------------------------
class TConcretePart {

public:

	TConcretePart();
	TConcretePart(String slab_type, SlabType slab_type_enum, Concrete concrete, Rebar rebar, double h_f, double h_n);
	void save(std::ostream& ostr) const;
	void load(std::istream& istr);
	void set_default_values();
	bool get_status()const{return concrete_part_calculated_;}

	void calculate();
	double calculate_SW_concrete();

	String get_slab_type() const {return slab_type_;}
	SlabType get_slab_type_enum() const {return slab_type_enum_;}
	Concrete& get_concrete() {return concrete_;}
	const Concrete& get_concrete()const {return concrete_;}
	Rebar get_rebar() const {return rebar_;}
	double get_SW_concrete(LoadUnit load_unit=LoadUnit::N, LengthUnit length_unit=LengthUnit::mm) const
		{return SW_concrete_/static_cast<int>(load_unit)*std::pow(static_cast<int>(length_unit),2);}
	double get_C_b(LengthUnit length_unit=LengthUnit::mm) const {return C_b_/static_cast<int>(length_unit);}
	double get_h(LengthUnit length_unit=LengthUnit::mm) const {return h_/static_cast<int>(length_unit);}
	double get_h_f(LengthUnit length_unit=LengthUnit::mm) const {return h_f_/static_cast<int>(length_unit);}
	double get_h_n(LengthUnit length_unit=LengthUnit::mm) const {return h_n_/static_cast<int>(length_unit);}
	void set_b(double b){b_ = b;}
	void set_phi_b_cr(double phi_b_cr){concrete_.set_phi_b_cr(phi_b_cr);}
	double get_b(LengthUnit length_unit=LengthUnit::mm) const {return b_/static_cast<int>(length_unit);}
	double get_A_b(LengthUnit length_unit=LengthUnit::mm) const {return A_b_/std::pow(static_cast<int>(length_unit),2);}
	double get_I_b(LengthUnit length_unit=LengthUnit::mm) const {return I_b_/std::pow(static_cast<int>(length_unit),4);}

private:

	String slab_type_;
	SlabType slab_type_enum_;
	Concrete concrete_;
	Rebar rebar_;
	double h_f_ = 0.; // ��������� ������� �������������� �����
	double h_n_ = 0.; // ���������� �� �������� ����� ������� ����� �������� �� ���������� ����� ��������� �������������� �����

	double SW_concrete_ = 0.;//����������� ��� �������������� �����
	double b_ = 0.; // ����������� ������
	double h_ = 0.;// ������ �������������� �����
	double C_b_ = 0.; 	// ���������� �� �������� ����� ������� ����� �������� �� ����� ������� ��������� �������������� �����
   	double h_b_ = 0.; //���������� ������� ������
	double A_b_ = 0.; // ������� ��������� �������������� �����
	double I_b_ = 0.; //������ ������� ��������� �������������� �����

	double effective_width_calc(double t_sl, double a, double B, double l);
	double effective_width_cantilever_calc(double t_slc, double a, double �, double l);

	bool concrete_part_calculated_ = false;
};


#endif
