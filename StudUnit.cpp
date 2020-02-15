//---------------------------------------------------------------------------
#include<algorithm>
#include<cmath>

#pragma hdrstop

#include "StudUnit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//double* out_my;
//
//get_array(.............,out_my)
//
//int get_array(double* in, int size_in, double* out)
//{
//  return size_out;
//}
//
//out_my[10]

TStudBasicNamedList stud_named_list  //������������� ������ �������� � ��� ������ ����� ��� ��� �����
										//����������� ������ �� ������ �� �����, ���� �� ����� ��� �� ���������� ����� �� �������� ������
{
	{{"SDx10x50"},		{"SDx10x50",10,50}},  //�������� �� BasicStud ������������� ��������� ���
	{{"SDx10x75"},		{"SDx10x75",10,75}},
	{{"SDx10x100"},		{"SDx10x100",10,100}},
	{{"SDx10x125"},		{"SDx10x125",10,125}},
	{{"SDx10x150"},		{"SDx10x150",10,150}},
	{{"SDx10x175"},		{"SDx10x175",10,175}},
	{{"SDx10x200"},		{"SDx10x200",10,200}},
	{{"SDx10x225"},		{"SDx10x225",10,225}},
	{{"SDx10x250"},		{"SDx10x250",10,250}},
	{{"SDx10x275"},		{"SDx10x275",10,275}},
	{{"SDx10x300"},		{"SDx10x300",10,300}},
	{{"SDx10x325"},		{"SDx10x325",10,325}},
	{{"SDx10x350"},		{"SDx10x350",10,350}},

	{{"SDx13x50"},		{"SDx10x50",13,50}},
	{{"SDx13x75"},		{"SDx10x75",13,75}},
	{{"SDx13x100"},		{"SDx10x100",13,100}},
	{{"SDx13x125"},		{"SDx10x125",13,125}},
	{{"SDx13x150"},		{"SDx10x150",13,150}},
	{{"SDx13x175"},		{"SDx10x175",13,175}},
	{{"SDx13x200"},		{"SDx10x200",13,200}},
	{{"SDx13x225"},		{"SDx10x225",13,225}},
	{{"SDx13x250"},		{"SDx10x250",13,250}},
	{{"SDx13x275"},		{"SDx10x275",13,275}},
	{{"SDx13x300"},		{"SDx10x300",13,300}},
	{{"SDx13x325"},		{"SDx10x325",13,325}},
	{{"SDx13x350"},		{"SDx10x350",13,350}},

	{{"SDx16x50"},		{"SDx10x50",16,50}},
	{{"SDx16x75"},		{"SDx10x75",16,75}},
	{{"SDx16x100"},		{"SDx10x100",16,100}},
	{{"SDx16x125"},		{"SDx10x125",16,125}},
	{{"SDx16x150"},		{"SDx10x150",16,150}},
	{{"SDx16x175"},		{"SDx10x175",16,175}},
	{{"SDx16x200"},		{"SDx10x200",16,200}},
	{{"SDx16x225"},		{"SDx10x225",16,225}},
	{{"SDx16x250"},		{"SDx10x250",16,250}},
	{{"SDx16x275"},		{"SDx10x275",16,275}},
	{{"SDx16x300"},		{"SDx10x300",16,300}},
	{{"SDx16x325"},		{"SDx10x325",16,325}},
	{{"SDx16x350"},		{"SDx10x350",16,350}},

	{{"SDx19x50"},		{"SDx10x50",19,50}},
	{{"SDx19x75"},		{"SDx10x75",19,75}},
	{{"SDx19x100"},		{"SDx10x100",19,100}},
	{{"SDx19x125"},		{"SDx10x125",19,125}},
	{{"SDx19x150"},		{"SDx10x150",19,150}},
	{{"SDx19x175"},		{"SDx10x175",19,175}},
	{{"SDx19x200"},		{"SDx10x200",19,200}},
	{{"SDx19x225"},		{"SDx10x225",19,225}},
	{{"SDx19x250"},		{"SDx10x250",19,250}},
	{{"SDx19x275"},		{"SDx10x275",19,275}},
	{{"SDx19x300"},		{"SDx10x300",19,300}},
	{{"SDx19x325"},		{"SDx10x325",19,325}},
	{{"SDx19x350"},		{"SDx10x350",19,350}},

	{{"SDx22x50"},		{"SDx10x50",22,50}},
	{{"SDx22x75"},		{"SDx10x75",22,75}},
	{{"SDx22x100"},		{"SDx10x100",22,100}},
	{{"SDx22x125"},		{"SDx10x125",22,125}},
	{{"SDx22x150"},		{"SDx10x150",22,150}},
	{{"SDx22x175"},		{"SDx10x175",22,175}},
	{{"SDx22x200"},		{"SDx10x200",22,200}},
	{{"SDx22x225"},		{"SDx10x225",22,225}},
	{{"SDx22x250"},		{"SDx10x250",22,250}},
	{{"SDx22x275"},		{"SDx10x275",22,275}},
	{{"SDx22x300"},		{"SDx10x300",22,300}},
	{{"SDx22x325"},		{"SDx10x325",22,325}},
	{{"SDx22x350"},		{"SDx10x350",22,350}},

	{{"SDx25x50"},		{"SDx10x50",25,50}},
	{{"SDx25x75"},		{"SDx10x75",25,75}},
	{{"SDx25x100"},		{"SDx10x100",25,100}},
	{{"SDx25x125"},		{"SDx10x125",25,125}},
	{{"SDx25x150"},		{"SDx10x150",25,150}},
	{{"SDx25x175"},		{"SDx10x175",25,175}},
	{{"SDx25x200"},		{"SDx10x200",25,200}},
	{{"SDx25x225"},		{"SDx10x225",25,225}},
	{{"SDx25x250"},		{"SDx10x250",25,250}},
	{{"SDx25x275"},		{"SDx10x275",25,275}},
	{{"SDx25x300"},		{"SDx10x300",25,300}},
	{{"SDx25x325"},		{"SDx10x325",25,325}},
	{{"SDx25x350"},		{"SDx10x350",25,350}},

};

