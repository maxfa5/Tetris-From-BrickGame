/**
 * @file tetris.c
 * @author sandajer
 * @brief
 * @version 0.1
 * @date 2024-06-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "tetris.h"

#include <stdio.h>
#include <stdlib.h>
#ifndef TETRIS_C
#define TETRIS_C
/**
 * @brief get pointer of game
 *
 *
 * @return point of game struct
 */
GameInfo_t *get_ptr() {
  static GameInfo_t game_info_1;
  return &game_info_1;
}

/**
 * @brief initializing main game struct and set zero for stats
 *
 * @param field_width
 * @param field_height
 * @param figure_size
 * @param count
 * @param figures_template
 * @return GameInfo_t*
 */
GameInfo_t *init_game(int field_width, int field_height, int figure_size,
                      int count, tetris_block *figures_template) {
  GameInfo_t *game = get_ptr();
  game->field = init_game_field(field_height, field_width);
  game->figures = init_figures_pattern(figures_template, count, figure_size);
  set_start_struct_info(game);
  if (!game->field || !game->figures) {
    printf("\nerror of memory, try again later \n");
    game->error = 1;
  }
  return game;
}

/**
 * @brief initializing field
 *
 * @param field_height
 * @param field_width
 * @return game_field*
 */
game_field *init_game_field(int field_height, int field_width) {
  game_field *field = malloc(sizeof(game_field));
  if (field) {
    field->height = field_height;
    field->width = field_width;
    field->blocks = malloc(sizeof(tetris_block) * field_height * field_width);
    if (field->blocks) set_zero_game_field(field);
  }
  return field;
}

/**
 * @brief initializing patterns of figures
 *
 * @param figures_template
 * @param count
 * @param figures_size
 * @return patterns_figures*
 */
patterns_figures *init_figures_pattern(tetris_block *figures_template,
                                       int count, int figures_size) {
  patterns_figures *patterns = malloc(sizeof(patterns_figures));
  if (patterns) {
    patterns->count = count;
    patterns->size = figures_size;
    patterns->block = figures_template;
  }
  return patterns;
}

/**
 * @brief
 *
 * @param game
 */
void free_game(GameInfo_t *game) {
  if (game) {
    free_figures_pattern(game->figures);
    free_game_field(game->field);
    free_figure(game->figure);
  }
}

/**
 * @brief
 *
 * @param game
 */
void set_start_struct_info(GameInfo_t *game) {
  game->ticks_before_move = TET_TICK_START;
  game->score = 0;
  game->next = -1;
  game->high_score = get_score_from_saver();
  game->speed = 2;
  game->level = 1;
  game->error = 0;
  game->pause = GAME_READY;
  game->ticks = TET_TICK_START;
}

/**
 * @brief
 *
 * @param game
 */
void set_start_game(GameInfo_t *game) {
  if (game) {
    set_zero_game_field(game->field);
    set_start_struct_info(game);
    if (!game->field || !game->figures) {
      printf("\nerror of memory, try again later \n");
      game->error = 1;
    }
  }
}

/**
 * @brief
 *
 * @param field
 */
void free_game_field(game_field *field) {
  if (field) {
    if (field->blocks) {
      free(field->blocks);
      field->blocks = NULL;
    }
    free(field);
    field = NULL;
  }
}

/**
 * @brief set zero for all field
 *
 * @param field
 */
void set_zero_game_field(game_field *field) {
  if (field) {
    if (field->blocks) {
      int field_height = field->height, field_width = field->width;
      for (int i = 0; i < field_height * field_width && field->blocks; i++) {
        field->blocks[i].is_life = 0;
      }
    }
  }
}
/**
 * @brief
 *
 * @param patterns_figures
 */
void free_figures_pattern(patterns_figures *patterns_figures) {
  if (patterns_figures) {
    free(patterns_figures);
    patterns_figures = NULL;
  }
}

/**
 * @brief cycle of game
 * cycle for main process
 * when tick is low this function is start falling
 * @param game
 * @param action
 * @param hold
 */
void calc_tick(GameInfo_t *game, UserAction_t action, int hold) {
  hold = 0;
  int flag_game_stop = 0;
  if (game->ticks <= 0 && !game->pause) {
    game->ticks = game->ticks_before_move;
    flag_game_stop = falling_process(game);
  }
  if (action == Pause || action == Terminate) {
    updateCurrentState(action, hold);
  } else if (!flag_game_stop && !game->pause) {
    level_switch(game);
    high_score_save(game);
    updateCurrentState(action, hold);
    game->ticks--;
  }
}

void userInput(UserAction_t action, int hold) {
  GameInfo_t *game = get_ptr();
  hold = 0;
  hold++;
  switch (action) {
    case Up:
      action = Up;
      break;
    case Down:
      action = Down;
      break;
    case Left:
      action = Left;
      break;
    case Right:
      action = Right;
      break;
    case Pause:
      action = Pause;
      break;
    case Terminate:
      action = Terminate;
    default:
      break;
  }
  calc_tick(game, action, 0);
}

/**
 * @brief falling and check fall or place figure
 *
 * @return int
 */
