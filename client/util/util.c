#include "util.h"

char timestr[64];

const char *get_formatted_time(){
	time_t t = time(0);
	strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", localtime(&t));
	return timestr;
}

FILE *create_log_file(const char* filename){
	return fopen(filename, "w+");
}

int write_log(FILE * file, const char* content){
	if(file == NULL) return -1;
	return fprintf(file, "%s - %s\n", get_formatted_time(), content);
}
