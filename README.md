# foo_skipcount

Track skip statistics for [foobar2000](https://www.foobar2000.org/), [official component page](https://www.foobar2000.org/components/view/foo_skipcount). Download from the official component page or this repo's [release page](https://github.com/Fjara-h/foo_skipcount/releases)

The custom column patterns are %skip_count%, %latest_skip%, %oldest_skip%, skip_times_raw%, and %skip_times_js%.%first_skip% and %last_skip% are also supported, but may be subject to removal in the future.

Runs on Foobar2000 32-bit, 64-bit and legacy Foobar2000 1.5/1.6.

# Build notes

Uses Foobar2000 SDK version 2022-10-20

Install ATL, MFC, and WTL for prc, libppui, and sdk-helper to build. File structure is displayed in subdirectories of the repo. Portable foobar2000 installation's are used and test tracks are in their respect doc folders. WTL is through nuget, as of this edit, 10.0.10320

# External links
[Official discussion thread](https://hydrogenaud.io/index.php/topic,124741.0.html)
[Official upload thread](https://hydrogenaud.io/index.php/topic,124742.0.html)

Note: This is my first foobar2000 component and first c++ in a decade, feel free to pull request or open an issue with requests for fixes or improvements.
