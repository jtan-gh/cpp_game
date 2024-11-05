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

global_variable const int TARGET_tile_coordinate_WIDTH = 48;
global_variable const int TARGET_tile_coordinate_HEIGHT = 27;

inline int
clamp(int min, int val, int max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}