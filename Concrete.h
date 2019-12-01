//---------------------------------------------------------------------------

#ifndef ConcreteH
#define ConcreteH
#include <System.hpp>
#include <map>
#include <utility>
//---------------------------------------------------------------------------
class ConcreteBasic{
private:
	String grade_;
	double E_b_;
	double R_b_;
	double R_bt_;
public:
	ConcreteBasic();
	ConcreteBasic(String grade, double E_b, double R_b, double R_bt);
	inline double get_E_b()const {return E_b_;}
	inline double get_R_b()const {return R_b_;}
	inline double get_R_bt()const {return R_bt_;}
};
typedef std::map <String, ConcreteBasic> ConcreteBasicData;
typedef std::pair <String, ConcreteBasic> ConcreteBasicDataItem;

class Concrete:public ConcreteBasic{
private:
	double gamma_b_;
	double epsilon_b_lim_;
public:
	Concrete();
	Concrete(ConcreteBasic concrete_basic, double gamma_b, double epsilon_b_lim);
	inline double get_gamma_b()const {return gamma_b_;}
	inline double get_epsilon_b_lim()const {return epsilon_b_lim_;}


};

#endif
