#ifndef SRC_SRC_S1_GREP_H_
#define SRC_SRC_S1_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define sizeSearch 4096
#define sizeBuf 1024
#define sizeInf 512

typedef struct options {
  int eflag;
  int iflag;
  int vflag;
  int cflag;
  int lflag;
  int nflag;
  int hflag;
  int sflag;
  int fflag;
  int oflag;
  int nefl;
  int find;
  int success;
  int FilenameFlag;
  int registflag;
  int NcountPrev;
  int Ncount;
  int CFlagcount;
  int C;
  int gline;
  int regFound;
  int argc;
  char *strStr;
  char *strFile;
  char strSearch[sizeSearch];
  char strPattern[sizeSearch];
  char strBuf[sizeBuf];
  char strInf[sizeInf];
  char **argv;
} opt;

struct option opts[] = {
    {"regexp", no_argument, 0, 'e'},
    {"ignore-case", no_argument, 0, 'i'},
    {"invert-match", no_argument, 0, 'v'},
    {"count", no_argument, 0, 'c'},
    {"files-with-matches", no_argument, 0, 'l'},
    {"line-number", no_argument, 0, 'n'},
    {"no-filename", no_argument, 0, 'h'},
    {"no-messages", no_argument, 0, 's'},
    {"file", no_argument, 0, 'f'},
    {"only-matching", no_argument, 0, 'o'},
    {0, 0, 0, 0},
};

void mainwork(opt *options, size_t *sizeStr, regex_t *regex,
              regmatch_t pmatch[1]);
void Preparing(int argc, char **argv, opt *options, size_t sizeStr);
void File_range(opt *options, int *Nullflag, FILE **fp);
void free_at_exit(opt *options);
void N_Fname_comb(opt *options, int find);
void Filename(opt *options);
void FileString_format(opt *options);
void Vflag(opt *options);
void broadPrint(opt *options);
void CloseFile(FILE *fp);
void NeedName(opt *options);
void oFlagBody(opt *options, regex_t *regex, regmatch_t pmatch[1], int find);
void broadFunctionalPart(opt *options, regex_t *regex, regmatch_t pmatch[1],
                         int find);
void iFlagbody(opt *options);
void cFlagbody(opt *options, int find);
void lFlagbody(opt *options, int find);
void argc_check(opt *options);
void pars_and_prework(opt *options);
void file_check(char *str, int *Nullflag, FILE **fp, opt *options);
void malloc_check(char *str, opt *options);
void last_sym_rewrite(char *strPattern);
void decrement_for_EF(opt *options);
void if_E_or_F(opt options, char *strPattern, char *strSearch);
void Ecase(char *strPattern, char *str);
void Fcase(opt *options);
void switchcase(int *opchar, opt *options);
void file_check_exit(char *str, int *Nullflag, FILE **fp, opt *options);

#endif // SRC_S1_GREP_H_