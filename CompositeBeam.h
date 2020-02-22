//---------------------------------------------------------------------------

#ifndef CompositeBeamH
#define CompositeBeamH


#include <vector>
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

//---------------------------------------------------------------------------
enum class AnalysisTheory{  //�������� ��������
	ELASTO_PLASTCIC,
	RIGID
};

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
	using RatiosListIterator=RatiosList::iterator ;
	using RatiosNamedList=std::map<Impact, RatiosList> ;
	using RatiosNamedListItem=std::pair <Impact, RatiosList> ;
	using RatiosNamedListIterator=RatiosNamedList::iterator ;

public:
	TCompositeBeam();
	TCompositeBeam(AnalysisTheory           analysis_theory,
				   TGeometry 				geometry,
				   TLoads 					loads,
				   CompositeSection         composite_section,
				   Studs 					stud,
				   WorkingConditionsFactors working_conditions_factors);

	inline CompositeSection get_composite_section()const{return composite_section_;}//��������� ������!!!//�� ������������ ������ �� ������
	inline InternalForcesNamedList get_internal_forces_LC()const{return internal_forces_;}
	inline std::vector<double> get_CS_coordinates()const {return cs_coordinates_;}
	inline Studs get_studs()const {return studs_;}
	inline TGeometry get_geometry()const {return geometry_;}
	inline TLoads get_loads()const {return loads_;}
	inline WorkingConditionsFactors get_working_conditions_factors() const {return working_conditions_factors_;}

	String get_analysis_theory();
	double get_max_upper_flange_ratio();
	double get_max_lower_flange_ratio();
	double get_ratio_rigid_plastic() const {return ratio_rigid_plastic_;}

private:
//���� � ��������� �������
	AnalysisTheory analysis_theory_;
	TGeometry geometry_;
	TLoads loads_;
	WorkingConditionsFactors working_conditions_factors_;
	CompositeSection composite_section_;
	Studs studs_;
//���� � ������������ ��������
	std::vector<double> cs_coordinates_;
	std::vector<double> stud_coordinates_;//���������� ������������ ������
	int cs_num_;//���������� ��������� �������.��� ����� ������ ��� ���������!!!!
	int studs_num_;//���������� ���������� ����� ������.��� ����� ������ ��� ���������!!!!
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
	double gamma_1_;//����������� ������� ������ �������� �����



private:
//������� ������� � �������������� ������������� ������������ �������
	double get_h_b() const {return composite_section_.get_concrete_part()->get_h_b();}
	double get_C_b() const {return composite_section_.get_concrete_part()->get_C_b();}
	double get_t_f2() const {return composite_section_.get_steel_part().get_t_uf();}
	double get_t_f1() const {return composite_section_.get_steel_part().get_t_lf();}
	double get_b_f2() const {return composite_section_.get_steel_part().get_b_uf();}
	double get_A_w_st() const {return composite_section_.get_steel_part().get_b_uf();}
	double get_A_b() const {return composite_section_.get_concrete_part()->get_A_b();}
	double get_R_s() const {return  composite_section_.get_concrete_part()->get_rebar().get_R_s();}
	double get_A_s() const {return  composite_section_.get_concrete_part()->get_rebar().get_A_s();}
	double get_R_b() const {return  composite_section_.get_concrete_part()->get_concrete().get_R_b();}
	double get_R_y() const {return  composite_section_.get_steel_grade().get_R_y ();}
	double get_A_st() const {return  composite_section_.get_steel_part().get_A_st();}
	double get_A_f1_st() const {return  composite_section_.get_steel_part().get_A_f1_st();}
	double get_A_f2_st() const {return  composite_section_.get_steel_part().get_A_f2_st();}
	double get_b_sl() const {return composite_section_.get_concrete_part()->get_b_sl();}
	double get_h_w() const {return composite_section_.get_steel_part().get_h_w();}
	double get_t_w() const {return composite_section_.get_steel_part().get_t_w();}

//������� ������� � �������

	std::vector<double> get_M(Impact impact);

	void calculate_gamma_1();
	void calc_cs_coordinates(); //����������� ��������� ������� ��� ����������� ������ ��������� ��� �������� �����
	void calc_studs_coordinates(); // ����������� ��������� ������� ��� ����������� ������ ��������� ��� ������
	void calc_inter_forces_for_studs();
	void calc_inter_forces();
	void calc_stresses();
	void calc_ratios();
	void calc_studs_ratios();
	NeutralAxis calc_neutral_axis();
	double calc_rigid_plastic_moment();
    void calc_ratio_rigid_plastic();
//������ ����������
	void calculate_stresses();
 // ������ ������������� �������������
	Ratios calculate_I_case(Impact impact, int cs_id);
	Ratios calculate_II_case(Impact impact, int cs_id);
	Ratios calculate_III_case(Impact impact, int cs_id);

//������ � ���
	void log_stresses();
 };

#endif

//class TheoryCalculator
//{
//virtual calc1();
//virtual void calc1(CompositeBeam* compos_beam)=0;
//}
//
//class Theory1Calculator:TheoryCalculator
//{
//	public:
// calc(CompositeBeam* compos_beam)
//	{
//	void calc2();
//	calc3();
//	}
//	private:
//    void calc2();
//    void calc3();
//}
//
//class Theory2Calculator:TheoryCalculator
//{
//	public:
// calc(CompositeBeam* compos_beam)
//	{
//	calc3()
//	}
//	private:
//	void calc3();
//}
//
//class TheoryFactory
//{
//    virtual void TheoryCalculator createCalculator()=0;
//}
//
//class Theory1Factory:TheoryFactory
//{
//	TheoryCalculator createCalculator()
//    {
//        return new Theory1Calculator();
//	}
//}
//class Theory2Factory:TheoryFactory
//{
//	TheoryCalculator createCalculator()
//    {
//        return new Theory2Calculator();
//	}
//}
//
//main()
//{
//TheoryFactory* theoryFactory;
//
//if(....)
//theoryFactory=new Theory1Factory();
//
//TheoryCalculator *calc = theoryFactory->createCalculator(&balka);
//calc->calc();
//
//}
