//---------------------------------------------------------------------------

#pragma hdrstop

#include "uStudRowCalculatorSP35.h"
#include<algorithm>
#include "uSteelTableObjects.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

StudRowCalculatorSP35::StudRowCalculatorSP35(ComBeamInputSP35 const & input)
{


	IntForcesCalculator ifc {input.glob_geom().tmp_sup_num(),
							 input.glob_geom().span(),
							 input.loads()};

	 ComposSectGeomSP35 cs (input.steel(),
							input.steel_sect().get(),
							input.concrete(),
							input.concrete_sect().get(),
							ComposSectGeomSP35::ConcStateConsid::normal);

	calc_ = {ifc, cs, input.glob_geom().span()};
}



StudRowOutputSP35 StudRowCalculatorSP35::run(StudRowInputSP35 & sri, StudSP35 & st)const
{
	int const id = sri.id();

	double const x = sri.x();
	double const x_l = sri.x_l();
	double const x_r = sri.x_r();

	int const st_num = sri.st_num();

	double const S_h = st.S_h();
	double const S_1 = st.S_1();

	double S_i = calc_.run(x_l, x_r);

	double const ratio = S_i / (st_num * std::min(S_h, S_1));

	return {id, x, S_i, ratio};

}
StudsOutputSP35 StudRowCalculatorSP35::run(StudsInputSP35 & st_input)const
{
	StudsOutputSP35 st_output;

	for(auto & sr0:st_input.rows())
		st_output.emplace_back(run(sr0, st_input.stud()));

	return st_output;
}
