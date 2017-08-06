/**************************************************/
/*名称：list.h
/*描述：群组和联系人菜单
/*作成日期： 2016.8.26
/*返回值：void
/*作者：郝程乾
/***************************************************/

#ifndef __LIST_H
#define __LIST_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include "ui.h"

int cmp_grouplist(const void *,const void *);

int cmp_friendlist(const void *,const void *);

int str2int(char *);

void int2str(char *txt,int a);

//========================== GTK CALLBACK FUNCTION DECLARATION ====================
gint moveto_group(GtkWidget *widget,GdkEventCrossing *event,struct args_grouplist *a);//鼠标进入
gint leave_group(GtkWidget *widget,GdkEventCrossing *event,struct args_grouplist *a); //鼠标离开
gint focusin_group(GtkWidget *widget,GdkEventFocus *event,struct args_grouplist *a);  //鼠标选中
gint confirm_group(GtkWidget *widget,struct args1 *a);		      //群界面确认
gint add_group(GtkWidget *widget,struct args1 *a);		      //搜索群
gint create_group(GtkWidget *widget,struct args1 *a);		      //创建群
gint close_window_group(GtkWidget *widget,struct args1 *a);	      //关闭窗口(group)

gint moveto_friend(GtkWidget *widget,GdkEventCrossing *event,struct args_friendlist *a);//鼠标进入
gint leave_friend(GtkWidget *widget,GdkEventCrossing *event,struct args_friendlist *a); //鼠标离开
gint focusin_friend(GtkWidget *widget,GdkEventFocus *event,struct args_friendlist *a);  //鼠标选中
gint confirm_friend(GtkWidget *widget,struct args2 *a);		      //好友界面确认
gint search_friend(GtkWidget *widget,struct args2 *a);		      //搜索好友
gint close_window_friend(GtkWidget *widget,struct args2 *a);          //关闭窗口(friend)

gint destroy_widget(GtkWidget *widget,GtkWidget *a);

struct widget_pack create_grouplist(int *num_group,int *talk_to_group,char *group_ID,int *focus_group,int *lastfocus,struct args_grouplist *args_for_callback_grouplist,struct group_profile *attribute_group,struct group_profile *groupdata,GtkWidget **group_box,GtkWidget **label_grouplist);
struct widget_pack create_friendlist(int *num_friend,char *talk_to_user,char *friend_ID,char *focus_user,int *lastfocus_friend,struct args_friendlist *args_for_callback_friendlist,struct user *attribute_friend,struct user_profile *userdata,GtkWidget **friend_box,GtkWidget **label_friendlist);

#endif
