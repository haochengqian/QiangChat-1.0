/**************************************************/
/*名称：cmUI.h
/*描述：显示聊天窗口界面的功能
/*作成日期：2016-08-31

/*作者：郑安庆
/***************************************************/

#ifndef __CMUI_H
#define __CMUI_H

#include <gtk/gtk.h>
#include <string.h>
#include <malloc.h>
#include "ui.h"
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<arpa/inet.h>
#define POSITION_SIZE 1024
#define HELLO_WORLD_SERVER_PORT    9898
#define LENGTH_OF_LISTEN_QUEUE     20
#define BUFFER_SIZE                1024
#define FILE_NAME_MAX_SIZE         512

struct sticker_info{
    GtkWidget *sticker_window;
    GtkTextBuffer *view2_buffer;
    char *str;
    struct sticker_info **spointer;
};

void send_button_press(GtkWidget *widget,struct text_view_info *viewinfo);
void print_msg(struct text_view_info *viewinfo, const char* info, const char* content);
void print_content(struct text_view_info *viewinfo, const char* content, int state);
void print_sticker(struct text_view_info *viewinfo, const char* content);
void sticker_button_press(GtkWidget *widget, GdkEvent *event, struct text_view_info *viewinfo);
void OpenFile_OfBackgroundTransform(GtkWidget *widget,gpointer *data);
void button_background_transform_function(GtkWidget *widget,gpointer *data);
void FileTransform_button_press(GtkWidget *widget, GdkEvent *event, struct text_view_info *viewinfo);
void OpenFile(GtkWidget *widget,gpointer *data);
void create_message_dialog (GtkMessageType type, gchar* message);
void scroll_to_the_end(struct text_view_info *viewinfo);
void insert_sticker(GtkWidget *widget,GdkEventButton *event, struct sticker_info *sinfo);
void destory_sticker_window(GtkWidget *widget,GdkEventCrossing *event,GtkWidget* data);
void group_friend_list_add(GtkWidget* list, const char *buf);
void group_friend_list_clear(GtkWidget* list);
void change_chat_mode(int mode);
void update_group_friend_list(int num,struct group_username *name);
void set_chat_title(GtkWidget *cmlabel, const char *buf);
void clear_chat_window();

#endif
