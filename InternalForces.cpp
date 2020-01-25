//---------------------------------------------------------------------------

#pragma hdrstop

#include "InternalForces.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
InternalForces::InternalForces()
{
}
InternalForces::InternalForces(double p, CS_Coordinates cs_coordinates, int temporary_supports_number)
{
	M_.clear(); //���������� �������
	Q_.clear();//���������� ����
	Q_jump_.clear();//������� ������� ����
	R_.clear();//������� ���������� ����


	switch (temporary_supports_number)
	{
		case static_cast<int>(SupportsNumber::Zero):
			R_calc_simple_beam(p, cs_coordinates.back());
			M_calc_simple_beam(p, cs_coordinates, temporary_supports_number);
			Q_calc_simple_beam(p, cs_coordinates, temporary_supports_number);
			break;
		case static_cast<int>(SupportsNumber::One):
			R_calc_two_span_beam(p, cs_coordinates.back());
			M_calc_two_span_beam(p, cs_coordinates, temporary_supports_number);
			Q_calc_two_span_beam(p, cs_coordinates, temporary_supports_number);
			break;
		case static_cast<int>(SupportsNumber::Two):
			R_calc_three_span_beam(p, cs_coordinates.back());
			M_calc_three_span_beam(p, cs_coordinates, temporary_supports_number);
			Q_calc_three_span_beam(p, cs_coordinates, temporary_supports_number);
			break;
		case static_cast<int>(SupportsNumber::Three):
			R_calc_four_span_beam(p, cs_coordinates.back());
			M_calc_four_span_beam(p, cs_coordinates, temporary_supports_number);
			Q_calc_four_span_beam(p, cs_coordinates, temporary_supports_number);
		break;

	}
}
void InternalForces::R_calc_simple_beam(double p, double l)
{
	R_.push_back(p*(l/2));
	R_.push_back(p*(l/2));
}
void InternalForces::R_calc_two_span_beam(double p, double l)
{
	R_.push_back(0.375*p*(l/2));
	R_.push_back(1.25*p*(l/2));
	R_.push_back(0.375*p*(l/2));
}
void InternalForces::R_calc_three_span_beam(double p, double l)
{


}
void InternalForces::R_calc_four_span_beam(double p, double l)
{


}
void InternalForces::M_calc_simple_beam(double p, CS_Coordinates cs_coordinates, int temporary_supports_number)
{
	double temp_M=0.0;
	double l=cs_coordinates.back();

	double A=0.5*p*l;

	for (int n = 0; n < cs_coordinates.size(); ++n)
	{
		double temp_l=cs_coordinates[n];
		temp_M=A*cs_coordinates[n]-p*temp_l*temp_l/2;
		M_.push_back(temp_M);
	}
}
void InternalForces::M_calc_two_span_beam(double p, CS_Coordinates cs_coordinates, int temporary_supports_number)//����� �������������
{
	// ������ ������� �������� I ��� ����� 8.1.7
	double temp_M=0.0;
	double l=cs_coordinates.back();

	double A=0.375*p*l/2.0;
	double B=1.25*p*l/2.0;

	for (int n = 0; n< cs_coordinates.size(); ++n)
	{
		double temp_l=cs_coordinates[n];

		if (cs_coordinates[n]<=l/2)
		{ //����� ������ ��� ���������� �������. ���������� ��������� enum

			temp_M=A*temp_l-p*temp_l*temp_l/2;
		}
		else
		{
			temp_M=A*temp_l-p*temp_l*temp_l/2+B*(temp_l-l/2);
		}
		M_.push_back(temp_M);
	}
}
//-----------------------------------------------------------
// ������ ����� ��������
void InternalForces::M_calc_three_span_beam(double p, CS_Coordinates cs_coordinates, int temporary_supports_number)
{
	double M_temp = 0.0;
	double length = cs_coordinates.back();
	// �������� �������������� ���� �� ������
	double Q[6] = {0.4, -0.6, 0.5, 0.6, -0.5, -0.4};

	int n_point = cs_coordinates.size();
	int i, j;
	double del_x = length/n_point;  // ���������� ����� �������
	double del_Q = del_x*p;     // �������� �������������� ����
	double Q_temp;

	M_.push_back(M_temp);

	for (i = 0; i < 3; i++) {
	  Q_temp = Q[i*2]*length*p/3;
	  for (int j = 0; j < n_point/3 ; j++) {
		M_.push_back(M_temp);
		M_temp += (Q_temp - del_Q/2)*del_x;
		Q_temp -= del_Q;
	  }
	}
   //�������� � WatchList ���������	*(M_._Mypair._Myval2._Myfirst+i). � ���� Repeat Count ������ ���������� ������������ ���������


}
void InternalForces::M_calc_four_span_beam(double p, CS_Coordinates cs_coordinates, int temporary_supports_number)
{
	double M_temp;
	double length = cs_coordinates.back();
	// �������� �������������� ���� �� ������
	double Q[8] = {0.392, -0.608, 0.536, -0.464, 0.464, -0.536, 0.608, -0.392};

	int n_point = cs_coordinates.size();
	int i, j;
	double del_x = length/n_point;  // ���������� ����� �������
	double del_Q = del_x*p;     // �������� �������������� ����
	double Q_temp;
	for (i = 0; i < 4; i++) {
	  Q_temp = Q[i*2]*length*p/4;
	  for (int j = 0; j < n_point/4 ; j++) {
		M_.push_back(M_temp);
		M_temp += (Q_temp - del_Q/2)*del_x;
		Q_temp -= del_Q;
	  }
	}
	  //
}
void InternalForces::Q_calc_simple_beam(double p, CS_Coordinates cs_coordinates, int temporary_supports_number)
{
	double temp_Q=0.0;
	double l=cs_coordinates.back();

	double A=0.5*p*l;

	for (int n = 0; n < cs_coordinates.size(); ++n)
	{
		double temp_l=cs_coordinates[n];
		temp_Q=A-p*temp_l;
		Q_.push_back(temp_Q);
	}
	Q_jump_=Q_JumpList(Q_.size(),0);
}
void InternalForces::Q_calc_two_span_beam(double p, CS_Coordinates cs_coordinates, int temporary_supports_number)//����� �������������
{
	// ������ ������� �������� I ��� ����� 8.1.7
	double temp_Q=0.0;
	double l=cs_coordinates.back();

	double A=0.375*p*l/2.0;
	double B=1.25*p*l/2.0;

	for (int n = 0; n < cs_coordinates.size(); ++n)
	{
		double temp_l=cs_coordinates[n];

		if (cs_coordinates[n]<=l/2)
			{  //����� ������ ��� ���������� �������. ���������� ��������� enum
				temp_Q=A-p*temp_l;
			}
		else
			{
				temp_Q=A+B-p*temp_l;
			}
		Q_.push_back(temp_Q);
	}
	Q_jump_=Q_JumpList(Q_.size(),0);
	std::vector<double>::iterator itr = std::find(std::begin(cs_coordinates), std::end(cs_coordinates), 9000);

	Q_jump_[std::distance(cs_coordinates.begin(),itr)]=B;
}
//-----------------------------------------------------------------------------------------
// ���������� ����� �������������� ���
void InternalForces::Q_calc_three_span_beam(double p, CS_Coordinates cs_coordinates, int temporary_supports_number)
{

	double length = cs_coordinates.back();
	// �������� �������������� ���� �� ������
	double Q[6] = {0.4, -0.6, 0.5, 0.6, -0.5, -0.4};

	int n_point = cs_coordinates.size();
	int i, j;
	double del_x = length/n_point;  // ���������� ����� �������
	double del_Q = del_x*p;     // �������� �������������� ����
	double Q_temp;
	for (i = 0; i < 3; i++) {
	  Q_temp = Q[i*2]*p*length/3;
	  for (int j = 0; j < n_point/3 ; j++) {
		Q_.push_back(Q_temp);
		Q_temp -= del_Q;
	  }
	}

	Q_jump_=Q_JumpList(Q_.size(),0);
	std::vector<double>::iterator itr = std::find(std::begin(cs_coordinates), std::end(cs_coordinates), 9000);

	Q_jump_[std::distance(cs_coordinates.begin(),itr)] = 0;


}
void InternalForces::Q_calc_four_span_beam(double p, CS_Coordinates cs_coordinates, int temporary_supports_number)
{

	double length = cs_coordinates.back();
	// �������� �������������� ���� �� ������
	double Q[8] = {0.392, -0.608, 0.536, -0.464, 0.464, -0.536, 0.608, -0.392};

	int n_point = cs_coordinates.size();
	int i, j;
	double del_x = length/n_point;  // ���������� ����� �������
	double del_Q = del_x*p;     // �������� �������������� ����
	double Q_temp;
	for (i = 0; i < 4; i++) {
	  Q_temp = Q[i*2]*p*length/4;
	  for (int j = 0; j < n_point/4; j++) {
		Q_.push_back(Q_temp);
		Q_temp -= del_Q;
	  }
	}

	Q_jump_=Q_JumpList(Q_.size(),0);
	std::vector<double>::iterator itr = std::find(std::begin(cs_coordinates), std::end(cs_coordinates), 9000);

	Q_jump_[std::distance(cs_coordinates.begin(),itr)] = 0;

}
InternalForces::M_List InternalForces::get_M(LoadUnit load_unit,LengthUnit length_unit)const
{
	std::vector<double> temp_M (M_);
	std::transform(M_.begin(),M_.end(),temp_M.begin(), [load_unit, length_unit](double M){return M/static_cast<double>(load_unit)/static_cast<double>(length_unit);});
	return temp_M;
}
InternalForces::Q_List InternalForces::get_Q(LoadUnit load_unit) const
{
   std::vector<double> temp_Q (Q_);
   std::transform(Q_.begin(),Q_.end(),temp_Q.begin(), [load_unit](double Q){return Q/static_cast<double>(load_unit);});
   return temp_Q;
}
InternalForces::Q_List InternalForces::get_Q_jump(LoadUnit load_unit)const
{
	std::vector<double> temp_Q (Q_jump_);
	std::transform(Q_jump_.begin(),Q_jump_.end(),temp_Q.begin(), [load_unit](double Q){return Q/static_cast<double>(load_unit);});
	return temp_Q;
}


