//---------------------------------------------------------------------------

#pragma hdrstop

#include "CompositeBeam.h"
#include "Logger.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
TCompositeBeam::TCompositeBeam()
{

}
//---------------------------------------------------------------------------
//����������� ����������� �����
//---------------------------------------------------------------------------
 TCompositeBeam::TCompositeBeam(TGeometry                geometry,
								TLoads     				 loads,
								CompositeSection         composite_section,
								TStud 					 stud,
								WorkingConditionsFactors working_conditions_factors,
								int 					 beam_division)
 	:geometry_(geometry),
	composite_section_(composite_section),
	stud_(stud),
	loads_(loads),
	working_conditions_factors_(working_conditions_factors),
	beam_division_(beam_division)
 {
	CS_coordinates_calc();
	calc_inter_forces();
	calc_stresses();
	print_i_sect_to_log();
	print_concrete_part_to_log();
	print_comp_sect_to_log();
	print_inter_forces_to_log();
	print_stresses_to_log();
}
//---------------------------------------------------------------------------
//������ ���������� �������� � ��� ��� ����������� ������� ����������
//---------------------------------------------------------------------------
void TCompositeBeam::calc_inter_forces()
{
	double SW=loads_.get_self_weight();
	double DL_I=loads_.get_dead_load_first_stage();
	double DL_II=loads_.get_dead_load_second_stage();
	double LL=loads_.get_live_load();

	double bl=geometry_.get_trib_width_left();
	double br=geometry_.get_trib_width_right();

	//������ �������� ��������

	double SW_l=SW;
	double DL_I_l=DL_I*(bl+br);
	double DL_II_l=DL_II*(bl+br);
	double LL_l=LL*(bl+br);

	//���������� ������� � �������� ���������� ���

   //	InternalForces::CS_coordinates cs_coordinates=geometry_.get_CS_coordinates();
	int propping_number=geometry_.get_propping_number();

	//������������ ������������ ������ � ����������� �������� �� ������� ����������

	internal_forces_LC_.insert(InternalForcesNamededListItem(LoadCaseNames::SW,InternalForces(SW_l,cs_coordinates_,0)));
	internal_forces_LC_.insert(InternalForcesNamededListItem(LoadCaseNames::DL_I,InternalForces(DL_I_l,cs_coordinates_,0)));
	internal_forces_LC_.insert(InternalForcesNamededListItem(LoadCaseNames::DL_II,InternalForces(DL_II_l,cs_coordinates_,propping_number)));
	internal_forces_LC_.insert(InternalForcesNamededListItem(LoadCaseNames::LL,InternalForces(LL_l,cs_coordinates_,propping_number)));

	//������������ ������ � ����������� ��������

	double gamma_f_SW=loads_.get_gamma_f_SW();
	double gamma_f_DL_I=loads_.get_gamma_f_DL_I();
	double gamma_f_DL_II=loads_.get_gamma_f_DL_II();
	double gamma_f_LL=loads_.get_gamma_f_LL();

	for (int i = 0; i < (beam_division_+1); i++)
	{
		double temp_M=0.0;
		double temp_Q=0.0;

		temp_M=gamma_f_SW*internal_forces_LC_[LoadCaseNames::SW].get_M()[i]+
			   gamma_f_DL_I*internal_forces_LC_[LoadCaseNames::DL_I].get_M()[i];
		temp_Q=gamma_f_SW*internal_forces_LC_[LoadCaseNames::SW].get_Q()[i]+
			   gamma_f_DL_I*internal_forces_LC_[LoadCaseNames::DL_I].get_Q()[i];

		internal_forces_I_.add_M(temp_M);
		internal_forces_I_.add_Q(temp_Q);
	}

	for (int i = 0; i < (beam_division_+1); i++)
	{
		double temp_M=0.0;
		double temp_Q=0.0;

		temp_M=gamma_f_DL_II*internal_forces_LC_[LoadCaseNames::DL_II].get_M()[i]+
			   gamma_f_LL*internal_forces_LC_[LoadCaseNames::LL].get_M()[i];
		temp_Q=gamma_f_DL_II*internal_forces_LC_[LoadCaseNames::DL_II].get_Q()[i]+
			   gamma_f_LL*internal_forces_LC_[LoadCaseNames::LL].get_Q()[i];

		internal_forces_II_.add_M(temp_M);
		internal_forces_II_.add_Q(temp_Q);

	}

	for (int i = 0; i < (beam_division_+1); i++)
	{
		double temp_M=0.0;
		double temp_Q=0.0;

		temp_M=internal_forces_I_.get_M()[i]+internal_forces_II_.get_M()[i];
		temp_Q=internal_forces_I_.get_Q()[i]+internal_forces_II_.get_Q()[i];

		internal_forces_total_.add_M(temp_M);
		internal_forces_total_.add_Q(temp_Q);
	}

}
//---------------------------------------------------------------------------
//������ ����������
//---------------------------------------------------------------------------
void TCompositeBeam::calc_stresses()
{
	stresses_LC_.insert(StressesNamedListItem(LoadCaseNames::SW,Stresses(internal_forces_LC_[LoadCaseNames::SW],composite_section_, working_conditions_factors_)));
	stresses_LC_.insert(StressesNamedListItem(LoadCaseNames::DL_I,Stresses(internal_forces_LC_[LoadCaseNames::DL_I],composite_section_, working_conditions_factors_)));
	stresses_LC_.insert(StressesNamedListItem(LoadCaseNames::DL_II,Stresses(internal_forces_LC_[LoadCaseNames::DL_II],composite_section_, working_conditions_factors_)));
	stresses_LC_.insert(StressesNamedListItem(LoadCaseNames::LL,Stresses(internal_forces_LC_[LoadCaseNames::LL],composite_section_, working_conditions_factors_)));

	stresses_I_= Stresses(internal_forces_I_,composite_section_, working_conditions_factors_);
	stresses_II_= Stresses(internal_forces_II_,composite_section_, working_conditions_factors_);
	stresses_total_= Stresses(internal_forces_II_,composite_section_, working_conditions_factors_);
	
}
//---------------------------------------------------------------------------
//������ ���� � ������������ ��������� �������
//---------------------------------------------------------------------------
void TCompositeBeam::CS_coordinates_calc()
	{   double span=geometry_.get_span();
		double propping_number_=geometry_.get_propping_number();

		double temp_CS_coordinate=0.0;

		//��������� � ���� ��������� ��������� ������� ���������� ������� � �������� �������� ������ �����

		cs_coordinates_.push_back(span/2.0);

		for (int n = 0; n <= beam_division_; ++n)
		{
			temp_CS_coordinate=span/beam_division_*n;
			cs_coordinates_.push_back(temp_CS_coordinate);
		}
		//��������� � ���� ��������� ��������� ������� ���������� ������� � ��� �������������� �������
		for (int n = 1; n < (propping_number_+1); ++n)
		{
			temp_CS_coordinate=span/(propping_number_+1)*n;
			cs_coordinates_.push_back(temp_CS_coordinate);
		}

		//��������� � � ���� ��������� ��������� ������� ��������� ������� � ��������� �������
		for (int n = 1; n < (propping_number_+1); ++n)
		{
			temp_CS_coordinate=span/(2.0*(propping_number_+1))*n;
			cs_coordinates_.push_back(temp_CS_coordinate);
		}

		std::sort(cs_coordinates_.begin(), cs_coordinates_.end());
		cs_coordinates_.erase( std::unique( cs_coordinates_.begin(), cs_coordinates_.end()), cs_coordinates_.end() );
	}
