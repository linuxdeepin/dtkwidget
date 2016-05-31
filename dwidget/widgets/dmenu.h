#pragma once

#ifdef Q_OS_LINUX
#include "platforms/linux/ddeepinmenu.h"
#else
#include "platforms/windows/dwindowsmenu.h"
#endif
