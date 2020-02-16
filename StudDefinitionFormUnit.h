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
#include "UnitStud.h"
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TStudDefinitionForm : public TForm
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
	TBitBtn *bt_btn_stud_choice;
	TLabel *lbl_stud_safety_factor;
	TEdit *edt_stud_safety_factor;
	TPanel *pnl_stud_part_number;
	void __fastcall bt_btn_stud_choiceClick(TObject *Sender);
	void __fastcall cmb_bx_stud_part_numberChange(TObject *Sender);
private:
   //	Studs stud_;
	void fill_stud_data();
    void init_stud();
public:
	__fastcall TStudDefinitionForm(TComponent* Owner);
   //	inline Studs get_stud()const{return stud_;};
};
//---------------------------------------------------------------------------
extern PACKAGE TStudDefinitionForm *StudDefinitionForm;
//---------------------------------------------------------------------------
#endif
