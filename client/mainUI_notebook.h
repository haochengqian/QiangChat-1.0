/**************************************************/
/*名称：mainUI_notebook.c
/*描述：实现主界面的绘制
/*作成日期： 2016.8.31
/*作者：郝程乾
/***************************************************/


#ifndef MAIN_H
#define MAIN_H
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

#define NumOfPortrait 5
static GdkColor moved = {0,0xffff,0xe4e4,0xc4c4};
static GdkColor white= {0,0xffff,0xffff,0xffff};
static GdkColor focus={0,0xf4f4,0xa4a4,0x6060};
static GdkColor skyblue={0,0x8787,0xcece,0xebeb};
static GdkColor red={0,0xffff,0,0};
static GdkColor Blue = {0,0x2121,0x9696,0xf3f3};
static GdkColor AliceBlue = {0,0xf0f0,0xf8f8,0xffff};

struct widget_pack create_grouplist(int *num_group,int *talk_to_group,char *group_ID,int *focus_group,int *lastfocus,struct args_grouplist *args_for_callback_grouplist,struct group_profile *attribute_group,struct group_profile *groupdata,GtkWidget **group_box,GtkWidget **label_grouplist);//创建群列表
gint update_grouplist(struct args_grouplist *tmp,struct group_profile *attribute_group,struct group_profile *data,int *num_group,int *lastfocus,int *talk_to_group,int *focus_group,GtkWidget *group_page,GtkWidget **group_box,GtkWidget **label_grouplist);//更新群列表

struct widget_pack create_friendlist(int *num_friend,char *talk_to_user,char *friend_ID,char *focus_user,int *lastfocus_friend,struct args_friendlist *args_for_callback_friendlist,struct user *attribute_friend,struct user_profile *userdata,GtkWidget **friend_box,GtkWidget **label_friendlist);//创建好友列表
gint update_friendlist(struct args_friendlist *tmp,struct user *attribute_friend,struct user_profile *data,int *num_friend,int *lastfocus_friend,char *talk_to_user,char *focus_user,GtkWidget *friend_page,GtkWidget **friend_box,GtkWidget **label_friendlist);//更新好友列表

void print_msg(struct text_view_info *viewinfo, const char* info, const char* content);
void print_content(struct text_view_info *viewinfo, const char* content, int state);
void print_sticker(struct text_view_info *viewinfo, const char* content);
void send_button_press(GtkWidget *widget,struct text_view_info *viewinfo);
void sticker_button_press(GtkWidget *widget, GdkEvent *event, struct text_view_info *viewinfo);
void FileTransform_button_press(GtkWidget *widget, GdkEvent *event, struct text_view_info *viewinfo);
void create_message_dialog (GtkMessageType type, gchar* message);
void scroll_to_the_end(struct text_view_info *viewinfo);
void insert_sticker(GtkWidget *widget,GdkEventButton *event, struct sticker_info *sinfo);
void destory_sticker_window(GtkWidget *widget,GdkEventCrossing *event,GtkWidget* data);
void group_friend_list_add(GtkWidget* list, const char *buf);
void group_friend_list_clear(GtkWidget* list);
void change_chat_mode(int mode);
void button_background_transform_function(GtkWidget *widget,gpointer *data);
void OpenFile_OfBackgroundTransform(GtkWidget *widget,gpointer *data);
void exec_cmd(int op,char* arg1,char *arg2);

typedef struct
_Data{
    int x;
    int y;
    int width;
    int height;
    gboolean press;
}DATA;
//用于改变左边按钮栏的颜色
typedef struct btn_swap{
    GtkWidget* main_box;
    GtkWidget* main_window[3];
    GtkWidget* main_button[3];
    GtkWidget* chat_box;
    GtkWidget* init_welcome_box;
    GtkWidget* group_friend_list;
    GtkWidget* group_would;
    int* now_id;
    int aim_id;
    int* init_welcome_flag;
}window_status;
//主界面
typedef struct maximize_state{
    int *state;
    GtkWidget* window;
}maximize_state;
//用于最大化/还原窗口
typedef struct _textView
{
  char *ip;
  char *name;
  char *message;

  GtkWidget *textView1;
  //GtkWidget *textView2;
  GtkTextBuffer *buffer1;
  //GtkTextBuffer *buffer2;
}textView;
typedef struct _fontWindow_textView
{
  GtkWidget *window;
  textView *textViewAll;
}fontWindow_textView;
//左边按钮栏的移入回调函数
void on_event_box_moved(GtkWidget* event_box,GdkEventCrossing *event,window_status* data){
    if(*(data->now_id)!=data->aim_id)
        gtk_widget_modify_bg(GTK_WIDGET(event_box),GTK_STATE_NORMAL,&moved);
}
//左边按钮栏的移出回调函数
void on_leave_box_moved(GtkWidget* event_box,GdkEventCrossing *event,window_status* data){
    if(*(data->now_id)!=data->aim_id)
        gtk_widget_modify_bg(GTK_WIDGET(event_box),GTK_STATE_NORMAL,&white);
}
//修改字体回调函数
GtkWidget* get_event_box_with_label(const char* label){
    GtkWidget* eventbox=gtk_event_box_new();
    GtkWidget* lbl=gtk_label_new(label);
    gtk_container_add(GTK_CONTAINER(eventbox),lbl);
    gtk_widget_modify_bg(GTK_WIDGET(eventbox),GTK_STATE_NORMAL,&white);
    return eventbox;
}
//菜单栏回调函数
void title_bar_on_event_box_moved(GtkWidget* event_box,GdkEventCrossing *event,GdkColor* data){
    gtk_widget_modify_bg(GTK_WIDGET(event_box),GTK_STATE_NORMAL,data);
}

void title_bar_on_leave_box_moved(GtkWidget* event_box,GdkEventCrossing *event,gpointer data){
    gtk_widget_modify_bg(GTK_WIDGET(event_box),GTK_STATE_NORMAL,&white);
}

void minimize_event(GtkWidget* event_box,GdkEventCrossing *event,GtkWidget* data){
    gtk_window_iconify(GTK_WINDOW(data));
}

void maximize_event(GtkWidget* event_box,GdkEventCrossing *event,maximize_state* data){
    if(*data->state==0){
        gtk_window_maximize(GTK_WINDOW(data->window));
        *data->state=1;
    }
    else{
        gtk_window_unmaximize(GTK_WINDOW(data->window));
        *data->state=0;
    }
}


//================================mainUI_notebook.c 变量声明
#endif //MAIN_H
