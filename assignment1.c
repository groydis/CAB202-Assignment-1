 #include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>
#include <curses.h>

#define DELAY 10 /* milliseconds */

#define AVATAR_WIDTH 3 
#define AVATAR_HEIGHT 3

#define GROUND_H 1
#define PLAT_H 1
#define PIT_H 1

#define LEVEL_EXIT_HEIGHT 4
#define LEVEL_EXIT_WIDTH 4

#define ZOMBIE_WIDTH 4
#define ZOMBIE_HEIGHT 4

#define BOULDER_WIDTH 4
#define BOULDER_HEIGHT 2

#define CAVE_BAT_WIDTH 3
#define CAVE_BAT_HEIGHT 2

#define BARRIER_WIDTH 2

bool game_over;
bool update_screen = true;

// Game Text
int score = 0;
int lives = 10;
int level = 1;

// FOR TIMER
int sec = 0;
int min = 0;
int delay_count = 0;
int new_level_time = 0;

// Player Movement
double movementSpeed = 0.25;
double maxSpeed = 0.5;
double jumpHeight = 0.7;
bool avatar_is_jumping;

// ENEMIES
double zombie_movementSpeed = 0.25;
// General
double gravity = 0.015;

double count;

bool hasKey;
bool collectedTreasure;

// SPRITES
char * avatar_image =
/**/	" o "
/**/	"/|\\"
/**/	"/ \\";

char * avatar_jump_image =
/**/	"\\o/"
/**/	" | "
/**/	"/ \\";

char * avatar_right_image =
/**/	"  o"
/**/	"/|\\"
/**/	"/ \\";

char * avatar_left_image =
/**/	"o  "
/**/	"/|\\"
/**/	"/ \\";


char * ground_img = 
/**/	"========================================================================================================================================================================================================";

char * level_exit_image =
/**/	"EXIT"
/**/	"|  |"
/**/	"| .|"
/**/	"|  |";

char * zombie_image =
/**/	"ZZZZ"
/**/	"  Z "
/**/	" Z  "
/**/	"ZZZZ";

char * boulder_image =
/**/	"/''\\"
/**/	"\\__/";

char * cave_bat_image_1 =
/**/	" | "
/**/	"/ \\";

char * cave_bat_image_2 =
/**/	"\\ /"
/**/	" | ";

char * barrier_image =
/**/	"[]"
/**/	"[]"
/**/	"[]"
/**/	"[]"
/**/	"[]"
/**/	"[]"
/**/	"[]"
/**/	"[]"
/**/	"[]"
/**/	"[]"
/**/	"[]"
/**/	"[]"
/**/	"[]"
/**/	"[]"
/**/	"[]"
/**/	"[]";

char * key_image =
/**/	"O-+";

char * treasure_image =
/**/	"$";

char * effect_image =
/**/	",";

Sprite * avatar;
Sprite * ground;
Sprite * ground_left;
Sprite * ground_right;

Sprite * platform;
Sprite * platform_2;
Sprite * platform_3;

Sprite * level_exit;

Sprite * zombie;
Sprite * boulder;
Sprite * cave_bat;

Sprite * barrier;

Sprite * treasure;
Sprite * effect;
Sprite * key;
Sprite * key_effect;

void clock() {
	delay_count++;
	if (delay_count == 100) {
		sec++;
		//new_level_time++;
		delay_count = 0;
		if (sec == 60) {
			sec = 0;
			min++;
		}
	}
}

void draw_boarder() {
	//Top Line
	draw_line( 0, 1, screen_width(), 1, '_' );
	// Left Side
	draw_line( 0, 2, 0, screen_height() - 2, '|' );
	// Right Side
	draw_line( screen_width() - 1, 2, screen_width() - 1, screen_height() - 2, '|' );
}

void draw_game_text() {
	draw_formatted(((screen_width() / 7) * 0), 0, " Time: %02d:%02d", min, sec);
	draw_formatted(((screen_width() / 7) * 2) - 7, 0, " Lives: %d ", lives);
	draw_formatted(((screen_width() / 7) * 4) - 7, 0, " Level: %d ", level);
	draw_formatted(((screen_width() / 7) * 6) - 7, 0, " Score: %d ", score);
}

void create_avatar() {
	int startX = 3;
	int startY = screen_height() - AVATAR_HEIGHT - 1;
	avatar = sprite_create(startX, startY, AVATAR_WIDTH, AVATAR_HEIGHT, avatar_image );
}

