#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "../dungeon.h"
#include "../character.h"
#include "../coordinate.h"

Coordinate move_away(Dungeon *dungeon, Coordinate start, Coordinate end, int avoid_npc, int (*immutable)(Cell));

Coordinate move_towards(Dungeon *dungeon, Coordinate start, Coordinate end, int avoid_npc, int (*immutable)(Cell));

Coordinate move_min_ntunneling(Dungeon *dungeon, Coordinate loc, int avoid_npc);

Coordinate move_min_tunneling(Dungeon *dungeon, Coordinate loc, int avoid_npc);

Coordinate move_rand(Dungeon *dungeon, Coordinate location, int avoid_npc, int (*immutable)(Cell));

Coordinate move_pc(Dungeon* dungeon, Character_PC *pc);

Coordinate move_npc0(Dungeon *dungeon, Character_NPC *npc);

Coordinate move_npc1(Dungeon *dungeon, Character_NPC *npc);

Coordinate move_npc2(Dungeon *dungeon, Character_NPC *npc);

Coordinate move_npc3(Dungeon *dungeon, Character_NPC *npc);

Coordinate move_npc4(Dungeon *dungeon, Character_NPC *npc);

Coordinate move_npc5(Dungeon *dungeon, Character_NPC *npc);

Coordinate move_npc6(Dungeon *dungeon, Character_NPC *npc);

Coordinate move_npc7(Dungeon *dungeon, Character_NPC *npc);

Coordinate move_npc8(Dungeon *dungeon, Character_NPC *npc);

Coordinate move_npc9(Dungeon *dungeon, Character_NPC *npc);

Coordinate move_npcA(Dungeon *dungeon, Character_NPC *npc);

Coordinate move_npcB(Dungeon *dungeon, Character_NPC *npc);

Coordinate move_npcC(Dungeon *dungeon, Character_NPC *npc);

Coordinate move_npcD(Dungeon *dungeon, Character_NPC *npc);

Coordinate move_npcE(Dungeon *dungeon, Character_NPC *npc);

Coordinate move_npcF(Dungeon *dungeon, Character_NPC *npc);

#endif /* MOVEMENT_H */