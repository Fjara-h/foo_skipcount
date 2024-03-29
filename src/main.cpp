#include "stdafx.h"
#include "globals.h"

#define FOOBAR2000_TARGET_VERSION 80

// Declaration of component's version information
// Since foobar2000 v1.0 having at least one of these is mandatory to let the troubleshooter tell different versions of the component apart.
// Note that it is possible to declare multiple components within one DLL, but it's strongly recommended to keep only one declaration per DLL.
// As for 1.1, the version numbers are used by the component update finder to find updates; for that to work, there can be ONLY ONE declaration per DLL.
// If there are multiple declarations, the component is assumed to be outdated and a version number of "0" is assumed, to overwrite the component with whatever is currently on the site assuming that it comes with proper version numbers.
DECLARE_COMPONENT_VERSION("Skip count", COMPONENT_VERSION, "Track skip statistics for every file");

// This will prevent users from renaming your component around (important for proper troubleshooter behaviors) or loading multiple instances of it.
VALIDATE_COMPONENT_FILENAME("foo_skipcount.dll");

// https://hydrogenaud.io/index.php/topic,124741.0.html
// https://hydrogenaud.io/index.php/topic,124742.0.html

// GUID for non-pref dialogs?
// How can I make save from dialogs update the preferences page when both are open?
// Should XML export and tag write dialogs be modeless? How?
// Should anything run async/threaded_process? How? (like foo_dop, search load_info_async)