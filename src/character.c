/******** include std libs ********/
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
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
		.speed = speed,
		.num_kills = 0
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

void character_print_pc(Character_PC pc, uint8_t y, uint8_t x, int print_fill, int print_color) {
	
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	
	if (print_color) { attron(COLOR_PAIR(1)); }
	if (print_fill) {
		
		if (pc.hp > 0) 	{ mvprintw(y, x, "%2s", "@"); }
		else 			{ mvprintw(y, x, "%2s", "X"); }
	} else {
		
		if (pc.hp > 0) 	{ mvaddch(y, x, '@'); }
		else 			{ mvaddch(y, x, 'X'); }
	}
	
	
	attroff(COLOR_PAIR(1));
}

void character_print_npc(Character_NPC npc, uint8_t y, uint8_t x, int print_fill, int print_color) {
	
	start_color();
	init_pair(2, COLOR_RED, COLOR_BLACK);
	
	if (print_color) 	{ attron(COLOR_PAIR(2)); }
	if (print_fill) 	{ mvprintw(y, x, "%2x", npc.type); }
	else 				{ mvprintw(y, x, "%x", npc.type); }
	
	attroff(COLOR_PAIR(1));
}