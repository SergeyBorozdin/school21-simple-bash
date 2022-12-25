#ifndef SRC_SRC_S1_CAT_H_
#define SRC_SRC_S1_CAT_H_

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct options {
  int b; // нумерует не пустые строки
  int e; // показывает конец строки
  int n; // нумерует все строки
  int s; // сжимает пустые строки до одной
  int t; // показыввает табы
  int v; // показывает не читаемые символы
} opt;

void cat_not_arg();
void parser(int argc, char *argv[], opt *options);
void reader(int argc, char *argv[], opt options);

#endif // SRC_S1_CAT_H_