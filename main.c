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

bool showError;

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

bool DoMenuCommand(long menuCommand)
{
	Str255 str;
	WindowRef w;
	bool quitFlag=false;

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
			quitFlag = true;
//			ExitToShell();
			break;
		}
	}

	HiliteMenu(0);

	return quitFlag;
}

void ShowError(int err)
{
    Str255 errStr;
    NumToString( err, errStr );
	ParamText(errStr,errStr,errStr,errStr);


	DialogPtr dlg = GetNewDialog(128,0,(WindowPtr)-1);
	//Alert(128, NULL);

	showError = true;


}

/**@brief Test sound
 *
 * Info in Inside Macintosh V, p. 473 "Sound Manager"
 *
 * More info:
 * http://mirror.informatimago.com/next/developer.apple.com/documentation/mac/Sound/Sound-51.html
 */
void TestSound(SndChannelPtr *ppChannel)
{
	OSErr myErr;
	long amp, pitch;

	SndCommand myCmd;

	amp = 0xFF000000;
	pitch = 60;

	myCmd.cmd = noteCmd;
	myCmd.param1 = 1000;
	myCmd.param2 = amp + pitch;

	*ppChannel = 0L;

	myErr = SndNewChannel(ppChannel, noteSynth, 0, 0L);
	if (myErr != noErr)
		ShowError(myErr);
	else
	{
//		for(int i=0;i<10;i++)
//		{
//			pitch = 60+i;
//			myCmd.param2 = amp + pitch;
//			//		myErr = SndDoImmediate(myChannel, &myCmd);
			myErr = SndDoCommand(*ppChannel, &myCmd, false);
			if (myErr != noErr)
				ShowError(myErr);
//		}
	}


}

void TestSound_Res()
{
	Handle mySndHdl;
	Ptr myPtr;
	SndChannelPtr mySndChnPtr;

	OSErr retCode;




	mySndChnPtr = 0;
	mySndHdl = GetNamedResource('snd ', "\pJah");
    if (ResError() != noErr || mySndHdl == nil)
           Debugger();
	//ShowError((int)mySndHdl);
	//SndNewChannel(&mySndChnPtr, noteSynth, 0, 0);
	retCode = SndPlay(0, mySndHdl, 0);
	ShowError(retCode);
	ReleaseResource(mySndHdl);
	Debugger();

}

void MainLoop()
{
	EventRecord 	event;
	WindowRef 	win;
	OSErr 		myErr;
	bool quitFlag = false;

    short item;
    SndChannelPtr pChannel;

	TestSound(&pChannel);

	item = 0;
	do
	{
		if(showError)
		{
			ModalDialog(NULL, &item);
		}


		if(GetNextEvent(everyEvent, &event))
		{
			switch(event.what)
			{
			case keyDown:
			{
				quitFlag = true;
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
					quitFlag |= DoMenuCommand( MenuSelect(event.where) );
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
	} while((item != 1)
			&& (!quitFlag));

	myErr = SndDisposeChannel(pChannel, true);
	if (myErr != noErr)
		ShowError(myErr);


	//		Debugger();

	//		Debugger();
}

void Terminate() {
  ExitToShell();
}
