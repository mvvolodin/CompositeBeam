//---------------------------------------------------------------------------

#pragma hdrstop

#include "ISection.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

	ISection::ISection()
		:profile_number_(""),
		b_uf_			(0.0),
		t_uf_			(0.0),
		b_lf_			(0.0),
		t_lf_			(0.0),
		h_w_			(0.0),
		t_w_			(0.0),
		h_st_			(0.0),
		r_				(0.0),
		weight_			(0.0),
		Z_f2_st_		(0.0),
		Z_f1_st_		(0.0),
		A_st_			(0.0),
		I_st_			(0.0),
		Wf2_st_			(0.0),
		Wf1_st_			(0.0){};

	ISection::ISection(TISectionInitialData i_section_initial_data)
	{
	 profile_number_=i_section_initial_data.profile_number_;
	 b_uf_= i_section_initial_data.b_uf_;
	 t_uf_= i_section_initial_data.t_uf_;
	 b_lf_= i_section_initial_data.b_lf_;
	 t_lf_= i_section_initial_data.t_lf_;
	 h_w_= i_section_initial_data.h_w_;
	 t_w_= i_section_initial_data.t_w_;
	 A_st_=i_section_initial_data.Ast;
	 I_st_=i_section_initial_data.Ist;
	 h_st_=i_section_initial_data.Hsect;
	 r_= i_section_initial_data.r_;
	 weight_=i_section_initial_data.weight_;
	 Z_f2_st_=i_section_initial_data.Z_f2_st;
	 Z_f1_st_=i_section_initial_data.Z_f1_st;
	 Wf2_st_=i_section_initial_data.Wf2_st;
	 Wf1_st_=i_section_initial_data.Wf2_st;


	}




