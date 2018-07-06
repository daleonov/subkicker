# Based on http://subsynth.sourceforge.net/midinote2freq.html
import argparse


def a_valid_freq(x):
    x = float(x)
    if x < 430.0 or x > 450.0:
        raise argparse.ArgumentTypeError("%r not in valid range 430.0..450.0"%(x,))
    return x

def GenerateNamesAndFrequencies(nNumberOfNotes, sConvention, sFlatSharp, fAFrequency):
	dnOctaveNumberOffset = {
		'C3': -2,
		'C4': -1
	}
	dlsNoteNames = {
		'#': ('C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B'),
		'b': ('C', 'Db', 'D', 'Eb', 'E', 'F', 'Gb', 'G', 'Ab', 'A', 'Bb', 'B')
	}
	#lfNoteFrequencies = []
	lsNoteLabels = []
	dNoteFrequencies = {}
	for i in range(0, nNumberOfNotes-1):
		nOctaveNumber = dnOctaveNumberOffset[sConvention] + i // 12
		nNoteNameIndex = i % 12
		sNoteLabel = dlsNoteNames[sFlatSharp][nNoteNameIndex] + str(nOctaveNumber)
		lsNoteLabels.append(sNoteLabel)
		fFrequency = (fAFrequency/32)*(2**(float(i-9)/12))
		#lfNoteFrequencies.append(fFrequency)
		dNoteFrequencies[sNoteLabel] = fFrequency
	return (lsNoteLabels, dNoteFrequencies)

if __name__ == '__main__':
	cParser = argparse.ArgumentParser()
	cParser.add_argument(
		'-n','--number-of-notes',
		type = int,
		default = 128,
		choices = range(1, 128),
		help = 'generates that amount of notes from lowest C (always). Default is 128. ',
		required = False
		)
	cParser.add_argument(
		'-c','--convention',
		type = str,
		default = 'C3',
		choices = ['C3', 'C4'],
		help = '\"C3\" or \"C4\" - number of the middle C (Yamaha or Roland standards). Default is C3. ',
		required = False
		)
	cParser.add_argument(
		'-fs','--flat-sharp',
		type = str,
		default = '#',
		choices = ['#', 'b'],
		help = 'Use \"#\" or \"b\" in note names. Default is \"#\". ',
		required = False
		)
	cParser.add_argument(
		'-a','--a-frequency',
		type = a_valid_freq,
		default = a_valid_freq(440.),
		help = 'frequency of middle A note. Default is 440.',
		required = False
		)
	cArgs = cParser.parse_args()
	print cArgs
	lsNoteLabels, dNoteFrequencies = GenerateNamesAndFrequencies(cArgs.number_of_notes, cArgs.convention, cArgs.flat_sharp, cArgs.a_frequency)

	for sNoteName in lsNoteLabels:
		print(sNoteName + ': ' + str(dNoteFrequencies[sNoteName]))