int falling_process() {
  GameInfo_t *game = get_ptr();
  move_figure_down(game);
  if (check_collision(game)) {
    move_figure_up(game);
    plant_figure(game);
    game->score += erase_line(game);
    high_score_save(game);
    drop_new_figure(game);
    if (check_collision(game)) {
      game->pause = GAME_OVER;
      return 1;
    }
  }
  return 0;
}

/**
 * @brief
 *
 * @param game
 */
void move_figure_up(GameInfo_t *game) { game->figure->y--; }

/**
 * @brief
 *
 * @param game
 */
void move_figure_down(GameInfo_t *game) { game->figure->y++; }

/**
 * @brief
 *
 * @param game
 */
void move_figure_left(GameInfo_t *game) { game->figure->x--; }

/**
 * @brief
 *
 * @param game
 */
void move_figure_right(GameInfo_t *game) { game->figure->x++; }

/**
 * @brief check field for collision
 * check field for collision falling figure from border, and old figures
 *
 * @param game
 * @return int
 */
int check_collision(GameInfo_t *game) {
  int coord_x_figure = 0, coord_y_figure = 0, result = 0;
  falling_figure *tmp_figure = game->figure;
  game_field *tmp_field = game->field;
  for (int i = 0; i < tmp_figure->size; i++) {
    for (int j = 0; j < tmp_figure->size; j++) {
      if (tmp_figure->blocks[i * tmp_figure->size + j].is_life) {
        coord_x_figure = tmp_figure->x + j;
        coord_y_figure = tmp_figure->y + i;
        if (tmp_field
                ->blocks[coord_y_figure * tmp_field->width + coord_x_figure]
                .is_life)
          result = 1;
        else if (coord_x_figure < 0 || coord_x_figure >= tmp_field->width ||
                 coord_y_figure < 0 || coord_y_figure >= tmp_field->height)
          result = 1;
      }
    }
  }
  return result;
}

/**
 * @brief place falling figure for field
 *
 * @param game
 */
void plant_figure(GameInfo_t *game) {
  falling_figure *tmp_figure = game->figure;
  game_field *tmp_field = game->field;
  int x = 0, y = 0;
  for (int i = 0; i < tmp_figure->size; i++) {
    for (int j = 0; j < tmp_figure->size; j++) {
      if (tmp_figure->blocks[i * tmp_figure->size + j].is_life) {
        x = tmp_figure->x + j;
        y = tmp_figure->y + i;
        tmp_field->blocks[y * tmp_field->width + x].is_life =
            tmp_figure->blocks[i * tmp_figure->size + j].is_life;
      }
    }
  }
}

/**
 * @brief drop line
 * drop line from i to high
 *
 * @param tmp_field
 * @param i - number of line
 */
void drop_line(game_field *tmp_field, int i) {
  if (i == 0) {
    for (int j = 0; j < tmp_field->width; j++) tmp_field->blocks[j].is_life = 0;
  } else {
    for (int i2 = i; i2 > 0; i2--) {
      for (int j = 0; j < tmp_field->width; j++)
        tmp_field->blocks[(i2)*tmp_field->width + j].is_life =
            tmp_field->blocks[(i2 - 1) * tmp_field->width + j].is_life;
    }
  }
}

/**
 * @brief process erase line
 * process erase line and calc score
 *
 * @param game
 * @return int
 */
int erase_line(GameInfo_t *game) {
  int count = 0, result = 0;
  game_field *tmp_field = game->field;

  for (int i = tmp_field->height - 1; i >= 0; i--) {
    while (line_is_field(tmp_field, i)) {
      drop_line(tmp_field, i);
      count++;
    }
  }
  if (count == 1) {
    result = 100;
  } else if (count == 2) {
    result = 300;
  } else if (count == 3) {
    result = 700;
  } else if (count == 4) {
    result = 1500;
  }
  return result;
}

/**
 * @brief Get the next number figure
 * Get the next number figure from rand
 *
 * @param game
 * @return int
 */
int get_next_number(GameInfo_t *game) {
  int result;
  if (game->next != -1) {
    int tmp = game->next;
    game->next = rand() % game->figures->count;
    result = tmp;
  } else {
    result = rand() % game->figures->count;
    game->next = rand() % game->figures->count;
  }
  return result;
}

/**
 * @brief put new figure
 *
 * @param game
 */
void drop_new_figure(GameInfo_t *game) {
  falling_figure *new_figure = create_figure(game);
  new_figure->x = game->field->width / 2 - new_figure->size / 2;
  new_figure->y = 0;
  int number_figure = get_next_number(game);
  for (int i = 0; i < new_figure->size; i++) {
    for (int j = 0; j < new_figure->size; j++) {
      if ((i * new_figure->size + j) < new_figure->size * new_figure->size) {
        new_figure->blocks[i * new_figure->size + j].is_life =
            game->figures
                ->block[number_figure * new_figure->size * new_figure->size +
                        i * new_figure->size + j]
                .is_life;
      }
    }
  }
  free_figure(game->figure);
  game->figure = new_figure;
}

