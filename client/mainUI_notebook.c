/**************************************************/
/*名称：mainUI_notebook.c
/*描述：实现主界面的绘制
/*作成日期： 2016.8.31
/*作者：郝程乾
/***************************************************/

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
//颜色
#define NumOfPortrait 5
static GdkColor moved = {0,0xffff,0xe4e4,0xc4c4};
static GdkColor white= {0,0xffff,0xffff,0xffff};
static GdkColor focus={0,0xf4f4,0xa4a4,0x6060};
static GdkColor skyblue={0,0x8787,0xcece,0xebeb};
static GdkColor red={0,0xffff,0,0};
static GdkColor Blue = {0,0x2121,0x9696,0xf3f3};
static GdkColor AliceBlue = {0,0xf0f0,0xf8f8,0xffff};
//static GdkColor pink={0,0x,0x,0x};


struct sticker_info{ //头像结构体
    GtkWidget *sticker_window;
    GtkTextBuffer *view2_buffer;
    char *str;
    struct sticker_info **spointer;
};
//菜单栏参数
int maximize_flag=0;
int init_welcome_flag=1;
int now_id;
int n_g,t_t_g,f_g,l,n_f,l_f;
n_g = 0;
l = -1;
n_f = 0;
l_f = -1;
f_g = -1;
//==============群组参数
struct widget_pack sky_group,sky_friend;
GtkWidget *group_box[N];//eventbox
GtkWidget *label_grouplist[N];	//label_grouplist
GtkWidget *group_page;
int *num_group = &n_g;
int *talk_to_group = &t_t_g;//当前面向的群组
int *focus_group = &f_g;	//刷新前面向的群组
char group_ID[100];//搜索时用
int *lastfocus = &l;
struct args_grouplist args_for_callback_grouplist[N];//回调函数参数
struct group_profile attribute_group[N];		 //eventbox属性
struct group_profile groupdata[N]; //获取的群组属性
//===============
int portrait_open=0;
int photo_id;
GtkWidget *hboxofzaqdown,*hboxofzaqup;
GtkWidget *friend_box[N];//eventbox
GtkWidget *label_friendlist[N];	//label_friendlist
GtkWidget *friend_page;
GtkWidget *global_button,*global_image;
GtkWidget *zaq_button,*zaq_image,*zaqfather_image;
int *num_friend = &n_f;
char talk_to_user[100];
char focus_user[100];
char friend_ID[100];//搜索时用
int *lastfocus_friend = &l_f;
struct args_friendlist args_for_callback_friendlist[N];//回调函数参数
struct user attribute_friend[N];		 //eventbox属性
struct user_profile userdata[N]; //获取的用户属性
int group_list_size;
int friend_list_size;
int flag_now_window;
GtkWidget* login_window;
GtkWidget* mainUI_window;
GtkWidget* regist_window;
GtkWidget* mainUI_window_scwinlist;
GtkWidget* mainUI_window_group_list_box;
GtkWidget* mainUI_window_about_box;
GtkWidget* mainUI_window_group_would;
GtkWidget* mainUI_window_group_transform;
GtkWidget* mainUI_window_recipt;
GtkWidget* mainUI_window_about;
GtkWidget* mainUI_window_about_image;
GtkWidget* mainUI_window_about_button;
GtkWidget* mainUI_window_about_window;

GtkWidget *cmeventbox1,*cmeventbox2, *cmeventbox3 ,*cmeventbox4;
GtkWidget *scwinlist;

GtkWidget *temp_ui_window_zaq;

GtkWidget *window;
GtkWidget *vbox;
GtkWidget *hbox;
GtkWidget *bbox;
GtkWidget *button;
GtkWidget *label;
GtkWidget *image;
GtkWidget *sep;

GtkWidget *FileSelection;//背景切换
//===========================================顶部菜单变量
static GtkWidget *entry;
gboolean flag_recipt_visiable = 0;
gint count = 0;
gdouble nn = 0;
gdouble mm = 0;
gint s = 0;
gboolean first = TRUE;
gboolean have_dot = FALSE;
gboolean have_result = FALSE;
gchar number[100];
typedef struct
_Data{
    int x;
    int y;
    int width;
    int height;
    gboolean press;
}DATA;
//===============================================================
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
window_status w_status[2];
//用于最大化/还原窗口
typedef struct maximize_state{
    int *state;
    GtkWidget* window;
}maximize_state;
maximize_state maxi_state;

