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

void character_print(Character character) {
	
	switch (character.type) {
		
	case CharacterType_PC: 	//print PC character type
		
		printf("%s%c%s", "\x1b[92m", '@', "\x1b[0m"); //color, char, reset-color
		break;
	default :				//print NPC character type
		
		printf("%s%c%s", "\x1b[31m", '&', "\x1b[0m"); //color, char, reset-color
		break;
	}
	
	return;
}