/**************************************************/
/*名称：new_small_window.h
/*描述：用于显示弹出的询问是否执行操作的窗口
/*作成日期：2016.8.31
/*参数：无
/*返回值：0
/*作者：张宇森
/***************************************************/

#ifndef __NEW_SMALL_WINDOW_H
#define __NEW_SMALL_WINDOW_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

void new_small_window_callback(GtkWidget* button,gpointer data);

GtkWidget* new_small_window();

void create_new_pop_window(const char* text);

#endif
