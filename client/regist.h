#ifndef __REGIST_H
#define __REGIST_H
#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>

typedef struct registmsg{
    GtkWidget* widget;
    GtkWidget* fwidget;
    GtkWidget* username;
    GtkWidget* password;
    GtkWidget* rpassword;
}regist_msg;

void regist_on_button_clicked(GtkWidget* button,regist_msg* data);
void regist_to_login_on_button_clicked(GtkWidget* button,gpointer data);

GtkWidget* registUI();

#endif