void create_zombie() {
	int startX = screen_width() - ZOMBIE_WIDTH - 1;
	int startY = screen_height() - ZOMBIE_HEIGHT - 1;
	zombie = sprite_create(startX, startY, ZOMBIE_WIDTH, ZOMBIE_HEIGHT, zombie_image);
}

void create_cave_bat() {
	int startX = screen_width() - CAVE_BAT_WIDTH - 1;
	int startY = screen_height() - CAVE_BAT_HEIGHT - 2;
	cave_bat = sprite_create(startX, startY, CAVE_BAT_WIDTH, CAVE_BAT_HEIGHT, cave_bat_image_1);
}

void create_boulder(int x_spawn, int y_spawn) {
	boulder = sprite_create(x_spawn, y_spawn, BOULDER_WIDTH, BOULDER_HEIGHT, boulder_image);
}

void create_barrier() {
	int startX = 0.60 * screen_width();
	int startY = screen_height() - (AVATAR_HEIGHT * 4);

	barrier = sprite_create(startX, startY, BARRIER_WIDTH, (AVATAR_HEIGHT * 4) - 1, barrier_image);
}

void destroy_level() {
	if (level == 1) {
		sprite_destroy(ground);
		sprite_destroy(platform);
		sprite_destroy(level_exit);
		sprite_destroy(avatar);

		sprite_destroy(zombie);
	}
	if (level == 2) {
		sprite_destroy(ground_left);
		sprite_destroy(ground_right);
		sprite_destroy(platform);
		sprite_destroy(level_exit);
		sprite_destroy(avatar);

		sprite_destroy(cave_bat);
		if (!collectedTreasure) {
			sprite_destroy(effect);
			sprite_destroy(treasure);
		}
	}
	if (level == 3) {
		sprite_destroy(ground_left);
		sprite_destroy(ground_right);
		sprite_destroy(platform);
		sprite_destroy(platform_2);
		sprite_destroy(level_exit);
		sprite_destroy(avatar);

		if (!collectedTreasure) {
			sprite_destroy(effect);
			sprite_destroy(treasure);
		}
	}
	if (level == 4) {
		sprite_destroy(ground_left);
		sprite_destroy(ground_right);
		sprite_destroy(platform);
		sprite_destroy(platform_2);
		sprite_destroy(platform_3);
		sprite_destroy(level_exit);
		sprite_destroy(avatar);

		sprite_destroy(boulder);

	}
	if (level == 5) {
		sprite_destroy(ground_left);
		sprite_destroy(ground_right);
		sprite_destroy(platform);
		sprite_destroy(platform_2);
		sprite_destroy(platform_3);
		sprite_destroy(level_exit);
		sprite_destroy(avatar);

		sprite_destroy(boulder);
		sprite_destroy(barrier);

		if (!collectedTreasure) {
			sprite_destroy(effect);
			sprite_destroy(treasure);
		}

		if(!hasKey) {
			sprite_destroy(key_effect);
			sprite_destroy(key);
		}
	}
}

