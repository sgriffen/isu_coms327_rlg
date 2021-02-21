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
	
	uint32_t id;
	
	Cell *location;
	
	CharacterType type;
	
	int meta_data;
} Character;

/****** function declarations *****/
Character character_init(uint32_t id, Cell *loc, CharacterType type);

void character_print(Character character, int print_color);

#endif /* CHARACTER_H */