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
    kItemAbout = 1,
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

    HiliteMenu(0);
}

void MainLoop()
{
	EventRecord 	event;
	WindowRef 	win;

	while (true)
	{
		switch(GetNextEvent(everyEvent, &event))
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

void Terminate() {
  ExitToShell();
}
