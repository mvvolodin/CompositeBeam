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
#include "DefineSteelFormUnit.h"
#include "StudDefinitionFormUnit.h"
#include "ConcreteDefinitionFormUnit.h"
#include "RebarDefinitionFormUnit.h"

 #include "DrawEpurMain.h" //���������� ������� ��������� ����
 #include "Word_Automation.h"
#include <Vcl.Imaging.jpeg.hpp>//���������� ���� ������� ������� �����
 //@
void ModelName(char * str0, char *ModelFile);  // ��������� �� ����� ����� � ����� ������
//@@
class TCompositeBeamMainForm : public TForm
{

__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *NFile;
	TMenuItem *N3;
	TButton *BtnCalculate;
	TButton *btn_report;
	TLabel *lbl_beam_division;
	TEdit *edt_beam_division;
	TMenuItem *NOpen;
	TMenuItem *NSave;
	TMenuItem *NSaveAs;
	TMenuItem *NOutReport;
	TMenuItem *N8;
	TSaveDialog *SaveDialog_Model;
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
	TImage *img_static_scheme;
	TLabel *lblLoadCase;
	TComboBox *cmb_bx_LC;
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
	TPanel *pnl_rebar_viewer;
	TBitBtn *BtBtnRebarsChoice;
	TGroupBox *GrpBxShearStuds;
	TPanel *pnl_shear_stud_viewer;
	TBitBtn *BtBtnShearStudsChoice;
	TGroupBox *GroupBox6;
	TImage *Image1;
	TStringGrid *strng_grd_compos_sect_geom_character;
	TTabSheet *tb_results;
	TStringGrid *strngGrdResults;
	TOpenDialog *OpenDialog1;
	TPageControl *pg_ctrl_geom_char;
	TTabSheet *tb_sht_composite_geom_char;
	TTabSheet *tb_sht_concrete_geom_char;
	TTabSheet *tb_sht_steel_geom_char;
	TStringGrid *strng_grd_concrete_sect_geom_character;
	TStringGrid *strng_grd_steel_sect_geom_character;

	void __fastcall BtnCalculateClick(TObject *Sender);
	void __fastcall btn_reportClick(TObject *Sender);
	void __fastcall rdgrp_slab_typeClick(TObject *Sender);
	void __fastcall strng_grd_rendering(TObject *Sender, int ACol, int ARow, TRect &Rect,
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
	void __fastcall cmb_bx_LCChange(TObject *Sender);
	void __fastcall rd_grp_internal_forces_typeClick(TObject *Sender);
	void __fastcall NNewClick(TObject *Sender);
	void __fastcall NSaveClick(TObject *Sender);
	void __fastcall NSaveAsClick(TObject *Sender);
	void __fastcall NOpenClick(TObject *Sender);
	void __fastcall OnControlsChange(TObject *Sender);
	void __fastcall chck_bx_end_beamClick(TObject *Sender);
	void __fastcall CmbBxAnalysisTheoryChoiceChange(TObject *Sender);
	void __fastcall ComboBox2Change(TObject *Sender);

public:		// User declarations
	__fastcall TCompositeBeamMainForm(TComponent* Owner);

private:

	TCompositeBeam composite_beam_; //�������� ������ � ���������  // ����������������� private �������

	TGeometry init_geomet();//������������� ���������
	TLoads init_loads(); //������������� ��������
	TISectionInitialData init_i_section();//������������� ������� ��������� ��������
	TSteelInitialData init_steel_i_section(); //������������� ����� ��������
	TConcretePart* init_concrete_part();//������������� �������� ����� ������������ �������
	TStud init_stud();//������������� ������
	WorkingConditionsFactors init_working_conditions_factors();//������������� ������������� ������� ������
	CompositeSection init_composite_section(TGeometry geometry,
											TSteelInitialData steel_i_section_initial_data,
											TISectionInitialData i_section_initial_data,
											TConcretePart* concrete_part);//������������� ������������ �������
	void init_composite_beam(TGeometry geometry,
									   TLoads loads,
									   CompositeSection composite_section,
									   TStud stud,
									   WorkingConditionsFactors working_conditions_factors);
	void generate_report();
	void grid_constructor_ratios(); // ��������������� Grid �������� ��� ������ �����������
	void grid_constr_comp_sect_geometr(); //��������������� Grid ��� ������ �������������� ������. ����. �������
	void grid_constr_steel_sect_geometr();
	void grid_constr_concrete_sect_geometr();
	void fill_cmb_bx_LC();//��������� ComboBox ���������� ������� ���������� � ���������������� ��������� ���� ������������ ������� ����������
	void fill_grid_with_results();
	void draw_diagram();//��������� ����
	void calculate_composite_beam();//�������������� � ������������ �����

	//@----------------------------------------------------------------
	#define UNTITLED  "��� �����"
	bool modify_project;  // ������� ��������� ������� ����� ����������
	char ModelFile[240];
	AnsiString FileDir_Name;

	//@@
};
//---------------------------------------------------------------------------
extern PACKAGE TCompositeBeamMainForm *CompositeBeamMainForm;
//---------------------------------------------------------------------------
#endif
