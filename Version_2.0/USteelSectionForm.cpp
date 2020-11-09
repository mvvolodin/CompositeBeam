//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <fstream>
#include "uSteelSectionForm.h"
#include "String_doubleUnit.h"
#include "uFrmLogger.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

bool flag_image = true;

TSteelSectionForm *SteelSectionForm;
//---------------------------------------------------------------------------
__fastcall TSteelSectionForm::TSteelSectionForm(TComponent* Owner)
	: TForm(Owner),
	weld_sect_temp_(WeldedSection(300,24,200,12,1200,8)),
	rolled_sect_temp_(RolledSection (std::wstring(L"35�2"),
									 175., 11.,
									 175., 11.,
									 328., 7.,
									 175., 6314.,135590100.))
{
	StringGrid_B->Cells[0][0]="h (��)";
	StringGrid_B->Cells[0][1]="bf (��)";
	StringGrid_B->Cells[0][2]="tw (��)";
	StringGrid_B->Cells[0][3]="tf (��)";
	StringGrid_B->Cells[0][4]="A (c�2)";
	StringGrid_B->Cells[0][5]="Iyy (c�4)";
	StringGrid_B->Cells[0][6]="iy (��)";
}
void __fastcall TSteelSectionForm::FormShow(TObject *Sender)
{
	set_form_controls();

	update_cntrls_state();
}
//---------------------------------------------------------------------------
//������������ �������� ��������� ���������� �� ��������� ������� ���� ISection
//---------------------------------------------------------------------------
void TSteelSectionForm::set_form_controls(ISection i_section)
{
	i_section_temp_ = i_section;
	set_form_controls();
	iobserver_ -> update(this);
}
//---------------------------------------------------------------------------
//������������ �������� ��������� ���������� �� ���� ������ ���� ISection
//---------------------------------------------------------------------------
void TSteelSectionForm::set_form_controls()
{
	rd_grp_rolled_sect_typeClick(nullptr);
	update_weld_sect_ctrls();
}
//---------------------------------------------------------------------------
//��������� �������� ��������� ���������� ����� �� ���� ������ ���� WeldedSection
//---------------------------------------------------------------------------
void TSteelSectionForm::update_weld_sect_ctrls()
{
	double b_f2 = weld_sect_temp_.b_f2();
	double t_f2 = weld_sect_temp_.t_f2();
	double b_f1 = weld_sect_temp_.b_f1();
	double t_f1 = weld_sect_temp_.t_f1();
	double h_w = weld_sect_temp_.h_w();
	double t_w = weld_sect_temp_.t_w();

	edt_b_f2 -> Text = b_f2;
	edt_t_f2 -> Text = t_f2;
	edt_b_f1 -> Text = b_f1;
	edt_t_f1 -> Text = t_f1;
	edt_h_w -> Text = h_w;
	edt_t_w -> Text = t_w;

	SECT_DVUTAVR weld_sect = {t_w, h_w, b_f2, t_f2, b_f1, t_f1};

	Draw_dvutavr_weld_plane(img_weld_sect, &weld_sect);
}
//---------------------------------------------------------------------------
//��������� ������ ��������� ������� ���������� ��������� ���������� �����
//---------------------------------------------------------------------------
void TSteelSectionForm::update_steel_section()
{
	if(pg_cntrl_sect_type -> ActivePage == tb_sheet_welded_profile)
	{

		WeldedSection weld_sect {cntrls_state_.edt_b_f1_, cntrls_state_.edt_t_f1_,
								 cntrls_state_.edt_b_f2_, cntrls_state_.edt_t_f2_,
								 cntrls_state_.edt_h_w_, cntrls_state_.edt_t_w_};
	}
	else
		RolledSection rolled_sect{cntrls_state_.rd_grp_rolled_sect_type_,
			 cntrls_state_.cmb_bx_rolled_sect_num_};



}
GeneralSteelSection const & TSteelSectionForm::get_section()
{
	update_steel_section();

	if(pg_cntrl_sect_type -> ActivePage == tb_sheet_welded_profile)
		return weld_sect_temp_;

	return rolled_sect_temp_;

}

