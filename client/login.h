/**************************************************/
/*名称：login.h
/*描述：创建注册的窗口
/*作成日期： 2016-08-31
/*参数：
        参数1：window、GtkWidget* 、输出参数、登录界面的窗口
/*返回值：GtkWidget*
/*作者：杨芳
/***************************************************/

#ifndef __LOGIN_H
#define __LOGIN_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include "ui.h"

typedef struct loginmsg{
    GtkWidget* widget;
    GtkWidget* fwidget;
    GtkWidget* window;
    GtkWidget* username;
    GtkWidget* password;
}login_msg;
void login_on_button_clicked(GtkWidget* button,login_msg* data);
void login_to_regist_on_button_clicked(GtkWidget* button,gpointer data);
gboolean window_drag(GtkWidget *widget, GdkEventButton *event, GdkWindowEdge edge);
GtkWidget* loginUI();
#endif
