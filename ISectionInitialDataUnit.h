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
	double Ar;
	double Izz;
	double Iyy;
	double Hsect; // ������ ������� (�� ��� Z)
	double Zct;   // ��������� �.�. ������� ������������ ������� �����
	double Sx[3]; // ���������� ������� � ������ ���������� ����������� ����������
	double Sy[3]; // ���������� ������� � ������ ���������� ����������� ����������
	double Hp;    // ���������� ����� �.�. ������
	double Zp;    // ��������� �.�. ������� ������������ �.�. �������� �����
	double Wu_max;
	double Wu_min;
	double Wv_max;
	double Wv_min;

	TISectionInitialData(){};
	TISectionInitialData (SECT_DVUTAVR* i_section_initial_data);

};
#endif

