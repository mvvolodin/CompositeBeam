//---------------------------------------------------------------------------

#ifndef CompositeBeamH
#define CompositeBeamH


#include <vector>
#include <ostream>
//---------------------------------------------------------------------------
#include "GeometryUnit.h"
#include "LoadsUnit.h"

#include "Steel.h"
#include "CompositeSection.h"
#include "InternalForces.h"
#include "UnitStud.h"
#include "Rebar.h"
#include "WorkingConditionsFactors.h"
#include "Stresses.h"
#include "Ratios.h"
#include "CorrugatedSheet.h"


class TCompositeBeam{
//---------------------------------------------------------------------------
	enum class NA_Location{
		CONCRETE,
		UPPER_FLANGE,
		WEB,
		NO_SOLUTION
	};
//---------------------------------------------------------------------------
	struct NeutralAxis{
		NA_Location na_location_;
		double x_na_; //���������� �� �������� ����� ������������������� ����� �� ����������� ���
	};

//---------------------------------------------------------------------------
public:
//---------------------------------------------------------------------------
	TCompositeBeam();
	TCompositeBeam(TGeometry 				geometry,
				   TLoads 					loads,
				   CompositeSection         composite_section,
				   Studs 					stud,
				   WorkingConditionsFactors working_conditions_factors);

	void set_default_values();

	void save(std::ostream& ostr) const;
	void load(std::istream& istr);

	void calculate();

//������� ������� � �����

	TGeometry get_geometry()const {return geometry_;}
	TLoads get_loads()const {return loads_;}
	WorkingConditionsFactors get_working_conditions_factors() const {return working_conditions_factors_;}
	CompositeSection get_composite_section()const{return composite_section_;}
	Studs get_studs()const {return studs_;}

//������� ���������� ����

	InternalForcesNamedList get_internal_forces_LC()const{return internal_forces_;}
	std::vector<double> get_CS_coordinates()const {return cs_coordinates_;}

//������� ��������� ����������� ������� ��� ������ � �����

	double get_max_upper_flange_ratio()const;
	double get_max_lower_flange_ratio()const;
	double get_max_upper_flange_ratio_coordinate() const;
	double get_max_lower_flange_ratio_coordinate() const;

	double get_M_I_for_cs_with_max_lower_flange_ratio(LoadUnit load_unit=LoadUnit::N, LengthUnit length_unit=LengthUnit::mm);
	double get_M_II_for_cs_with_max_lower_flange_ratio(LoadUnit load_unit=LoadUnit::N, LengthUnit length_unit=LengthUnit::mm);
	double get_M_total_for_cs_with_max_lower_flange_ratio(LoadUnit load_unit=LoadUnit::N, LengthUnit length_unit=LengthUnit::mm);

	double get_sigma_b_for_cs_with_max_lower_flange_ratio(LoadUnit load_unit=LoadUnit::N, LengthUnit length_unit=LengthUnit::mm);
	double get_sigma_s_for_cs_with_max_lower_flange_ratio(LoadUnit load_unit=LoadUnit::N, LengthUnit length_unit=LengthUnit::mm);

	double get_max_shear_ratio() const {return *std::max_element(shear_ratios_.begin(),shear_ratios_.end());}
	double get_max_stud_ratio();
	double get_max_stud_ratio_coordinate();
	double get_max_S_h(LoadUnit load_unit=LoadUnit::N);


	double get_ratio_rigid_plastic() const {return ratio_rigid_plastic_;}
//---------------------------------------------------------------------------
private:
//---------------------------------------------------------------------------
//���� � ��������� �������

	TGeometry geometry_; //src- �������� ����...dst - ���� � ������������..... aSize,
	TLoads loads_;
	WorkingConditionsFactors working_conditions_factors_;
	CompositeSection composite_section_;
	CompositeSection composite_section2_;
	Studs studs_;
//---------------------------------------------------------------------------
//�������������� ������ ������
//---------------------------------------------------------------------------

	std::vector<StudOnBeam> stud_on_beam_list;
	double shear_force(StudOnBeam stud_on_beam);
	double calculate_sigma_b(double x);
	double calculate_sigma_s(double x);

	double calculate_M_point_load(double x, double P, double x_P);
	double calculate_M_uniform_load(double x, double q, SupportsNumber s_num);
	std::array<double, 4> calculate_R(double q, SupportsNumber s_num);

