﻿//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include "uFrmCompositeBeam.h"
#include <fstream>
#include <System.Win.Registry.hpp>

#include "uCompBeamObjsCreatorSP35.h"

#include "uCompSectsCalculatorSP35.h"
#include "uCompSectsOutputSP35.h"
#include "uStudsSP35Calculator.h"
#include "uStudsSP35Calculated.h"

#include "uCompBeamObjsCreatorSP266.h"
#include "uCompSectsCalculatorSP266.h"
#include "uCompSectsOutputSP266.h"
#include "uStudsSP266Calculator.h"
#include "uStudsSP266Calculated.h"
//---------------------------------------------------------------------------
TCompositeBeamMainForm  *CompositeBeamMainForm;

extern std::wstring file_path;
const wchar_t* RegKey = L"Software\\CompositeBeam_v_2.1";

CompSectsOutputSP35 comp_sects_output_SP35;
StudsSP35Calculated studs_SP35_output;

CompSectsOutputSP266 comp_sects_output_list_SP266;
StudsSP266Calculated studs_SP266_output;

//----------------------------------------------------------------------
 _fastcall TCompositeBeamMainForm ::TCompositeBeamMainForm (TComponent* Owner)
	: TForm(Owner)
{
	#ifdef DEBUG_ENABLED
    btn_logger -> Visible = true;
	#endif
	read_mru_files_list();

	cotr_comp_sect_geometr_grid();
	cotr_steel_sect_geometr_grid();
	ctor_concrete_sect_geometr_grid();

	fill_cmb_bx_impact();

	fill_cmb_bx_corrugated_sheets();

}
 _fastcall TCompositeBeamMainForm ::~TCompositeBeamMainForm ()
{
	write_mru_files_list();
}
//----------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm::FormShow(TObject *Sender)
{
	if(!file_path.empty())
		open(file_path);
	file_path.clear();//чтобы не загрузить данные снова в случае события FormShow

	initialize_GUI();
	initialize_forms_controls();

    store_forms_controls();
	calculate_composite_beam();
	after_calculation();
#ifdef DEBUG_ENABLED
	FormLogger -> Show();
#endif
}
void TCompositeBeamMainForm::open(std::wstring const & fp)
{
	std::ifstream ifs {fp, std::ios::in | std::ios::binary};

	cntrls_state_.load(ifs);
	SteelSectionForm -> load(ifs);
	ConcreteDefinitionForm -> load(ifs);
	RebarDefinitionForm -> load(ifs);
	StudDefinitionForm -> load(ifs);
	DefineSteelForm -> load(ifs);

	ifs.close();

	Caption = L"Расчет комбинированной балки - " + String{fp.c_str()};
}
//---------------------------------------------------------------------------
void __fastcall TCompositeBeamMainForm ::BtnCalculateClick(TObject *Sender)
{
	try {
		store_forms_controls();
		calculate_composite_beam();

	} catch (int rc) {
		if(rc == 2)
			Application -> MessageBox(
				L"Условие применимости метода плоской плиты:\n"
				L"Eb * Ib <= 0.2 Est * Is не выполняется.\n"
				L"Необходимо изменить исходные данные!",
				L"Предупреждение!",
				MB_OK | MB_ICONWARNING);

		return;
	}

	after_calculation();
}
//---------------------------------------------------------------------------
void __fastcall TCompositeBeamMainForm ::btn_reportClick(TObject *Sender)
{
	Screen -> Cursor = crHourGlass;//На время создания отчёта присвоем курсору вид часов
	switch(cntrls_state_.rd_grp_code_data_)
	{
		case 0: generate_report_SP266();
				break;

		case 1:  generate_report_SP35();
				break;
	}
	Screen -> Cursor = crDefault;//Возвращаем курсору вид по умолчанию
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
	TStringGrid* str_gr = static_cast<TStringGrid*>(Sender);

	switch (rd_grp_code -> ItemIndex) {

	case(0):
		render_ratios_grid_SP266(str_gr, ACol, ARow, Rect);
		break;
	case(1):
		render_ratios_grid_SP35(str_gr, ACol, ARow, Rect);
		break;
	}

}
//---------------------------------------------------------------------------
//	Функция заполняющая TStringGrid выводящий результаты расчёта геометрических характеристик композитного сечения
//---------------------------------------------------------------------------
void TCompositeBeamMainForm ::cotr_comp_sect_geometr_grid()
{
	strng_grd_compos_sect_geom_character->Cells [0][0]=L"Геометрические характеристики";
	strng_grd_compos_sect_geom_character->Cells [1][0]=L"Значения";
	strng_grd_compos_sect_geom_character->Cells [0][1]=L"Площадь, мм2";
	strng_grd_compos_sect_geom_character->Cells [0][2]=L"Момент инерции, мм4 ";
	strng_grd_compos_sect_geom_character->Cells [0][3]=L"Момент сопротивления Ц.Т. ж.б. плиты, мм3";
	strng_grd_compos_sect_geom_character->Cells [0][4]=L"Расстояние от Ц.Т. до Ц.Т. бетона, мм";
	strng_grd_compos_sect_geom_character->Cells [0][5]=L"Расстояние от Ц.Т. до Ц.Т. стали мм";
	strng_grd_compos_sect_geom_character->Cells [0][6]=L"Расстояние от Ц.Т. стали до Ц.Т. бетона мм";
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
}
void TCompositeBeamMainForm::clean_grid(TStringGrid* str_gr)
{
	for( int i = 0; i < str_gr -> ColCount; ++i)
		str_gr -> Cols[i] -> Clear();
}
//---------------------------------------------------------------------------
//	Функция заполняющая Grid выводящий результаты расчёта композитной балки
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::cotr_ratios_grid_SP266()
{
	clean_grid(strng_grd_results);

	strng_grd_results -> RowCount = 18;

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
//	Функция заполняющая Grid выводящий результаты расчёта композитной балки
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::cotr_ratios_grid_SP35()
{
	clean_grid(strng_grd_results);

	strng_grd_results -> RowCount = 15;

	strng_grd_results -> Cells [0][0] = u"Проверка";
	strng_grd_results -> Cells [0][1] = u"На действие изгибающих моментов при монтаже";
	strng_grd_results -> Cells [0][2] = u"      Координата критического сечения, мм";
	strng_grd_results -> Cells [0][3] = u"      Прочность";
	strng_grd_results -> Cells [0][4] = u"На действие изгибающих моментов, раздел";
	strng_grd_results -> Cells [0][5] = u"      Координата критического сечения, мм";
	strng_grd_results -> Cells [0][6] = u"      Прочность верхнего пояса стального сечения";
	strng_grd_results -> Cells [0][7] = u"      Прочность нижнего пояса стального сечения";
	strng_grd_results -> Cells [0][8] = u"      Прочность железобетона";
	strng_grd_results -> Cells [0][9] = u"Прочности на действие поперечной силы";
	strng_grd_results -> Cells [0][10] = u"      Координата критического сечения, мм";
	strng_grd_results -> Cells [0][11] = u"      Прочность сечения, раздел";
	strng_grd_results -> Cells [0][12] = u"Упоров объединения";
	strng_grd_results -> Cells [0][13] = u"      Координата критического упора, мм";
	strng_grd_results -> Cells [0][14] = u"      Прочность упора";

	strng_grd_results -> Cells [1][0] = u"Коэффициенты Использования (КИ) ";
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
		cmb_bx_corrugated_sheeting_part_number -> Items -> Add(corrugated_sheet);

	cmb_bx_corrugated_sheeting_part_number->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TCompositeBeamMainForm ::BtBtnExitClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TCompositeBeamMainForm ::BtBtnSteelChoiceClick(TObject *Sender)
{
	 DefineSteelForm -> ShowModal();
	 pnl_steel -> Caption = DefineSteelForm -> info();
}
//---------------------------------------------------------------------------
void __fastcall TCompositeBeamMainForm ::BtnConcreteChoiceClick(TObject *Sender)
{
	ConcreteDefinitionForm -> ShowModal();
	pnl_concrete_grade -> Caption = ConcreteDefinitionForm -> info();
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm ::BtBtnRebarsChoiceClick(TObject *Sender)
{
	RebarDefinitionForm -> ShowModal();
	pnl_rebar_viewer -> Caption = RebarDefinitionForm -> info();
}
//---------------------------------------------------------------------------
void __fastcall TCompositeBeamMainForm ::BtBtnShearStudsChoiceClick(TObject *Sender)
{
	StudDefinitionForm -> ShowModal();
	pnl_shear_stud_viewer -> Caption = StudDefinitionForm -> info();
}
//---------------------------------------------------------------------------
void __fastcall TCompositeBeamMainForm ::BtnSteelSectionChoiceClick(TObject *Sender)
{
	SteelSectionForm -> ShowModal();
	pnl_steel_section_viewer -> Caption = SteelSectionForm -> info();

}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm ::NOutReportClick(TObject *Sender)
{
	btn_reportClick(nullptr);
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm ::NExitClick(TObject *Sender)
{
	Close();
}

void TCompositeBeamMainForm::generate_report_SP35()
{
	TWord_Automation report = TWord_Automation("ReportCompositeBeamSP35.dotx");

	comp_sects_output_SP35.print(report);

	studs_SP35_output.print(report);
}
void TCompositeBeamMainForm::generate_report_SP266()
{
	TWord_Automation report = TWord_Automation("ReportCompositeBeamSP266.dotx");

	comp_sects_output_list_SP266.print(report);
	studs_SP266_output.print(report);
}
void TCompositeBeamMainForm::draw_diagram_SP266()
{
	std::vector<double> M;
	std::vector<double> Q;
	std::vector<double> R;
	std::vector<double> f;

	std::vector<double>	coor_supp;

	switch (cmb_bx_impact -> ItemIndex)
	{
	case(0): // Нагрузки 1a стадии

		M = comp_sects_output_list_SP266.M_1a_lst();
		Q = comp_sects_output_list_SP266.Q_1a_lst();
		R = comp_sects_output_list_SP266.R_1a_lst();
		f = comp_sects_output_list_SP266.f_1a_lst();

		coor_supp = comp_sects_output_list_SP266.sup_coord();

		break;
	case(1): // Нагрузки 1b стадии

		M = comp_sects_output_list_SP266.M_1b_lst();
		Q = comp_sects_output_list_SP266.Q_1b_lst();
		R = comp_sects_output_list_SP266.R_1b_lst();
		f = comp_sects_output_list_SP266.f_1b_lst();

		coor_supp = comp_sects_output_list_SP266.sup_coord();

		break;
	case(2): // Нагрузки 2c стадии

		M = comp_sects_output_list_SP266.M_2c_lst();
		Q = comp_sects_output_list_SP266.Q_2c_lst();
		R = comp_sects_output_list_SP266.R_2c_lst();
		f = comp_sects_output_list_SP266.f_2c_lst();

		coor_supp = comp_sects_output_list_SP266.end_sup_coord();

		break;

	case(3): // Нагрузки 2d стадии

		M = comp_sects_output_list_SP266.M_2d_lst();
		Q = comp_sects_output_list_SP266.Q_2d_lst();
		R = comp_sects_output_list_SP266.R_2d_lst();
		f = comp_sects_output_list_SP266.f_2d_lst();

		coor_supp = comp_sects_output_list_SP266.end_sup_coord();

		break;

	case(4)://Нагрузки полные

		M = comp_sects_output_list_SP266.M_total_lst();
		Q = comp_sects_output_list_SP266.Q_total_lst();
		R = comp_sects_output_list_SP266.R_total_lst();
		f = comp_sects_output_list_SP266.f_total_lst();

		coor_supp = comp_sects_output_list_SP266.end_sup_coord();

		break;
	}

	{

	using namespace units;

	std::transform(M.begin(), M.end(), M.begin(),
				   [](auto & M0){ return M0 * kN * m ;});
	std::transform(Q.begin(), Q.end(), Q.begin(),
				   [](auto & Q0){ return Q0 * kN ;});
	std::transform(R.begin(), R.end(), R.begin(),
				   [](auto & R0){ return R0 * kN;});
	}

	TImage *Image1=img_static_scheme;
	bool flag_sign = true;
	int num_digits = 2;
	bool con_sign_practice = true;

	std::vector<double> coor_epur = comp_sects_output_list_SP266.x_lst();

	switch (rd_grp_internal_forces_type -> ItemIndex)
	{
	case(0):

		DrawEpur(Image1,
				 M.size(),
				 coor_epur.data(),
				 M.data(),
				 nullptr,
				 coor_supp.size(),
				 coor_supp.data(),
				 flag_sign,
				 num_digits,
				 con_sign_practice);


		break;

	case(1):

		DrawEpur(Image1,
				 Q.size(),
				 coor_epur.data(),
				 Q.data(), R.data(),
				 coor_supp.size(),
				 coor_supp.data(),
				 flag_sign,
				 num_digits,
				 con_sign_practice);

		break;

	case(2):

		DrawEpur(Image1,
				 f.size(),
				 coor_epur.data(),
				 f.data(), nullptr,
				 coor_supp.size(),
				 coor_supp.data(),
				 flag_sign, num_digits,
				 false);

		break;
	case(3):

		DrawEpur(Image1,
				 studs_SP266_output.S_overline_lst_.size(),
				 studs_SP266_output.coord_.data(),
				 studs_SP266_output.S_overline_lst_.data(),
				 nullptr,
				 coor_supp.size(),
				 coor_supp.data(),
				 flag_sign,
				 num_digits,
				 false);

		break;
	}
}
void TCompositeBeamMainForm::draw_diagram_SP35()
{
	std::vector<double> M;
	std::vector<double> Q;
	std::vector<double> R;
	std::vector<double> f;

	std::vector<double>	coor_supp;

	switch (cmb_bx_impact -> ItemIndex)
	{
	case(0): // Нагрузки 1a стадии

		M = comp_sects_output_SP35.M_1a_lst();
		Q = comp_sects_output_SP35.Q_1a_lst();
		R = comp_sects_output_SP35.R_1a_lst();
		f = comp_sects_output_SP35.f_1a_lst();

		coor_supp = comp_sects_output_SP35.sup_coord();

		break;
	case(1): // Нагрузки 1b стадии

		M = comp_sects_output_SP35.M_1b_lst();
		Q = comp_sects_output_SP35.Q_1b_lst();
		R = comp_sects_output_SP35.R_1b_lst();
		f = comp_sects_output_SP35.f_1b_lst();

		coor_supp = comp_sects_output_SP35.sup_coord();

		break;
	case(2): // Нагрузки 2c стадии

		M = comp_sects_output_SP35.M_2c_lst();
		Q = comp_sects_output_SP35.Q_2c_lst();
		R = comp_sects_output_SP35.R_2c_lst();
		f = comp_sects_output_SP35.f_2c_lst();

		coor_supp = comp_sects_output_SP35.end_sup_coord();

		break;

	case(3): // Нагрузки 2d стадии

		M = comp_sects_output_SP35.M_2d_lst();
		Q = comp_sects_output_SP35.Q_2d_lst();
		R = comp_sects_output_SP35.R_2d_lst();
		f = comp_sects_output_SP35.f_2d_lst();

		coor_supp = comp_sects_output_SP35.end_sup_coord();

		break;

	case(4)://Нагрузки полные

		M = comp_sects_output_SP35.M_total_lst();
		Q = comp_sects_output_SP35.Q_total_lst();
		R = comp_sects_output_SP35.R_total_lst();
		f = comp_sects_output_SP35.f_total_lst();

		coor_supp = comp_sects_output_SP35.end_sup_coord();

		break;
	}

	{

	using namespace units;

	std::transform(M.begin(), M.end(), M.begin(),
				   [](auto & M0){ return M0 * kN * m ;});
	std::transform(Q.begin(), Q.end(), Q.begin(),
				   [](auto & Q0){ return Q0 * kN ;});
	std::transform(R.begin(), R.end(), R.begin(),
				   [](auto & R0){ return R0 * kN;});
	}

	TImage *Image1=img_static_scheme;
	bool flag_sign = true;
	int num_digits = 2;
	bool con_sign_practice = true;

	std::vector<double> coor_epur = comp_sects_output_SP35.x_lst();

	switch (rd_grp_internal_forces_type -> ItemIndex)
	{
	case(0):

		DrawEpur(Image1,
				 M.size(),
				 coor_epur.data(),
				 M.data(),
				 nullptr,
				 coor_supp.size(),
				 coor_supp.data(),
				 flag_sign,
				 num_digits,
				 con_sign_practice);


		break;

	case(1):

		DrawEpur(Image1,
				 Q.size(),
				 coor_epur.data(),
				 Q.data(), R.data(),
				 coor_supp.size(),
				 coor_supp.data(),
				 flag_sign,
				 num_digits,
				 con_sign_practice);

		break;

	case(2):

		DrawEpur(Image1,
				 f.size(),
				 coor_epur.data(),
				 f.data(), nullptr,
				 coor_supp.size(),
				 coor_supp.data(),
				 flag_sign, num_digits,
				 false);

		break;
	case(3):

		DrawEpur(Image1,
				 studs_SP35_output.S_overline_lst_.size(),
				 studs_SP35_output.coord_.data(),
				 studs_SP35_output.S_overline_lst_.data(),
				 nullptr,
				 coor_supp.size(),
				 coor_supp.data(),
				 flag_sign,
				 num_digits,
				 false);

		break;
	}

}

void __fastcall TCompositeBeamMainForm ::cmb_bx_impactChange(TObject *Sender)
{
	switch(cntrls_state_.rd_grp_code_data_){
		case 0: draw_diagram_SP266();
				break;

		case 1:	draw_diagram_SP35();
	}
}
//---------------------------------------------------------------------------

void __fastcall TCompositeBeamMainForm ::rd_grp_internal_forces_typeClick(TObject *Sender)
{
	switch(cntrls_state_.rd_grp_code_data_){
		case 0: draw_diagram_SP266();
				break;

		case 1:	draw_diagram_SP35();
				break;
	}
}
void TCompositeBeamMainForm::calculate_composite_beam()
{
	switch(cntrls_state_.rd_grp_code_data_)
	{
		case 0:
			calculate_composite_sections_SP266();
			calculate_studs_SP266();
			break;
		case 1:
			calculate_composite_sections_SP35();
			calculate_studs_SP35();
			break;
	}
}
void TCompositeBeamMainForm::calculate_composite_sections_SP266()
{
	CompBeamObjsCreatorSP266 creator {
		this -> cntrls_state_,
		ConcreteDefinitionForm -> cntrls_state(),
		SteelSectionForm -> cntrls_state(),
		DefineSteelForm -> cntrls_state(),
		RebarDefinitionForm -> cntrls_state(),
		StudDefinitionForm -> cntrls_state()}; //объект 488 байта

	CompSectsCalculatorSP266 calculator {creator};
	comp_sects_output_list_SP266 = calculator.run(); //объект 560 байт
}
void TCompositeBeamMainForm::calculate_studs_SP266()
{
	CompBeamObjsCreatorSP266 creator {this -> cntrls_state_,
		ConcreteDefinitionForm -> cntrls_state(),
		SteelSectionForm -> cntrls_state(),
		DefineSteelForm -> cntrls_state(),
		RebarDefinitionForm -> cntrls_state(),
		StudDefinitionForm -> cntrls_state()};//объект 488 байта

		StudsSP266Calculator calculator {creator};
		studs_SP266_output = calculator.run();
}
void TCompositeBeamMainForm ::calculate_composite_sections_SP35()
{
	CompBeamObjsCreatorSP35 creator{this -> cntrls_state_,
		 ConcreteDefinitionForm -> cntrls_state(),
		 SteelSectionForm -> cntrls_state(),
		 DefineSteelForm -> cntrls_state(),
		 RebarDefinitionForm -> cntrls_state(),
		 StudDefinitionForm -> cntrls_state()};

	CompSectsCalculatorSP35 calculator {creator};
	comp_sects_output_SP35 = calculator.run();
}

void TCompositeBeamMainForm::calculate_studs_SP35()
{
	CompBeamObjsCreatorSP35 creator {this -> cntrls_state_,
		ConcreteDefinitionForm -> cntrls_state(),
		SteelSectionForm -> cntrls_state(),
		DefineSteelForm -> cntrls_state(),
		RebarDefinitionForm -> cntrls_state(),
		StudDefinitionForm -> cntrls_state()};

	StudsSP35Calculator calculator {creator};
	studs_SP35_output = calculator.run();
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::after_calculation()
{
	switch(cntrls_state_.rd_grp_code_data_)
	{
		case 0:	update_SW_edts_SP266();

				draw_diagram_SP266();

				update_steel_sect_geometr_grid_SP266();
				update_concrete_sect_geometr_grid_SP266();
				update_composite_sect_geometr_grid_SP266();

				update_results_grid_SP266();
				break;

		case 1: update_SW_edts_SP35();

				draw_diagram_SP35();

				update_steel_sect_geometr_grid_SP35();
				update_concrete_sect_geometr_grid_SP35();
				update_composite_sect_geometr_grid_SP35();

				update_results_grid_SP35();
				break;
	}
	btn_report -> Enabled = true;
	tb_results -> TabVisible = true;

	PgCntrlCompositeBeam -> ActivePage = TbShtStaticScheme;
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm ::update_SW_edts_SP266()
{
	using namespace units;
	edt_SW_steel_beam -> Text = double_to_str(
		comp_sects_output_list_SP266.loads_.SW_steel_beam());
	edt_SW_conc -> Text = double_to_str(
		comp_sects_output_list_SP266.loads_.SW_concrete()* kN / m2);
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm ::update_SW_edts_SP35()
{
	using namespace units;
	edt_SW_steel_beam -> Text = double_to_str(
		comp_sects_output_SP35.loads_.SW_steel_beam());
	edt_SW_conc -> Text = double_to_str(
		comp_sects_output_SP35.loads_.SW_concrete()* kN / m2);
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::update_results_grid_SP266()
{
	comp_sects_output_list_SP266.fill_ratios_grid(strng_grd_results);
	studs_SP266_output.fill_grid(strng_grd_results);
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::update_results_grid_SP35()
{
	comp_sects_output_SP35.fill_ratios_grid(strng_grd_results);
	studs_SP35_output.fill_grid(strng_grd_results);
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::update_steel_sect_geometr_grid_SP35()
{
	comp_sects_output_SP35.
		fill_comp_sect_grid(strng_grd_steel_sect_geom_character);
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::update_steel_sect_geometr_grid_SP266()
{
	comp_sects_output_list_SP266.
		fill_steel_sect_grid(strng_grd_steel_sect_geom_character);
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm ::update_concrete_sect_geometr_grid_SP266()
{
	comp_sects_output_list_SP266.
		fill_conc_sect_grid(strng_grd_concrete_sect_geom_character);
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm ::update_concrete_sect_geometr_grid_SP35()
{
	comp_sects_output_SP35.
		fill_conc_sect_grid(strng_grd_concrete_sect_geom_character);
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::update_composite_sect_geometr_grid_SP266()
{
	comp_sects_output_list_SP266.comp_sect_geom_.
		fill_comp_sect_grid(strng_grd_compos_sect_geom_character);
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::update_composite_sect_geometr_grid_SP35()
{
	comp_sects_output_SP35.
		fill_comp_sect_grid(strng_grd_compos_sect_geom_character);
}
//---------------------------------------------------------------------------
void __fastcall TCompositeBeamMainForm ::NNewClick(TObject *Sender)
{
	int i;
	if (is_proj_modified) {
		 i=Application->MessageBox(L"Сохранить текущий проект?", L" ",
				  MB_YESNO | MB_ICONQUESTION);
		 if (i==IDYES) NSaveClick(Sender);
	}
	wcscpy(ModelFile, UNTITLED);
	is_proj_modified = false;

	Caption = "Расчет комбинированной балки - [Новый проект]";
}
//---------------------------------------------------------------------------
void __fastcall TCompositeBeamMainForm ::NSaveClick(TObject *Sender)
{
	store_forms_controls(); //актуализируем композитную балку из полей формы
   // Получение имени директории, в которой находится исполняемый модуль

   if  (wcscmp(ModelFile, UNTITLED)==0) {
	  if(SaveDialog_Model->Execute())
	  {
		  FileDir_Name = SaveDialog_Model->FileName;;//поле класс хранит путь полный
		  ModelName(FileDir_Name.c_str(), ModelFile); //выделяет имя файла для отображения на форме из пути
	  }
   }

	std::ofstream ofs {FileDir_Name.c_str(), std::ios::out | std::ios::binary};
	calculate_composite_beam();

	cntrls_state_.save(ofs);

	SteelSectionForm -> save(ofs);
	ConcreteDefinitionForm -> save(ofs);
	RebarDefinitionForm -> save(ofs);
	StudDefinitionForm -> save(ofs);
	DefineSteelForm -> save(ofs);

	ofs.close();

   Caption = "Расчет комбинированной балки - " + AnsiString(ModelFile);

   is_proj_modified = false;
}

void __fastcall TCompositeBeamMainForm ::NSaveAsClick(TObject *Sender)
{
	wcscpy(ModelFile, UNTITLED);
	NSaveClick(Sender);
}
//---------------------------------------------------------------------------
//@--------------------------------------------------------------------------
// Выделение из имени файла в имени модели
void ModelName(wchar_t * str0, wchar_t* ModelFile)
{
	  wchar_t *ptr1, *ptr2;
	 // char i, str[240];
	 wchar_t str[240]; //MV 15.04.2020

	  if  (wcscmp(ModelFile, UNTITLED)==0) {
	   //i= 240<strlen(str0) ? 240 : strlen(str0);
	   240<wcslen(str0) ? 240 : wcslen(str0); //MV 15.04.2020
	   wcscpy(str,str0);
	   ptr1 = wcsrchr(str,'\\');
	   ptr2 = wcsrchr(ptr1,'.');
	   if (ptr2!=NULL)
		 *ptr2='\0';
	   if (ptr1==NULL)
		 wcscpy(ModelFile, str);
	   else
		 wcscpy(ModelFile, ptr1+1);
	  }
}
bool TCompositeBeamMainForm ::is_already_opened(std::wstring const & fp)
{
	return fp == mru_files_.back();
}
void __fastcall TCompositeBeamMainForm ::NOpenClick(TObject *Sender)
{
//   NNewClick(Sender);

   if(OpenDialog_Model->Execute())
	  FileDir_Name = OpenDialog_Model -> FileName;

//   if (is_already_opened(FileDir_Name.c_str()))
//		return;

	if (FileDir_Name!="") {

		add_mru_file(FileDir_Name.c_str());

		open(FileDir_Name.c_str());

		initialize_GUI();
		initialize_forms_controls();

		store_forms_controls();
		calculate_composite_beam();
		after_calculation();

		wcscpy(ModelFile, UNTITLED);

		ModelName(FileDir_Name.c_str(), ModelFile);

		Caption = "Расчет комбинированной балки - " + AnsiString(ModelFile);

		is_proj_modified = false;

   }


}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm ::clean_static_scheme()
{
	img_static_scheme -> Canvas -> Brush -> Color = clWhite;
	img_static_scheme -> Canvas -> FillRect (img_static_scheme -> Canvas -> ClipRect);
   //img_static_scheme -> Picture = nullptr;  нет ли утечки памяти?
}
void TCompositeBeamMainForm ::clean_2nd_col_grid(TStringGrid* str_grd)
{
	for(int i =1; i < str_grd -> RowCount; ++i)
	   str_grd -> Cells [1][i] = "";
}

void __fastcall TCompositeBeamMainForm::OnControlsChange(TObject *Sender)
{
	if (btn_report -> Enabled)
		btn_report -> Enabled=false;
	if(tb_results -> TabVisible)
		tb_results -> TabVisible=false;
	clean_static_scheme();
	clean_2nd_col_grid(strng_grd_compos_sect_geom_character);
	clean_2nd_col_grid(strng_grd_concrete_sect_geom_character);
	clean_2nd_col_grid(strng_grd_steel_sect_geom_character);
	clean_2nd_col_grid(strng_grd_results);

    is_proj_modified = true;
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
void __fastcall TCompositeBeamMainForm ::N8Click(TObject *Sender)
{
	AboutProgForm->ShowModal();
}
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
	WideString path = ExtractFilePath(Application -> ExeName) + "Руководство Пользователя Комбинированная Балка v.2.1.pdf";

	HelpForm -> CppWebBrowser1 -> Navigate((BSTR)path);

	HelpForm -> Show();
}

void TCompositeBeamMainForm::update_panels_info()
{
	pnl_steel_section_viewer -> Caption = SteelSectionForm -> info();
	pnl_rebar_viewer -> Caption = RebarDefinitionForm -> info();
	pnl_concrete_grade -> Caption = ConcreteDefinitionForm -> info();
	pnl_steel -> Caption = DefineSteelForm -> info();
	pnl_shear_stud_viewer -> Caption = StudDefinitionForm -> info();
}

//---------------------------------------------------------------------------
void TCompositeBeamMainForm::initialize_controls()
{
	set_controls();
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::update_controls()
{
	set_controls();
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::set_controls()
{
	// Параметры расчёта
	rd_grp_code -> ItemIndex = cntrls_state_.rd_grp_code_data_;
	edt_max_elem_length -> Text = cntrls_state_.edt_max_elem_length_data_;

	// Геометрия
	edt_span -> Text = cntrls_state_.edt_span_data_;
	edt_width_left -> Text = cntrls_state_.edt_width_left_data_;
	edt_width_right -> Text = cntrls_state_.edt_width_right_data_;
	chck_bx_end_beam -> Checked = cntrls_state_.chck_bx_end_beam_data_;
	cmb_bx_number_propping_supports -> ItemIndex = cntrls_state_.cmb_bx_number_propping_supports_data_;

	// Нагрузки
	edt_SW_add_concrete -> Text = cntrls_state_.edt_SW_add_concrete_data_;
	edt_dead_load_first_stage -> Text = cntrls_state_.edt_dead_load_first_stage_data_;
	edt_dead_load_second_stage -> Text = cntrls_state_.edt_dead_load_second_stage_data_;
	edt_live_load -> Text = cntrls_state_.edt_live_load_data_;

	// Коэффициенты надёжности по нагрузке

	edt_gamma_f_st_SW -> Text = cntrls_state_.edt_gamma_f_st_SW_data_;
	edt_gamma_f_concrete_SW -> Text = cntrls_state_.edt_gamma_f_concrete_SW_data_;
	edt_gamma_f_add_concrete_SW -> Text = cntrls_state_.edt_gamma_f_add_concrete_SW_data_;
	edt_gamma_f_DL_I -> Text = cntrls_state_.edt_gamma_f_DL_I_data_;
	edt_gamma_f_DL_II -> Text = cntrls_state_.edt_gamma_f_DL_II_data_;
	edt_gamma_f_LL -> Text = cntrls_state_.edt_gamma_f_LL_data_;

	// Коэффициенты условий работы

	edt_gamma_c -> Text = cntrls_state_.edt_gamma_c_data_;
	edt_gamma_bi -> Text = cntrls_state_.edt_gamma_bi_data_;
	edt_gamma_si -> Text = cntrls_state_.edt_gamma_si_data_;

	// Прочие коэффициенты

	edt_sheeting_continuity_coefficient -> Text = cntrls_state_.edt_sheeting_continuity_coefficient_data_;
	edt_fact_quasi_perm_load -> Text = cntrls_state_.edt_fact_quasi_perm_load_data_;

	// Отрисовка эпюр

	cmb_bx_impact -> ItemIndex = cntrls_state_.cmb_bx_impact_data_;
	rd_grp_internal_forces_type -> ItemIndex = cntrls_state_.rd_grp_internal_forces_type_data_;

	//Тип жб плиты

	rdgrp_slab_type -> ItemIndex = cntrls_state_.rdgrp_slab_type_data_;

	//Плита по настилу

	cmb_bx_corrugated_sheeting_part_number -> ItemIndex = cntrls_state_.cmb_bx_corrugated_sheeting_part_number_data_;
	edt_h_f -> Text = cntrls_state_.edt_h_f_data_;
	chck_bx_wider_flange_up -> Checked = cntrls_state_.chck_bx_wider_flange_up_data_;
	chck_bx_sheet_orient_along -> Checked = cntrls_state_.chck_bx_sheet_orient_along_data_;

	//Плита плоская

	edt_h_f_flat -> Text = cntrls_state_.edt_h_f_flat_data_;
	edt_h_n -> Text = cntrls_state_.edt_h_n_data_;

	//Панели для отображения данных
	update_panels_info();
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::store_controls()
{
	int rc = 0;
	// Геометрия
	rc = String_double_plus(lbl_span -> Caption,
							edt_span -> Text,
							&cntrls_state_.edt_span_data_);
	if(rc > 0) throw(rc);

	rc = String_double_plus(lbl_trib_width_left -> Caption,
							edt_width_left -> Text,
							&cntrls_state_.edt_width_left_data_);
	if(rc > 0) throw(rc);

	rc = String_double_plus(lbl_trib_width_right -> Caption,
							edt_width_right -> Text,
							&cntrls_state_.edt_width_right_data_);
	if(rc > 0) throw(rc);

	cntrls_state_.chck_bx_end_beam_data_ = chck_bx_end_beam -> Checked;
	cntrls_state_.cmb_bx_number_propping_supports_data_ = cmb_bx_number_propping_supports -> ItemIndex;

	// Нагрузки
	rc = String_double_zero_plus(lbl_SW_add_concrete -> Caption,
								 edt_SW_add_concrete -> Text,
								 &cntrls_state_.edt_SW_add_concrete_data_);
   if(rc > 0) throw(rc);

	rc = String_double_zero_plus(lbl_dead_load_first_stage -> Caption,
								 edt_dead_load_first_stage -> Text,
								 &cntrls_state_.edt_dead_load_first_stage_data_);
	if(rc > 0) throw(rc);

	rc = String_double_zero_plus(lbl_dead_load_second_stage -> Caption,
								 edt_dead_load_second_stage -> Text,
								 &cntrls_state_.edt_dead_load_second_stage_data_);
	if(rc > 0) throw(rc);

	rc = String_double_zero_plus(lbl_live_load -> Caption,
								 edt_live_load -> Text,
								 &cntrls_state_.edt_live_load_data_);
	if(rc > 0) throw(rc);
	// Коэффициенты надёжности по нагрузке
	rc = String_double_zero_plus(lbl_gamma_f_st_SW -> Caption,
								 edt_gamma_f_st_SW -> Text,
								 &cntrls_state_.edt_gamma_f_st_SW_data_);
	if(rc > 0) throw(rc);

	rc = String_double_zero_plus(lbl_gamma_f_concrete_SW -> Caption,
								 edt_gamma_f_concrete_SW -> Text,
								 &cntrls_state_.edt_gamma_f_concrete_SW_data_);
	if(rc > 0) throw(rc);

	rc = String_double_zero_plus(lbl_gamma_f_add_concrete_SW -> Caption,
								 edt_gamma_f_add_concrete_SW -> Text,
								 &cntrls_state_.edt_gamma_f_add_concrete_SW_data_);
	if(rc > 0) throw(rc);

	rc = String_double_zero_plus(lbl_gamma_f_DL_I -> Caption,
								 edt_gamma_f_DL_I -> Text,
								 &cntrls_state_.edt_gamma_f_DL_I_data_);
	if(rc > 0) throw(rc);

	rc = String_double_zero_plus(lbl_gamma_f_DL_II -> Caption,
								 edt_gamma_f_DL_II -> Text,
								 &cntrls_state_.edt_gamma_f_DL_II_data_);
	if(rc > 0) throw(rc);

	rc = String_double_zero_plus(lbl_gamma_f_LL -> Caption,
								 edt_gamma_f_LL -> Text,
								 &cntrls_state_.edt_gamma_f_LL_data_);
	if(rc > 0) throw(rc);

	// Коэффициенты условий работы

	rc = String_double_zero_plus(lbl_gamma_c -> Caption,
								 edt_gamma_c -> Text,
								 &cntrls_state_.edt_gamma_c_data_);
	if(rc > 0) throw(rc);

	rc = String_double_zero_plus(lbl_gamma_bi -> Caption,
								 edt_gamma_bi -> Text,
								 &cntrls_state_.edt_gamma_bi_data_);
	if(rc > 0) throw(rc);

	rc = String_double_zero_plus(lbl_gamma_si -> Caption,
								 edt_gamma_si -> Text,
								 &cntrls_state_.edt_gamma_si_data_);
	if(rc > 0) throw(rc);
	// Прочие коэффициенты
	rc = String_double_zero_plus(lbl_sheeting_continuity_coefficient -> Caption,
								 edt_sheeting_continuity_coefficient -> Text,
								 &cntrls_state_.edt_sheeting_continuity_coefficient_data_);
	if(rc > 0) throw(rc);

	rc = String_double_zero_plus(lbl_fact_quasi_perm_load -> Caption,
								 edt_fact_quasi_perm_load -> Text,
								 &cntrls_state_.edt_fact_quasi_perm_load_data_);
	if(rc > 0) throw(rc);

    // Отрисовка эпюр

	cntrls_state_.cmb_bx_impact_data_ = cmb_bx_impact -> ItemIndex;
	cntrls_state_.rd_grp_internal_forces_type_data_ = rd_grp_internal_forces_type -> ItemIndex;

	// Параметры расчёта

	cntrls_state_.rd_grp_code_data_ = rd_grp_code -> ItemIndex;
	rc = String_double_zero_plus(lbl_max_elem_length -> Caption,
								 edt_max_elem_length -> Text,
								 &cntrls_state_.edt_max_elem_length_data_);
	if(rc > 0) throw(rc);

	//Тип жб плиты

	cntrls_state_.rdgrp_slab_type_data_ = rdgrp_slab_type -> ItemIndex ;

	//Плита по настилу
	rc = String_double_zero_plus(lbl_h_f -> Caption,
								 edt_h_f -> Text,
								 &cntrls_state_.edt_h_f_data_);
	if(rc > 0) throw(rc);

	cntrls_state_.cmb_bx_corrugated_sheeting_part_number_data_ = cmb_bx_corrugated_sheeting_part_number -> ItemIndex ;
	cntrls_state_.chck_bx_wider_flange_up_data_ = chck_bx_wider_flange_up -> Checked ;
	cntrls_state_.chck_bx_sheet_orient_along_data_ = chck_bx_sheet_orient_along -> Checked;

	//Плита плоская
	rc = String_double_zero_plus(lbl_h_f_flat -> Caption,
								 edt_h_f_flat -> Text,
								 &cntrls_state_.edt_h_f_flat_data_);
	if(rc > 0) throw(rc);

	rc = String_double_zero_plus(lbl_h_n -> Caption,
								 edt_h_n -> Text,
								 &cntrls_state_.edt_h_n_data_);
	if(rc > 0) throw(rc);

}
void TCompositeBeamMainForm::initialize_forms_controls()
{
	SteelSectionForm -> update_cntrls_state();
	RebarDefinitionForm -> update_cntrls_state();
	ConcreteDefinitionForm -> update_cntrls_state();
	DefineSteelForm -> update_cntrls_state();
	StudDefinitionForm -> update_cntrls_state();

	initialize_controls();
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::update_forms_controls()
{
	SteelSectionForm -> update_cntrls_state();
	RebarDefinitionForm -> update_cntrls_state();
	ConcreteDefinitionForm -> update_cntrls_state();
	DefineSteelForm -> update_cntrls_state();
	StudDefinitionForm -> update_cntrls_state();
	//второстепенные формы обновляются первыми для того, чтобы верно отобразить информацию на панелях
	update_controls();
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::set_forms_controls()
{
	SteelSectionForm -> update_cntrls_state();
	RebarDefinitionForm -> update_cntrls_state();
	ConcreteDefinitionForm -> update_cntrls_state();
	DefineSteelForm -> update_cntrls_state();
	StudDefinitionForm -> update_cntrls_state();
	//второстепенные формы обновляются первыми для того, чтобы верно отобразить информацию на панелях
	set_controls();
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::store_forms_controls()
{
		store_controls();

		SteelSectionForm -> store_cntrls_state();
		RebarDefinitionForm -> store_cntrls_state();
		ConcreteDefinitionForm -> store_cntrls_state();
		DefineSteelForm -> store_cntrls_state();
		StudDefinitionForm -> store_cntrls_state();

}
void TCompositeBeamMainForm::initialize_GUI()
{
	GUI const init = static_cast<GUI>(cntrls_state_.rd_grp_code_data_);

	GUI gui_init = gui_;

	switch (init){

	case(GUI::SP266):

		set_GUI_SP266();
		ConcreteDefinitionForm -> set_GUI_SP266();
		StudDefinitionForm -> set_GUI_SP266();

		break;
	case(GUI::SP35):

		set_GUI_SP35();
		ConcreteDefinitionForm -> set_GUI_SP35();
		StudDefinitionForm -> set_GUI_SP35();

		break;
	}

	gui_ = init;
}


void TCompositeBeamMainForm::update_GUI(GUI new_gui)
{
	if(gui_ == new_gui)//требуется для того, чтобы избежать смены GUI
		return;        // после инитиализации полей, к примеру при событии
                       //переключение радио кнопки с нормами
	switch (new_gui){

	case(GUI::SP266):

		set_GUI_SP266();
		ConcreteDefinitionForm -> set_GUI_SP266();
		StudDefinitionForm -> set_GUI_SP266();
		update_panels_info();
		messages_after_GUI_SP266_set();

		break;
	case(GUI::SP35):

		set_GUI_SP35();
		ConcreteDefinitionForm -> set_GUI_SP35();
		StudDefinitionForm -> set_GUI_SP35();
        update_panels_info();
		messages_after_GUI_SP35_set();

		break;
	}

	gui_ = new_gui;
}
void __fastcall TCompositeBeamMainForm::rd_grp_codeClick(TObject *Sender)
{
	OnControlsChange(nullptr);

	int const code_index = static_cast<TRadioGroup*>(Sender) -> ItemIndex;

	update_GUI(static_cast<GUI>(code_index));
}
void TCompositeBeamMainForm::set_GUI_SP35()
{
	cotr_ratios_grid_SP35();

	rdgrp_slab_type -> ItemIndex = 0;
	rdgrp_slab_type -> Buttons [1] -> Visible = false;

	grp_bx_corrugated_slab -> Visible = false;

	btn_add_impacts -> Visible = false;

	edt_sheeting_continuity_coefficient -> Visible = false;
	lbl_sheeting_continuity_coefficient -> Visible = false;
	edt_fact_quasi_perm_load -> Top = 22;
	lbl_fact_quasi_perm_load -> Top = 22;
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::set_GUI_SP266()
{
	cotr_ratios_grid_SP266();

	rdgrp_slab_type -> ItemIndex = 0;
	rdgrp_slab_type -> Buttons [1] -> Visible = true;

	grp_bx_corrugated_slab -> Visible = true;

	btn_add_impacts -> Visible = true;

	edt_sheeting_continuity_coefficient -> Visible = true;
	lbl_sheeting_continuity_coefficient -> Visible = true;
	edt_fact_quasi_perm_load -> Top = 62;
	lbl_fact_quasi_perm_load -> Top = 62;

}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::messages_after_GUI_SP266_set()
{
	Application -> MessageBox(L"Класс прочности бетона изменён на \"B10\"!",
		L"Предупреждение!", MB_OK | MB_ICONWARNING);
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::messages_after_GUI_SP35_set()
{
	Application -> MessageBox(L"Класс прочности бетона изменён на \"B20\"!",
		L"Предупреждение!", MB_OK | MB_ICONWARNING);
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::render_ratios_grid_SP266(TStringGrid* str_gr, int ACol,
	int ARow, TRect &Rect)
{
    	if (ARow == 0)
	{
		str_gr -> Canvas -> Font -> Style = TFontStyles() << fsBold;
		str_gr -> Canvas -> TextOut(Rect.Left+3, Rect.Top+5, str_gr -> Cells[ACol][ARow]);
	}
	if (ARow == 1 && ACol == 0)
	{
		str_gr -> Canvas -> Font -> Style = TFontStyles() << fsBold << fsUnderline << fsItalic;
		str_gr -> Canvas -> TextOut(Rect.Left+3, Rect.Top+5, str_gr -> Cells[ACol][ARow]);
	}
	if (ARow == 4 && ACol == 0)
	{
		str_gr -> Canvas -> Font -> Style = TFontStyles() << fsBold << fsUnderline << fsItalic;
		str_gr -> Canvas -> TextOut(Rect.Left+3, Rect.Top+5, str_gr -> Cells[ACol][ARow]);
	}
	if (ARow == 9 && ACol == 0)
	{
		str_gr -> Canvas -> Font -> Style = TFontStyles() << fsBold << fsUnderline << fsItalic;
		str_gr -> Canvas -> TextOut(Rect.Left+3, Rect.Top+5, str_gr -> Cells[ACol][ARow]);
	}
	if (ARow == 12 && ACol == 0)
	{
		str_gr -> Canvas -> Font -> Style = TFontStyles() << fsBold << fsUnderline << fsItalic;
		str_gr -> Canvas -> TextOut(Rect.Left+3, Rect.Top+5, str_gr -> Cells[ACol][ARow]);
	}
	if (ARow == 15 && ACol == 0)
	{
		str_gr -> Canvas -> Font -> Style = TFontStyles() << fsBold << fsUnderline << fsItalic;
		str_gr -> Canvas -> TextOut(Rect.Left+3, Rect.Top+5, str_gr -> Cells[ACol][ARow]);
	}
	if (ARow == 1 && ACol == 1)
	{
		str_gr -> Canvas -> Brush -> Color = clBtnFace;
		str_gr -> Canvas -> FillRect(Rect);
	}
	if (ARow == 4 && ACol == 1)
	{
		str_gr -> Canvas -> Brush -> Color = clBtnFace;
		str_gr -> Canvas -> FillRect(Rect);
	}
	if (ARow == 9 && ACol == 1)
	{
		str_gr -> Canvas -> Brush -> Color = clBtnFace;
		str_gr -> Canvas -> FillRect(Rect);
	}
	if (ARow == 12 && ACol == 1)
	{
		str_gr -> Canvas -> Brush -> Color = clBtnFace;
		str_gr -> Canvas -> FillRect(Rect);
	}
	if (ARow == 15 && ACol == 1)
	{
		str_gr -> Canvas -> Brush -> Color = clBtnFace;
		str_gr -> Canvas -> FillRect(Rect);
	}

}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::render_ratios_grid_SP35(TStringGrid* str_gr, int ACol,
	int ARow, TRect &Rect)
{
	if (ARow == 0)
	{
		str_gr -> Canvas -> Font -> Style = TFontStyles() << fsBold;
		str_gr -> Canvas -> TextOut(Rect.Left+3, Rect.Top+5, str_gr -> Cells[ACol][ARow]);
	}
	if (ARow == 1 && ACol == 0)
	{
		str_gr -> Canvas -> Font -> Style = TFontStyles() << fsBold << fsUnderline << fsItalic;
		str_gr -> Canvas -> TextOut(Rect.Left+3, Rect.Top+5, str_gr -> Cells[ACol][ARow]);
	}
	if (ARow == 4 && ACol == 0)
	{
		str_gr -> Canvas -> Font -> Style = TFontStyles() << fsBold << fsUnderline << fsItalic;
		str_gr -> Canvas -> TextOut(Rect.Left+3, Rect.Top+5, str_gr -> Cells[ACol][ARow]);
	}
	if (ARow == 9 && ACol == 0)
	{
		str_gr -> Canvas -> Font -> Style = TFontStyles() << fsBold << fsUnderline << fsItalic;
		str_gr -> Canvas -> TextOut(Rect.Left+3, Rect.Top+5, str_gr -> Cells[ACol][ARow]);
	}
	if (ARow == 12 && ACol == 0)
	{
		str_gr -> Canvas -> Font -> Style = TFontStyles() << fsBold << fsUnderline << fsItalic;
		str_gr -> Canvas -> TextOut(Rect.Left+3, Rect.Top+5, str_gr -> Cells[ACol][ARow]);
	}
	if (ARow == 1 && ACol == 1)
	{
		str_gr -> Canvas -> Brush -> Color = clBtnFace;
		str_gr -> Canvas -> FillRect(Rect);
	}
	if (ARow == 4 && ACol == 1)
	{
		str_gr -> Canvas -> Brush -> Color = clBtnFace;
		str_gr -> Canvas -> FillRect(Rect);
	}
	if (ARow == 9 && ACol == 1)
	{
		str_gr -> Canvas -> Brush -> Color = clBtnFace;
		str_gr -> Canvas -> FillRect(Rect);
	}
	if (ARow == 12 && ACol == 1)
	{
		str_gr -> Canvas -> Brush -> Color = clBtnFace;
		str_gr -> Canvas -> FillRect(Rect);
	}
}
//---------------------------------------------------------------------------
void __fastcall TCompositeBeamMainForm::mru_file_path_click(TObject *Sender)
{
	TMenuItem* item_clicked  {dynamic_cast<TMenuItem*>(Sender)};

	int const index = mru0 -> MenuIndex - item_clicked -> MenuIndex;
	std::wstring const file_path {mru_files_[index]};

	open(file_path); //данные файла записаны в cntrls_state

	initialize_GUI();
	initialize_forms_controls();

	store_forms_controls();
	calculate_composite_beam();
	after_calculation();
}
void TCompositeBeamMainForm::add_mru_file(std::wstring const & fp)
{
	if (fp == mru_files_.back())
		return;

	if (mru_files_.size() == num_mru_files_)
		mru_files_.erase(mru_files_.begin());

	mru_files_.emplace_back(fp);
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::fill_mru_files_list()
{
	if(mru_files_.empty())
		return;

	MainMenu1-> Items -> Items[0] ->
		Items[mru4 -> MenuIndex - 1] -> Visible = true;

	for(int i = 0; i < mru_files_.size(); ++i)
		if(mru_files_[i] != L""){
			MainMenu1-> Items -> Items[0] -> Items[mru0 -> MenuIndex - i] ->
				Caption = mru_files_[i].c_str();
			MainMenu1-> Items -> Items[0] -> Items[mru0 -> MenuIndex - i] -> Visible = true;
		}
}
//---------------------------------------------------------------------------
void TCompositeBeamMainForm::write_mru_files_list()
{
	std::unique_ptr<TRegistry> reg = std::make_unique<TRegistry>();

	reg -> OpenKey(RegKey, true);

	std::wstring str1 = mru_files_[0];
	std::wstring str2 = mru_files_[1];
	std::wstring str3 = mru_files_[2];
	std::wstring str4 = mru_files_[3];
	std::wstring str5 = mru_files_[4];

	reg -> WriteString("MRU1", mru_files_[0].c_str());
	reg -> WriteString("MRU2", mru_files_[1].c_str());
	reg -> WriteString("MRU3", mru_files_[2].c_str());
	reg -> WriteString("MRU4", mru_files_[3].c_str());
	reg -> WriteString("MRU5", mru_files_[4].c_str());
}
void TCompositeBeamMainForm::read_mru_files_list()
{
	std::unique_ptr<TRegistry> reg = std::make_unique<TRegistry>();

	if (!reg -> OpenKey(RegKey, false)){
	reg -> OpenKey(RegKey, true);
	reg -> WriteString("MRU1", "");
	reg -> WriteString("MRU2", "");
	reg -> WriteString("MRU3", "");
	reg -> WriteString("MRU4", "");
	reg -> WriteString("MRU5", "");
	}

	mru_files_.push_back(reg -> ReadString("MRU1").c_str());
	mru_files_.push_back(reg -> ReadString("MRU2").c_str());
	mru_files_.push_back(reg -> ReadString("MRU3").c_str());
	mru_files_.push_back(reg -> ReadString("MRU4").c_str());
	mru_files_.push_back(reg -> ReadString("MRU5").c_str());
}

void __fastcall TCompositeBeamMainForm::NFileClick(TObject *Sender)
{
	fill_mru_files_list();
}
//---------------------------------------------------------------------------


void __fastcall TCompositeBeamMainForm::btn_loggerClick(TObject *Sender)
{
	#ifdef DEBUG_ENABLED
	FormLogger -> Show();
	#endif
}


//---------------------------------------------------------------------------


