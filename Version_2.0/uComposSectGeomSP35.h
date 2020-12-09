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
#include <Vcl.Grids.hpp>
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


	ComposSectGeomSP35(Steel const & steel,
					   GeneralSteelSection const * const st_sect,
					   Concrete const & concrete,
					   GeneralConcreteSection const * const conc_sect,
					   ConcStateConsid const conc_st_consid);

	double E_rs()const;
	double E_b()const;
	double E_b_shr()const;
	double E_st()const;

	double n_b()const;
	double n_r()const;

	double R_y()const;
	double R_b()const;
	double R_r()const;

	double eps_b_lim() const;

	double A_s()const;
	double A_s2()const;
	double A_s1()const;
    double A_w()const;
	double W_s1_s()const;
	double W_s2_s()const;
	double I_s()const;

	double A_st()const;
	double S_st()const;
    double I_st()const;

	double A_b()const;
	double I_b()const;

	double Z_b_s()const;
	double Z_b_stb()const;
    double Z_b_st()const;
	double Z_s_stb()const;
	double Z_r_stb()const;
	double A_stb()const;
	double W_b_stb()const;
    double W_b_s()const;
	double I_stb()const;
	double eps_shr()const;

	double A_r()const;

	void print(TWord_Automation & report)const;
	void fill_grid(TStringGrid* str_grid)const;
	GeneralSteelSection const * const st_sect()const;
	GeneralConcreteSection const * const conc_sect()const;

#ifndef NDEBUG
	void print_data_to_logger(TFormLogger const & log)const;
#endif

private:

	Steel const & steel_;
	GeneralSteelSection const * const st_sect_;
	Concrete const & concrete_;
	GeneralConcreteSection const * const conc_sect_;

	void calculate(double const E_b);
	void calculate(ConcStateConsid conc_st_consid);

	double E_ef_kr()const;

	double n_r_= 0.;//����������� ���������� � �����
	double n_b_= 0.;//����������� ���������� � ������

	double A_st_ = 0.;//������� �������� ����� ������� ������� ��������
	double S_st_ = 0.;//����������� ������ �������� ����� ������� ������� ��������
    double I_st_ = 0.;//������ ������� �������� ����� ������� ������� ��������

	double h_stb_= 0.; //������ ������������ �������
	double A_stb_= 0.; //������� �������������������� �������
	double S_stb_= 0.;//����������� ������ ������� �������������������� ������� ������������ �.�. ��������� �������
	double I_stb_= 0.;  //������ ������� �������������������� ������� ����������� � �������
	double W_b_stb_= 0.;//������ ������������� �������������������� ������� ��� �.�. �������������� �����

	double W_b_s_ = 0.;//�������� ������ ������������� �� ������ ������ ������� ������� ������

	double Z_s_r_u_= 0.;//���������� ����� �.�. �������� ����� � ������� ���������
	double Z_s_r_l_= 0.;//���������� ����� �.�. �������� ����� � ������ ���������
	double Z_b_s_= 0.; //���������� ����� �������� ������� ����� � �������� �����

	double Z_s_stb_ = 0.;//���������� ����� �.�. ������� �������� � ������������������� �����
	double Z_b_st_ = 0.;//���������� ����� �.�.��������� ������� � �������� ����� ������� ��������
	double Z_s_st_ = 0.; //���������� ����� �.�. ������� �������� ����� � �������� ����� ������� ��������
	double Z_st_stb_ = 0.;//���������� ����� �.�. �������� ����� ������� ������� �������� � �������������������� �������
	double Z_b_stb_= 0.;  //���������� ����� �.�. ��������� � �������������������� �������
	double Z_r_u_stb_= 0.;//���������� ����� �.�. �������������������� ������� � ������� ���������
	double Z_r_l_stb_= 0.;//���������� ����� �.�. �������������������� ������� � ������ ���������


};

//---------------------------------------------------------------------------
#endif