//============================================
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
extern PangoFontDescription *font_desc;
extern char current_username[32];
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
/**************************************************/
/*名称：font_selection_ok
/*描述：字体选择窗口OK选项的回调函数
/*作成日期： 2016.8.30
/*参数：参数1：参数名称 *button、参数类型 GtkWidget 、输入参数、参数含义 按钮名称
        参数2：参数名称*fontWindow_textViewAll、参数类型 fontWindow_textView 、输入参数、字体信息
/*返回值：void
/*作者：张宇森
/***************************************************/
void font_selection_ok( GtkWidget *button, fontWindow_textView *fontWindow_textViewAll)
{
    gchar *font;
    font = gtk_font_selection_dialog_get_font_name (GTK_FONT_SELECTION_DIALOG (fontWindow_textViewAll->window));
    font_desc = pango_font_description_from_string (font);
    gtk_widget_destroy(GTK_OBJECT(fontWindow_textViewAll->window));
}
/**************************************************/
/*名称：fontSelect
/*描述：字体选择窗口OK选项的回调函数
/*作成日期： 2016.8.30
/*参数：参数1：参数名称 *button、参数类型 GtkWidget 、输入参数、参数含义 按钮名称
        参数2：参数名称*textViewAll、参数类型 textView 、输入参数、参数含义 字体选择界面
/*返回值：void
/*作者：张宇森
/***************************************************/
void fontSelect( GtkWidget *button, textView *textViewAll)
{

    GtkWidget *window;
    window = gtk_font_selection_dialog_new ("QiangChat 字体选择");
    gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_MOUSE);
    fontWindow_textView *fontWindow_textViewAll = (fontWindow_textView *)malloc(sizeof(fontWindow_textView));
    fontWindow_textViewAll->window = window;
    fontWindow_textViewAll->textViewAll = textViewAll;

    g_signal_connect (G_OBJECT (GTK_FONT_SELECTION_DIALOG (window)->ok_button),
		      "clicked", G_CALLBACK (font_selection_ok),
		      fontWindow_textViewAll);

    g_signal_connect (G_OBJECT (window), "destroy",
	              G_CALLBACK (gtk_widget_destroyed),
		      NULL);

    gtk_signal_connect_object(GTK_OBJECT (GTK_FONT_SELECTION_DIALOG (window)->cancel_button),
			      "clicked",
			      GTK_SIGNAL_FUNC(gtk_widget_destroy), 
			      GTK_OBJECT(window));

    gtk_widget_show (window);

    g_print("font");

}
/**************************************************/
/*名称：on_main_button_clicked
/*描述：friend 和 group 点击确认的回调函数
/*作成日期： 2016.8.30
/*参数：参数1：参数名称 *button、参数类型 GtkWidget 、输入参数、参数含义 按钮名称
        参数2：参数名称 *event、参数类型 GdkEventButton、输入参数、参数含义 按钮触发的事件
        参数3：参数名称 * data、参数类型 window_status、输入参数、参数含义 主界面信息
/*返回值：void
/*作者：郝程乾
/***************************************************/
//左边按钮栏的点击回调函数
void on_main_button_clicked(GtkWidget* button,GdkEventButton *event,window_status* data){
    int now_id=*(data->now_id),aim_id=data->aim_id;
    flag_now_window = now_id;
    if(now_id!=aim_id){
        gtk_widget_modify_bg(GTK_WIDGET(button),GTK_STATE_NORMAL,&focus);
        gtk_widget_modify_bg(data->main_button[now_id],GTK_STATE_NORMAL,&white);
        gtk_widget_hide_all(data->main_window[now_id]);
        gtk_widget_show_all(data->main_window[aim_id]);
        *(data->now_id)=aim_id;
        if(aim_id==2){
            gtk_widget_hide_all(data->chat_box);
            //printf("2222\n%d\n",*(data->init_welcome_flag));
            if(*(data->init_welcome_flag)==0)
                gtk_widget_hide_all(data->init_welcome_box);
        }
        else{
            if(*(data->init_welcome_flag)==0)
                gtk_widget_show_all(data->init_welcome_box);
            else{
                gtk_widget_show_all(data->chat_box);
                gtk_widget_hide_all(mainUI_window_group_would);
            }
        }
        if(aim_id==1){
            gtk_widget_hide_all(data->group_friend_list);
            gtk_widget_show_all(mainUI_window_group_would);
            gtk_widget_hide_all(mainUI_window_group_transform);
        }
    }
}
/**************************************************/
/*名称：on_event_box_moved
/*描述：friend 和 group 鼠标移入变色的回调函数
/*作成日期： 2016.8.30
/*参数：参数1：参数名称 *button、参数类型 GtkWidget 、输入参数、参数含义 按钮名称
        参数2：参数名称 *event、参数类型 GdkEventButton、输入参数、参数含义 按钮触发的事件
        参数3：参数名称 * data、参数类型 window_status、输入参数、参数含义 主界面信息
/*返回值：void
/*作者：郝程乾
/***************************************************/
//左边按钮栏的移入回调函数
void on_event_box_moved(GtkWidget* event_box,GdkEventCrossing *event,window_status* data){
    if(*(data->now_id)!=data->aim_id)
        gtk_widget_modify_bg(GTK_WIDGET(event_box),GTK_STATE_NORMAL,&moved);
}
/**************************************************/
/*名称：on_event_box_moved
/*描述：friend 和 group 鼠标移出变色的回调函数
/*作成日期： 2016.8.30
/*参数：参数1：参数名称 *button、参数类型 GtkWidget 、输入参数、参数含义 按钮名称
        参数2：参数名称 *event、参数类型 GdkEventButton、输入参数、参数含义 按钮触发的事件
        参数3：参数名称 * data、参数类型 window_status、输入参数、参数含义 主界面信息
/*返回值：void
/*作者：郝程乾
/***************************************************/
//左边按钮栏的移出回调函数
void on_leave_box_moved(GtkWidget* event_box,GdkEventCrossing *event,window_status* data){
    if(*(data->now_id)!=data->aim_id)
        gtk_widget_modify_bg(GTK_WIDGET(event_box),GTK_STATE_NORMAL,&white);
}
/**************************************************/
/*名称：title_bar_on_event_box_moved title_bar_on_leave_box_moved
/*描述：顶部菜单鼠标移入/移出的回调函数
/*作成日期： 2016.8.30
/*参数：参数1：参数名称 event_box、参数类型 GtkWidget 、输入参数、参数含义 按钮名称
        参数2：参数名称 *event、参数类型 GdkEventCrossing、输入参数、参数含义 按钮触发的事件
        参数3：参数名称 * data、参数类型 GdkColor、输入参数、参数含义 点击以后传入的颜色信息
/*返回值：void
/*作者：张宇森
/***************************************************/
void title_bar_on_event_box_moved(GtkWidget* event_box,GdkEventCrossing *event,GdkColor* data){
    gtk_widget_modify_bg(GTK_WIDGET(event_box),GTK_STATE_NORMAL,data);
}

