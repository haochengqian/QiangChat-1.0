src = server.c server.h server_main.c ./util/util.c ./util/cJSON.c ./util/sqlite3.c
server : $(src)
	gcc -o bin/server $(src) -lpthread -lm -ldl -w `pkg-config --cflags --libs gtk+-2.0`
