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

inline int
clamp(int min, int val, int max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}