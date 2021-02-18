/******** include std libs ********/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/******* include custom libs ******/
#include "character.h"

/*********** global vars **********/

Character character_init(Cell *loc, CharacterType type) {
	
	Character character;
	
	character.location = loc;
	character.type = type;
	character.meta_data = -1;
	
	return character;
}

void character_print(Character character, int print_color) {
	
	char *color = "\x1b[97m";
	
	switch (character.type) {
		
	case CharacterType_PC: 	//print PC character type
		
		if (print_color) { color = "\x1b[92m"; }
		printf("%s%c%s", color, '@', "\x1b[0m"); //color, char, reset-color
		break;
	default :				//print NPC character type
		
		if (print_color) { color = "\x1b[31m"; }
		printf("%s%c%s", color, '&', "\x1b[0m"); //color, char, reset-color
		break;
	}
	
	return;
}