	double calculate_M_I_stage(double x);
	double calculate_M_R_I_stage(double x);
	double calculate_M_II_stage(double x);
	double calculate_M_total(double x);

	std::vector<double> calculate_M_I_stage();
	std::vector<double> calculate_M_R_I_stage();
	std::vector<double> calculate_M_II_stage();
	std::vector<double> calculate_M_total();


//---------------------------------------------------------------------------
//�������������� ������ ������
//---------------------------------------------------------------------------

//���� � ������������ �������� ���������

	std::vector<double> cs_coordinates_;
	std::vector<double> stud_coordinates_;//���������� ������������ ������
	std::vector<double> S_coordinates_;
	int cs_num_;//���������� ��������� �������.��� ����� ������ ��� ���������!!!!
	int studs_num_;//���������� ���������� ����� ������.��� ����� ������ ��� ���������!!!!

//���� � ������������ �������� ���������� ������ ��� �������� ������

	InternalForcesNamedList internal_forces_;
	InternalForcesNamedList internal_forces_studs_;

//���� � ������������ �������� ����� ����������

	std::vector<Stresses> stresses_list_;
	std::vector<Stresses> stresses_list_studs_;

//���������� ������ �� ��� ����������� �������������� ����� � �������� �����

	std::vector<double> S_h_list_;

//���� � ������������ �������� ����� ������������ �������������

	std::vector<double> ratios_studs_;
	std::vector<Ratios> ratios_cs_list_;
	double ratio_rigid_plastic_; //�� ��� ������� � ������������� ����� - ������������� ���������
	std::vector<double> shear_ratios_;

	double get_max_abs_M_coordinate(Impact impact);//��������� ���������� ������� � ������������ ��������
	double get_max_abs_Q_coordinate(Impact impact);//��������� ���������� ������� � ����������� ���������� �����



//������ ����������� ������

	void calculate_effective_width();
	double calculate_basic_effective_width(double t_sl, double a,  double B, double l);
	double calculate_cantilever_effective_width(double t_slc, double a,  double C, double l);

//������ ���������
	void calc_cs_coordinates();
	std::vector<double> S_coordinates(std::vector<double> studs_coordinates);
	std::vector<double> intr_frcs_coordinates_for_studs_verification(std::vector<double> studs_coordinates_list);

//������ ������

	void calc_inter_forces();
	std::vector<double> internal_forces_for_studs(std::vector<Stresses>& stresses);

 //������ ����������

	Stresses calculate_stresses(double M, CompositeSection& cs);
	std::vector<Stresses> calculate_stresses(std::vector<double>& cs_coordinates, CompositeSection& cs, InternalForcesNamedList& intr_frcs);

 // ������ ������������� ������������� �������

	std::vector<Ratios> calculate_ratios(std::vector<double>& cs_coordinates, InternalForcesNamedList& intr_frcs);
	Ratios calculate_ratios(double M, Stresses stresses);

 // ������ ������������� ������������� ������

	void calculate_studs_ratios();
	void calculate_shear_ratios();

// ������ ������������� ������������� �����-������������ ������

	NeutralAxis calc_neutral_axis();
	double calc_rigid_plastic_moment();
	void calc_ratio_rigid_plastic();

//������� ������� � �������

	std::vector<double> get_M_list(Impact impact);
	double get_M(double cs_coordinate, Impact impact, InternalForcesNamedList& intr_frcs_named_list, std::vector<double> cs_list);
	double get_M_for_studs(double cs_coordinate, Impact impact);
	std::vector<double> get_M(std::vector<double>& cs_coordinates, Impact impact, InternalForcesNamedList& intr_frcs_named_list);
	std::vector<double> get_Q_list(Impact impact);
	double get_max_abs_M(Impact impact);
	double get_max_abs_Q(Impact impact);
	double get_Q(double cs_coordinate, Impact impact);
	double get_stud_ratio_coordinate(double ratio);

//������� ������� � �����������

	Stresses get_stresses(double cs_coordinate);

//�������  ������� � ��

	double get_upper_flange_ratio_coordinate(double uf_ratio)const;
	double get_lower_flange_ratio_coordinate(double lf_ratio)const;

//������ � ���

	void log_stresses();
 };

#endif



