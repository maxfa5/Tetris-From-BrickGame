#include "../tetris.h"

int main(void) {
  tetris_block templates[] = {{0}, {0}, {1}, {0}, {0}, {0}, {1}, {0}, {0},
                              {0}, {1}, {0}, {0}, {0}, {1}, {0}

  };
  GameInfo_t *tetris_test = init_game(10, 20, 4, 1, templates);

  tetris_test->score = 601;
  tetris_test->level = 1;
  level_switch(tetris_test);

  tetris_test->score = 1201;
  level_switch(tetris_test);

  tetris_test->score = 1801;
  level_switch(tetris_test);

  tetris_test->score = 2401;
  level_switch(tetris_test);

  tetris_test->score = 3001;
  level_switch(tetris_test);

  tetris_test->score = 3601;
  level_switch(tetris_test);

  tetris_test->score = 4201;
  level_switch(tetris_test);

  tetris_test->score = 4801;
  level_switch(tetris_test);

  tetris_test->score = 5401;
  level_switch(tetris_test);
  drop_new_figure(tetris_test);
  drop_new_figure(tetris_test);

  move_figure_down(tetris_test);
  move_figure_up(tetris_test);
  move_figure_down(tetris_test);
  move_figure_left(tetris_test);
  move_figure_right(tetris_test);

  tetris_block rotated_figure[] = {
      {0}, {0}, {0}, {0}, {1}, {1}, {1}, {1},
      {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},

  };

  falling_figure *old = tetris_test->figure;

  tetris_test->figure = rotate_figure(tetris_test);
  int result_rotated = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if ((tetris_test->figure->blocks[i * 4 + j].is_life !=
           rotated_figure[i * 4 + j].is_life)) {
        result_rotated++;
      }
    }
  }
  free_figure(old);

  move_figure_right(tetris_test);
  move_figure_right(tetris_test);
  move_figure_right(tetris_test);
  move_figure_right(tetris_test);
  move_figure_left(tetris_test);

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 20; j++) {
      tetris_test->field->blocks[i * 10 + j].is_life = 1;
    }
  }
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 20; j++) {
      tetris_test->field->blocks[i * 10 + j].is_life = 0;
    }
  }
  int check_plant[] = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1};
  int filed = 0;
  plant_figure(tetris_test);

  for (int j = 0; j < 10; j++) {
    if (tetris_test->field->blocks[20 + j].is_life != check_plant[j]) filed++;
  }

  for (int j = 0; j < 10; j++) {
    tetris_test->field->blocks[j].is_life = 1;
  }

  userInput(Up, 0);
  userInput(Down, 0);
  userInput(Right, 0);
  userInput(Right, 0);
  userInput(Left, 0);
  userInput(Right, 0);

  erase_line(tetris_test);

  for (int j = 10; j < 30; j++) {
    tetris_test->field->blocks[j].is_life = 1;
  }

  erase_line(tetris_test);

  for (int j = 10; j < 40; j++) {
    tetris_test->field->blocks[j].is_life = 1;
  }

  erase_line(tetris_test);

  for (int j = 10; j < 50; j++) {
    tetris_test->field->blocks[j].is_life = 1;
  }

  for (int j = 10; j < 60; j++) {
    tetris_test->field->blocks[j].is_life = 1;
  }
  falling_process();
  erase_line(tetris_test);
  falling_process();

  calc_tick(tetris_test, Left, 0);
  calc_tick(tetris_test, Pause, 0);
  calc_tick(tetris_test, Pause, 0);

  for (int j = 1; j < 10 * 20; j++) {
    tetris_test->field->blocks[j].is_life = 1;
  }
  falling_process();
  tetris_test->ticks = 0;
  calc_tick(tetris_test, Pause, 0);
  calc_tick(tetris_test, Terminate, 0);

  falling_figure *test_figure = create_figure(tetris_test);
  free_figure(test_figure);
  free_game(tetris_test);
}