void title_bar_on_leave_box_moved(GtkWidget* event_box,GdkEventCrossing *event,gpointer data){
    gtk_widget_modify_bg(GTK_WIDGET(event_box),GTK_STATE_NORMAL,&white);
}
/**************************************************/
/*名称：minimize_event/maximize_event
/*描述：窗口最大最小化
/*作成日期： 2016.8.30
/*参数：参数1：参数名称 event_box、参数类型 GtkWidget 、输入参数、参数含义 按钮名称
        参数2：参数名称 *event、参数类型 GdkEventCrossing、输入参数、参数含义 按钮触发的事件
        参数3：参数名称 * data、参数类型 GtkWidget、输入参数、参数含义 点击以后传入的信息
/*返回值：void
/*作者：张宇森
/***************************************************/
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
/**************************************************/
/*名称： button_press
/*描述：判断是否鼠标点击
/*作成日期： 2016.8.30
/*参数：参数1：参数名称 event_box、参数类型 GtkWidget 、输入参数、参数含义 按钮名称
        参数2：参数名称 *event、参数类型 GdkEventCrossing、输入参数、参数含义 按钮触发的事件
        参数3：参数名称 * data、参数类型 gint、输入参数、参数含义 点击以后传入的信息
/*返回值：gboolean
/*作者：郝程乾
/***************************************************/
gboolean button_press(GtkWidget *widget,GdkEventButton *event,gint data)
{
    if(event->type == GDK_BUTTON_PRESS) //判断鼠标是否被按下
    {
        if(event->button == 1)// 1代表鼠标左键!! 2 鼠标中间那个东东!! 3 就是剩下的那个
        { //gtk_widget_get_toplevel 返回顶层窗口 就是window
            gtk_window_begin_move_drag(GTK_WINDOW(gtk_widget_get_toplevel(widget)), event->button,event->x_root, event->y_root,event->time);
        }
    }
    return TRUE;
}
/**************************************************/
/*名称： get_event_box_with_label
/*描述：判断是否鼠标点击
/*作成日期： 2016.8.30
/*参数：参数1：参数名称 * label、参数类型 const char 、输入参数、参数含义 标签
/*返回值：GtkWidget*
/*作者：郝程乾
/***************************************************/
GtkWidget* get_event_box_with_label(const char* label){
    GtkWidget* eventbox=gtk_event_box_new();
    GtkWidget* lbl=gtk_label_new(label);
    gtk_container_add(GTK_CONTAINER(eventbox),lbl);
    gtk_widget_modify_bg(GTK_WIDGET(eventbox),GTK_STATE_NORMAL,&white);
    return eventbox;
}
/**************************************************/
/*名称： change_portrait
/*描述：改变头像按钮回调函数
/*作成日期： 2016.8.27
/*参数：参数1：参数名称 *widget、参数类型 GtkWidget 、输入参数、参数含义 进行操作的窗口 
        参数2：参数名称 data、参数类型 gpointer 、输入参数、参数含义 回调函数固定参数
/*返回值：void
/*作者：郑安庆
/***************************************************/
void change_portrait(GtkWidget *widget,gpointer data) // 更换头像按钮
{
    gtk_widget_destroy(global_image);
    gtk_widget_destroy(zaq_image);
    char buf_string[10];
    int i = (int)data;
    photo_id=i;
    exec_cmd(16,current_username,&photo_id);
    sprintf(buf_string,"portrait/%d.png",i);
    printf("%s\n",buf_string);
    global_image=gtk_image_new_from_file(buf_string);
    zaq_image=gtk_image_new_from_file(buf_string);
    gtk_container_add(GTK_CONTAINER(global_button),global_image);
    gtk_container_add(GTK_CONTAINER(hboxofzaqdown),zaq_image);
    gtk_widget_show_all(global_button);
    gtk_widget_show_all(hboxofzaqdown);
}
/**************************************************/
/*名称： on_portrait_delete_event
/*描述：改变头像按钮回调函数
/*作成日期： 2016.8.27
/*参数：参数1：参数名称 *widget、参数类型 GtkWidget 、输入参数、参数含义 进行操作的窗口
        参数2：参数名称 * event、参数类型 GdkEvent 、输入参数、参数含义 进行操作的时间
        参数3：参数名称 data、参数类型 gpointer 、输入参数、参数含义 窗口所有信息
/*返回值：void
/*作者：郑安庆
/***************************************************/
void on_portrait_delete_event (GtkWidget *widget,GdkEvent* event,gpointer data) // 聊天窗"关闭"按钮回调函数
{
	portrait_open = 0;
	gtk_widget_destroy(data);
}
/**************************************************/
/*名称： on_portrait_delete_event
/*描述：改变头像按钮回调函数
/*作成日期： 2016.8.27
/*参数：参数1：参数名称 *widget、参数类型 GtkWidget 、输入参数、参数含义 进行操作的窗口
        参数2：参数名称 data、参数类型 gpointer 、输入参数、参数含义 窗口所有信息
/*返回值：void
/*作者：郑安庆
/***************************************************/
void on_portrait(GtkWidget *widget,gpointer data) // "更换头像"窗口
{
	if  (portrait_open==1) return ;
	else portrait_open = 1;
    GtkWidget* window,*hbox,*button,*image;
    char buf_string[10];
    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(on_portrait_delete_event),window);
    gtk_window_set_title(GTK_WINDOW(window),"change your portrait!");
    gtk_window_set_default_size(GTK_WINDOW(window),300,200);
    gtk_container_set_border_width(GTK_CONTAINER(window),10);
    hbox=gtk_hbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(window),hbox);

	int i=0;
    for(;i<NumOfPortrait;i++)
    {
        button=gtk_button_new();
        sprintf(buf_string,"portrait/%d.png",i);
        image=gtk_image_new_from_file(buf_string);
        gtk_container_add(GTK_CONTAINER(button),image);
        gtk_box_pack_start(GTK_BOX(hbox),button,FALSE,FALSE,5);
        g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(change_portrait),(gpointer)i); // change_portrait
    }

    gtk_widget_show_all(window);

}
//==========================calculator================
/**************************************************/
/*名称： clear_all
/*描述：初始化计算器
/*作成日期： 2016.8.29
/*返回值：void
/*作者：张宇森
/***************************************************/
void clear_all()
{
    gint i;
    nn = 0;
    mm = 0;
    s = 0;
    count = 0;
    first = TRUE;
    have_dot = FALSE;
    have_result = FALSE;
    for (i = 0; i < sizeof(number); i++)
        number[i] = '\0';
    gtk_entry_set_text(GTK_ENTRY(entry), "");
}
/**************************************************/
/*名称： on_clear_clicked on_num_clicked on_suan_clicked on_dot_clicked on_eq_clicked
/*描述： 清空计算器按钮 数字按钮 计算按钮 小数点按钮 等于按钮 功能回调函数
/*作成日期： 2016.8.27
/*参数：参数1：参数名称 *button、参数类型 GtkButton 、输入参数、参数含义 进行操作的窗口
        参数2：参数名称 data、参数类型 gpointer 、输入参数、参数含义 窗口所有信息
/*返回值：void
/*作者：张宇森
/***************************************************/
void on_clear_clicked(GtkButton *button, gpointer data)
{
    clear_all();
}
void on_num_clicked(GtkButton *button, gpointer data)
{
    const gchar *num;
    gint i;
    if (have_result) clear_all();
    if (count == 6) return;
    count++;
    num = gtk_button_get_label(GTK_BUTTON(button));
    i = g_strlcat(number, num, 100);
    if (first) {
        nn = g_ascii_strtod(number, NULL);
    }
    else
        mm = g_ascii_strtod(number, NULL);
    gtk_entry_set_text(GTK_ENTRY(entry), number);
}
void on_suan_clicked(GtkButton *button, gpointer data)
{
    gint i;
    s = GPOINTER_TO_INT(data);
    first = FALSE;
    count = 0;
    have_dot = FALSE;
    gtk_entry_set_text(GTK_ENTRY(entry), "");
    for (i = 0; i < sizeof(number); i++)
        number[i] = '\0';
}
void on_dot_clicked(GtkButton *button, gpointer data)
{
    gint i;
    if (have_result) clear_all();
    if (have_dot == FALSE) {
        have_dot = TRUE;
        i = g_strlcat(number, ".", 100);
        gtk_entry_set_text(GTK_ENTRY(entry), number);
    }
}
void on_eq_clicked(GtkButton *button, gpointer data)
{
    gdouble numb;
    gchar num[100];
    gchar *result;
    switch(s) {
    case 1:
        numb = nn + mm;
        break;
    case 2:
        numb = nn - mm;
        break;
    case 3:
        numb = nn * mm;
        break;
    case 4:
        numb = nn / mm;
        break;
    }
    result = g_ascii_dtostr(num, 100, numb);
    have_result = TRUE;
    gtk_entry_set_text(GTK_ENTRY(entry), result);
}
/**************************************************/
/*名称： button_calculator
/*描述： 点击计算器按钮回调函数
/*作成日期： 2016.8.27
/*参数：参数1：参数名称 *button、参数类型 GtkButton 、输入参数、参数含义 进行操作的窗口
        参数2：参数名称 data、参数类型 gpointer 、输入参数、参数含义 窗口所有信息
/*返回值：void
/*作者：张宇森
/***************************************************/
void button_calculator(GtkWidget *widget,gpointer *data)
{
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *hbox, *hbox1, *hbox2, *hbox3, *hbox4;
    GtkWidget *button;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_title(GTK_WINDOW(window), "calculator");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);    //postion
    gtk_widget_set_usize (window, 230, 230);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    entry = gtk_entry_new();
    gtk_widget_set_direction(entry, GTK_TEXT_DIR_RTL);
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 5);

    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);
    button = gtk_button_new_with_label("CE");
    gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 5);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_clear_clicked), NULL);

    hbox1 = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox1, FALSE, FALSE, 5);
    button = gtk_button_new_with_label("3");
    gtk_box_pack_start(GTK_BOX(hbox1), button, TRUE, TRUE, 5);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_num_clicked), NULL);
    button = gtk_button_new_with_label("2");
    gtk_box_pack_start(GTK_BOX(hbox1), button, TRUE, TRUE, 5);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_num_clicked), NULL);
    button = gtk_button_new_with_label("1");
    gtk_box_pack_start(GTK_BOX(hbox1), button, TRUE, TRUE, 5);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_num_clicked), NULL);
    button = gtk_button_new_with_label("+");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_suan_clicked), (gpointer)1);
    gtk_box_pack_start(GTK_BOX(hbox1), button, TRUE, TRUE, 5);

    hbox2 = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox2, FALSE, FALSE, 5);
    button = gtk_button_new_with_label("6");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_num_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(hbox2), button, TRUE, TRUE, 5);
    button = gtk_button_new_with_label("5");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_num_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(hbox2), button, TRUE, TRUE, 5);
    button = gtk_button_new_with_label("4");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_num_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(hbox2), button, TRUE, TRUE, 5);
    button = gtk_button_new_with_label("-");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_suan_clicked), (gpointer)2);
    gtk_box_pack_start(GTK_BOX(hbox2), button, TRUE, TRUE, 5);

    hbox3 = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox3, FALSE, FALSE, 5);
    button = gtk_button_new_with_label("9");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_num_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(hbox3), button, TRUE, TRUE, 5);
    button = gtk_button_new_with_label("8");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_num_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(hbox3), button, TRUE, TRUE, 5);
    button = gtk_button_new_with_label("7");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_num_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(hbox3), button, TRUE, TRUE, 5);
    button = gtk_button_new_with_label("*");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_suan_clicked), (gpointer)3);
    gtk_box_pack_start(GTK_BOX(hbox3), button, TRUE, TRUE, 5);

    hbox4 = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox4, FALSE, FALSE, 5);
    button = gtk_button_new_with_label("0");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_num_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(hbox4), button, TRUE, TRUE, 5);
    button = gtk_button_new_with_label(".");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_dot_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(hbox4), button, TRUE, TRUE, 5);
    button = gtk_button_new_with_label("=");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_eq_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(hbox4), button, TRUE, TRUE, 5);
    button = gtk_button_new_with_label("/");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_suan_clicked), (gpointer)4);
    gtk_box_pack_start(GTK_BOX(hbox4), button, TRUE, TRUE, 5);

    gtk_widget_show_all(window);
    gtk_main();
    printf("calculator readly\n");
    return;
}
//=============================
//============================ 顶部菜单函数
//============================
/**************************************************/
/*名称： button_calenda
/*描述： 点击日历按钮回调函数
/*作成日期： 2016.8.27
/*参数：参数1：参数名称 *button、参数类型 GtkButton 、输入参数、参数含义 进行操作的窗口
        参数2：参数名称 data、参数类型 gpointer 、输入参数、参数含义 窗口所有信息
/*返回值：void
/*作者：张宇森
/***************************************************/
//============================Calendar========================
void button_calendar(GtkWidget *widget,gpointer *data)
{
	printf("calendar readly\n");
 	GtkWidget *calendar;
        GtkWidget *window;
	GtkWidget *box;
	GtkWidget *button;

	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_signal_connect(GTK_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
	box=gtk_vbox_new(FALSE,10);/*建立组装盒*/
	gtk_container_add(GTK_CONTAINER(window),box);/*将组装盒加入窗体*/

	calendar=gtk_calendar_new();/*建立日历构件*/
	gtk_box_pack_start(GTK_BOX(box),calendar,TRUE,TRUE,5);/*将日历构件加入组装盒*/
	gtk_widget_show(calendar);/*显示日历构件*/

	gtk_widget_show(box);
	gtk_widget_show(window);
	gtk_main();
}
//================================ScreenShot=====================
/**************************************************/
/*名称： show_picture
/*描述： 截图功能函数
/*作成日期： 2016.8.27
/*参数：参数1：参数名称 *button、参数类型 GtkButton 、输入参数、参数含义 进行操作的窗口
        参数2：参数名称 *data、参数类型 DATA 、输入参数、参数含义 窗口所有信息
/*返回值：void
/*作者：张宇森
/***************************************************/
void show_picture(GdkWindow *window,DATA *data)
{
    GtkWidget *win;
    GdkPixbuf *pixbuf;
    GtkWidget *image;
    gchar name[100];
    win=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(win),GTK_WIN_POS_CENTER);
    gtk_widget_set_app_paintable(win,TRUE);
    g_signal_connect(G_OBJECT(win),"delete_event",
            G_CALLBACK(gtk_main_quit),NULL);

    gdk_window_set_cursor(gdk_get_default_root_window(),
            gdk_cursor_new(GDK_LEFT_PTR)); 
    gdk_flush(); 
    pixbuf=gdk_pixbuf_get_from_drawable(NULL,window,NULL,
            data->x,data->y,0,0,
            data->width,data->height); 
    image=gtk_image_new_from_pixbuf(pixbuf);
    
    gtk_container_add(GTK_CONTAINER(win),image);

    gtk_widget_show_all(win);
    
    g_sprintf(name,"ScreenShot/%d.jpg",rand());
    gdk_pixbuf_save(pixbuf, name, "jpeg", NULL, "quality", "100", NULL);
    gtk_main();
}
/**************************************************/
/*名称： select_area_press select_area_release select_area_move
/*描述： 截图功能函数 截图开始 松开鼠标截图结束 选择截图区域 
/*作成日期： 2016.8.27
/*参数：参数1：参数名称 *widget、参数类型 GtkWidget 、输入参数、参数含义 进行操作的窗口
        参数2：参数名称 *event、参数类型 GdkEventButton  、输入参数、参数含义 窗口所有信息
        参数3：参数名称 *data、参数类型 DATA 、输入参数、参数含义 窗口所有信息
/*返回值：void
/*作者：张宇森
/***************************************************/
void select_area_press(GtkWidget *widget,GdkEventButton *event,DATA *data)  
{
    if(data->press == TRUE) 
        return; 
    gtk_window_move(GTK_WINDOW(widget),-100,-100);
    gtk_window_resize(GTK_WINDOW(widget),10,10);
    gtk_window_set_opacity(GTK_WINDOW(widget),0.8);
    data->press=TRUE;
    data->x=event->x_root;
    data->y=event->y_root; 
}

