#include <cstdlib> // For rand() and srand()
#include <ctime>   // For seeding rand()
#include <cmath>   // For sqrt and round

class Ball {
public:
    PixelCoordinate position;
    float velocity_x;
    float velocity_y;
    float speed;
    const float speed_increment = 20.0f; // Speed increase on each bounce

    Ball(PixelCoordinate start_position, float initial_speed)
        : position(start_position), speed(initial_speed) {
        velocity_x = generate_random();
        velocity_y = generate_random();
    }

    void update(float dt) {
        float velocity_magnitude = std::sqrt(velocity_x * velocity_x + velocity_y * velocity_y);
        velocity_x = (velocity_x / velocity_magnitude) * speed;
        velocity_y = (velocity_y / velocity_magnitude) * speed;

        position.x += static_cast<int>(std::round(velocity_x * dt));
        position.y += static_cast<int>(std::round(velocity_y * dt));
    }

    void bounce_vertical() {
        velocity_y *= -1;
        increase_speed();
    }

    void bounce_horizontal() {
        velocity_x *= -1;
        increase_speed();
    }

    bool is_out_of_bounds(const PixelCoordinate& arena_top_left, const PixelCoordinate& arena_bottom_right) const {
        return position.x <= arena_top_left.x || position.x + static_cast<int>(tile_width) >= arena_bottom_right.x;
    }

    bool is_hitting_wall(const PixelCoordinate& arena_top_left, const PixelCoordinate& arena_bottom_right) const {
        return position.y <= arena_top_left.y || position.y + static_cast<int>(tile_height) >= arena_bottom_right.y;
    }

    PixelCoordinate get_top_left() const {
        return position;
    }

    PixelCoordinate get_bottom_right() const {
        return { position.x + static_cast<int>(tile_width), position.y + static_cast<int>(tile_height) };
    }

    void draw() const {
        fill_between_pixel_coordinates(get_top_left(), get_bottom_right(), 0xFFFFF);
    }

private:
    void increase_speed() {
        speed += speed_increment;
    }

    float generate_random() {
        float direction = (rand() % 2) * 2 - 1;
        return direction * (rand() % 301 + 500);
    }
};

class Bar {
public:
    float tile_x;
    float tile_y;
    float tile_x2;
    float tile_y2;
    uint32_t color;

    Bar(float x, float y, float width, float height, uint32_t bar_color = 0xFFFFFF)
        : tile_x(x), tile_y(y), tile_x2(x + width), tile_y2(y + height), color(bar_color) {}

    void move_up(float dt, float speed, float min_tile_y) {
        if (tile_y > min_tile_y) {
            tile_y -= speed * dt;
            tile_y2 -= speed * dt;
        }
    }

    void move_down(float dt, float speed, float max_tile_y) {
        if (tile_y2 < max_tile_y) {
            tile_y += speed * dt;
            tile_y2 += speed * dt;
        }
    }

    PixelCoordinate get_top_left() const {
        return calculate_top_left_pixel_coord_for_tile(tile_x, tile_y);
    }

    PixelCoordinate get_bottom_right() const {
        return calculate_top_left_pixel_coord_for_tile(tile_x2, tile_y2);
    }

    bool check_collision(const Ball& ball) const {
        PixelCoordinate ball_top_left = ball.get_top_left();
        PixelCoordinate ball_bottom_right = ball.get_bottom_right();
        PixelCoordinate bar_top_left = get_top_left();
        PixelCoordinate bar_bottom_right = get_bottom_right();

        return !(ball_top_left.x > bar_bottom_right.x || ball_bottom_right.x < bar_top_left.x ||
            ball_top_left.y > bar_bottom_right.y || ball_bottom_right.y < bar_top_left.y);
    }

    void draw() const {
        fill_between_pixel_coordinates(get_top_left(), get_bottom_right(), color);
    }
};

class Arena {
public:
    TileCoordinate top_left;
    TileCoordinate bottom_right;

    Arena(int x, int y, int x2, int y2) {
        top_left = { x, y };
        bottom_right = { x2, y2 };
    }

    void draw() const {
        fill_between_tile_coordinates(top_left.x, top_left.y, bottom_right.x, bottom_right.y, 0xBBBBBB);
    }
};

class GameState {
public:
    Arena arena;
    Ball ball;
    Bar left_bar;
    Bar right_bar;

    GameState()
        : arena(1, 1, tile_columns - 1, tile_rows - 1),
        ball({ 1000, 500 }, 600.0f),
        left_bar(2, 10, 1, tile_rows - 20),
        right_bar(tile_columns - 3, 10, 1, tile_rows - 20) {}
};

void simulate_game(Input* input, float dt, GameState& game_state) {
    game_state.ball.update(dt);

    PixelCoordinate arena_pixel_top_left = calculate_top_left_pixel_coord_for_tile(1, 1);
    PixelCoordinate arena_pixel_bottom_right = calculate_top_left_pixel_coord_for_tile(tile_columns - 1, tile_rows - 1);

    if (game_state.ball.is_hitting_wall(arena_pixel_top_left, arena_pixel_bottom_right)) {
        game_state.ball.bounce_vertical();
    }

    if (game_state.ball.is_out_of_bounds(arena_pixel_top_left, arena_pixel_bottom_right)) {
        game_state.ball.velocity_x = 0;
        game_state.ball.velocity_y = 0;
    }

    if (game_state.left_bar.check_collision(game_state.ball) || game_state.right_bar.check_collision(game_state.ball)) {
        game_state.ball.bounce_horizontal();
    }

    const float bar_speed = 60.0f;
    if (is_down(W_KEY)) {
        game_state.left_bar.move_up(dt, bar_speed, game_state.arena.top_left.y);
    }
    if (is_down(S_KEY)) {
        game_state.left_bar.move_down(dt, bar_speed, game_state.arena.bottom_right.y);
    }
    if (is_down(BUTTON_UP)) {
        game_state.right_bar.move_up(dt, bar_speed, game_state.arena.top_left.y);
    }
    if (is_down(BUTTON_DOWN)) {
        game_state.right_bar.move_down(dt, bar_speed, game_state.arena.bottom_right.y);
    }
}

void render_game(const GameState& game_state) {
    clear_screen(0xff5500);
    draw_grid();
    game_state.arena.draw();
    game_state.ball.draw();
    game_state.left_bar.draw();
    game_state.right_bar.draw();
}
