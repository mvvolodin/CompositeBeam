//---------------------------------------------------------------------------

#ifndef UnitStudH
#define UnitStudH

#include<System.hpp>
#include<map>
#include<utility>
#include<vector>
#include <ostream>
#include "Units.h"



class StudData{
	String name_ = L"SDx10x100";
	double d_an_ = 10.; //������� ������� ������� �����
	double l_= 100. ;   //����� �������� ������� ������� �����

	double P_rd_ = 0.;

	double resistance(double R_b, double R_y, double gamma_c);

	public:
		StudData(String name, double d_an, double l);
		void calculate(double R_b, double R_y, double gamma_c)
		{
			P_rd_ = resistance(R_b, R_y, gamma_c);
		}
};

class StudOnBeam{
	static StudData sd_;
	double x_;
	double x_l_;
	double x_r_;

	double id_;
	double sigma_b_l_;
    double sigma_b_r_;
	double S_;

public:
	StudOnBeam(double id, double S, double x):
		id_(id), S_(S), x_(x){}
	void set_S(double S){S_ = S;}
	double get_id()const{return id_;}
	double get_location()const{return x_;}
	double get_x_r()const{return x_r_;}
	double get_x_l()const{return x_l_;}
	double get_S()const{return S_;}
};



class TStudBasic{
public:
	TStudBasic();
	TStudBasic(String name, double d_an, double l);

	String get_name() const {return name_;};
	double get_l(LengthUnit length_unit=LengthUnit::mm) const
		{return l_/static_cast<int>(length_unit);}
	double get_d_an(LengthUnit length_unit=LengthUnit::mm) const
		{return d_an_/static_cast<int>(length_unit);}
protected:
	String name_ = L"SDx10x100";
	double d_an_ = 10.; //������� ������� ������� �����
	double l_= 100. ;   //����� �������� ������� ������� �����

	void save_stud_basic(std::ostream& ostr)const;
	void load_stud_basic(std::istream& istr);
};

class Studs:public TStudBasic{
public:
	Studs();
	Studs(String name,
		 double d_an, double l,
		 double z_e, double z_m,
		 double edge_rows_num, double middle_rows_num,
		 double R_y, double gamma_c);

	void set_default_values();
	void save(std::ostream& ostr)const;
	void load(std::istream& istr);
	void calculate(double L, double R_b, double R_y, double gamma_c);
	std::vector<double> calculate_coordinates(double L);//����������� ��������� ���������� ����-������
	//int calculate_studs_transverse_rows_number(double L);//����������� ���������� ���������� ����� ����-������
	double calculate_capacity(double R_b, double R_y, double gamma_c);
	std::vector<double> calc_ratios(std::vector<double> S);//������ � ������� ������������� ������� ��

	double get_edge_rows_dist(LengthUnit length_unit=LengthUnit::mm) const
		{return edge_rows_dist_/static_cast<int>(length_unit);}
	double get_middle_rows_dist(LengthUnit length_unit=LengthUnit::mm) const
		{return middle_rows_dist_/static_cast<int>(length_unit);}
	double get_edge_rows_num() const
		{return edge_rows_num_;}
	double get_middle_rows_num() const
		{return middle_rows_num_;}
	double get_gamma_c()const{return gamma_c_;}
	double get_R_y()const{return R_y_;}
	std::vector<double> get_coordinates_list()const{return coordinates_list_;}
	double get_P_rd(LoadUnit load_unit=LoadUnit::N)const{return P_rd_/static_cast<int>(load_unit);}

private:
	double edge_rows_dist_ = 0.;//��� ������ � ������� ������
	double middle_rows_dist_ = 0.; //��� ������ � ������� �����
	double edge_rows_num_= 0.; // ���������� ����� ������ � ������� ������
	double middle_rows_num_ = 0.; //���������� ����� ������ � ������� �����
	double R_y_= 0. ; //������ ���������
	double gamma_c_ = 0.; //����������� ������� ������

 //���� � ������������ ��������
	std::vector<double> coordinates_list_;//���������� ������
	double P_rd_ = 0.;

};

using TStudBasicPair = std::pair <String, TStudBasic>;
using TStudBasicNamedList = std::map <String, TStudBasic>;
using IteratorTStudBasicNamedList = TStudBasicNamedList::iterator;

extern TStudBasicNamedList stud_named_list;
//---------------------------------------------------------------------------
#endif

