﻿//---------------------------------------------------------------------------
#include <vcl.h>
#include<ComObj.hpp>
#include <ostream>
#include <fstream>
#pragma hdrstop
#include<vector>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include "CompositeBeamMainFormUnit.h"
#include "String_doubleUnit.h"
#include "uWord_Automation.h"
#include "AboutProg.h"
#include "uComposSectCalculatorS35.h"
#include "uComposSectGeomSP35.h"
#include "uIntForcesCalculator.h"

TCompositeBeamMainForm  *CompositeBeamMainForm;

//----------------------------------------------------------------------
 _fastcall TCompositeBeamMainForm ::TCompositeBeamMainForm (TComponent* Owner)
	: TForm(Owner), frm_logger_(new TFormLogger(this))
{
	composite_beam_calculator_.set_default_values();

	cotr_ratios_grid();
	cotr_comp_sect_geometr_grid();
	cotr_steel_sect_geometr_grid();
	ctor_concrete_sect_geometr_grid();;
	fill_cmb_bx_impact();
	fill_cmb_bx_corrugated_sheets();
	modify_project = false;
}
//----------------------------------------------------------------------
void __fastcall TCompositeBeamMainForm ::FormShow(TObject *Sender)
{

	register_observers();

	register_I_composite_beam();

	set_form_controls();

	NNewClick(Sender);

  //	rdgrp_slab_typeClick(Sender);

	calculate_composite_beam();

}
//----------------------------------------------------------------------
////Присваивение значений полям формы из данных класс типа TCompositeBeam
//----------------------------------------------------------------------
void TCompositeBeamMainForm ::set_form_controls()
{
 // Данные типа Geometry
	Geometry geom = composite_beam_calculator_.get_geometry();
	edt_max_elem_length -> Text = composite_beam_calculator_.get_max_elem_length();
	edt_span -> Text = geom.get_span();
	edt_width_left -> Text = geom.get_spacing_left();
	edt_width_right -> Text = geom.get_spacing_right();
	cmb_bx_number_propping_supports -> ItemIndex =
		cmb_bx_number_propping_supports -> Items -> IndexOf(static_cast<int>(geom.get_temporary_supports_number()));
//Данные типа Loads
	Loads loads = composite_beam_calculator_.get_loads();

	edt_SW_add_concrete -> Text = loads.get_SW_add_concrete(LoadUnit::kN, LengthUnit::m);
	edt_dead_load_first_stage -> Text = loads.get_dead_load_first_stage(LoadUnit::kN, LengthUnit::m);
	edt_dead_load_second_stage -> Text = loads.get_dead_load_second_stage(LoadUnit::kN, LengthUnit::m);
	edt_live_load -> Text = loads.get_live_load(LoadUnit::kN, LengthUnit::m);

	edt_gamma_f_st_SW -> Text = loads.get_gamma_f_st_SW();
	edt_gamma_f_concrete_SW -> Text = loads.get_gamma_f_concrete_SW();
	edt_gamma_f_add_concrete_SW -> Text = loads.get_gamma_f_add_concrete_SW();
	edt_gamma_f_DL_I -> Text = loads.get_gamma_f_DL_I();
	edt_gamma_f_DL_II -> Text = loads.get_gamma_f_DL_II();
	edt_gamma_f_LL -> Text = loads.get_gamma_f_LL();

	edt_sheeting_continuity_coefficient -> Text = loads.get_sheeting_continuity_coefficient();

	FrmAddImpacts -> set_sigma_bi(loads.get_sigma_bi());
	FrmAddImpacts -> set_sigma_si(loads.get_sigma_si());

//Данные типа WorkingConditionsFactors
	WorkingConditionsFactors wcf = composite_beam_calculator_.get_working_conditions_factors();
	edt_gamma_bi -> Text = wcf.gamma_bi();
	edt_gamma_si -> Text = wcf.gamma_si();
	edt_gamma_c -> Text = wcf.gamma_c();

//Максимальное расстояние между расчётными сечениями:
	edt_max_elem_length -> Text = composite_beam_calculator_.get_max_elem_length();

//Панели для отображения данных
	pnl_shear_stud_viewer -> Caption = StudDefinitionForm -> get_studs_on_beam().get_name();
	pnl_rebar_viewer -> Caption = RebarDefinitionForm -> get_rebar().get_grade();
	pnl_concrete_grade -> Caption = ConcreteDefinitionForm -> get_concrete().get_grade();
	Pnl_SteelSectionViewer -> Caption = SteelSectionForm -> get_i_section().get_profile_number();

//Данные для плиты

	switch (composite_beam_calculator_.get_composite_section().get_concrete_part().get_slab_type_enum())
	{
	case SlabType::FLAT:
		rdgrp_slab_type -> ItemIndex = 0;
		break;
	case SlabType::CORRUGATED:
		rdgrp_slab_type -> ItemIndex = 1;
		cmb_bx_corrugated_sheeting_part_number -> ItemIndex =
			cmb_bx_corrugated_sheeting_part_number -> Items -> IndexOf(composite_beam_calculator_.get_composite_section().get_concrete_part().get_slab_type());
		chck_bx_wider_flange_up -> Checked = composite_beam_calculator_.get_composite_section().
			get_concrete_part().get_wider_flange_up();
		chck_bx_sheet_orient_along -> Checked = composite_beam_calculator_.get_composite_section().
			get_concrete_part().get_sheet_orient_along();
		break;
	}
	edt_h_f_flat -> Text = composite_beam_calculator_.get_composite_section().get_concrete_part().get_h_f();
	edt_h_f -> Text = composite_beam_calculator_.get_composite_section().get_concrete_part().get_h_f();
	edt_h_n -> Text = composite_beam_calculator_.get_composite_section().get_concrete_part().get_h_n();

//Данные типа Studs
   	StudDefinitionForm -> set_form_controls(composite_beam_calculator_.get_studs_on_beam());

//Данные типа Rebar
	RebarDefinitionForm -> set_form_controls(composite_beam_calculator_.get_composite_section().get_concrete_part().get_rebar());

//Данные типа Concrete
	ConcreteDefinitionForm -> set_form_controls(composite_beam_calculator_.get_composite_section().get_concrete_part().get_concrete());
	Concrete con = composite_beam_calculator_.get_composite_section().get_concrete_part().get_concrete();

//Данные типа Steel
	DefineSteelForm -> set_form_controls(composite_beam_calculator_.get_composite_section().get_steel_part().get_steel());
//Данные типа Section
	SteelSectionForm -> set_form_controls(composite_beam_calculator_.get_composite_section().get_steel_part().get_section());

}
void TCompositeBeamMainForm ::register_observers()
{
	std::vector<IPublisher*> ipublishers;
	ipublishers.push_back(RebarDefinitionForm);
	ipublishers.push_back(StudDefinitionForm);
	ipublishers.push_back(ConcreteDefinitionForm);
	ipublishers.push_back(DefineSteelForm);
	ipublishers.push_back(SteelSectionForm);
	for(auto ip:ipublishers)
	ip -> register_observer(this);

}
void TCompositeBeamMainForm ::register_I_composite_beam()
{
	DefineSteelForm -> register_icopmosite_beam_user(this);
}


