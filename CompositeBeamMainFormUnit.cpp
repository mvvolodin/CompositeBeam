//---------------------------------------------------------------------------
#include <vcl.h>
#include<ComObj.hpp>
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include "CompositeBeamMainFormUnit.h"
#include "String_doubleUnit.h"  //������� ����������� ������������ ����� ������ � ���� �����
#include "SteelSectionFormUnit.h"
#include "Report.h" //���������� ��������� �������
#include "Word_Automation.h"  // ����� ������ � Word
#include "AboutProg.h"

TCompositeBeamMainForm *CompositeBeamMainForm;

//----------------------------------------------------------------------
 _fastcall TCompositeBeamMainForm::TCompositeBeamMainForm(TComponent* Owner)
	: TForm(Owner)
{
//:composite_section_(nullptr)
	cotr_ratios_grid();
	cotr_comp_sect_geometr_grid();
	cotr_steel_sect_geometr_grid();
	ctor_concrete_sect_geometr_grid();;
	fill_cmb_bx_impact();
	fill_cmb_bx_corrugated_sheets();
	modify_project = false;
}
//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void __fastcall TCompositeBeamMainForm::FormShow(TObject *Sender)
{
//��� ��� ������� ����� �������� ����� ���������� ������� � ��������, ��� ����� ����������
//������ ���� ���, ��� ���������� ������������ ������ ������ ���� ������ ���� �������� �
//���������� ������� OnShow, ����� ��� ����� ������������� ������� � ����������������.
	RebarDefinitionForm -> register_observer(this);

	NNewClick(Sender);

	SteelSectionForm->SteelSectionDefinitionFrame->RadioGroupGOST57837->ItemIndex=0;
	SteelSectionForm->SteelSectionDefinitionFrame->RadioGroupGOST57837Click(Sender);
	Pnl_SteelSectionViewer->Caption = SteelSectionForm->SteelSectionDefinitionFrame
	->ComboBox_profil->Text;
	DefineSteelForm->ComboBox_steelChange(Sender);
	pnl_steel->Caption = DefineSteelForm->ComboBox_steel->Text;
	pnl_concrete_grade->Caption=ConcreteDefinitionForm->cmb_bx_concrete_grade_list->Text;
	rdgrp_slab_typeClick(Sender);
	pnl_shear_stud_viewer->Caption=StudDefinitionForm->cmb_bx_stud_part_number->Text;
	pnl_rebar_viewer->Caption = RebarDefinitionForm ->get_rebar().get_grade();
	calculate_composite_beam();

}
//---------------------------------------------------------------------------
//������������� ���������
//---------------------------------------------------------------------------
TGeometry TCompositeBeamMainForm::init_geomet()
{
	double beam_division = 0.;//�������� ��� �� double � �������� ������� �������� �������� ���� �� ��� int
	double span = 0.;
	double trib_width_left = 0.;
	double trib_width_right = 0.;

	String_double_plus(lbl_beam_division->Caption, edt_beam_division->Text, &beam_division);
	String_double_plus(lbl_span->Caption, edt_span->Text, &span);
	String_double_plus(lbl_trib_width_left->Caption, edt_width_left->Text, &trib_width_left);
	String_double_plus(lbl_trib_width_right->Caption, edt_width_right->Text, &trib_width_right);

	return TGeometry(static_cast<int>(beam_division),
					 chck_bx_end_beam->Checked,
					 span, trib_width_left,
					 trib_width_right,
					 StrToFloat(cmb_bx_number_propping_supports->Text));
}
//---------------------------------------------------------------------------
//������������� �������� � ������������� ��������� �� ���������
//(��� ������������� SW (������������ ����) ���������� ������������� ��������� I �������- �����������)
//---------------------------------------------------------------------------
TLoads TCompositeBeamMainForm::init_loads()
{
	double SW_sheets = 0.;

	if (rdgrp_slab_type->ItemIndex==1)
		SW_sheets = corrugated_sheets_map[cmb_bx_corrugated_sheeting_part_number->Text].get_weight()* GRAV_ACCELERAT;

	double SW = SteelSectionForm->SteelSectionDefinitionFrame->common_sect_.dvutavr.weight * GRAV_ACCELERAT;

	double DL_I = 0.;
	double DL_II = 0.;
	double LL = 0.;
	double gamma_f_st_SW = 0.;
	double gamma_f_DL_I = 0.;
	double gamma_f_DL_II= 0.;
	double gamma_f_LL = 0.;

	String_double_plus(lbl_dead_load_first_stage->Caption, edt_dead_load_first_stage->Text, &DL_I);
	String_double_plus(lbl_dead_load_second_stage->Caption, edt_dead_load_second_stage->Text, &DL_II);
	String_double_plus(lbl_live_load->Caption, edt_live_load->Text, &LL);
	String_double_plus(lbl_gamma_f_st_SW->Caption, edt_gamma_f_st_SW_->Text, &gamma_f_st_SW);
	String_double_plus(lbl_gamma_f_DL_I->Caption, edt_gamma_f_DL_I->Text, &gamma_f_DL_I);
	String_double_plus(lbl_gamma_f_DL_II->Caption, edt_gamma_f_DL_II->Text, &gamma_f_DL_II);
	String_double_plus(lbl_gamma_f_LL->Caption, edt_gamma_f_LL->Text, &gamma_f_LL);

	return TLoads (SW, SW_sheets, DL_I, DL_II, LL, gamma_f_st_SW, gamma_f_DL_I, gamma_f_DL_II, gamma_f_LL);
}
//---------------------------------------------------------------------------
//������������� ��������� ��������
//---------------------------------------------------------------------------
TISectionInitialData TCompositeBeamMainForm::init_i_section()
{
	return TISectionInitialData (&(SteelSectionForm->SteelSectionDefinitionFrame->common_sect_.dvutavr));
}
//---------------------------------------------------------------------------
//	������������� ��������� ��������
//---------------------------------------------------------------------------
Steel TCompositeBeamMainForm::init_steel_i_section()
{
	STEEL_PARAM my_steel_param;
	TISectionInitialData i_section=init_i_section();//��������� ��� ��������� ����������� ������� ��������

	int rc;
	char title[8] = "";
	double E = 0.;
	double G = 0.;
	double nu = 0.;
	double dens = 0.;
	double gamma_m = 0.;
	double R_yn = 0.;
	double R_un = 0.;

	double t_max = i_section.t_uf_;
	char* str=((AnsiString)DefineSteelForm->ComboBox_steel->Text).c_str();

	bool  flag_diag_thick=false;//��� ���� ���� ����

	rc=Steel_param(str, t_max, &my_steel_param, flag_diag_thick);
	if(rc!=0)
		return Steel();//��� ���������� �� ������� ���� Steel_param ������ ������?
	R_yn =  my_steel_param.Ryn;
	R_un =  my_steel_param.Run;
	strcpy(title, my_steel_param.title);

	String_double_plus(DefineSteelForm->Label3->Caption, DefineSteelForm->Edit_E->Text, &E);
	String_double_zero_plus(DefineSteelForm->Label4->Caption, DefineSteelForm->Edit_G->Text, &G);
	String_double_plus(DefineSteelForm->Label5->Caption, DefineSteelForm->Edit_nu->Text, &nu);
	String_double_zero_plus(DefineSteelForm->Label_gamma_m->Caption, DefineSteelForm->Edit_gamma_m->Text, &gamma_m);

	return Steel(title, E, G, nu, gamma_m,R_yn,R_un);
}
//---------------------------------------------------------------------------
//������������� �������������� ����� �������
//---------------------------------------------------------------------------
TConcretePart* TCompositeBeamMainForm::init_concrete_part()
{
	TGeometry geometry = init_geomet();
	TISectionInitialData init_i_section = &(SteelSectionForm->SteelSectionDefinitionFrame->common_sect_.dvutavr);
	double b_uf = init_i_section.b_uf_;

	if (rdgrp_slab_type->ItemIndex==0)
	{
		double t_sl= 0.;
		String_double_plus(lbl_flat_slab_thickness->Caption, edt_flat_slab_thickness->Text, &t_sl);
		return new TFlatSlab(ConcreteDefinitionForm->get_concrete(),
							 RebarDefinitionForm->get_rebar(),
							 t_sl , geometry, b_uf);
	}
	else
	{
	double h_f= 0.;

	String_double_plus(lbl_h_f->Caption, edt_h_f->Text, &h_f);

	return new TCorrugatedSlab(cmb_bx_corrugated_sheeting_part_number->Text,
								  ConcreteDefinitionForm->get_concrete(),
								  RebarDefinitionForm->get_rebar(),
								  h_f, geometry, b_uf);
	}
}
//---------------------------------------------------------------------------
//������������� �������� ����� �������
//---------------------------------------------------------------------------
SteelPart TCompositeBeamMainForm::init_steel_part()
{

   TISectionInitialData i_section_initial_data=init_i_section();
   Steel steel_i_section=init_steel_i_section();

	return SteelPart(ISection(i_section_initial_data), steel_i_section);
}
//---------------------------------------------------------------------------
//	������������� ������
//---------------------------------------------------------------------------
Studs TCompositeBeamMainForm::init_stud()
{
	String name="";
	double d_an=0.;
	double l=0.;
	double gamma_c=0.;
	double R_y=0.;

	double edge_studs_dist=0.;
	double middle_studs_dist=0.;
	double edge_studs_rows_num=0.;
	double middle_studs_rows_num=0.;

	name=StudDefinitionForm->cmb_bx_stud_part_number->Text;
	d_an=StrToFloat(StudDefinitionForm->edt_stud_diameter->Text); //��������� long double 10 ����
	l=StrToFloat(StudDefinitionForm->edt_stud_height->Text);
	R_y=StrToFloat(StudDefinitionForm->edt_stud_yield_strength->Text);
	String_double_plus(StudDefinitionForm->lbl_stud_safety_factor->Caption,
						  StudDefinitionForm->edt_stud_safety_factor->Text,
						  &gamma_c);

	String_double_plus(lbl_edge_studs_dist->Caption, edt_edge_studs_dist->Text, &edge_studs_dist);
	String_double_plus(lbl_middle_studs_dist->Caption, edt_middle_studs_dist->Text, &middle_studs_dist);

	edge_studs_rows_num=StrToFloat(cmb_bx_edge_studs_rows_num->Text);
	middle_studs_rows_num=StrToFloat(cmb_bx_middle_studs_rows__num->Text);

	 return Studs(name, d_an, l,
				edge_studs_dist, middle_studs_dist,
				edge_studs_rows_num, middle_studs_rows_num,
				R_y, gamma_c); ;
}
//---------------------------------------------------------------------------
//	������������� ������������� ������� ������
//---------------------------------------------------------------------------
 WorkingConditionsFactors TCompositeBeamMainForm::init_working_conditions_factors()
 {
	double gamma_bi=0.;
	double gamma_si=0.;
	double gamma_c=0.;

	String_double_plus(lbl_gamma_bi->Caption, edt_gamma_bi->Text, &gamma_bi);
	String_double_plus(lbl_gamma_si->Caption, edt_gamma_si->Text, &gamma_si);
	String_double_plus(lbl_gamma_c->Caption, edt_gamma_c->Text, &gamma_c);

	return WorkingConditionsFactors(gamma_bi,gamma_si,gamma_c);
 }