void create_level() {
	if (level == 1) {
		ground = sprite_create(0, screen_height() - 1, screen_width(), GROUND_H, ground_img);
		platform = sprite_create((screen_width() / 2) - ((screen_width() / 3) / 2), screen_height() - (AVATAR_HEIGHT * 4) - 1, screen_width() / 3, PLAT_H, ground_img);
		level_exit = sprite_create(screen_width() - LEVEL_EXIT_WIDTH - 5, screen_height() - LEVEL_EXIT_HEIGHT - 1, LEVEL_EXIT_WIDTH, LEVEL_EXIT_HEIGHT, level_exit_image);
		create_avatar();
		create_zombie();
	}
	if (level == 2) {
		ground_left = sprite_create(0, screen_height() - 1, 0.25 * screen_width(), GROUND_H, ground_img);
		ground_right = sprite_create((screen_width() / 2) + 0.25 * screen_width(), screen_height() - 1, 0.25 * screen_width(), GROUND_H, ground_img);

		platform = sprite_create((screen_width() / 2) - ((screen_width() / 3) / 2), screen_height() - (AVATAR_HEIGHT * 4) - 1, screen_width() / 3, PLAT_H, ground_img);
		level_exit = sprite_create(screen_width() - LEVEL_EXIT_WIDTH - 5, screen_height() - LEVEL_EXIT_HEIGHT - 1, LEVEL_EXIT_WIDTH, LEVEL_EXIT_HEIGHT, level_exit_image);
		create_avatar();
		create_cave_bat();

		treasure = sprite_create(screen_width() / 2, screen_height() - (AVATAR_HEIGHT * 8) - 1, 1, 1, treasure_image);
		effect = sprite_create(screen_width() / 2, screen_height() - (AVATAR_HEIGHT * 8) - 1, 1, 1, effect_image);
		collectedTreasure = false;
	}
	if (level == 3) {
		ground_left = sprite_create(0, screen_height() - 1, 0.25 * screen_width(), GROUND_H, ground_img);
		ground_right = sprite_create((screen_width() / 2) + 0.25 * screen_width(), screen_height() - 1, 0.25 * screen_width(), GROUND_H, ground_img);

		platform = sprite_create((screen_width() / 2) - ((screen_width() / 3) / 2), screen_height() - (AVATAR_HEIGHT * 4) - 1, screen_width() / 3, PLAT_H, ground_img);
		platform_2 = sprite_create((screen_width() / 2) - 6, (screen_height() / 2) - 1, 12, PLAT_H, ground_img );
		
		level_exit = sprite_create((screen_width() / 2) - (LEVEL_EXIT_WIDTH / 2), (screen_height() / 2) - LEVEL_EXIT_HEIGHT - 1, LEVEL_EXIT_WIDTH, LEVEL_EXIT_HEIGHT, level_exit_image);
		create_avatar();

		treasure = sprite_create(screen_width() - 10, screen_height() - 5, 1, 1, treasure_image);
		effect = sprite_create(screen_width() - 10, screen_height() - 5, 1, 1, effect_image);
		
		collectedTreasure = false;
	}
	if (level == 4) {
		int gap_constant = 12;
		ground_left = sprite_create(0, screen_height() - 1, 0.20 * screen_width(), GROUND_H, ground_img);
		ground_right = sprite_create(0.20 * screen_width() + gap_constant, screen_height() - 1, 0.80 * screen_width() - gap_constant, GROUND_H, ground_img);

		platform = sprite_create(0, screen_height() - (AVATAR_HEIGHT * 4) -1, (screen_width() / 2) + ((screen_width() / 2) / 2), PLAT_H, ground_img);
		platform_2 = sprite_create((screen_width() / 2) - 0.25 * screen_width(), screen_height() - (AVATAR_HEIGHT * 8) - 1, (screen_width() / 2) + 0.25 * screen_width(), PLAT_H, ground_img);
		platform_3 = sprite_create(0, screen_height() - (AVATAR_HEIGHT * 12) - 1, (screen_width() / 2) + ((screen_width() / 2) / 2), PLAT_H, ground_img);

		level_exit = sprite_create(4, (screen_height() - (AVATAR_HEIGHT * 12)) - LEVEL_EXIT_HEIGHT - 1, LEVEL_EXIT_WIDTH, LEVEL_EXIT_HEIGHT, level_exit_image);
		create_avatar();

		create_boulder(0.20 * screen_width(), 3);
	}
	if (level == 5) {
		int gap_constant = 12;
		ground_left = sprite_create(0, screen_height() - 1, 0.70 * screen_width(), GROUND_H, ground_img);
		ground_right = sprite_create(0.70 * screen_width() + gap_constant, screen_height() - 1, 0.30 * screen_width() - gap_constant, GROUND_H, ground_img);
	
		platform = sprite_create((screen_width() / 2) - 0.25 * screen_width(), screen_height() - (AVATAR_HEIGHT * 4) -1, (screen_width() / 2) + 0.25 * screen_width(), PLAT_H, ground_img);
		platform_2 = sprite_create((screen_width() / 2) - 0.25 * screen_width(), screen_height() - (AVATAR_HEIGHT * 8) - 1, (screen_width() / 2) + 0.25 * screen_width(), PLAT_H, ground_img);
		platform_3 = sprite_create((screen_width() / 2) - 0.25 * screen_width(), screen_height() - (AVATAR_HEIGHT * 12) - 1, (screen_width() / 2) + 0.25 * screen_width(), PLAT_H, ground_img);

		level_exit = sprite_create(screen_width() - LEVEL_EXIT_WIDTH - 5, screen_height() - LEVEL_EXIT_HEIGHT - 1, LEVEL_EXIT_WIDTH, LEVEL_EXIT_HEIGHT, level_exit_image);
		create_avatar();

		create_boulder(0.75 * screen_width(), 3);
		create_barrier();

		treasure = sprite_create(screen_width() / 2, screen_height() - (AVATAR_HEIGHT * 11), 1, 1, treasure_image);
		effect = sprite_create(screen_width() / 2, screen_height() - (AVATAR_HEIGHT * 11), 1, 1, effect_image);

		collectedTreasure = false;

		key = sprite_create(screen_width() - 10, screen_height() - (AVATAR_HEIGHT * 15), 3, 1, key_image);
		key_effect = sprite_create(screen_width() - 10, screen_height() - (AVATAR_HEIGHT * 15), 1, 1, effect_image);

		hasKey = false;
	}
}

