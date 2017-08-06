/**************************************************/
/*名称：main_program.c
/*描述：打开窗口，分派资源，与server进行通讯，初始化客户端
/*作成日期：2016.8.31
/*参数：void
/*返回值:void
/*作者：全组成员
/***************************************************/

#ifndef __MAIN_WINDOW_H
#define __MAIN_WINDOW_H

#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <time.h>
#include "util/util.h"
#include "ui.h"
#include <gtk/gtk.h>
#include <time.h>
#include <stdlib.h>   
#include <arpa/inet.h>
#include <pthread.h>
//收发文件的端口
#define HELLO_WORLD_SERVER_PORT    9898
//监听队列长度
#define LENGTH_OF_LISTEN_QUEUE     20  
//文件名称缓冲区长度
#define FILE_NAME_MAX_SIZE         512 
//监听端口
#define LISTEN_PORT 1235
//缓冲区长度
#define BUFFER_SIZE 2048
//Socket转化成ip所用的结构体

#define FILE_BUFFER_SIZE 1024

struct Sock_ntop_Transform{
	char ip[127];
	char position[1024];
};
//消息列表
struct message_list_item {
	//sendfrom and time
	char message_prop[64];
	//all message's size doesn't exceed 1024 byte!
	char content[1024];
};
char *sock_ntop(const int sin_addr);
void focusin_friend_callback() ;
void focusin_group_callback();
void confirm_group_callback();
void create_group_callback() ;
void confirm_friend_callback();
void send_message_callback(char *message);
int login(char *username, char *password) ;
int regist(char *username, char *password);
int db_init(char *filename);
void init_main_window();
void login_action(char *username);
void logout_action() ;
void show_chatrecord_single(char *username);
void change_my_portrait(int status);
void change_you_portrait(int status);
void show_chatrecord_group(int groupID);
void save_chatrecord_single(const char * jsonstr);
void save_chatrecord_group(const char *jsonstr);
void send_main( void *target_temp );
void handle_message(char *message);
void exec_cmd(int op, void *arg1, void *arg2);
int send_function(char *message);
void auto_update_thread();
void recv_thread() ;
void on_ok_clicked(GtkButton* button,gpointer data);
void on_cancel_clicked(GtkButton* button,gpointer data);
void is_file_accepted_new_object();
void is_file_accepted() ;
void rev_FILE_thread();
int init_client(char * serverIP);
#endif