// ---------------------------------------------------------------------------
// ������������� ����������� �����
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::init_composite_beam(AnalysisTheory           analysis_theory,
												 TGeometry 				  geometry,
												 TLoads 				  loads,
												 CompositeSection 		  composite_section,
												 Studs 					  stud,
												 WorkingConditionsFactors working_conditions_factors)
{
 composite_beam_=TCompositeBeam(analysis_theory,
								geometry,
								loads,
								composite_section,stud,
								working_conditions_factors);
}
//---------------------------------------------------------------------------
//	������� ����������� ������ ����������� �����
//---------------------------------------------------------------------------
void __fastcall TCompositeBeamMainForm::BtnCalculateClick(TObject *Sender)
{
	calculate_composite_beam();
}
//---------------------------------------------------------------------------
//������������ � ������� �����
//---------------------------------------------------------------------------
void __fastcall TCompositeBeamMainForm::btn_reportClick(TObject *Sender)
{
	Screen->Cursor = crHourGlass;//�� ����� �������� ������ �������� ������� ��� �����
	generate_report();
	Screen->Cursor = crDefault;//���������� ������� ��� �� ���������
}
//---------------------------------------------------------------------------
//	��������� ��������� ��� �������� ������ � �������� ������� � �����������
//	�� ���������� ����: ����� �� ������� ��� ������� �����
//---------------------------------------------------------------------------
void __fastcall TCompositeBeamMainForm::rdgrp_slab_typeClick(TObject *Sender)
{
	switch(rdgrp_slab_type->ItemIndex)
	{
		case 0: grp_bx_corrugated_slab->Visible=false;
				grp_bx_flat_slab->Visible=true;
				Image2->Visible=false;
				Image1->Visible=true;
				break;

	   case 1:	grp_bx_corrugated_slab->Visible=true;
				grp_bx_flat_slab->Visible=false;
				Image2->Visible=true;
				Image1->Visible=false;
				break;
	}
		OnControlsChange(Sender);
}
//---------------------------------------------------------------------------
//���������� ������� �������������� ���������� ������ ������ ������ �������
//---------------------------------------------------------------------------
void __fastcall TCompositeBeamMainForm::strng_grd_rendering(TObject *Sender,
																int ACol, int ARow,
																TRect &Rect, TGridDrawState State)
{
	TStringGrid* str_grid=static_cast<TStringGrid*>(Sender);
	if (ARow ==0)
	{
		str_grid->Canvas->Font->Style=TFontStyles()<< fsBold;
		str_grid->Canvas->Font->Style<<fsBold;
		str_grid->Canvas->FillRect(Rect);
		str_grid->Canvas->TextOut(Rect.Left+3, Rect.Top+5, str_grid->Cells[ACol][ARow]);
	}
}
//---------------------------------------------------------------------------
//	������� ����������� TStringGrid ��������� ���������� ������� �������������� ������������� ������������ �������
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::cotr_comp_sect_geometr_grid()
{
	strng_grd_compos_sect_geom_character->Cells [0][0]=L"�������������� ��������������";
	strng_grd_compos_sect_geom_character->Cells [1][0]=L"��������";
	strng_grd_compos_sect_geom_character->Cells [0][1]=L"������� Ared, ��2";
	strng_grd_compos_sect_geom_character->Cells [0][2]=L"������ ������� Ired, ��4 ";
	strng_grd_compos_sect_geom_character->Cells [0][3]=L"������ �������������, �.�. �.�. ����� Wb,red, ��3";
	strng_grd_compos_sect_geom_character->Cells [0][4]=L"������ �������������, �����. ����� Wf2,red, ��3";
	strng_grd_compos_sect_geom_character->Cells [0][5]=L"������ �������������, ����. ����� Wf1,red, ��3";
	strng_grd_compos_sect_geom_character->Cells [0][6]=L"���������� Zb,red, ��";
	strng_grd_compos_sect_geom_character->Cells [0][7]=L"���������� Zf2,red ��";
	strng_grd_compos_sect_geom_character->Cells [0][8]=L"���������� Zf1,red ��";
}
//---------------------------------------------------------------------------
//������� ����������� ������ TStringGrid ��������������� ���������������� ��������� �������
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::cotr_steel_sect_geometr_grid()
{
	strng_grd_steel_sect_geom_character->Cells [0][0]=L"�������������� ��������������";
	strng_grd_steel_sect_geom_character->Cells [1][0]=L"��������";
	strng_grd_steel_sect_geom_character->Cells [0][1]=L"�������";
	strng_grd_steel_sect_geom_character->Cells [0][2]=L"������ �������";
	strng_grd_steel_sect_geom_character->Cells [0][3]=L"������ ������������� ������� ������� ������� �����";
	strng_grd_steel_sect_geom_character->Cells [0][4]=L"������ ������������� ������� ������� ������ �����";
	strng_grd_steel_sect_geom_character->Cells [0][5]=L"���������� �� �.�. �� �������� ����� ������� �����";
	strng_grd_steel_sect_geom_character->Cells [0][6]=L"���������� �� �.�. �� �������� ����� ������ �����";
}
//---------------------------------------------------------------------------
//������� ����������� ������ TStringGrid ��������������� ���������������� ��������������� �������
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::ctor_concrete_sect_geometr_grid()
{
	strng_grd_concrete_sect_geom_character->Cells [0][0]=L"�������������� ��������������";
	strng_grd_concrete_sect_geom_character->Cells [1][0]=L"��������";
	strng_grd_concrete_sect_geom_character->Cells [0][1]=L"��������� �������� ����� �����";
	strng_grd_concrete_sect_geom_character->Cells [0][2]=L"��������� �������� ����� ������";
	strng_grd_concrete_sect_geom_character->Cells [0][3]=L"���������� �� ������ ����� �� ������ �������";
	strng_grd_concrete_sect_geom_character->Cells [0][4]=L"�������";
	strng_grd_concrete_sect_geom_character->Cells [0][5]=L"������ �������";
   //	strng_grd_concrete_sect_geom_character->Cells [0][6]=L"������ �������������";
}
void TCompositeBeamMainForm::fill_steel_sect_geometr_grid()
{
	ISection i_section= composite_beam_.get_composite_section().get_steel_part().get_I_section();

	strng_grd_steel_sect_geom_character->Cells [1][1]=FloatToStrF(i_section.get_A_st(), ffFixed, 15, 0);
	strng_grd_steel_sect_geom_character->Cells [1][2]=FloatToStrF(i_section.get_I_st(), ffFixed, 15, 0);
	strng_grd_steel_sect_geom_character->Cells [1][3]=FloatToStrF(i_section.get_Wf2_st(), ffFixed, 15, 0);
	strng_grd_steel_sect_geom_character->Cells [1][4]=FloatToStrF(i_section.get_Wf1_st(), ffFixed, 15, 0);
	strng_grd_steel_sect_geom_character->Cells [1][5]=FloatToStrF(i_section.get_Z_f2_st(), ffFixed, 15, 0);
	strng_grd_steel_sect_geom_character->Cells [1][6]=FloatToStrF(i_section.get_Z_f1_st(), ffFixed, 15, 0);
}
void TCompositeBeamMainForm::fill_concrete_sect_geometr_grid()
{
	TConcretePart* concrete_part=composite_beam_.get_composite_section().get_concrete_part();

	strng_grd_concrete_sect_geom_character->Cells [1][1]=FloatToStrF(concrete_part->get_b_l(), ffFixed, 15, 0);
	strng_grd_concrete_sect_geom_character->Cells [1][2]=FloatToStrF(concrete_part->get_b_r(), ffFixed, 15, 0);
	strng_grd_concrete_sect_geom_character->Cells [1][3]=FloatToStrF(concrete_part->get_C_b(), ffFixed, 15, 0);
	strng_grd_concrete_sect_geom_character->Cells [1][4]=FloatToStrF(concrete_part->get_A_b(), ffFixed, 15, 0);
	strng_grd_concrete_sect_geom_character->Cells [1][5]=FloatToStrF(concrete_part->get_I_b(), ffFixed, 15, 0);
}
void TCompositeBeamMainForm::fill_composite_sect_geometr_grid()
{
	double A_red=composite_beam_.get_composite_section().get_A_red();
	double I_red=composite_beam_.get_composite_section().get_I_red();
	double W_b_red=composite_beam_.get_composite_section().get_W_b_red();
	double W_f2_red=composite_beam_.get_composite_section().get_W_f2_red();
	double W_f1_red=composite_beam_.get_composite_section().get_W_f1_red();
	double Z_b_red=composite_beam_.get_composite_section().get_Z_b_red();
	double Z_f2_red=composite_beam_.get_composite_section().get_Z_f2_red();
	double Z_f1_red=composite_beam_.get_composite_section().get_Z_f1_red();

	strng_grd_compos_sect_geom_character->Cells [1][1]=FloatToStrF(A_red, ffFixed, 15, 0); //��������� �������� �������� ���
	strng_grd_compos_sect_geom_character->Cells [1][2]=FloatToStrF(I_red, ffFixed, 15, 0); //���� double 15
	strng_grd_compos_sect_geom_character->Cells [1][3]=FloatToStrF(W_b_red, ffFixed, 15, 0);
	strng_grd_compos_sect_geom_character->Cells [1][4]=FloatToStrF(std::abs(W_f2_red), ffFixed, 15, 0);
	strng_grd_compos_sect_geom_character->Cells [1][5]=FloatToStrF(W_f1_red, ffFixed, 15, 0);
	strng_grd_compos_sect_geom_character->Cells [1][6]=FloatToStrF(Z_b_red, ffFixed, 15, 0);
	strng_grd_compos_sect_geom_character->Cells [1][7]=FloatToStrF(std::abs(Z_f2_red), ffFixed, 15, 0);
	strng_grd_compos_sect_geom_character->Cells [1][8]=FloatToStrF(Z_f1_red, ffFixed, 15, 0);
}

