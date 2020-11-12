//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("uFrmHelp.cpp", HelpForm);
USEFORM("uFrmLogger.cpp", FormLogger);
USEFORM("uFrmAddImacts.cpp", FrmAddImpacts);
USEFORM("uFrmSteelSection.cpp", SteelSectionForm);
USEFORM("uFrmAboutProg.cpp", AboutProgForm);
USEFORM("uFrmSteel.cpp", DefineSteelForm);
USEFORM("uFrmConcrete.cpp", ConcreteDefinitionForm);
USEFORM("uFrmCompositeBeam.cpp", CompositeBeamMainForm);
USEFORM("uFrmRebar.cpp", RebarDefinitionForm);
USEFORM("uFrmStud.cpp", StudDefinitionForm);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TCompositeBeamMainForm), &CompositeBeamMainForm);
		Application->CreateForm(__classid(TConcreteDefinitionForm), &ConcreteDefinitionForm);
		Application->CreateForm(__classid(TRebarDefinitionForm), &RebarDefinitionForm);
		Application->CreateForm(__classid(TStudDefinitionForm), &StudDefinitionForm);
		Application->CreateForm(__classid(TFormLogger), &FormLogger);
		Application->CreateForm(__classid(TAboutProgForm), &AboutProgForm);
		Application->CreateForm(__classid(TDefineSteelForm), &DefineSteelForm);
		Application->CreateForm(__classid(TFrmAddImpacts), &FrmAddImpacts);
		Application->CreateForm(__classid(THelpForm), &HelpForm);
		Application->CreateForm(__classid(TSteelSectionForm), &SteelSectionForm);
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
