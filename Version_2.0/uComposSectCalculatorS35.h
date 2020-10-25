//---------------------------------------------------------------------------

#ifndef uComposSectCalculatorS35H
#define uComposSectCalculatorS35H

#include "uGeometry.h"
#include "uLoads.h"
#include "uSteel.h"
#include "uCompositeSectionGeometry.h"
#include "uStud.h"
#include "uRebar.h"
#include "uWorkingConditionsFactors.h"
#include "uCorrugatedSheet.h"
#include "uSection.h"
#include "uSectOutputSP35.h"
#include "uIntForcesCalculator.h"

#include <utility>

enum class DesignCase{
	Case_A,
	Case_B,
	Case_C,
};
enum class Flange{
	upper,
	lower,
};

class ComposSectCalculatorSP35{

public:


	ComposSectCalculatorSP35(IntForcesCalculator const & intr_frcs_calculator,
							 WorkingConditionsFactors const & work_cond_factors,
							 ComposSectGeomSP35 const & com_sect,
							 double const sigma_bi,
							 double const sigma_ri);

	SectOutputSP35 calculate(int const id, double const x);

private:

	IntForcesCalculator const & intr_frcs_calculator_;
	WorkingConditionsFactors const & work_cond_factors_;
	ComposSectGeomSP35 const & com_sect_;
	double const sigma_bi_;
	double const sigma_ri_;

	DesignCase design_case(double sigma_b, double sigma_r);
	double N(DesignCase dc, Flange fl);


};

//---------------------------------------------------------------------------
#endif
