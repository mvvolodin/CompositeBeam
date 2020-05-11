//---------------------------------------------------------------------------

#ifndef uCompositeBeamH
#define uCompositeBeamH

#include "CompositeSection.h"
#include "uWorkingConditionsFactors.h"
#include "uInternalForcesCalculator.h"
#include "uSection.h"


#include "MathFunction.h"

class CompositeBeam{
public:
	CompositeBeam();
	void set_intr_frcs_calculator(InternalForcesCalculator& intr_frcs_calculator);
	void set_composite_section(CompositeSection& com_sect);
	void set_working_conditions_factors(WorkingConditionsFactors& working_conditions_factors);
	void initialize_section_list(double L, int temporary_supports_number);

	const std::vector<Section>& get_section_list(){return section_list_;}

	void calculate();

    std::vector<double> get_x_list()const;
	std::vector<double> get_support_x_list()const;

	std::vector<double> get_M_Ia_design_list(LoadUnit load_unit=LoadUnit::N, LengthUnit length_unit=LengthUnit::mm)const;
	std::vector<double> get_M_Ib_design_list(LoadUnit load_unit=LoadUnit::N, LengthUnit length_unit=LengthUnit::mm)const;
	std::vector<double> get_M_IIa_design_list(LoadUnit load_unit=LoadUnit::N, LengthUnit length_unit=LengthUnit::mm) const;
	std::vector<double> get_M_IIb_design_list(LoadUnit load_unit=LoadUnit::N, LengthUnit length_unit=LengthUnit::mm)const;
	std::vector<double> get_M_total_design_list(LoadUnit load_unit=LoadUnit::N, LengthUnit length_unit=LengthUnit::mm)const;

	std::vector<double> get_Q_Ia_design_list(LoadUnit load_unit=LoadUnit::N)const;
	std::vector<double> get_Q_Ib_design_list(LoadUnit load_unit=LoadUnit::N)const;
	std::vector<double> get_Q_IIa_design_list(LoadUnit load_unit=LoadUnit::N)const;
	std::vector<double> get_Q_IIb_design_list(LoadUnit load_unit=LoadUnit::N)const;
	std::vector<double> get_Q_total_design_list(LoadUnit load_unit=LoadUnit::N)const;

	std::vector<double> get_R_Ia_design_list(LoadUnit load_unit=LoadUnit::N)const;
	std::vector<double> get_R_Ib_design_list(LoadUnit load_unit=LoadUnit::N)const;
	std::vector<double> get_R_IIa_design_list(LoadUnit load_unit=LoadUnit::N)const;
	std::vector<double> get_R_IIb_design_list(LoadUnit load_unit=LoadUnit::N)const;
	std::vector<double> get_R_total_design_list(LoadUnit load_unit=LoadUnit::N)const;

	const Section& get_max_direct_stress_ratio_section()const;
	const Section& get_max_shear_stress_ratio_section()const;
	const Section& get_max_rigid_plastic_ratio_section()const;

	const CompositeSection& get_composite_section()const {return com_sect_;};

private:
	InternalForcesCalculator intr_frcs_calculator_;
	WorkingConditionsFactors working_conditions_factors_;
	CompositeSection com_sect_;

	std::vector<Section> section_list_;
    std::vector<double> support_list_;

	std::vector<double> make_R_list(std::map<double, double> R_LCC)const;
	void initialize_support_list(double L, int temporary_supports_number);
};

//---------------------------------------------------------------------------
#endif
