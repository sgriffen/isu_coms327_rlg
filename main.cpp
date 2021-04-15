/******** include std libs ********/
#include <iostream>
#include <fstream>

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <ncurses.h>
/******* include custom libs ******/
#include "./main.h"
#include "./src/utils/math_utils.h"
#include "./src/utils/str_utils.h"

/****** function definitions ******/
int main(int argc, char *argv[]) {
	
	std::string dir_home(getenv("HOME"));
	RunArgs run_args = {
		
		.print_config = {
			.fill 				= 1,
			.hardness 			= 0,
			.weight_ntunneling 	= 0,
			.weight_tunneling 	= 0,
			.traversal_cost 	= 0
		},
		.config_npc 			= dir_home + "/.rlg327/monster_desc.txt",
		.config_item 			= dir_home + "/.rlg327/object_desc.txt",
		.num_npcs 				= 0,
		.num_items				= 0,
		.num_dungeons 			= 0
	};
	GameState g_state;
	
	srand(time(NULL));
	
	/* parse command args */
	args_parse(argc, argv, &run_args);
	
	/* Create new game state, load in monster and item configurations */
	rouge_init(&g_state, argv, run_args);
	
	/* main game logic */
	g_state.run(run_args);
	
	/* save dungeons to file if desired */
	rouge_clean(&g_state, argv, run_args);
	
	return 0;
}

void args_parse(int argc, char *argv[], RunArgs *run_args) {
	
	int i = 0;
	
	for (i = 1; i < argc; i++) {
		
		if (!strcmp(argv[i], "--config-npc") || !strcmp(argv[i], "--c-npc")) 		{ run_args->config_npc.assign(argv[i+1]); } //define absolute path to file to read npc config from 
		
		if (!strcmp(argv[i], "--config-item") || !strcmp(argv[i], "--c-item")) 		{ run_args->config_item.assign(argv[i+1]); } //define absolute path to file to read item config from
		
		if (!strcmp(argv[i], "--nummon") || !strcmp(argv[i], "--nm")) 				{ run_args->num_npcs = atoi(argv[i+1]); } //define number of npcs per dungeon
		
		if (!strcmp(argv[i], "--numitem") || !strcmp(argv[i], "--nt")) 				{ run_args->num_items = atoi(argv[i+1]); } //define number of items per dungeon
		
		if (!strcmp(argv[i], "--numdun") || !strcmp(argv[i], "--nd")) 				{ run_args->num_dungeons = atoi(argv[i+1]); } //define number of dungeons to generate
		
		if (i+1 < argc && (!strcmp(argv[i], "--print") || !strcmp(argv[i], "--p"))) { //print a (dungeons border) or (filled-in walls and empty rooms)
			
			if (!strcmp(argv[i+1], "f")) 	{ run_args->print_config.fill = 2; } //print (filled-in walls and empty rooms)
		}
	}
	
	if (!(run_args->num_dungeons)) { run_args->num_dungeons = utils_rand_between(2, 10, NULL); }
	
	return;
}

void rouge_init(GameState *g_state, char *argv[], RunArgs run_args) {
	
	srand(time(NULL));
	
	std::vector<NPC_Template>::iterator	it_npc;
	std::vector<NPC_Template> npc_templates = rouge_parse_npc(run_args.config_npc);
	
	std::vector<Item_Template>::iterator it_item;
	std::vector<Item_Template> item_templates = rouge_parse_item(run_args.config_item);
	
	*g_state = GameState(run_args.num_dungeons, run_args.num_npcs, run_args.num_items, npc_templates, item_templates);
	
	rouge_init_terminal();
	
	return;
}

