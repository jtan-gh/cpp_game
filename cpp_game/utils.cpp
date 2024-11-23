#define global_variable static;
#define internal static;

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

global_variable const int tile_columns = 48;
global_variable const int tile_rows = 27;
global_variable float tile_width = 48.f;
global_variable float tile_height = 27.f;

struct Button_State {
	bool is_down;
	bool changed;
};

enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,

	W_KEY,
	S_KEY,
	A_KEY,
	D_KEY,

	BUTTON_COUNT
};

struct Input {
	Button_State buttons[BUTTON_COUNT];
};

#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)