void TSteelSectionForm::set_i_section()
{
//��������� �� �������� ���������� ������ ������ ��������
	int profile_group_index = rd_grp_rolled_sect_type -> ItemIndex + typeGOST_G57837_B;
//���������� ������ ������ �������� �� ������� ������ ��������
	TStandartProfil StandartProfil;
	StandartProfil.SetProfil(profile_group_index);

//�������� ������ ��� �������� �� ������� ������ ��������
	int n_profil;
	AnsiString *NameProfil;
	NameProfil = StandartProfil.GetVectorNameProfil(&n_profil);

//��������� �� �������� ���������� ������ �������
	int profile_number_index = cmb_bx_rolled_sect_num -> ItemIndex;
//��������� ������ ������� �� ������� �������
	double * ParamProfil;
	ParamProfil = StandartProfil.GetVectorParamProfil(profile_number_index);
//������ � ����������� ���� ������ ������ ISection;

	i_section_temp_ = ISection( NameProfil[profile_number_index],
								static_cast<ProfileGroup>(rd_grp_rolled_sect_type -> ItemIndex),
								ParamProfil[parBSECT],
								ParamProfil[parTF],
								ParamProfil[parBSECT],
								ParamProfil[parTF],
								ParamProfil[parHSECT] - 2 * ParamProfil[parTF],
								ParamProfil[parTW],
								ParamProfil[parHSECT],
								ParamProfil[parRAD],
								ParamProfil[parWEIGHT],
								ParamProfil[parHSECT] / 2.,
								ParamProfil[parHSECT] / 2.,
								ParamProfil[parAREA],
								ParamProfil[parIZZ],
								ParamProfil[parWZ],
								ParamProfil[parWZ]);
}

