//---------------------------------------------------------------------------
#ifndef uRolledSectionH
#define uRolledSectionH
//---------------------------------------------------------------------------
#include "uGeneralSteelSection.h"
#include <string>
#include "Logger.h"
//---------------------------------------------------------------------------
class RolledSection:public GeneralSteelSection{
public:
	 RolledSection(int rolled_sect_type, int rolled_sect_num);

	~RolledSection();

	std::wstring name()const override;

	double upper_fl_width() const override;
	double upper_fl_thick() const override;
	double lower_fl_width() const override;
	double lower_fl_thick() const override;
	double web_height()const override;
	double web_thick()const override;
	double sect_height()const override;

	double grav_cent_upper_fl_dist() const override;
	double grav_cent_lower_fl_dist() const override;
	double grav_cent() const override;
	double area() const override;
	double area_upper_fl() const override ;
	double area_lower_fl() const override;
	double area_web()const override;
	double inertia() const override;
	double modulus_upper_fl()const override;
	double modulus_lower_fl()const override;
	double smaller_fl_to_larger_fl_ratio()const override;
	double smaller_fl_area_to_web_area_ratio()const override;
	double smaller_fl_area_plus_web_area_to_total_area_ratio()const override;
	double SW()const override;
    double radius()const;
#ifndef NDEBUG
	void print_data_to_logger(TFormLogger const & log)const override;
#endif
private:
	std::wstring prof_num_;
	double lower_fl_width_;
	double lower_fl_thick_;
	double upper_fl_width_;
	double upper_fl_thick_;
	double web_height_;
	double web_thick_;
    double rad_;
	double grav_cent_;
	double area_;
	double inertia_;
};
//---------------------------------------------------------------------------
#endif
