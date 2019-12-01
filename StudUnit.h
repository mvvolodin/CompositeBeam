//---------------------------------------------------------------------------

#ifndef StudUnitH
#define StudUnitH

#include<System.hpp>
#include<map>
#include<utility>

class TStud{
	String name_;
	double l_;   //����� �������� ������� ������� �����
	double d_an_; //������� ������� ������� �����
	double gamma_c_; //����������� ������� ������
	double R_y_; //������ ���������
public:
	TStud();
	TStud(String, double, double, double, double);
	inline String get_name() const {return name_;};
	inline double get_l()const{return l_;}
	inline double get_d_an()const{return d_an_;}
	inline double get_gamma_c()const{return gamma_c_;}
	inline double get_R_y()const{return R_y_;}

};

typedef std::map <String, TStud> TStudData;
typedef std::pair <String, TStud> TStudDataItem;
typedef std::map <String, TStud>::iterator stud_data_iterator;
//---------------------------------------------------------------------------
#endif
