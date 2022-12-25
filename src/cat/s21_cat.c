#include "s21_cat.h"

void cat_not_arg() {
  char symbol;
  while (scanf("%c", &symbol)) {
    printf("%c", symbol);
  }
}

void parser(int argc, char *argv[], opt *options) {
  int opt = 0;
  int option_index = 0;
  struct option opts[] = {
      {"number-nonblank", 0, 0, 'b'},
      {"show-ends", 0, 0, 'e'},
      {"number", 0, 0, 'n'},
      {"squeeze-blank", 0, 0, 's'},
      {"show-tabs", 0, 0, 't'},
      {"show-nonprinting", 0, 0, 'v'},
      {0, 0, 0, 0},
  };

  while (-1 !=
         (opt = getopt_long(argc, argv, "+benstvTE", opts, &option_index))) {
    switch (opt) {
      case 'b':
        options->b = 1;
        break;
      case 'e':
        options->e = 1;
        options->v = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 't':
        options->t = 1;
        options->v = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'E':
        options->e = 1;
        options->v = 1;
        break;
      case 'T':
        options->t = 1;
        break;
      default:
        fprintf(stderr, "usage - 'benstvTE'");
        exit(1);
    }
  }
}

void reader(int argc, char *argv[], opt options) {
  for (int i = 1; i < argc; i++) {
    if (*argv[i] == '-') {
      continue;
    } else {
      FILE *fp = fopen(argv[i], "r");

      if (fp != NULL) {
        int symbol = 0;
        int symbol_prev = '\n';
        int flag_bool = 0;
        int string_count = 0;
        int empty_string = 0;

        while ((symbol = fgetc(fp)) != EOF) {
          if (options.s == 1 && symbol == '\n' && symbol_prev == '\n') {
            empty_string++;
            if (empty_string > 1) {
              flag_bool = 1;
            }
          } else {
            flag_bool = 0;
            empty_string = 0;
          }
          if (flag_bool == 0) {
            if (((options.n == 1 && options.b == 0) ||
                 (options.b == 1 && symbol != '\n')) &&
                symbol_prev == '\n') {
              printf("%*d\t", 6, ++string_count);
            }
            if (options.t == 1 && symbol == '\t') {
              printf("^");
              symbol = 'I';
            }
            if (options.e == 1 && symbol == '\n') {
              printf("$");
            }
            if (options.v == 1) {
              if ((symbol >= 0 && symbol < 9) || (symbol > 10 && symbol < 32)) {
                printf("^");
                symbol = symbol + 64;
              } else if (symbol == 127) {
                printf("^");
                symbol = '?';
              }
            }
            printf("%c", symbol);
            symbol_prev = symbol;
          }
        }
      } else {
        printf("%s: %s: No such file or directory\n", argv[0], argv[i]);
      }
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    cat_not_arg();
  }

  opt options = {0, 0, 0, 0, 0, 0};
  parser(argc, argv, &options);
  reader(argc, argv, options);

  return 0;
}
