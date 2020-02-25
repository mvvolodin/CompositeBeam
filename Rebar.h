//---------------------------------------------------------------------------

#ifndef RebarH
#define RebarH
#include<System.hpp> //���������� ��� String
#include<map>

class RebarBasic{
public:
	RebarBasic();
	RebarBasic(String grade, double R_sn);
	String get_grade() const {return grade_;}
	double get_R_sn() const {return R_sn_;}
protected:
	String grade_ = "";
	double R_sn_ = 0.;
};

class Rebar:public RebarBasic{
public:
	Rebar();
	Rebar(String grade, double R_sn, double d_s, double b, double a_u,
	double a_l, double gamma_s_);
	double get_E_s() const {return E_s_;}
	double get_R_s() const {return R_sn_ / gamma_s_ ;}
	double get_diameter() const {return d_s_;}
	double get_b() const {return b_;}
	double get_a_u() const {return a_u_;}
	double get_a_l() const {return a_l_;}
	double get_gamma_s_() const {return gamma_s_;}
	double get_A_s() const {return A_s_;}
	double get_num_rows(){return num_rows_;}

private:
	double E_s_ = 200000.;
	double d_s_ = 0.;
	double b_  = 0.;//��� ��������
	double a_u_ = 0.;// �������� ���� ������� ��������
	double a_l_ = 0.; //�������� ���� ������ ��������
	static const int num_rows_ = 2;
	double gamma_s_ = 0.;
	double A_s_ = 0.;// ������� �������� �� ������� �����

	void calc_A_s();
};

using RebarPair=std::pair<String, RebarBasic>;
using RebarNamedList=std::map<String, RebarBasic> ;
using IteratorRebarNamedList=RebarNamedList::iterator ;

extern RebarNamedList rebar_named_list;
//---------------------------------------------------------------------------
#endif