void select_area_release(GtkWidget *widget,GdkEventButton *event,DATA *data) 
{
    if(!data->press)
        return;

    data->width=ABS(data->x-event->x_root);
    data->height=ABS(data->y-event->y_root);
    data->x=MIN(data->x,event->x_root);
    data->y=MIN(data->y,event->y_root); 
    data->press=FALSE;

    gtk_widget_destroy(widget);
    gtk_main_quit();
}

void select_area_move(GtkWidget *widget,GdkEventMotion *event,DATA *data)
{
    GdkRectangle draw;

    if(!data->press)
        return;

    draw.width=ABS(data->x-event->x_root);
    draw.height=ABS(data->y-event->y_root);
    draw.x=MIN(data->x,event->x_root);
    draw.y=MIN(data->y,event->y_root); 

    if(draw.width <= 0 || draw.height <=0)
    {
        gtk_window_move(GTK_WINDOW(widget),-100,-100);
        gtk_window_resize(GTK_WINDOW(widget),10,10);
        return;
    }

    gtk_window_move(GTK_WINDOW(widget),draw.x,draw.y);
    gtk_window_resize(GTK_WINDOW(widget),draw.width,draw.height); 
}
/**************************************************/
/*名称： button_screenshot
/*描述： 截图按钮点击的回调函数
/*作成日期： 2016.8.27
/*参数：参数1：参数名称 *widget、参数类型 GtkWidget 、输入参数、参数含义 进行操作的窗口
        参数2：参数名称 *dat、参数类型 gpointer 、输入参数、参数含义 回调函数固定信息
/*返回值：void
/*作者：张宇森
/***************************************************/
int button_screenshot(GtkWidget *widget,gpointer *dat)
{
    GtkWidget *win;
    GdkScreen *screen;
    GdkColor color;
    DATA data;

    screen=gdk_screen_get_default();
    win=gtk_window_new(GTK_WINDOW_POPUP);
    gtk_widget_set_app_paintable(win,TRUE);

    data.press=FALSE;
    gtk_widget_add_events(win,GDK_BUTTON_MOTION_MASK|GDK_BUTTON_PRESS_MASK|GDK_BUTTON_RELEASE_MASK); 
    g_signal_connect(G_OBJECT(win),"button_press_event",
            G_CALLBACK(select_area_press),&data);
    g_signal_connect(G_OBJECT(win),"button_release_event",
            G_CALLBACK(select_area_release),&data);
    g_signal_connect(G_OBJECT(win),"motion_notify_event",
            G_CALLBACK(select_area_move),&data);

    color.blue=0;
    color.green=0;
    color.red=0;
    gtk_widget_modify_bg(win,GTK_STATE_NORMAL,&color);

    gtk_window_set_opacity(GTK_WINDOW(win),0); 
    gtk_window_resize(GTK_WINDOW(win),
            gdk_screen_get_width(screen),
            gdk_screen_get_height(screen)); 
    gdk_window_set_cursor(gdk_get_default_root_window(),
            gdk_cursor_new(GDK_CROSSHAIR));
    gdk_flush(); 

    gtk_widget_show_all(win);
    
    gtk_main();
    usleep(10000); 

    show_picture(gdk_get_default_root_window(),&data);
	
    gtk_main();
    return 0;
}
/**************************************************/
/*名称： button_about_gtk_draw
/*描述：   绘制about界面
/*作成日期： 2016.8.27
/*返回值：void
/*作者：郝程乾
/***************************************************/
void button_about_gtk_draw(){
    mainUI_window_about = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    mainUI_window_about_box = gtk_vbox_new(FALSE,0);
    mainUI_window_about_button = gtk_button_new();
    mainUI_window_about_image = gtk_image_new_from_file("About.jpg");
    gtk_container_add(GTK_CONTAINER(mainUI_window_about_button),mainUI_window_about_image);
    gtk_box_pack_start(GTK_BOX(mainUI_window_about_box),mainUI_window_about_button,FALSE,FALSE,5);
    gtk_container_add(GTK_CONTAINER(mainUI_window_about),mainUI_window_about_box);
    gtk_widget_show_all(mainUI_window_about);
}
/**************************************************/
/*名称： button_about
/*描述： 截图按钮回调函数
/*作成日期： 2016.8.27
/*参数：参数1：参数名称 *widget、参数类型 GtkWidget 、输入参数、参数含义 进行操作的窗口
        参数2：参数名称 *d、参数类型 gpointer 、输入参数、参数含义 回调函数固定信息
/*返回值：void
/*作者：郝程乾
/***************************************************/
void button_about(GtkWidget *widget,gpointer *d){
    button_about_gtk_draw();
}
//=============================================================
//creat Item
/**************************************************/
/*名称： CreateMenuItem
/*描述： 创建菜单项
/*作成日期： 2016.8.27
/*参数：参数1：参数名称 *MenuBar、参数类型 GtkWidget 、输入参数、参数含义 进行操作的窗口
        参数2：参数名称 *test、参数类型 char 、输入参数、参数含义 菜单名字
/*返回值：GtkWidget *
/*作者：张宇森
/***************************************************/
GtkWidget *CreateMenuItem(GtkWidget *MenuBar,char *test)
{
	GtkWidget *MenuItem;

	MenuItem=gtk_menu_item_new_with_label(test);
	gtk_menu_shell_append(GTK_MENU_SHELL(MenuBar),MenuItem);
	gtk_widget_show(MenuItem);
	return MenuItem;
}
//creat submenu
/**************************************************/
/*名称： CreateMenu
/*描述： 创建菜单项
/*作成日期： 2016.8.27
/*参数：参数1：参数名称 *MenuItem1、参数类型 GtkWidget 、输入参数、参数含义 进行操作的菜单窗口
        参数2：参数名称 *MenuItem1、参数类型 GtkWidget 、输入参数、参数含义 进行操作的菜单窗口
        参数3：参数名称 *MenuItem1、参数类型 GtkWidget 、输入参数、参数含义 进行操作的菜单窗口
/*返回值：GtkWidget *
/*作者：张宇森
/***************************************************/
GtkWidget *CreateMenu(GtkWidget *MenuItem1,GtkWidget *MenuItem2,GtkWidget *MenuItem3)
{
	GtkWidget *Menu;
	GtkWidget *Exit,*Calendar,*Calculator,*About,*Background,*Screenshot,*Font;
		
	Menu=gtk_menu_new();
	Calculator=CreateMenuItem(Menu,"Calculator");
	Calendar=CreateMenuItem(Menu,"Calendar");
	Screenshot=CreateMenuItem(Menu,"Screenshot");
	Exit=CreateMenuItem(Menu,"Exit");

	gtk_signal_connect(GTK_OBJECT(Exit),"activate",G_CALLBACK(gtk_main_quit),NULL);
	gtk_signal_connect(GTK_OBJECT(Calendar),"activate",GTK_SIGNAL_FUNC(button_calendar),NULL);
	gtk_signal_connect(GTK_OBJECT(Calculator),"activate",GTK_SIGNAL_FUNC(button_calculator),NULL);
	gtk_signal_connect(GTK_OBJECT(Screenshot),"activate",GTK_SIGNAL_FUNC(button_screenshot),NULL);
    
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(MenuItem1),Menu);
	gtk_widget_show(Menu);

	Menu=gtk_menu_new();
	Background=CreateMenuItem(Menu,"Background");
	Font=CreateMenuItem(Menu,"Font");

	gtk_signal_connect(GTK_OBJECT(Background),"activate",GTK_SIGNAL_FUNC(button_background_transform_function),"clicked");//BACKGROUND
	gtk_signal_connect(GTK_OBJECT(Font),"activate",GTK_SIGNAL_FUNC(fontSelect),"clicked");//Font
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(MenuItem2),Menu);
	gtk_widget_show(Menu);

	Menu=gtk_menu_new();
	About=CreateMenuItem(Menu,"About");

	gtk_signal_connect(GTK_OBJECT(About),"activate",GTK_SIGNAL_FUNC(button_about),"clicked");//About
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(MenuItem3),Menu);
	gtk_widget_show(Menu);
}

