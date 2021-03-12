#include <stdint.h>

#ifndef CONFIG_H
#define CONFIG_H

/********** definitions **********/


/************* macros *************/


/******* enums declarations *******/


/******* struct declarations ******/
typedef struct {
	
	uint8_t fill;
	uint8_t color;
	uint8_t hardness;
	uint8_t weight_ntunneling;
	uint8_t weight_tunneling;
	uint8_t traversal_cost;
} PrintConfig;

typedef struct {
	
	PrintConfig print_config;
	
	uint8_t file_load;
	char *load_dir;
	uint8_t file_save;
	char *save_dir;
	
	uint32_t num_npcs;
	uint32_t num_dungeons;
} RunArgs;

#endif /* CONFIG_H */