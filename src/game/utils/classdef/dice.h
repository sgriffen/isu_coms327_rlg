#include <stdint.h>
#include <string>

#ifndef DICE_H
#define DICE_H

/********** definitions **********/


/************* macros *************/


/******* enums declarations *******/


/******* class declarations ******/
class Dice {
	public:
		uint32_t base;
		uint32_t num_dice;
		uint32_t num_sides;
	
		/* CONSTRUCTORS */
		Dice();
		Dice(uint32_t dice_base, uint32_t dice_num_dice, uint32_t dice_num_sides);
		Dice(std::string dice_str);
		
		/* FUNCTIONS */
		uint32_t roll();
		std::string to_string();
		void print();
};

#endif /* DICE_H */