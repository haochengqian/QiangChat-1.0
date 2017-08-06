#ifndef __SERVER_H
#define __SERVER_H

#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <memory.h>
#include "util/util.h"

#define LISTEN_PORT 1235 //监听端口
#define BACKLOG 16 //记录日志
#define MAX_CLIENT_COUNT 16 //最大连接数
#define MAX_SEND_QUEUE_SIZE 256 //请求处理队列长度
//buffer size for receiving message
#define BUFFER_SIZE 2048 //消息缓冲区长度

//storing the clients' property
struct client_property {
	struct sockaddr_in addr;
	//initial value is -1
	int client_fd;

	//describes the user
	int is_online;
	char username[32];
};
//发送的消息包
struct send_prop{
	char *message;
	char username[32];
};

//copy string in src and return the head pointer
char * copy_string(const char *src);

//獲取用戶套接字
struct client_property * get_user_prop(char *username);

////=======================转换套接字
char *sock_ntop(const struct sockaddr *sa);

//get user's ip 
char *get_user_ip(char *username);

//open the database & create tables
int db_init();

//===================SERVER LOGIC=================================
//user register
void user_register(struct client_property *prop, char *username, char *password);
//user login 
void user_login(struct client_property *prop, char *username, char *password);
//user logout
void user_logout(struct client_property *prop, char *username);
//callback fun of send single meesage
int send_single_message_callback(void *arg, int nr, char **values, char **names);

//char *message should be static...
void send_single_message(char *message);
//传文件时为了返回ip的函数
void send_return_message(char *message);

void send_friend_list(char *username);

//add ub to ua's contact list
void add_contact(char *ua, char *ub);

void create_group_callback(void *arg, int nr, char **values, char **names);

void create_group(char *username);

int group_exist(int groupID);

//TODO: receipt
void join_group(char *username, int groupID);

//TODO: receipt
void quit_group(char *username, int groupID);

void send_group_list(char *username);

int send_group_message_callback(void *arg, int nr, char **values, char **names);

void send_group_message(char *message);

void send_group_profile(char *requested_user, int groupID);

void send_friend_image(char* username,char* sendfrom);

void user_image_modify(char *username, int user_image);

void handle_client_message(struct client_property * prop, const char * message);

int get_user_fd(char *username);

void client_thread_function(void *arg);

void listen_thread_function(void *arg);

void send_thread_function(void *arg);

void save_offline_message(struct send_prop msg);

void send_offline_message(char *username);

void send_message_by_username(char *username, char *msg);

void send_message_by_fd(int fd, char *message);

void add_client(int connectfd, struct sockaddr_in addr);

void delete_client(struct client_property *prop);

int init_socket();

void show_server_status();

void exec_cmd(char * command);

void exit_server();

void init_server();

#endif
