#include <string>

#include <stdint.h>

#ifndef CONFIG_H
#define CONFIG_H

/********** definitions **********/


/************* macros *************/


/******* enums declarations *******/


/******* struct declarations ******/
typedef struct {
	
	uint8_t fill;
	uint8_t hardness;
	uint8_t weight_ntunneling;
	uint8_t weight_tunneling;
	uint8_t traversal_cost;
} PrintConfig;

typedef struct {
	
	PrintConfig print_config;
	
	std::string config_npc;
	std::string config_item;
	uint32_t num_npcs;
	uint32_t num_items;
	uint32_t num_dungeons;
} RunArgs;

#endif /* CONFIG_H */