void draw_level() {
	if (level == 1) {
		sprite_draw(ground);
		sprite_draw(platform);
		sprite_draw(level_exit);

		sprite_draw(zombie);
	}
	if (level == 2) {
		sprite_draw(ground_left);
		sprite_draw(ground_right);
		sprite_draw(platform);
		sprite_draw(level_exit);

		sprite_draw(cave_bat);

		if (!collectedTreasure) {
			sprite_draw(treasure);
		}
		if (!collectedTreasure) {
			sprite_draw(effect);
		}
	}
	if (level == 3) {
		sprite_draw(ground_left);
		sprite_draw(ground_right);
		sprite_draw(platform);
		sprite_draw(platform_2);
		sprite_draw(level_exit);

		sprite_draw(treasure);
		sprite_draw(effect);

	}
	if (level == 4) {
		sprite_draw(ground_left);
		sprite_draw(ground_right);
		sprite_draw(platform);
		sprite_draw(platform_2);
		sprite_draw(platform_3);
		sprite_draw(level_exit);

		sprite_draw(boulder);

	}
	if (level == 5) {
		sprite_draw(ground_left);
		sprite_draw(ground_right);
		sprite_draw(platform);
		sprite_draw(platform_2);
		sprite_draw(platform_3);
		sprite_draw(level_exit);

		sprite_draw(boulder);
		sprite_draw(barrier);

		if (!collectedTreasure) {
			sprite_draw(treasure);
		}
		if (!collectedTreasure) {
			sprite_draw(effect);
		}

		if (!hasKey) {
			sprite_draw(key);
		}
		if (!hasKey) {
			sprite_draw(key_effect);
		}

	}
}

void show_gameover_screen() {
	clear_screen();
	if (lives > 0) {
	draw_string((screen_width() / 2) - 8, (screen_height() / 2) - 2, "Congratulations!");
	draw_formatted((screen_width() / 2) - 12, screen_height() / 2, "Your final score was %d ", score);
	draw_string((screen_width() / 2) - 11, (screen_height() / 2) + 2, "Press any key to exit");	

	} else {
		draw_string((screen_width() / 2) - 5, (screen_height() / 2) - 1, "Game Over");
		draw_string((screen_width() / 2) - 11, (screen_height() / 2) + 2, "Press any key to exit");		
	}
	show_screen();
	game_over = true;
	wait_char();
	return;
}

void avatar_death() {
	destroy_level();
	lives--;
	if (lives == 0) {
		show_gameover_screen();
	}
	avatar_is_jumping = false;
	create_level();
}

bool wall_collision(sprite_id colliding_sprite) {
	int colliding_sprite_x_position = sprite_x(colliding_sprite);
	int colliding_sprite_w = sprite_width(colliding_sprite);

	if (colliding_sprite_x_position < 1 || colliding_sprite_x_position > (screen_width() - 2) - colliding_sprite_w) {
		return true;
	}
	return false;
}

bool collision(sprite_id spriteOne, sprite_id spriteTwo) {
	int spriteOne_x = round(sprite_x(spriteOne));
	int spriteOne_y = round(sprite_y(spriteOne));

	int spriteTwo_x = round(sprite_x(spriteTwo));
	int spriteTwo_y = round(sprite_y(spriteTwo));

	int spriteOne_w = sprite_width(spriteOne);
	int spriteOne_h = sprite_height(spriteOne);

	int spriteTwo_w = sprite_width(spriteTwo);
	int spriteTwo_h = sprite_height(spriteTwo);

	bool collided = true;

	if ( spriteTwo_x >= spriteOne_x + spriteOne_w ) {
		collided = false; 
	}
	if ( spriteTwo_y >= spriteOne_y + spriteOne_h  ) {
		collided = false;
	}
	if ( spriteOne_x >= spriteTwo_x + spriteTwo_w ) {
		collided = false;
	}
	if ( spriteOne_y >= spriteTwo_y + spriteTwo_h ) {
		collided = false;
	}

	return collided;
}

