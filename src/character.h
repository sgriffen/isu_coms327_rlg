#include <stdint.h>

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
typedef struct {
	
	Coordinate location;
	
	uint16_t hp;
	uint8_t speed;
	uint16_t damage;
} Character_PC;

typedef struct {
	
	uint32_t id;

	Coordinate location;
	Coordinate prev_location;
	Coordinate last_known;
	
	uint8_t type;
	
	uint8_t speed;
	uint16_t hp;
	uint16_t damage;
} Character_NPC;

typedef struct {
	
	Character_PC *pc;
	Character_NPC *npc;
} Character_Wrapper;

/****** function declarations *****/
Character_PC character_init_pc(Coordinate loc, uint16_t hp, uint16_t damage, uint8_t speed);

Character_NPC character_init_npc(uint32_t id, Coordinate loc, uint8_t type, uint16_t hp, uint16_t damage, uint8_t speed);

void character_print_pc(Character_PC pc, int print_fill, int print_color);

void character_print_npc(Character_NPC npc, int print_fill, int print_color);

#endif /* CHARACTER_H */