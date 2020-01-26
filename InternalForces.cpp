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

			temp_M = A*temp_l - p*temp_l*temp_l/2;
		}
		else
		{
			temp_M=A*temp_l - p*temp_l*temp_l/2 + B*(temp_l-l/2);
		}
		M_.push_back(temp_M);
	}
}
//-----------------------------------------------------------
// ������ ����� ��������
void InternalForces::M_calc_three_span_beam(double p, CS_Coordinates cs_coordinates, int temporary_supports_number)
{
	double temp_M=0.0;
	double l=cs_coordinates.back();

	double A=0.4*p*l/3.0;
	double B=1.1*p*l/3.0;
	double C=1.1*p*l/3.0;

	for (int n = 0; n< cs_coordinates.size(); ++n)
	{
		double temp_l=cs_coordinates[n];

		if (cs_coordinates[n]<=l/3) {
		   temp_M = A*temp_l - p*temp_l*temp_l/2;
		   M_.push_back(temp_M);
		   continue;
		}
		if (cs_coordinates[n]<=2*l/3) {
		   temp_M = A*temp_l - p*temp_l*temp_l/2 + B*(temp_l - l/3);
		   M_.push_back(temp_M);
		   continue;
		}
		temp_M = A*temp_l + B*(temp_l - l/3) - p*temp_l*temp_l/2 + C*(temp_l - 2*l/3);
		M_.push_back(temp_M);
	}

}
void InternalForces::M_calc_four_span_beam(double p, CS_Coordinates cs_coordinates, int temporary_supports_number)
{
	double temp_M=0.0;
	double l=cs_coordinates.back();

	double A = 0.393*p*l/4.0;
	double B = 1.143*p*l/4.0;
	double C = 0.929*p*l/4.0;
	double D = 1.143*p*l/4.0;

	for (int n = 0; n< cs_coordinates.size(); ++n)
	{
		double temp_l=cs_coordinates[n];

		if (cs_coordinates[n]<=l/4)
		{
			temp_M = A*temp_l - p*temp_l*temp_l/2;
			M_.push_back(temp_M);
			continue;
		}
		if (cs_coordinates[n]<=l/2) {
			temp_M = A*temp_l - p*temp_l*temp_l/2 + B*(temp_l - l/4);
			M_.push_back(temp_M);
			continue;
		}
		if (cs_coordinates[n]<=3*l/4) {
			temp_M = A*temp_l + B*(temp_l - l/4) - p*temp_l*temp_l/2 + C*(temp_l - l/2);
			M_.push_back(temp_M);
			continue;
		}
		temp_M = A*temp_l + B*(temp_l - l/4) - p*temp_l*temp_l/2 + C*(temp_l - l/2) + D*(temp_l - 3*l/4);
		M_.push_back(temp_M);
	}


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
	std::vector<double>::iterator itr = std::find(std::begin(cs_coordinates), std::end(cs_coordinates), l/2);

	Q_jump_[std::distance(cs_coordinates.begin(),itr)]=B;
}
//-----------------------------------------------------------------------------------------
// ���������� ����� �������������� ���
void InternalForces::Q_calc_three_span_beam(double p, CS_Coordinates cs_coordinates, int temporary_supports_number)
{

	double temp_Q=0.0;
	double l=cs_coordinates.back();

	double A=0.4*p*l/3.0;
	double B=1.1*p*l/3.0;
	double C=1.1*p*l/3.0;

	for (int n = 0; n< cs_coordinates.size(); ++n)
	{
		double temp_l=cs_coordinates[n];

		if (cs_coordinates[n]<=l/3) {
		   temp_Q = A - p*temp_l;
		   Q_.push_back(temp_Q);
		   continue;
		}
		if (cs_coordinates[n]<=2*l/3) {
		   temp_Q = A - p*temp_l + B;
		   Q_.push_back(temp_Q);
		   continue;
		}
		temp_Q = A + B - p*temp_l + C;
		Q_.push_back(temp_Q);
	}

	Q_jump_=Q_JumpList(Q_.size(),0);
	std::vector<double>::iterator itr1 = std::find(std::begin(cs_coordinates), std::end(cs_coordinates), l/3);
	Q_jump_[std::distance(cs_coordinates.begin(),itr1)] = B;
	std::vector<double>::iterator itr2 = std::find(std::begin(cs_coordinates), std::end(cs_coordinates), 2*l/3);
	Q_jump_[std::distance(cs_coordinates.begin(),itr2)] = C;


}
void InternalForces::Q_calc_four_span_beam(double p, CS_Coordinates cs_coordinates, int temporary_supports_number)
{

	double temp_Q=0.0;
	double l=cs_coordinates.back();

	double A = 0.393*p*l/4.0;
	double B = 1.143*p*l/4.0;
	double C = 0.929*p*l/4.0;
	double D = 1.143*p*l/4.0;

	for (int n = 0; n< cs_coordinates.size(); ++n)
	{
		double temp_l=cs_coordinates[n];

		if (cs_coordinates[n]<=l/4)
		{
		   temp_Q = A - p*temp_l;
		   Q_.push_back(temp_Q);
		   continue;
		}
		if (cs_coordinates[n]<=l/2) {
		   temp_Q = A - p*temp_l + B;
		   Q_.push_back(temp_Q);
			continue;
		}
		if (cs_coordinates[n]<=3*l/4) {
		   temp_Q = A - p*temp_l + B + C;
		   Q_.push_back(temp_Q);
			continue;
		}
		temp_Q = A - p*temp_l + B + C + D;
		Q_.push_back(temp_Q);
	}

	Q_jump_=Q_JumpList(Q_.size(),0);
	std::vector<double>::iterator itr1 = std::find(std::begin(cs_coordinates), std::end(cs_coordinates), l/4);
	Q_jump_[std::distance(cs_coordinates.begin(),itr1)] = B;
	std::vector<double>::iterator itr2 = std::find(std::begin(cs_coordinates), std::end(cs_coordinates), l/2);
	Q_jump_[std::distance(cs_coordinates.begin(),itr2)] = C;
	std::vector<double>::iterator itr3 = std::find(std::begin(cs_coordinates), std::end(cs_coordinates), 3*l/4);
	Q_jump_[std::distance(cs_coordinates.begin(),itr3)] = D;

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


