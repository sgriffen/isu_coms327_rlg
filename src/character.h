#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdint.h>

#include "cell.h"

/********** definitions **********/

/******* enums declarations *******/
typedef enum {
	
	CharacterType_PC,
	CharacterType_NPC
} CharacterType;

/******* struct declarations ******/
typedef struct {
	
	Cell *location;
	
	CharacterType type;
	
	int meta_data;
} Character;

/****** function declarations *****/
Character character_init(Cell *loc, CharacterType type);

void character_print(Character character, int print_color);

#endif /* CHARACTER_H */