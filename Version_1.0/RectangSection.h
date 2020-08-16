//---------------------------------------------------------------------------

#ifndef RectangSectionH
#define RectangSectionH
//---------------------------------------------------------------------------
#include<cmath>

using namespace std;

class RectangSection{
private:
	double h_p;//������ �������
	double h_f; //������� ������������ ��� ��������
	double A_b;//������� ������������
	double I_b;//������ ������� �������������� �����
	double b_sl;//��������� ������ �������������� �����
	double t_sl;//������� �������������� �����

private:
	void calc_inertia();

public:
	inline double get_sheet_height() const {return h_p;}
	inline double get_conc_topping() const {return h_f;}
	inline double get_area() const {return A_b;}
	inline double get_width() const {return b_sl;}
	inline double get_thickness() const {return t_sl;}
	double get_inertia() const {return I_b;};
};
#endif