//----------------------------------------------------------------------
// ��������� �������� �������� � ��������
//----------------------------------------------------------------------
void TSteelSectionForm::Draw_dvutavr_weld_plane(TImage * Image_stand, SECT_DVUTAVR *sect) {
  TPoint vertices1[30];
  TPoint vertices2[30];
  TPoint vert_rect[30];
  int zero, zero1, zero2;
  double zero_f, zero1_f, zero2_f;
  double scale_1, scale;
  int rc;


	double * ParamProfil;
	int shiftY = 40;
	int shiftX = 80;

	TRect NewRect = Rect(0, 0, Image_stand->Width,Image_stand->Height);
	Image_stand->Canvas->Brush->Color = clWhite;
	Image_stand->Canvas->FillRect(NewRect);
	//Image_stand->Canvas->Rectangle(0, 0, Image_stand->Width,Image_stand->Height);

	scale=(Image_stand->Width-20)/(1e0*sect->b2);
	scale_1=MIN(scale,(Image_stand->Width - 20)/(1e0*sect->b1));
	scale=MIN(scale_1,(Image_stand->Height- 20)/(1e0*(sect->h + sect->h1 + sect->h2)));

	zero_f = (Image_stand->Height - (sect->h + sect->h1 + sect->h2)*scale)/2;
	zero1_f=(Image_stand->Width - sect->b2*scale)/2;
	zero2_f=(Image_stand->Width - sect->b1*scale)/2;

	zero = zero_f;
	zero1 = zero1_f;
	zero2 = zero2_f;
	//  ����� ��������

	Point_weld_dvutavr(zero, zero1, zero2, sect, scale, vertices1); // �������� ����� ������� ��������

	Image_stand->Canvas->Brush->Color = clMedGray;
	//----------------------------------------------------------------

	//--------------------------------------------
	// ��������� ��������
	Image_stand->Canvas->Brush->Color = clScrollBar;
	Image_stand->Canvas->Polygon(vertices1, 12);

	Image_stand->Canvas->Brush->Style=bsClear;

	//Draw_axes_zero(Image_stand, vPLANE, (vertices1[0].x + vertices1[1].x)/2, (vertices1[0].y + vertices1[7].y)/2);
	draw_axes_zero(Image_stand, (vertices1[0].x + vertices1[1].x)/2, (vertices1[0].y + vertices1[7].y)/2);

	TPoint Point0_r;
    TPoint Point1_r;

	flag_image = true;
	Image_stand->Canvas->Brush->Style=bsClear;
	// ������ �����
	Point0_r = vertices1[1];
	Point1_r = vertices1[2];

    pCanvas_Dim_vt(Image_stand, Point0_r, Point1_r, orHORIZ, sideUP, "bf",
				 13, 0, 0, 0);
    // ������ �������
	Point0_r = vertices1[10];
	Point1_r = vertices1[0];

	pCanvas_Dim_vt(Image_stand, Point0_r, Point1_r, orVERT, sideLEFT, "h",
				 6, 0, 4, 0);
	// ������� ����� �������
	Point0_r = vertices1[1];
	Point1_r = vertices1[2];

	int posY = vertices1[2].y - vertices1[1].y;
    pCanvas_Dim_vt(Image_stand, Point0_r, Point1_r, orVERT, sideRIGHT, "tf",
                 6, 0, -4, -posY/2 - 4);
    // ������� ������
	Point0_r = vertices1[15];
	Point1_r = vertices1[6];

	pCanvas_Dim_vt(Image_stand, Point0_r, Point1_r, orHORIZ, sideUP, "tw",
				 30, 0, 13, 0);
}
// �������� ����� ������� �������� ��������
void  TSteelSectionForm::Point_weld_dvutavr(int zero, int zero1, int zero2, SECT_DVUTAVR *sect, float scale, TPoint *vertices) {
  double b1c = sect->b1*scale;
  double b1p = 0.5*(sect->b1 + sect->b)*scale;
  double b1m = 0.5*(sect->b1 - sect->b)*scale;
  double b2c = sect->b2*scale;
  double b2p = 0.5*(sect->b2 + sect->b)*scale;
  double b2m = 0.5*(sect->b2 - sect->b)*scale;
  double h1c = sect->h1*scale;
  double h_h1 = (sect->h + sect->h1)*scale;
  double h_2h1 = (sect->h + sect->h1 + sect->h2)*scale;
  double h2c = sect->h2*scale;
  double h_h2 = (sect->h + sect->h2)*scale;
  double h_2h2 = (sect->h + 2*sect->h2)*scale;

  vertices[0] = Point(zero2, zero);
  vertices[1] = Point(zero2 + b1c, zero);
  vertices[2] = Point(zero2 + b1c, zero + h1c);
  vertices[3] = Point(zero2 + b1p, zero + h1c);
  vertices[4] = Point(zero2 + b1p, zero + h_h1);
  vertices[5] = Point(zero1 + b2c, zero + h_h1);
  vertices[6] = Point(zero1 + b2c, zero + h_2h1);
  vertices[7] = Point(zero1, zero + h_2h1);
  vertices[8] = Point(zero1, zero + h_h1);
  vertices[9] = Point(zero2 + b1m, zero + h_h1);
  vertices[10] = Point(zero2 + b1m, zero + h1c);
  vertices[11] = Point(zero2, zero + h1c);
  vertices[12] = Point(zero2, zero);
}