bool ground_collision(sprite_id colliding_sprite, sprite_id ground_sprite) {
	int colliding_sprite_x_position = round(sprite_x(colliding_sprite));
	int colliding_sprite_y_position = round(sprite_y(colliding_sprite));
	int colliding_sprite_w = sprite_width(colliding_sprite);

	int colliding_sprite_height = sprite_height(colliding_sprite);

	int ground_sprite_x = round(sprite_x(ground_sprite));
	int ground_sprite_y = round(sprite_y(ground_sprite));

	int ground_width = sprite_width(ground_sprite);

	if (colliding_sprite_x_position + colliding_sprite_w > ground_sprite_x) {
		if (colliding_sprite_x_position < (ground_sprite_x + ground_width) + 1) {
			if (colliding_sprite_y_position + colliding_sprite_height == ground_sprite_y) {
				return true;
			}
		}
	}
	return false;
}

bool platform_collision(sprite_id colliding_sprite, sprite_id platform_sprite) {
	int colliding_sprite_x_position = round(sprite_x(colliding_sprite));
	int colliding_sprite_y_position = round(sprite_y(colliding_sprite));

	int colliding_sprite_w = sprite_width(colliding_sprite);

	int colliding_sprite_height = sprite_height(colliding_sprite);

	int platform_sprite_x = round(sprite_x(platform_sprite));
	int platform_sprite_y = round(sprite_y(platform_sprite));

	int platform_width = sprite_width(platform_sprite);

	if (colliding_sprite_x_position + colliding_sprite_w > platform_sprite_x) {
		if (colliding_sprite_x_position < (platform_sprite_x + platform_width) + 1) {
			if (colliding_sprite_y_position + colliding_sprite_height == platform_sprite_y) {
				return true;
			}
		}
	}
	return false;
}

bool platform_collision_bottom(sprite_id colliding_sprite, sprite_id platform_sprite) {
	int colliding_sprite_x_position = round(sprite_x(colliding_sprite));
	int colliding_sprite_y_position = round(sprite_y(colliding_sprite));
	int colliding_sprite_w = sprite_width(colliding_sprite);

	//int colliding_sprite_height = sprite_height(colliding_sprite);

	int platform_sprite_x = round(sprite_x(platform_sprite));
	int platform_sprite_y = round(sprite_y(platform_sprite));

	int platform_width = sprite_width(platform_sprite);

	if (colliding_sprite_x_position + colliding_sprite_w > platform_sprite_x) {
		if (colliding_sprite_x_position < platform_sprite_x + platform_width) {
			if (colliding_sprite_y_position == platform_sprite_y + 1) {
				return true;
			}
		}
	}
	return false;
}

bool platform_collision_sides(sprite_id colliding_sprite, sprite_id platform_sprite) {
	int colliding_sprite_x_position = round(sprite_x(colliding_sprite));
	int colliding_sprite_y_position = round(sprite_y(colliding_sprite));

	int colliding_sprite_dx = sprite_dx(colliding_sprite);

	int colliding_sprite_w = sprite_width(colliding_sprite);

	int colliding_sprite_h = sprite_height(colliding_sprite);

	int platform_sprite_x = round(sprite_x(platform_sprite));
	int platform_sprite_y = round(sprite_y(platform_sprite));

	int platform_w = sprite_width(platform_sprite);

	if (avatar_is_jumping) {
		if (colliding_sprite_dx > 0) {
			if (colliding_sprite_x_position + colliding_sprite_w == platform_sprite_x) {
				if (colliding_sprite_y_position <= platform_sprite_y 
					&& colliding_sprite_y_position + colliding_sprite_h >= platform_sprite_y) {
					return true;
				}
			}
		}

		if (colliding_sprite_x_position == platform_sprite_x + platform_w) {
			if (colliding_sprite_y_position <= platform_sprite_y 
				&& colliding_sprite_y_position + colliding_sprite_h >= platform_sprite_y) {
				return true;
			}
		}
			
	}

	return false;
}

bool roof_collision(sprite_id colliding_sprite) {
	int colliding_sprite_y_position = round(sprite_y(colliding_sprite));

	if (colliding_sprite_y_position < 3) {
		return true;
	}
	return false;
}

