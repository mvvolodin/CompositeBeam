//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <process.h>
#include <time.h>
#include <io.h>
#include <dir.h>
#include <errno.h>
#include <fcntl.h>
#include <Clipbrd.hpp>
#include <sys\stat.h>
#include <math.h>
#pragma hdrstop


#include "uFrmAboutProg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAboutProgForm *AboutProgForm;
static AnsiString Month[12]={"������","�������","�����","������","���","����","����","�������","��������","�������","������","�������"};
//---------------------------------------------------------------------------
__fastcall TAboutProgForm::TAboutProgForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAboutProgForm::FormShow(TObject *Sender)
{
	 struct ftime ft;
     FILE *file_;
     char stroka[120];
	 char min[4];

	 if ((file_ = fopen("CompositeBeamApp.exe", "r")) != NULL)  {
       // ���������� ���� exe-�����
       getftime(fileno(file_), &ft);
       if (ft.ft_min <10)
         sprintf(min, "0%d",ft.ft_min);
       else
         sprintf(min, "%d",ft.ft_min);
       sprintf(stroka, "%d %s %d �, %d:%s ",
		  ft.ft_day, Month[ft.ft_month-1].c_str(),
          ft.ft_year+1980,
          ft.ft_hour, min
       );
     // ���������� ����
	   Label_Prog->Caption = "������ 1.0 �� " + AnsiString(stroka);
     }
     else {
        //InfMessage("���������� ���������� ���� ������������ ����� ");
        //return;
     }

}
//---------------------------------------------------------------------------
