//---------------------------------------------------------------------------

#ifndef StressesH
#define StressesH

#include "uCompositeSectionGeometry.h"
#include "uWorkingConditionsFactors.h"


enum StressStateCase{ //��������� �������������� ��������
	UNDEFINED = 1,
	CASE_I,
	CASE_II,
	CASE_III,
};
//---------------------------------------------------------------------------
struct Stresses{ //��� ���������� ������������� ����� Stresses?
public:
	Stresses();
	Stresses (double sigma_b, double sigma_s, StressStateCase stress_state_case);
	inline double get_sigma_b()const {return sigma_b_;}
	inline double get_sigma_s()const {return sigma_s_;}
//	inline void add_sigma_b(double sigma_b){sigma_b_.push_back(sigma_b);}
//	inline void add_sigma_s(double sigma_s){sigma_s_.push_back(sigma_s);}
	inline StressStateCase get_state() const {return stress_state_case_;}
private:

	double sigma_b_ = 0.;//���������� � ������ �� ������ ������ ������� �����
	double sigma_s_ = 0.;//���������� � �������� ��������������� ����������� � ������ ��� ���������� sigma_b
	StressStateCase stress_state_case_ = UNDEFINED;
};

#endif
