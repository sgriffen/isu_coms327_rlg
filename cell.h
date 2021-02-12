/********** definitions **********/
#define CELL_HARDNESS_MIN 0
#define CELL_HARDNESS_MAX 255

/******* enums declarations *******/
enum cell_type {
	
	Border_h,
	Border_v,
	Wall,
	Room,
	Cooridor,
	Stair_up,
	Stair_down
};

/******* struct declarations ******/
struct Cell {
	
	int y, x;
	
	int meta_data;
	
	enum cell_type type;
	unsigned short hardness;
};

/****** function declarations *****/
struct Cell cell_init(int y, int x);

struct Cell cell_init_rand(int y, int x);

int cell_is_same(struct Cell beta, struct Cell alpha);

void cell_print(struct Cell cell, int print_border);