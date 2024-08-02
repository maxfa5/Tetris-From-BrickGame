#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../brick_game/tetris/tetris.h"

void print_tetris(GameInfo_t *game);

void print_scores(GameInfo_t *game);
void print_next_figure(GameInfo_t *game);

void print_border(int width, int height);

int main(void) {
  int error = 0;
  tetris_block templates[] = {
      {0}, {0}, {1}, {0}, {0}, {0}, {1}, {0},
      {0}, {0}, {1}, {0}, {0}, {0}, {1}, {0},

      {0}, {0}, {0}, {0}, {0}, {0}, {1}, {0},
      {0}, {0}, {1}, {0}, {0}, {0}, {1}, {1},

      {0}, {0}, {0}, {0}, {0}, {0}, {1}, {1},
      {0}, {1}, {1}, {0}, {0}, {0}, {0}, {0},

      {0}, {0}, {0}, {0}, {0}, {0}, {1}, {0},
      {0}, {0}, {1}, {0}, {0}, {1}, {1}, {0},

      {0}, {0}, {1}, {0}, {0}, {1}, {1}, {1},
      {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},

      {0}, {0}, {0}, {0}, {1}, {1}, {0}, {0},
      {1}, {1}, {0}, {0}, {0}, {0}, {0}, {0},

      {0}, {0}, {0}, {0}, {0}, {1}, {1}, {0},
      {0}, {0}, {1}, {1}, {0}, {0}, {0}, {0},
  };

  srand(time(NULL));
  initscr();
  keypad(stdscr, 1);
  noecho();
  timeout(20);
  curs_set(0);
  int c = 0;
  GameInfo_t *game = init_game(10, 20, 4, 7, templates);
  while (c != 27) {
    while (c != 10) {
      clear();
      mvprintw(12, 12, "Please Enter to start Tetris, or Esc to Exit!");
      c = getch();
      if (c == 27) {
        break;
      }
    }
    if (c == 27) {
      break;
    }
    set_start_game(game);
    if (!game->error) {
      tetris_player player = {0};
      game->player = &player;
      drop_new_figure(game);

      while (game->pause != GAME_OVER) {
        print_scores(game);
        print_next_figure(game);
        print_border(30, 22);
        c = getch();
        userInput(c, 0);
        clear();
        print_tetris(game);
      }
      c = 0;
    } else {
      error = game->error;
      c = 27;
    }
  }
  free_game(game);
  endwin();
  return error;
}

void print_tetris(GameInfo_t *game) {
  game_field *field = game->field;
  falling_figure *figure = game->figure;
  int x = 0, y = 0, symbol;
  for (int i = 0; i < field->height; i++) {
    for (int j = 0; j < field->width; j++) {
      symbol = 0;
      if (field->blocks[i * field->width + j].is_life) {
        symbol = 1;
      } else {
        x = j - figure->x;
        y = i - figure->y;
        if (x >= 0 && x < figure->size && y >= 0 && y < figure->size)
          if (figure->blocks[y * figure->size + x].is_life) symbol = 1;
      }
      if (symbol) {
        mvprintw(i + 1, (j + 1) * 3, "[ ]");
      } else {
        mvprintw(i + 1, (j + 1) * 3, " . ");
      }
    }
  }
}

void print_border(int width, int height) {
  int i = 0, j = 0;
  for (i = 0; i < width * 2; i++) {
    mvaddch(j, i, '-');
    mvaddch(height, i, '-');
  }
  for (i = 0, j = 0; j < height; j++) {
    mvaddch(j, i, '|');
    mvaddch(j, width + 3, '|');
    mvaddch(j, width * 2, '|');
  }
}

void print_scores(GameInfo_t *game) {
  mvprintw(1, 12 * 3, "HIGHT SCORE: %d", game->high_score);
  mvprintw(3, 12 * 3, "SCORE: %d", game->score);
  mvprintw(5, 12 * 3, "Level: %d", game->level);
  mvprintw(7, 12 * 3, "Next: ");
  mvprintw(12, 12 * 3, "Pause: ");
  if (game->pause)
    mvprintw(12, 18 * 3, "YES");
  else
    mvprintw(12, 18 * 3, "NO");
}

void print_next_figure(GameInfo_t *game) {
  int number_figure = game->next;
  for (int i = 0; i < game->figure->size; i++) {
    for (int j = 0; j < game->figure->size; j++)
      if (game->figures
              ->block[number_figure * game->figure->size * game->figure->size +
                      i * game->figure->size + j]
              .is_life) {
        mvprintw(j + 8, (i + 12) * 3, "[ ]");
      };
  }
}