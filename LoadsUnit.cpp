//---------------------------------------------------------------------------

#pragma hdrstop
#include "LoadsUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TLoads::TLoads(){}

TLoads::TLoads(double SW_steel_beam, double SW_corrugated_sheets, double DL_I, double DL_II, double LL,
	double gamma_f_st_SW, double gamma_f_DL_I, double gamma_f_DL_II, double gamma_f_LL)
	:SW_steel_beam_(SW_steel_beam/static_cast<int>(LengthUnit::m)),
	SW_corrugated_sheets_(SW_corrugated_sheets/std::pow(static_cast<int>(LengthUnit::m),2)),
	DL_I_(DL_I*static_cast<int>(LoadUnit::kN)/std::pow(static_cast<int>(LengthUnit::m),2)),  //������� ������� �������!
	DL_II_(DL_II*static_cast<int>(LoadUnit::kN)/std::pow(static_cast<int>(LengthUnit::m),2)),
	LL_(LL*static_cast<int>(LoadUnit::kN)/std::pow(static_cast<int>(LengthUnit::m),2)),
	gamma_f_st_SW_(gamma_f_st_SW),
	gamma_f_DL_I_(gamma_f_DL_I),
	gamma_f_DL_II_(gamma_f_DL_II),
	gamma_f_LL_(gamma_f_LL){}
/* TODO 1 -oMV :
�������� � ������� ���������� � ���� � � ������� �������� �� �����
"gamma_f_concrete_SW_", "b_" � "SW_concrete_"
 */
void TLoads::save(std::ostream& ostr) const
{
	ostr.write((char*)&SW_steel_beam_, sizeof(SW_steel_beam_));
	ostr.write((char*)&SW_corrugated_sheets_, sizeof(SW_corrugated_sheets_));
	ostr.write((char*)&DL_I_, sizeof(DL_I_));
	ostr.write((char*)&DL_II_, sizeof(DL_II_));
	ostr.write((char*)&LL_, sizeof(LL_));
	ostr.write((char*)&gamma_f_st_SW_, sizeof(gamma_f_st_SW_));
	ostr.write((char*)&gamma_f_DL_I_, sizeof(gamma_f_DL_I_));
	ostr.write((char*)&gamma_f_DL_II_, sizeof(gamma_f_DL_II_));
	ostr.write((char*)&gamma_f_LL_, sizeof(gamma_f_LL_));

}
void TLoads::load(std::istream& istr)
{
	istr.read((char*)&SW_steel_beam_, sizeof(SW_steel_beam_ ));
	istr.read((char*)&SW_corrugated_sheets_, sizeof(SW_corrugated_sheets_));
	istr.read((char*)&DL_I_, sizeof(DL_I_));
	istr.read((char*)&DL_II_, sizeof(DL_II_));
	istr.read((char*)&LL_, sizeof(LL_));
	istr.read((char*)&gamma_f_st_SW_, sizeof(gamma_f_st_SW_));
	istr.read((char*)&gamma_f_DL_I_, sizeof(gamma_f_DL_I_));
	istr.read((char*)&gamma_f_DL_II_, sizeof(gamma_f_DL_II_));
	istr.read((char*)&gamma_f_LL_, sizeof(gamma_f_LL_));

}
//-----------------------------------------------------------------------------
//������ �������� �������� �� ���������� ���������� Ia
//-----------------------------------------------------------------------------
double TLoads::calculate_Ia_design_LCC()
{
	assert(fully_initialized_);
	return gamma_f_st_SW_ * SW_steel_beam_ + gamma_f_st_SW_ * SW_corrugated_sheets_ * b_ +
		gamma_f_concrete_SW_ * SW_concrete_ * b_+ gamma_f_DL_I_ * DL_I_ * b_;
}
//-----------------------------------------------------------------------------
//������ �������� �������� �� ���������� ���������� Ib
//-----------------------------------------------------------------------------
double TLoads::calculate_Ib_design_LCC()
{
    assert(fully_initialized_);
	return gamma_f_st_SW_ * SW_steel_beam_ + gamma_f_st_SW_ * SW_corrugated_sheets_ * b_ +
		gamma_f_concrete_SW_ * SW_concrete_ * b_;
}
//-----------------------------------------------------------------------------
//������ �������� �������� �� ���������� ���������� II
//-----------------------------------------------------------------------------
double TLoads::calculate_II_design_LCC()
{
	assert(fully_initialized_);
	 return gamma_f_DL_II_ * DL_II_ * b_ + gamma_f_LL_ * LL_ * b_;
}
//-----------------------------------------------------------------------------
//������ �������� �������� �� ������ ���������� ����������
//-----------------------------------------------------------------------------
double TLoads::calculate_total_design_LCC()
{
	assert(fully_initialized_);
	 return gamma_f_st_SW_ * SW_steel_beam_ + gamma_f_st_SW_ * SW_corrugated_sheets_ * b_ +
		gamma_f_concrete_SW_ * SW_concrete_ * b_ + gamma_f_DL_II_ * DL_II_ * b_ + gamma_f_LL_ * LL_ * b_;
}
//-----------------------------------------------------------------------------
//���������� ������ ������ �������� �� ���������
//-----------------------------------------------------------------------------
void TLoads::set_default_values()
{
	SW_steel_beam_ = 0.;
	SW_corrugated_sheets_ = 0.;
	SW_concrete_ = 0.;
	DL_I_ = 0.0012;
	DL_II_ = 0.0015;
	LL_ =  0.002;
	gamma_f_st_SW_ = 1.05;
	gamma_f_concrete_SW_ = 1.2;
	gamma_f_DL_I_ = 1.35;
	gamma_f_DL_II_ = 1.35;
	gamma_f_LL_ = 1.35;
    b_ = 6000;
}

void TLoads::set_data(double SW_steel_beam, double SW_corrugated_sheets, double SW_concrete, double b)
{
	SW_steel_beam_ = SW_steel_beam;
	SW_corrugated_sheets_ = SW_corrugated_sheets;
	SW_concrete_ = SW_concrete;
	b_ = b;

	fully_initialized_ = true;
}


