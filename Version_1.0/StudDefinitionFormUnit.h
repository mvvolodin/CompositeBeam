//---------------------------------------------------------------------------

#ifndef StudDefinitionFormUnitH
#define StudDefinitionFormUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
#include "uStud.h"
#include <Vcl.ExtCtrls.hpp>

#include "ObserverPatternInterfaces.h"

//---------------------------------------------------------------------------
class TStudDefinitionForm : public TForm, public IPublisher
{
__published:	// IDE-managed Components
	TLabel *lbl_stud_yield_strength;
	TLabel *lbl_stud_diameter;
	TLabel *lbl_stud_height;
	TEdit *edt_stud_yield_strength;
	TEdit *edt_stud_diameter;
	TEdit *edt_stud_height;
	TComboBox *cmb_bx_stud_part_number;
	TLabel *lbl_stud_part_number;
	TBitBtn *btn_ok;
	TLabel *lbl_stud_safety_factor;
	TEdit *edt_stud_safety_factor;
	TPanel *pnl_stud_part_number;
	TButton *btn_cancel;
	TGroupBox *grp_bx_placement;
	TLabel *lbl_edge_studs_dist;
	TLabel *lbl_middle_studs_dist;
	TLabel *edt_edge_studs_rows_num;
	TLabel *lbl_middle_studs_rows_num;
	TEdit *edt_edge_studs_dist;
	TEdit *edt_middle_studs_dist;
	TComboBox *cmb_bx_middle_studs_rows_num;
	TComboBox *cmb_bx_edge_studs_rows_num;
	TGroupBox *grp_bx_characteristics;
	TButton *btn_close;
	TCheckBox *chck_bx_more_than_one_stud_per_corrugation_edge;
	TCheckBox *chck_bx_more_than_one_stud_per_corrugation_middle;
	void __fastcall btn_okClick(TObject *Sender);
	void __fastcall cmb_bx_stud_part_numberChange(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btn_cancelClick(TObject *Sender);
	void __fastcall btn_closeClick(TObject *Sender);
private:
	static const Publisher_ID id_ = Publisher_ID::STUDS_FORM;
	IObserver_* iobserver_;
	StudsOnBeam studs_on_beam_temp_;

	void fill_stud_data();
	void set_studs();
	void set_form_controls();
	virtual String get_information()const override {return studs_on_beam_temp_.get_name();}
	virtual Publisher_ID get_id()const override {return id_;}
public:
	__fastcall TStudDefinitionForm(TComponent* Owner)override;
	StudsOnBeam get_studs_on_beam()const{return studs_on_beam_temp_;}
	void register_observer(IObserver_* iobserver) override {iobserver_ = iobserver;}
	void set_form_controls(StudsOnBeam studs_on_beam);
};
//---------------------------------------------------------------------------
extern PACKAGE TStudDefinitionForm *StudDefinitionForm;
//---------------------------------------------------------------------------
#endif