void move_platform(sprite_id m_platform) {
	int m_platform_x_position = sprite_x(m_platform);
	double m_platform_dx = sprite_dx(m_platform);
	if (wall_collision(m_platform)) {
		if (m_platform_x_position + sprite_width(m_platform) > screen_width() - 2) {
			m_platform_dx = 0.0;
			m_platform_dx += -movementSpeed;
		} else if (m_platform_x_position <= 1) {
			m_platform_dx = 0.0;
			m_platform_dx += movementSpeed;
		}
	} 
	if (m_platform_dx == 0) {
		m_platform_dx += -movementSpeed;
	}
	sprite_turn_to(m_platform, m_platform_dx, sprite_dy(m_platform));
	sprite_move(m_platform, m_platform_dx, sprite_dy(m_platform));
}

void zombie_movement() {
	int zombie_x_position = sprite_x(zombie);
	double zombie_dx = sprite_dx(zombie);
	if (wall_collision(zombie)) {
		if (zombie_x_position > screen_width() / 2) {
			zombie_dx += -zombie_movementSpeed;
		} else {
			zombie_dx += zombie_movementSpeed;
		}
	}
	sprite_turn_to(zombie, zombie_dx, sprite_dy(zombie));
	sprite_move(zombie, zombie_dx, sprite_dy(zombie));
}

void cave_bat_movement() {
	int bat_x_position = sprite_x(cave_bat);
	double bat_dx = sprite_dx(cave_bat);
	if (wall_collision(cave_bat)) {
		if (bat_x_position > screen_width() / 2) {
			bat_dx += -0.75;
		} else {
			bat_dx += 0.75;
		}
	}
	sprite_turn_to(cave_bat, bat_dx, sprite_dy(cave_bat));
	sprite_move(cave_bat, bat_dx, sprite_dy(cave_bat));
}

void move_boulder() {
	double boulder_dx = sprite_dx(boulder);
	double boulder_dy = sprite_dy(boulder);
	int boulder_x = sprite_x(boulder);
	int boulder_y = sprite_y(boulder);
	int boulder_width = sprite_width(boulder);
	if (boulder_y > screen_height() ) {
		sprite_destroy(boulder);
		if (level == 4) {
			create_boulder(0.20 * screen_width(), 3);
		}
		if (level == 5) {
			create_boulder(0.75 * screen_width(), 3);
		}
	}

	if (wall_collision(boulder)) {
		if (boulder_x + boulder_width > screen_width() - 2) {
			boulder_dx = 0.0;
			boulder_dx += -0.665;
		} else if (boulder_x <= 1) {
			boulder_dx = 0.0;
			boulder_dx += 0.665;
		}
	}
	if (platform_collision(boulder, platform_3) 
		|| platform_collision(boulder, platform_2) 
		|| platform_collision(boulder, platform)
		|| ground_collision(boulder, ground_left)
		|| ground_collision(boulder, ground_right)) {
		boulder_dy = 0.0;
	} else {
		boulder_dy += gravity;
	}
	if (boulder_dx == 0.0) {
		boulder_dx += 0.665;
	}
	sprite_turn_to(boulder, boulder_dx, boulder_dy);
	sprite_move(boulder, boulder_dx, boulder_dy);
}

void collect_treasure() {
	if (treasure != NULL) {
		if (collision(avatar, treasure)) {
			if (!collectedTreasure) {
				score = score + 50;
				collectedTreasure = true;
				sprite_destroy(treasure);
				sprite_destroy(effect);
			}
		}
	}
}

void collect_key() {
	if (key != NULL) {
		if (collision(avatar, key)) {
			if (!hasKey) {
				//score = score + 50;
				hasKey = true;
				sprite_destroy(key);
				sprite_destroy(key_effect);
			}
		}
	}
}

void check_lives() {
	if (lives < 1) {
		show_gameover_screen();
	}
}

void animate_cave_bat() {
	if (count < 0.75) {
		sprite_set_image(cave_bat, cave_bat_image_1);
		count = count + 0.4;
	} else {
		sprite_set_image(cave_bat, cave_bat_image_2);
		count = 0.0;
	}
}

void animate_treasure_effect() {
	if (effect != NULL) {
		int treasure_x = round(sprite_x(treasure));
		int treasure_y = round(sprite_y(treasure));

		double new_x = treasure_x + rand() % 4-2;
		double new_y = treasure_y + rand() % 4-2;

		sprite_move_to(effect, new_x, new_y);
	}
}

