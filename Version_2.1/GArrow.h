//---------------------------------------------------------------------------
#include <vcl.h>
#ifndef GArrowH
#define GArrowH
#define MAX_PIC  10
class TArrow
{
  public:
  int Ax0,Ay0; // ����� 0 �������
  int Ax1,Ay1; // ����� 1 �������
 // long double Ax0,Ay0; // ����� 0 �������   ��� classic Borland compiler ���������� ������ long double
  //long double Ax1,Ay1; // ����� 1 �������
  float Sign;
  int    MaxPic;  // ������������ ����� ������� �������
  TPoint LArrow[3];//����� ��� ��������� �������???
  TPoint Ar[4];
  void  PutArrow(TPoint * Ax_c, float Signature,
								int MaxPic0);   //�� ���� ��� ����������� ��� ��� ���������� ������������� �����
  void  SetArrow();
};

//---------------------------------------------------------------------------
#endif