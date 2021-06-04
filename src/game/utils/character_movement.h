#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "../classdef/dungeon.h"
#include "../classdef/character.h"
#include "../classdef/coordinate.h"

/********** definitions **********/


/************* macros *************/


/******* enums declarations *******/


/******* struct declarations ******/


/****** function declarations *****/
Coordinate move_away(Dungeon *dungeon, Coordinate start, Coordinate end, int avoid_npc, int (*immutable)(Cell));

Coordinate move_towards(Dungeon *dungeon, Coordinate start, Coordinate end, int avoid_npc, int (*immutable)(Cell));

Coordinate move_min_ntunneling(Dungeon *dungeon, Coordinate loc, int avoid_npc);

Coordinate move_min_tunneling(Dungeon *dungeon, Coordinate loc, int avoid_npc);

Coordinate move_rand(Dungeon *dungeon, Coordinate location, int avoid_npc, int (*immutable)(Cell));

Coordinate move_pc(Dungeon* dungeon, PC *pc, int direction[2]);

Coordinate move_npc0(Dungeon *dungeon, NPC *npc);

Coordinate move_npc1(Dungeon *dungeon, NPC *npc, Coordinate reference);

Coordinate move_npc2(Dungeon *dungeon, NPC *npc);

Coordinate move_npc3(Dungeon *dungeon, NPC *npc);

Coordinate move_npc4(Dungeon *dungeon, NPC *npc);

Coordinate move_npc5(Dungeon *dungeon, NPC *npc, Coordinate reference);

Coordinate move_npc6(Dungeon *dungeon, NPC *npc);

Coordinate move_npc7(Dungeon *dungeon, NPC *npc);

Coordinate move_npc8(Dungeon *dungeon, NPC *npc);

Coordinate move_npc9(Dungeon *dungeon, NPC *npc, Coordinate reference);

Coordinate move_npcA(Dungeon *dungeon, NPC *npc);

Coordinate move_npcB(Dungeon *dungeon, NPC *npc);

Coordinate move_npcC(Dungeon *dungeon, NPC *npc);

Coordinate move_npcD(Dungeon *dungeon, NPC *npc, Coordinate reference);

Coordinate move_npcE(Dungeon *dungeon, NPC *npc);

Coordinate move_npcF(Dungeon *dungeon, NPC *npc);


#endif /* MOVEMENT_H */