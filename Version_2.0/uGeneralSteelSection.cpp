﻿//---------------------------------------------------------------------------

#pragma hdrstop

#include "uGeneralSteelSection.h"
#include <iostream>
#include <algorithm>
#include "uUnits.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

GeneralSteelSection::GeneralSteelSection()
{
}
GeneralSteelSection:: ~ GeneralSteelSection()
{
}

double GeneralSteelSection::t_max()const
{
	return std::max({upper_fl_thick(),
					 lower_fl_thick(),
					 web_thick()});
}

void GeneralSteelSection::fill_grid(TStringGrid* str_grid)const
{
	str_grid -> Cells [1][1] = FloatToStrF(area(), ffFixed, 15, 0);
	str_grid -> Cells [1][2] = FloatToStrF(inertia(), ffFixed, 15, 0);
	str_grid -> Cells [1][3] = FloatToStrF(modulus_upper_fl(), ffFixed, 15, 0);
	str_grid -> Cells [1][4] = FloatToStrF(modulus_lower_fl(), ffFixed, 15, 0);
	str_grid -> Cells [1][5] = FloatToStrF(grav_cent_upper_fl_dist(), ffFixed, 15, 0);
	str_grid -> Cells [1][6] = FloatToStrF(grav_cent_lower_fl_dist(), ffFixed, 15, 0);
}
void GeneralSteelSection::print(TWord_Automation & report)const
{
	using namespace units;

	report.PasteTextPattern(name().c_str() ,"%name%");
	report.PasteTextPattern(double_to_str(sect_height(), 0), "%sect_height%");
	report.PasteTextPattern(double_to_str(upper_fl_width(), 0),"%upper_fl_width%");
	report.PasteTextPattern(double_to_str(upper_fl_thick(), 0),"%upper_fl_thick%");
	report.PasteTextPattern(double_to_str(lower_fl_width(), 0),"%lower_fl_width%");
	report.PasteTextPattern(double_to_str(lower_fl_thick(), 0),"%lower_fl_thick%");
	report.PasteTextPattern(double_to_str(web_height(), 0),"%web_height%");
	report.PasteTextPattern(double_to_str(web_thick(), 0),"%web_thick%");
	report.PasteTextPattern(double_to_str(0, 0),"%radius%");

	report.PasteTextPattern(double_to_str(area() * cm2),"%area%");
	report.PasteTextPattern(double_to_str(inertia() * cm4),"%inertia%");
	report.PasteTextPattern(double_to_str(modulus_upper_fl() * cm3),"%modulus_upper_fl%");
	report.PasteTextPattern(double_to_str(modulus_lower_fl() * cm3),"%modulus_lower_fl%");
	report.PasteTextPattern(double_to_str(grav_cent_upper_fl_dist()),"%GC_upper_fl_dist%");
	report.PasteTextPattern(double_to_str(grav_cent_lower_fl_dist()),"%GC_lower_fl_dist%");
}
#ifdef DEBUG_STEEL_SECT
void GeneralSteelSection::log()const
{
	FormLogger -> print(
		{
		 "*****************************",
		 "Геометрические характеристики",
		 "*****************************",
		 "h_sect = " + FloatToStr(sect_height()) + " мм; ",
		 "buf = " + FloatToStr(upper_fl_width()) + " мм; " +
			"tuf = " + FloatToStr(upper_fl_thick()) + " мм",
		 "blf = " + FloatToStr(lower_fl_width()) + " мм; " +
			"tlf = " + FloatToStr(lower_fl_thick()) + " мм",
		 "hw = " + FloatToStr(web_height()) + " мм; " +
			"tw = " + FloatToStr(web_thick()) + " мм",
		 "C = " + FloatToStr(grav_cent()) + " мм",
		 "A = " + FloatToStr(area()) + " мм2",
		 "Wuf = " + FloatToStr(modulus_upper_fl()) + " мм3",
		 "Wlf = " + FloatToStr(modulus_lower_fl()) + " мм3",
		 "I = " + FloatToStr(inertia()) + " мм2"});
}
#endif






