//---------------------------------------------------------------------------

#ifndef uBilinearInterpH
#define uBilinearInterpH
//---------------------------------------------------------------------------
namespace SP_35_13330_2011_table_8_16
{

	double bilinear_interpolation(double A_f_min_to_A_w_ratio,
								  double A_f_min_plus_A_w_to_A_ratio);
}
namespace SP_35_13330_2011_table_9_5
{
	enum class FlangeBendingAndAxialStressSumUp
	{
		bigger,
		smaller
	};
	double bilinear_interpolation(double fl_ratio,
								  double str_ratio,
								  FlangeBendingAndAxialStressSumUp fl);
}
#endif


