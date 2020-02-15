//---------------------------------------------------------------------------

#ifndef UnitStudH
#define UnitStudH

#include<System.hpp>
#include<map>
#include<utility>
#include<vector>
#include "Units.h"

class TStudBasic{
private:
	String name_="";
	double d_an_=0.; //������� ������� ������� �����
	double l_=0.;   //����� �������� ������� ������� �����
public:
	TStudBasic();
	TStudBasic(String name, double d_an, double l);
	inline String get_name() const {return name_;};
	inline double get_l(LengthUnit length_unit=LengthUnit::mm) const
		{return l_/static_cast<int>(length_unit);}
	inline double get_d_an(LengthUnit length_unit=LengthUnit::mm) const
		{return d_an_/static_cast<int>(length_unit);}
};

class TStud:public TStudBasic{
public:
	TStud();
	TStud(String name,
		 double d_an, double l,
		 double z_e, double z_m,
		 double edge_rows_num, double middle_rows_num,
		 double R_y, double gamma_c);
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
	inline std::vector<double> get_stud_coordinates()const {return stud_coordinates_;}

private:
	double edge_rows_dist_=0.;//��� ������ � ������� ������
	double middle_rows_dist_=0.; //��� ������ � ������� �����
	double edge_rows_num_=0.; // ���������� ����� ������ � ������� ������
	double middle_rows_num_=0.; //���������� ����� ������ � ������� �����
	double R_y_=0.; //������ ���������
	double gamma_c_=0.; //����������� ������� ������
	std::vector<double> stud_coordinates_;
	std::vector<double> cs_shear_forces_coordinates_;

	void calculate_coordinates(double span);//����������� ��������� ���������� ����-������
    void calc_ratios();
	void calc_shear_forces(double A_b, double A_s, std::vector<double> sigma_b,
	std::vector<double> sigma_s, int num_coord_shear_forces);
};

typedef std::pair <String, TStudBasic> TStudBasicPair;
typedef std::map <String, TStudBasic> TStudBasicNamedList;
typedef TStudBasicNamedList::iterator IteratorTStudBasicNamedList;

extern TStudBasicNamedList stud_named_list;
//---------------------------------------------------------------------------
#endif