//=============================
//============================ 顶部菜单函数
//============================
/**************************************************/
/*名称： mainUI
/*描述： 主界面绘制
/*作成日期： 2016.8.23
/*返回值：GtkWidget*
/*作者：郝程乾
/***************************************************/
GtkWidget *name_label;
GtkWidget* mainUI(){
    //===============需要的变量================

	int i;
	
	//===================Using in the Communication Window=======================
	//struct text_view_info viewinfo;
	GtkWidget *scwin1, *scwin2;
	GtkWidget *cmtable;
	GtkWidget *button1, *button2, *buttonFileTransform ,*button_Font, *button_background;
    	
    GtkWidget *logo_label;
    GtkWidget *msg_box, *send_box, *button_box, *group_friend_list_box, *button_box_FIle_Stiker;
    GtkWidget *hbox_qiangchatworld;
	//===============================
    GtkWidget* window;//主窗体
    GtkWidget* main_window;//加入自制关闭等按钮
    GtkWidget* main_box;//实现功能的主box
    GtkWidget* title_bar_box;
    GtkWidget* btn_box;
    GtkWidget* friend_list_box;
    GtkWidget* group_list_box;
    GtkWidget* chat_box;
    GtkWidget* quit_btn;
    GtkWidget* minimize_btn;
    GtkWidget* maximize_btn;
    GtkWidget* friend_btn;
    GtkWidget* group_btn;
    GtkWidget* about_btn;
    GtkWidget* init_welcome_box=gtk_vbox_new(FALSE,0);
    GtkWidget* init_welcome_label;
    GtkWidget* group_would;
    GtkWidget* menu_bar_box;
    //window_status w_status[3];
    //maximize_state maxi_state;
    
    //int maximize_flag=0;
    //int init_welcome_flag=1;
//================菜单变量
	GtkWidget *MenuBar;
	GtkWidget *MenuItemFile1;
	GtkWidget *MenuItemFile2;
	GtkWidget *MenuItemFile3;


//================

    maxi_state.state=&maximize_flag;

    	//=====================调用姿势==================
	//群组列表
	sky_group = create_grouplist(CREATE_GROUPLIST);
	group_page = sky_group.update_box;
	
	//好友列表
	sky_friend = create_friendlist(CREATE_FRIENDLIST);
	friend_page = sky_friend.update_box;

	friend_list_box=sky_friend.Mainbox;
	//gtk_widget_modify_bg(friend_list_box,GTK_STATE_NORMAL,&white);
	gtk_widget_set_size_request(GTK_WIDGET(friend_list_box),300,300);
	group_list_box=sky_group.Mainbox;
	//gtk_widget_modify_bg(group_list_box,GTK_STATE_NORMAL,&white);
	gtk_widget_set_size_request(GTK_WIDGET(group_list_box),300,300);
    chat_box=gtk_vbox_new(FALSE,0);


    //=====================加入自制关闭 最小化 最大化 按钮=============================
    title_bar_box=gtk_hbox_new(FALSE,0);
    quit_btn=get_event_box_with_label("X");
    gtk_widget_set_size_request(GTK_WIDGET(quit_btn),20,20);
    g_signal_connect(G_OBJECT(quit_btn),"enter_notify_event",G_CALLBACK(title_bar_on_event_box_moved),&red);
    g_signal_connect(G_OBJECT(quit_btn),"leave_notify_event",G_CALLBACK(title_bar_on_leave_box_moved),NULL);
    g_signal_connect(G_OBJECT(quit_btn),"button_press_event",G_CALLBACK(gtk_main_quit),NULL);
    minimize_btn=get_event_box_with_label("一");
    gtk_widget_set_size_request(GTK_WIDGET(minimize_btn),20,20);
    g_signal_connect(G_OBJECT(minimize_btn),"enter_notify_event",G_CALLBACK(title_bar_on_event_box_moved),&skyblue);
    g_signal_connect(G_OBJECT(minimize_btn),"leave_notify_event",G_CALLBACK(title_bar_on_leave_box_moved),NULL);
    maximize_btn=get_event_box_with_label("O");
    gtk_widget_set_size_request(GTK_WIDGET(maximize_btn),20,20);
    g_signal_connect(G_OBJECT(maximize_btn),"enter_notify_event",G_CALLBACK(title_bar_on_event_box_moved),&skyblue);
    g_signal_connect(G_OBJECT(maximize_btn),"leave_notify_event",G_CALLBACK(title_bar_on_leave_box_moved),NULL);
    gtk_box_pack_end(GTK_BOX(title_bar_box),quit_btn,FALSE,FALSE,0);
    gtk_box_pack_end(GTK_BOX(title_bar_box),maximize_btn,FALSE,FALSE,0);
    gtk_box_pack_end(GTK_BOX(title_bar_box),minimize_btn,FALSE,FALSE,0);
    GtkWidget* SkyQ_title_label=gtk_label_new("QiangChat");
    gtk_box_pack_end(GTK_BOX(title_bar_box),SkyQ_title_label,TRUE,TRUE,0);
//==============================================================


    MenuBar = gtk_menu_bar_new();
    MenuItemFile1=CreateMenuItem(MenuBar,"Menu");
	MenuItemFile2=CreateMenuItem(MenuBar,"Settings");
	MenuItemFile3=CreateMenuItem(MenuBar,"Help");
    CreateMenu(MenuItemFile1,MenuItemFile2,MenuItemFile3);
//    gtk_widget_show(MenuBar);
    menu_bar_box = gtk_hbox_new(FALSE,0);

    gtk_box_pack_start(GTK_BOX(menu_bar_box),MenuBar,FALSE,FALSE,0);
	mainUI_window_recipt = menu_bar_box;

//==================main_window======================================
    main_window=gtk_vbox_new(FALSE,0);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_decorated(GTK_WINDOW(window),TRUE);
    gtk_window_set_title(GTK_WINDOW(window),"QiangChat 1.0 beta");
    gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);
    main_box=gtk_hbox_new(FALSE,0);
    gtk_widget_modify_bg(window,GTK_STATE_NORMAL,&white);
    g_signal_connect(G_OBJECT(window), "button-press-event",G_CALLBACK(button_press), NULL);
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
    g_signal_connect(G_OBJECT(minimize_btn),"button_press_event",G_CALLBACK(minimize_event),window);
    maxi_state.window=window;
    g_signal_connect(G_OBJECT(maximize_btn),"button_press_event",G_CALLBACK(maximize_event),&maxi_state);
    //gtk_window_set_title(GTK_WINDOW(window),"SkyQ");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window),0);
    gtk_window_set_default_size(GTK_WINDOW(window),1000,600);
    gtk_widget_set_size_request(window,1000,600);
    gtk_box_pack_start(GTK_BOX(main_window),title_bar_box,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(main_window),menu_bar_box,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(main_window),main_box,TRUE,TRUE,0);
    gtk_container_add(GTK_CONTAINER(window),main_window);
