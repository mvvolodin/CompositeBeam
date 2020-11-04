//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uFrmLogger.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormLogger *FormLogger;
//---------------------------------------------------------------------------
__fastcall TFormLogger::TFormLogger(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TFormLogger::print_sections_coordinates(double id, double x)
{
	String str = L"id = " + FloatToStrF(id, ffFixed, 15, 0) + "; " +
				 L"x = " + FloatToStrF(x, ffFixed, 15, 0) + "; " ;
	mm_logger -> Lines -> Add(str);
}
void TFormLogger::print_M_X(double x, double M)
{
		String str = L"x = " + FloatToStrF(x, ffFixed, 15, 0) + "; " +
					 L"M = " + FloatToStrF(M / 1000000, ffFixed, 15, 2);
		mm_logger -> Lines -> Add(str);
}
void TFormLogger::print_M_X(std::vector<double> M_list)
{
	for(auto M:M_list)
	{
		String str = L"M = " + FloatToStrF(M / 1000000, ffFixed, 15, 2);
		mm_logger -> Lines -> Add(str);
	}
}
void TFormLogger::print_sigma_b_sigma_s_X(double x, double sigma_b, double sigma_s)
{
		String str = L"x = " + FloatToStrF(x, ffFixed, 15, 0) + "; " +
					 L"sigma_b = " + FloatToStrF(sigma_b, ffFixed, 15, 2)+
					 L"sigma_s = " + FloatToStrF(sigma_s, ffFixed, 15, 2);
		mm_logger -> Lines -> Add(str);
}

void TFormLogger::print_ratios(double x, double upper_fl_ratio, double lower_fl_ratio,
		double conc_ratio, double shear_ratio)
{
		String str = L"x = " + FloatToStrF(x, ffFixed, 15, 0) + "; " +
					 L"upper_fl_ratio = " + FloatToStrF(upper_fl_ratio, ffFixed, 15, 2)+
					 L"lower_fl_ratio = " + FloatToStrF(lower_fl_ratio, ffFixed, 15, 2)+
					 L"conc_ratio = " + FloatToStrF(conc_ratio, ffFixed, 15, 2)+
					 L"shear_ratio = " + FloatToStrF(shear_ratio, ffFixed, 15, 2);
		mm_logger -> Lines -> Add(str);
}
void TFormLogger::clean_logger()
{
	 mm_logger -> Clear();
}

void TFormLogger::add_heading(String const & str) const
{
	 mm_logger -> Lines -> Add("******************************************");
	 mm_logger -> Lines -> Add(str);
	 mm_logger -> Lines -> Add("******************************************");
}
void TFormLogger::add_chapter(String const & str) const
{
	 mm_logger -> Lines -> Add("------------------------------------------");
	 mm_logger -> Lines -> Add(str);
     mm_logger -> Lines -> Add("------------------------------------------");
}
void TFormLogger::print_stud_coordinates(double id, double x, double x_l, double x_r)
{
	String str = L"id = " + FloatToStrF(id, ffFixed, 15, 0) + "; " +
				 L"x = " + FloatToStrF(x, ffFixed, 15, 0) + "; " +
				 L"x_l = " + FloatToStrF(x_l, ffFixed, 15, 0) + "; " +
				 L"x_r = " + FloatToStrF(x_r, ffFixed, 15, 0) + "; ";

	mm_logger -> Lines -> Add(str);

}
void TFormLogger::print_stud_shear_force(double id, double S)
{
	String str = L"id = " + FloatToStrF(id, ffFixed, 15, 0) + "; " +
				 L"S = " + FloatToStrF(S, ffFixed, 15, 0);

	mm_logger -> Lines -> Add(str);
}
void TFormLogger::print_ratio(double id, double ratio)
{
	String str = L"id = " + FloatToStrF(id, ffFixed, 15, 0) + "; " +
				 L"�� = " + FloatToStrF(ratio, ffFixed, 15, 2);

	mm_logger -> Lines -> Add(str);

}
void __fastcall TFormLogger::btn_cleanClick(TObject *Sender)
{
	mm_logger -> Clear();
}
void TFormLogger::print_string(String const & str)const
{
	mm_logger -> Lines -> Add(str);
}
void TFormLogger::print_double(String const & name, double val, String const & unit)const
{
	mm_logger -> Lines -> Add(name + val + unit);
}
void TFormLogger::print_2_doubles(String const & name1, double val1, String const & unit1,
						 String const & name2, double val2, String const & unit2)const
{
	mm_logger -> Lines -> Add(name1 + val1 + unit1 + L"    " +
		name2 + val2 + unit2);
}
//---------------------------------------------------------------------------

