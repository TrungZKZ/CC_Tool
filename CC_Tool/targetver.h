
#pragma once

// The following macro defines the minimum required platform. The minimum required platform is
// The minimum version of Windows, Internet Explorer, etc. required to run the application.
// This macro enables all functionality that can be performed on the available platform versions.
// You can also specify the platform version.

// If you need to specify the target platform before the definition specified below, change the definition below.
// See MSDN for the latest information on the corresponding values for different platforms.
#ifndef WINVER                          // Windows Vista is specified as the minimum required platform.
#define WINVER 0x0600           // Change this to the appropriate value for other versions of Windows.
#endif

#ifndef _WIN32_WINNT            // Windows Vista is specified as the minimum required platform.
#define _WIN32_WINNT 0x0600     // Change this to the appropriate value for other versions of Windows.
#endif

#ifndef _WIN32_WINDOWS          // Windows 98 is specified as the minimum required platform.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value for Windows Me or later versions.
#endif

#ifndef _WIN32_IE                       // Internet Explorer 7.0 is specified as the minimum required platform.
#define _WIN32_IE 0x0700        //Change this to the appropriate value for other versions of IE.
#endif

