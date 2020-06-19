
    //---------------------------------------------------------------------------

    #pragma hdrstop

	#include<cmath>
	#include "uConcretePart.h"
	//---------------------------------------------------------------------------
	#pragma package(smart_init)

//---------------------------------------------------------------------------
ConcretePart::ConcretePart()
{
}
ConcretePart::ConcretePart(String slab_type, SlabType slab_type_enum, Concrete concrete,
	Rebar rebar, double h_f, double h_n)
	:slab_type_(slab_type),
	 slab_type_enum_(slab_type_enum),
	 concrete_(concrete),
	 rebar_(rebar),
	 h_f_(h_f),
	 h_n_(h_n)
{}
ConcretePart::ConcretePart(String slab_type, SlabType slab_type_enum, Concrete concrete,
	Rebar rebar, double h_f, double h_n, bool wider_flange_up, bool sheet_orient_along)
	:slab_type_(slab_type),
	 slab_type_enum_(slab_type_enum),
	 concrete_(concrete),
	 rebar_(rebar),
	 h_f_(h_f),
	 h_n_(h_n),
	 wider_flange_up_(wider_flange_up),
	 sheet_orient_along_(sheet_orient_along)
{}
//-----------------------------------------------------------------------------
//���������� ������ ������ �������� �� ���������
//-----------------------------------------------------------------------------
void ConcretePart::set_default_values()
{
	slab_type_ = L"�75-750-0.8";
	slab_type_enum_ = SlabType::CORRUGATED;
	h_f_ = 200.;
	h_n_ = 0.;
    wider_flange_up_ = true;

	concrete_.set_default_values();
	rebar_.set_default_values();
}
//---------------------------------------------------------------------------
//���������� ������� � �������� ����
//---------------------------------------------------------------------------
void ConcretePart::save(std::ostream& ostr) const
{
	concrete_.save(ostr);
	rebar_.save(ostr);

	wchar_t* buf = slab_type_.w_str();
	unsigned short l = slab_type_.Length()+1;
	ostr.write((char*)&l,sizeof(l));
	ostr.write((char*)buf,l*sizeof(wchar_t));

	ostr.write((char*)&slab_type_enum_ ,sizeof(slab_type_enum_));
	ostr.write((char*)&h_f_,sizeof(h_f_));
	ostr.write((char*)&h_n_,sizeof(h_n_));
	ostr.write((char*)&wider_flange_up_,sizeof(wider_flange_up_));
	ostr.write((char*)&sheet_orient_along_,sizeof(sheet_orient_along_));
}
//---------------------------------------------------------------------------
//�������� ������� �� ��������� �����
//---------------------------------------------------------------------------
void ConcretePart::load(std::istream& istr)
{
	concrete_.load(istr);
	rebar_.load(istr);

	wchar_t* buf;
	unsigned short l;
	istr.read((char*)&l,sizeof(l));
	buf =(wchar_t*) malloc(l*sizeof(wchar_t));
	istr.read((char*)buf,l*sizeof(wchar_t));
	slab_type_ = String(buf);
	free(buf);

	istr.read((char*)&slab_type_enum_ ,sizeof(slab_type_enum_));
	istr.read((char*)&h_f_,sizeof(h_f_));
	istr.read((char*)&h_n_,sizeof(h_n_));
	istr.read((char*)&wider_flange_up_,sizeof(wider_flange_up_));
	istr.read((char*)&sheet_orient_along_,sizeof(sheet_orient_along_));
}
double ConcretePart::get_h(LengthUnit length_unit) const
{
	return (get_h_n() + h_f_) / static_cast<int>(length_unit);;
}
double ConcretePart::get_C_b(LengthUnit length_unit) const
{
	return (get_h_n() + h_f_ / 2.) / static_cast<int>(length_unit);
}

double ConcretePart::get_A_b(LengthUnit length_unit) const
{
	double A_b = 0.;

	A_b = b_sl_ * h_f_;

	return A_b/std::pow(static_cast<int>(length_unit),2);
}
double ConcretePart::get_I_b(LengthUnit length_unit) const
{
	double I_b = 0.;

	I_b = b_sl_ * std::pow(h_f_,3) / 12;

	return I_b/std::pow(static_cast<int>(length_unit),4);
}
double ConcretePart::get_SW_concrete(LoadUnit load_unit, LengthUnit length_unit) const
{
	double SW_concrete = 0.;
	double density = concrete_.get_density();

	if (slab_type_enum_ == SlabType::CORRUGATED)
		SW_concrete = density * GRAV_ACCELERAT * (h_f_ + CorrugatedSheetsData::get_corrugated_sheet(slab_type_).get_h_b(wider_flange_up_));
	else
		SW_concrete = density * GRAV_ACCELERAT * h_f_;

	return SW_concrete / static_cast<int>(load_unit)*std::pow(static_cast<int>(length_unit),2);
}
CorrugatedSheet ConcretePart::get_corrugated_sheet()const
{
	return CorrugatedSheetsData::get_corrugated_sheet(slab_type_);
}
double ConcretePart::get_h_n(LengthUnit length_unit) const
{
	if (slab_type_enum_ == SlabType::CORRUGATED)
		return CorrugatedSheetsData::get_corrugated_sheet(slab_type_).get_height() / static_cast<int>(length_unit);

	return h_n_ / static_cast<int>(length_unit);

}