void animate_key_effect() {
	if (effect != NULL) {
		int key_x = round(sprite_x(key));
		int key_y = round(sprite_y(key));

		double new_x = key_x + rand() % 8-2;
		double new_y = key_y + rand() % 4-2;

		sprite_move_to(key_effect, new_x, new_y);
	}
}

void animate_avatar() {
	double avatar_dx = sprite_dx(avatar);

	if (avatar_is_jumping) {
    	sprite_set_image(avatar, avatar_jump_image);
    } else {
    	if (avatar_dx == 0.0) {
    		sprite_set_image(avatar, avatar_image);
    	}
    	if (avatar_dx > 0) {
    		sprite_set_image(avatar, avatar_right_image);
    	}
    	if (avatar_dx < 0) {
    		sprite_set_image(avatar, avatar_left_image);
    	}
    }
}

void avatar_movement() {
	int avatar_y = sprite_y(avatar);
	double avatar_dx = sprite_dx(avatar);
	double avatar_dy = sprite_dy(avatar);

	int key = get_char();

	if ( level == 1) {
		if (ground_collision(avatar, ground) || platform_collision(avatar, platform)) {
			avatar_is_jumping = false;
		} else {
			avatar_is_jumping = true;
		}
		if (platform_collision_bottom(avatar, platform)) {
			avatar_dy = 0.0;
			avatar_dy += gravity;
		}
		if (platform_collision_sides(avatar, platform)) {
			avatar_dx = 0.0;
			avatar_dy = 0.3;
			avatar_dy += gravity;
		}
		if (collision(avatar, zombie)) {
			avatar_dy = 0.0;
			avatar_dx = 0.0;
			avatar_death();
		}
	}
	if (level == 2) {
		if (ground_collision(avatar, ground_left) 
			|| ground_collision(avatar, ground_right)
			|| platform_collision(avatar, platform)) {
			avatar_is_jumping = false;
		} else {
			avatar_is_jumping = true;
		}
		if (platform_collision_bottom(avatar, platform)) {
			avatar_dy = 0.0;
			avatar_dy += gravity;
		}
		if (platform_collision_sides(avatar, platform)) {
			avatar_dx = 0.0;
			avatar_dy = 0.3;
			avatar_dy += gravity;
		}
		if (collision(avatar, cave_bat)) {
			avatar_dy = 0.0;
			avatar_dx = 0.0;
			avatar_death();
		}

	}
	if (level == 3) {
		if (ground_collision(avatar, ground_left) 
			|| ground_collision(avatar, ground_right)
			|| platform_collision(avatar, platform)
			|| platform_collision(avatar, platform_2)) {
			avatar_is_jumping = false;
		} else {
			avatar_is_jumping = true;
		}
		if (platform_collision_bottom(avatar, platform) || platform_collision_bottom(avatar, platform_2)) {
			avatar_dy = 0.0;
			avatar_dy += gravity;
		}
		if (platform_collision_sides(avatar, platform)) {
			avatar_dx = 0.0;
			avatar_dy = 0.3;
			avatar_dy += gravity;
		}
		if (platform_collision_sides(avatar, platform_2)) {
			avatar_dx = 0.0;
			avatar_dy += gravity;
		}
	}
	if (level == 4) {
		if (ground_collision(avatar, ground_left) 
			|| ground_collision(avatar, ground_right)
			|| platform_collision(avatar, platform)
			|| platform_collision(avatar, platform_2)
			|| platform_collision(avatar, platform_3)) {
			avatar_is_jumping = false;
		} else {
			avatar_is_jumping = true;
		}
		if (platform_collision_bottom(avatar, platform) || platform_collision_bottom(avatar, platform_2) || platform_collision_bottom(avatar, platform_3)) {
			avatar_dy = 0.0;
			avatar_dy += gravity;
		}
		if (collision(avatar, boulder)) {
			avatar_dy = 0.0;
			avatar_dx = 0.0;
			avatar_death();
		}
		if (platform_collision_sides(avatar, platform)) {
			avatar_dx = 0.0;
			avatar_dy = 0.3;
			avatar_dy += gravity;
		}
		if (platform_collision_sides(avatar, platform_2)) {
			avatar_dx = 0.0;
			avatar_dy += gravity;
		}
		if (platform_collision_sides(avatar, platform_3)) {
			avatar_dx = 0.0;
			avatar_dy += gravity;
		}
	}
	if (level == 5) {
		if (ground_collision(avatar, ground_left) 
			|| ground_collision(avatar, ground_right)
			|| platform_collision(avatar, platform)
			|| platform_collision(avatar, platform_2)
			|| platform_collision(avatar, platform_3)) {
			avatar_is_jumping = false;
		} else {
			avatar_is_jumping = true;
		}
		if (platform_collision_bottom(avatar, platform) || platform_collision_bottom(avatar, platform_2) || platform_collision_bottom(avatar, platform_3)) {
			avatar_dy = 0.0;
			avatar_dy += gravity;
		}
		if (platform_collision_sides(avatar, platform)) {
			avatar_dx = 0.0;
			avatar_dy = 0.3;
			avatar_dy += gravity;
		}
		if (platform_collision_sides(avatar, platform_2)) {
			avatar_dx = 0.0;
			avatar_dy += gravity;
		}
		if (platform_collision_sides(avatar, platform_3)) {
			avatar_dx = 0.0;
			avatar_dy += gravity;
		}

		if (collision(avatar, boulder)) {
			avatar_dy = 0.0;
			avatar_dx = 0.0;
			avatar_death();
		}

		if (collision(avatar, barrier)) {
			if (hasKey) {
				sprite_hide(barrier);
			} else {
				avatar_dx = 0.0;
				sprite_back(avatar);
			}
		}
	}

	if (roof_collision(avatar)) {
		avatar_dy = 0.0;
	} 


	if (avatar_is_jumping) {
		avatar_dy += gravity;
	} else {
		avatar_dy = 0.0;
	}

	if (key == KEY_LEFT && !avatar_is_jumping) {
		// Check that speed is not maximum and increase
		if (avatar_dx > -maxSpeed) {
			avatar_dx += -movementSpeed;
		}
	}

	if (key == KEY_RIGHT && !avatar_is_jumping) {
		// Check that speed is not maximum and increase
		if (avatar_dx < maxSpeed) {
			avatar_dx += movementSpeed;
		}
	}

	if (key == KEY_UP && !avatar_is_jumping) {
		// TODO: Jump
		avatar_dy -= jumpHeight;
		avatar_is_jumping = true;
	}

	if (key == 'l' || key == 'L') {
		destroy_level();
		level++;
		avatar_dy = 0.0;
		avatar_dx = 0.0;
		avatar_is_jumping = false;
		create_level();
	}
	if (collision(avatar, level_exit)) {
		destroy_level();
		level++;
		score = score + 100;
		avatar_dy = 0.0;
		avatar_dx = 0.0;
		avatar_is_jumping = false;
		create_level();
	}

	if (key == 'q' || key == 'Q') {
		// TODO: Quit Game
	}

	if (wall_collision(avatar)) {
		avatar_dx = 0.0;
		sprite_back(avatar);
	}

	if (avatar_y > screen_height() - 3) {
		avatar_dy = 0.0;
		avatar_dx = 0.0;
		avatar_is_jumping = false;
		create_level();
	}

	sprite_turn_to(avatar, avatar_dx, avatar_dy);
	sprite_move(avatar, avatar_dx, avatar_dy);
}

