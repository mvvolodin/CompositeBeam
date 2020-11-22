//---------------------------------------------------------------------------

#ifndef uComBeamOutputSP35H
#define uComBeamOutputSP35H
//---------------------------------------------------------------------------
#include <memory>
#include "uSectOutputSP35.h"
#include "uComposSectGeomSP35.h"
#include "uWord_Automation.h"
//---------------------------------------------------------------------------
class ComBeamOutputSP35{
public:

	ComBeamOutputSP35(ComposSectGeomSP35  com_sect,
					  ComposSectGeomSP35  com_sect_shr,
					  ComposSectGeomSP35  com_sect_kr,
					  std::vector<SectOutputSP35>  sect_outp_list);

	void print_data_to_report(TWord_Automation & report)const;

	std::vector<double> x_lst()const;

	std::vector<double> end_sup_coord()const;
	std::vector<double> inter_sup_coord()const;
	std::vector<double> sup_coord()const;

	std::vector<double> M_1a_lst()const;
	std::vector<double> M_1b_lst()const;
	std::vector<double> M_2c_lst()const;
	std::vector<double> M_2d_lst()const;
	std::vector<double> M_total_lst()const;

	std::vector<double> Q_1a_lst()const;
	std::vector<double> Q_1b_lst()const;
	std::vector<double> Q_2c_lst()const;
	std::vector<double> Q_2d_lst()const;
	std::vector<double> Q_total_lst()const;

	std::vector<double> R_1a_lst()const;
	std::vector<double> R_1b_lst()const;
	std::vector<double> R_2c_lst()const;
	std::vector<double> R_2d_lst()const;
    std::vector<double> R_total_lst()const;

private:
	ComposSectGeomSP35 const com_sect_;
	ComposSectGeomSP35 const com_sect_shr_;
	ComposSectGeomSP35 const com_sect_kr_;

	std::vector<SectOutputSP35> const sect_outp_list_;

	SectOutputSP35 const & max_st_sect_ratio_sect()const;
	SectOutputSP35 const & max_direct_str_ratio_sect()const;
	SectOutputSP35 const & max_shear_str_ratio_sect()const;

};

#endif
