//---------------------------------------------------------------------------

#ifndef uCorrugatedSheetH
#define uCorrugatedSheetH

#include<System.hpp>
#include<map>
#include<vector>

class CorrugatedSheet{
public:
	CorrugatedSheet();
	CorrugatedSheet (String name, double h_n, double t, double w);
	CorrugatedSheet (String name, double h_n, double t,
					 double b_ap_, double b, double S_n, double w);
	String get_name() const {return name_;}
	double get_height() const {return h_n_;}
	double get_thickness() const {return t_;}
	double get_weight() const {return w_;}
	double get_h_b();
private:
	String name_ = "";
	double h_n_ = 0.;// ������ �������
	double t_ = 0.; // ������� �������
	double b_ap_ = 0.; //���������� ����� ����������� ������� �����
	double b_ = 0.;// ������ ������ �����
	double S_n_ = 0.;// ��� ������ �����
	double w_ {0.}; //����� 1�2

	double h_b_;
	bool fully_initialized = false;


};

class CorrugatedSheetsData{
public:
	static CorrugatedSheet get_corrugated_sheet(const String& name);
	static std::vector<String> get_corrugated_sheet_names();
private:
	static std::vector<CorrugatedSheet> corrugated_sheets_list_;
};



//---------------------------------------------------------------------------
#endif
