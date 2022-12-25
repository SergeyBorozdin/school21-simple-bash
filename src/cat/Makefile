TARGET1 = s21_cat
CC = gcc
CFLAGS = -Wall -Werror -Wextra


s21_cat : 
	$(CC) $(CFLAGS) s21_cat.c s21_cat.h -o $(TARGET1)

clean : 
	rm -f *.o $(TARGET1)

rebuild : clean s21_cat
