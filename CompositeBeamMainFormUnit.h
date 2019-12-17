//---------------------------------------------------------------------------

#ifndef CompositeBeamMainFormUnitH
#define CompositeBeamMainFormUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.OleCtnrs.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ToolWin.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Graphics.hpp>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ���������������� ������������ �����
//---------------------------------------------------------------------------


#include "CompositeBeam.h"//���������� ������

#include "SteelDefinitionFormUnit.h"   //���������� �����
#include "SteelSectionFormUnit.h"
#include "StudDefinitionFormUnit.h"
#include "ConcreteDefinitionFormUnit.h"
#include "RebarDefinitionFormUnit.h"

 #include "DrawEpurMain.h" //���������� ������� ��������� ����

class TCompositeBeamMainForm : public TForm
{
// ���� ��������� IDE

__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *NFile;
	TMenuItem *N3;
	TButton *BtnCalculate;
	TButton *BtnReport;
	TLabel *lbl_beam_division;
	TEdit *edt_beam_division;
	TMenuItem *NOpen;
	TMenuItem *NSave;
	TMenuItem *NSaveAs;
	TMenuItem *NOutReport;
	TMenuItem *N8;
	TSaveDialog *SaveDialog1;
	TOpenDialog *OpenDialog_Model;
	TImageList *ImageList1;
	TToolBar *ToolBar1;
	TToolButton *ToolButtonOpen;
	TToolButton *ToolButtonSave;
	TToolButton *ToolButtonSaveAs;
	TToolButton *ToolButtonOutReport;
	TBitBtn *BtBtnExit;
	TMenuItem *NNew;
	TMenuItem *NExit;
	TMenuItem *N2;
	TToolButton *ToolButton1;
	TLabel *Label1;
	TPageControl *PgCntrlCompositeBeam;
	TTabSheet *TbShtStaticScheme;
	TGroupBox *GrpBxLoadsSafetyFactors;
	TLabel *lbl_gamma_f_DL_I;
	TLabel *lbl_gamma_f_DL_II;
	TLabel *Label2;
	TLabel *Label6;
	TLabel *lbl_gamma_f_LL;
	TEdit *edt_gamma_f_DL_I;
	TEdit *edt_gamma_f_DL_II;
	TEdit *edt_gamma_f_LL;
	TGroupBox *GrpBxLoadCases;
	TLabel *lbl_dead_load_first_stage;
	TLabel *lbl_dead_load_second_stage;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *lbl_live_load;
	TEdit *edt_dead_load_first_stage;
	TEdit *edt_dead_load_second_stage;
	TEdit *edt_live_load;
	TGroupBox *GrpBxTopology;
	TLabel *lbl_span;
	TLabel *lbl_trib_width_left;
	TLabel *lbl_trib_width_right;
	TEdit *edt_span;
	TEdit *edt_width_left;
	TEdit *edt_width_right;
	TCheckBox *chck_bx_end_beam;
	TGroupBox *GrpBxStaticScheme;
	TImage *ImgStaticScheme;
	TLabel *lblLoadCase;
	TComboBox *cmb_bx_LC;
	TButton *btn_draw_diagram;
	TRadioGroup *rd_grp_internal_forces_type;
	TGroupBox *GrpBxErection;
	TLabel *lbl_number_propping_supports;
	TComboBox *cmb_bx_number_propping_supports;
	TGroupBox *GrpBxShearStudsPlacement;
	TLabel *Label25;
	TLabel *Label26;
	TEdit *Edit1;
	TEdit *Edit2;
	TGroupBox *GrpBxAnalysisTheory;
	TComboBox *CmbBxAnalysisTheoryChoice;
	TGroupBox *grp_bx_working_conditions_factors;
	TLabel *lbl_gamma_c;
	TLabel *lbl_gamma_bi;
	TLabel *lbl_gamma_si;
	TEdit *edt_gamma_bi;
	TEdit *edt_gamma_si;
	TEdit *edt_gamma_c;
	TTabSheet *TbShtSectionMaterials;
	TGroupBox *GrpBxSteelSection;
	TPanel *Pnl_SteelSectionViewer;
	TBitBtn *BtnSteelSectionChoice;
	TRadioGroup *rdgrp_slab_type;
	TGroupBox *grp_bx_flat_slab;
	TLabel *lbl_flat_slab_thickness;
	TEdit *edt_flat_slab_thickness;
	TGroupBox *grpBxCorrugatedSlab;
	TComboBox *ComboBox2;
	TGroupBox *GrpBxSteel;
	TPanel *PnlSteelViewer;
	TBitBtn *BtBtnSteelChoice;
	TGroupBox *GrpBxConcrete;
	TPanel *pnl_concrete_grade;
	TBitBtn *BtnConcreteChoice;
	TGroupBox *GrpBxRebars;
	TPanel *PnlRebarsViewer;
	TBitBtn *BtBtnRebarsChoice;
	TGroupBox *GrpBxShearStuds;
	TPanel *PnlShearStudsViewer;
	TBitBtn *BtBtnShearStudsChoice;
	TGroupBox *GroupBox6;
	TImage *Image1;
	TStringGrid *strng_grd_compos_sect_geom_character;
	TTabSheet *TbResults;
	TStringGrid *strngGrdResults;
	TLabel *Label3;
	TEdit *Edit3;

