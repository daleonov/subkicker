@echo off
:: git commit hash
FOR /F "tokens=*" %%g IN ('git rev-parse --short HEAD') do (SET VAR=%%g)
::local time
for /F "usebackq tokens=1,2 delims==" %%i in (`wmic os get LocalDateTime /VALUE 2^>NUL`) do if '.%%i.'=='.LocalDateTime.' set ldt=%%j
set ldt=%ldt:~0,4%-%ldt:~4,2%-%ldt:~6,2%
set date=%ldt%

echo *** Generating a header with current git revision ***
echo Date: %date%
echo Version: %VAR%

SET header_name="DLPG_Version.h"
SET header_define=_DLPG_VERSION_H
echo #ifndef %header_define% > %header_name%
echo #define %header_define% >> %header_name%
echo // Do not modify. Generated automatically. >> %header_name%
echo // Generated from PC compiler >> %header_name%
echo char sPlugVersionDate[11] = "%date%"; >> %header_name%
echo char sPlugVersionGitHead[8] = "%var%"; >> %header_name%
echo #endif >> %header_name%
