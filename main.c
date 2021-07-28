#include <Events.h>
#include <Dialogs.h>
#include <ToolUtils.h>
#include <Sound.h>

void Initialize();
void MainLoop();
void Terminate();


// constants
const short defaultMenubar = 128;
const short menuitemAbout = 1;
const short menuitemQuit = 1;
const short defaultWindow = 128;

enum
{
    kMenuApple = 128,
    kMenuFile,
    kMenuEdit
};
enum
{
    kItemAbout 	= 1,

    kItemQuit 	= 7,
};

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
	AppendResMenu(GetMenu(kMenuApple), 'DRVR');
	myMenu=GetMenuHandle(kMenuFile);
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

void ShowAboutBox()
{
    WindowRef w = GetNewWindow(128, NULL, (WindowPtr) - 1);
    MoveWindow(w,
        qd.screenBits.bounds.right/2 - w->portRect.right/2,
        qd.screenBits.bounds.bottom/2 - w->portRect.bottom/2,
        false);
    ShowWindow(w);
    SetPort(w);

    Handle h = GetResource('TEXT', 128);
    HLock(h);
    Rect r = w->portRect;
    InsetRect(&r, 10,10);
    TETextBox(*h, GetHandleSize(h), &r, teJustLeft);

    ReleaseResource(h);
    while(!Button())
        ;
    while(Button())
        ;
    FlushEvents(everyEvent, 0);
    DisposeWindow(w);
}

void DoMenuCommand(long menuCommand)
{
	Str255 str;
	WindowRef w;
	short menuID = menuCommand >> 16;
	short menuItem = menuCommand & 0xFFFF;
	if(menuID == kMenuApple)
	{
		if(menuItem == kItemAbout)
			ShowAboutBox();
		else
		{
			GetMenuItemText(GetMenu(128), menuItem, str);
			OpenDeskAcc(str);
		}
	}
	else if(menuID == kMenuFile)
	{
		switch(menuItem)
		{
		case kItemQuit:
			ExitToShell();
			break;
		}
	}

	HiliteMenu(0);
}

void TestSound()
{
	Handle myHandle;
	Ptr myPtr;
	SWSynthPtr mySWPtr;

	myHandle = NewHandle(120);
	HLock(myHandle);
	myPtr = *myHandle;
	mySWPtr = (SWSynthPtr)myPtr;
	mySWPtr->mode = swMode;
	mySWPtr->triplets[0].tcount = 1000;
	mySWPtr->triplets[0].amplitude = 100;
	mySWPtr->triplets[0].tduration = 100;
	mySWPtr->triplets[1].tcount = 9000;
	mySWPtr->triplets[1].amplitude = 100;
	mySWPtr->triplets[1].tduration = 100;
	mySWPtr->triplets[2].tcount = 8000;
	mySWPtr->triplets[2].amplitude = 100;
	mySWPtr->triplets[2].tduration = 100;

	StopSound();
	StartSound(myPtr, (LONGINT)3, (ProcPtr)-1);

}

void MainLoop()
{
	EventRecord 	event;
	WindowRef 	win;

	TestSound();

	while (true)
	{
		if(GetNextEvent(everyEvent, &event))
		{
			switch(event.what)
			{
			case keyDown:
			{
				Terminate();
			}
			break;

			case mouseDown:
				switch(FindWindow(event.where, &win))
				{
				case inGoAway:
					if(TrackGoAway(win, event.where))
						DisposeWindow(win);
					break;
				case inDrag:
					DragWindow(win, event.where, &qd.screenBits.bounds);
					break;
				case inMenuBar:
					DoMenuCommand( MenuSelect(event.where) );
					break;
				case inContent:
					SelectWindow(win);
					break;
				case inSysWindow:
					SystemClick(&event, win);
					break;
				}
				break;
			}
		}
	}
}

void Terminate() {
  ExitToShell();
}
