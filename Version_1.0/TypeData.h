/*------------------------------------------------------------------*/
/*                       ���� ������                                */
/*------------------------------------------------------------------*/

#ifndef tdataH
#define tdataH

#include <vcl.h>
#include "Alinit.h"
#include "stdio.h"

#define  MAX_GRAN 6      /* ������������ ����� ������ �� �������� */
#define  MAX_GRAN_NE 4   /* ������������ ����� ����� �� ����� */
#define  MAX_MATERIAL  10     // ������������ ����� ����������

#define  COMMON     "Common"
#define  MATERIAL   "Material"
#define  CONSTR     "Constr"
#define  LOADING    "Loading"
#define  COMBIN    "Combin"
#define  JOINT     "Joint"
#define  ANALYSIS_PARAM     "Analysis_Param"
#define  MESH       "Mesh"
#define  RESULT     "Result"
#define  UNIT_PARAM "Unit_Param"
#define  ZATYAGHKA_PARAM "Zatyaghka_Param"
#define  CONSTRUCT_ELEMENT "Construct_Element"
#define  MASS_AND_DYNAM "Mass_and_Dynam"
#define  ARBIT_UNIT_TYPE "Arbit_Unit_Type"
#define  PURLINS_PARAM "Purlins_Param"
#define  SKELETON "Skeleton"


typedef struct _release              /* ������� ���������� */
    {
       short node;
       bool rel[6];
    }  RELEASE;

typedef struct _point_3D              /* ���������� ����� */
    {
       float x;
       float y;
       float z;
    } POINT_3D;
typedef struct _numgran              /* �������� ����� */
    {
       int n_elem;  // ����� ��������, �������� ����������� �����
       short n_gran;    // ����� ����� �� ��������
       int i_gran;  // �������� ����� �����
    } NUM_GRAN;

typedef struct _gran              /* �������� ����� */
    {
       short gran_node[MAX_GRAN_NE+1];
    }GRAN;


typedef struct _vname               /* ������������ ������������ */
    {
      unsigned short node1;          /* ����� ������� ���� */
      unsigned short node2;          /* ����� ������� ���� */
      unsigned char direct;          /* ����������� */
    }  VNAME;

#define VNSIZE sizeof(VNAME)

typedef union _anytype
{
  char ch;
  VNAME vn;
  int  in;
  short sh;
  double dou;
} ANYTYPE;

typedef struct _cord  /* ������� ������� ������ ��������� */
    {
      char type;         /* ��� ������� ���������: R, � ��� S */
      int  meth;         /* ������ �������: 1 ��� 2 */
      int  rid;          /* ����� ������� ���������, � �������
                            ���������� ������ ������ ��������� */
      int  num[3];       /* ������ ���� ����� ��� ������� 1   */
      double coor[3][3]; /* ���������� ���� ����� ��� ������� 2 */
    }  CORDSYS;

typedef struct _material  /* ������� ������� ������ ��������� */
    {
      char   title[12];
      double E;          /* ������ ���� */
      double G;          /* ������ ������ */
      double nu;         /* ����������� �������� */
	  double Ry;         /* ��������� ����������� ���������� (������ ���������) ��� ����������� ������ */
	  double Ru;         /* ��������� ��������� ������������� ������� */
	  double gamma_m;    /* ����������� ���������� �� ��������� */
      double dens;       /* ��������� */
    }  MATER;

typedef struct _felem                  /* �������� ��������� */
    {
      char  *type;
      short nnode;                     /* ����� ����� �� �������� */
      short nstif;                     /* ����� ���������� �� �������� */
      short ndens;                     /* ����� ���������� �� �������� */
      short norder;                    /* ������� �� */
      short ord1;                      /* ����� ����� ������� */
      short ord2;                      /* ����� �������� */
      short nrez;                      /* ����� ����������� */
      char *direct;                    /* ����������� */
      short n_dimention;               /* ����������� �������� */
      short n_gran;                    /* ����� ��������� ������ */
      short m_gran;                    /* ����� ����� �� ����� */
      short *num_sigma_equ_down;       /* ������ ������������� ���������� � ����� � � ������ */
      short *num_sigma_equ_up;         /* ������ ������������� ���������� � ����� � � ������ */
      short *num_sigma_comp_down;      /* ������ ����������� ���������� � ������ �������� � ��� */
      short *num_sigma_comp_up;        /* ������ ����������� ���������� � ������ �������� � ��� */
      short *num_force_equ;            /* ������ ������������� �������� ������ � ����� � � ������ */
      short *num_sigma_master;         /* ������ ������� ���������� S1 � S2 � ���� */
      short *num_force_master;         /* ������ ������� ������ N1 � N2 � ���� */
      short *num_gran[MAX_GRAN];        /* �������� ��������� ������ */
      unsigned char  *num1;
      unsigned char  *num2;
    }  FELEM;


 typedef struct _cons
 {
     int  node;               /* ����� ���� */
     bool constr[6];          /* ����������� */
 } CONS;

 typedef struct _cons_c
 {
     int  node;               /* ����� ���� */
     char DOF[7];          /* ����������� */
 } CONS_c;

