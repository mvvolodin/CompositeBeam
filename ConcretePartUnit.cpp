
    //---------------------------------------------------------------------------

    #pragma hdrstop

	#include<cmath>
	#include "ConcretePartUnit.h"
	//---------------------------------------------------------------------------
	#pragma package(smart_init)

//---------------------------------------------------------------------------
TConcretePart::TConcretePart()
{
}
TConcretePart::TConcretePart(String slab_type, SlabType slab_type_enum, Concrete concrete, Rebar rebar, double h_f)
	:slab_type_(slab_type), slab_type_enum_(slab_type_enum),concrete_(concrete),rebar_(rebar), h_f_(h_f)
{}
//-----------------------------------------------------------------------------
//���������� ������ ������ �������� �� ���������
//-----------------------------------------------------------------------------
void TConcretePart::set_default_values()
{
	slab_type_ = L"������� �����";
	slab_type_enum_ = SlabType::FLAT;
    h_f_ = 200;
	concrete_.set_default_values();
	rebar_.set_default_values();
}
//---------------------------------------------------------------------------
//���������� ������� � �������� ����
//---------------------------------------------------------------------------
void TConcretePart::save(std::ostream& ostr) const
{
	concrete_.save(ostr);
	rebar_.save(ostr);

	wchar_t* buf = slab_type_.w_str();
	unsigned short l = slab_type_.Length()+1;
	ostr.write((char*)&l,sizeof(l));
	ostr.write((char*)buf,l*sizeof(wchar_t));
	free(buf);

	ostr.write((char*)&slab_type_enum_ ,sizeof(slab_type_enum_));
	ostr.write((char*)&h_f_,sizeof(h_f_));
}
//---------------------------------------------------------------------------
//�������� ������� �� ��������� �����
//---------------------------------------------------------------------------
void TConcretePart::load(std::istream& istr)
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
}
void TConcretePart::calculate()
{
	rebar_.calculate();
	concrete_.calculate();

	switch (slab_type_enum_)
	{

	case SlabType::FLAT:
		h_b_=h_f_;
		C_b_=h_b_/2.;
		A_b_=(b_l_+b_r_)*h_f_;
		I_b_=(b_l_+b_r_)*std::pow(h_f_,3)/12;
		break;
	case SlabType::CORRUGATED:
		CorrugatedSheet corrugated_sheet = corrugated_sheets_map[slab_type_];
		h_b_=corrugated_sheet.get_height()+h_f_;
		C_b_=corrugated_sheet.get_height()+h_f_/2.;
		A_b_=(b_l_+b_r_)*h_f_;
		I_b_=(b_l_+b_r_)*std::pow(h_f_,3)/12;
		break;
	}
}


