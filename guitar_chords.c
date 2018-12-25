/* Guitar chord/scale display program
 * Written by: Arthur Singas
 * Objective: 1. Develop an algorithm for properly displaying requested chord shapes/scales in text form
 * 			  2. Indirectly master the notes of the fretboard by attempting to program them
*/

/*
 * Program asks for user input for a note - DONE
 * Pulls up a list of all possible chords with that note as the root
 * Displays the proper fretting in the form of text using print statements
 * Possible implementation of scales and all of their positions, labelling notes of scale
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* DEFINING MAX SIZE OF A NOTE WHEN REPRESENTED AS A STRING */
#define MAX_NOTE_SIZE 2

/* GLOBAL VARIABLES */	
char note_requested[MAX_NOTE_SIZE];																/* for storing the note value user requests */
char chord_or_scale;																			/* for taking in user request chords or scale */
char *valid_notes[17] = { "E", "F", "F#", "G", "G#", "A", "A#", "B", "C", "C#", "D", "D#" };	/* list of all valid notes on fretboard */
int num_scales;																					/* number of scales seen that are relevant to note_requested */

/* Struct of fretboard on with 6 arrays of character pointers, each index representing each note
 * on the fretboard starting from the open string (index: [0]) to the 11th. 12th fret is equal to open string
*/

struct fretboard {
	char *e_string[12];
	char *b_string[12];
	char *g_string[12];
	char *d_string[12];
	char *a_string[12];
	char *low_e_string[12];
};

/* Initialization of fretboard / setting of each index (fret) of string to respective notes */
struct fretboard fret = { { "E", "F", "F#", "G", "G#", "A", "A#", "B", "C", "C#", "D", "D#" },
						  { "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#" },
						  { "G", "G#", "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#" },
						  { "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "C", "C#" },
						  { "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" },
						  { "E", "F", "F#", "G", "G#", "A", "A#", "B", "C", "C#", "D", "D#" } };

void print_scale(char *note)
{

}

char* parse_available_scales(FILE *in, char *note)
{	
	char *check;	
	char **scales = (char **)malloc(100*sizeof(char *));							/* char* to check if current scale is of the correct note  */
    char delimiters[] = ":";														/* text file elements are separated by ":", must use this token */
	char *scale_entry_check = (char *)malloc(101 * sizeof(char));					/* scale_entry is every line of text file */
	char *scale_entry = (char *)malloc(101 * sizeof(char));

	for (int i = 0; i < 3; i++) {
		*(scales + i) = (char *)malloc(20*sizeof(char *));							/* malloc for 1) note value, 2) name 3) notes of scale */
	}
	if (fgets(scale_entry_check, 100, in) == NULL) {								/* setting each line of text file = to scale_entry */
		return (char *) NULL;
	}

	strcpy(scale_entry, scale_entry_check);														
	check = strtok(scale_entry_check, delimiters);
	if (strcmp(check, note) == 0) {
		scales[0] = strtok(scale_entry, delimiters);
		scales[1] = strtok(NULL, delimiters);
		scales[2] = strtok(NULL, delimiters);
		printf("%s", scales[2]);
	}
	return scales[1];
}

void parse_scale_notes(FILE *in, char *note)
{
	
}

void print_chord(char *note)
{

}

void print_fretboard()
{
	for (int fret = 0; fret <= 24; fret++) {
		printf("%d ", fret);
	}
}

int get_num_lines_file(char *name)
{
	FILE *fp; 
    int count = 0;  																/* Line counter (result) */
    char c;  										 								/* To store a character read from file  */
  
    fp = fopen(name, "r");  	
    if (fp == NULL) {																/* Check if file exists */ 
        printf("Could not open file %s", name); 
        return 0; 
    } 
  
    for (c = getc(fp); c != EOF; c = getc(fp)) { 									/* Extract characters from file and store in character c */
        if (c == '\n') {															/* Increment count if this character is newline */
            count++;
        }
  	}
    fclose(fp); 
    return count; 
} 

char* get_note()
{
	/* List of all valid notes */
	printf("List of valid notes: ");
    for (int i = 0; i < 12; i++) {
    	printf("%s ", valid_notes[i]);
    }

	while (1) {
		printf("\nEnter a note on the fretboard: ");
		scanf("%s", note_requested);

		/* Check if user input is a valid note */
		for (int i = 0; i < 12; i++) {
			if (strcmp(note_requested, valid_notes[i]) == 0) {
				return note_requested;
			}
		}
		/* If check makes it to end of list without returning, note entered is invalid. User must re-input */
		if (strcmp(note_requested, valid_notes[11]) != 0) {
        	printf("Invalid note, re-enter");
        	continue;
		}
	}
}

char get_chord_or_scale()
{
	char chord = 'c';
	char scale = 's';
	printf("Enter 'c' for chords or 's' for scales relative to %s: ", note_requested);
	while (1) {
		scanf(" %c", &chord_or_scale);
	    if (chord_or_scale == chord || chord_or_scale == scale) {
			return chord_or_scale;
		} else {
			printf("Invalid option, re-enter: ");
			continue;
		} 	
	}
}

int main()
{
	FILE *to_read = NULL;
	get_note();																		/* Start by asking for a note on the fretboard */
	get_chord_or_scale();														    /* Ask whether user wants chords or scale */
	if (chord_or_scale == 's') {													/* Open respective file containing list of scales/chords to parse */
		to_read = fopen("scale_list.txt", "r");
		for (int i = 0; i < get_num_lines_file("scale_list.txt"); i++){
			printf("%s", parse_available_scales(to_read, note_requested));
		}
		fclose(to_read);
	} else if (chord_or_scale == 'c') {
		to_read = fopen("chord_list.txt", "r");
	}

}