//----------------------------------------------------------------------
// ��������� ��������
//----------------------------------------------------------------------
void  TSteelSectionForm::draw_dvutavr(TImage * Image_stand, SECT_DVUTAVR *sect)
{
  TPoint vertices[30];
  int zero, zero1, zero2;
  float scale_1, scale;
  int rc; //return code � ������� �� �����������!!! �


  if (sect->n_group < 0) {
	TRect NewRect = Rect(0, 0, Image_stand->Width,Image_stand->Height);
	Image_stand->Canvas->Brush->Color = clWhite;
	Image_stand->Canvas->FillRect(NewRect);
	Image_stand->Canvas->Rectangle(0, 0, Image_stand->Width,Image_stand->Height);

    scale=(Image_stand->Width-20)/(1e0*sect->b2);
	scale_1=MIN(scale,(Image_stand->Width-20)/(1e0*sect->b1));
	scale=MIN(scale_1,(Image_stand->Height-20)/(1e0*(sect->h + sect->h1 + sect->h2)));

    zero=(Image_stand->Height - (sect->h + sect->h1 + sect->h2)*scale)/2;
	zero1=(Image_stand->Width - sect->b2*scale)/2;
	zero2=(Image_stand->Width - sect->b1*scale)/2;

	Point_dvutavr(zero, zero1, zero2, sect, scale, vertices); // �������� ����� ������� ��������

	Image_stand->Canvas->Brush->Color = clMedGray;
	Image_stand->Canvas->Polygon(vertices, 12);
    Image_stand->Canvas->Brush->Color = clWhite;
	draw_axes(Image_stand);
  }
  else {
	double * ParamProfil;

    TRect NewRect = Rect(0, 0, Image_stand->Width,Image_stand->Height);
	Image_stand->Canvas->Brush->Color = clWhite;
    Image_stand->Canvas->FillRect(NewRect);
	Image_stand->Canvas->Rectangle(0, 0, Image_stand->Width,Image_stand->Height);

	scale=(Image_stand->Width-35)/(1e0*sect->b2);
    scale_1=MIN(scale,(Image_stand->Width-35)/(1e0*sect->b1));
    scale=MIN(scale_1,(Image_stand->Height-30)/(1e0*(sect->h + sect->h1 + sect->h2)));

    zero=(Image_stand->Height - (sect->h + sect->h1 + sect->h2)*scale)/2 + 10;
	zero1=(Image_stand->Width - sect->b2*scale)/2;
    zero2=(Image_stand->Width - sect->b1*scale)/2;
	//StandartProfil.SetProfil(sect->n_group);
	//ParamProfil = StandartProfil.GetVectorParamProfil(sect->n_profil);

	Point_stand_dvutavr(zero, zero1, zero2, sect, /*ParamProfil[parRAD]*/sect->r, scale, vertices); // �������� ����� ������� ��������

	Image_stand->Canvas->Brush->Color = clMedGray;
	Image_stand->Canvas->Polygon(vertices, 20);
    Image_stand->Canvas->Brush->Color = clWhite;

	draw_axes_zero(Image_stand, (vertices[0].x + vertices[1].x)/2, (vertices[0].y + vertices[11].y)/2);

	TPoint Point0_r;
    TPoint Point1_r;

	flag_image = true;
	Image_stand->Canvas->Brush->Style=bsClear;
	// ������ �����
    Point0_r = vertices[0];
	Point1_r = vertices[1];

    pCanvas_Dim_vt(Image_stand, Point0_r, Point1_r, orHORIZ, sideUP, "bf",
				 13, 0, 0, 0);
    // ������ �������
	Point0_r = vertices[11];
    Point1_r = vertices[0];

	pCanvas_Dim_vt(Image_stand, Point0_r, Point1_r, orVERT, sideLEFT, "h",
                 6, 0, 4, 0);
    // ������� ����� �������
    Point0_r = vertices[1];
    Point1_r = vertices[2];

    int posY = vertices[2].y - vertices[1].y;
    pCanvas_Dim_vt(Image_stand, Point0_r, Point1_r, orVERT, sideRIGHT, "tf",
                 6, 0, -4, -posY/2 - 4);
    // ������� ������
	Point0_r = vertices[15];
    Point1_r = vertices[6];

	pCanvas_Dim_vt(Image_stand, Point0_r, Point1_r, orHORIZ, sideUP, "tw",
				 30, 0, 13, 0);

  }
}