//---------------------------------------------------------------------------
//������ ������������ ������ ������ �������� �����
//---------------------------------------------------------------------------
 void TCompositeBeam::calculate_gamma_1()
 {
	double gamma_bi=working_conditions_factors_.get_gamma_bi();
	double R_b=composite_section_.get_concrete_part()->get_R_b();
	//double R_y=composite_section_->get_steel_part().get_R_y();
   //	gamma_1_=1+(gamma_bi*R_b-sigma_b)/(gamma_c*R_y)*A_b/A_f2_st;
 }

 //---------------------------------------------------------------------------
//������ ��������� �������� � log ����
//---------------------------------------------------------------------------
void TCompositeBeam::print_i_sect_to_log()
{
	PRINT(logfile,"-----��������� ��������-------------------") ;
	PRINT(logfile,"------------------------------------------") ;
	DUMP(logfile,"�������",composite_section_.get_steel_part().get_A_st());
	DUMP(logfile,"����������� �� ����� ������� �� �������� ����� ������� �����",composite_section_.get_steel_part().get_Z_f2_st());
	DUMP(logfile,"������ �������",composite_section_.get_steel_part().get_I_st());
}
//---------------------------------------------------------------------------
//������ �������������� ����� � log
//---------------------------------------------------------------------------
void TCompositeBeam::print_concrete_part_to_log()
{
    PRINT(logfile,"------------------------------------------")
	PRINT(logfile,"-----�������������� �����-------") ;
	PRINT(logfile,"------------------------------------------") ;
	DUMP(logfile,"��������� ������������� ������ ������� ������",composite_section_.get_concrete_part()->get_R_b());
	DUMP(logfile,"��������� ������������� ������ ������� ����������",composite_section_.get_concrete_part()->get_R_bt());
	DUMP(logfile,"��������� ������ ��������� ������",composite_section_.get_concrete_part()->get_E_b());
	DUMP(logfile,"������� �������������� �����",composite_section_.get_concrete_part()->get_h_b());
	DUMP(logfile,"��������� ������� �������������� �����",composite_section_.get_concrete_part()->get_t_sl());
	DUMP(logfile,"��������� ������ �������������� ����� �����",composite_section_.get_concrete_part()->get_b_l());
	DUMP(logfile,"��������� ������ �������������� ����� ������",composite_section_.get_concrete_part()->get_b_r());
	DUMP(logfile,"���������� �� ������ ����� �������������� ����� �� ������ �������",composite_section_.get_concrete_part()->get_C_b());
	DUMP(logfile,"������� �������������� �����",composite_section_.get_concrete_part()->get_A_b());
	DUMP(logfile,"������ ������� ������������� �����",composite_section_.get_concrete_part()->get_I_b());
}
//---------------------------------------------------------------------------
//������ ���������� ������ � log ����
//---------------------------------------------------------------------------

