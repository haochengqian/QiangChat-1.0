/**************************************************/
/*名称：cmUI.c
/*描述：显示聊天窗口界面的功能
/*作成日期：2016-08-31

/*作者：郑安庆
/***************************************************/
#include "cmUI.h"

static GdkColor Blue = {0,0x2121,0x9696,0xf3f3};
static GdkColor AliceBlue = {0,0xf0f0,0xf8f8,0xffff};

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
void update_group_friend_list();
void change_chat_mode(int mode);
void update_group_friend_list(int num,struct group_username * name);
void set_chat_title(GtkWidget *cmlabel, const char *buf);

extern GtkWidget *cmeventbox2;
extern GtkWidget *group_friend_list;
extern struct text_view_info viewinfo;
extern GtkWidget *cmlabel;
extern GtkWidget *scwinlist;
extern GtkWidget *mainUI_window;
extern char talk_to_user[100];

PangoFontDescription *font_desc;

/**************************************************/
/*名称：send_button_press
/*描述：发送按钮的回调函数
/*作成日期： 2016-08-31
/*参数：
        参数1：widget、GtkWidget*、输入参数、发送按钮
        参数2：*viewinfo、struct text_view_info、输入参数、发送的概况
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void send_button_press(GtkWidget *widget,struct text_view_info *viewinfo){
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(viewinfo->view2_buffer,&start);
    gtk_text_buffer_get_end_iter(viewinfo->view2_buffer,&end);
    char *buffer=gtk_text_buffer_get_text(viewinfo->view2_buffer,&start,&end,TRUE);
    int len=strlen(buffer);
    if (len > 0){
        gtk_text_buffer_set_text(viewinfo->view2_buffer,"", -1);
	send_message_callback(buffer);
    }
    else create_message_dialog(GTK_MESSAGE_INFO,"The message is not allowed to be empty!");
}

/**************************************************/
/*名称：print_msg
/*描述：打印发送的消息
/*作成日期： 2016-08-31
/*参数：
        参数1：viewinfo、struct text_view_info*、输入参数、发送的概况
        参数2：info、 const char*、输入参数、发送的对象
        参数3：content、 const char*、输入参数、发送的内容
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void print_msg(struct text_view_info *viewinfo, const char* info, const char* content){
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(viewinfo->view1_buffer,&end);
    gtk_text_buffer_insert_with_tags_by_name(viewinfo->view1_buffer,&end,info,-1,"blue_foreground",NULL);
    if(font_desc != NULL) {
        GtkStyle *style;
        style = gtk_style_copy (gtk_widget_get_style (viewinfo->view1));
        style->font_desc = font_desc;
        gtk_widget_set_style (viewinfo->view1, style);
    }
    print_content(viewinfo,content,1);
    scroll_to_the_end(viewinfo);
}

/**************************************************/
/*名称：print_content
/*描述：打印发送的内容
/*作成日期： 2016-08-31
/*参数：
        参数1：viewinfo、struct text_view_info*、输入参数、发送的概况
        参数2：content、 const char*、输入参数、发送的内容
        参数3：state、 int、输入参数、状态
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void print_content(struct text_view_info *viewinfo, const char* content, int state){
    GtkTextIter end;
    int i=0;
    if (strlen(content) == 0) return;
    while (1){
        if (content[i] == '\0') {
            gtk_text_buffer_get_end_iter(viewinfo->view1_buffer,&end);
            gtk_text_buffer_insert(viewinfo->view1_buffer,&end,content,-1);
            break;
        }
        else if (content[i] == '/') {
            //打印处'/'前面的部分
            if (i > 0){
                char precontent[i+1];
                int j=0;
                for (j=0;j<i;j++) precontent[j]=content[j];
                precontent[j]='\0';
                gtk_text_buffer_get_end_iter(viewinfo->view1_buffer,&end);
                gtk_text_buffer_insert(viewinfo->view1_buffer,&end,precontent,-1);
            }

            //如果是表情，插入缓冲区,否则将这段字符插入
            if (content[i+1] != '\0' && content[i+2] != '\0'){
                char tmp[4];
                tmp[0]='/';
                tmp[1]=content[i+1];
                tmp[2]=content[i+2];
                tmp[3]='\0';
                print_sticker(viewinfo,tmp);
                //打印后面部分
                print_content(viewinfo,&content[i+3],0);
                break;
            }
            else {
                gtk_text_buffer_get_end_iter(viewinfo->view1_buffer,&end);
                gtk_text_buffer_insert(viewinfo->view1_buffer,&end,"/",-1);
                print_content(viewinfo,&content[i+1],0);
                break;
            }
        }
        i++;
    }
    if (state == 1) {
        gtk_text_buffer_get_end_iter(viewinfo->view1_buffer,&end);
        gtk_text_buffer_insert(viewinfo->view1_buffer,&end,"\n",-1);
    }
}
/**************************************************/
/*名称：print_sticker
/*描述：打印发送的表情
/*作成日期： 2016-08-31
/*参数：
        参数1：viewinfo、struct text_view_info*、输入参数、发送的概况
        参数2：content、 const char*、输入参数、发送的内容
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void print_sticker(struct text_view_info *viewinfo, const char* content){
    GtkTextIter end;
    GtkWidget *image;
    GtkTextChildAnchor *anchor;
    gtk_text_buffer_get_end_iter(viewinfo->view1_buffer,&end);
    int flag = 1;
    if (strcmp(content,"/am") == 0) image = gtk_image_new_from_file("./sticker/am.gif");
    else if (strcmp(content,"/dk") == 0) image = gtk_image_new_from_file("./sticker/dk.gif");
    else if (strcmp(content,"/fd") == 0) image = gtk_image_new_from_file("./sticker/fd.gif");
    else if (strcmp(content,"/sk") == 0) image = gtk_image_new_from_file("./sticker/sk.gif");
    else if (strcmp(content,"/wx") == 0) image = gtk_image_new_from_file("./sticker/wx.gif");
    else if (strcmp(content,"/zj") == 0) image = gtk_image_new_from_file("./sticker/zj.gif");
    else flag = 0;
    if (flag){
        anchor = gtk_text_buffer_create_child_anchor(viewinfo->view1_buffer,&end);//创建子控件的位置标记
        gtk_text_view_add_child_at_anchor(GTK_TEXT_VIEW(viewinfo->view1),image,anchor);
        gtk_widget_show(image);
    }
    else gtk_text_buffer_insert(viewinfo->view1_buffer,&end,content,-1);
}

/**************************************************/
/*名称：sticker_button_press
/*描述：表情按钮的回调函数
/*作成日期： 2016-08-31
/*参数：
        参数1：widget、GtkWidget*、输入参数、表情按钮
        参数2：event、GdkEvent*、输入参数、事件
        参数3：viewinfo、struct text_view_info*、输入参数、发送的概况
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void sticker_button_press(GtkWidget *widget, GdkEvent *event, struct text_view_info *viewinfo){
    if(event->type == GDK_BUTTON_RELEASE){
        GdkEventButton *event_button = (GdkEventButton *) event;
        if(event_button->button == 1){
            GtkWidget *sticker_window;
            GtkWidget *eventbox;
            GtkWidget *sbox1, *sbox2, *sbox3, *sbox4, *sbox5, *sbox6;  //盛放6个表情的eventbox
            GtkWidget *table;
            GtkWidget *s1, *s2, *s3, *s4, *s5, *s6; //6个表情的image
            sticker_window = gtk_window_new(GTK_WINDOW_POPUP);
            gtk_window_set_default_size(GTK_WINDOW(sticker_window),100,100);
            gtk_widget_set_size_request(sticker_window,200,100);
            gtk_window_set_position(GTK_WINDOW(sticker_window), GTK_WIN_POS_MOUSE);
            eventbox = gtk_event_box_new();
            g_signal_connect(G_OBJECT(eventbox),"leave_notify_event",G_CALLBACK(destory_sticker_window),sticker_window);
            gtk_container_add(GTK_CONTAINER(sticker_window),eventbox);
            table = gtk_table_new(2,3,FALSE);
            gtk_container_add(GTK_CONTAINER(eventbox),table);
            sbox1 = gtk_event_box_new();
            sbox2 = gtk_event_box_new();
            sbox3 = gtk_event_box_new();
            sbox4 = gtk_event_box_new();
            sbox5 = gtk_event_box_new();
            sbox6 = gtk_event_box_new();
            struct sticker_info **spointer = (struct sticker_info **)malloc(sizeof(struct sticker_info *)*6);
            struct sticker_info *sinfo1 = (struct sticker_info *)malloc(sizeof(struct sticker_info));
            sinfo1->str = "/am";sinfo1->view2_buffer = viewinfo->view2_buffer;
            sinfo1->sticker_window = sticker_window;sinfo1->spointer = spointer;
            struct sticker_info *sinfo2 = (struct sticker_info *)malloc(sizeof(struct sticker_info));
            sinfo2->str = "/dk";sinfo2->view2_buffer = viewinfo->view2_buffer;
            sinfo2->sticker_window = sticker_window;sinfo2->spointer = spointer;
            struct sticker_info *sinfo3 = (struct sticker_info *)malloc(sizeof(struct sticker_info));
            sinfo3->str = "/fd";sinfo3->view2_buffer = viewinfo->view2_buffer;
            sinfo3->sticker_window = sticker_window;sinfo3->spointer = spointer;
            struct sticker_info *sinfo4 = (struct sticker_info *)malloc(sizeof(struct sticker_info));
            sinfo4->str = "/sk";sinfo4->view2_buffer = viewinfo->view2_buffer;
            sinfo4->sticker_window = sticker_window;sinfo4->spointer = spointer;
            struct sticker_info *sinfo5 = (struct sticker_info *)malloc(sizeof(struct sticker_info));
            sinfo5->str = "/wx";sinfo5->view2_buffer = viewinfo->view2_buffer;
            sinfo5->sticker_window = sticker_window;sinfo5->spointer = spointer;
            struct sticker_info *sinfo6 = (struct sticker_info *)malloc(sizeof(struct sticker_info));
            sinfo6->str = "/zj";sinfo6->view2_buffer = viewinfo->view2_buffer;
            sinfo6->sticker_window = sticker_window;sinfo6->spointer = spointer;
            spointer[0] = sinfo1;
            spointer[1] = sinfo2;
            spointer[2] = sinfo3;
            spointer[3] = sinfo4;
            spointer[4] = sinfo5;
            spointer[5] = sinfo6;
            g_signal_connect(G_OBJECT(sbox1),"button_press_event",G_CALLBACK(insert_sticker),sinfo1);
            g_signal_connect(G_OBJECT(sbox2),"button_press_event",G_CALLBACK(insert_sticker),sinfo2);
            g_signal_connect(G_OBJECT(sbox3),"button_press_event",G_CALLBACK(insert_sticker),sinfo3);
            g_signal_connect(G_OBJECT(sbox4),"button_press_event",G_CALLBACK(insert_sticker),sinfo4);
            g_signal_connect(G_OBJECT(sbox5),"button_press_event",G_CALLBACK(insert_sticker),sinfo5);
            g_signal_connect(G_OBJECT(sbox6),"button_press_event",G_CALLBACK(insert_sticker),sinfo6);
            s1 = gtk_image_new_from_file("./sticker/am.gif");
            s2 = gtk_image_new_from_file("./sticker/dk.gif");
            s3 = gtk_image_new_from_file("./sticker/fd.gif");
            s4 = gtk_image_new_from_file("./sticker/sk.gif");
            s5 = gtk_image_new_from_file("./sticker/wx.gif");
            s6 = gtk_image_new_from_file("./sticker/zj.gif");
            gtk_container_add(GTK_CONTAINER(sbox1),s1);
            gtk_container_add(GTK_CONTAINER(sbox2),s2);
            gtk_container_add(GTK_CONTAINER(sbox3),s3);
            gtk_container_add(GTK_CONTAINER(sbox4),s4);
            gtk_container_add(GTK_CONTAINER(sbox5),s5);
            gtk_container_add(GTK_CONTAINER(sbox6),s6);
            gtk_table_attach_defaults(GTK_TABLE(table),sbox1,0,1,0,1);
            gtk_table_attach_defaults(GTK_TABLE(table),sbox2,1,2,0,1);
            gtk_table_attach_defaults(GTK_TABLE(table),sbox3,2,3,0,1);
            gtk_table_attach_defaults(GTK_TABLE(table),sbox4,0,1,1,2);
            gtk_table_attach_defaults(GTK_TABLE(table),sbox5,1,2,1,2);
            gtk_table_attach_defaults(GTK_TABLE(table),sbox6,2,3,1,2);
            gtk_widget_show_all(sticker_window);
        }
    }
}
//============================更改背景====================
GtkWidget *FileSelection;

/**************************************************/
/*名称：OpenFile_OfBackgroundTransform
/*描述：更改背景
/*作成日期： 2016-08-31
/*参数：
        参数1：widget、GtkWidget*、输入参数、修改背景按钮
        参数2：data、gpointer*、输入参数、指向背景图片所在位置的指针
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void OpenFile_OfBackgroundTransform(GtkWidget *widget,gpointer *data)
{
	gchar *path=gtk_file_selection_get_filename(GTK_FILE_SELECTION(FileSelection));
	gint w,h;//size unchangable
	widget=mainUI_window;
        gtk_widget_get_size_request(GTK_WIDGET(widget),&w,&h);
	gtk_widget_set_app_paintable(widget, TRUE);
        gtk_widget_realize(widget);
        gtk_widget_queue_draw(widget);
        GdkPixbuf *src = gdk_pixbuf_new_from_file(path, NULL);
        GdkPixbuf *dst = gdk_pixbuf_scale_simple(src,w,h,GDK_INTERP_BILINEAR);
        GdkPixmap *pixmap = NULL;
        gdk_pixbuf_render_pixmap_and_mask(dst,&pixmap,NULL,128);
        gdk_window_set_back_pixmap(widget->window,pixmap,FALSE);
        g_object_unref(src);
        g_object_unref(dst);
        g_object_unref(pixmap);
}

/**************************************************/
/*名称：button_background_transform_function
/*描述：更改背景的回调函数
/*作成日期： 2016-08-31
/*参数：
        参数1：widget、GtkWidget*、输入参数、修改背景按钮
        参数2：data、gpointer*、输入参数、指向背景图片所在位置的指针
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void button_background_transform_function(GtkWidget *widget,gpointer *data)
{
	FileSelection=gtk_file_selection_new("select background photo");/*创建文件选择构件*/
	gtk_file_selection_set_filename(GTK_FILE_SELECTION(FileSelection),"*.jpg");
	gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(FileSelection)->ok_button),"clicked",GTK_SIGNAL_FUNC(OpenFile_OfBackgroundTransform),data);/*捕捉打开按纽的”clicked”信号*/
    gtk_widget_show(FileSelection);
    g_signal_connect(G_OBJECT (FileSelection), "destroy", G_CALLBACK (gtk_widget_destroyed),NULL);
    gtk_signal_connect_object(GTK_OBJECT (GTK_FILE_SELECTION(FileSelection)->cancel_button),"clicked", GTK_SIGNAL_FUNC(gtk_widget_destroy), GTK_OBJECT(FileSelection));
}
//============================增加传送文件文本框============
char position[POSITION_SIZE];
void OpenFile(GtkWidget *widget,gpointer *data);

