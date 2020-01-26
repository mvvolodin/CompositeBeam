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
	ISection steel_part;
	TConcretePart* concrete_part;//����������� ���. ���� �� ����������� ���� �� ���������?
	Steel i_sect_steel; //��������� � ��� ISection, ��� ��� ��������� � ������������� TSteelPart

public:
	double Alfa_s_;//����������� ���������� � �����
	double Alfa_b_;//����������� ���������� � ������
	double H_stb_; //������ ������������ �������
	double A_red_; //������� �������������������� �������
	double Z_b_st_; //���������� ����� �������� ������� ����� � �������� �����
	double S_stb_;//����������� ������ ������� �������������������� ������� ������������ �.�. ��������� �������
	double Z_st_red_;//���������� ����� ������� ������� ��������� � �������������������� �������
	double I_red_;  //������ ������� �������������������� ������� ����������� � �������
	double Z_red_f2_;//���������� ����� �.�. �������������������� ������� � �������� ������ ������� �����
	double Z_red_f1_; //���������� ����� �.�. �������������������� ������� � �������� ������ ������ �����
	double Z_b_red_;  //���������� ����� �.�. ��������� � �������������������� �������
	double W_f2_red_; //������ ������������� �������������������� ������� ��� ������� �����
	double W_f1_red_; //������ ������������� �������������������� ������� ��� ������ �����
	double W_b_red_;//������ ������������� �������������������� ������� ��� �.�. �������������� �����
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


