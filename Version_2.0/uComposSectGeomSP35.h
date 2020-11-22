//---------------------------------------------------------------------------
#ifndef uComposSectGeomSP35H
#define uComposSectGeomSP35H
 //---------------------------------------------------------------------------
//#define NDEBUG
#ifndef NDEBUG
#include "uFrmLogger.h"
#endif
//---------------------------------------------------------------------------
#include <memory>
#include "uGeneralConcreteSection.h"
#include "uGeneralSteelSection.h"
#include "uConcrete.h"
#include "uSteel.h"
#include "uWord_Automation.h"
//---------------------------------------------------------------------------
class ComposSectGeomSP35{

public:

	enum class ConcStateConsid{
		normal,
		shrink,
		creep,
	};
	ComposSectGeomSP35() = default;


	ComposSectGeomSP35(Steel const steel,
					   std::shared_ptr<GeneralSteelSection const> st_sect,
					   Concrete const concrete,
					   std::shared_ptr<GeneralConcreteSection const> conc_sect,
					   ConcStateConsid const conc_st_consid);

	double E_rs()const;
	double E_b()const;
	double E_st()const;

	double n_b()const;
	double n_r()const;

	double R_y()const;
	double R_b()const;
	double R_r()const;

	double A_s()const;
	double A_s2()const;
	double A_s1()const;
	double W_s1_s()const;
	double W_s2_s()const;
	double I_s()const;

	double A_st()const;
	double I_st()const;

	double A_b()const;
	double I_b()const;

	double Z_b_s()const;
	double Z_b_stb()const;
	double Z_s_stb()const;
	double Z_r_stb()const;
	double A_stb()const;
	double W_b_stb()const;
	double I_stb()const;
	double eps_shr()const;

	double A_r()const;

	void print_data_to_report(TWord_Automation & report)const;

#ifndef NDEBUG
	void print_data_to_logger(TFormLogger const & log)const;
#endif

private:

	Steel const steel_;
	std::shared_ptr<GeneralSteelSection const> st_sect_;
	Concrete const concrete_;
	std::shared_ptr<GeneralConcreteSection const> conc_sect_;

	void calculate(double const E_b);
	void calculate(ConcStateConsid conc_st_consid);

	double E_ef_kr()const;

	double n_r_= 0.;//����������� ���������� � �����
	double n_b_= 0.;//����������� ���������� � ������

	double H_stb_= 0.; //������ ������������ �������
	double A_stb_= 0.; //������� �������������������� �������
	double S_stb_= 0.;//����������� ������ ������� �������������������� ������� ������������ �.�. ��������� �������
	double I_stb_= 0.;  //������ ������� �������������������� ������� ����������� � �������
	double W_b_stb_= 0.;//������ ������������� �������������������� ������� ��� �.�. �������������� �����

	double Z_s_stb_= 0.;//���������� ����� ������� ������� ��������� � �������������������� �������
	double Z_b_stb_= 0.;  //���������� ����� �.�. ��������� � �������������������� �������

	double Z_st_r_u_= 0.;//���������� ����� �.�. �������� ����� � ������� ���������
	double Z_st_r_l_= 0.;//���������� ����� �.�. �������� ����� � ������ ���������
	double Z_b_st_= 0.; //���������� ����� �������� ������� ����� � �������� �����

	double Z_stb_r_u_= 0.;//���������� ����� �.�. �������������������� ������� � ������� ���������
	double Z_stb_r_l_= 0.;//���������� ����� �.�. �������������������� ������� � ������ ���������

};

//---------------------------------------------------------------------------
#endif
