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
#include "StudUnit.h"
#include "Rebar.h"
#include "WorkingConditionsFactors.h"
#include "Stresses.h"
#include "Ratios.h"
//---------------------------------------------------------------------------

class TCompositeBeam{

	typedef std::vector<Stresses> StressesList;
	typedef std::map<Impact, StressesList> StressesNamedList;
	typedef std::pair <Impact, StressesList> StressesNamedListItem;
	typedef StressesNamedList::iterator StressesNamedListIterator;

	typedef std::vector<Ratios> RatiosList;
	typedef RatiosList::iterator RatiosListIterator;
	typedef std::map<Impact, RatiosList> RatiosNamedList;
	typedef std::pair <Impact, RatiosList> RatiosNamedListItem;
	typedef RatiosNamedList::iterator RatiosNamedListIterator;

public:

	TCompositeBeam();
	TCompositeBeam(TGeometry 				geometry,
				   TLoads 					loads,
				   CompositeSection         composite_section,
				   TStud 					stud,
				   WorkingConditionsFactors working_conditions_factors);

	inline CompositeSection get_composite_section()const{return composite_section_;}//��������� ������!!!//�� ������������ ������ �� ������
	inline InternalForcesNamedList get_internal_forces_LC()const{return internal_forces_;}
	inline std::vector<double> get_CS_coordinates()const {return cs_coordinates_;}
	inline TStud get_studs()const {return studs_;}
	inline TGeometry get_geometry()const {return geometry_;}
	inline TLoads get_loads()const {return loads_;}

	double get_max_upper_flange_ratio();
	double get_max_lower_flange_ratio();

private:
//���� � ��������� �������
	TGeometry geometry_;
	TLoads loads_;
	WorkingConditionsFactors working_conditions_factors_;
	CompositeSection composite_section_;
	TStud studs_;
//���� � ������������ ��������
	std::vector<double> cs_coordinates_;
	int cs_num_;//���������� ��������� �������.��� ����� ������ ��� ���������!!!!
	InternalForcesNamedList internal_forces_;
//���� � ������������ �������� ����� ����������
	StressesNamedList stresses_named_list_; //
//���� � ������������ �������� ����� ������������ �������������
	RatiosNamedList ratios_named_list_;//std::map<Impact, StressesList> StressesNamedList//std::vector<Stresses> StressesList

	double gamma_1_;//����������� ������� ������ �������� �����



private:
	void calculate_gamma_1();
	void CS_coordinates_calc();
	void calc_inter_forces();
	void calc_stresses();
	void calc_ratios();
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
