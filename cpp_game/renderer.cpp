struct TileCoordinate {
    int x;
    int y;
};

struct PixelCoordinate {
    int x;
    int y;
};


void clear_screen(u32 color) {
    u32* pixel = (u32*)render_state.memory;
    for (int y = 0; y < render_state.height; y++) {
        for (int x = 0; x < render_state.width; x++) {
            *pixel++ = color;
        }
    }
}

void draw_grid() {
    u32 grid_color = 0xFFAAAAAA;  // Light gray color for the grid

    // Calculate the width and height of each grid cell based on the target game units
    float unit_width = (float)render_state.width / TARGET_tile_coordinate_WIDTH;  // Width of each unit in pixels
    float unit_height = (float)render_state.height / TARGET_tile_coordinate_HEIGHT; // Height of each unit in pixels

    // Draw vertical lines
    for (int i = 0; i <= TARGET_tile_coordinate_WIDTH; ++i) {
        int x = (int)(i * unit_width);
        // Draw a vertical line by setting each pixel in that column
        for (int y = 0; y < render_state.height; ++y) {
            if (x >= 0 && x < render_state.width) {
                *(u32*)((u32*)render_state.memory + (y * render_state.width + x)) = grid_color;
            }
        }
    }

    // Draw horizontal lines
    for (int j = 0; j <= TARGET_tile_coordinate_HEIGHT; ++j) {
        int y = (int)(j * unit_height);
        // Draw a horizontal line by setting each pixel in that row
        for (int x = 0; x < render_state.width; ++x) {
            if (y >= 0 && y < render_state.height) {
                *(u32*)((u32*)render_state.memory + (y * render_state.width + x)) = grid_color;
            }
        }
    }
}


internal void draw_rect_in_pixels( int x0, int y0, int x1, int y1, u32 color) {

	x0 = clamp(0, x0, render_state.width);
	x1 = clamp(0, x1, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	y1 = clamp(0, y1, render_state.height);



	for (int y = y0; y < y1; y++) {
		u32* pixel = (u32*)render_state.memory + x0 + y * render_state.width;
		for (int x = x0; x < x1; x++) {
			*pixel++ = color;
		}
	}
}

global_variable float render_scale = 0.01f;

internal void
draw_rect(float x, float y, float half_size_x, float half_size_y, u32 color) {
	x *= render_state.height * render_scale;
	y *= render_state.height * render_scale;
	half_size_x *= render_state.height * render_scale;
	half_size_y *= render_state.height * render_scale;

	x += render_state.width / 2.f;
	y += render_state.height / 2.f;


	int x0 = x - half_size_x;
	int x1 = x + half_size_x;
	int y0 = y - half_size_y;
	int y1 = y + half_size_y;

	draw_rect_in_pixels(x0, y0, x1, y1, color);
}
void draw_square_at_center(int size, u32 color = 0xFF0000FF) {
    // Calculate the pixel dimensions of the square based on the current grid size
    float unit_width = (float)render_state.width / TARGET_tile_coordinate_WIDTH;
    float unit_height = (float)render_state.height / TARGET_tile_coordinate_HEIGHT;

    // Calculate the center offsets
    int center_x = render_state.width / 2;
    int center_y = render_state.height / 2;

    // Define the square's corners in game units (from -1 to 1)
    int half_size = size / 2;

    // Calculate the pixel coordinates
    int start_x = center_x - (half_size * unit_width);
    int end_x = center_x + (half_size * unit_width);
    int start_y = center_y - (half_size * unit_height);
    int end_y = center_y + (half_size * unit_height);

    // Fill in the square
    for (int y = start_y; y < end_y; ++y) {
        for (int x = start_x; x < end_x; ++x) {
            if (x >= 0 && x < render_state.width && y >= 0 && y < render_state.height) {
                *(u32*)((u32*)render_state.memory + x + y * render_state.width) = color; // Fill with blue color
            }
        }
    }
}

void fill_square_between_pixel_coordinates(const PixelCoordinate& top_left, const PixelCoordinate& bottom_right, u32 color = 0xFF0000FF) {
    // Iterate over the y-coordinates from top to bottom
    for (int y = top_left.y; y < bottom_right.y; ++y) {
        // Calculate the corresponding pixel row in the buffer for top-left origin
        int pixel_y = render_state.height - y - 1;

        // Iterate over the x-coordinates from left to right
        for (int x = top_left.x; x < bottom_right.x; ++x) {
            // Check bounds to avoid accessing out-of-range memory
            if (x >= 0 && x < render_state.width && pixel_y >= 0 && pixel_y < render_state.height) {
                // Fill the pixel color at (x, pixel_y)
                *(u32*)((u32*)render_state.memory + (pixel_y * render_state.width + x)) = color; // Fill with blue color
            }
        }
    }
}



std::pair<PixelCoordinate, PixelCoordinate> calculate_pixel_coords_for_tile(int tile_x, int tile_y) {
    // Calculate the pixel dimensions of one game unit
    int unit_width = render_state.width / TARGET_tile_coordinate_WIDTH;
    int unit_height = render_state.height / TARGET_tile_coordinate_HEIGHT;

    int start_x = tile_x * unit_width;
    int start_y = tile_y * unit_height;
    int end_x = start_x + unit_width;
    int end_y = start_y + unit_height;

    return { {start_x, start_y}, {end_x, end_y} };
}

void fill_square_at_tile_coordinate(int tile_x, int tile_y, u32 color = 0xFF0000FF) {
    std::pair<PixelCoordinate, PixelCoordinate> coords = calculate_pixel_coords_for_tile(tile_x, tile_y);
    PixelCoordinate top_left = coords.first;
    PixelCoordinate bottom_right = coords.second;

    // Fill in the square with a color (e.g., blue)
    fill_square_between_pixel_coordinates(top_left, bottom_right, color);
}
