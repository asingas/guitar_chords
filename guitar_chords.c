/* Guitar chord/scale display program
 * Written by: Arthur Singas
 * Objective: 1. Develop an algorithm for properly displaying requested chord shapes/scales in text form
 * 			  2. Indirectly learn the notes of the fretboard by attempting to program them
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
char note_requested[MAX_NOTE_SIZE];			/* for storing the note value user requests */
char chord_or_scale;						/* for taking in user request chords or scale */
int num_scales = 0;							/* number of scales seen that are relevant to note_requested */
char **scales;								/* array of strings to store names of scales */
char **notes_of_scale;						/* array of strings to store notes of each scale to be then parsed by their spaces */
char **root;								/* array of strings to store root notes, not needed besides when using strtok in parse_available_scales() */
char **individual_notes;					/* array of strings that store each note of the scale individually so that they can be compared
											 * explicitly with each fret note insteda of as an entire string */

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

void print_scale(int num_notes_in_scale, int scale_index, char *string_name[])
{
	int note_at_fret;

	for (int fret_num = 0; fret_num < 22; fret_num++) {
		for (int i = 0; i < num_notes_in_scale; i++){								
			if (fret_num < 12 && strcmp(individual_notes[i], string_name[fret_num]) == 0) {
				if (strlen(individual_notes[i]) == 2) { 							/* if note is sharp, remove space to keep proper spacing */
					printf("%s", string_name[fret_num]);
				} else {
					printf("%s ", string_name[fret_num]);							/* else, keep space to keep proper spacing */
				}
				note_at_fret = 1;
				break;
			} else {
				note_at_fret = 0;
			}
		}
		if (note_at_fret == 0 && fret_num < 12) {
			printf("- ");
		}
		if (fret_num == 10) {
			printf(" ");
		}
		for (int i = 0; i < num_notes_in_scale; i++) {
			if (fret_num >= 11 && strcmp(individual_notes[i], string_name[fret_num - 11]) == 0) {
				if (strlen(individual_notes[i]) == 2) {
					printf(" %s", string_name[fret_num - 11]);
				} else {
					printf(" %s ", string_name[fret_num - 11]);
				}
				note_at_fret = 1;
				break;
			} else {
				note_at_fret = 0;
			}
		}
		
		if (note_at_fret == 0 && fret_num >= 11) {
			printf(" - ");
		}
	}
	printf("\n");
}

void parse_notes_of_scale(int num_notes_in_scale, int scale_index) 
{
	char delimiters[] = " ";

	individual_notes[0] = strtok(notes_of_scale[scale_index - 1], delimiters);
	for (int i = 1; i < num_notes_in_scale; i++) {
		if (i == num_notes_in_scale - 1) {
			individual_notes[i] = strtok(NULL, delimiters);
			char* temp = (char *)malloc(10 * sizeof(char));						/* temp string to remove newline and copy back to individual_notes */
		 	strcpy(temp, individual_notes[i]);
		 	int j = strlen(individual_notes[i]);								/* integer used to check if last note is a # (size 3) or not (size 2)*/
			if (j == 2){														/* if last note in scale isnt a sharp note */
				if (temp[1] == '\n') {
    				temp[1] = '\0';
				}
			}
			if (j == 3){														/* If last note in scale IS a sharp note */
				if (temp[2] == '\n') {
    				temp[2] = '\0';
				}
			}
			strcpy(individual_notes[i], temp);
			break;	
		} else {
			individual_notes[i] = strtok(NULL, delimiters);
		}
	}
}

