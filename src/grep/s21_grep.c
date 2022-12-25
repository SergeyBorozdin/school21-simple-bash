#include "s21_grep.h"

int main(int argc, char **argv) {
  opt options = {0};
  size_t sizeStr = 1024;
  regex_t regex;
  regmatch_t pmatch[1];

  // checks
  options.argc = argc;
  options.argv = argv;
  options.strStr = (char *)malloc(sizeStr * sizeof options.strStr);
  malloc_check(options.strStr, &options);
  pars_and_prework(&options);
  argc_check(&options);
  last_sym_rewrite(options.strPattern);
  options.find = optind;
  options.registflag = REG_NEWLINE | REG_EXTENDED;
  NeedName(&options);
  decrement_for_EF(&options);
  strcat(options.strSearch, argv[options.find]);
  if_E_or_F(options, options.strPattern, options.strSearch);

  // work
  while (options.find < (options.argc)) {
    int Nullflag = 0;
    FILE *fp = NULL;
    File_range(&options, &Nullflag, &fp);
    options.CFlagcount = 0;
    while (!Nullflag &&
           ((options.gline = getline(&(options.strStr), &sizeStr, fp)) != -1)) {
      memset(options.strBuf, 0, 1024);
      memset(options.strInf, 0, 512);
      FileString_format(&options);
      options.NcountPrev = options.Ncount;
      options.Ncount++;
      iFlagbody(&options);
      Vflag(&options);
      regcomp(&regex, options.strSearch, options.registflag);
      if (options.oflag && !options.vflag) {
        oFlagBody(&options, &regex, pmatch, options.find);
      } else {
        broadFunctionalPart(&options, &regex, pmatch, options.find);
      }
      broadPrint(&options);
      regfree(&regex);
    }
    cFlagbody(&options, options.find);
    lFlagbody(&options, options.find);
    options.Ncount = 0;
    CloseFile(fp);
    options.find++;
  }

  free_at_exit(&options);
  return 0;
}

void broadPrint(opt *options) {
  if (!options->cflag && !options->lflag &&
      (options->regFound == options->success)) {
    printf("%s", options->strBuf);
  }
}

void CloseFile(FILE *fp) {
  if (fp != NULL) {
    fclose(fp);
  }
}

void NeedName(opt *options) {
  if (!options->hflag) {
    Filename(options);
  }
}

void oFlagBody(opt *options, regex_t *regex, regmatch_t pmatch[1], int find) {
  int line_pos = 0;
  int length = strlen(options->strStr);
  options->regFound = regexec(regex, options->strStr, 1, pmatch, 0);
  if (options->regFound == options->success) {
    options->CFlagcount++;
  }
  N_Fname_comb(options, find);
  strcat(options->strBuf, options->strInf);
  while (regexec(regex, options->strStr + line_pos, 1, pmatch, 0) ==
         options->success) {
    int k = strlen(options->strBuf);
    for (int i = pmatch->rm_so; i < pmatch->rm_eo; i++) {
      (options->strBuf)[k] = (options->strStr + line_pos)[i];
      k++;
    }
    (options->strBuf)[strlen(options->strBuf)] = '\n';
    line_pos = line_pos + pmatch->rm_eo;
    if (pmatch->rm_eo == pmatch->rm_so) line_pos++;
    if (line_pos > length) {
      break;
    }
  }
}

void broadFunctionalPart(opt *options, regex_t *regex, regmatch_t pmatch[1],
                         int find) {
  options->regFound = regexec(regex, options->strStr, 1, pmatch, 0);
  if (options->regFound == options->success) {
    options->CFlagcount++;
    N_Fname_comb(options, find);
    strcpy(options->strBuf, options->strInf);
    strcat(options->strBuf, options->strStr);
  }
}

void iFlagbody(opt *options) {
  if (options->iflag) {
    options->registflag = REG_NEWLINE | REG_EXTENDED | REG_ICASE;
  }
}

void cFlagbody(opt *options, int find) {
  if (options->cflag) {
    if (options->lflag && options->CFlagcount > 0) {
      options->CFlagcount = 1;
    }
    if ((find + 1) < options->argc) {
      if (options->FilenameFlag) {
        sprintf(options->strBuf, "%s:%d", (options->argv)[find + 1],
                options->CFlagcount);
      } else {
        sprintf(options->strBuf, "%d", options->CFlagcount);
      }
      printf("%s\n", options->strBuf);
    }
  }
}

void lFlagbody(opt *options, int find) {
  if (options->lflag && options->CFlagcount > 0) {
    strcpy(options->strBuf, (options->argv)[1 + find]);
    strcat(options->strBuf, "\n");
    printf("%s", options->strBuf);
  }
}

void File_range(opt *options, int *Nullflag, FILE **fp) {
  if ((options->find + 1) >= (options->argc)) {
    *Nullflag = 1;
  } else {
    file_check((options->argv)[1 + options->find], Nullflag, fp, options);
  }
}

void free_at_exit(opt *options) {
  if (options->strStr) {
    free(options->strStr);
    options->strStr = NULL;
  }
  if (options->strFile) {
    free(options->strFile);
    options->strFile = NULL;
  }
}