void TCompositeBeam::print_inter_forces_to_log()
{
	PRINT(logfile,"-------------------------------------------------------------");
	PRINT(logfile,"-------------���������� ���ר���� �������--------------------") ;
	PRINT(logfile,"-------------------------------------------------------------") ;

	for (int i=0; i <cs_coordinates_.size(); ++i)
		DUMP(logfile,"CS ���������� ", cs_coordinates_[i]);

	PRINT(logfile,"-------------------------------------------------------------");
	PRINT(logfile,"-------------���������� ������ LC SW-------------------------") ;
	PRINT(logfile,"-------------------------------------------------------------") ;

	for (int i=0; i < internal_forces_LC_[LoadCaseNames::SW].get_Q().size(); ++i)
		DUMP(logfile,"Q",internal_forces_LC_[LoadCaseNames::SW].get_Q()[i]);
	for (int i=0; i < internal_forces_LC_[LoadCaseNames::SW].get_M().size(); ++i)
		DUMP(logfile,"M",internal_forces_LC_[LoadCaseNames::SW].get_M()[i]);

	PRINT(logfile,"-------------------------------------------------------------");
	PRINT(logfile,"-------------���������� ������ LC DL_I--------------- --------") ;
	PRINT(logfile,"-------------------------------------------------------------") ;

	for (int i=0; i < internal_forces_LC_[LoadCaseNames::DL_I].get_Q().size(); ++i)
		DUMP(logfile,"Q",internal_forces_LC_[LoadCaseNames::DL_I].get_Q()[i]);
	for (int i=0; i < internal_forces_LC_[LoadCaseNames::DL_I].get_M().size(); ++i)
		DUMP(logfile,"M",internal_forces_LC_[LoadCaseNames::DL_I].get_M()[i]);

	PRINT(logfile,"-------------------------------------------------------------");
	PRINT(logfile,"-------------���������� ������ LC DL_II----------------------") ;
	PRINT(logfile,"-------------------------------------------------------------") ;

	for (int i=0; i < internal_forces_LC_[LoadCaseNames::DL_II].get_Q().size(); ++i)
		DUMP(logfile,"Q",internal_forces_LC_[LoadCaseNames::DL_II].get_Q()[i]);
	for (int i=0; i < internal_forces_LC_[LoadCaseNames::DL_II].get_M().size(); ++i)
		DUMP(logfile,"M",internal_forces_LC_[LoadCaseNames::DL_II].get_M()[i]);

	PRINT(logfile,"-------------------------------------------------------------");
	PRINT(logfile,"-------------���������� ������ LC LL-------------------------") ;
	PRINT(logfile,"-------------------------------------------------------------") ;

	for (int i=0; i < internal_forces_LC_[LoadCaseNames::LL].get_Q().size(); ++i)
		DUMP(logfile,"Q",internal_forces_LC_[LoadCaseNames::LL].get_Q()[i]);
	for (int i=0; i < internal_forces_LC_[LoadCaseNames::LL].get_M().size(); ++i)
		DUMP(logfile,"M",internal_forces_LC_[LoadCaseNames::LL].get_M()[i])

	PRINT(logfile,"-------------------------------------------------------------");
	PRINT(logfile,"-------------���������� ������ I ������ ������----------------") ;
	PRINT(logfile,"-------------------------------------------------------------") ;

	for (int i=0; i < internal_forces_I_.get_Q().size(); ++i)
		DUMP(logfile,"Q",internal_forces_I_.get_Q()[i]);
	for (int i=0; i < internal_forces_I_.get_M().size(); ++i)
		DUMP(logfile,"M",internal_forces_I_.get_M()[i])

	PRINT(logfile,"-------------------------------------------------------------");
	PRINT(logfile,"-------------���������� ������ II ������ ������--------------") ;
	PRINT(logfile,"-------------------------------------------------------------") ;

	for (int i=0; i < internal_forces_II_.get_Q().size(); ++i)
		DUMP(logfile,"Q",internal_forces_II_.get_Q()[i]);
	for (int i=0; i < internal_forces_II_.get_M().size(); ++i)
		DUMP(logfile,"M",internal_forces_II_.get_M()[i])

	PRINT(logfile,"-------------------------------------------------------------");
	PRINT(logfile,"-------------���������� ������ ���������----------------------") ;
	PRINT(logfile,"-----------------------------------------------------------------") ;

	for (int i=0; i < internal_forces_total_.get_Q().size(); ++i)
		DUMP(logfile,"Q",internal_forces_total_.get_Q()[i]);
	for (int i=0; i < internal_forces_total_.get_M().size(); ++i)
		DUMP(logfile,"M",internal_forces_total_.get_M()[i])

}
 //---------------------------------------------------------------------------