//---------------------------------------------------------------------------
//Инициализация топологии
//---------------------------------------------------------------------------
Geometry TCompositeBeamMainForm ::update_geometry()
{
	int rc = 0;

	double span = 0.;
	double trib_width_left = 0.;
	double trib_width_right = 0.;

	rc = String_double_plus(lbl_span->Caption, edt_span->Text, &span);
	if(rc > 0)
		throw(rc);
		
	rc = String_double_plus(lbl_trib_width_left->Caption, edt_width_left->Text, &trib_width_left);
	if(rc > 0)
		throw(rc);

	rc = String_double_plus(lbl_trib_width_right->Caption, edt_width_right->Text, &trib_width_right);
	if(rc > 0)
		throw(rc);	


	return Geometry{chck_bx_end_beam->Checked,
					 span, trib_width_left,
					 trib_width_right,
					 static_cast<SupportsNumber>(StrToFloat(cmb_bx_number_propping_supports -> Text))};

}
//---------------------------------------------------------------------------
//Инициализация нагрузок и коэффициентов надёжности по нагрузкам
//(для инициализации SW (собственного веса) необходима инициализация структуры I сечения- предусловие)
//---------------------------------------------------------------------------
Loads TCompositeBeamMainForm ::update_loads()
{
	double SW_sheets = 0.;

	double SW = SteelSectionForm -> get_i_section().get_weight();

	double SW_add_concrete = 0.;
	double DL_I = 0.;
	double DL_II = 0.;
	double LL = 0.;

	double gamma_f_st_SW = 0.;
	double gamma_f_concrete_SW = 0.;
	double gamma_f_add_concrete_SW = 0.;
	double gamma_f_DL_I = 0.;
	double gamma_f_DL_II = 0.;
	double gamma_f_LL = 0.;

	double sheeting_continuity_coefficient = 0.;

	int rc = 0;

	rc = String_double_zero_plus(lbl_dead_load_first_stage -> Caption, edt_dead_load_first_stage -> Text, &DL_I);
	if(rc > 0)
		throw(rc);
	rc = String_double_zero_plus(lbl_SW_add_concrete -> Caption, edt_SW_add_concrete -> Text, &SW_add_concrete);
	if(rc > 0)
		throw(rc);
	rc = String_double_zero_plus(lbl_dead_load_second_stage -> Caption, edt_dead_load_second_stage -> Text, &DL_II);
	if(rc > 0)
		throw(rc);
	rc = String_double_zero_plus(lbl_live_load -> Caption, edt_live_load -> Text, &LL);
	if(rc > 0) 
		throw(rc);

	rc = String_double_zero_plus(lbl_gamma_f_st_SW -> Caption, edt_gamma_f_st_SW -> Text, &gamma_f_st_SW);
	if(rc > 0) 
		throw(rc);
	rc = String_double_zero_plus(lbl_gamma_f_concrete_SW -> Caption, edt_gamma_f_concrete_SW -> Text, &gamma_f_concrete_SW);
	if(rc > 0) 
		throw(rc);
	rc = String_double_zero_plus(lbl_gamma_f_DL_I -> Caption, edt_gamma_f_DL_I -> Text, &gamma_f_DL_I);
	if(rc > 0) 
		throw(rc);
	rc = String_double_zero_plus(lbl_gamma_f_add_concrete_SW -> Caption, edt_gamma_f_add_concrete_SW -> Text, &gamma_f_add_concrete_SW);
	if(rc > 0) 
		throw(rc);
	rc = String_double_zero_plus(lbl_gamma_f_DL_II -> Caption, edt_gamma_f_DL_II -> Text, &gamma_f_DL_II);
	if(rc > 0) 
		throw(rc);
	rc = String_double_zero_plus(lbl_gamma_f_LL -> Caption, edt_gamma_f_LL -> Text, &gamma_f_LL);
	if(rc > 0) 
		throw(rc);

	rc = String_double_zero_plus(lbl_sheeting_continuity_coefficient -> Caption, edt_sheeting_continuity_coefficient -> Text, &sheeting_continuity_coefficient);
	if(rc > 0) 
		throw(rc);
	
	return Loads {SW, SW_sheets, SW_add_concrete, DL_I, DL_II, LL,
		gamma_f_st_SW, gamma_f_concrete_SW, gamma_f_add_concrete_SW, gamma_f_DL_I, gamma_f_DL_II, gamma_f_LL,
		sheeting_continuity_coefficient,
		FrmAddImpacts -> get_sigma_bi(),
		FrmAddImpacts -> get_sigma_si()};
}
Loads TCompositeBeamMainForm ::update_loads(double SW_st_beam, double conc_sect, double SW_corrug_sheet)
{
	return Loads{};
}
//---------------------------------------------------------------------------
//Инициализация геометрии двутавра
//---------------------------------------------------------------------------
ISection TCompositeBeamMainForm ::update_i_section()
{
	return SteelSectionForm -> get_i_section();
}
std::unique_ptr<GeneralConcreteSection> update_steel_section()
{
	return std::unique_ptr<GeneralConcreteSection> {};
}
//---------------------------------------------------------------------------
//	Инициализация материала двутавра
//---------------------------------------------------------------------------
Steel TCompositeBeamMainForm ::update_steel_i_section()
{
	return DefineSteelForm -> get_steel();
}

std::unique_ptr<GeneralConcreteSection> TCompositeBeamMainForm ::update_concrete_section(
	double L, double B_l, double B_r, bool is_end_beam, double b_uf)
{
	double h_f = 0.;

	if(int rc = String_double_plus(lbl_h_f->Caption, edt_h_f->Text, &h_f))
		throw(rc);

	Rebars rebars = RebarDefinitionForm -> get_rebars();

	if (rdgrp_slab_type -> ItemIndex == 0)
	{
		return std::unique_ptr<GeneralConcreteSection>{new SlabConcreteSection{
				h_f,
				L,
				B_l,
				B_r,
				b_uf,
				is_end_beam,
				rebars}};
	}
	else
	{
		return std::unique_ptr<GeneralConcreteSection>{new CorrugatedConcreteSection{
			CorrugatedSheetsData::get_corrugated_sheet(cmb_bx_corrugated_sheeting_part_number -> Text),
			h_f,
			L,
			B_l,
			B_r,
			b_uf,
			is_end_beam,
			rebars}};
	}

}
//---------------------------------------------------------------------------
//Инициализация железобетонной части сечения
//---------------------------------------------------------------------------
ConcretePart TCompositeBeamMainForm ::update_concrete_part()
{
	Geometry geometry = update_geometry();

	ISection i_section = SteelSectionForm -> get_i_section();
	double b_uf = i_section.get_b_uf();

	if (rdgrp_slab_type -> ItemIndex ==0)
	{
		double t_sl = 0.;
		double h_n = 0.;
		String_double_plus(lbl_h_f_flat -> Caption, edt_h_f_flat -> Text, &t_sl);
		String_double_zero_plus(lbl_h_n -> Caption, edt_h_n -> Text, &h_n);
		return ConcretePart (L"Плоская плита",
							  SlabType::FLAT,
							  ConcreteDefinitionForm->get_concrete(),
							  RebarDefinitionForm->get_rebar(),
							  t_sl,
							  h_n);
	}
	else
	{
		double h_f = 0.;

		String_double_plus(lbl_h_f->Caption, edt_h_f->Text, &h_f);

		return ConcretePart (cmb_bx_corrugated_sheeting_part_number->Text,
							  SlabType::CORRUGATED,
							  ConcreteDefinitionForm->get_concrete(),
							  RebarDefinitionForm->get_rebar(),
							  h_f,
							  0.,
							  chck_bx_wider_flange_up -> Checked,
							  chck_bx_sheet_orient_along -> Checked);
	}
}
//---------------------------------------------------------------------------
//Инициализация стальной части сечения
//---------------------------------------------------------------------------
SteelPart TCompositeBeamMainForm ::update_steel_part()
{

   ISection i_section = update_i_section();
   Steel steel_i_section = update_steel_i_section();

	return SteelPart(i_section, steel_i_section);
}
//---------------------------------------------------------------------------
//	Инициализация упоров
//---------------------------------------------------------------------------
StudsOnBeam TCompositeBeamMainForm ::update_studs_on_beam()
{
	return StudDefinitionForm -> get_studs_on_beam();
}
//---------------------------------------------------------------------------
//	Инициализация коэффициентов условий работы
//---------------------------------------------------------------------------
 WorkingConditionsFactors TCompositeBeamMainForm ::update_working_conditions_factors()
 {
	int rc = 0;
	double gamma_bi=0.;
	double gamma_si=0.;
	double gamma_c=0.;

	rc = String_double_plus(lbl_gamma_bi->Caption, edt_gamma_bi->Text, &gamma_bi);
	if(rc > 0)
		throw(rc);

	rc = String_double_plus(lbl_gamma_si->Caption, edt_gamma_si->Text, &gamma_si);
	if(rc > 0)
		throw(rc);

	rc = String_double_plus(lbl_gamma_c->Caption, edt_gamma_c->Text, &gamma_c);
	if(rc > 0)
		throw(rc);

	return WorkingConditionsFactors{gamma_bi,gamma_si,gamma_c};
 }

