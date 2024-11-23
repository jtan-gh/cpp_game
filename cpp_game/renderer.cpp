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
    float unit_width = (float)render_state.width / tile_columns;  // Width of each unit in pixels
    float unit_height = (float)render_state.height / tile_rows; // Height of each unit in pixels

    // Draw vertical lines
    for (int i = 0; i <= tile_columns; ++i) {
        int x = (int)(i * unit_width);
        // Draw a vertical line by setting each pixel in that column
        for (int y = 0; y < render_state.height; ++y) {
            if (x >= 0 && x < render_state.width) {
                *(u32*)((u32*)render_state.memory + (y * render_state.width + x)) = grid_color;
            }
        }
    }

    // Draw horizontal lines
    for (int j = 0; j <= tile_rows; ++j) {
        int y = (int)(j * unit_height);
        // Draw a horizontal line by setting each pixel in that row
        for (int x = 0; x < render_state.width; ++x) {
            if (y >= 0 && y < render_state.height) {
                *(u32*)((u32*)render_state.memory + (y * render_state.width + x)) = grid_color;
            }
        }
    }
}

global_variable float render_scale = 0.01f;

void draw_square_at_center(int size, u32 color = 0xFF0000FF) {
    // Calculate the pixel dimensions of the square based on the current grid size
    float unit_width = (float)render_state.width / tile_columns;
    float unit_height = (float)render_state.height / tile_rows;

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

void fill_between_pixel_coordinates(const PixelCoordinate& top_left, const PixelCoordinate& bottom_right, u32 color = 0xFF0000FF) {
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


PixelCoordinate calculate_top_left_pixel_coord_for_tile(int tile_x, int tile_y) {
    float start_x = tile_x * tile_width;
    float start_y = tile_y * tile_height;
    return { static_cast<int>(std::round(start_x)), static_cast<int>(std::round(start_y)) };
}

PixelCoordinate calculate_top_left_pixel_coord_for_tile(float tile_x, float tile_y) {
    float start_x = tile_x * tile_width;
    float start_y = tile_y * tile_height;
    return { static_cast<int>(std::round(start_x)), static_cast<int>(std::round(start_y)) };
}

PixelCoordinate calculate_bottom_right_pixel_coord_for_tile(int tile_x, int tile_y) {
    float end_x = (tile_x + 1) * tile_width; // Offset by 1 tile
    float end_y = (tile_y + 1) * tile_height; // Offset by 1 tile
    return { static_cast<int>(std::round(end_x)), static_cast<int>(std::round(end_y)) };
}

std::pair<PixelCoordinate, PixelCoordinate> calculate_pixel_coords_for_tile(int tile_x, int tile_y) {
    PixelCoordinate top_left = calculate_top_left_pixel_coord_for_tile(tile_x, tile_y);
    PixelCoordinate bottom_right = calculate_bottom_right_pixel_coord_for_tile(tile_x, tile_y);
    return { top_left, bottom_right };
}


void fill_tile(int tile_x, int tile_y, u32 color = 0xFF0000FF) {
    std::pair<PixelCoordinate, PixelCoordinate> coords = calculate_pixel_coords_for_tile(tile_x, tile_y);
    PixelCoordinate top_left = coords.first;
    PixelCoordinate bottom_right = coords.second;

    // Fill in the square with a color (e.g., blue)
    fill_between_pixel_coordinates(top_left, bottom_right, color);
}


void fill_between_tile_coordinates(int tile_x1, int tile_y1, int tile_x2, int tile_y2, u32 color = 0xFF0000FF) {
    // Get the pixel coordinates for the top-left and bottom-right corners of the rectangle
    std::pair<PixelCoordinate, PixelCoordinate> top_left_coords = calculate_pixel_coords_for_tile(tile_x1, tile_y1);
    std::pair<PixelCoordinate, PixelCoordinate> bottom_right_coords = calculate_pixel_coords_for_tile(tile_x2, tile_y2);

    PixelCoordinate top_left = top_left_coords.first;
    PixelCoordinate bottom_right = bottom_right_coords.first;

    // Fill the rectangle area with the given color
    fill_between_pixel_coordinates(top_left, bottom_right, color);
}
