//---------------------------------------------------------------------------

#ifndef RebarH
#define RebarH
#include<System.hpp> //���������� ��� String
#include<map>

class RebarBasic{
public:
	RebarBasic();
	RebarBasic(String grade, double R_s_);
	inline String get_grade() const {return grade_;}
	inline double get_R_s() const {return R_s_;}
protected:
	String grade_ {""};
	double R_s_ {0.};
};

class Rebar:public RebarBasic{
public:
	Rebar();
	Rebar(String grade, double R_s, double diameter, double b, double a_u,
	double a_l, double safety_factor);
	inline double get_E_s() const {return E_s_;}
	inline double get_diameter() const {return diameter_;}
	inline double get_b() const {return b_;}
	inline double get_a_u() const {return a_u_;}
	inline double get_a_l() const {return a_l_;}
	inline double get_safety_factor() const {return safety_factor_;}
	inline double get_A_s(){return 3.14159*diameter_*diameter_/4;}
	inline double get_num_rows(){return num_rows_;}

private:
	double E_s_ {0.};
	double diameter_ {0.};
	double b_ {0.};//��� ��������
	double a_u_ {0.};// �������� ���� ������� ��������
	double a_l_ {0.}; //�������� ���� ������ ��������
	static const int num_rows_ {2};
	double safety_factor_ {0.};
};

using RebarPair=std::pair<String, RebarBasic>;
using RebarNamedList=std::map<String, RebarBasic> ;
using IteratorRebarNamedList=RebarNamedList::iterator ;

extern RebarNamedList rebar_named_list;
//---------------------------------------------------------------------------
#endif