//---------------------------------------------------------------------------
// ��������� ������� ���������
void   TSteelSectionForm::draw_axes(TImage *Image_Ax) {

  draw_axes_zero(Image_Ax, Image_Ax->Width/2, Image_Ax->Height/2);
}
//---------------------------------------------------------------------------
// ��������� ������� ���������
void   TSteelSectionForm::draw_axes_zero(TImage *Image_Ax, int x0, int y0) {
  TPoint points[10];
  //int zero, zero1,zero2;
  //float scale_1, scale;


  points[0]=Point(x0, y0);
  points[1]=Point(x0, y0 - 40);

  CoorAx.PutArrow(points, 0, MAX_PIC);
  CoorAx.SetArrow();

  Image_Ax->Canvas->Pen->Width=1;
  Image_Ax->Canvas->Polyline(CoorAx.LArrow, 2);
  Image_Ax->Canvas->Pen->Width=1;
  Image_Ax->Canvas->Brush->Color=clBlack;
  Image_Ax->Canvas->Polygon(CoorAx.Ar, 3);
  Image_Ax->Canvas->Brush->Color=clWhite;

  points[0]=Point(x0, y0);
  points[1]=Point(x0 + 40, y0);

  CoorAx.PutArrow(points, 0, MAX_PIC);
  CoorAx.SetArrow();

  Image_Ax->Canvas->Pen->Width=1;
  Image_Ax->Canvas->Polyline(CoorAx.LArrow, 2);
  Image_Ax->Canvas->Pen->Width=1;
  Image_Ax->Canvas->Brush->Color=clBlack;
  Image_Ax->Canvas->Polygon(CoorAx.Ar, 3);
  Image_Ax->Canvas->Brush->Color=clWhite;
  Image_Ax->Canvas->Ellipse(x0-4, y0-4, x0+4, y0+4);
  Image_Ax->Canvas->Brush->Style=bsClear;
  Image_Ax->Canvas->TextOut(x0+6, y0 - 40,"Y");
  Image_Ax->Canvas->TextOut(x0 + 40, y0-16,"X");
  Image_Ax->Canvas->Brush->Style=bsSolid;
}
//---------------------------------------------------------------------------
// �������� ����� ������� ���������� ��������
//---------------------------------------------------------------------------
void  TSteelSectionForm::Point_stand_dvutavr(int zero, int zero1, int zero2, SECT_DVUTAVR *sect, double rad, float scale, TPoint *vertices)
{
  double r_c = rad*scale;
  double r2_c = Round(r_c - r_c*cos(45/57.3), 0);
  double b1c = sect->b1*scale;
  double b1p = 0.5*(sect->b1 + sect->b)*scale;
  double b1m = 0.5*(sect->b1 - sect->b)*scale;
  double h1c = sect->h1*scale;
  double h_h1 = (sect->h + sect->h1)*scale;
  double h_2h1 = (sect->h + 2*sect->h1)*scale;

  vertices[0] = Point(zero2, zero);
  vertices[1] = Point(zero2 + b1c, zero);
  vertices[2] = Point(zero2 + b1c, zero + h1c);
  vertices[3] = Point(zero2 + b1p + r_c, zero + h1c);
  vertices[4] = Point(zero2 + b1p + r2_c, zero + h1c + r2_c);
  vertices[5] = Point(zero2 + b1p, zero + h1c + r_c);
  vertices[6] = Point(zero2 + b1p, zero + h_h1 - r_c);
  vertices[7] = Point(zero1 + b1p + r2_c, zero + h_h1 - r2_c);
  vertices[8] = Point(zero1 + b1p + r_c, zero + h_h1);
  vertices[9] = Point(zero1 + b1c, zero + h_h1);
  vertices[10] = Point(zero1 + b1c, zero + h_2h1);
  vertices[11] = Point(zero1, zero + h_2h1);
  vertices[12] = Point(zero1, zero + h_h1);
  vertices[13] = Point(zero2 + b1m - r_c, zero + h_h1);
  vertices[14] = Point(zero2 + b1m - r2_c, zero + h_h1 - r2_c);
  vertices[15] = Point(zero2 + b1m , zero + h_h1 - r_c);
  vertices[16] = Point(zero2 + b1m, zero + h1c + r_c);
  vertices[17] = Point(zero2 + b1m - r2_c, zero + h1c + r2_c);
  vertices[18] = Point(zero2 + b1m - r_c, zero + h1c);
  vertices[19] = Point(zero2, zero + h1c);
  vertices[20] = Point(zero2, zero);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TSteelSectionForm::cmb_bx_rolled_sect_numChange(TObject *Sender)
{
//��������� ������ ������� �� ������� �������
	int profile_number_index = 0;
	double * ParamProfil;
	if(Sender)
		profile_number_index = cmb_bx_rolled_sect_num -> ItemIndex;
	else
		{
			profile_number_index = cmb_bx_rolled_sect_num -> Items -> IndexOf(i_section_temp_.get_profile_number());
			cmb_bx_rolled_sect_num -> ItemIndex = profile_number_index;
        }

		ParamProfil = StandartProfil_.GetVectorParamProfil(profile_number_index);
//���������� �������� ���������� ������� �������
	StringGrid_B -> Cells[1][0] = FloatToStr(ParamProfil[parHSECT]);
	StringGrid_B -> Cells[1][1] = FloatToStr(ParamProfil[parBF]);
	StringGrid_B -> Cells[1][2] = FloatToStr(ParamProfil[parTW]);
	StringGrid_B -> Cells[1][3] = FloatToStr(ParamProfil[parTF]);
	StringGrid_B -> Cells[1][4] = FloatToStrF(ParamProfil[parAREA]/100, ffFixed, 6, 0);
	StringGrid_B -> Cells[1][5] = FloatToStrF(ParamProfil[parIZZ]/10000, ffFixed, 6, 0);
	StringGrid_B -> Cells[1][6] = FloatToStrF(ParamProfil[parIRADZ], ffFixed, 6, 1);

//	�������� ������� ����  SECT_DVUTAVR ��� �������� � ������� ��������
	SECT_DVUTAVR dvutavr;
	dvutavr.b = ParamProfil[parTW];
	dvutavr.h = ParamProfil[parHSECT] - 2*ParamProfil[parTF];
	dvutavr.b1 = ParamProfil[parBF];
	dvutavr.b2 = ParamProfil[parBF];
	dvutavr.h1 = ParamProfil[parTF];
	dvutavr.h2 = ParamProfil[parTF];
	dvutavr.r = ParamProfil[parRAD];
	dvutavr.flag_concl=true; //��������� ��
	dvutavr.index_orient = 0; // ��������� ��

	draw_dvutavr(Image_stand, &dvutavr);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TSteelSectionForm::rd_grp_rolled_sect_typeClick(TObject *Sender)
{
//��������� �� �������� ���������� ������� ������ ��������
	int profile_group_index = 0;
	if(Sender)
		profile_group_index = rd_grp_rolled_sect_type -> ItemIndex + typeGOST_G57837_B;
	else
		profile_group_index = static_cast<int>(i_section_temp_.get_profile_group()) + typeGOST_G57837_B;
//���������� ������ ������ �������� �� ������� ������ ��������
	StandartProfil_.SetProfil(profile_group_index);

//�������� ������� ���� �������� � ��� �����
	AnsiString *NameProfil;
	int n_profil;

	NameProfil = StandartProfil_.GetVectorNameProfil(&n_profil);

//���������� �������� ���������� ������� ��������
	cmb_bx_rolled_sect_num -> Items -> Clear();

	for (int i=0; i < n_profil; i++)
		cmb_bx_rolled_sect_num -> Items -> Add(NameProfil[i]);

	cmb_bx_rolled_sect_num -> ItemIndex = 0;

	cmb_bx_rolled_sect_numChange(Sender);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void TSteelSectionForm::register_observer(IObserver_* iobserver)
{
	iobserver_ = iobserver;
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
String TSteelSectionForm::get_information()const
{
   return sect_name();
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
Publisher_ID TSteelSectionForm::get_id()const
{
   return id_;
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TSteelSectionForm::btk_okClick(TObject *Sender)
{
	write_cntrls_state();


	set_i_section();
	iobserver_ -> update(this);
	Close();
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TSteelSectionForm::btn_cancelClick(TObject *Sender)
{
	update_cntrls_state();

	set_form_controls();


}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TSteelSectionForm::btn_closeClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TSteelSectionForm::btn_launch_loggerClick(TObject *Sender)
{

	log_.reset(new TFormLogger(this));
	log_.get()-> Show();
	get_section().print_data_to_logger(*log_);

}

//---------------------------------------------------------------------------

void __fastcall TSteelSectionForm::btn_drawClick(TObject *Sender)
{
	weld_sect_temp_.draw(img_weld_sect->Canvas);

}
//---------------------------------------------------------------------------
void TSteelSectionForm::write_cntrls_state()
{
		int rc = 0;

		rc = String_double_plus(lbl_b_f2 -> Caption, edt_b_f2 -> Text, &cntrls_state_.edt_b_f2_);
		if(rc > 0)throw(rc);

		rc = String_double_plus(lbl_t_f2 -> Caption, edt_t_f2 -> Text, &cntrls_state_.edt_t_f2_);
		if(rc > 0)throw(rc);

		rc = String_double_plus(lbl_b_f1 -> Caption, edt_b_f1 -> Text, &cntrls_state_.edt_b_f1_);
		if(rc > 0)throw(rc);

		rc = String_double_plus(lbl_t_f1 -> Caption, edt_t_f1 -> Text, &cntrls_state_.edt_t_f1_);
		if(rc > 0)throw(rc);

		rc = String_double_plus(lbl_h_w -> Caption, edt_h_w -> Text, &cntrls_state_.edt_h_w_);
		if(rc > 0)throw(rc);

		rc = String_double_plus(lbl_t_w -> Caption, edt_t_w -> Text, &cntrls_state_.edt_t_w_);
		if(rc > 0)throw(rc);

		cntrls_state_.pg_cntrl_sect_type_ = pg_cntrl_sect_type -> ActivePageIndex;
		cntrls_state_.cmb_bx_rolled_sect_num_ = cmb_bx_rolled_sect_num -> ItemIndex;
		cntrls_state_.rd_grp_rolled_sect_type_ = rd_grp_rolled_sect_type -> ItemIndex;

}
void TSteelSectionForm::update_cntrls_state()
{
		edt_b_f2 -> Text = cntrls_state_.edt_b_f2_;
		edt_t_f2 -> Text = cntrls_state_.edt_t_f2_;
		edt_b_f1 -> Text = cntrls_state_.edt_b_f1_;
		edt_t_f1 -> Text = cntrls_state_.edt_t_f1_;
		edt_h_w -> Text = cntrls_state_.edt_h_w_;
		edt_t_w -> Text = cntrls_state_.edt_t_w_;

		pg_cntrl_sect_type -> ActivePageIndex = cntrls_state_.pg_cntrl_sect_type_;
		cmb_bx_rolled_sect_num -> ItemIndex = cntrls_state_.cmb_bx_rolled_sect_num_;
		rd_grp_rolled_sect_type -> ItemIndex = cntrls_state_.rd_grp_rolled_sect_type_;

}

void __fastcall TSteelSectionForm::btn_saveClick(TObject *Sender)
{
	write_cntrls_state();
	std::ofstream ofstr {"test.cb"};
	cntrls_state_.save_cntls_state(ofstr);
	ofstr.close();
}
//---------------------------------------------------------------------------

void __fastcall TSteelSectionForm::btn_loadClick(TObject *Sender)
{

	std::ifstream ifstr {"test.cb"};
	ifstr.seekg(0);
	cntrls_state_.load_cntrls_state(ifstr);
	update_cntrls_state();
}

String TSteelSectionForm::sect_name()const
{
	if(pg_cntrl_sect_type -> ActivePage == tb_sheet_welded_profile)
	{
		double h_sect = cntrls_state_.edt_t_f1_ + cntrls_state_.edt_h_w_ + cntrls_state_.edt_t_f2_;
		double max_width_fl = (cntrls_state_.edt_b_f2_ >= cntrls_state_.edt_b_f1_) ?
			cntrls_state_.edt_b_f2_:cntrls_state_.edt_b_f1_;

		return L"��. " + FloatToStrF(max_width_fl, ffFixed, 15, 0) +
			L"x" + FloatToStrF(h_sect, ffFixed, 15, 0);
	}
	else
		return cmb_bx_rolled_sect_num -> Items -> Strings[cmb_bx_rolled_sect_num -> ItemIndex];

}




//---------------------------------------------------------------------------