/**************************************************/
/*名称：FileTransform_button_press
/*描述：文件发送
/*作成日期： 2016-08-31
/*参数：
        参数1：widget、GtkWidget*、输入参数、发送文件的按钮
        参数2：event、GtkWidget*、输入参数、事件
        参数2：viewinfo、struct text_view_info *、输入参数、------
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void FileTransform_button_press(GtkWidget *widget, GdkEvent *event, struct text_view_info *viewinfo){
	FileSelection=gtk_file_selection_new("选择文件");/*创建文件选择构件*/
	gtk_file_selection_set_filename(GTK_FILE_SELECTION(FileSelection),"*.txt");
	gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(FileSelection)->ok_button),"clicked",GTK_SIGNAL_FUNC(OpenFile),NULL);/*捕捉打开按纽的”clicked”信号*/
	gtk_widget_show(FileSelection);
    g_signal_connect(G_OBJECT (FileSelection), "destroy",G_CALLBACK (gtk_widget_destroyed),NULL);
    gtk_signal_connect_object(GTK_OBJECT (GTK_FILE_SELECTION(FileSelection)->cancel_button),"clicked",
    GTK_SIGNAL_FUNC(gtk_widget_destroy), GTK_OBJECT(FileSelection));
}

/**************************************************/
/*名称：OpenFile
/*描述：打开文件
/*作成日期： 2016-08-31
/*参数：
        参数1：widget、GtkWidget*、输入参数、打开文件的按钮
        参数2：data、gpointer *、输入参数、发送文件的数据
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void OpenFile(GtkWidget *widget,gpointer *data)
{
	strcpy( position,gtk_file_selection_get_filename(GTK_FILE_SELECTION(FileSelection)) );   /*输出选择的文件名*/
	gtk_window_set_resizable(GTK_WINDOW(FileSelection), FALSE);
	exec_cmd(14,talk_to_user,position);
}
//===============================================

