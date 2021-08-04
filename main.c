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

/** @brief Test capabilities
 * From http://mirror.informatimago.com/next/developer.apple.com/documentation/mac/Sound/Sound-53.html#MARKER-9-235
 *
 */
static bool MyCanPlayMultiChannels()
{
	long myResponse;
	bool myResult=false;
	OSErr myErr;
	NumVersion myVersion;

	myVersion = SndSoundManagerVersion();
	myErr = Gestalt(gestaltSoundAttr, &myResponse);
//	if(myVersion.majorRevs >=3)
//	{
//		if((myErr == noErr)
//				&& (BitTst(myResponse, gestaltMultiChannels)))
//		{
//			myResult = true;
//		}
//	}
//	else
//	{
		myErr = Gestalt(gestaltHardwareAttr, &myResponse);
		if((myErr == noErr)
			&& (BitTst((Ptr)&myResponse, (long)gestaltHasASC)))
		{
			myResult = true;
		}
//	}
	return myResult;
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
	SndCommand myAmpCmd;
	SndCommand myTimbreCmd;

	amp = 0xFF000000;
	pitch = 60;

	myCmd.cmd = noteCmd;
	myCmd.param1 = 200;
	myCmd.param2 = amp + pitch;

	myAmpCmd.cmd = ampCmd;
	myAmpCmd.param1 = 0;
	myAmpCmd.param2 = 0;

	myTimbreCmd.cmd = timbreCmd;
	myTimbreCmd.param1 = 0;
	myTimbreCmd.param2 =0;

	*ppChannel = 0L;

	myErr = SndNewChannel(ppChannel, noteSynth, 0, 0L);
	if (myErr != noErr)
		ShowError(myErr);
	else
	{
		for(int i=0;i<30;i++)
		{
//			myAmpCmd.param1 = 255-(255/30*(30-i));
//			myErr = SndDoCommand(*ppChannel, &myAmpCmd, false);
//			if (myErr != noErr)
//				ShowError(myErr);

			myTimbreCmd.param1 = 255-(255/30*(30-i));
			myTimbreCmd.param1 = 0;
			myErr = SndDoCommand(*ppChannel, &myTimbreCmd, false);
			if (myErr != noErr)
				ShowError(myErr);

			pitch = 60+i;
			//amp = 0xFF000000 & (0xFF - i*8)<<24;
			amp = 0x00000000 & (0xFF - i*8)<<24;

			myCmd.param2 = amp + pitch;
			//		myErr = SndDoImmediate(myChannel, &myCmd);
			myErr = SndDoCommand(*ppChannel, &myCmd, false);
			if (myErr != noErr)
				ShowError(myErr);

		}
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
