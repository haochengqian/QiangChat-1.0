#ifndef __UI_H
#define __UI_H

#ifdef __cplusplus
extern "C"
{
#endif
//====================ADD AFTER THIS LINE==========================

int login(char *username, char *password);
int regist(char *username, char *password);

void focusin_friend_callback();
void focusin_group_callback();


//=======================UI STATE===================================
#include <gtk/gtk.h>

GtkWidget* loginUI();

//UI状态
struct UI_state {
	int tab;
	char current_username[32];
	int current_groupID;
};

//========================UI STATE==================================

/////////////////////////char window///////////////////////////////

//聊天界面
struct text_view_info{
    GtkWidget *view1, *view2;
    GtkTextBuffer *view1_buffer,*view2_buffer;
};

struct text_view_info viewinfo;
GtkWidget *cmlabel;
GtkWidget *group_friend_list;

//群组用户名结构
struct group_username{
	char username[32];
};

void print_msg(struct text_view_info *viewinfo, const char* info, const char* content);
void update_group_friend_list(int num,struct group_username *name);
void set_chat_title(GtkWidget *cmlabel, const char *buf);  

// buf should be like "<span foreground=\"white\"><big>Communicating With Bilv</big></span>"
void clear_chat_window();

//callback function in main.c
void send_message_callback(char *message);
void change_chat_mode(int mode);

//////////////////////////friend list////////////////////////////////

//===================list==========================================
typedef GdkColor color;
typedef gushort gs;
static color move = {0,0xffff,0xe4e4,0xc4c4};
static color choose = {0,0xf4f4,0xa4a4,0x6060};
static color blue = {0,0x2121,0x9696,0xf3f3};
static color grey = {0,0xbebe,0xbebe,0xbebe};
#define N 1005

//=============================== struct ============================

//群组信息
struct group_profile{
	int groupID;
};
//控件包
struct widget_pack{
	GtkWidget *Mainbox;
	GtkWidget *update_box;
};
//用户信息
struct user_profile{
	char username[32];
	int status;
};
//群组列表信息
struct args_grouplist{
	int id;
	int *lastfocus;
	int *num_group;
	GtkWidget **event_box;
	GtkWidget **label;
	int *focus_group;
	int *talk_to;
	struct group_profile *attribute_group;
};

//群组功能按钮
struct args1{
	GtkWidget *button_add_group,*button_create_group,*entry,*window;
	char *buf;
};
//好友信息列表
struct args_friendlist{
	int id;
	int *lastfocus_friend;
	int *num_friend;
	GtkWidget **event_box;
	GtkWidget **label;
	char *focus_user;
	char *talk_to;
	struct user *attribute_friend;
};
//好友功能按钮
struct args2{
	GtkWidget *button_add_friend,*entry,*window;
	char *buf;

};
//用户结构
struct user{
	char *username;
	int status;
};

//========================= 群列表宏定义 ===============================
#define CREATE_GROUPLIST num_group,talk_to_group,group_ID,focus_group,lastfocus,args_for_callback_grouplist,attribute_group,groupdata,group_box,label_grouplist
#define UPDATE_GROUPLIST args_for_callback_grouplist,attribute_group,groupdata,num_group,lastfocus,talk_to_group,focus_group,group_page,group_box,label_grouplist
//========================= 好友宏定义 =================================
#define CREATE_FRIENDLIST num_friend,talk_to_user,friend_ID,focus_user,lastfocus_friend,args_for_callback_friendlist,attribute_friend,userdata,friend_box,label_friendlist
#define UPDATE_FRIENDLIST args_for_callback_friendlist,attribute_friend,userdata,num_friend,lastfocus_friend,talk_to_user,focus_user,friend_page,friend_box,label_friendlist

//==========================函数声明====================================
struct widget_pack create_grouplist(int *num_group,int *talk_to_group,char *group_ID,int *focus_group,int *lastfocus,struct args_grouplist *args_for_callback_grouplist,struct group_profile *attribute_group,struct group_profile *groupdata,GtkWidget **group_box,GtkWidget **label_grouplist);//创建群列表
gint update_grouplist(struct args_grouplist *tmp,struct group_profile *attribute_group,struct group_profile *data,int *num_group,int *lastfocus,int *talk_to_group,int *focus_group,GtkWidget *group_page,GtkWidget **group_box,GtkWidget **label_grouplist);//更新群列表

struct widget_pack create_friendlist(int *num_friend,char *talk_to_user,char *friend_ID,char *focus_user,int *lastfocus_friend,struct args_friendlist *args_for_callback_friendlist,struct user *attribute_friend,struct user_profile *userdata,GtkWidget **friend_box,GtkWidget **label_friendlist);//创建好友列表
gint update_friendlist(struct args_friendlist *tmp,struct user *attribute_friend,struct user_profile *data,int *num_friend,int *lastfocus_friend,char *talk_to_user,char *focus_user,GtkWidget *friend_page,GtkWidget **friend_box,GtkWidget **label_friendlist);//更新好友列表


/*
update_grouplist(UPDATE_GROUPLIST);
update_friendlist(UPDATE_FRIENDLIST);
*/

struct UI_state get_UI_state();
//=======================mainUI===============================


//display main window
void mainUI_window_show();
void create_new_pop_window(const char* text);

//====================ADD BEFORE THIS LINE====================
#ifdef __cplusplus
}
#endif

#endif
