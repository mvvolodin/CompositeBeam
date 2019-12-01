/*------------------------------------------------------------------*/
/*                       ���� ������ ��������                       */
/*------------------------------------------------------------------*/

#ifndef DATA_PROFILH
#define DATA_PROFILH

#include <vcl.h>

#define NO_CHOISE          0
#define DVUTAVR            1
#define SQUARE_TUBE        2
#define RECT_TUBE          3
#define CIRCULAR_TUBE      4
#define RECTANCULAR        5
#define SHVELLER           6
#define TAVR_UP            7
#define TAVR_DOWN          8
//#define DVUTAVR_YZ         9

#define GNU_CH              10
#define GNU_C              11
#define GNU_Z              12
#define GNU_ZC             13
#define GNU_SIGMA          14

typedef struct _sect_dvutavr {
   double b;      // ������� ������ ��������
   double h;      // ������ ������ ��������
   double b1;     // ������ �������� ����� ��������
   double h1;     // ������� ���������� ����� ��������
   double b2;     // ������ �������� ����� ��������
   double h2;     // ������� ���������� ����� ��������
   int    n_group;   // ����� ������ ��������
   int    n_profil;  // ����� ������� � ������
   int    index_orient;   // ���������� = 0 (� ��������� XZ) ��� 1 (� ��������� YZ)
   int    type_prof;  // ��� ������� (DVUTAVR) , ��� ������
   float  fG;
   float  tnom;    // ����������� ������� � ������ ���� ��������
   float  b2G;     // ������ ������ ����� �������
   float  gG;    //
   bool flag_concl;
}  SECT_DVUTAVR;

typedef struct _sect_gnutic {
   double tG;      // ������� ������ (�����)
   double hG;      // ������ �������
   double bG;      // ������ �����
   double cG;      // ������ ������
   double rG;      // ������ ����������
   double jG;      // ������ �����������
   int    n_group;   // ����� ������ ��������
   int    n_profil;  // ����� ������� � ������
   int    index_orient;   // ���������� = 0 (� ��������� XZ) ��� 1 (� ��������� YZ)
   int    type_prof;      //
   float  fG;      // ������� �����������
   float  tnom;    // ����������� ������� � ������ ���� ��������
   float  b2G;     // ������ ������ ����� �������
   float  gG;      // ������ ���������� �����
   bool flag_concl;
}  SECT_GNUTIC;

typedef struct _sect_square {
   double thickness;  // ������� ������
   double side;       // ������ �������
   double b1;   // �� ������������
   double h1;   // �� ������������
   double b2;   // �� ������������
   double h2;   // �� ������������
   int    n_group;   // ����� ������ �������� - ���������� h*b
   int    n_profil;  // ����� ������� � ������ - ���������� t
   int    index_orient;   // ���������� = 0
   int    dummy_int;
   double rad;
   double t_red;
   bool flag_concl;
}  SECT_SQUARE;

typedef struct _sect_rect {
   double b;      // ������ �������
   double h;      // ������ �������
   double b1;     // ������� ������ �� ������
   double h1;     // ������� ������ �� ������
   double b2;     // �� ������������
   double h2;     // �� ������������
   int    n_group;   // ����� ������ �������� - ���������� h*b
   int    n_profil;  // ���������� ����� ������� � ������ - ���������� t
   int    index_orient;   // ����������: 0 - �� ������; 1 - �� ������
   int    dummy_int;
   double rad;
   double t_red;
   bool flag_concl;
}  SECT_RECT;

typedef struct _sect_rect_solid {
   double b;      // ������ �������
   double h;      // ������ �������
   double b1;     // �� ������������
   double h1;     // �� ������������
   double b2;     // �� ������������
   double h2;     // �� ������������
   int    n_group;   // ����� ������ ��������
   int    n_profil;  // ����� ������� � ������
   int    index_orient;   // ���������� = 0
   int    dummy_int;
   double dummy_double[2];
   bool flag_concl;
}  SECT_RECT_SOLID;

typedef struct _sect_circ {
   double thickness;  // ������� ������
   double diameter;   // �������
   double b1;         // �� ������������
   double h1;         // �� ������������
   double b2;         // �� ������������
   double h2;         // �� ������������
   int    n_group;   // ����� ������ ��������
   int    n_profil;  // ����� ������� � ������
   int    index_orient;   // ���������� = 0
   int    dummy_int;
   double dummy;
   double t_red;
   bool flag_concl;
}  SECT_CIRC;

typedef union _common_sect {   // ���������� ������ � �������
   SECT_DVUTAVR dvutavr;
   SECT_SQUARE  square;
   SECT_RECT    rect;
   SECT_CIRC    circ;
   SECT_RECT_SOLID rect_solid;
}  COMMON_SECT;

typedef struct _data_sect_scad {   // ���������� ������ � ������� ��������
   int  type_sect;
   COMMON_SECT com_sect;
}  DATA_SECT_SCAD;


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

typedef struct _stress_param_sect {   // ��������� ������������ ��������� �������
   double sigma_ext;
   double sigma_w_ext; // ���������� � ������ � ����� ����� � ������� ������
   double sigma_int;
   double sigma_w_int; // ���������� � ������ � ����� ����� � ���������� ������
   double sigma_ct;    // ���������� � ������ � ������ ������� �������
   double Hsect;       // ������ �������
   double Zct;         // ��������� �.�. ������� ������������ ������� �����
   double z0;         // ���������� sigma=0, ������������� �� ������� �����
}  STRESS_PARAM_SECT;

void  Point_dvutavr(int zero, int zero1, int zero2, SECT_DVUTAVR *sect, float scale, TPoint *vertices);

#endif
