#define is_down(b) input -> buttons[b].is_down
#define pressed(b) (input -> buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input -> buttons[b].is_down && input->buttons[b].changed)

int ball_pos_x = 0;
int ball_pos_y = 0;
PixelCoordinate player_pixel_coordinate = { 0, 0 };

float left_bar_tile_x = 2;
float left_bar_tile_y = 10;
float left_bar_tile_x2 = left_bar_tile_x+1;
float left_bar_tile_y2 = tile_rows- left_bar_tile_y;

internal void simulate_game(Input* input, float dt) {
	clear_screen(0xff5500);
	draw_grid();
	//fill_between_tile_coordinates(1, 1, tile_columns-1, tile_rows-1, 0xBBBBBB); // arena

	float speed = 1;
	float adjusted_pixel_speed = speed * render_state.width;
	float adjusted_tile_speed = speed * 60.f;

	if (is_down(BUTTON_UP)) player_pixel_coordinate.y -= adjusted_pixel_speed * dt;
	if (is_down(BUTTON_DOWN)) player_pixel_coordinate.y += adjusted_pixel_speed * dt;
	if (is_down(BUTTON_LEFT)) player_pixel_coordinate.x -= adjusted_pixel_speed * dt;
	if (is_down(BUTTON_RIGHT)) player_pixel_coordinate.x += adjusted_pixel_speed * dt;

	if (is_down(BUTTON_UP)) {
		left_bar_tile_y -= adjusted_tile_speed * dt;
		left_bar_tile_y2 -= adjusted_tile_speed * dt;
	}
	if (is_down(BUTTON_DOWN)) {
		left_bar_tile_y += adjusted_tile_speed * dt;
		left_bar_tile_y2 += adjusted_tile_speed * dt;
	}

	PixelCoordinate temp = calculate_top_left_pixel_coord_for_tile(left_bar_tile_x, left_bar_tile_y);
	PixelCoordinate temp2 = calculate_top_left_pixel_coord_for_tile(left_bar_tile_x2, left_bar_tile_y2);

	fill_between_pixel_coordinates(player_pixel_coordinate, { player_pixel_coordinate.x + 100, player_pixel_coordinate.y + 100 }, 0xFFFFF);
	fill_between_pixel_coordinates(temp, temp2); //left bar

	fill_between_tile_coordinates(tile_columns-3, 5, tile_columns-2, tile_rows - 5); //right bar

}