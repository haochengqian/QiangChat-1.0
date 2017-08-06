src = cmUI.c cmUI.h regist.h list.c list.h login.c login.h main.c main_program.c main_program.h mainUI_notebook.c new_small_window.c new_small_window.h password_check.c password_check.h regist.c ui.h ./util/util.c ./util/cJSON.c ./util/sqlite3.c
client : $(src)
	gcc -o bin/client $(src) -g -lpthread -lm -ldl -w `pkg-config gtk+-2.0 --cflags --libs gtk+-2.0 gthread-2.0`
