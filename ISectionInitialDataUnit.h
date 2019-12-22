//---------------------------------------------------------------------------

#ifndef ISectionInitialDataUnitH
#define ISectionInitialDataUnitH

#include "Data_Profil.h"
#include "Get_param_sect_func.h"

//---------------------------------------------------------------------------
struct TISectionInitialData{

	GEOM_PARAM_SECT excessive_sect_geomet_data;
	double b_uf_;
	double t_uf_;
	double b_lf_;
	double t_lf_;
	double h_w_;
	double t_w_;
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

/*
    typedef struct _geom_param_sect {   // �������������� ��������� �������
       double Ar;
       double Izz;
       double Iyy;
       double Iyz;
       double Itor;
       double Hsect; // ������ ������� (�� ��� Z)
       double Wsect; // ������ ������� (�� ��� Y)
       double Zct;   // ��������� �.�. ������� ������������ ������� �����
       double Yct;   // ��������� �.�. ������� ������������ ������� �����  Yct = Wsect/2 (������� �������������� ������������)
       double Ze;    // ��������� ������ ������ ������� ������������ ������� �����
       double Ye;    // ��������� ������ ������ ������� ������������ ������� �����
       double ky;    // ����������� ��������� ��������� �� ��� Y
       double kz;    // ����������� ��������� ��������� �� ��� Z
       double point_stress[4][2]; // ���������� ����� ����������� ���������� (��� �������� � NASTRAN)
       double z_tau[3]; // ���������� Z ����� ����������� ����������� ���������� (������������ �.�)
       double Sx[3]; // ���������� ������� � ������ ���������� ����������� ����������
       double Sy[3]; // ���������� ������� � ������ ���������� ����������� ����������
       double Hp;    // ���������� ����� �.�. ������
       double Zp;    // ��������� �.�. ������� ������������ �.�. �������� �����
       double Rad;   // ������ ���������� ����� �� ������� ��� ����������� ��������
       double alfa;  // ���� ����� ���� Y � ������� ����
       double I1;    // ������� ������ �������
       double I2;    // ������ ������� ������ �������
       double Iw;    // ������������� ������ �������
       double Wx_f;  // ������ ������������� ������ �������, ���������� ������ �� �����
       double Wx_pl; // ������������ ������ ������������� ������
       double Wu_max;
       double Wu_min;
       double Wv_max;
       double Wv_min;
       int    index_orient;  // ���������� ������� 0 - � ��. XZ; 1 - � ��. YZ
	}  GEOM_PARAM_SECT;
*/
