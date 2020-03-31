//---------------------------------------------------------------------------

#ifndef CompositeBeamH
#define CompositeBeamH


#include <vector>
#include <ostream>
//---------------------------------------------------------------------------
#include "GeometryUnit.h"
#include "LoadsUnit.h"

#include "ISectionInitialDataUnit.h"
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

	enum class NA_Location{
	CONCRETE,
	UPPER_FLANGE,
	WEB,
	NO_SOLUTION
	};

	struct NeutralAxis{
		NA_Location na_location_;
		double x_na_; //���������� �� �������� ����� ������������������� ����� �� ����������� ���
	};

	using StressesList=std::vector<Stresses> ;
	using StressesNamedList=std::map<Impact, StressesList> ;
	using StressesNamedListItem=std::pair <Impact, StressesList> ;
	using StressesNamedListIterator=StressesNamedList::iterator ;

	using RatiosList=std::vector<Ratios> ;
	using RatiosListIterator=RatiosList::iterator;
	using RatiosNamedList=std::map<Impact, RatiosList> ;
	using RatiosNamedListItem=std::pair <Impact, RatiosList> ;
	using RatiosNamedListIterator=RatiosNamedList::iterator ;

public:
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

	InternalForcesNamedList get_internal_forces_LC()const{return internal_forces_;}
	std::vector<double> get_CS_coordinates()const {return cs_coordinates_;}

	TGeometry get_geometry()const {return geometry_;}
	TLoads get_loads()const {return loads_;}
	WorkingConditionsFactors get_working_conditions_factors() const {return working_conditions_factors_;}
	CompositeSection get_composite_section()const{return composite_section_;}
	Studs get_studs()const {return studs_;}

	double get_max_upper_flange_ratio();
	double get_max_lower_flange_ratio();
	double get_max_stud_ratio();
	double get_ratio_rigid_plastic() const {return ratio_rigid_plastic_;}
	double get_max_shear_ratio() const {return *std::max_element(shear_ratios_.begin(),shear_ratios_.end());}

private:
//���� � ��������� �������
	TGeometry geometry_; //src- �������� ����...dst - ���� � ������������..... aSize,
	TLoads loads_;
	WorkingConditionsFactors working_conditions_factors_;
	CompositeSection composite_section_;
	Studs studs_;

//���� � ������������ �������� ���������
	std::vector<double> cs_coordinates_;
	std::vector<double> stud_coordinates_;//���������� ������������ ������
	int cs_num_;//���������� ��������� �������.��� ����� ������ ��� ���������!!!!
	int studs_num_;//���������� ���������� ����� ������.��� ����� ������ ��� ���������!!!!

//���� � ������������ �������� ���������� ������
	InternalForces if_SW_;
	InternalForces if_sheets_;
	InternalForces if_DL_I_;
	InternalForces if_DL_II_;
	InternalForces if_LL_;

	InternalForces if_stage_I_;
	InternalForces if_stage_II_;
	InternalForces if_stage_total_;

//���� � ������������ �������� ���������� ������ ��� �������� ������

	InternalForcesNamedList internal_forces_studs_;
	InternalForcesNamedList internal_forces_;

//���� � ������������ �������� ����� ����������
	StressesNamedList stresses_named_list_; //
	StressesNamedList stresses_named_list_studs_;
	std::vector<double> S_; //���������� ������ �� ��� ����������� �������������� ����� � �������� �����

//���� � ������������ �������� ����� ������������ �������������

	RatiosNamedList ratios_named_list_;//std::map<Impact, StressesList> StressesNamedList//std::vector<Stresses> StressesList
	std::vector<double> ratios_studs_;
	double ratio_rigid_plastic_; //�� ��� ������� � ������������� ����� - ������������� ���������
	std::vector<double> shear_ratios_;

	double get_max_abs_M_coordinate(Impact impact);//��������� ���������� ������� � ������������ ��������
	double get_max_abs_Q_coordinate(Impact impact);//��������� ���������� ������� � ����������� ���������� �����

//�������

	void calculate_effective_width();
	double calculate_basic_effective_width(double t_sl, double a,  double B, double l);
	double calculate_cantilever_effective_width(double t_slc, double a,  double C, double l);
	void calc_cs_coordinates(); //����������� ��������� ������� ��� ����������� ������ ��������� ��� �������� �����
	void calc_studs_coordinates(); // ����������� ��������� ������� ��� ����������� ������ ��������� ��� ������
	void calc_inter_forces_for_studs();
	void calc_inter_forces();
	void calc_ratios();
	void calc_studs_ratios();
	NeutralAxis calc_neutral_axis();
	double calc_rigid_plastic_moment();
	void calc_ratio_rigid_plastic();
	void calc_shear_ratios();

 //������ ����������

	void calculate_stresses(Impact impact);

 // ������ ������������� �������������

	Ratios calculate_I_case(Impact impact, int cs_id);
	Ratios calculate_II_case(Impact impact, int cs_id);
	Ratios calculate_III_case(Impact impact, int cs_id);

//������� ������� � �������

	std::vector<double> get_M_list(Impact impact);
	std::vector<double> get_Q_list(Impact impact);
	double get_max_abs_M(Impact impact);
	double get_max_abs_Q(Impact impact);
	double get_M(double cs_coordinate, Impact impact);
	double get_Q(double cs_coordinate, Impact impact);

//������� ������� � �����������
	Stresses get_stresses(double cs_coordinate, Impact impact);

//������ � ���
	void log_stresses();
 };

#endif