/**
 * @brief init a figure
 *
 * @param game
 * @return falling_figure*
 */
falling_figure *create_figure(GameInfo_t *game) {
  falling_figure *new_figure = malloc(sizeof(falling_figure));
  new_figure->x = 0;
  new_figure->y = 0;
  new_figure->size = game->figures->size;
  new_figure->blocks =
      malloc(sizeof(tetris_block) * new_figure->size * new_figure->size);
  return new_figure;
}

/**
 * @brief
 *
 * @param figure
 */
void free_figure(falling_figure *figure) {
  if (figure) {
    free(figure->blocks);
    figure->blocks = NULL;
    free(figure);
    figure = NULL;
  }
}

/**
 * @brief check line for filling
 *
 * @param tmp_field
 * @param i
 * @return int
 */
int line_is_field(game_field *tmp_field, int i) {
  int result = 1;
  for (int j = 0; j < tmp_field->width; j++) {
    if (!tmp_field->blocks[i * tmp_field->width + j].is_life) result = 0;
  }
  return result;
}

/**
 * @brief save score in file
 *
 * @param game
 */
void high_score_save(GameInfo_t *game) {
  char str[11];
  FILE *scores_file;
  if (game->score > game->high_score) {
    game->high_score = game->score;
    scores_file = fopen("scores.txt", "w");
    if (scores_file) {
      sprintf(str, "%d", game->high_score);
      fputs(str, scores_file);
      fclose(scores_file);
      scores_file = NULL;
      for (int i = 0; str[i] != '\0'; i++) {
        str[i] = '\0';
      }
    } else {
      printf("ERROR!\n can`t save score!\n");
    }
  }
}
/**
 * @brief rotate figure
 *
 * rotate the figure clockwise
 * @param game
 * @return falling_figure*
 */
falling_figure *rotate_figure(GameInfo_t *game) {
  falling_figure *new = create_figure(game);
  falling_figure *old = game->figure;
  new->x = old->x;
  new->y = old->y;
  for (int i = 0; i < old->size; i++) {
    for (int j = 0; j < old->size; j++) {
      new->blocks[i * new->size + j].is_life =
          old->blocks[j * old->size + old->size - 1 - i].is_life;
    }
  }
  return new;
}
/**
 * @brief Processing the user Input
 *
 * @param action
 * @param hold
 */
void updateCurrentState(UserAction_t action, int hold) {
  hold = 0;
  hold++;
  GameInfo_t *game = get_ptr();
  falling_figure *new;
  falling_figure *old;
  switch (action) {
    case Up:
      new = rotate_figure(game);
      old = game->figure;
      game->figure = new;
      if (check_collision(game)) {
        game->figure = old;
        free_figure(new);
        new = NULL;
      } else {
        free_figure(old);
        old = NULL;
      }
      break;
    case Down:
      move_figure_down(game);
      if (check_collision(game)) {
        move_figure_up(game);
      }
      break;
    case Right:
      move_figure_right(game);
      if (check_collision(game)) {
        move_figure_left(game);
      }
      break;
    case Left:
      move_figure_left(game);
      if (check_collision(game)) {
        move_figure_right(game);
      }
      break;
    case Pause:
      game->pause = game->pause ? GAME_READY : GAME_PAUSE;
      break;
    case Terminate:
      game->pause = GAME_OVER;
    default:
      break;
  }
  high_score_save(game);
}

/**
 * @brief Get the score from saver file
 *
 * @return int
 */
int get_score_from_saver() {
  FILE *scores_file;
  int result = 0;
  char result_text[11];
  scores_file = fopen("scores.txt", "r");
  if (scores_file) {
    fgets(result_text, 11, scores_file);
    result = atoi(result_text);
    fclose(scores_file);
    scores_file = NULL;
  } else {
    printf("\ncan`t get saved score!\n");
  }
  return result;
}
/**
 * @brief check and switch level from score
 *
 * @param game
 */
void level_switch(GameInfo_t *game) {
  if (game->score >= 600 && game->level == 1) {
    game->level = 2;
    game->ticks_before_move -= 4;
  } else if (game->score >= 1200 && game->level == 2) {
    game->level = 3;
    game->ticks_before_move -= 4;
  } else if (game->score >= 1800 && game->level == 3) {
    game->level = 4;
    game->ticks_before_move -= 4;
  } else if (game->score >= 2400 && game->level == 4) {
    game->level = 5;
    game->ticks_before_move -= 4;
  } else if (game->score >= 3000 && game->level == 5) {
    game->level = 6;
    game->ticks_before_move -= 4;
  } else if (game->score >= 3600 && game->level == 6) {
    game->level = 7;
    game->ticks_before_move -= 4;
  } else if (game->score >= 4200 && game->level == 7) {
    game->level = 8;
    game->ticks_before_move -= 4;
  } else if (game->score >= 4800 && game->level == 8) {
    game->level = 9;
    game->ticks_before_move -= 4;
  } else if (game->score >= 5400 && game->level == 9) {
    game->level = 10;
    game->ticks_before_move -= 4;
  }
}

#endif