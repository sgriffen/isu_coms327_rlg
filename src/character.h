#include <iostream>
#include <stdint.h>
#include <type_traits>

#ifndef CHARACTER_H
#define CHARACTER_H

#include "./coordinate.h"

/********** definitions **********/
#define PC_SPEED 10
#define NPC_SPEED_MIN 5
#define NPC_SPEED_MAX 20
#define NPC_TYPE_INTELLIGENT 0x01
#define NPC_TYPE_TELEPATHIC 0x02
#define NPC_TYPE_TUNNELING 	0x04
#define NPC_TYPE_ERRATIC 	0x08

/************* macros *************/
#define CHARACTER_TURN(speed) (1000/speed)

/******* enums declarations *******/


/******* struct declarations ******/
class Character {
	public:
		Coordinate location;
		Coordinate prev_location;
		
		uint32_t id;
		
		uint8_t speed;
		uint16_t hp;
		uint16_t damage;
};

class PC: public Character {
	public:
		Character* last_seen;
		
		uint32_t num_kills;
};

class NPC : public Character {
	public: 
		uint8_t type;
		
		Coordinate pc_last_known;
};

/****** function declarations *****/
PC character_init_pc(Coordinate loc, uint16_t hp, uint16_t damage, uint8_t speed);

NPC character_init_npc(uint32_t id, Coordinate loc, uint8_t type, uint16_t hp, uint16_t damage, uint8_t speed);

void character_print_pc(PC pc, uint8_t y, uint8_t x, int print_fill, int print_color);

void character_print_npc(NPC npc, uint8_t y, uint8_t x, int print_fill, int print_color);


#endif /* CHARACTER_H */