//������ ���������� � log
//---------------------------------------------------------------------------
void TCompositeBeam::print_stresses_to_log()
{

	PRINT(logfile,"-------------------------------------------------------------");
	PRINT(logfile,"-------------���������� LC SW---------------------------------") ;
	PRINT(logfile,"-------------------------------------------------------------") ;

	for (int i=0; i < stresses_LC_[LoadCaseNames::SW].get_sigma_b().size(); ++i)
	{

		DUMP_PAIR_FLOAT_STRING(logfile, "SigmaB", stresses_LC_[LoadCaseNames::SW].get_sigma_b()[i],
										"State",  stresses_LC_[LoadCaseNames::SW].get_state_sigma_b()[i]);
	}
	PRINT(logfile,"");
	for (int i=0; i < stresses_LC_[LoadCaseNames::SW].get_sigma_b().size(); ++i)
	{

		DUMP_PAIR_FLOAT_STRING(logfile, "SigmaS", stresses_LC_[LoadCaseNames::SW].get_sigma_s()[i],
										"State",  stresses_LC_[LoadCaseNames::SW].get_state_sigma_s()[i]);
	}
	PRINT(logfile,"-------------------------------------------------------------");
	PRINT(logfile,"-------------���������� LC DL_I---------------------------------") ;
	PRINT(logfile,"-------------------------------------------------------------")
	for (int i=0; i < stresses_LC_[LoadCaseNames::SW].get_sigma_b().size(); ++i)
	{

		DUMP_PAIR_FLOAT_STRING(logfile, "SigmaB", stresses_LC_[LoadCaseNames::DL_I].get_sigma_b()[i],
										"State",  stresses_LC_[LoadCaseNames::DL_I].get_state_sigma_b()[i]);
	}
	PRINT(logfile,"");
	for (int i=0; i < stresses_LC_[LoadCaseNames::SW].get_sigma_b().size(); ++i)
	{

		DUMP_PAIR_FLOAT_STRING(logfile, "SigmaS", stresses_LC_[LoadCaseNames::DL_I].get_sigma_s()[i],
										"State",  stresses_LC_[LoadCaseNames::DL_I].get_state_sigma_s()[i]);
	}
	PRINT(logfile,"-------------------------------------------------------------");
	PRINT(logfile,"-------------���������� LC DL_II---------------------------------") ;
	PRINT(logfile,"-------------------------------------------------------------")
	for (int i=0; i < stresses_LC_[LoadCaseNames::SW].get_sigma_b().size(); ++i)
	{

		DUMP_PAIR_FLOAT_STRING(logfile, "SigmaB", stresses_LC_[LoadCaseNames::DL_II].get_sigma_b()[i],
										"State",  stresses_LC_[LoadCaseNames::DL_II].get_state_sigma_b()[i]);
	}
	PRINT(logfile,"");
	for (int i=0; i < stresses_LC_[LoadCaseNames::SW].get_sigma_b().size(); ++i)
	{

		DUMP_PAIR_FLOAT_STRING(logfile, "SigmaS", stresses_LC_[LoadCaseNames::DL_II].get_sigma_s()[i],
										"State",  stresses_LC_[LoadCaseNames::DL_II].get_state_sigma_s()[i]);
	}
	PRINT(logfile,"-------------------------------------------------------------");
	PRINT(logfile,"-------------���������� LC LL---------------------------------") ;
	PRINT(logfile,"-------------------------------------------------------------")
	for (int i=0; i < stresses_LC_[LoadCaseNames::SW].get_sigma_b().size(); ++i)
	{

		DUMP_PAIR_FLOAT_STRING(logfile, "SigmaB", stresses_LC_[LoadCaseNames::LL].get_sigma_b()[i],
										"State",  stresses_LC_[LoadCaseNames::LL].get_state_sigma_b()[i]);
	}
	PRINT(logfile,"");
	for (int i=0; i < stresses_LC_[LoadCaseNames::SW].get_sigma_b().size(); ++i)
	{

		DUMP_PAIR_FLOAT_STRING(logfile, "SigmaS", stresses_LC_[LoadCaseNames::LL].get_sigma_s()[i],
										"State",  stresses_LC_[LoadCaseNames::LL].get_state_sigma_s()[i]);
	}
	PRINT(logfile,"-------------------------------------------------------------");
	PRINT(logfile,"-------------���������� I ������ ������------------------------") ;
	PRINT(logfile,"-------------------------------------------------------------")
	for (int i=0; i < stresses_I_.get_sigma_b().size(); ++i)
	{
		double  a= stresses_I_.get_sigma_b()[2];
	  //	DUMP_PAIR_FLOAT_STRING(logfile, "SigmaB", stresses_I_.get_sigma_b()[i],
					//					"State",  stresses_I_.get_state_sigma_b()[i]);
	}
	PRINT(logfile,"");
	for (int i=0; i < stresses_I_.get_sigma_b().size(); ++i)
	{

		DUMP_PAIR_FLOAT_STRING(logfile, "SigmaS", stresses_I_.get_sigma_s()[i],
										"State",  stresses_I_.get_state_sigma_s()[i]);
	}
	PRINT(logfile,"-------------------------------------------------------------");
	PRINT(logfile,"-------------���������� II ������ ������------------------------") ;
	PRINT(logfile,"-------------------------------------------------------------")
	for (int i=0; i < stresses_II_.get_sigma_b().size(); ++i)
	{

		DUMP_PAIR_FLOAT_STRING(logfile, "SigmaB", stresses_II_.get_sigma_b()[i],
										"State",  stresses_II_.get_state_sigma_b()[i]);
	}
	PRINT(logfile,"");
	for (int i=0; i < stresses_II_.get_sigma_b().size(); ++i)
	{

		DUMP_PAIR_FLOAT_STRING(logfile, "SigmaS", stresses_II_.get_sigma_s()[i],
										"State",  stresses_II_.get_state_sigma_s()[i]);
	}
	PRINT(logfile,"-------------------------------------------------------------");
	PRINT(logfile,"-------------���������� ���������------------------------") ;
	PRINT(logfile,"-------------------------------------------------------------")
	for (int i=0; i < stresses_total_.get_sigma_b().size(); ++i)
	{

		DUMP_PAIR_FLOAT_STRING(logfile, "SigmaB", stresses_total_.get_sigma_b()[i],
										"State",  stresses_total_.get_state_sigma_b()[i]);
	}
	PRINT(logfile,"");
	for (int i=0; i < stresses_total_.get_sigma_b().size(); ++i)
	{

		DUMP_PAIR_FLOAT_STRING(logfile, "SigmaS", stresses_total_.get_sigma_s()[i],
										"State",  stresses_total_.get_state_sigma_s()[i]);
	}

}

