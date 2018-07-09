#ifndef __DLPG_SUBDIVISIONS_H
#define __DLPG_SUBDIVISIONS_H

#include <limits>

/*
American Name                  British Name 

double note                 |   breve   
whole note                  |   semibreve
half note                   |   minim   
quarter note                |   crotchet    
eighth note                 |   quaver
sixteenth note              |   semiquaver  
thirty-second note          |   demisemiquaver  
sixty-fourth note           |   hemidemisemiquaver
hundred twenty-eighth note  |   semihemidemisemiquaver

*/

#define DLPG_NUMBER_OF_SUBDIVISIONS 6
#define DLPG_USE_BRITISH_SUBDIVISION_NAMES false

#if DLPG_USE_BRITISH_SUBDIVISION_NAMES
char *asSubdivisionNames[DLPG_NUMBER_OF_SUBDIVISIONS] = {
	"demisemiquaver", "semiquaver", "quaver", "crotchet", "minim", "semibreve"
};
#else
char *asSubdivisionNames[DLPG_NUMBER_OF_SUBDIVISIONS] = {
	"1/32", "1/16", "1/8", "1/4", "1/2", "1"
};
#endif

const double afSubdivisionCoefficients[DLPG_NUMBER_OF_SUBDIVISIONS] = {
	1./32, 1./16, 1./8, 1./4, 1./2, 1.
};

inline double SubdivisionToSeconds(int nSubdivisionIndex, double fTempoBpm){
/*
Quick maths:
Duration of quarter note = 60./fTempoBpm (in seconds)
Duration of bar = 4*60./fTempoBpm
Duration of arbitrary subdivision = afSubdivisionCoefficients[nSubdivisionIndex]*4*60./fTempoBpm
*/
	if(fTempoBpm < std::numeric_limits<double>::epsilon()) return -1.;
	return afSubdivisionCoefficients[nSubdivisionIndex] * 240. / fTempoBpm;
}

#endif // __DLPG_SUBDIVISIONS_H
