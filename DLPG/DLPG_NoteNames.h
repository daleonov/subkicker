#ifndef _DLPG_NOTENAMES_H
#define _DLPG_NOTENAMES_H
/*
Quick maths:
((int)NoteNumber / 12) - 2 = OctaveNumber (C3 convention)
((int)NoteNumber / 12) - 1 = OctaveNumber (C4 convention)
(int)NoteNumber % 12 = NoteIndex
*/

#if (DLPG_USE_FLATS_IN_NOTE_NAMES == true)
char *asNoteNames[] = {
"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B" 
};
#else
char *asNoteNames[] = {
"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" 
};
#endif
#define DLPG_MIDI_NOTE_NAME(n) (asNoteNames[n % 12])


#if (DLPG_USE_C4_CONVENTION == true)
#define DLPG_MIDI_OCTAVE_NUMBER(n) (((int)n / 12) - 1)
#else
#define DLPG_MIDI_OCTAVE_NUMBER(n) (((int)n / 12) - 2)
#endif

#endif //_DLPG_NOTENAMES_H