//---------------------------------------------------------------------------
//������ ����������������� � log
//---------------------------------------------------------------------------
void TCompositeBeam::print_comp_sect_to_log()
{
	PRINT(logfile,"------------------------------------------");
	PRINT(logfile,"----------����������� �������-------------") ;
	PRINT(logfile,"------------------------------------------") ;
	DUMP(logfile,"����������� ���������� ��������",composite_section_.Alfa_s_);
	DUMP(logfile,"����������� ���������� ������",composite_section_.Alfa_b_);
	DUMP(logfile,"������ ������������ �������",composite_section_.H_stb_);
	DUMP(logfile,"���������� ����� �������� ������� ����� � �������� �����",composite_section_.Z_b_st_);
	DUMP(logfile,"���������� ����� ������� ������� ��������� � �������������������� �������",composite_section_.Z_st_red_);
	DUMP(logfile,"���������� ����� ������� ������� ��������� � �������������������� �������",composite_section_.Z_b_red_);
	DUMP(logfile,"���������� ����� ������� ������� �������������������� ������� � �������� ������ ������� �����",composite_section_.Z_red_f2_);
	DUMP(logfile,"���������� ����� ������� ������� �������������������� ������� � �������� ������ ������ �����",composite_section_.Z_red_f1_);
	DUMP(logfile,"������� �������������������� �������",composite_section_.A_red_);
	DUMP(logfile,"����������� ������ ������� �������������������� ������� ������������ �.�. ��������� �������",composite_section_.S_stb_);
	DUMP(logfile,"������ ������� �������������������� �������",composite_section_.I_red_);
	DUMP(logfile,"������ ������������� �������������������� ������� ��� ������� �����",composite_section_.W_f2_red_);
	DUMP(logfile,"������ ������������� �������������������� ������� ��� ������ �����",composite_section_.W_f1_red_);
	DUMP(logfile,"������ ������������� �������������������� ������� ��� �.�. �������������� �����",composite_section_.W_b_red_);
}



