//---------------------------------------------------------------------------
#include <math.h>

#pragma hdrstop

#include "ReportCopmpositeBeam.h"
#include "Word_Automation.h"

static    Variant vTabl;
static    Variant my_tables;
static    Variant my_table;

typedef struct _data_comp {  // ���������
		int num_prof;
		double h;
		double hw;
		double b;
} Data_Comp;

static    Data_Comp DC = {1, 300, 285, 150};


//------------------------------------------------------------------
int ReportCompositeBeam() {
  int i, k, rc;

  Screen->Cursor = crHourGlass;
  // �������� ������� Word
  Word_Automation.GetActiveWord();
  // �������� ���������
  rc = Word_Automation.CreateDocument("ReportCompositeBeam.docx");
  if (rc>0)
	 return rc;

  // �������� ���� ��������
  Word_Automation.this_doc.OleProcedure("Select");
  // �������� ������ �� ������ ���������
  Word_Automation.my_selection = Word_Automation.my_word.OlePropertyGet("Selection");
  //--------------------------------------------------
  // ������ ������� ����� *.doc
  // Word_Automation.PasteFilePattern("Report_x.doc", "%FILE%");

  Word_Automation.PasteTextPattern(IntToStr(DC.num_prof), "%profile_number%");
  Word_Automation.PasteTextPattern(FloatToStr(DC.h*0.1), "%h%");
  Word_Automation.PasteTextPattern(FloatToStr(DC.hw*0.1), "%h_w%");
  Word_Automation.PasteTextPattern(FloatToStr(DC.b*0.1), "%b%");


  Screen->Cursor = crDefault;


  return 0;
}

