//---------------------------------------------------------------------------

#ifndef WRData_CheckH
#define WRData_CheckH
//---------------------------------------------------------------------------
///---------------------------------------------------------------------------
//#include "Data_Gen.h"
#include<stdio.h>

#define LENGHT_CATALOG 1000
#define NDATA 20
#define ALREADY_WRITE  1
#define ALREADY_NDATA  2
#define NO_DATAS       3
#define DATA_WRITING   4
#define ERROR_WRITING  5
#define FULL_CATALOG   6

#define PRINT_ERROR_MESSAGE "������ ��� ������� �� ������", "ALREADY_NDATA", "��� ������", \
                      "��� ������ (������� ��������)", "������ ������", "������� ��������"

class FData {
  public:
    int  pos_catalog; // ������� � ��������
    bool flag_free;
    int offset;       // �������� �����
    int offset_max;   // ����������� ���������� �������� �����
    char mode;        // ��� �������
};
class CATALOG_BD {
  public:
    char title[24]; // ��� ������
    int type;       // ��� ������
    int condition;  // ��������� (0-�������, 1-������ ������, 2-������ ������
    int offset;     // �������� ������������ ������ �����
    int offset_max; // ������������ ��������
    int param[16];  // ����������� ��������� ������
};
class CONT_BD {
  public:
    int version;    // ����� ������ ��
    char title[24]; // ��� ����� - �����
    int condition;  // 0-��������, 1-���� ������, 2-���� ������
    int NData;      // ���������� ������ � ��������
    int NNode;      // ����� �����
    int NElem;      // ����� ���������
    int NDOF;       // ����� �����������
    int WFront;     // ������ ������
	int B;
    int C;
    int D;
    int Cur_offset; // ������� ��������
};
FILE * BD_New(char *NameBD);

/*
    int BD_RCommon(FILE * BD_file, char *name_file);
    bool BD_RMaterial(FILE * BD_file, char *name_file);
    bool BD_RMesh(FILE * BD_file, char *name_file);
    bool BD_RLoad(FILE * BD_file, char *name_file);
    bool BD_RConstr(FILE * BD_file, char *name_file);
    bool BD_RCombin(FILE * BD_file, char *name_file);
    bool BD_RJoint(FILE * BD_file, char *title);
    bool BD_RAnalys_Param(FILE * BD_file, char *name_file);
    bool BD_RResult(FILE * BD_file, char *title);
    bool BD_RUnits(FILE * BD_file, char *name_file);
    bool BD_RZatParam(FILE* BD_file, char *name_file);
    bool BD_RConstr_elem(FILE* BD_file, char *name_file);
    bool BD_RMass_and_Dynam(FILE* BD_file, char *name_file);
    bool BD_RArbitUnit(FILE* BD_file, char *name_file);
    bool BD_RPurlins(FILE* BD_file, char *name_file);

    bool BD_WCommon(FILE * BD_file, char *name_file);
    bool BD_WMaterial(FILE * BD_file, char *name_file);
    bool BD_WMesh(FILE * BD_file, char *name_file);
    bool BD_WLoad(FILE * BD_file, char *name_file);
    bool BD_WConstr(FILE * BD_file, char *name_file);
    bool BD_WCombin(FILE * BD_file, char *name_file);
    bool BD_WJoint(FILE * BD_file, char *title);
    bool BD_WAnalys_Param(FILE * BD_file, char *name_file);
    bool BD_WResult(FILE * BD_file, char *title);
    bool BD_WUnits(FILE * BD_file, char *name_file);
    bool BD_WZatParam(FILE* BD_file, char *name_file);
    bool BD_WConstr_elem(FILE* BD_file, char *name_file);
    bool BD_WMass_and_Dynam(FILE* BD_file, char *name_file);
    bool BD_WArbitUnit(FILE* BD_file, char *name_file);
    bool BD_WPurlins(FILE* BD_file, char *name_file);


    bool BD_RCommon3DSkeleton(FILE * BD_file);
    bool BD_RCommon3D(FILE * BD_file, int num_rama);
    bool BD_RMaterial3D(FILE * BD_file, int num_rama);
    bool BD_RMesh3D(FILE * BD_file, int num_rama);
    bool BD_RLoad3D(FILE * BD_file, int num_rama);
    bool BD_RConstr3D(FILE * BD_file, int num_rama);
    bool BD_RCombin3D(FILE * BD_file, int num_rama);
    bool BD_RJoint3D(FILE * BD_file, int num_rama);
    bool BD_RAnalys_Param3D(FILE * BD_file, int num_rama);
    bool BD_RResult3D(FILE * BD_file, int num_rama);
    bool BD_RUnits3D(FILE * BD_file, int num_rama);
    bool BD_RZatParam3D(FILE * BD_file, int num_rama);
    bool BD_RConstr_elem3D(FILE * BD_file, int num_rama);
    bool BD_RMass_and_Dynam3D(FILE * BD_file, int num_rama);
    bool BD_RArbitUnit3D(FILE * BD_file, int num_rama);
    bool BD_RPurlins3D(FILE* BD_file, int num_rama);

    bool BD_WCommon3DSkeleton(FILE * BD_file);
    bool BD_WCommon3D(FILE * BD_file, int num_rama);
    bool BD_WMaterial3D(FILE * BD_file, int num_rama);
    bool BD_WMesh3D(FILE * BD_file, int num_rama);
    bool BD_WLoad3D(FILE * BD_file, int num_rama);
    bool BD_WConstr3D(FILE * BD_file, int num_rama);
    bool BD_WCombin3D(FILE * BD_file, int num_rama);
    bool BD_WJoint3D(FILE * BD_file, int num_rama);
    bool BD_WAnalys_Param3D(FILE * BD_file, int num_rama);
    bool BD_WResult3D(FILE * BD_file, int num_rama);
    bool BD_WUnits3D(FILE * BD_file, int num_rama);
    bool BD_WZatParam3D(FILE * BD_file, int num_rama);
    bool BD_WConstr_elem3D(FILE * BD_file, int num_rama);
    bool BD_WMass_and_Dynam3D(FILE * BD_file, int num_rama);
	bool BD_WArbitUnit3D(FILE * BD_file, int num_rama);
*/

void PrinCodOpenData(int rc);

//extern "C" void _declspec(dllexport) BD_Control(COM_RAMA  *Common_rama);   // ����� ������ � ����
FILE * BD_Open(char *NameBD, char* index);
int  BD_Close(FILE * BD_file);
FData * BD_WOpenData(FILE *FileData, char *NameData,
                 int type, int * param, int *rc);
FData * BD_ROpenData(FILE *FileData, char *NameData,
				 int type, int * param, int *rc);
void BD_CloseData(FData * Pfdata);
int  BD_WriteData(char * ptr, int size, int n, FData * Pfdata);
int  BD_ReadData(char * ptr, int size, int n, FData * Pfdata);

#endif