void TCompositeBeamMainForm::fill_results_grid()
{
	double max_upper_flange_ratio = composite_beam_.get_max_upper_flange_ratio();
	double max_lower_flange_ratio = composite_beam_.get_max_lower_flange_ratio();
	double ratio_rigid_plastic = composite_beam_.get_ratio_rigid_plastic();
	double max_ratio_studs = composite_beam_.get_max_stud_ratio();
	double max_shear_ratio = composite_beam_.get_max_shear_ratio();
	
	strng_grd_results->Cells [1][1]=FloatToStrF(std::abs(max_upper_flange_ratio), ffFixed, 15, 2);
	strng_grd_results->Cells [1][2]=FloatToStrF(std::abs(max_lower_flange_ratio), ffFixed, 15, 2);
	strng_grd_results->Cells [1][3]=FloatToStrF(std::abs(max_shear_ratio), ffFixed, 15, 2);
	strng_grd_results->Cells [1][4]=FloatToStrF(std::abs(max_ratio_studs), ffFixed, 15, 2);
	strng_grd_results->Cells [1][5]=FloatToStrF(ratio_rigid_plastic, ffFixed, 15, 2);

}
//---------------------------------------------------------------------------
//	������� ����������� Grid ��������� ���������� ������� ����������� �����
//---------------------------------------------------------------------------
void TCompositeBeamMainForm:: cotr_ratios_grid()
{
	strng_grd_results->Cells [0][0]="��������";
	strng_grd_results->Cells [1][0]="������������ ������������� (��) ";
	strng_grd_results->Cells [0][1]="������� ���� ��������� �������, ������ 6.2.1";
	strng_grd_results->Cells [0][2]="������ ���� ��������� ��������, ������ 6.2.1";
	strng_grd_results->Cells [0][3]="���������� ����, ������ 6.2.2";
	strng_grd_results->Cells [0][4]="��������� ������ �����������, ������ 9.1.2";
	strng_grd_results->Cells [0][5]="��������� ����� ��� ���������������� ��������� ������ 6.2.1.6";


}
//---------------------------------------------------------------------------
//	������� ����������� ComboBox �������� ����������
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::fill_cmb_bx_impact()
{
	cmb_bx_impact->Items->Insert(static_cast<int>(Impact::SW_BEAM), "����������� ��� �����");
	cmb_bx_impact->Items->Insert(static_cast<int>(Impact::SW_SHEETS), "����������� ��� �������");
	cmb_bx_impact->Items->Insert(static_cast<int>(Impact::DL_I) , "���������� �������� I ������");
	cmb_bx_impact->Items->Insert(static_cast<int>(Impact::DL_II), "���������� �������� II ������");
	cmb_bx_impact->Items->Insert(static_cast<int>(Impact::LL), "��������� �������� II ������");
	cmb_bx_impact->Items->Insert(static_cast<int>(Impact::I_stage), "��������� �������� I ������");
	cmb_bx_impact->Items->Insert(static_cast<int>(Impact::II_stage), "��������� �������� II ������");
	cmb_bx_impact->Items->Insert(static_cast<int>(Impact::Total), "��������� ��������");
	cmb_bx_impact->ItemIndex = (int)Impact::SW_BEAM;
}
//---------------------------------------------------------------------------
//	������� ����������� ComboBox ���������
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::fill_cmb_bx_corrugated_sheets()
{
	for(auto corrugated_sheet: corrugated_sheets_map)
	cmb_bx_corrugated_sheeting_part_number->Items->Add(corrugated_sheet.first);
	cmb_bx_corrugated_sheeting_part_number->ItemIndex = 0;

}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm::BtBtnSteelChoiceClick(TObject *Sender)
{
	 //SteelDefinitionForm->ShowModal();
	 //PnlSteelViewer->Caption = SteelDefinitionForm->ComboBox_steel->Text;
	 DefineSteelForm->ShowModal();
	 pnl_steel->Caption = DefineSteelForm->ComboBox_steel->Text;

}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm::BtnSteelSectionChoiceClick(TObject *Sender)
{
	SteelSectionForm->ShowModal();
	Pnl_SteelSectionViewer->Caption = SteelSectionForm->SteelSectionDefinitionFrame->ComboBox_profil->Text;
}
//---------------------------------------------------------------------------


