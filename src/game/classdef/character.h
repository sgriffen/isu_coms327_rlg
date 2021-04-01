#include <iostream>
#include <stdint.h>
#include <type_traits>
#include <ncurses.h>

#ifndef CHARACTER_H
#define CHARACTER_H

#include "./npc_template.h"
#include "./item_template.h"
#include "./coordinate.h"
#include "../../res/color.h"

/********** definitions **********/
#define PC_SPEED 10

/************* macros *************/
#define CHARACTER_TURN(speed) (1000/speed)

/******* enums declarations *******/


/******* struct declarations ******/
class Character {
	public:
		Coordinate location;
		Coordinate prev_location;
		
		uint32_t id;
		char *name;
		char *description;
		char symbol;
		
		uint8_t speed;
		uint16_t hp;
		uint16_t damage;
		uint8_t color;
		
		/* CONSTRUCTORS */
		Character();
		Character(uint32_t character_id, Coordinate character_loc, uint16_t character_hp, uint16_t character_damage, uint8_t character_speed);
		Character(char *character_name, char *character_description, char character_symbol, uint8_t character_color, uint32_t character_id, Coordinate character_loc, uint16_t character_hp, uint16_t character_damage, uint8_t character_speed);

		/* FUNCTIONS */
		void draw(uint8_t print_y, uint8_t print_x, int print_fill);
		
};

class PC : public Character {
	public:
		Character* last_seen;
		
		uint32_t num_kills;
		
		/* CONSTRUCTORS */
		PC();
		PC(Coordinate pc_loc, uint16_t pc_hp, uint16_t pc_damage, uint8_t pc_speed);
		PC(char *pc_name, char *pc_description, char pc_symbol, uint8_t pc_color, Coordinate pc_loc, uint16_t pc_hp, uint16_t pc_damage, uint8_t pc_speed);

		/* FUNCTIONS */
		
};

class NPC : public Character {
	public: 
		uint16_t type;
		uint8_t rarity;
		
		Coordinate pc_last_known;
		
		/* CONSTRUCTORS */
		NPC();
		NPC(uint32_t npc_id, Coordinate npc_loc, uint16_t npc_hp, uint16_t npc_type, uint16_t npc_damage, uint8_t npc_speed);
		NPC(uint8_t npc_rarity, char *npc_name, char *npc_description, char npc_symbol, uint8_t npc_color, uint32_t npc_id, Coordinate npc_loc, uint16_t npc_hp, uint16_t npc_type, uint16_t npc_damage, uint8_t npc_speed);

		/* FUNCTIONS */
		
};

/****** function declarations *****/


#endif /* CHARACTER_H */