#it commit hash
ver=$(git rev-parse --short HEAD)
#local time
ver_date=$(date +%F)

#echo *** Generating a header with current git revision ***
echo "Date: $ver_date"
echo "Version: $ver"

header_name="DLPG_Version.h"
header_define=_DLPG_VERSION_H

echo "#ifndef $header_define" > $header_name
echo "#define $header_define" >> $header_name
echo "// Do not modify. Generated automatically." >> $header_name
echo "// Generated from Mac compiler" >> $header_name
echo "char sPlugVersionDate[11] = \"$ver_date\";" >> $header_name
echo "char sPlugVersionGitHead[8] = \"$ver\";" >> $header_name
echo "#endif" >> $header_name