//=================================================================

//=================================================================
    friend_btn=get_event_box_with_label("friend");
    group_btn=get_event_box_with_label("group");

    GtkWidget* view_box,*head_image,*ip_pos,*name_pos,*ipname_box,*headip_box;
    view_box=gtk_vbox_new(FALSE,0);
    //add
    headip_box=gtk_hbox_new(TRUE,0);
    head_image= gtk_hbox_new(TRUE,0);
    char buf_string[20];
    photo_id=photo_id % NumOfPortrait;
    sprintf(buf_string,"portrait/%d.png", photo_id); // 图片张数
    global_button=gtk_button_new();
    global_image=gtk_image_new_from_file(buf_string);

    gtk_box_pack_start(GTK_BOX(head_image),global_button,FALSE,FALSE,5);
    g_signal_connect(G_OBJECT(global_button),"clicked",G_CALLBACK(on_portrait),NULL); // on_portrait
    gtk_container_add(GTK_CONTAINER(global_button),global_image);
    ipname_box=gtk_vbox_new(TRUE,0);
    name_label=gtk_label_new("defalut");
    name_pos=gtk_vbox_new(TRUE,0);
    ip_pos=gtk_label_new("昵称");
    btn_box=gtk_hbox_new(TRUE,0);

    gtk_box_pack_start(GTK_BOX(main_box),view_box,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(view_box),headip_box,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(headip_box),head_image,TRUE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(headip_box),ipname_box,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(ipname_box),ip_pos,TRUE,TRUE,0);

    gtk_box_pack_start(GTK_BOX(ipname_box),name_pos,TRUE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(name_pos),name_label,TRUE,TRUE,0);
    gtk_widget_set_size_request(GTK_WIDGET(headip_box),200,50);
    gtk_box_pack_start(GTK_BOX(view_box),btn_box,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(btn_box),friend_btn,TRUE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(btn_box),group_btn,TRUE,TRUE,0);
    gtk_widget_set_size_request(GTK_WIDGET(btn_box),200,50);

    gtk_box_pack_start(GTK_BOX(view_box),friend_list_box,FALSE,FALSE,1);
    gtk_box_pack_start(GTK_BOX(view_box),group_list_box,FALSE,FALSE,1);
    gtk_widget_set_size_request(GTK_WIDGET(friend_list_box),200,450);
    gtk_widget_set_size_request(GTK_WIDGET(group_list_box),200,450);
 
    gtk_widget_modify_bg(GTK_WIDGET(friend_btn),GTK_STATE_NORMAL,&focus);

    hboxofzaqup = gtk_hbox_new(FALSE,0);
    hboxofzaqdown = gtk_hbox_new(FALSE,0);
    group_friend_list_box = gtk_vbox_new(FALSE,0);
    group_would = gtk_vbox_new(FALSE,0);
    //int i;
    for(i=0;i<2;i++){
        w_status[i].main_box=main_box;
        w_status[i].aim_id=i;
        w_status[i].now_id=&now_id;
        w_status[i].main_window[0]=friend_list_box;
        w_status[i].main_window[1]=group_list_box;
        w_status[i].chat_box=chat_box;
        w_status[i].main_button[0]=friend_btn;
        w_status[i].main_button[1]=group_btn;
        w_status[i].group_friend_list = group_friend_list_box;
        w_status[i].init_welcome_flag=&init_welcome_flag;
        w_status[i].init_welcome_box=init_welcome_box;
    }

    g_signal_connect(G_OBJECT(friend_btn),"enter_notify_event",G_CALLBACK(on_event_box_moved),&w_status[0]);
    g_signal_connect(G_OBJECT(friend_btn),"leave_notify_event",G_CALLBACK(on_leave_box_moved),&w_status[0]);
    g_signal_connect(G_OBJECT(friend_btn),"button_press_event",G_CALLBACK(on_main_button_clicked),&w_status[0]);


    g_signal_connect(G_OBJECT(group_btn),"enter_notify_event",G_CALLBACK(on_event_box_moved),&w_status[1]);
    g_signal_connect(G_OBJECT(group_btn),"leave_notify_event",G_CALLBACK(on_leave_box_moved),&w_status[1]);
    g_signal_connect(G_OBJECT(group_btn),"button_press_event",G_CALLBACK(on_main_button_clicked),&w_status[1]);

    ////////////////////////////Communication Window//////////////////////////////////////
	cmtable = gtk_table_new(400,1,FALSE);

    ///Part1
	cmeventbox1 = gtk_event_box_new();
	gtk_widget_modify_bg(cmeventbox1,GTK_STATE_NORMAL,&Blue);

    cmlabel = gtk_label_new(NULL);

    gtk_container_add(GTK_CONTAINER(cmeventbox1),cmlabel);
    ///Part2
    msg_box = gtk_hbox_new(FALSE,0);
    scwin1= gtk_scrolled_window_new(NULL,NULL);

    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scwin1),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    viewinfo.view1 = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(viewinfo.view1),FALSE);
    gtk_container_add(GTK_CONTAINER(scwin1),viewinfo.view1);

    scwinlist = gtk_scrolled_window_new(NULL,NULL);
    group_friend_list = gtk_list_new();
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scwinlist),group_friend_list);
	gtk_list_set_selection_mode(GTK_LIST(group_friend_list),GTK_SELECTION_SINGLE); 	//设置列表框的选择模式(单选或多选)

    cmeventbox2 = gtk_event_box_new();
    zaq_button=gtk_button_new();
    zaq_image=gtk_image_new_from_file(buf_string);
    gtk_container_add(GTK_CONTAINER(hboxofzaqdown),zaq_image);
   
    gtk_widget_modify_bg(cmeventbox2,GTK_STATE_NORMAL,&AliceBlue);
    logo_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(logo_label),"<span foreground=\"blue\"><big>Welcome to QiangChat</big></span>");
    gtk_container_add(GTK_CONTAINER(cmeventbox2),logo_label);
    gtk_box_pack_start(GTK_BOX(group_friend_list_box),hboxofzaqup,TRUE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(group_friend_list_box),cmeventbox2,TRUE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(group_friend_list_box),hboxofzaqdown,TRUE,TRUE,0);

    gtk_box_pack_start(GTK_BOX(group_would),scwinlist,TRUE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(group_would),cmeventbox2,TRUE,TRUE,0);

    gtk_widget_set_size_request (hboxofzaqup, 180,20);
    gtk_widget_set_size_request (hboxofzaqdown, 180, 20);
    gtk_box_pack_start(GTK_BOX(msg_box),scwin1,TRUE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(msg_box),group_friend_list_box,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(msg_box),group_would,FALSE,FALSE,0);
    gtk_widget_set_size_request (group_friend_list_box, 180, 50);
    gtk_widget_set_size_request(group_would,180,50);

    ///Part3
    cmeventbox3 = gtk_event_box_new();
	gtk_widget_modify_bg(cmeventbox3,GTK_STATE_NORMAL,&Blue);

    ///Part4
    send_box = gtk_hbox_new(FALSE,0);

    scwin2 = gtk_scrolled_window_new(NULL,NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scwin2),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    viewinfo.view2 = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(scwin2),viewinfo.view2);

    button_box = gtk_vbox_new(TRUE,0);

    button2 = gtk_button_new_with_label("temp");
    g_signal_connect(G_OBJECT(button2),"clicked",G_CALLBACK(send_button_press),&viewinfo);
    gtk_widget_modify_bg(button2,GTK_STATE_NORMAL,&Blue);
    GtkWidget *labelChild2 = gtk_bin_get_child(GTK_WIDGET(button2));
    gtk_label_set_markup(GTK_LABEL(labelChild2), "<span foreground=\"#FFFFFF\">发送</span>");

    gtk_box_pack_start(GTK_BOX(button_box),button2,TRUE,TRUE,0);

    gtk_box_pack_start(GTK_BOX(send_box),scwin2,TRUE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(send_box),button_box,FALSE,FALSE,0);
    gtk_widget_set_size_request (button_box, 180, 50);
    //part 5 聊天框上方文件传送
    cmeventbox4=gtk_event_box_new();
    button_box_FIle_Stiker = gtk_hbox_new(TRUE,0);
    button1 = gtk_button_new_with_label("tmep");
    g_signal_connect(G_OBJECT(button1),"button_release_event",G_CALLBACK(sticker_button_press),&viewinfo);
    GtkWidget *labelChild1 = gtk_bin_get_child(GTK_WIDGET(button1));
    gtk_widget_modify_bg(button1,GTK_STATE_NORMAL,&Blue);
    gtk_label_set_markup(GTK_LABEL(labelChild1),"<span foreground=\"#FFFFFF\">表情</span>");

    buttonFileTransform=gtk_button_new_with_label("tmep");
    mainUI_window_group_transform = buttonFileTransform;
    g_signal_connect(G_OBJECT(buttonFileTransform),"button_release_event",G_CALLBACK(FileTransform_button_press),&viewinfo);
    GtkWidget *labelChildFileTransform = gtk_bin_get_child(GTK_WIDGET(buttonFileTransform));
    gtk_widget_modify_bg(buttonFileTransform,GTK_STATE_NORMAL,&Blue);
    gtk_label_set_markup(GTK_LABEL(labelChildFileTransform),"<span foreground=\"#FFFFFF\">文件传输</span>");

    button_Font = gtk_button_new_with_label("temp");
    g_signal_connect(G_OBJECT(button_Font),"button_release_event",G_CALLBACK(fontSelect),&viewinfo);//callback()需要添加换字体回调函数
    GtkWidget *labelChileButton_Font = gtk_bin_get_child(GTK_WIDGET(button_Font));
    gtk_widget_modify_bg(button_Font,GTK_STATE_NORMAL,&Blue);
    gtk_label_set_markup(GTK_LABEL(labelChileButton_Font),"<span foreground=\"#FFFFFF\">修改字体</span>");

    button_background = gtk_button_new_with_label("temp");
    g_signal_connect(G_OBJECT(button_background),"button_release_event",G_CALLBACK(button_background_transform_function),&viewinfo);
    GtkWidget *labelChileButton_background = gtk_bin_get_child(GTK_WIDGET(button_background));
    gtk_widget_modify_bg(button_background,GTK_STATE_NORMAL,&Blue);
    gtk_label_set_markup(GTK_LABEL(labelChileButton_background),"<span foreground=\"#FFFFFF\">修改主题</span>");

    gtk_box_pack_start(GTK_BOX(button_box_FIle_Stiker),button1,TRUE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(button_box_FIle_Stiker),buttonFileTransform,TRUE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(button_box_FIle_Stiker),button_Font,TRUE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(button_box_FIle_Stiker),button_background,TRUE,TRUE,0);
    gtk_widget_set_size_request (button_box_FIle_Stiker,320,30);
    ///把控件放入cmtable
    gtk_table_attach_defaults(GTK_TABLE(cmtable),cmeventbox1,0,1,0,50);
    gtk_table_attach_defaults(GTK_TABLE(cmtable),msg_box,0,1,50,320);
    gtk_table_attach_defaults(GTK_TABLE(cmtable),cmeventbox3,0,1,320,325);
    gtk_table_attach_defaults(GTK_TABLE(cmtable),send_box,0,1,345,400);
    gtk_table_attach_defaults(GTK_TABLE(cmtable),button_box_FIle_Stiker,0,1,325,345);

    ///Set the tags of the TextView
    viewinfo.view1_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(viewinfo.view1));
    viewinfo.view2_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(viewinfo.view2));

    gtk_text_buffer_create_tag(viewinfo.view1_buffer, "blue_foreground","foreground", "blue", NULL);//创建前景标记
    gtk_text_buffer_create_tag(viewinfo.view1_buffer, "yellow_background","background","yellow",NULL);//创建背景标记
    gtk_text_buffer_create_tag(viewinfo.view1_buffer, "simhei", "family", "Simhei",NULL);
    gtk_text_buffer_create_tag(viewinfo.view1_buffer, "sans", "family", "Sans",NULL);//以上两行创建字体标记
    gtk_text_buffer_create_tag (viewinfo.view1_buffer, "heading","justification",GTK_JUSTIFY_LEFT,NULL);//居左
    gtk_text_buffer_create_tag (viewinfo.view1_buffer, "no_wrap","wrap_mode", GTK_WRAP_NONE, NULL);//不换行
    gtk_text_buffer_create_tag (viewinfo.view1_buffer, "word_wrap","wrap_mode", GTK_WRAP_WORD, NULL);//以词为单位换行
    gtk_text_buffer_create_tag(viewinfo.view1_buffer, "center","justification", GTK_JUSTIFY_CENTER, NULL);//居中
    gtk_text_buffer_create_tag(viewinfo.view1_buffer, "right_justify","justification", GTK_JUSTIFY_RIGHT, NULL);//居右

    gtk_box_pack_start(GTK_BOX(chat_box),cmtable,TRUE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(main_box),chat_box,TRUE,TRUE,0);
    ////////////////////////////Communication Window//////////////////////////////////////


