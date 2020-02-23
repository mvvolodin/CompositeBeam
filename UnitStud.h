//---------------------------------------------------------------------------

#ifndef UnitStudH
#define UnitStudH

#include<System.hpp>
#include<map>
#include<utility>
#include<vector>
#include "Units.h"

class TStudBasic{
protected:
	String name_ = "";
	double d_an_ = 0.; //������� ������� ������� �����
	double l_= 0. ;   //����� �������� ������� ������� �����
public:
	TStudBasic();
	TStudBasic(String name, double d_an, double l);
	inline String get_name() const {return name_;};
	inline double get_l(LengthUnit length_unit=LengthUnit::mm) const
		{return l_/static_cast<int>(length_unit);}
	inline double get_d_an(LengthUnit length_unit=LengthUnit::mm) const
		{return d_an_/static_cast<int>(length_unit);}
};

class Studs:public TStudBasic{
public:
	Studs();
	Studs(String name,
		 double d_an, double l,
		 double z_e, double z_m,
		 double edge_rows_num, double middle_rows_num,
		 double R_y, double gamma_c);

	std::vector<double> calculate_coordinates(double L);//����������� ��������� ���������� ����-������
	int calculate_studs_transverse_rows_number(double L);//����������� ���������� ���������� ����� ����-������
	void calculate_capacity(double R_b, double R_y, double gamma_c);
	std::vector<double> calc_ratios(std::vector<double> S);//������ � ������� ������������� ������� ��

	inline double get_edge_rows_dist(LengthUnit length_unit=LengthUnit::mm) const
		{return edge_rows_dist_/static_cast<int>(length_unit);}
	inline double get_middle_rows_dist(LengthUnit length_unit=LengthUnit::mm) const
		{return middle_rows_dist_/static_cast<int>(length_unit);}
	inline double get_edge_rows_num() const
		{return edge_rows_num_;}
	inline double get_middle_rows_num() const
		{return middle_rows_num_;}
	inline double get_gamma_c()const{return gamma_c_;}
	inline double get_R_y()const{return R_y_;}

private:
	double edge_rows_dist_ = 0.;//��� ������ � ������� ������
	double middle_rows_dist_ = 0.; //��� ������ � ������� �����
	double edge_rows_num_= 0.; // ���������� ����� ������ � ������� ������
	double middle_rows_num_ = 0.; //���������� ����� ������ � ������� �����
	double R_y_= 0. ; //������ ���������
	double gamma_c_ = 0.; //����������� ������� ������

	double P_rd_ = 0.;
	double P_rd_addition_ = 0.;//����������� ������ ����� ������� (9.7)

};

typedef std::pair <String, TStudBasic> TStudBasicPair;
typedef std::map <String, TStudBasic> TStudBasicNamedList;
typedef TStudBasicNamedList::iterator IteratorTStudBasicNamedList;

extern TStudBasicNamedList stud_named_list;
//---------------------------------------------------------------------------
#endif