void N_Fname_comb(opt *options, int find) {
  if (options->FilenameFlag && options->nflag) {
    sprintf(options->strInf, "%s:%d:", (options->argv)[find + 1],
            options->Ncount);
  } else {
    if (options->FilenameFlag && !options->nflag) {
      sprintf(options->strInf, "%s:", (options->argv)[find + 1]);
    } else {
      if (!(options->FilenameFlag) && options->nflag) {
        sprintf(options->strInf, "%d:", options->Ncount);
      }
    }
  }
}

void Filename(opt *options) {
  int k = 0;
  for (int i = options->find; i < options->argc; i++) {
    k++;
  }
  if ((options->eflag || options->fflag) && k >= 2) {
    options->FilenameFlag = 1;
  } else {
    if (k >= 3) {
      options->FilenameFlag = 1;
    }
  }
}

void FileString_format(opt *options) {
  int len = strlen((options->strStr));
  if ((options->strStr)[len - 1] != '\n') {
    strcat((options->strStr), "\n");
  }
}

void Vflag(opt *options) {
  if (options->vflag) {
    options->success = REG_NOMATCH;
  }
}

void pars_and_prework(opt *options) {
  int opchar = 0;
  int opindex = 0;
  while (-1 != (opchar = getopt_long(options->argc, options->argv,
                                     "e:ivclnhsf:o", opts, &opindex))) {
    switchcase(&opchar, options);
  }
}

void malloc_check(char *str, opt *options) {
  if (str == NULL) {
    if (!options->sflag) {
      fprintf(stderr, "memory error\n");
    }
    free_at_exit(options);
    exit(1);
  }
}

void last_sym_rewrite(char *strPattern) {
  if ((strPattern) != NULL) {
    int length = strlen(strPattern);
    strPattern[length - 1] = '\0';
  }
}

void decrement_for_EF(opt *options) {
  if (options->eflag || options->fflag) {
    (options->find)--;
  }
}

void if_E_or_F(opt options, char *strPattern, char *strSearch) {
  if (options.eflag || options.fflag) {
    strcpy(strSearch, strPattern);
  }
}

void Ecase(char *strPattern, char *str) {
  int length = strlen(str);
  if (strlen(str) > 0) {
    if ((length == 1) && (str[length - 1] == '*')) {
      strcpy(str, " *");
    }
    strcat(strPattern, str);
    strcat(strPattern, "|");
  }
}

void argc_check(opt *options) {
  if (options->argc < 3) {
    if (!options->sflag) {
      fprintf(stderr, "too few arguments\n");
    }
    free_at_exit(options);
    exit(1);
  }
}

void file_check(char *str, int *Nullflag, FILE **fp, opt *options) {
  *fp = fopen(str, "r");
  if ((*fp) == NULL) {
    if (!options->sflag) {
      fprintf(stderr, "grep: %s: No such file or directory\n", str);
    }
    *Nullflag = 1;
  } else {
    *Nullflag = 0;
  }
}

void file_check_exit(char *str, int *Nullflag, FILE **fp, opt *options) {
  *fp = fopen(str, "r");
  if ((*fp) == NULL) {
    if (!options->sflag) {
      fprintf(stderr, "grep: %s: No such file or directory\n", str);
    }
    *Nullflag = 1;
    free_at_exit(options);
    exit(1);
  } else {
    *Nullflag = 0;
  }
}

void Fcase(opt *options) {
  FILE *fpattern = NULL;
  size_t sizeStr = 1024;
  options->strFile = (char *)malloc(sizeStr * sizeof options->strFile);
  int getCheck = 0;
  int Nullflag = 0;
  int starflag = 0;
  malloc_check(options->strFile, options);
  file_check_exit(optarg, &Nullflag, &fpattern, options);
  while (
      ((getCheck = getline(&(options->strFile), &sizeStr, fpattern)) != -1) &&
      (!Nullflag)) {
    int length = strlen(options->strFile);
    if ((strlen(options->strFile) == 1) &&
        ((options->strFile)[length - 1] == '\n') && !starflag) {
      (options->strFile)[length - 1] = '*';
      Ecase(options->strPattern, options->strFile);
      starflag = 1;
    } else {
      if ((options->strFile)[length - 1] == '\n') {
        (options->strFile)[length - 1] = '\0';
        Ecase(options->strPattern, options->strFile);
      } else {
        Ecase(options->strPattern, options->strFile);
      }
    }
  }
  CloseFile(fpattern);
}

void switchcase(int *opchar, opt *options) {
  switch (*opchar) {
    case 'e':
      options->eflag = 1;
      Ecase(options->strPattern, optarg);
      break;
    case 'i':
      options->iflag = 1;
      break;
    case 'v':
      options->vflag = 1;
      break;
    case 'c':
      options->cflag = 1;
      break;
    case 'l':
      options->lflag = 1;
      break;
    case 'n':
      options->nflag = 1;
      break;
    case 'h':
      options->hflag = 1;
      break;
    case 's':
      options->sflag = 1;
      break;
    case 'f':
      options->fflag = 1;
      Fcase(options);
      break;
    case 'o':
      options->oflag = 1;
      break;
    default:
      printf("usage: grep [-benstuv] [file ...]\n");
  }
}
