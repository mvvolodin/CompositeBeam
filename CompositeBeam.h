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
//---------------------------------------------------------------------------

class TCompositeBeam{
private:
	typedef std::vector<double> CS_Coordinates;
private:
//������� ����
	int beam_division_;
	TGeometry geometry_;
	TLoads loads_;
	WorkingConditionsFactors working_conditions_factors_;
	CompositeSection composite_section_;
	TStud stud_;

//���������� ����
	CS_Coordinates cs_coordinates_;
	InternalForcesNamededList internal_forces_LC_;
	InternalForces internal_forces_I_;
	InternalForces internal_forces_II_;
	InternalForces internal_forces_total_;

//����������� ���� ����������

	StressesNamedList stresses_LC_;
	Stresses stresses_I_;
	Stresses stresses_II_;
    Stresses total_;

	std::vector<double> test_M_;
    std::vector<double> test_Q_;
	//StressesNamedList stresses_;
	double gamma_1_;//����������� ������� ������ �������� �����

private:
	void calculate_gamma_1();
	void CS_coordinates_calc();
	void print_i_sect_to_log();//������ ��������� �������� � log
	void print_concrete_part_to_log();//������ �������������� ����� � log
	void print_inter_forces_to_log();//������ ���������� ������ � log
	void print_comp_sect_to_log();//������ ��������� ������������ ������� � log
	void print_stresses_to_log();
	void calc_inter_forces();
	void calc_stresses();

public:
	TCompositeBeam();
	TCompositeBeam(TGeometry 				geometry,
				   TLoads 					loads,
				   CompositeSection         composite_section,
				   TStud 					stud,
				   WorkingConditionsFactors working_conditions_factors,
				   int 						beam_division);



inline CompositeSection get_composite_section()const{return composite_section_;}//��������� ������!!!
 };                                                                                //�� ������������ ������ �� ������
#endif
