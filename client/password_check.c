/**************************************************/
/*名称：password_check.c
/*描述：检查密码是否正确
/*作成日期：2016.8.31
/*参数：void
/*返回值：void
/*作者：张宇森
/***************************************************/
#include "password_check.h"

//void new_small_window(const char* title,const char* text);
/**************************************************/
/*名称：password_check
/*描述：检测输入的密码是否正确
/*作成日期：2016.8.31
/*参数：
	const char* username 输入的用户名称 输入参数
	const char* password 输入的密码内容 输入参数
/*返回值：gboolean 表示用户密码写的是否正确
/*作者：张宇森
/***************************************************/
gboolean password_check(const char* username,const char* password){
    puts(username);
    puts(password);
    int username_len=strlen(username),password_len=strlen(password),i;
    if(username_len>30){
        create_new_pop_window("the length of username cannt large than 30 character");
        return FALSE;
    }
    if(password_len<6||password_len>16){
        create_new_pop_window("the length of password must be in 6~16 character");
        return FALSE;
    }
    for(i=0;i<username_len;i++){
        if((username[i]>='0'&&username[i]<='9')||(username[i]>='a'&&username[i]<='z')){}
        else{
            create_new_pop_window("username can be number & lowercase letter only!");
            return FALSE;
        }
    }
    for(i=0;i<password_len;i++){
        if(password[i]==' '){
            create_new_pop_window("password cannot include space!");
            return FALSE;
        }
    }
    return TRUE;
}
