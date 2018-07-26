#ifndef __DLPG_FEEDBACKSENDER_H
#define __DLPG_FEEDBACKSENDER_H
/*
List of parameters expected by the server:
host_name
host_version
host_os
plug_architecture
plug_format
plug_name
plug_version
plug_build
plug_build_date
*/
#define DLPG_FEEDBACK_URL_STR \
"\
https://danielleonovplugs.com/feedback#\
host_name=%s&\
host_version=%s&\
host_os=%s&\
plug_architecture=%s&\
plug_format=%s&\
plug_name=%s&\
plug_version=%s&\
plug_build=%s&\
plug_build_date=%s\
"
/*
@brief Generates with URL to pre-fill a bugreport form on the website
@param sDest Output string
*/
inline void PLUG_CLASS_NAME::MakeFeedbackUrl(char* sDest){
	char sHostName[32];
	char sHostVersion[16];
	GetHostNameStr(GetHost(), sHostName);
	GetHostVersionStr(sHostVersion);

	#ifdef OS_WIN
	char sHostOs[] = "Windows";
	#elif defined OS_OSX
	char sHostOs[] = "macOS";
	#else
	char sHostOs[] = "Unknown";
	#endif

	#ifdef ARCH_64BIT
	char sPlugArchitecture[]="64";
	#else
	char sPlugArchitecture[]="32";
	#endif

	sprintf(
		sDest,
		DLPG_FEEDBACK_URL_STR,
		sHostName,
		sHostVersion,
		sHostOs,
		sPlugArchitecture,
		GetAPIString(),
		PLUG_NAME,
		VST3_VER_STR,
		sPlugVersionGitHead,
		sPlugVersionDate
	);
}

#endif // __DLPG_FEEDBACKSENDER_H
