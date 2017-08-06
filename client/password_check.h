/**************************************************/
/*名称：password_check.h
/*描述：检查密码是否正确
/*作成日期：2016.8.31
/*参数：void
/*返回值：void
/*作者：张宇森
/***************************************************/
#ifndef __PASSWORD_CHECK_H
#define __PASSWORD_CHECK_H

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

gboolean password_check(const char* username,const char* password);

#endif