void rouge_init_terminal() {
	
	initscr();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	
	start_color();
	init_pair(PAIR_BLACK, COLOR_WHITE, COLOR_BLACK);
	init_pair(PAIR_WHITE, COLOR_WHITE, COLOR_BLACK);
	init_pair(PAIR_RED, COLOR_RED, COLOR_BLACK);
	init_pair(PAIR_GREEN, COLOR_GREEN, COLOR_BLACK);
	init_pair(PAIR_BLUE, COLOR_BLUE, COLOR_BLACK);
	init_pair(PAIR_CYAN, COLOR_CYAN, COLOR_BLACK);
	init_pair(PAIR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
	init_pair(PAIR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
}

void rouge_clean(GameState *g_state, char *argv[], RunArgs run_args) {
	
	rouge_clean_terminal();
	
	return;
}

void rouge_clean_terminal() {
	
	endwin();
}

std::vector<NPC_Template> rouge_parse_npc(std::string filename) {
	
	int f_npc_desc = 0;
	std::string npc_desc = "";
	int npc_param_count = 0;
	
	int f_linecount = 0;
	std::vector<NPC_Template> npc_templates;
	std::ifstream file(filename);
	
	if (!file.is_open()) { std::cout << "file [" << filename << "] invalid" << std::endl; }
	else {
		
		std::string line;
		while (std::getline(file, line)) {
			if (line.size() > 0) {
				
				if (line[line.size()-1] == '\r') { line = line.substr(0, line.size()-1); }
				std::vector<std::string> line_splt = strsplit(line, " ");
				if (!f_npc_desc) {
					
					if (!f_linecount) {
					
						if (line.compare("RLG327 MONSTER DESCRIPTION 1")) {
						
							std::cout << "NPC configuration file version invalid" << std::endl;
							break;
						}
					}
					if (!line.compare("BEGIN MONSTER")) { npc_templates.push_back(NPC_Template()); }
					else if (!line.compare("END")) {
						
						if (npc_param_count != NPC_NUM_PARAM) { npc_templates.pop_back(); }
						
						npc_param_count = 0;
					}
					else if (!line.compare("DESC")) {
						
						f_npc_desc = 1;
						npc_param_count++;
					}
					else if (!line_splt[0].compare("NAME")) {
						
						npc_templates.back().name.assign("");
						std::vector<std::string>::iterator line_splt_itr;
						for (line_splt_itr = line_splt.begin() + 1; line_splt_itr != line_splt.end(); line_splt_itr++) {
							
							npc_templates.back().name.append(*line_splt_itr);
							if (line_splt_itr != line_splt.end() - 1) { npc_templates.back().name.append(" "); }
						}
						
						npc_param_count++;
					}
					else if (!line_splt[0].compare("SYMB")) {
						
						npc_templates.back().symbol = line_splt[1].c_str()[0];
						
						npc_param_count++;
					}
					else if (!line_splt[0].compare("SPEED")) {
						
						npc_templates.back().speed = Dice(line_splt[1]);
						
						npc_param_count++;
					}
					else if (!line_splt[0].compare("DAM")) {
						
						npc_templates.back().damage = Dice(line_splt[1]);
						
						npc_param_count++;
					}
					else if (!line_splt[0].compare("HP")) {
						
						npc_templates.back().hp = Dice(line_splt[1]);
						
						npc_param_count++;
					}
					else if (!line_splt[0].compare("RRTY")) {
						
						npc_templates.back().rarity = (uint8_t)std::stoul(line_splt[1]);
						
						npc_param_count++;
					}
					else if (!line_splt[0].compare("COLOR")) {
						
						std::vector<std::string>::iterator line_splt_itr;
						for (line_splt_itr = line_splt.begin() + 1; line_splt_itr != line_splt.end(); line_splt_itr++) {
							
							if 		(!(*line_splt_itr).compare("BLACK")) 	{ npc_templates.back().color.push_back((uint8_t)PAIR_BLACK); }
							else if (!(*line_splt_itr).compare("RED")) 		{ npc_templates.back().color.push_back((uint8_t)PAIR_RED); }
							else if (!(*line_splt_itr).compare("GREEN")) 	{ npc_templates.back().color.push_back((uint8_t)PAIR_GREEN); }
							else if (!(*line_splt_itr).compare("BLUE")) 	{ npc_templates.back().color.push_back((uint8_t)PAIR_BLUE); }
							else if (!(*line_splt_itr).compare("CYAN")) 	{ npc_templates.back().color.push_back((uint8_t)PAIR_CYAN); }
							else if (!(*line_splt_itr).compare("YELLOW")) 	{ npc_templates.back().color.push_back((uint8_t)PAIR_YELLOW); }
							else if (!(*line_splt_itr).compare("MAGENTA")) 	{ npc_templates.back().color.push_back((uint8_t)PAIR_MAGENTA); }
							else 											{ npc_templates.back().color.push_back((uint8_t)PAIR_WHITE); }
						}
						npc_param_count++;
					}
					else if (!line_splt[0].compare("ABIL")) {
						
						std::vector<std::string>::iterator line_splt_itr;
						for (line_splt_itr = line_splt.begin() + 1; line_splt_itr != line_splt.end(); line_splt_itr++) {
							
							if 		(!(*line_splt_itr).compare("SMART")) 	{ npc_templates.back().type |= NPC_TYPE_INTELLIGENT; }
							else if (!(*line_splt_itr).compare("TELE")) 	{ npc_templates.back().type |= NPC_TYPE_TELEPATHIC; }
							else if (!(*line_splt_itr).compare("TUNNEL")) 	{ npc_templates.back().type |= NPC_TYPE_TUNNELING; }
							else if (!(*line_splt_itr).compare("ERRATIC")) 	{ npc_templates.back().type |= NPC_TYPE_ERRATIC; }
							else if (!(*line_splt_itr).compare("PASS")) 	{ npc_templates.back().type |= NPC_TYPE_PASS; }
							else if (!(*line_splt_itr).compare("PICKUP")) 	{ npc_templates.back().type |= NPC_TYPE_PICKUP; }
							else if (!(*line_splt_itr).compare("DESTROY")) 	{ npc_templates.back().type |= NPC_TYPE_DESTROY; }
							else if (!(*line_splt_itr).compare("UNIQ")) 	{ npc_templates.back().type |= NPC_TYPE_UNIQUE; }
							else if (!(*line_splt_itr).compare("BOSS")) 	{ npc_templates.back().type |= NPC_TYPE_BOSS; }
						}
						
						npc_param_count++;
					}
				} else {
					
					if (!line.compare(".")) {
						
						npc_desc[npc_desc.size()-1] = '\0';
						npc_templates.back().description.assign(npc_desc);
						
						f_npc_desc = 0;
						npc_desc = "";
					} else {
						
						npc_desc.append(line);
						npc_desc.append("\n");
					}
				}
				f_linecount++;
			}
		}
		
		file.close();
	}
	
	return npc_templates;
}

std::vector<Item_Template> rouge_parse_item(std::string filename) {
	
	int f_item_desc = 0;
	std::string item_desc = "";
	int item_param_count = 0;
	
	int f_linecount = 0;
	std::vector<Item_Template> item_templates;
	std::ifstream file(filename);
	
	if (!file.is_open()) { std::cout << "file [" << filename << "] invalid" << std::endl; }
	else {
		
		std::string line;
		while (std::getline(file, line)) {
			if (line.size() > 0) {
				
				if (line[line.size()-1] == '\r') { line = line.substr(0, line.size()-1); }
				std::vector<std::string> line_splt = strsplit(line, " ");
				if (!f_item_desc) {
					
					if (!f_linecount && line.compare("RLG327 OBJECT DESCRIPTION 1")) {
						
						std::cout << "Object configuration file version invalid" << std::endl;
						break;
					}
					if (!line.compare("BEGIN OBJECT")) { item_templates.push_back(Item_Template()); }
					else if (!line.compare("END")) {
						
						if (item_param_count != ITEM_NUM_PARAM) { item_templates.pop_back(); }
						
						item_param_count = 0;
					}
					else if (!line.compare("DESC")) {
						
						f_item_desc = 1;
						item_param_count++;
					}
					else if (!line_splt[0].compare("NAME")) {
						
						item_templates.back().name.assign("");
						std::vector<std::string>::iterator line_splt_itr;
						for (line_splt_itr = line_splt.begin() + 1; line_splt_itr != line_splt.end(); line_splt_itr++) {
							
							item_templates.back().name.append(*line_splt_itr);
							if (line_splt_itr != line_splt.end() - 1) { item_templates.back().name.append(" "); }
						}
						
						item_param_count++;
					}
					else if (!line_splt[0].compare("HIT")) {
						
						item_templates.back().bonus_hit = Dice(line_splt[1]);
						
						item_param_count++;
					}
					else if (!line_splt[0].compare("DAM")) {
						
						item_templates.back().bonus_damage = Dice(line_splt[1]);
						
						item_param_count++;
					}
					else if (!line_splt[0].compare("DODGE")) {
						
						item_templates.back().bonus_dodge = Dice(line_splt[1]);
						
						item_param_count++;
					}
					else if (!line_splt[0].compare("DEF")) {
						
						item_templates.back().bonus_defense = Dice(line_splt[1]);
						
						item_param_count++;
					}
					else if (!line_splt[0].compare("WEIGHT")) {
						
						item_templates.back().weight = Dice(line_splt[1]);
						
						item_param_count++;
					}
					else if (!line_splt[0].compare("SPEED")) {
						
						item_templates.back().bonus_speed = Dice(line_splt[1]);
						
						item_param_count++;
					}
					else if (!line_splt[0].compare("ATTR")) {
						
						item_templates.back().attribute = Dice(line_splt[1]);
						
						item_param_count++;
					}
					else if (!line_splt[0].compare("VAL")) {
						
						item_templates.back().value = Dice(line_splt[1]);
						
						item_param_count++;
					}
					else if (!line_splt[0].compare("ART")) {
						
						if (!line_splt[1].compare("TRUE")) 	{ item_templates.back().status = 1; }
						else 								{ item_templates.back().status = 0; }
						
						item_param_count++;
					}
					else if (!line_splt[0].compare("RRTY")) {
						
						item_templates.back().rarity = (uint8_t)std::stoul(line_splt[1]);
						
						item_param_count++;
					}
					else if (!line_splt[0].compare("TYPE")) {
						
						std::vector<std::string>::iterator line_splt_itr;
						for (line_splt_itr = line_splt.begin() + 1; line_splt_itr != line_splt.end(); line_splt_itr++) {
							
							if 		(!(*line_splt_itr).compare("WEAPON")) 		{ item_templates.back().type |= ITEM_TYPE_WEAPON; }
							else if (!(*line_splt_itr).compare("OFFHAND")) 		{ item_templates.back().type |= ITEM_TYPE_OFFHAND; }
							else if (!(*line_splt_itr).compare("RANGED")) 		{ item_templates.back().type |= ITEM_TYPE_RANGED; }
							else if (!(*line_splt_itr).compare("ARMOR")) 		{ item_templates.back().type |= ITEM_TYPE_ARMOR; }
							else if (!(*line_splt_itr).compare("HELMET")) 		{ item_templates.back().type |= ITEM_TYPE_HELMET; }
							else if (!(*line_splt_itr).compare("CLOAK")) 		{ item_templates.back().type |= ITEM_TYPE_CLOAK; }
							else if (!(*line_splt_itr).compare("GLOVES"))		{ item_templates.back().type |= ITEM_TYPE_GLOVES; }
							else if (!(*line_splt_itr).compare("BOOTS")) 		{ item_templates.back().type |= ITEM_TYPE_BOOTS; }
							else if (!(*line_splt_itr).compare("RING")) 		{ item_templates.back().type |= ITEM_TYPE_RING; }
							else if (!(*line_splt_itr).compare("AMULET")) 		{ item_templates.back().type |= ITEM_TYPE_AMULET; }
							else if (!(*line_splt_itr).compare("LIGHT")) 		{ item_templates.back().type |= ITEM_TYPE_LIGHT; }
							else if (!(*line_splt_itr).compare("SCROLL")) 		{ item_templates.back().type |= ITEM_TYPE_SCROLL; }
							else if (!(*line_splt_itr).compare("BOOK")) 		{ item_templates.back().type |= ITEM_TYPE_BOOK; }
							else if (!(*line_splt_itr).compare("FLASK")) 		{ item_templates.back().type |= ITEM_TYPE_FLASK; }
							else if (!(*line_splt_itr).compare("GOLD")) 		{ item_templates.back().type |= ITEM_TYPE_GOLD; }
							else if (!(*line_splt_itr).compare("AMMUNITION")) 	{ item_templates.back().type |= ITEM_TYPE_AMMUNITION; }
							else if (!(*line_splt_itr).compare("FOOD")) 		{ item_templates.back().type |= ITEM_TYPE_FOOD; }
							else if (!(*line_splt_itr).compare("WAND")) 		{ item_templates.back().type |= ITEM_TYPE_WAND; }
							else if (!(*line_splt_itr).compare("CONTAINER")) 	{ item_templates.back().type |= ITEM_TYPE_CONTAINER; }
						}
						
						item_param_count++;
					}
					else if (!line_splt[0].compare("COLOR")) {
						
						std::vector<std::string>::iterator line_splt_itr;
						for (line_splt_itr = line_splt.begin() + 1; line_splt_itr != line_splt.end(); line_splt_itr++) {
							
							if 		(!(*line_splt_itr).compare("BLACK")) 	{ item_templates.back().color.push_back((uint8_t)PAIR_BLACK); }
							else if (!(*line_splt_itr).compare("RED")) 		{ item_templates.back().color.push_back((uint8_t)PAIR_RED); }
							else if (!(*line_splt_itr).compare("GREEN")) 	{ item_templates.back().color.push_back((uint8_t)PAIR_GREEN); }
							else if (!(*line_splt_itr).compare("BLUE")) 	{ item_templates.back().color.push_back((uint8_t)PAIR_BLUE); }
							else if (!(*line_splt_itr).compare("CYAN")) 	{ item_templates.back().color.push_back((uint8_t)PAIR_CYAN); }
							else if (!(*line_splt_itr).compare("YELLOW")) 	{ item_templates.back().color.push_back((uint8_t)PAIR_YELLOW); }
							else if (!(*line_splt_itr).compare("MAGENTA")) 	{ item_templates.back().color.push_back((uint8_t)PAIR_MAGENTA); }
							else 											{ item_templates.back().color.push_back((uint8_t)PAIR_WHITE); }
						}
						item_param_count++;
					}
				} else {
					
					if (!line.compare(".")) {
						
						item_desc[item_desc.size()-1] = '\0';
						item_templates.back().description.assign(item_desc);
						
						f_item_desc = 0;
						item_desc = "";
					} else {
						
						item_desc.append(line);
						item_desc.append("\n");
					}
				}
				
				f_linecount++;
			}
		}
		
		file.close();
	}
	
	return item_templates;
}

