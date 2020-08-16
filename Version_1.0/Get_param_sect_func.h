//---------------------------------------------------------------------------

#ifndef Get_param_sect_funcH
#define Get_param_sect_funcH
#include "Data_Profil.h"

//---------------------------------------------------------------------------
// ��������� �������������� ��������� ������������ ������� � ������ ���������� �����������
void Get_param_sect_netto(SECT_DVUTAVR *section, GEOM_PARAM_SECT *param_sect, double *diametr, int *num_hole,
                          double *space, double *step);
                          
void  Put_vecd(int type_section, SECT_DVUTAVR *sect_e, COMMON_SECT *sect_comm, int local, int num_material, int index_weight, bool flag_reverse,
               double size_e, double uklon_h, double *vecd) ;

// ������������� ��������� ����� � ���������� ����������� ������� ������
int  StiffBeamToCommSect(double *gt, COMMON_SECT *sect_comm, int *type_sect, int index_sect, double *h_sten);
// ��������� �������������� ��������� ������������� ������� �� ������ �� �����������
void Get_param_sect_gener(int type_sect, SECT_DVUTAVR* section, GEOM_PARAM_SECT *param_sect);
void Get_param_sect(SECT_DVUTAVR* section, GEOM_PARAM_SECT *param_sect);
void Get_param_sect_comm(int type,COMMON_SECT* section, GEOM_PARAM_SECT *param_sect);
void Get_param_sect_pop(SECT_DVUTAVR* section, double t_pop, double pos_pop, GEOM_PARAM_SECT *param_sect);
// ��������� �������������� ��������� ������������ �������
void Get_param_sect(SECT_DVUTAVR* section, GEOM_PARAM_SECT *param_sect);

#endif
 