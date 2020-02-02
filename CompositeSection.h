//---------------------------------------------------------------------------

#ifndef CompositeSectionH
#define CompositeSectionH
#include "ISectionInitialDataUnit.h"
#include "ISection.h"
#include "ConcretePartUnit.h"
#include "Steel.h"
#include "Rebar.h"
#include "SteelPart.h"

using namespace std;

struct TSteelInitialData;//������� forward declaration
class TGeometry;


class CompositeSection {
private:
	double id_;//�� ������������;
	double coorinate_; //�� ������������

	ISection steel_part;
	TConcretePart* concrete_part;//����������� ���. ���� �� ����������� ���� �� ���������?
	Steel i_sect_steel; //��������� � ��� ISection, ��� ��� ��������� � ������������� TSteelPart

	static ISection steel_part___; //�� ������������
	static TConcretePart* concrete_part___;  //�� ������������
	static Steel i_sect_steel___;  //�� ������������

 //	InternalForces2 internal_forces_;
//	Stresses stresses_;
//	Ratios ratios_;

public:
	double Alfa_s_=0.0;//����������� ���������� � �����
	double Alfa_b_=0.0;//����������� ���������� � ������
	double H_stb_=0.0; //������ ������������ �������
	double A_red_=0.0; //������� �������������������� �������
	double Z_b_st_=0.0; //���������� ����� �������� ������� ����� � �������� �����
	double S_stb_=0.0;//����������� ������ ������� �������������������� ������� ������������ �.�. ��������� �������
	double Z_st_red_=0.0;//���������� ����� ������� ������� ��������� � �������������������� �������
	double I_red_=0.0;  //������ ������� �������������������� ������� ����������� � �������
	double Z_red_f2_=0.0;//���������� ����� �.�. �������������������� ������� � �������� ������ ������� �����
	double Z_red_f1_=0.0; //���������� ����� �.�. �������������������� ������� � �������� ������ ������ �����
	double Z_b_red_=0.0;  //���������� ����� �.�. ��������� � �������������������� �������
	double W_f2_red_=0.0; //������ ������������� �������������������� ������� ��� ������� �����
	double W_f1_red_=0.0; //������ ������������� �������������������� ������� ��� ������ �����
	double W_b_red_=0.0;//������ ������������� �������������������� ������� ��� �.�. �������������� �����
public:
	CompositeSection();
	CompositeSection(SteelPart    steel_part,
					 TConcretePart* concrete_part);
	CompositeSection(TGeometry geometry,
					  Steel steel_i_section,
					  TISectionInitialData i_sect_initial_data,
					  TConcretePart* concrete_part);
private:
	void alfa_to_rebar_steel_calc();
	void alfa_to_concrete_calc();
	void compos_sect_height_calc();
	void compos_sect_area_calc();
	void dist_conc_to_steel_calc();
	void first_moment_of_area_comp_calc();
	void dist_steel_to_compos_calc();
	void inertia_compos_calc();
	void dist_compos_to_upper_fl_calc();
	void dist_compos_to_lower_fl_calc();
	void dist_conc_to_compos_calc();
	void sect_modulus_upper_fl();
	void sect_modulus_lower_fl();
	void sect_modulus_conc();
public:
	inline ISection get_steel_part()const {return steel_part;}
	inline TConcretePart* get_concrete_part()const {return concrete_part;}
	inline Steel get_steel_grade(){return i_sect_steel;}
	inline double get_alfa_b() const {return Alfa_b_;}
	inline double get_alfa_s() const {return Alfa_s_;}
	inline double get_A_red() const {return A_red_;}
	inline double get_I_red() const {return I_red_;}
	inline double get_W_f2_red() const {return W_f2_red_;}
	inline double get_W_f1_red() const {return W_f1_red_;}
    inline double get_W_b_red() const {return W_b_red_;}
	inline double get_Z_b_st() const {return Z_b_st_;}

} ;
//---------------------------------------------------------------------------
#endif