//==========================welcome_page===================================
    init_welcome_label=gtk_label_new("Welcome to world of ZSQ!");
    gtk_box_pack_start(GTK_BOX(init_welcome_box),init_welcome_label,TRUE,TRUE,0);
//======================================================================
	mainUI_window_scwinlist=scwinlist;
	mainUI_window_group_list_box=group_list_box;
    mainUI_window_group_would=group_would;
	return window;
}
/**************************************************/
/*名称： mainUI_window_show
/*描述： 使得可视化主窗口
/*作成日期： 2016.8.22
/*参数：参数1：参数名称 * cu、参数类型 char 、输入参数、参数含义 给name_label标签
/*返回值：void
/*作者：郝程乾
/***************************************************/
void mainUI_window_show(char* cu){
    gtk_label_set_text(name_label,cu);
	gtk_widget_show_all(mainUI_window);
	gtk_widget_hide_all(mainUI_window_group_list_box);
    gtk_widget_hide_all(mainUI_window_about_box);
   	gtk_widget_hide_all(mainUI_window_scwinlist);
    gtk_widget_hide_all(mainUI_window_group_would);
    gtk_widget_hide_all(mainUI_window_about_box);
}
/**************************************************/
/*名称： get_UI_state()
/*描述： 获取当前用户的ui信息
/*作成日期： 2016.8.22
/*返回值：struct UI_state
/*作者：郝程乾
/***************************************************/
struct UI_state get_UI_state() {
	struct UI_state state;
	state.tab = now_id;
	strcpy(state.current_username, talk_to_user);
	state.current_groupID = &talk_to_group;
	return state;
}

