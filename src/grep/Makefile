TARGET1 = s21_grep
CC = gcc
CFLAGS = -Wall -Werror -Wextra

s21_grep : 
	$(CC) $(CFLAGS) s21_grep.c s21_grep.h -o $(TARGET1)

clean : 
	rm -f *.o $(TARGET1)

rebuild : clean s21_grep
