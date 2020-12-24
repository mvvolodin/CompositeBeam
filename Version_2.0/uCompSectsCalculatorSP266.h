//---------------------------------------------------------------------------
#ifndef uCompSectsCalculatorSP266H
#define uCompSectsCalculatorSP266H
//---------------------------------------------------------------------------
#include <vector>
#include <string>
#include "uCompSectOutputSP266.h"
#include "uCompSectsOutputListSP266.h"

#include "uGlobGeom.h"
#include "uIntForcesCalculator.h"
#include "uCompSectGeomSP266.h"

#include "uComposSectGeomSP35.h"
#include "uWorkingConditionsFactors.h"

#include "uCompBeamObjsCreatorSP266.h"

enum class DesignCaseSP266{
	Case_A,
	Case_B,
	Case_C,
	Case_F // ������, ����� � ������ ����������
};

class CompSectsCalculatorSP266{
public:
	CompSectsCalculatorSP266 (CompBeamObjsCreatorSP266 const & creator);
	CompSectsOutputListSP266 run();
private:
	CompBeamObjsCreatorSP266 const creator_;

	IntForcesCalculator const intr_frcs_calculator_;
	CompSectGeomSP266 const comp_sect_;
	WorkingConditionsFactors const work_cond_factors_;

	CompSectOutputSP266 calculate(Node const & node);
	DesignCaseSP266 design_case(double sigma_b, double sigma_r);
};
#endif





