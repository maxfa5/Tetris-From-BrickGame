#ifndef TETRIS_H
#define TETRIS_H
#include <stdlib.h>

#define TET_TICK_START 70
enum { GAME_READY = 0, GAME_OVER, GAME_PAUSE };

typedef enum {
  Start,
  Pause = 10,
  Terminate = 27,
  Left = 0404,
  Right = 0405,
  Up = 0403,
  Down = 0402,
  Action
} UserAction_t;

typedef struct tetris_player {
  UserAction_t action;
} tetris_player;

typedef struct tetris_block {
  int is_life;
} tetris_block;

typedef struct game_field {
  int width;
  int height;
  tetris_block *blocks;
} game_field;

typedef struct falling_figure {
  int x;
  int y;
  int size;
  tetris_block *blocks;
} falling_figure;

typedef struct patterns_figures {
  int count;
  int size;
  tetris_block *block;

} patterns_figures;

/**
 * @brief STRUCT
 *
 *
 * @return STRUCT
 */
typedef struct {
  game_field *field;
  int next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
  patterns_figures *figures;
  falling_figure *figure;
  tetris_player *player;
  long double ticks_before_move;
  long double ticks;
  int error;
} GameInfo_t;

GameInfo_t *init_game(int field_width, int field_height, int figure_size,
                      int count, tetris_block *figures_template);

game_field *init_game_field(int field_height, int field_width);
void set_zero_game_field(game_field *field);
void set_start_game(GameInfo_t *game);
patterns_figures *init_figures_pattern(tetris_block *figures_template,
                                       int count, int figure_size);
void set_start_struct_info(GameInfo_t *game);
void free_game(GameInfo_t *game);

void free_game_field(game_field *field);

void free_figure(falling_figure *figure);
void free_figures_pattern(patterns_figures *patterns_figures);
void calc_tick(GameInfo_t *game, UserAction_t action, int hold);

int falling_process();

void move_figure_up(GameInfo_t *game);

void move_figure_down(GameInfo_t *game);

void move_figure_left(GameInfo_t *game);

void move_figure_right(GameInfo_t *game);
int check_collision(GameInfo_t *game);
void plant_figure(GameInfo_t *game);
void drop_line(game_field *tmp_field, int i);
int erase_line(GameInfo_t *game);
void drop_new_figure(GameInfo_t *game);
falling_figure *create_figure(GameInfo_t *game);
void free_figure(falling_figure *figure);

int line_is_field(game_field *tmp_field, int i);
falling_figure *rotate_figure(GameInfo_t *game);
void userInput(UserAction_t action, int hold);
void high_score_save(GameInfo_t *game);
void level_switch(GameInfo_t *game);

void updateCurrentState(UserAction_t action, int hold);

int get_score_from_saver();

/**
 * @brief get pointer of game
 *
 *
 * @return point of game struct
 */
GameInfo_t *get_ptr();
#endif