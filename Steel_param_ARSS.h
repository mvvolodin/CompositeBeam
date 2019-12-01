//---------------------------------------------------------------------------

#ifndef Steel_param_ARSSH
#define Steel_param_ARSSH

#define EMBRAC //���������������� �� ��������
// ��������� ��������� �� ����. �.3 �� 16.13330.2017
#define N_STEEL_HOT 15  //�����������, ��� ����� ������� ���������� ����� �����
#define N_STEEL_HOTB 8
#define N_STEEL_COLD 6

#define STEEL_HOT  "C235", "C245", "C345K", "C355",  "C355-1", "C355-K", \
				   "C355�", "C390", "C390", "C390-1", "C440", "C590", "C550", "C590", "C690"
#define STEEL_HOTB "C255�", "C255�-1", "C345�", "C345�-1", "C355�", "C355�-1", "C390�", "C440�"
#define STEEL_COLD "220", "��,��", "250", "280", "320", "350"     //���������������� �� �������� 29.09.19  ���. 15

#define N_C235  1 // ���������� ���������� ������
#define N_C245  1
#define N_C255  4
#define N_C345  6
#define N_C345K 1
#define N_C355  6
#define N_C355_1  3
#define N_C355_K  3
#define N_C355_P  2
#define N_C390  1
#define N_C390_1 1
#define N_C440  1
#define N_C550  1
#define N_C590  1
#define N_C690 1
//--------------------------------
#define N_C255B    7
#define N_C345B    4   //
#define N_C345B_1  4
#define N_C355B    6
#define N_C355B_1  3
#define N_C390B    6
#define N_C440B    5


#define steel_T_UPPER  1       //���������������� �� �������� 29.09.19  ���. 42-52
#define steel_T_LESS   2
#define steel_NO_STEEL 3
#define steel_T_UPPER_GLOB  4

#define weldSINGLE 1
#define weldDOUBLE 2

#define weldTAVR2 0
#define weldNAXLEST 1
#define weldTAVR1  2

//---------------------------------------------------------------------------
typedef struct _steel_param {  // ������ � ���������� ����� ��� ������ �������
	char   title[8];
	double thick;
	double Ryn;
	double Run;                 // ��������� ������������� ����� ������� - sigma_b
	double Ry;
	double Ru;
} STEEL_PARAM;

typedef struct _mater_param {  // ������ � ���������� �����
	char   title[8];
	int    n_row;
	double thick_row[10];
	double Ryn_row[10];
	double Run_row[10];
	double Ry_row[10];
	double Ru_row[10];
} MATER_PARAM;

// �������� ��������� ������������� ����������, ������ � ������ �� ����� ����� � ������� �������
double Steel_param_Ry(char *Msteel, double thick, int *rc, bool flag_diag_thick);
//  �������� ����������� � ��������� ������������� �� ����� ����� � ������� �������
int Steel_param(char *Msteel, double thick, STEEL_PARAM* steel_param, bool flag_diag_thick);
//  �������� ������� ������� ����� �� ����� �����
int Get_Mater_param(char *Msteel, MATER_PARAM* mater_param);
// �������� ����������� � ��������� ������������� �� ����� ����� � ������� ������� � �������������� ������� ���������
int Get_steel_param(MATER_PARAM *mater_param, double thick, STEEL_PARAM* steel_param);
// ��������� ������� ���������
void Fill_mater_param(char* Msteel, int n_thick, double steel[][4], double *row_t, MATER_PARAM* mater_param);

#endif
