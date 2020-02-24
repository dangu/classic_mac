#include <Events.h>
#include <Dialogs.h>
#include <ToolUtils.h>
#include <Sound.h>

void Initialize();
void MainLoop();
void Terminate();


// constants
const short defaultMenubar = 128;
const short menuApple = 128;
const short menuFile = 129;
const short menuitemAbout = 1;
const short menuitemQuit = 1;
const short defaultWindow = 128;

QDGlobals qd;

void main()
{
  Initialize();

  MainLoop();

  Terminate();
}

static void initAppl(void)
{
  Handle 	myMenuBar;
  MenuHandle	myMenu;
  Str255 name = "\x06TicTacToe";

  myMenuBar = GetNewMBar(defaultMenubar);
  SetMenuBar(myMenuBar);
  myMenu=GetMenuHandle(menuFile);
  AppendResMenu(myMenu, 'DRVR');
  AppendMenu(myMenu, name);
  DrawMenuBar();

//  WindowPtr window = GetNewWindow(defaultWindow, nil, (WindowPtr) -1);
//  SetWTitle(window, name);
}

void Initialize()
{
  MaxApplZone();
  MoreMasters();

  InitGraf(&qd.thePort);
  InitFonts();
  FlushEvents(everyEvent, 0);
  InitWindows();
  InitMenus();
  TEInit();
  InitDialogs(0L);
  InitCursor();

  initAppl();
}

void MainLoop()
{
  EventRecord event;

  while (true)
    {
      if (GetNextEvent(everyEvent, &event))
	{
	  if (event.what == keyDown)
	    {
	      Terminate();
	    }
	  if (event.what == mouseDown)
	    {
	      WindowPtr clickedWindow;
	      short clickedPart = FindWindow(event.where, &clickedWindow);

	      if (clickedPart == inMenuBar)
		{
		  SysBeep(1);
		}
	    }
	}
    }
}

void Terminate() {
  ExitToShell();
}