void __fastcall TCompositeBeamMainForm::BtBtnExitClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm::BtnConcreteChoiceClick(TObject *Sender)
{
	ConcreteDefinitionForm->ShowModal();//����� �������� ����� ����������� ��������� ������� ��������������� �������� ComboBox � Panel
	pnl_concrete_grade->Caption = ConcreteDefinitionForm->cmb_bx_concrete_grade_list->Text;
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm::BtBtnRebarsChoiceClick(TObject *Sender)
{
	RebarDefinitionForm->ShowModal();
	//������������� ������ �� pnl � ��������� ������
	if(pnl_rebar_viewer->Caption!=RebarDefinitionForm->get_rebar().get_grade())
		pnl_rebar_viewer->Caption=RebarDefinitionForm->get_rebar().get_grade();
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm::BtBtnShearStudsChoiceClick(TObject *Sender)
{
	StudDefinitionForm->ShowModal();
	//������������� ������ �� pnl � ��������� ������
   //	if(pnl_shear_stud_viewer->Caption!=StudDefinitionForm->get_stud().get_name())
	 //	pnl_shear_stud_viewer->Caption=StudDefinitionForm->get_stud().get_name();
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm::NOutReportClick(TObject *Sender)
{
	generate_report();
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm::NExitClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
//�������� ������
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::generate_report()
{
	TWord_Automation report_=TWord_Automation("ReportCompositeBeam.docx");
	TGeometry geometry=composite_beam_.get_geometry();
	TLoads loads=composite_beam_.get_loads();
	CompositeSection composite_section=composite_beam_.get_composite_section();
	ISection i_section= composite_beam_.get_composite_section().get_steel_part().get_I_section();
	TConcretePart* concrete_part=composite_beam_.get_composite_section().get_concrete_part();
	Concrete concrete=concrete_part->get_concrete();
	Rebar rebar=concrete_part->get_rebar();
	Steel steel=composite_beam_.get_composite_section().get_steel_grade();
	Studs studs=composite_beam_.get_studs();
	WorkingConditionsFactors working_conditions_factors=composite_beam_.get_working_conditions_factors();

//[1.1] ���������
	report_.PasteTextPattern(geometry.is_end_beam_to_str(), "%end_beam%");
	report_.PasteTextPattern(FloatToStr(geometry.get_span(LengthUnit::mm)), "%span%");
	report_.PasteTextPattern(FloatToStr(geometry.get_trib_width_left(LengthUnit::mm)), "%trib_width_left% ");
	report_.PasteTextPattern(FloatToStr(geometry.get_trib_width_right(LengthUnit::mm)), "%trib_width_right% ");
//[1.2] ����������
	report_.PasteTextPattern(FloatToStr(loads.get_dead_load_first_stage(LoadUnit::kN, LengthUnit::m)), "%DL_I%");
	report_.PasteTextPattern(FloatToStr(loads.get_dead_load_second_stage(LoadUnit::kN, LengthUnit::m)), "%DL_II%");
	report_.PasteTextPattern(FloatToStr(loads.get_live_load(LoadUnit::kN, LengthUnit::m)), "%LL%");

//[1.3] ������������ ��������� �� ��������
	report_.PasteTextPattern(FloatToStr(loads.get_gamma_f_st_SW()), "%gamma_f_st_SW%");
	report_.PasteTextPattern(FloatToStr(loads.get_gamma_f_DL_I()), "%gamma_f_DL_I%");
	report_.PasteTextPattern(FloatToStr(loads.get_gamma_f_DL_II()), "%gamma_f_DL_II%");
	report_.PasteTextPattern(FloatToStr(loads.get_gamma_f_LL()), "%gamma_f_LL%");
//[1.4] �������� �������
//[1.4.1] ����������� ������� ��������
	report_.PasteTextPattern(i_section.get_profile_number(),"%profile_number%");
	report_.PasteTextPattern(i_section.get_h_st(LengthUnit::mm),"%h%");
	report_.PasteTextPattern(0,"%h%");
	report_.PasteTextPattern(i_section.get_b_uf(LengthUnit::mm),"%b%");
	report_.PasteTextPattern(0,"%b_w%");
	report_.PasteTextPattern(i_section.get_t_uf(LengthUnit::mm),"%t%");
	report_.PasteTextPattern(i_section.get_t_w(LengthUnit::mm),"%s%");
	report_.PasteTextPattern(i_section.get_r(LengthUnit::mm),"%r%");
//[1.4.2] �������������� �����
	report_.PasteTextPattern(steel.get_steel_grade(),"%steel_grade%");
	report_.PasteTextPattern(steel.get_R_yn(),"%R_yn%");
	report_.PasteTextPattern(steel.get_R_un(),"%R_un%");
	report_.PasteTextPattern(steel.get_E_st(),"%E_st%");
	report_.PasteTextPattern(steel.get_G_st(),"%G_st%");
	report_.PasteTextPattern(steel.get_nu_st(),"%nu_st%");
	report_.PasteTextPattern(steel.get_gamma_m(),"%gamma_m%");
//[1.5] �������������� �������
//[1.5.1] ����������� ������� �����
	report_.PasteTextPattern(concrete_part->get_slab_type(),"%slab_type%");
	report_.PasteTextPattern(concrete_part->get_h_b(),"%t_sl%");

//[1.5.2] �������������� ������

	report_.PasteTextPattern(concrete.get_grade(),"%conc_grade%");
	report_.PasteTextPattern(concrete.get_R_bn(),"%R_bn%");
	report_.PasteTextPattern(concrete.get_R_btn(),"%R_btn%");
	report_.PasteTextPattern(concrete.get_E_b(),"%E_b%");
	report_.PasteTextPattern(concrete.get_gamma_b(),"%gamma_b%");
	report_.PasteTextPattern(concrete.get_gamma_bt(),"%gamma_bt%");


//[1.6] ��������
	report_.PasteTextPattern(rebar.get_grade(),"%grade%");
	report_.PasteTextPattern(rebar.get_diameter(),"%d%");
	report_.PasteTextPattern(rebar.get_R_sn(),"%R_sn%");
//[1.7] �������������� ��������
	report_.PasteTextPattern(studs.get_name(),"%name%");
	report_.PasteTextPattern(studs.get_l(LengthUnit::cm),"%l%");
	report_.PasteTextPattern(studs.get_d_an(LengthUnit::cm),"%d_an%");
	report_.PasteTextPattern(studs.get_R_y(),"%R_y%");
	report_.PasteTextPattern(studs.get_edge_rows_dist(LengthUnit::cm),"%z_e%");
	report_.PasteTextPattern(studs.get_middle_rows_dist(LengthUnit::cm),"%z_m%");
	report_.PasteTextPattern(studs.get_edge_rows_num(),"%ed_rw_num%");
	report_.PasteTextPattern(studs.get_middle_rows_num(),"%mid_rw_num%");
//[1.8] ������������
	report_.PasteTextPattern(working_conditions_factors.get_gamma_c(),"%gamma_c%");
	report_.PasteTextPattern(working_conditions_factors.get_gamma_bi(),"%gamma_bi%");
	report_.PasteTextPattern(working_conditions_factors.get_gamma_si(),"%gamma_si%");
//[1.9] ������
	report_.PasteTextPattern(composite_beam_.get_analysis_theory(),"%analys_theory%");
	report_.PasteTextPattern(geometry.get_temporary_supports_number(),"%temp_supp%");


//[2] ���������� �������
//[2.1] ��������� �������


//[2.2] �������������� ���������
//[2.2.1] ��������� �������
	report_.PasteTextPattern(i_section.get_A_st(LengthUnit::cm),"%A_st%");
	report_.PasteTextPattern(i_section.get_I_st(LengthUnit::cm),"%I_st%");
	report_.PasteTextPattern(i_section.get_Wf2_st(LengthUnit::cm),"%Wf2_st%");
	report_.PasteTextPattern(i_section.get_Wf1_st(LengthUnit::cm),"%Wf1_st%");
	report_.PasteTextPattern(i_section.get_Z_f2_st(LengthUnit::cm),"%Z_f2_st%");
	report_.PasteTextPattern(i_section.get_Z_f1_st(LengthUnit::cm),"%Z_f1_st%");
//[2.2.2] ��������������� �������
	report_.PasteTextPattern(concrete_part->get_slab_type(),"%slab_type%");
	report_.PasteTextPattern(concrete_part->get_b_l(LengthUnit::cm),"%b_l%");
	report_.PasteTextPattern(concrete_part->get_b_r(LengthUnit::cm),"%b_r%");
	report_.PasteTextPattern(concrete_part->get_C_b(LengthUnit::cm),"%C_b%");
	report_.PasteTextPattern(concrete_part->get_A_b(LengthUnit::cm),"%A_b%");
	report_.PasteTextPattern(concrete_part->get_I_b(LengthUnit::cm),"%I_b%");
//[2.2.3] ������������ �������
	report_.PasteTextPattern(std::round(composite_section.get_A_red(LengthUnit::cm)),"%A_red%");
	report_.PasteTextPattern(std::round(composite_section.get_I_red(LengthUnit::cm)),"%I_red%");
	report_.PasteTextPattern(std::round(composite_section.get_W_b_red(LengthUnit::cm)),"%W_b_red%");
	report_.PasteTextPattern(std::abs(std::round(composite_section.get_W_f2_red(LengthUnit::cm))),"%W_f2_red%");
	report_.PasteTextPattern(std::round(composite_section.get_W_f1_red(LengthUnit::cm)),"%W_f1_red%");
	report_.PasteTextPattern(std::round(composite_section.get_Z_b_red(LengthUnit::cm)),"%Z_b_red%");
	report_.PasteTextPattern(std::abs(std::round(composite_section.get_Z_f2_red(LengthUnit::cm))),"%Z_f2_red%");
	report_.PasteTextPattern(std::round(composite_section.get_Z_f1_red(LengthUnit::cm)),"%Z_f1_red%");


}

//---------------------------------------------------------------------------
// ��������� ����
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::draw_diagram()
{
	TImage *Image1=img_static_scheme;
	//�������� ������ ��������� ����� ����� �� ������� ����������� �����
	std::vector<double> coor_epur=composite_beam_.get_CS_coordinates();

	InternalForces internal_forces = composite_beam_.get_internal_forces_LC()[static_cast<Impact>(cmb_bx_impact->ItemIndex)];
	//��������� ��������� ���������� ���� (��������� � ����������)
	int n_supp=composite_beam_.get_geometry().get_permanent_supports_number()+
	internal_forces.get_num_temp_supports();
	std::vector<double>	coor_supp = internal_forces.get_supports_coordinates();
	//���� ��������� �������� �� �����
	bool flag_sign=true;

	if (rd_grp_internal_forces_type->ItemIndex==0)
	{
		//�������� ������ ���������� �������� �� ������� ����������� �����
		std::vector<double> M= internal_forces .get_M(LoadUnit::kN, LengthUnit::m);
		//����������� ������ ��� ������. �������� ���� ��������� �� ��������������� � �������� �� �������� ����� ����� �������
		std::transform(M.begin(),M.end(), M.begin(), [](double M) { return -1*std::round(M*1000)/1000;});
		DrawEpur(Image1, M.size(), &coor_epur[0], &M[0], nullptr, n_supp, &coor_supp[0], flag_sign);
	}
	else
	{
		//�������� ���������� ���� �� ������� ����������� �����
		std::vector<double> Q=internal_forces .get_Q(LoadUnit::kN);
		std::vector<double> Q_jump=internal_forces .get_Q_jump(LoadUnit::kN);
		//����������� ������ ��� ������. �������� ���� ��������� �� ��������������� � �������� �� �������� ����� ����� �������
		std::transform(Q.begin(),Q.end(), Q.begin(), [](double Q) { return -1*std::round(Q*1000)/1000;});
		std::transform(Q_jump.begin(),Q_jump.end(), Q_jump.begin(), [](double Q_jump) { return -1*std::round(Q_jump*1000)/1000;});
		DrawEpur(Image1, Q.size(), &coor_epur[0], &Q[0], &Q_jump[0], n_supp, &coor_supp[0], flag_sign);
	}
}

void __fastcall TCompositeBeamMainForm::cmb_bx_impactChange(TObject *Sender)
{
	draw_diagram();
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm::rd_grp_internal_forces_typeClick(TObject *Sender)

{
	draw_diagram();
}

AnalysisTheory TCompositeBeamMainForm::get_analysis_theory()
{
	if(cmb_bx_analysis_theory->Text==L"������-������������")
		return AnalysisTheory::ELASTO_PLASTCIC;
	else
		return AnalysisTheory::RIGID;
}

void TCompositeBeamMainForm::calculate_composite_beam()
{
   //�������� ������� � � ������ ��� ������� ����� ������ ����������
   //�������� � nullptr  � concrete_section_
   //���������� ��� ������� � ����

   AnalysisTheory analysis_theory=get_analysis_theory();
   TGeometry geometry=init_geomet();//���� ���������� ���������
   TLoads loads=init_loads();//���� ���������� �������� � ������������ ��������� �� ���������
   Studs stud=init_stud(); //���� ��������� ����� ��������
   WorkingConditionsFactors working_conditions_factors=init_working_conditions_factors();
   SteelPart steel_part = init_steel_part();
   TConcretePart* concrete_part=init_concrete_part();//������ ������������ ������, ������� ���������!
   CompositeSection composite_section = CompositeSection(steel_part, concrete_part, geometry);
   init_composite_beam(analysis_theory, geometry,loads,composite_section, stud,working_conditions_factors);
//����� ����������� ������� � GUI
	draw_diagram();
	fill_steel_sect_geometr_grid();
	fill_concrete_sect_geometr_grid();
	fill_composite_sect_geometr_grid();
	fill_results_grid();

//����� ����������� ������� � �����
	btn_report->Enabled=true;
	tb_results->TabVisible=true;
}
//---------------------------------------------------------------------------



void __fastcall TCompositeBeamMainForm::NNewClick(TObject *Sender)
{
	int i;
	if (modify_project) {
		 i=Application->MessageBox(L"��������� ������� ������?", L" ",
				  MB_YESNO | MB_ICONQUESTION);
		 if (i==IDYES) NSaveClick(Sender);
	}
	strcpy(ModelFile, UNTITLED);
	modify_project = false;

	Caption = "������ ��������������� ����� - [����� ������]";

}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm::NSaveClick(TObject *Sender)
{
   // ��������� ����� ����������, � ������� ��������� ����������� ������

   if  (strcmp(ModelFile, UNTITLED)==0) {
	  if(SaveDialog_Model->Execute())
	  {
		  FileDir_Name = SaveDialog_Model->FileName;;//���� ����� ������ ���� ������
		  ModelName(FileDir_Name.c_str(), ModelFile); //�������� ��� ����� ��� ����������� �� ����� �� ����
	  }
   }
	   //�������� ������� �������� ������������ ����������� ������
	save_controls_to_file();

   Caption = "������ ��������������� ����� - " + AnsiString(ModelFile);

   modify_project = false;
}
//---------------------------------------------------------------------------
//��������� ��������� ��������� ���������� � ����
//---------------------------------------------------------------------------
void __fastcall TCompositeBeamMainForm::save_controls_to_file()
{
	TFileStream* fs;

	try
	{
		fs = new TFileStream(FileDir_Name, fmCreate);
		//���������
		fs->WriteComponent(edt_span);
		fs->WriteComponent(edt_width_left);
		fs->WriteComponent(edt_width_right);
		fs->WriteComponent(chck_bx_end_beam);
		fs->WriteComponent(lbl_trib_width_left);
		fs->WriteComponent(lbl_trib_width_right);
		//����������
		fs->WriteComponent(edt_dead_load_first_stage);
		fs->WriteComponent(edt_dead_load_second_stage);
		fs->WriteComponent(edt_live_load);
		//������������ ��������� �� ���������
		fs->WriteComponent(edt_gamma_f_st_SW_);
		fs->WriteComponent(edt_gamma_f_DL_I);
		fs->WriteComponent(edt_gamma_f_DL_II);
		fs->WriteComponent(edt_gamma_f_LL);
		//������������ ������� �����
		fs->WriteComponent(edt_gamma_c);
		fs->WriteComponent(edt_gamma_bi);
		fs->WriteComponent(edt_gamma_si);
		//������ �������
		fs->WriteComponent(cmb_bx_analysis_theory);
		//������ �����
		fs->WriteComponent(edt_edge_studs_dist);
		fs->WriteComponent(edt_middle_studs_dist);
		fs->WriteComponent(cmb_bx_edge_studs_rows_num);
		fs->WriteComponent(cmb_bx_middle_studs_rows__num);
		//������
		fs->WriteComponent(cmb_bx_number_propping_supports);
		//��������� �����
		fs->WriteComponent(cmb_bx_impact);
		//������� ������
		fs->WriteComponent(rd_grp_internal_forces_type);
		//����� �����
		fs->WriteComponent(pnl_steel);
		fs->WriteComponent(DefineSteelForm->ComboBox_gost);
		fs->WriteComponent(DefineSteelForm->ComboBox_steel);
		fs->WriteComponent(DefineSteelForm->Edit_E);
		fs->WriteComponent(DefineSteelForm->Edit_G);
		fs->WriteComponent(DefineSteelForm->Edit_nu);
		fs->WriteComponent(DefineSteelForm->Edit_gamma_m);
		fs->WriteComponent(DefineSteelForm->StringGrid_Prop);
		//����� �����
		fs->WriteComponent(pnl_concrete_grade);
		fs->WriteComponent(ConcreteDefinitionForm->cmb_bx_concrete_grade_list);
		fs->WriteComponent(ConcreteDefinitionForm->edt_R_bn);
		fs->WriteComponent(ConcreteDefinitionForm->edt_R_btn);
		fs->WriteComponent(ConcreteDefinitionForm->edt_E_b);
		fs->WriteComponent(ConcreteDefinitionForm->edt_gamma_b);
		fs->WriteComponent(ConcreteDefinitionForm->edt_gamma_bt);
		fs->WriteComponent(ConcreteDefinitionForm->edt_epsilon_b_lim);
		//����� ��������
		fs->WriteComponent(pnl_rebar_viewer);
		fs->WriteComponent(RebarDefinitionForm->cmb_bx_rebar_grade);
		fs->WriteComponent(RebarDefinitionForm->edt_R_s_n);
		fs->WriteComponent(RebarDefinitionForm->edt_diameter);
		fs->WriteComponent(RebarDefinitionForm->edt_safety_factor);
		//����� ������ �����
		fs->WriteComponent(pnl_shear_stud_viewer);
		fs->WriteComponent(StudDefinitionForm->cmb_bx_stud_part_number);
		fs->WriteComponent(StudDefinitionForm->edt_stud_diameter);
		fs->WriteComponent(StudDefinitionForm->edt_stud_height);
		fs->WriteComponent(StudDefinitionForm->edt_stud_safety_factor);
		//����� ��������� �������
		fs->WriteComponent(SteelSectionForm->SteelSectionDefinitionFrame->ComboBox_profil);
        fs->WriteComponent(Pnl_SteelSectionViewer);

		//��� �������������� �����
		fs->WriteComponent(rdgrp_slab_type);
		fs->WriteComponent(cmb_bx_corrugated_sheeting_part_number);
		fs->WriteComponent(edt_h_f);
		fs->WriteComponent(edt_flat_slab_thickness);

	}
	__finally
	{
		delete fs;
	}
}
//---------------------------------------------------------------------------
//��������� ��������� ��������� ���������� �� �����
//---------------------------------------------------------------------------
void __fastcall TCompositeBeamMainForm::load_controls_from_file()
{
	TFileStream* fs;

	try
	{
		fs = new TFileStream(FileDir_Name, fmOpenRead);
		//���������
		fs->ReadComponent(edt_span);
		fs->ReadComponent(edt_width_left);
		fs->ReadComponent(edt_width_right);
		fs->ReadComponent(chck_bx_end_beam);
		fs->ReadComponent(lbl_trib_width_left);
		fs->ReadComponent(lbl_trib_width_right);
		//����������
		fs->ReadComponent(edt_dead_load_first_stage);
		fs->ReadComponent(edt_dead_load_second_stage);
		fs->ReadComponent(edt_live_load);
		//������������ ��������� �� ���������
		fs->ReadComponent(edt_gamma_f_st_SW_);
		fs->ReadComponent(edt_gamma_f_DL_I);
		fs->ReadComponent(edt_gamma_f_DL_II);
		fs->ReadComponent(edt_gamma_f_LL);
		//������������ ������� �����
		fs->ReadComponent(edt_gamma_c);
		fs->ReadComponent(edt_gamma_bi);
		fs->ReadComponent(edt_gamma_si);
		//������ �������
		fs->ReadComponent(cmb_bx_analysis_theory);
		//������ �����
		fs->ReadComponent(edt_edge_studs_dist);
		fs->ReadComponent(edt_middle_studs_dist);
		fs->ReadComponent(cmb_bx_edge_studs_rows_num);
		fs->ReadComponent(cmb_bx_middle_studs_rows__num);
		//������
		fs->ReadComponent(cmb_bx_number_propping_supports);
		//��������� �����
		fs->ReadComponent(cmb_bx_impact);
		//������� ������
		fs->ReadComponent(rd_grp_internal_forces_type);
        //����� �����
		fs->ReadComponent(pnl_steel);
		fs->ReadComponent(DefineSteelForm->ComboBox_gost);
		fs->ReadComponent(DefineSteelForm->ComboBox_steel);
		fs->ReadComponent(DefineSteelForm->Edit_E);
		fs->ReadComponent(DefineSteelForm->Edit_G);
		fs->ReadComponent(DefineSteelForm->Edit_nu);
		fs->ReadComponent(DefineSteelForm->Edit_gamma_m);
		fs->ReadComponent(DefineSteelForm->StringGrid_Prop);
		//����� �����
		fs->ReadComponent(pnl_concrete_grade);
		fs->ReadComponent(ConcreteDefinitionForm->cmb_bx_concrete_grade_list);
		fs->ReadComponent(ConcreteDefinitionForm->edt_R_bn);
		fs->ReadComponent(ConcreteDefinitionForm->edt_R_btn);
		fs->ReadComponent(ConcreteDefinitionForm->edt_E_b);
		fs->ReadComponent(ConcreteDefinitionForm->edt_gamma_b);
		fs->ReadComponent(ConcreteDefinitionForm->edt_gamma_bt);
		fs->ReadComponent(ConcreteDefinitionForm->edt_epsilon_b_lim);
		//����� ��������
		fs->ReadComponent(pnl_rebar_viewer);
		fs->ReadComponent(RebarDefinitionForm->cmb_bx_rebar_grade);
		fs->ReadComponent(RebarDefinitionForm->edt_R_s_n);
		fs->ReadComponent(RebarDefinitionForm->edt_diameter);
		fs->ReadComponent(RebarDefinitionForm->edt_safety_factor);
		//����� ������ �����
		fs->ReadComponent(pnl_shear_stud_viewer);
		fs->ReadComponent(StudDefinitionForm->cmb_bx_stud_part_number);
		fs->ReadComponent(StudDefinitionForm->edt_stud_diameter);
		fs->ReadComponent(StudDefinitionForm->edt_stud_height);
		fs->ReadComponent(StudDefinitionForm->edt_stud_safety_factor);
		//����� ��������� �������
		fs->ReadComponent(SteelSectionForm->SteelSectionDefinitionFrame->ComboBox_profil);
		fs->ReadComponent(Pnl_SteelSectionViewer);

		//��� �������������� �����
		fs->ReadComponent(rdgrp_slab_type);
		fs->ReadComponent(cmb_bx_corrugated_sheeting_part_number);
		fs->ReadComponent(edt_h_f);
		fs->ReadComponent(edt_flat_slab_thickness);
	}
	__finally
	{
		delete fs;
	}

   // calculate_composite_beam();
}

void __fastcall TCompositeBeamMainForm::NSaveAsClick(TObject *Sender)
{
	strcpy(ModelFile, UNTITLED);
	NSaveClick(Sender);
}
//---------------------------------------------------------------------------
//@--------------------------------------------------------------------------
// ��������� �� ����� ����� � ����� ������
void ModelName(char * str0, char* ModelFile)
{
	  char *ptr1, *ptr2;
	  char i, str[240];

	  if  (strcmp(ModelFile, UNTITLED)==0) {
	   i= 240<strlen(str0) ? 240 : strlen(str0);
	   strcpy(str,str0);
	   ptr1 = strrchr(str,'\\');
	   ptr2 = strrchr(ptr1,'.');
	   if (ptr2!=NULL)
		 *ptr2='\0';
	   if (ptr1==NULL)
		 strcpy(ModelFile, str);
	   else
		 strcpy(ModelFile, ptr1+1);
	  }
}

void __fastcall TCompositeBeamMainForm::NOpenClick(TObject *Sender)
{

   NNewClick(Sender);

   if(OpenDialog_Model->Execute())
   {
	  FileDir_Name = OpenDialog_Model->FileName;
   }
   if (FileDir_Name!="") {

	  strcpy(ModelFile, UNTITLED);

	  load_controls_from_file();

	  ModelName(FileDir_Name.c_str(), ModelFile);

	  Caption = "������ ��������������� ����� - " + AnsiString(ModelFile);

	  modify_project = false;

   }

}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm::OnControlsChange(TObject *Sender)
{
	if (btn_report->Enabled)
		btn_report->Enabled=false;
	if(tb_results->TabVisible)
		tb_results->TabVisible=false;
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm::chck_bx_end_beamClick(TObject *Sender)
{
	if (chck_bx_end_beam->Checked){
		lbl_trib_width_left->Caption="���� ����� [��]:";
		lbl_trib_width_right->Caption="���������� ����� ������� [��]:";
		}
	else{
		lbl_trib_width_left->Caption="���������� ����� ������� ����� [��]:";
		lbl_trib_width_right->Caption="���������� ����� ������� ������ [��]:";
	}
	OnControlsChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm::cmb_bx_analysis_theoryChange(TObject *Sender)

{
    OnControlsChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm::cmb_bx_corrugated_sheeting_part_numberChange(TObject *Sender)
{
	OnControlsChange(Sender);
}

int __fastcall TCompositeBeamMainForm::LoadComponent(String filename, TComponent* Component)
{
	assert(Component!=nullptr);
	assert(Component->Owner!=nullptr);

	std::auto_ptr<TFileStream> fs (new TFileStream(filename, fmOpenRead));
	std::auto_ptr<TReader> Reader (new TReader(fs.get(), 4096));

	Reader->Root=Component->Owner;
	TControl* Control=dynamic_cast<TControl*>(Component);
	if(Control){
		Reader->Parent=Control->Parent;
	}

	delete Component;
	Component=nullptr;
	Reader->BeginReferences();
	try{
		Component=Reader->ReadComponent(nullptr);
	}

	__finally{
	Reader->FixupReferences();
	Reader->EndReferences();
	}
	return Reader->Position;
}
int __fastcall TCompositeBeamMainForm::SaveComponent(String filename, TComponent* Component)
{
	assert(Component!=nullptr);
	assert(Component->Owner!=nullptr);

	std::auto_ptr<TFileStream> fs (new TFileStream(filename, fmCreate));
	std::auto_ptr<TWriter> Writer (new TWriter(fs.get(), 4096));

	Writer->Root=Component->Owner;
	Writer->WriteComponent(Component);
	return Writer->Position;
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::update()
{
	pnl_rebar_viewer->Caption = RebarDefinitionForm -> get_rebar().get_grade();
}



//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm::btn_loggerClick(TObject *Sender)
{
	FormLogger->Show();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------



void __fastcall TCompositeBeamMainForm::N8Click(TObject *Sender)
{
	AboutProgForm->ShowModal();
}
//---------------------------------------------------------------------------