	void __fastcall chck_bx_end_beamClick(TObject *Sender);
	void __fastcall BtnCalculateClick(TObject *Sender);
	void __fastcall BtnReportClick(TObject *Sender);
	void __fastcall rdgrp_slab_typeClick(TObject *Sender);
	void __fastcall strngGrdResultsDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
		  TGridDrawState State);
	void __fastcall BtBtnSteelChoiceClick(TObject *Sender);
	void __fastcall BtnSteelSectionChoiceClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BtBtnExitClick(TObject *Sender);
	void __fastcall BtnConcreteChoiceClick(TObject *Sender);
	void __fastcall BtBtnRebarsChoiceClick(TObject *Sender);
	void __fastcall BtBtnShearStudsChoiceClick(TObject *Sender);
	void __fastcall NOutReportClick(TObject *Sender);
	void __fastcall NExitClick(TObject *Sender);
	void __fastcall btn_draw_diagramClick(TObject *Sender);


private:	//����������� ����

	TGeometry geometry_;//���� ���������� ���������
	TLoads loads_;//���� ���������� �������� � ������������ ��������� �� ���������
	TStud stud_; //���� ��������� ����� ��������
	TISectionInitialData i_section_initial_data_;
	TSteelInitialData steel_i_section_initial_data_;
	WorkingConditionsFactors working_conditions_factors_;
	TConcretePart* concrete_part_;//������ ������������ ������, ������� ���������!
	CompositeSection composite_section_; //�������������� �������������� ������������ �������
	TCompositeBeam composite_beam_; //�������� ������ � ���������, ����������� �� ��������� �������� �������� ����
//---------------------------------------------------------------------------

public:		// User declarations
	__fastcall TCompositeBeamMainForm(TComponent* Owner);
//---------------------------------------------------------------------------
//���������������� privat �������
//---------------------------------------------------------------------------
private:  // ����������������� private �������

	void init_geomet();//������������� ���������
	void init_loads(); //������������� ��������
	void init_i_section();//������������� ������� ��������� ��������
	void init_steel_i_section(); //������������� ����� ��������
	void init_concrete_part();//������������� �������� ����� ������������ �������
	void init_composite_section();//������������� ������������ �������
	void init_composite_beam();//������������� ����������� �����
	void init_stud();//������������� ������
	void init_working_conditions_factors();//������������� ������������� ������� ������

	void generate_report();

	void grid_constructor_ratios(); // ��������������� Grid �������� ��� ������ �����������
	void grid_constr_comp_sect_geometr(); //��������������� Grid ��� ������ �������������� ������. ����. �������
	void fill_cmb_bx_LC();//��������� ComboBox ���������� ������� ���������� � ���������������� ��������� ���� ������������ ������� ����������

	void fill_grid_with_results();
};
//---------------------------------------------------------------------------
extern PACKAGE TCompositeBeamMainForm *CompositeBeamMainForm;
//---------------------------------------------------------------------------
#endif