// ---------------------------------------------------------------------------
// Инициализация композитной балки
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::update_composite_beam()
{
	Geometry geometry = update_geometry();

	Loads loads = update_loads();

   SteelPart steel_part = update_steel_part();
   ConcretePart concrete_part = update_concrete_part();
   CompositeSectionGeometry composite_section = CompositeSectionGeometry(steel_part, concrete_part);

   StudsOnBeam studs_on_beam = update_studs_on_beam();

   WorkingConditionsFactors working_conditions_factors = update_working_conditions_factors();

   double max_elem_length = 0.;

	String_double_plus(lbl_max_elem_length -> Caption, edt_max_elem_length -> Text, &max_elem_length);

	composite_beam_calculator_ = CompositeBeamCalculator(geometry,
											  loads,
											  composite_section,
											  studs_on_beam,
											  working_conditions_factors,
											  max_elem_length);
}
//---------------------------------------------------------------------------
//	Функция запускающая расчёт композитной балки
//---------------------------------------------------------------------------
void __fastcall TCompositeBeamMainForm ::BtnCalculateClick(TObject *Sender)
{
	switch(rd_grp_code-> ItemIndex)
	{
		case 0:
			calculate_composite_beam();
			break;
		case 1:
			calculate_composite_beam_SP35();
			break;
	}
}
//---------------------------------------------------------------------------
//Сформировать и открыть отчёт
//---------------------------------------------------------------------------
void __fastcall TCompositeBeamMainForm ::btn_reportClick(TObject *Sender)
{
	Screen->Cursor = crHourGlass;//На время создания отчёта присвоем курсору вид часов
	generate_report();
	Screen->Cursor = crDefault;//Возвращаем курсору вид по умолчанию
}
//---------------------------------------------------------------------------
//	Активация контролов для внесения данных о бетонном сечении в зависимости
//	от выбранного типа: плита по настилу или плоская плита
//---------------------------------------------------------------------------
void __fastcall TCompositeBeamMainForm ::rdgrp_slab_typeClick(TObject *Sender)
{
	switch(rdgrp_slab_type -> ItemIndex)
	{
		case 0: grp_bx_corrugated_slab -> Visible = false;
				grp_bx_flat_slab -> Visible = true;
				Image2 -> Visible = false;
				Image1 -> Visible = true;
				break;

	   case 1:	grp_bx_corrugated_slab -> Visible = true;
				grp_bx_flat_slab -> Visible = false;
				Image2 -> Visible = true;
				Image1 -> Visible = false;
				break;
	}
		OnControlsChange(nullptr);
}

void __fastcall TCompositeBeamMainForm ::strng_grd_results_rendering(TObject *Sender,
																int ACol, int ARow,
																TRect &Rect, TGridDrawState State)
{
	TStringGrid* str_grid=static_cast<TStringGrid*>(Sender);
	if (ARow == 0)
	{
		str_grid -> Canvas -> Font -> Style = TFontStyles() << fsBold;
		str_grid -> Canvas -> TextOut(Rect.Left+3, Rect.Top+5, str_grid -> Cells[ACol][ARow]);
	}
	if (ARow == 1 && ACol == 0)
	{
		str_grid -> Canvas -> Font -> Style = TFontStyles() << fsBold << fsUnderline << fsItalic;
		str_grid -> Canvas -> TextOut(Rect.Left+3, Rect.Top+5, str_grid -> Cells[ACol][ARow]);
	}
	if (ARow == 4 && ACol == 0)
	{
		str_grid -> Canvas -> Font -> Style = TFontStyles() << fsBold << fsUnderline << fsItalic;
		str_grid -> Canvas -> TextOut(Rect.Left+3, Rect.Top+5, str_grid -> Cells[ACol][ARow]);
	}
	if (ARow == 9 && ACol == 0)
	{
		str_grid -> Canvas -> Font -> Style = TFontStyles() << fsBold << fsUnderline << fsItalic;
		str_grid -> Canvas -> TextOut(Rect.Left+3, Rect.Top+5, str_grid -> Cells[ACol][ARow]);
	}
	if (ARow == 12 && ACol == 0)
	{
		str_grid -> Canvas -> Font -> Style = TFontStyles() << fsBold << fsUnderline << fsItalic;
		str_grid -> Canvas -> TextOut(Rect.Left+3, Rect.Top+5, str_grid -> Cells[ACol][ARow]);
	}
	if (ARow == 15 && ACol == 0)
	{
		str_grid -> Canvas -> Font -> Style = TFontStyles() << fsBold << fsUnderline << fsItalic;
		str_grid -> Canvas -> TextOut(Rect.Left+3, Rect.Top+5, str_grid -> Cells[ACol][ARow]);
	}
	if (ARow == 1 && ACol == 1)
	{
		str_grid -> Canvas -> Brush -> Color = clBtnFace;
		str_grid -> Canvas -> FillRect(Rect);
	}
	if (ARow == 4 && ACol == 1)
	{
		str_grid -> Canvas -> Brush -> Color = clBtnFace;
		str_grid -> Canvas -> FillRect(Rect);
	}
	if (ARow == 9 && ACol == 1)
	{
		str_grid -> Canvas -> Brush -> Color = clBtnFace;
		str_grid -> Canvas -> FillRect(Rect);
	}
	if (ARow == 12 && ACol == 1)
	{
		str_grid -> Canvas -> Brush -> Color = clBtnFace;
		str_grid -> Canvas -> FillRect(Rect);
	}
	if (ARow == 15 && ACol == 1)
	{
		str_grid -> Canvas -> Brush -> Color = clBtnFace;
		str_grid -> Canvas -> FillRect(Rect);
	}

}
//---------------------------------------------------------------------------
//	Функция заполняющая TStringGrid выводящий результаты расчёта геометрических характеристик композитного сечения
//---------------------------------------------------------------------------
void TCompositeBeamMainForm ::cotr_comp_sect_geometr_grid()
{
	strng_grd_compos_sect_geom_character->Cells [0][0]=L"Геометрические характеристики";
	strng_grd_compos_sect_geom_character->Cells [1][0]=L"Значения";
	strng_grd_compos_sect_geom_character->Cells [0][1]=L"Площадь Ared, мм2";
	strng_grd_compos_sect_geom_character->Cells [0][2]=L"Момент инерции Ired, мм4 ";
	strng_grd_compos_sect_geom_character->Cells [0][3]=L"Момент сопротивления, Ц.Т. ж.б. плиты Wb,red, мм3";
	strng_grd_compos_sect_geom_character->Cells [0][4]=L"Расстояние Zb,red, мм";
	strng_grd_compos_sect_geom_character->Cells [0][5]=L"Расстояние Zst,red мм";
	strng_grd_compos_sect_geom_character->Cells [0][6]=L"Расстояние Zb,st мм";
}
//---------------------------------------------------------------------------
//Функция заполняющая объект TStringGrid геометрическими характеристиками стального сечения
//---------------------------------------------------------------------------
void TCompositeBeamMainForm ::cotr_steel_sect_geometr_grid()
{
	strng_grd_steel_sect_geom_character->Cells [0][0]=L"Геометрические характеристики";
	strng_grd_steel_sect_geom_character->Cells [1][0]=L"Значения";
	strng_grd_steel_sect_geom_character->Cells [0][1]=L"Площадь, мм2";
	strng_grd_steel_sect_geom_character->Cells [0][2]=L"Момент инерции, мм4";
	strng_grd_steel_sect_geom_character->Cells [0][3]=L"Момент сопротивления крайних волокон в.полки, мм3";
	strng_grd_steel_sect_geom_character->Cells [0][4]=L"Момент сопротивления крайних волокон н.полки, мм3";
	strng_grd_steel_sect_geom_character->Cells [0][5]=L"Расстояние Zf2,st, мм";
	strng_grd_steel_sect_geom_character->Cells [0][6]=L"Расстояние Zf1,st, мм";
}
//---------------------------------------------------------------------------
//Функция заполняющая объект TStringGrid геометрическими характеристиками железобетонного сечения
//---------------------------------------------------------------------------
void TCompositeBeamMainForm ::ctor_concrete_sect_geometr_grid()
{
	strng_grd_concrete_sect_geom_character->Cells [0][0]=L"Геометрические характеристики";
	strng_grd_concrete_sect_geom_character->Cells [1][0]=L"Значения";
	strng_grd_concrete_sect_geom_character->Cells [0][1]=L"Расчётная ширина, мм";
	strng_grd_concrete_sect_geom_character->Cells [0][2]=L"Расстояние от нижней грани до центра тяжести, мм";
	strng_grd_concrete_sect_geom_character->Cells [0][3]=L"Площадь, мм2";
	strng_grd_concrete_sect_geom_character->Cells [0][4]=L"Момент инерции, мм4";
   //	strng_grd_concrete_sect_geom_character->Cells [0][6]=L"Момент сопротивления";
}
void TCompositeBeamMainForm ::fill_steel_sect_geometr_grid()
{
	ISection i_section= composite_beam_calculator_.get_composite_section().get_steel_part().get_section();

	strng_grd_steel_sect_geom_character->Cells [1][1]=FloatToStrF(i_section.get_A_st(), ffFixed, 15, 0);
	strng_grd_steel_sect_geom_character->Cells [1][2]=FloatToStrF(i_section.get_I_st(), ffFixed, 15, 0);
	strng_grd_steel_sect_geom_character->Cells [1][3]=FloatToStrF(i_section.get_Wf2_st(), ffFixed, 15, 0);
	strng_grd_steel_sect_geom_character->Cells [1][4]=FloatToStrF(i_section.get_Wf1_st(), ffFixed, 15, 0);
	strng_grd_steel_sect_geom_character->Cells [1][5]=FloatToStrF(i_section.get_Z_f2_st(), ffFixed, 15, 0);
	strng_grd_steel_sect_geom_character->Cells [1][6]=FloatToStrF(i_section.get_Z_f1_st(), ffFixed, 15, 0);
}
void TCompositeBeamMainForm ::fill_concrete_sect_geometr_grid()
{
	ConcretePart concrete_part=composite_beam_calculator_.get_composite_section().get_concrete_part();

	strng_grd_concrete_sect_geom_character->Cells [1][1]=FloatToStrF(concrete_part.get_b_sl(), ffFixed, 15, 0);
	strng_grd_concrete_sect_geom_character->Cells [1][2]=FloatToStrF(concrete_part.get_C_b(), ffFixed, 15, 0);
	strng_grd_concrete_sect_geom_character->Cells [1][3]=FloatToStrF(concrete_part.get_A_b(), ffFixed, 15, 0);
	strng_grd_concrete_sect_geom_character->Cells [1][4]=FloatToStrF(concrete_part.get_I_b(), ffFixed, 15, 0);
}
void TCompositeBeamMainForm ::fill_composite_sect_geometr_grid()
{
	double A_red=composite_beam_calculator_.get_composite_section().get_A_red();
	double I_red=composite_beam_calculator_.get_composite_section().get_I_red();
	double W_b_red=composite_beam_calculator_.get_composite_section().get_W_b_red();
	double Z_b_red=composite_beam_calculator_.get_composite_section().get_Z_b_red();
	double Z_f2_red=composite_beam_calculator_.get_composite_section().get_Z_st_red();
	double Z_f1_red=composite_beam_calculator_.get_composite_section().get_Z_b_st();

	strng_grd_compos_sect_geom_character->Cells [1][1]=FloatToStrF(A_red, ffFixed, 15, 0); //Предельно значение точность для
	strng_grd_compos_sect_geom_character->Cells [1][2]=FloatToStrF(I_red, ffFixed, 15, 0); //типа double 15
	strng_grd_compos_sect_geom_character->Cells [1][3]=FloatToStrF(W_b_red, ffFixed, 15, 0);
	strng_grd_compos_sect_geom_character->Cells [1][4]=FloatToStrF(Z_b_red, ffFixed, 15, 0);
	strng_grd_compos_sect_geom_character->Cells [1][5]=FloatToStrF(std::abs(Z_f2_red), ffFixed, 15, 0);
	strng_grd_compos_sect_geom_character->Cells [1][6]=FloatToStrF(Z_f1_red, ffFixed, 15, 0);
}

