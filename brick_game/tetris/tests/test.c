#include <check.h>

#include "../tetris.h"

START_TEST(test_tetris) {
  tetris_block templates[] = {{0}, {0}, {1}, {0}, {0}, {0}, {1}, {0}, {0},
                              {0}, {1}, {0}, {0}, {0}, {1}, {0}

  };
  GameInfo_t *tetris_test = init_game(10, 20, 4, 1, templates);
  ck_assert_int_eq(get_score_from_saver(), 0);
  tetris_test->score = 601;
  tetris_test->level = 1;
  level_switch(tetris_test);
  ck_assert_int_eq(tetris_test->level, 2);

  tetris_test->score = 1201;
  level_switch(tetris_test);
  ck_assert_int_eq(tetris_test->level, 3);

  tetris_test->score = 1801;
  level_switch(tetris_test);
  ck_assert_int_eq(tetris_test->level, 4);

  tetris_test->score = 2401;
  level_switch(tetris_test);
  ck_assert_int_eq(tetris_test->level, 5);

  tetris_test->score = 3001;
  level_switch(tetris_test);
  ck_assert_int_eq(tetris_test->level, 6);

  tetris_test->score = 3601;
  level_switch(tetris_test);
  ck_assert_int_eq(tetris_test->level, 7);

  tetris_test->score = 4201;
  level_switch(tetris_test);
  ck_assert_int_eq(tetris_test->level, 8);

  tetris_test->score = 4801;
  level_switch(tetris_test);
  ck_assert_int_eq(tetris_test->level, 9);

  tetris_test->score = 5401;
  level_switch(tetris_test);
  ck_assert_int_eq(tetris_test->level, 10);
  drop_new_figure(tetris_test);
  drop_new_figure(tetris_test);

  move_figure_down(tetris_test);
  ck_assert_int_eq(1, tetris_test->figure->y);
  move_figure_up(tetris_test);
  ck_assert_int_eq(0, tetris_test->figure->y);
  move_figure_down(tetris_test);
  move_figure_left(tetris_test);
  move_figure_right(tetris_test);
  ck_assert_int_eq(
      tetris_test->field->width / 2 - tetris_test->figure->size / 2,
      tetris_test->figure->x);

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
  ck_assert_int_eq(0, result_rotated);
  free_figure(old);
  move_figure_right(tetris_test);
  move_figure_right(tetris_test);
  move_figure_right(tetris_test);
  move_figure_right(tetris_test);
  ck_assert_int_eq(check_collision(tetris_test), 1);
  move_figure_left(tetris_test);

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 20; j++) {
      tetris_test->field->blocks[i * 10 + j].is_life = 1;
    }
  }
  ck_assert_int_eq(check_collision(tetris_test), 1);
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

  ck_assert_int_eq(filed, 0);

  ck_assert_int_eq(line_is_field(tetris_test->field, 0), 0);

  for (int j = 0; j < 10; j++) {
    tetris_test->field->blocks[j].is_life = 1;
  }

  ck_assert_int_eq(line_is_field(tetris_test->field, 0), 1);

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
  userInput(Pause, 0);
  userInput(Terminate, 0);
  calc_tick(tetris_test, Terminate, 0);

  falling_figure *test_figure = create_figure(tetris_test);
  test_figure->blocks = NULL;
  free_figure(test_figure);
  free_game(tetris_test);
}
END_TEST

void case_test(Suite *s, int *fail) {
  SRunner *runner = srunner_create(s);
  srunner_run_all(runner, CK_NORMAL);
  srunner_set_fork_status(runner, CK_NOFORK);
  *fail = srunner_ntests_failed(runner);
  srunner_free(runner);
}

Suite *tetris(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("tetris");
  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_tetris);

  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int no_falied = 0;

  case_test(tetris(), &no_falied);
  return !no_falied ? 0 : 1;
}
