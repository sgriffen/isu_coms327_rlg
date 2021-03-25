/******** include std libs ********/
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

/******* include custom libs ******/
#include "./character.h"
#include "./utils/movement.h"

/*********** global vars **********/


/****** function definitions ******/
PC character_init_pc(Coordinate loc, uint16_t hp, uint16_t damage, uint8_t speed) {
	
	PC pc;		
	pc.location = loc;
	pc.prev_location = loc;
	pc.hp = hp;
	pc.damage = damage;
	pc.speed = speed;
	pc.num_kills = 0;
	
	return pc;
}
NPC character_init_npc(uint32_t id, Coordinate loc, uint8_t type, uint16_t hp, uint16_t damage, uint8_t speed) {
	
	NPC npc;
	npc.id = id;
	npc.location = loc;
	npc.prev_location = loc;
	npc.pc_last_known.y = 0;
	npc.pc_last_known.x = 0;
	npc.type = type;
	npc.hp = hp;
	npc.damage = damage;
	npc.speed = speed;
	
	return npc;
}

void character_print_pc(PC pc, uint8_t y, uint8_t x, int print_fill, int print_color) {
	
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

void character_print_npc(NPC npc, uint8_t y, uint8_t x, int print_fill, int print_color) {
	
	start_color();
	init_pair(2, COLOR_RED, COLOR_BLACK);
	
	if (print_color) 	{ attron(COLOR_PAIR(2)); }
	if (print_fill) 	{ mvprintw(y, x, "%2x", npc.type); }
	else 				{ mvprintw(y, x, "%x", npc.type); }
	
	attroff(COLOR_PAIR(1));
}