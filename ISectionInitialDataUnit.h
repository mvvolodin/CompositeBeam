//---------------------------------------------------------------------------

#ifndef ISectionInitialDataUnitH
#define ISectionInitialDataUnitH

#include "Data_Profil.h"
#include "Get_param_sect_func.h"

//---------------------------------------------------------------------------
struct TISectionInitialData{

	String profile_number_;//��� ������� (� ���� � 57837 ����� �������)
	double b_uf_;
	double t_uf_;
	double b_lf_;
	double t_lf_;
	double h_w_;
	double t_w_;
	double weight_;//�������� �����
	double r_;//������ �������
	double Ast;
	double Ist;
	double Hsect; // ������ ������� (�� ��� Z)
	double Sx[3]; // ���������� ������� � ������ ���������� ����������� ����������
	double Sy[3]; // ���������� ������� � ������ ���������� ����������� ����������
	double Hp;    // ���������� ����� �.�. ������
	double Z_f2_st;    // ��������� �.�. ������� ������������ �.�. �������� �����
	double Z_f1_st;  // ��������� �.�. ������� ������������ �.�. ����������� �����
	double Wf2_st;
	double Wf1_st;

	TISectionInitialData(){};
	TISectionInitialData (SECT_DVUTAVR* i_section_initial_data);

};
#endif

