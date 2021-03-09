/******** include std libs ********/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/******* include custom libs ******/
#include "./character.h"
#include "./utils/movement.h"

/*********** global vars **********/


/****** function definitions ******/
Character_PC character_init_pc(Coordinate loc, uint16_t hp, uint16_t damage, uint8_t speed) {
	
	Character_PC pc = {
		
		.location = loc,
		.prev_location = loc,
		.hp = hp,
		.damage = damage,
		.speed = speed
	};
	
	return pc;
}
Character_NPC character_init_npc(uint32_t id, Coordinate loc, uint8_t type, uint16_t hp, uint16_t damage, uint8_t speed) {
	
	Character_NPC npc = {
		
		.id = id,
		.location = loc,
		.prev_location = loc,
		.last_known = {
			
			.y = 0,
			.x = 0
		},
		.type = type,
		.hp = hp,
		.damage = damage,
		.speed = speed
	};
	
	return npc;
}

void character_print_pc(Character_PC pc, int print_fill, int print_color) {
	
	char *color = "\x1b[97m";
	
	if (print_color) 	{ color = "\x1b[92m"; }
	if (print_fill) 	{ printf("%s%2s%s", color, "@", "\x1b[0m"); } 	//color, str, reset-color
	else 				{ printf("%s%c%s", color, '@', "\x1b[0m"); } 	//color, char, reset-color
}

void character_print_npc(Character_NPC npc, int print_fill, int print_color) {
	
	char *color = "\x1b[97m";
	
	if (print_color) 	{ color = "\x1b[31m"; }
	if (print_fill) 	{ printf("%s%2x%s", color, npc.type, "\x1b[0m"); } 	//color, str, reset-color
	else 				{ printf("%s%x%s", color, npc.type, "\x1b[0m"); } 	//color, char, reset-color
}