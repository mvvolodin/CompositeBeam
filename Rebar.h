//---------------------------------------------------------------------------

#ifndef RebarH
#define RebarH
#include<System.hpp> //���������� ��� String
#include<map>

class RebarBasic{
public:
	RebarBasic();
	RebarBasic(String grade, double R_s);
	inline String get_grade() const {return grade;}
	inline double get_R_s() const {return R_s;}
protected:
	String grade;
	double R_s;
};

class Rebar:public RebarBasic{
public:
	Rebar();
	Rebar(String grade, double R_s, double diameter, double safety_factor);
	inline double get_E_s() const {return E_s;}
	inline double get_diameter() const {return diameter;}
	inline double get_safety_factor() const {return safety_factor;}
	inline double get_A_s(){return 3.14159*diameter*diameter/4;}

private:
	double E_s;
	double diameter;
	double safety_factor;
};

typedef std::pair<String, RebarBasic> RebarPair;
typedef std::map<String, RebarBasic> RebarNamedList;
typedef RebarNamedList::iterator IteratorRebarNamedList;

extern RebarNamedList rebar_named_list;
//---------------------------------------------------------------------------
#endif