void process(void) {
    // Keep the next line intact.
    clear_screen();

   	check_lives();

    if (level == 1) {
   		zombie_movement();
   	}

   	if (level == 2) {
   		animate_cave_bat();
		cave_bat_movement();
		collect_treasure();
		if (!collectedTreasure) {
			animate_treasure_effect();
		}
   	}

    if (level == 3) {
    	move_platform(platform);
    	collect_treasure();
    	if (!collectedTreasure) {
			animate_treasure_effect();
		}
    }
    if (level == 4) {
    	move_boulder();
    }
    if (level == 5) {
    	move_boulder();
    	move_platform(platform_2);
    	collect_treasure();
    	if (!collectedTreasure) {
			animate_treasure_effect();
		}
		collect_key();
		if(!hasKey) {
    		animate_key_effect();
    	}
    }
    if (level > 5) {
    	show_gameover_screen();
    }

    draw_game_text();
    draw_level();
    draw_boarder();
    avatar_movement();
    animate_avatar();
    sprite_draw(avatar);
}

void setup(void) {
	override_screen_size( 120, 50 );
	create_level();
	create_avatar();
	show_screen();
}


int main(void) {
    setup_screen();

#if defined(AUTO_SAVE_SCREEN)
    auto_save_screen(true);
#endif

    setup();
    show_screen();

    while ( !game_over ) {
        process();
        clock();
        if ( update_screen ) {
            show_screen();
        }
        timer_pause(DELAY);
    }

    return 0;
}