/**************************************************/
/*名称：create_message_dialog
/*描述：创建消息目录
/*作成日期： 2016-08-31
/*参数：
        参数1：type、GtkMessageType 输入参数、消息类型
        参数2：message、gchar*、输入参数、发送的消息
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void create_message_dialog (GtkMessageType type, gchar* message){
    GtkWidget* dialogx;
    dialogx = gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,type,GTK_BUTTONS_OK,message);
    gtk_dialog_run(GTK_DIALOG(dialogx));
    gtk_widget_destroy(dialogx);
}

/**************************************************/
/*名称：scroll_to_the_end
/*描述：回退到底部
/*作成日期： 2016-08-31
/*参数：
        参数1：viewinfo、struct text_view_info * 输入参数、视图
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void scroll_to_the_end(struct text_view_info *viewinfo){
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(viewinfo->view1_buffer,&end);
    GtkTextMark *mark=gtk_text_buffer_create_mark(viewinfo->view1_buffer,NULL,&end,1);
    gtk_text_buffer_move_mark(viewinfo->view1_buffer,mark,&end);
    gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(viewinfo->view1),mark,0,1,1,1);
}

/**************************************************/
/*名称：insert_sticker
/*描述：插入表情
/*作成日期： 2016-08-31
/*参数：
        参数1：type、GtkMessageType 输入参数、消息类型
        参数2：message、gchar*、输入参数、发送的消息
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void insert_sticker(GtkWidget *widget,GdkEventButton *event, struct sticker_info *sinfo){
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(sinfo->view2_buffer,&end);
    gtk_text_buffer_insert(sinfo->view2_buffer,&end,sinfo->str,-1);
    GtkWidget *sticker_window = sinfo->sticker_window;
    struct sticker_info **p = sinfo->spointer;
    int i;
    for (i=0;i<6;i++) free(p[i]);
    free(p);
    gtk_widget_destroy(sticker_window);
}

/**************************************************/
/*名称：destory_sticker_window
/*描述：销毁表情窗口
/*作成日期： 2016-08-31
/*参数：
        参数1：-----
        参数2：-----
        参数3：-----
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void destory_sticker_window(GtkWidget *widget,GdkEventCrossing *event,GtkWidget* data){
    gtk_widget_destroy(data);
}

/**************************************************/
/*名称：group_friend_list_add
/*描述：加入群组列表
/*作成日期： 2016-08-31
/*参数：
        参数1：-----
        参数2：-----
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void group_friend_list_add(GtkWidget* list, const char *buf){
	GtkWidget *item;
	//创建一个列表项
	item=gtk_list_item_new_with_label(buf);
	gtk_container_add(GTK_CONTAINER(list),item);
	gtk_widget_show(item);
}

/**************************************************/
/*名称：group_friend_list_clear
/*描述：清空群组列表
/*作成日期： 2016-08-31
/*参数：
        参数1：-----
        参数2：-----
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void group_friend_list_clear(GtkWidget* list){
	gtk_list_clear_items(GTK_LIST(list),0,-1);
}

/**************************************************/
/*名称：change_chat_mode
/*描述：改变聊天模式
/*作成日期： 2016-08-31
/*参数：
        参数1：-----
        参数2：-----
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void change_chat_mode(int mode){
    if (mode == 1){
        gtk_widget_show (cmeventbox2);
        gtk_widget_hide(scwinlist);
    }
    else {
        gtk_widget_show (scwinlist);
        gtk_widget_hide(cmeventbox2);
    }
}

/**************************************************/
/*名称：update_group_friend_list
/*描述：更新群组好友列表
/*作成日期： 2016-08-31
/*参数：
        参数1：-----
        参数2：-----
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void update_group_friend_list(int num,struct group_username *name){
    group_friend_list_clear(group_friend_list);
    int i = 0;
    for (i=0;i<num;i++)
		group_friend_list_add(group_friend_list, name[i].username);
}

/**************************************************/
/*名称：set_chat_title
/*描述：设置聊天标题
/*作成日期： 2016-08-31
/*参数：
        参数1：-----
        参数2：-----
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void set_chat_title(GtkWidget *cmlabel, const char *buf){
    gtk_label_set_markup(GTK_LABEL(cmlabel),buf);
}

/**************************************************/
/*名称：clear_chat_window
/*描述：清空聊天窗口
/*作成日期： 2016-08-31
/*参数：
        参数1：-----
        参数2：-----
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void clear_chat_window(){
    gtk_text_buffer_set_text(viewinfo.view1_buffer,"", -1);
}