void TCompositeBeamMainForm ::fill_results_grid()
{
	Section max_i_section_ratio_section = composite_beam_calculator_.get_composite_beam().get_max_i_section_ratio_section();

	strng_grd_results -> Cells [1][2] = FloatToStrF(std::abs(max_i_section_ratio_section.get_x()), ffFixed, 15, 0);
	strng_grd_results -> Cells [1][3] = FloatToStrF(std::abs(max_i_section_ratio_section.get_i_section_ratio()), ffFixed, 15, 2);
	Section max_direct_stress_ratio_section = composite_beam_calculator_.get_composite_beam().get_max_direct_stress_ratio_section();

	strng_grd_results -> Cells [1][5] = FloatToStrF(std::abs(max_direct_stress_ratio_section.get_x()), ffFixed, 15, 0);
	strng_grd_results -> Cells [1][6] = FloatToStrF(std::abs(max_direct_stress_ratio_section.get_upper_fl_ratio()), ffFixed, 15, 2);
	strng_grd_results -> Cells [1][7] = FloatToStrF(std::abs(max_direct_stress_ratio_section.get_lower_fl_ratio()), ffFixed, 15, 2);
	strng_grd_results -> Cells [1][8] = FloatToStrF(std::abs(max_direct_stress_ratio_section.get_conc_ratio()), ffFixed, 15, 2);

	Section max_rigid_plastic_ratio_section = composite_beam_calculator_.get_composite_beam().get_max_rigid_plastic_ratio_section();

	strng_grd_results -> Cells [1][10] = FloatToStrF(std::abs(max_rigid_plastic_ratio_section.get_x()), ffFixed, 15, 0);
	strng_grd_results -> Cells [1][11] = FloatToStrF(std::abs(max_rigid_plastic_ratio_section.get_rigid_plastic_ratio()), ffFixed, 15, 2);

	Section max_shear_stress_section = composite_beam_calculator_.get_composite_beam().get_max_shear_stress_ratio_section();

	strng_grd_results -> Cells [1][13] = FloatToStrF(std::abs(max_shear_stress_section .get_x()), ffFixed, 15, 0);
	strng_grd_results -> Cells [1][14] = FloatToStrF(std::abs(max_shear_stress_section.get_shear_ratio()), ffFixed, 15, 2);

	StudsRow max_ratio_studs_row = composite_beam_calculator_.get_studs_on_beam().get_max_ratio_studs_row();

	strng_grd_results -> Cells [1][16] = FloatToStrF(std::abs(max_ratio_studs_row.get_x()), ffFixed, 15, 0);
	strng_grd_results -> Cells [1][17] = FloatToStrF(std::abs(max_ratio_studs_row.get_ratio()), ffFixed, 15, 2);
}
//---------------------------------------------------------------------------
//	Функция заполняющая Grid выводящий результаты расчёта композитной балки
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::cotr_ratios_grid()
{
	strng_grd_results -> Cells [0][0] = L"Проверка";
	strng_grd_results -> Cells [0][1] = L"На действие изгибающих моментов при монтаже";
	strng_grd_results -> Cells [0][2] = L"      Координата критического сечения, мм";
	strng_grd_results -> Cells [0][3] = L"      Прочность";
	strng_grd_results -> Cells [0][4] = L"На действие изгибающих моментов, раздел 6.2.1:";
	strng_grd_results -> Cells [0][5] = L"      Координата критического сечения, мм";
	strng_grd_results -> Cells [0][6] = L"      Прочность верхнего пояса стального сечения";
	strng_grd_results -> Cells [0][7] = L"      Прочность нижнего пояса стального сечения";
	strng_grd_results -> Cells [0][8] = L"      Прочность железобетона";
	strng_grd_results -> Cells [0][9] = L"На действие изгибающих моментов (жёсткопластический материал), пункт 6.2.1.6:";
	strng_grd_results -> Cells [0][10] = L"      Координата критического сечения, мм";
	strng_grd_results -> Cells [0][11] = L"      Прочность балки";
	strng_grd_results -> Cells [0][12] = L"Прочности на действие поперечной силы:";
	strng_grd_results -> Cells [0][13] = L"      Координата критического сечения, мм";
	strng_grd_results -> Cells [0][14] = L"      Прочность сечения, раздел 6.2.2";
	strng_grd_results -> Cells [0][15] = L"Упоров объединения, раздел 9.1.2";
	strng_grd_results -> Cells [0][16] = L"      Координата критического упора, мм";
	strng_grd_results -> Cells [0][17] = L"      Прочность упора";

	strng_grd_results -> Cells [1][0] = L"Коэффициенты Использования (КИ) ";
}
//---------------------------------------------------------------------------
//	Функция заполняющая ComboBox случаями загружений
//---------------------------------------------------------------------------
void TCompositeBeamMainForm ::fill_cmb_bx_impact()
{
	cmb_bx_impact -> Items -> Append(L"Воздействие 1a");
	cmb_bx_impact -> Items -> Append(L"Воздействие 1b");
	cmb_bx_impact -> Items -> Append(L"Воздействие 2c");
	cmb_bx_impact -> Items -> Append(L"Воздействие 2d");
	cmb_bx_impact -> Items -> Append(L"Воздействие: 1b + 2c + 2d");

	cmb_bx_impact -> ItemIndex = 4;
}
//---------------------------------------------------------------------------
//	Функция заполняющая ComboBox настилами
//---------------------------------------------------------------------------
void TCompositeBeamMainForm ::fill_cmb_bx_corrugated_sheets()
{
	for(auto corrugated_sheet:CorrugatedSheetsData::get_corrugated_sheet_names())
		cmb_bx_corrugated_sheeting_part_number->Items->Add(corrugated_sheet);

	cmb_bx_corrugated_sheeting_part_number->ItemIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm ::BtBtnSteelChoiceClick(TObject *Sender)
{
	 DefineSteelForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm ::BtnSteelSectionChoiceClick(TObject *Sender)
{
	SteelSectionForm -> Show();
}
//---------------------------------------------------------------------------


void __fastcall TCompositeBeamMainForm ::BtBtnExitClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm ::BtnConcreteChoiceClick(TObject *Sender)
{
	ConcreteDefinitionForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm ::BtBtnRebarsChoiceClick(TObject *Sender)
{
	RebarDefinitionForm->Show();

}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm ::BtBtnShearStudsChoiceClick(TObject *Sender)
{
	StudDefinitionForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm ::NOutReportClick(TObject *Sender)
{
	generate_report();
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm ::NExitClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
//Создание отчёта
//---------------------------------------------------------------------------
void TCompositeBeamMainForm ::generate_report()
{
	TWord_Automation report_ = TWord_Automation("ReportCompositeBeam.dotx");

//[1.1] Топология
	Geometry geometry = composite_beam_calculator_.get_geometry();

	report_.PasteTextPattern(geometry.is_end_beam_to_str(), "%end_beam%");
	report_.PasteTextPattern(FloatToStrF(geometry.get_span(LengthUnit::mm), ffFixed, 15, 2), "%span%");
	report_.PasteTextPattern(FloatToStrF(geometry.get_spacing_left(LengthUnit::mm), ffFixed, 15, 2), "%trib_width_left% ");
	report_.PasteTextPattern(FloatToStrF(geometry.get_spacing_right(LengthUnit::mm), ffFixed, 15, 2), "%trib_width_right% ");
//[1.2] Загружения
	Loads loads = composite_beam_calculator_.get_loads();

	report_.PasteTextPattern(FloatToStrF(loads.get_self_weight(LoadUnit::kN, LengthUnit::m), ffFixed, 15, 2), "%steel_beam%");
	report_.PasteTextPattern(FloatToStrF(loads.get_self_weight_sheets(LoadUnit::kN, LengthUnit::m), ffFixed, 15, 2), "%SW_sheets%");
	report_.PasteTextPattern(FloatToStrF(loads.get_SW_concrete(LoadUnit::kN, LengthUnit::m), ffFixed, 15, 2), "%SW_concrete%");
	report_.PasteTextPattern(FloatToStrF(loads.get_SW_add_concrete(LoadUnit::kN, LengthUnit::m), ffFixed, 15, 2), "%SW_add_concrete%");
	report_.PasteTextPattern(FloatToStrF(loads.get_dead_load_first_stage(LoadUnit::kN, LengthUnit::m), ffFixed, 15, 2), "%DL_I%");
	report_.PasteTextPattern(FloatToStrF(loads.get_dead_load_second_stage(LoadUnit::kN, LengthUnit::m), ffFixed, 15, 2), "%DL_II%");
	report_.PasteTextPattern(FloatToStrF(loads.get_live_load(LoadUnit::kN, LengthUnit::m), ffFixed, 15, 2), "%LL%");

//[1.3] Коэффициенты надёжности по нагрузке

	report_.PasteTextPattern(FloatToStrF(loads.get_gamma_f_st_SW(), ffFixed, 15, 2), "%gamma_f_st_SW%");
	report_.PasteTextPattern(FloatToStrF(loads.get_gamma_f_concrete_SW(), ffFixed, 15, 2), "%gamma_f_concrete_SW%");
	report_.PasteTextPattern(FloatToStrF(loads.get_gamma_f_add_concrete_SW(), ffFixed, 15, 2), "%SW gamma_f_add_concr%");
	report_.PasteTextPattern(FloatToStrF(loads.get_gamma_f_DL_I(), ffFixed, 15, 2), "%gamma_f_DL_I%");
	report_.PasteTextPattern(FloatToStrF(loads.get_gamma_f_DL_II(), ffFixed, 15, 2), "%gamma_f_DL_II%");
	report_.PasteTextPattern(FloatToStrF(loads.get_gamma_f_LL(), ffFixed, 15, 2), "%gamma_f_LL%");
//[1.4] Стальное сечение
//[1.4.1] Номинальные размеры двутавра
	ISection i_section = composite_beam_calculator_.get_composite_section().get_steel_part().get_section();

	report_.PasteTextPattern(i_section.get_profile_number(),"%profile_number%");
	report_.PasteTextPattern(FloatToStrF(i_section.get_h_st(LengthUnit::mm), ffFixed, 15, 2),"%h%");
	report_.PasteTextPattern(FloatToStrF(0, ffFixed, 15, 2),"%h%");
	report_.PasteTextPattern(FloatToStrF(i_section.get_b_uf(LengthUnit::mm), ffFixed, 15, 2),"%b%");
	report_.PasteTextPattern(FloatToStrF(0, ffFixed, 15, 2),"%b_w%");
	report_.PasteTextPattern(FloatToStrF(i_section.get_t_uf(LengthUnit::mm), ffFixed, 15, 2),"%t%");
	report_.PasteTextPattern(FloatToStrF(i_section.get_t_w(LengthUnit::mm), ffFixed, 15, 2),"%s%");
	report_.PasteTextPattern(FloatToStrF(i_section.get_r(LengthUnit::mm), ffFixed, 15, 2),"%r%");
//[1.4.2] Характеристики стали
	Steel steel = composite_beam_calculator_.get_composite_section().get_steel_part().get_steel();

	report_.PasteTextPattern(steel.get_steel_grade(),"%steel_grade%");
	report_.PasteTextPattern(FloatToStrF(steel.get_R_yn(), ffFixed, 15, 2),"%R_yn%");
	report_.PasteTextPattern(FloatToStrF(steel.get_R_un(), ffFixed, 15, 2),"%R_un%");
	report_.PasteTextPattern(FloatToStrF(steel.get_E_st(), ffFixed, 15, 2),"%E_st%");
	report_.PasteTextPattern(FloatToStrF(steel.get_G_st(), ffFixed, 15, 2),"%G_st%");
	report_.PasteTextPattern(FloatToStrF(steel.get_nu_st(), ffFixed, 15, 2),"%nu_st%");
	report_.PasteTextPattern(FloatToStrF(steel.get_gamma_m(), ffFixed, 15, 2),"%gamma_m%");
//[1.5] Железобетонное сечение
//[1.5.1] Номинальные размеры плиты
	ConcretePart concrete_part = composite_beam_calculator_.get_composite_section().get_concrete_part();

	report_.PasteTextPattern(concrete_part.get_slab_type(),"%slab_type%");
	report_.PasteTextPattern(concrete_part.get_h_n(LengthUnit::cm),"%h_n%");
	report_.PasteTextPattern(concrete_part.get_h_f(LengthUnit::cm),"%t_sl%");

//[1.5.2] Характеристики бетона
	Concrete concrete = concrete_part.get_concrete();

	report_.PasteTextPattern(concrete.get_grade(),"%conc_grade%");
	report_.PasteTextPattern(FloatToStrF(concrete.get_R_bn(), ffFixed, 15, 2),"%R_bn%");
	report_.PasteTextPattern(FloatToStrF(concrete.get_R_btn(), ffFixed, 15, 2),"%R_btn%");
	report_.PasteTextPattern(FloatToStrF(concrete.get_density(LengthUnit::m), ffFixed, 15, 2),"%density%");
	report_.PasteTextPattern(FloatToStrF(concrete.get_phi_b_cr(), ffFixed, 15, 2),"%phi_b_cr%");
	report_.PasteTextPattern(FloatToStrF(concrete.get_E_b(), ffFixed, 15, 2),"%E_b%");
	report_.PasteTextPattern(FloatToStrF(concrete.get_gamma_b(), ffFixed, 15, 2),"%gamma_b%");
	report_.PasteTextPattern(FloatToStrF(concrete.get_gamma_bt(), ffFixed, 15, 2),"%gamma_bt%");
	report_.PasteTextPattern(FloatToStrF(concrete.get_epsilon_b_lim(), ffFixed, 15, 4),"%epsilon_b_lim%");

//[1.6] Арматура
	Rebar rebar = concrete_part.get_rebar();

	report_.PasteTextPattern(rebar.get_grade(),"%grade%");
	report_.PasteTextPattern(FloatToStrF(rebar.get_diameter(), ffFixed, 15, 2),"%d%");
	report_.PasteTextPattern(FloatToStrF(rebar.get_R_sn(), ffFixed, 15, 2),"%R_sn%");
//[1.7] Соединительные элементы
	Stud stud = composite_beam_calculator_.get_studs_on_beam().get_stud();

	StudsOnBeam studs_on_beam = composite_beam_calculator_.get_studs_on_beam();

	report_.PasteTextPattern(stud.get_name(),"%name%");
	report_.PasteTextPattern(FloatToStrF(stud.get_l(cm), ffFixed, 15, 2),"%l%");
	report_.PasteTextPattern(FloatToStrF(stud.get_d_an(cm), ffFixed, 15, 2),"%d_an%");
	report_.PasteTextPattern(FloatToStrF(stud.get_R_y(), ffFixed, 15, 2),"%R_y%");
	report_.PasteTextPattern(FloatToStrF(studs_on_beam.get_dist_e(cm), ffFixed, 15, 2),"%z_e%");
	report_.PasteTextPattern(FloatToStrF(studs_on_beam.get_dist_m(cm), ffFixed, 15, 2),"%z_m%");
	report_.PasteTextPattern(FloatToStrF(studs_on_beam.get_num_e(), ffFixed, 15, 2),"%ed_rw_num%");
	report_.PasteTextPattern(FloatToStrF(studs_on_beam.get_num_m(), ffFixed, 15, 2),"%mid_rw_num%");
	report_.PasteTextPattern(studs_on_beam.get_more_than_one_stud_per_corrugation_edge_string(),"%ed_more_than_one_stud%");
	report_.PasteTextPattern(studs_on_beam.get_more_than_one_stud_per_corrugation_middle_string(),"%mid_more_than_one_stud%");

//[1.8] Коэффициенты

	WorkingConditionsFactors working_conditions_factors = composite_beam_calculator_.get_working_conditions_factors();

	report_.PasteTextPattern(FloatToStrF(working_conditions_factors.gamma_c(), ffFixed, 15, 2),"%gamma_c%");
	report_.PasteTextPattern(FloatToStrF(working_conditions_factors.gamma_bi(), ffFixed, 15, 2),"%gamma_bi%");
	report_.PasteTextPattern(FloatToStrF(working_conditions_factors.gamma_si(), ffFixed, 15, 2),"%gamma_si%");
//[1.9] Учёт условий монтажа

	report_.PasteTextPattern(FloatToStrF(static_cast<int>(geometry.get_temporary_supports_number()), ffFixed, 15, 0),"%temp_supp%");
	report_.PasteTextPattern(FloatToStrF(loads.get_sheeting_continuity_coefficient(), ffFixed, 15, 2),"%sheeting_continuity_coeff%");

//[2] Результаты расчёта балки
//[2.1] Геометрические параметры
//[2.1.1] Стального сечения
	report_.PasteTextPattern(FloatToStrF(i_section.get_A_st(LengthUnit::cm), ffFixed, 15, 2),"%A_st%");
	report_.PasteTextPattern(FloatToStrF(i_section.get_I_st(LengthUnit::cm), ffFixed, 15, 2),"%I_st%");
	report_.PasteTextPattern(FloatToStrF(i_section.get_Wf2_st(LengthUnit::cm), ffFixed, 15, 2),"%Wf2_st%");
	report_.PasteTextPattern(FloatToStrF(i_section.get_Wf1_st(LengthUnit::cm), ffFixed, 15, 2),"%Wf1_st%");
	report_.PasteTextPattern(FloatToStrF(i_section.get_Z_f2_st(LengthUnit::cm), ffFixed, 15, 2),"%Z_f2_st%");
	report_.PasteTextPattern(FloatToStrF(i_section.get_Z_f1_st(LengthUnit::cm), ffFixed, 15, 2),"%Z_f1_st%");
//[2.1.2] Железобетонного сечения
	report_.PasteTextPattern(concrete_part.get_slab_type(),"%slab_type%");
	report_.PasteTextPattern(FloatToStrF(concrete_part.get_b_sl(LengthUnit::cm), ffFixed, 15, 2),"%b_sl%");
	report_.PasteTextPattern(FloatToStrF(concrete_part.get_C_b(LengthUnit::cm), ffFixed, 15, 2),"%C_b%");
	report_.PasteTextPattern(FloatToStrF(concrete_part.get_A_b(LengthUnit::cm), ffFixed, 15, 2),"%A_b%");
	report_.PasteTextPattern(FloatToStrF(concrete_part.get_I_b(LengthUnit::cm), ffFixed, 15, 2),"%I_b%");
//[2.1.3] Композитного сечения

	CompositeSectionGeometry composite_section_geometry = composite_beam_calculator_.get_composite_section();

	report_.PasteTextPattern(FloatToStrF(composite_section_geometry.get_A_red(LengthUnit::cm), ffFixed, 15, 2),"%A_red%");
	report_.PasteTextPattern(FloatToStrF(composite_section_geometry.get_I_red(LengthUnit::cm), ffFixed, 15, 2),"%I_red%");
	report_.PasteTextPattern(FloatToStrF(composite_section_geometry.get_W_b_red(LengthUnit::cm), ffFixed, 15, 2),"%W_b_red%");
	report_.PasteTextPattern(FloatToStrF(composite_section_geometry.get_Z_b_red(LengthUnit::cm), ffFixed, 15, 2),"%Z_b_red%");
	report_.PasteTextPattern(FloatToStrF(composite_section_geometry.get_Z_st_red(LengthUnit::cm), ffFixed, 15, 2),"%Z_st_red%");
	report_.PasteTextPattern(FloatToStrF(composite_section_geometry.get_Z_b_st(LengthUnit::cm), ffFixed, 15, 2),"%Z_b_st%");
//[2.2] Усилия
	Section max_direct_stress_ratio_section = composite_beam_calculator_.get_composite_beam().get_max_direct_stress_ratio_section();

	report_.PasteTextPattern(FloatToStrF(max_direct_stress_ratio_section.get_x(), ffFixed, 15, 0),"%cs_x%");
	report_.PasteTextPattern(FloatToStrF(max_direct_stress_ratio_section.get_M_Ia_design(kN, m), ffFixed, 15, 2),"%M_Ia%");
	report_.PasteTextPattern(FloatToStrF(max_direct_stress_ratio_section.get_M_Ib_design(kN, m), ffFixed, 15, 2),"%M_Ib%");
	report_.PasteTextPattern(FloatToStrF(max_direct_stress_ratio_section.get_M_IIa_design(kN, m), ffFixed, 15, 2),"%M_IIa%");
	report_.PasteTextPattern(FloatToStrF(max_direct_stress_ratio_section.get_M_IIb_design(kN, m), ffFixed, 15, 2),"%M_IIb%");
	report_.PasteTextPattern(FloatToStrF(max_direct_stress_ratio_section.get_M_total_design(kN, m), ffFixed, 15, 2),"%M_total%");

//[2.3] Напряжения
	report_.PasteTextPattern(FloatToStrF(max_direct_stress_ratio_section.get_sigma_b(), ffFixed, 15, 2),"%sigma_b%");
	report_.PasteTextPattern(FloatToStrF(max_direct_stress_ratio_section.get_sigma_s(), ffFixed, 15, 2),"%sigma_s%");
//[2.4] Коэффициенты использования
//[2.4.1] По изгибающим моментам при монтаже

	Section max_i_section_ratio_section = composite_beam_calculator_.get_composite_beam().get_max_i_section_ratio_section();

	report_.PasteTextPattern(FloatToStrF(max_i_section_ratio_section.get_x(),ffFixed, 15, 2),"%x_M_I%");
	report_.PasteTextPattern(FloatToStrF(max_i_section_ratio_section.get_i_section_ratio(),ffFixed, 15, 2),"%i_section_ratio%");

//[2.4.2] По изгибающим моментам, раздел 6.2.1

	report_.PasteTextPattern(FloatToStrF(max_direct_stress_ratio_section.get_x(),ffFixed, 15, 2),"%x_M%");
	report_.PasteTextPattern(FloatToStrF(max_direct_stress_ratio_section.get_upper_fl_ratio(),ffFixed, 15, 2),"%ratio_uf%");
	report_.PasteTextPattern(FloatToStrF(max_direct_stress_ratio_section.get_lower_fl_ratio(),ffFixed, 15, 2),"%ratio_lf%");
	report_.PasteTextPattern(FloatToStrF(max_direct_stress_ratio_section.get_conc_ratio(),ffFixed, 15, 2),"%conc_ratio%");

//[2.4.3] По изгибающим моментам (жёсткопластический материал), пункт 6.2.1.6

	Section max_rigid_plastic_ratio_section = composite_beam_calculator_.get_composite_beam().get_max_rigid_plastic_ratio_section();

	report_.PasteTextPattern(FloatToStrF(max_rigid_plastic_ratio_section.get_x(),ffFixed, 15, 2),"%x_rigid_plastic%");
	report_.PasteTextPattern(FloatToStrF(max_rigid_plastic_ratio_section.get_rigid_plastic_ratio(),ffFixed, 15, 2),"%ratio_rigid_plastic%");

//[2.4.4] По поперечным силам, пункт 6.2.2

	Section max_shear_stress_section = composite_beam_calculator_.get_composite_beam().get_max_shear_stress_ratio_section();

	report_.PasteTextPattern(FloatToStrF(max_shear_stress_section.get_x(),ffFixed, 15, 2),"%x_Q%");
	report_.PasteTextPattern(FloatToStrF(max_shear_stress_section.get_shear_ratio(),ffFixed, 15, 2),"%ratio_shear%");

//[3] Результаты расчёта конструкций объединения

//[3.1] Несущая способность упора

	StudsRow max_ratio_studs_row = composite_beam_calculator_.get_studs_on_beam().get_max_ratio_studs_row();

	report_.PasteTextPattern(FloatToStrF(stud.get_P_rd(kN),ffFixed, 15, 2),"%P_rd%");
	report_.PasteTextPattern(FloatToStrF(max_ratio_studs_row.get_k(),ffFixed, 15, 2),"%k%");
	report_.PasteTextPattern(FloatToStrF(stud.get_S_h(kN),ffFixed, 15, 2),"%S_h%");

  //[3.2] Усилия
	report_.PasteTextPattern(FloatToStrF(max_ratio_studs_row.get_x(),ffFixed, 15, 2),"%cs_stud%");
	report_.PasteTextPattern(FloatToStrF(max_ratio_studs_row.get_S(LoadUnit::kN),ffFixed, 15, 2),"%S_stud%");

 //  [3.3] Коэффициенты использования
	report_.PasteTextPattern(FloatToStrF(max_ratio_studs_row.get_ratio(),ffFixed, 15, 2),"%ratio_stud%");
}

//---------------------------------------------------------------------------
// Отрисовка эпюр
//---------------------------------------------------------------------------
void TCompositeBeamMainForm ::draw_diagram()
{
	std::vector<double> M;
	std::vector<double> Q;
	std::vector<double> R;
	std::vector<double> f;

	std::vector<double>	coor_supp {};

	switch (cmb_bx_impact->ItemIndex)
	{
	case(0): // Нагрузки Ia стадии

		M = composite_beam_calculator_.get_composite_beam().get_M_Ia_design_list(LoadUnit::kN, LengthUnit::m);
		Q = composite_beam_calculator_.get_composite_beam().get_Q_Ia_design_list(LoadUnit::kN);
		R = composite_beam_calculator_.get_composite_beam().get_R_Ia_design_list(LoadUnit::kN);
		f = composite_beam_calculator_.get_composite_beam().get_f_Ia_list(LengthUnit::mm);

		coor_supp = composite_beam_calculator_.get_composite_beam().get_support_x_list();

		break;
	case(1): // Нагрузки Ib стадии

		M = composite_beam_calculator_.get_composite_beam().get_M_Ib_design_list(LoadUnit::kN, LengthUnit::m);
		Q = composite_beam_calculator_.get_composite_beam().get_Q_Ib_design_list(LoadUnit::kN);
		R = composite_beam_calculator_.get_composite_beam().get_R_Ib_design_list(LoadUnit::kN);
		f = composite_beam_calculator_.get_composite_beam().get_f_Ib_list(LengthUnit::mm);

		coor_supp = composite_beam_calculator_.get_composite_beam().get_support_x_list();

		break;
	case(2): // Нагрузки IIa стадии

		M = composite_beam_calculator_.get_composite_beam().get_M_IIa_design_list(LoadUnit::kN, LengthUnit::m);
		Q = composite_beam_calculator_.get_composite_beam().get_Q_IIa_design_list(LoadUnit::kN);
		R = composite_beam_calculator_.get_composite_beam().get_P_IIa_design_list(LoadUnit::kN);
		f = composite_beam_calculator_.get_composite_beam().get_f_IIa_list(LengthUnit::mm);

		coor_supp.push_back(composite_beam_calculator_.get_composite_beam().get_support_x_list().front());
		coor_supp.push_back(composite_beam_calculator_.get_composite_beam().get_support_x_list().back());

		break;

	case(3): // Нагрузки IIb стадии

		M = composite_beam_calculator_.get_composite_beam().get_M_IIb_design_list(LoadUnit::kN, LengthUnit::m);
		Q = composite_beam_calculator_.get_composite_beam().get_Q_IIb_design_list(LoadUnit::kN);
		R = composite_beam_calculator_.get_composite_beam().get_R_IIb_design_list(LoadUnit::kN);
		f = composite_beam_calculator_.get_composite_beam().get_f_IIb_list(LengthUnit::mm);

		coor_supp.push_back(composite_beam_calculator_.get_composite_beam().get_support_x_list().front());
		coor_supp.push_back(composite_beam_calculator_.get_composite_beam().get_support_x_list().back());

		break;

	case(4)://Нагрузки полные

		M = composite_beam_calculator_.get_composite_beam().get_M_total_design_list(LoadUnit::kN, LengthUnit::m);
		Q = composite_beam_calculator_.get_composite_beam().get_Q_total_design_list(LoadUnit::kN);
		R = composite_beam_calculator_.get_composite_beam().get_R_total_design_list(LoadUnit::kN);
		f = composite_beam_calculator_.get_composite_beam().get_f_total_list(LengthUnit::mm);

		coor_supp.push_back(composite_beam_calculator_.get_composite_beam().get_support_x_list().front());
		coor_supp.push_back(composite_beam_calculator_.get_composite_beam().get_support_x_list().back());

		break;
	}

	TImage *Image1=img_static_scheme;
	std::vector<double> coor_epur = composite_beam_calculator_.get_composite_beam().get_x_list();

//флаг отрисовки значений на эпюре
	bool flag_sign = true;
	int num_digits = 2;
	bool con_sign_practice = true;

	switch (rd_grp_internal_forces_type->ItemIndex)
	{
	case(0):

		DrawEpur(Image1, M.size(), &coor_epur[0], &M[0], nullptr, coor_supp.size(), &coor_supp[0],
			flag_sign, num_digits, con_sign_practice);

		break;

	case(1):

		DrawEpur(Image1, Q.size(), &coor_epur[0], &Q[0], &R[0], coor_supp.size(), &coor_supp[0],
			flag_sign, num_digits, con_sign_practice);

		break;

	case(2):

		DrawEpur(Image1, f.size(), &coor_epur[0], &f[0], nullptr, coor_supp.size(), &coor_supp[0],
			flag_sign, num_digits, false);

		break;
	}
}

void __fastcall TCompositeBeamMainForm ::cmb_bx_impactChange(TObject *Sender)
{
	draw_diagram();
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm ::rd_grp_internal_forces_typeClick(TObject *Sender)
{
	draw_diagram();
}
void TCompositeBeamMainForm ::calculate_composite_beam_SP35()
{
	TSteelSectionFormCntrlsState const & cntrls_state = SteelSectionForm -> cntrls_state();

	std::unique_ptr<GeneralSteelSection const> st_sect{
		new WeldedSection{cntrls_state.edt_b_f1_, cntrls_state.edt_t_f1_,
						  cntrls_state.edt_b_f2_, cntrls_state.edt_t_f2_,
						  cntrls_state.edt_h_w_, cntrls_state.edt_t_w_}};

	if(cntrls_state.pg_cntrl_sect_type_ == 0)
		st_sect.reset(new RolledSection{cntrls_state.rd_grp_rolled_sect_type_,
										cntrls_state.cmb_bx_rolled_sect_num_});


	Steel st {update_steel_i_section()};

	Geometry geom {update_geometry()};
	std::unique_ptr<GeneralConcreteSection const> conc_sect = update_concrete_section(
	geom.get_span(), geom.get_spacing_left(), geom.get_spacing_right(), geom.is_end_beam(), st_sect -> b_f2());
	Concrete conc = ConcreteDefinitionForm -> get_concrete();

	double SW_corr_sheet = 0.;
	double SW_st_sect = st_sect -> SW();
	double SW_conc_sect = conc_sect -> SW(conc.get_density());
	if(dynamic_cast<CorrugatedConcreteSection const *>(conc_sect.get()))
		SW_corr_sheet = static_cast<CorrugatedConcreteSection const *>(conc_sect.get())
			-> corrugated_sheet().get_weight();

	  ComposSectGeomSP35 com_sect {st, *st_sect,
								   conc, *conc_sect,
								   ComposSectGeomSP35::ConcStateConsid::normal};
	  ComposSectGeomSP35 com_sect_sh {st, *st_sect,
									  conc, *conc_sect,
									  ComposSectGeomSP35::ConcStateConsid::shrink};
	  ComposSectGeomSP35 com_sect_cr {st, *st_sect,
									  conc, *conc_sect,
									  ComposSectGeomSP35::ConcStateConsid::creep};


	  //подготовка калькулятора внутренних усилий
	SupportsNumber tmp_sup_num = geom.get_temporary_supports_number();
	double L = geom.get_span();
	double B = geom.get_trib_width();
	Loads loads{update_loads()};
	loads.set_B(B);
	IntForcesCalculator int_frcs_calculator{tmp_sup_num, L, B, loads };

	WorkingConditionsFactors working_conditions_factors{update_working_conditions_factors()};

	  ComposSectCalculatorSP35 com_beam_calc {int_frcs_calculator,
												   working_conditions_factors,
												   com_sect,
												   com_sect_sh,
												   com_sect_cr};



	  std::vector<double> const x_lst {0, L/4,  L/3, L/2, 2*L/3, 3*L/4, L};

	  ComBeamOutputSP35 const com_beam_output = com_beam_calc.calculate(x_lst);

	  TWord_Automation report = TWord_Automation("ReportCompositeBeamSP35.docx");

	  geom.print_data_to_report(report);
	  loads.print_data_to_report(report);
	  working_conditions_factors.print_data_to_report_SP35(report);


	  com_beam_output.print_data_to_report(report);

//#ifndef NDEBUG
//	  com_sect.print_data_to_logger(*frm_logger_);
//	  com_sect_sh.print_data_to_logger(*frm_logger_);
//	  com_sect_cr.print_data_to_logger(*frm_logger_);
//
//	  for(auto  & sect_output:sect_output_lst)
//		sect_output.print_data_to_logger(*frm_logger_);
//#endif



}

void TCompositeBeamMainForm::calculate_composite_beam()
{
	try
	{
		update_composite_beam();
	}
	catch (int rc)
	{
		return;
	}

	try
	{
		composite_beam_calculator_.calculate();
	}
	catch (String str)
	{
		Application -> MessageBox(str.w_str(),
								  L"Расчёт невозможен",
								  MB_OK | MB_ICONERROR);
		return;
	}


	draw_diagram();
	fill_steel_sect_geometr_grid();
	fill_concrete_sect_geometr_grid();
	fill_composite_sect_geometr_grid();
	fill_results_grid();

	btn_report->Enabled=true;
	tb_results->TabVisible=true;

	PgCntrlCompositeBeam -> ActivePage = TbShtStaticScheme;

	
}
//---------------------------------------------------------------------------



void __fastcall TCompositeBeamMainForm ::NNewClick(TObject *Sender)
{
	int i;
	if (modify_project) {
		 i=Application->MessageBox(L"Сохранить текущий проект?", L" ",
				  MB_YESNO | MB_ICONQUESTION);
		 if (i==IDYES) NSaveClick(Sender);
	}
	strcpy(ModelFile, UNTITLED);
	modify_project = false;

	Caption = "Расчет комбинированной балки - [Новый проект]";

}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm ::NSaveClick(TObject *Sender)
{
	update_composite_beam(); //актуализируем композитную балку из полей формы
   // Получение имени директории, в которой находится исполняемый модуль

   if  (strcmp(ModelFile, UNTITLED)==0) {
	  if(SaveDialog_Model->Execute())
	  {
		  FileDir_Name = SaveDialog_Model->FileName;;//поле класс хранит путь полный
		  ModelName(FileDir_Name.c_str(), ModelFile); //выделяет имя файла для отображения на форме из пути
	  }
   }

	std::ofstream ofstr {FileDir_Name.c_str(), std::ios::out | std::ios::binary};
	calculate_composite_beam();

	composite_beam_calculator_.save(ofstr);
	ofstr.close();

   Caption = "Расчет комбинированной балки - " + AnsiString(ModelFile);

   modify_project = false;
}

void __fastcall TCompositeBeamMainForm ::NSaveAsClick(TObject *Sender)
{
	strcpy(ModelFile, UNTITLED);
	NSaveClick(Sender);
}
//---------------------------------------------------------------------------
//@--------------------------------------------------------------------------
// Выделение из имени файла в имени модели
void ModelName(char * str0, char* ModelFile)
{
	  char *ptr1, *ptr2;
	 // char i, str[240];
	 char str[240]; //MV 15.04.2020

	  if  (strcmp(ModelFile, UNTITLED)==0) {
	   //i= 240<strlen(str0) ? 240 : strlen(str0);
	   240<strlen(str0) ? 240 : strlen(str0); //MV 15.04.2020
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

void __fastcall TCompositeBeamMainForm ::NOpenClick(TObject *Sender)
{

   NNewClick(Sender);

   if(OpenDialog_Model->Execute())
   {
	  FileDir_Name = OpenDialog_Model->FileName;
   }
   if (FileDir_Name!="") {

	  strcpy(ModelFile, UNTITLED);

	  std::ifstream ifstr {FileDir_Name.c_str(), std::ios::in | std::ios::binary};

	  composite_beam_calculator_.load(ifstr);

	  ifstr.close();

	  set_form_controls();

      calculate_composite_beam();

	  ModelName(FileDir_Name.c_str(), ModelFile);

	  Caption = "Расчет комбинированной балки - " + AnsiString(ModelFile);

	  modify_project = false;

   }

}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm ::clean_static_scheme()
{
   	img_static_scheme -> Canvas -> Brush -> Color = clWhite;
	img_static_scheme -> Canvas -> FillRect (img_static_scheme -> Canvas -> ClipRect);
   //img_static_scheme -> Picture = nullptr;  нет ли утечки памяти?
}
void TCompositeBeamMainForm ::clean_grid(TStringGrid* str_grd)
{
	for(int i =1; i < str_grd -> RowCount; ++i)
	   str_grd -> Cells [1][i] = "";
}
void __fastcall TCompositeBeamMainForm ::OnControlsChange(TObject *Sender)
{
	if (btn_report->Enabled)
		btn_report->Enabled=false;
	if(tb_results->TabVisible)
		tb_results->TabVisible=false;
	clean_static_scheme();
	clean_grid(strng_grd_compos_sect_geom_character);
	clean_grid(strng_grd_concrete_sect_geom_character);
	clean_grid(strng_grd_steel_sect_geom_character);
	clean_grid(strng_grd_results);

}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm ::chck_bx_end_beamClick(TObject *Sender)
{
	if (chck_bx_end_beam->Checked){
		lbl_trib_width_left->Caption="Свес плиты, мм:";
		lbl_trib_width_right->Caption="Расстояние до балки справа, мм:";
		}
	else{
		lbl_trib_width_left->Caption="Расстояние до балки слева, мм:";
		lbl_trib_width_right->Caption="Расстояние до балки справа, мм:";
	}
	OnControlsChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm ::cmb_bx_analysis_theoryChange(TObject *Sender)

{
    OnControlsChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm ::cmb_bx_corrugated_sheeting_part_numberChange(TObject *Sender)
{
	OnControlsChange(Sender);
}

//---------------------------------------------------------------------------
void TCompositeBeamMainForm ::update(IPublisher* ipublisher )
{
	switch(ipublisher -> get_id())
	{
		case(Publisher_ID::REBARS_FORM):
			pnl_rebar_viewer -> Caption = ipublisher -> get_information();
			OnControlsChange(nullptr);
			break;
		case(Publisher_ID::STUDS_FORM):
			pnl_shear_stud_viewer -> Caption = ipublisher -> get_information();
			OnControlsChange(nullptr);
			break;
		case(Publisher_ID::CONCRETE_FORM):
			pnl_concrete_grade -> Caption = ipublisher -> get_information();
			OnControlsChange(nullptr);
			break;
		case(Publisher_ID::STEEL_FORM):
			pnl_steel -> Caption = ipublisher -> get_information();
			OnControlsChange(nullptr);
			break;
		case(Publisher_ID::SECTION_FORM):
			Pnl_SteelSectionViewer -> Caption = ipublisher -> get_information();
			OnControlsChange(nullptr);
			break;
	}

}

//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm ::btn_loggerClick(TObject *Sender)
{
   //	FormLogger->Show();
   frm_logger_ -> Show();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------



void __fastcall TCompositeBeamMainForm ::N8Click(TObject *Sender)
{
	AboutProgForm->ShowModal();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//Обработчик события обеспечивающий заполнение первой строки жирным шрифтом
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm::strng_grd_first_raw_bold(TObject *Sender,
          int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
	TStringGrid* str_grid=static_cast<TStringGrid*>(Sender);
	if (ARow == 0)
	{
		str_grid -> Canvas -> Font -> Style = TFontStyles() << fsBold;
		str_grid -> Canvas -> TextOut(Rect.Left+3, Rect.Top+5, str_grid -> Cells[ACol][ARow]);
	}
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm::btn_add_impactsClick(TObject *Sender)
{
	FrmAddImpacts -> Show();
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm::HelpClick(TObject *Sender)
{
	WideString path = ExtractFilePath(Application -> ExeName) + "Руководство Пользователя Комбинированная Балка v.1.0.0.pdf";

	HelpForm -> CppWebBrowser1 -> Navigate((BSTR)path);

	HelpForm -> ShowModal();
}
//---------------------------------------------------------------------------