//���� ����������� ��������� ��� ���������� ������ ����� pair � map.
//������, ��� �� ������ ������ ������. ���� ��������� [] ���, ��� �������� �������� value initialization
//������ ��������������� default ctr
TStudBasic::TStudBasic(){}
TStudBasic::TStudBasic(String name, double d_an, double l):
						name_(name),
						d_an_(d_an),
						l_(l){}

TStud::TStud(){}
TStud::TStud(String name,
			 double d_an, double l,
			 double edge_rows_dist, double middle_rows_dist,
			 double edge_rows_num, double middle_rows_num,
			 double R_y, double gamma_c):
	 TStudBasic(name, d_an, l),
	 edge_rows_dist_(edge_rows_dist),
	 middle_rows_dist_(middle_rows_dist),
	 edge_rows_num_(edge_rows_num),
	 middle_rows_num_(middle_rows_num),
	 R_y_(R_y),
	 gamma_c_(gamma_c)
{
	calculate_coordinates(18000);
}
//-----------------------------------------------------------------------------
//����������� ��������� ���������� ����-������
//in:l-����� �����
//-----------------------------------------------------------------------------
void TStud::calculate_coordinates(double span)
{
	const double L = span;
	const double L3 = L/3.0; //49

	double a = edge_rows_dist_; // 10

	int na = L3 / a + 1;
	a = L3/na;

	for (int i = 0; i < na; i++)
		stud_coordinates_.emplace_back(0 + a*i);

	double b = middle_rows_dist_; // 10
	int nb = L3 / b + 1;
	b = L3/nb;

	for (int i = 0; i < nb; i++)
		stud_coordinates_.emplace_back(L3 + b*i);

	for (int i = 0; i < na; i++)
		stud_coordinates_.emplace_back(2*L3 + a*i);

//	double sum = 0;
//	for (int i = 0; i < 10; i++) {
//		sum += 0.1; // 0.10000000000000000056
//	}
//	if (sum == 1) {
//		cout << "Yes";
//	}
//	else
//		cout << "No";

//	double span_third=span/3;
//
//	double temp_coord=0.; //������� ����������, ���������� ����� ��� ���� ������
//
//	while(temp_coord<span_third){
//
//		stud_coordinates_.push_back(temp_coord+=edge_rows_dist_);
//	}
//
//	while(temp_coord<2*span_third){
//
//		stud_coordinates_.push_back(temp_coord+=middle_rows_dist_);
//	}
//
//	while(temp_coord<span){
//
//		stud_coordinates_.push_back(temp_coord+=edge_rows_dist_);
//	}
//
//
	std::transform(stud_coordinates_.begin(),stud_coordinates_.end(),stud_coordinates_.begin(),[](double coord){return std::round(coord);});
}

