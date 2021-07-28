/*
	Copyright 2017 Wolfgang Thaller.

	This file is part of Retro68.

	Retro68 is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Retro68 is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Retro68.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Menus.r"
#include "MacTypes.r"

resource 'MENU' (128) {
    128, textMenuProc;
    allEnabled, enabled;
    apple;
    {
        "About Daniels program...", noIcon, noKey, noMark, plain;
        "-", noIcon, noKey, noMark, plain;
    }
};

resource 'MENU' (129) {
    129, textMenuProc;
    allEnabled, enabled;
    "File";
    {
        "New Document Window", noIcon, "N", noMark, plain;
        "New Rounded Window", noIcon, "M", noMark, plain;
        "New Custom Window (10-byte stub)", noIcon, "1", noMark, plain;
        "New Custom Window (code resource)", noIcon, "2", noMark, plain;
        "Close", noIcon, "W", noMark, plain;
        "-", noIcon, noKey, noMark, plain;
        "Quit", noIcon, "Q", noMark, plain;
    }
};

resource 'MENU' (130) {
    130, textMenuProc;
    0, enabled;
    "Edit";
    {
        "Undo", noIcon, "Z", noMark, plain;
        "-", noIcon, noKey, noMark, plain;
        "Cut", noIcon, "X", noMark, plain;
        "Copy", noIcon, "C", noMark, plain;
        "Paste", noIcon, "V", noMark, plain;
        "Clear", noIcon, noKey, noMark, plain;
    }
};

resource 'MBAR' (128) {
    { 128, 129, 130 };
};

data 'TEXT' (128) {
    "About Daniels program\r\r"
    "This is a late nostalgic trip back to the days when the only programming possible "
    "to do was on a Mac SE with black and white screen with 512x342 pixels and 22 kHz sound "
    "sampling rate. This is the first time I really succeed in using resources.\r"
    "Many thanks to the fine people creating the original Mac and as well as this "
    "Retro68 that makes this possible."
};

resource 'WIND' (128) {
    {0, 0, 220, 320}, altDBoxProc;
    invisible;
    noGoAway;
    0, "";
    noAutoCenter;
};