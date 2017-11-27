/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "osxwindow.h"

#include <Cocoa/Cocoa.h>

namespace OSX {

void HideWindowTitlebar(long winId)
{
    NSView *view = reinterpret_cast<NSView *>(winId);
    NSWindow* window = [view window];

    [window setStyleMask: [window styleMask] | NSFullSizeContentViewWindowMask | NSWindowTitleHidden];

    [window setTitlebarAppearsTransparent:YES];
    [window setMovableByWindowBackground:YES];

    [[window standardWindowButton:NSWindowCloseButton] setHidden:YES];
    [[window standardWindowButton:NSWindowMiniaturizeButton] setHidden:YES];
    [[window standardWindowButton:NSWindowZoomButton] setHidden:YES];

    window.titleVisibility = NSWindowTitleHidden;
}

}