typedef struct _load_nodal  {
    int    node;             // ����� ����
    double force[6];         // �������� �� ������������  
}  LOAD_NODAL;

typedef struct _load_elemental  {
    int    element;          // ����� ��������
    double length;           // ����� ��������
    double force_1[3];       // �������� �� ������������ �� ����� 1: �������� = 0
    double force_2[3];       // �������� �� ������������ �� ����� 2: �������� = length
}  LOAD_ELEMENTAL;

typedef struct _cont
 {
    char type[9];
    short cod;         /* ��� �������� */
    int gr_stf;        /* ����� ������ ���������� �������� */
    int num_gr;        /* ����� ������ (��������) ��������� */
    int num_el;        /* ����� �������� � ������ (��������) */
 } CONT;

typedef struct _param_analyse
 {
    char type_analyse[12];
    int num_active_load;
    double gamma;       // ����������� ���������� �� ���������������
 } PARAM_ANALYSE;

typedef struct _double3 {
    double g[3];
} DOUBLE3;

typedef struct _double9 {
    double g[9];
} DOUBLE9;

typedef struct _double11 {
    double g[11];
} DOUBLE11;

typedef struct _coor_el{
     double x1;
     double z1;
     double x2;
     double z2;
} COOR_EL;


#endif

void formtvm(double* xo, double* x1, double gam, double t_conv[][4],
             double *z_mod);


FELEM * antype(int);

int cmp_load_nodal(LOAD_NODAL*, LOAD_NODAL*);
int cmp_load_elemental(LOAD_ELEMENTAL*, LOAD_ELEMENTAL*);

int cmp_point3d(POINT_3D*, POINT_3D*);
int cmp_point3d_eps(POINT_3D*, POINT_3D*, float);
int cmpcont(CONT*, CONT*);
int cmpvname(VNAME*, VNAME*);
int cmp_gran(GRAN*, GRAN*);
int cmpdouble(double*, double*);
int cmpcoor(double *elem1, double *elem2);
int cmpi(int*, int*);
int cmp_coor_el(COOR_EL *elem1, COOR_EL *elem2);
int cmps(unsigned short*, unsigned short*);
int cmpd(double*, double*);
int cmpdi(double*, double*);
int cmp_double3(DOUBLE3*, DOUBLE3*);
int cmp_double9(DOUBLE9*, DOUBLE9*);
int cmp_double11(DOUBLE11*, DOUBLE11*);
int cmp_TPoint(TPoint *elem1, TPoint *elem2);
int cmp_TPoint_diff(TPoint *elem1, TPoint *elem2, TPoint *diff);

void prin_lis(char*, int, VNAME*);  /* ������ ������� */
void prinv_lis(char*,int, double*, VNAME*);  /* ������ ������� */
void prinv_tran(char*,int, double*, VNAME*);  /* ������ ������� �� ������������ u, v, w */
void prinv_point_3d(char*,int, POINT_3D*);  /* ������ ������� */
void prinv(char*,int, double*);  /* ������ ������� */
void prinv_bool(char *str, int n, bool *vec);
void prinv_F(char *str, int n, double *vec);
void prinvi(char*,int, int*);  /* ������ ������� */
void prinvf(char*,int, float*);  /* ������ ������� */
void prinm0(char*, int,int, double**);  /* ������ ������� �� ������� */
void prinv_cord(char *str, int n, CORDSYS* cord);
AnsiString Float_to_str(double value, char *format);


int Get_load_active( int n_dof, int nnel, DSC_MVC * elnum, VNAME* listvar,
             LOAD_NODAL * load_nodal, LOAD_ELEMENTAL *load_elemental, double *load, int *n_load, int *e_load );
int  ConvertLoadElemToNodal(int n_dof, VNAME *listvar,
                        LOAD_ELEMENTAL* load_elemental, int e_load, double *load_sign, double mult_F);    
int  ExpandLoadMeshBody(int nnel, int n_dof, DSC_MVC * elnum, VNAME *listvar, double *load);
void Set_disp_node(int nnuz, int n_dof, VNAME *list, double *disp, double **disp_node);
int Set_disp_node_m(int nnuz, int n_dof, VNAME *list, double *disp, int num_disp, double **disp_node);
void Set_koeff_disp(int n_coor, double **disp_node, double* koeff_disp, double span);
// �������� ������������ ������ ���� - ���� �� X, ���� �� Z
double Get_max_size();

int Get_coor_point_on_norm(int n_line, double length, double *coor_point, double dx, double dz);
int  restcons_node(int nnuz, CONS* constr_node, int *n_constr);

void rep_int(int* list, int* vec, int n);