/* parse the text file containing all of the scales in the formate of : note:scale name: notes of scale */
/* store results in the char **scales, only care about scales that have note_requested as first part of string */
void parse_available_scales(FILE *in, char *note)
{															
	char *check_root;																/* char* to check parsing relevant scale */
    char delimiters[] = ":";														/* text file elements are separated by ":", must use this token */
	char *scale_entry_check = (char *)malloc(101 * sizeof(char));					/* every line of file, use to check if parsing properly */
	char *scale_entry = (char *)malloc(101 * sizeof(char));							/* to be copied from scale_entry_check so we can use strtok */

	fgets(scale_entry_check, 1000, in);  											/* setting each line of text file = to scale_entry */
	strcpy(scale_entry, scale_entry_check);														
	check_root = strtok(scale_entry_check, delimiters);

	if (strcmp(check_root, note) == 0) {											/* If parsing a line that has root = note_requested */
			root[num_scales] = strtok(scale_entry, delimiters);
			scales[num_scales] = strtok(NULL, delimiters);
			notes_of_scale[num_scales] = strtok(NULL, delimiters);
			num_scales++;
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
	char *valid_notes[17] = { "E", "F", "F#", "G", "G#", "A", "A#", "B", "C", "C#", "D", "D#" };	/* list of all valid notes on fretboard */

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

int get_num_notes_in_scale(int scale_index)
{
	int count = 0;
	char *to_compare = (char *)malloc(30 * sizeof(char));
	strcpy(to_compare, notes_of_scale[scale_index - 1]);
	char space = ' ';
	
	for (int i = 0; i < strlen(notes_of_scale[scale_index - 1]); i++) {
		if (space == to_compare[i]) {
			count++;
		}
	}
	return count + 1;																/* number of white spaces + 1 = number of notes in scale */
}

int get_scale_option()
{
	int response;
	printf("Which scale would you like to see tabs of? Enter the number corresponding to that scale: ");
	while (1) {
		scanf(" %d", &response);
		for (int i = 0; i < num_scales; i++) {
			if (response <= num_scales) {
				return response;
			} else {
				printf("Invalid option, re-enter: ");
				continue;
			}
		}
	}
}

int main()
{
	FILE *to_read = NULL;
	/* Initialization of memory for arrays of strings holding 1) notes 2) scale names 3) notes of that scale */
	root = (char **) malloc(100 * sizeof(char *));
	scales = (char **) malloc(100 * sizeof(char *));
	notes_of_scale = (char **) malloc(100 * sizeof(char *));		
	individual_notes = (char **) malloc(100 * sizeof(char *));
	struct fretboard *fret_point = &fret;

	for (int i = 0; i < 4; i++) {
		*(root + i) = (char *) malloc(20 * sizeof(char *));
		*(scales + i) = (char *) malloc(20 * sizeof(char *));
		*(notes_of_scale + i) = (char *) malloc(20 * sizeof(char *));
		*(individual_notes + i) = (char *) malloc(20 * sizeof(char *));								
	}

	get_note();																		/* Start by asking for a note on the fretboard */
	get_chord_or_scale();														    /* Ask whether user wants chords or scale */

	if (chord_or_scale == 's') {													/* Open respective file containing list of scales/chords to parse */
		to_read = fopen("scale_list.txt", "r");
		printf("list of scales with %s as root: \n", note_requested);

		for (int i = 0; i < get_num_lines_file("scale_list.txt"); i++) {
			parse_available_scales(to_read, note_requested);
		}
		for (int i = 0; i < num_scales; i++) {
			printf("%d) %s %s\n", i+1, note_requested, scales[i]);
		}

		int scale_index = get_scale_option();
		int num_notes_in_scale = get_num_notes_in_scale(scale_index);
		parse_notes_of_scale(num_notes_in_scale, scale_index);
		
		printf("\n%s %s\n\n", note_requested, scales[scale_index-1]);
		for (int i = 0; i < 23; i++) {
			printf("%d ", i);
		}
		
		printf("\n");
		print_scale(num_notes_in_scale, scale_index, fret_point->e_string);
		print_scale(num_notes_in_scale, scale_index, fret_point->b_string);
		print_scale(num_notes_in_scale, scale_index, fret_point->g_string);
		print_scale(num_notes_in_scale, scale_index, fret_point->d_string);
		print_scale(num_notes_in_scale, scale_index, fret_point->a_string);
		print_scale(num_notes_in_scale, scale_index, fret_point->low_e_string);

		free(root);
		fclose(to_read);
	} else if (chord_or_scale == 'c') {
		to_read = fopen("chord_list.txt", "r");
	}
}

