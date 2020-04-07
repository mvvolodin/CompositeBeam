//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("RebarDefinitionFormUnit.cpp", RebarDefinitionForm);
USEFORM("SteelSectionFormUnit.cpp", SteelSectionForm);
USEFORM("SteelSectionDefinitionFrameUnit.cpp", SteelSectionDefinitionFrame); /* TFrame: File Type */
USEFORM("StudDefinitionFormUnit.cpp", StudDefinitionForm);
USEFORM("ConcreteDefinitionFormUnit.cpp", ConcreteDefinitionForm);
USEFORM("AboutProg.cpp", AboutProgForm);
USEFORM("CompositeBeamMainFormUnit.cpp", CompositeBeamMainForm);
USEFORM("DefineSteelFormUnit.cpp", DefineSteelForm);
USEFORM("LoggerFormUnit.cpp", FormLogger);
USEFORM("USteelSectionForm.cpp", SteelSectionForm2);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TCompositeBeamMainForm), &CompositeBeamMainForm);
		Application->CreateForm(__classid(TSteelSectionForm), &SteelSectionForm);
		Application->CreateForm(__classid(TConcreteDefinitionForm), &ConcreteDefinitionForm);
		Application->CreateForm(__classid(TRebarDefinitionForm), &RebarDefinitionForm);
		Application->CreateForm(__classid(TStudDefinitionForm), &StudDefinitionForm);
		Application->CreateForm(__classid(TDefineSteelForm), &DefineSteelForm);
		Application->CreateForm(__classid(TFormLogger), &FormLogger);
		Application->CreateForm(__classid(TAboutProgForm), &AboutProgForm);
		Application->CreateForm(__classid(TSteelSectionForm2), &SteelSectionForm2);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------
