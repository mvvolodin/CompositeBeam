//---------------------------------------------------------------------------

#pragma hdrstop

#include "uRebar.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

RebarStrengtNamedList const rebar_strength_named_list{
	{L"A240", 240},
	{L"A400", 400},
	{L"A500", 500},
	{L"A600", 600},
	{L"A800", 800},
	{L"A1000", 1000},
	};

RebarNamedList rebar_named_list
{
{{L"A240"},		{L"A240",240}},
{{L"A400"},		{L"A400",400}},
{{L"A500"},		{L"A500",500}},
{{L"A600"},		{L"A600",600}},
{{L"A800"},		{L"A800",800}},
{{L"A1000"},	{L"A1000",1000}},
};

RebarBasic::RebarBasic()
{
}

RebarBasic::RebarBasic(String grade, double R_sn):
	grade_(grade),
	R_sn_(R_sn){}

Rebar::Rebar():
	RebarBasic()
{

}

Rebar::Rebar(String grade, double R_sn, double E_s, double d_s, double b, double a_u,
	double a_l, double gamma_s):
	RebarBasic{grade, R_sn},
	E_s_(E_s),
	d_s_(d_s),
	b_(b),
	a_u_(a_u),
	a_l_(a_l),
	gamma_s_(gamma_s)
{
}
void Rebar::save(ostream& ostr) const
{
	RebarBasic::save(ostr);
	ostr.write((char*)&E_s_ ,sizeof(E_s_));
	ostr.write((char*)&d_s_ ,sizeof(d_s_));
	ostr.write((char*)&b_  ,sizeof(b_ ));
	ostr.write((char*)&a_u_ ,sizeof(a_u_));
	ostr.write((char*)&a_l_ ,sizeof(a_l_));
	ostr.write((char*)&gamma_s_ ,sizeof(gamma_s_));


}
void RebarBasic::save(ostream& ostr) const
{
	wchar_t* buf = grade_.w_str();
	unsigned short l = grade_.Length()+1;
	ostr.write((char*)&l,sizeof(l));
	ostr.write((char*)buf,l*sizeof(wchar_t));

	ostr.write((char*)&R_sn_ ,sizeof(R_sn_));
}
void Rebar::load(istream& istr)
{
	RebarBasic::load(istr);
	istr.read((char*)&E_s_ ,sizeof(E_s_));
	istr.read((char*)&d_s_ ,sizeof(d_s_));
	istr.read((char*)&b_  ,sizeof(b_ ));
	istr.read((char*)&a_u_ ,sizeof(a_u_));
	istr.read((char*)&a_l_ ,sizeof(a_l_));
	istr.read((char*)&gamma_s_ ,sizeof(gamma_s_));
}
void RebarBasic::load(istream& istr)
{
	wchar_t* buf;
	unsigned short l;
	istr.read((char*)&l,sizeof(l));
	buf =(wchar_t*) malloc(l*sizeof(wchar_t));
	istr.read((char*)buf,l*sizeof(wchar_t));
	grade_ = String(buf);
	free(buf);

	istr.read((char*)&R_sn_ ,sizeof(R_sn_));
}
void Rebar::set_default_values()
{
	grade_ = L"A500";
	R_sn_ = 500.;
	E_s_ = 200000.;
	d_s_ = 0.;
	b_  = 200.;
	a_u_ = 50.;
	a_l_ = 50.;
	gamma_s_ = 1.15;
}
Rebar2::Rebar2()
{

}
Rebar2::Rebar2(String const grade, double R_sn, double const E_s, double const d_s, double gamma_s):
	grade_{grade},
	E_s_{E_s},
	R_sn_{R_sn},
	d_s_{d_s},
	gamma_s_{gamma_s}
{
}
String Rebar2::grade() const
{
	return grade_;
}
double Rebar2::E_s() const
{
	return E_s_;
}
double Rebar2::R_sn() const
{
	return R_sn_;
}
double Rebar2::d_s() const
{
	return d_s_;
}
Rebars::Rebars()
{

}
Rebars::Rebars(Rebar2 const & reb,
			   double const a_u, double const a_l,
			   double const b_u, double const b_l):
					reb_{reb},
					a_u_{a_u}, a_l_{a_l},
					b_u_{b_u}, b_l_{b_l}
{

}
Rebar2 Rebars::rebar()
{
	return reb_;
}
double Rebars::A_u_s_per_unit()
{
	return 3.14159265 * reb_.d_s() * reb_.d_s() / (4 * b_u_);
}
double Rebars::A_l_s_per_unit()
{
	return 3.14159265 * reb_.d_s() * reb_.d_s() / (4 * b_l_);
}




