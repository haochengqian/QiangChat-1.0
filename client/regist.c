/**************************************************/
/*名称：regist.c
/*描述：创建注册界面
/*作成日期： 2016-08-31
/-*参数：
        参数1：window、GtkWidget* 、输出参数、注册界面的窗口
/*返回值：GtkWidget*
/*作者：郑安庆
/***************************************************/

#include "regist.h"

gboolean password_check(const char* username,const char* password);
gboolean window_drag(GtkWidget *widget, GdkEventButton *event, GdkWindowEdge edge);

extern GtkWidget* login_window;
extern GtkWidget* mainUI_window;
extern GtkWidget* regist_window;
extern GtkWidget* mainUI_window_scwinlist;
extern GtkWidget* mainUI_window_group_list_box;
extern GtkWidget* mainUI_window_about_box;
regist_msg regmsg;

/**************************************************/
/*名称：regist_on_button_clicked
/*描述：注册按钮的回调函数
/*作成日期： 2016-08-31
/*参数：
        参数1：button、GtkWidget* 、输入参数、注册按钮
        参数2：data、regist_msg* 、输入参数、注册信息
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void regist_on_button_clicked(GtkWidget* button,regist_msg* data){
    const char* username=gtk_entry_get_text(GTK_ENTRY(data->username));
    const char* password=gtk_entry_get_text(GTK_ENTRY(data->password));
    const char* rpassword=gtk_entry_get_text(GTK_ENTRY(data->rpassword));
    if(strcmp(password,rpassword)!=0){
        create_new_pop_window("please comfirm your password!");
        return;
    }
    if(password_check(username,password)==FALSE){
        return;
    }
    if(regist(username, password) == 0) {
	return ;
    }
    gtk_widget_hide_all(data->fwidget);
    gtk_widget_show_all(login_window);
}

/**************************************************/
/*名称：regist_to_login_on_button_clicked
/*描述：注册按钮的回调函数
/*作成日期： 2016-08-31
/*参数：
        参数1：button、GtkWidget* 、输入参数、注册按钮
        参数2：data、gpointer 、输入参数、注册信息
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void regist_to_login_on_button_clicked(GtkWidget* button,gpointer data){
    gtk_widget_hide_all(data);
    gtk_widget_show_all(login_window);
}

/**************************************************/
/*名称：registUI()
/*描述：创建注册界面
/*作成日期： 2016-08-31
/*参数：
       参数1：window、GtkWidget* 、输出参数、注册界面的窗口
/*返回值：GtkWidget*
/*作者：郑安庆
/***************************************************/
GtkWidget* registUI(){
    GtkWidget* window;
    GtkWidget* box;
    GtkWidget* urnamebox;
    GtkWidget* psdbox;
    GtkWidget* repeatpsdbox;
    GtkWidget* btnbox;
    GtkWidget* urnamelabel;
    GtkWidget* psdlabel;
    GtkWidget* repeatpsdlabel;
    GtkWidget* button;
    GtkWidget* loginbtn;
    GtkWidget* urname;
    GtkWidget* password;
    GtkWidget* rpassword;
    GtkWidget* sep;
    GtkWidget* main_box;
    GdkPixbuf* pixbuf = NULL;
    GdkPixmap* pixmap = NULL;
    GdkBitmap* bitmap = NULL;
    GtkWidget* vbox,*hbox;
    GtkWidget* box1,*box2;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
    main_box=gtk_vbox_new(FALSE,0);
    vbox = gtk_vbox_new(FALSE,0);
    hbox = gtk_hbox_new(FALSE,0);
    box1 = gtk_vbox_new(FALSE,0);
    box2 = gtk_vbox_new(FALSE,0);
    gtk_window_set_decorated(GTK_WINDOW(window),FALSE);
    //==========================显示登录界面的背景===============================
    gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);
    gtk_widget_set_size_request(window, 844, 415);
    g_signal_connect (G_OBJECT (window), "delete_event", G_CALLBACK (gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(window), "button_press_event", G_CALLBACK(window_drag), NULL);
    pixbuf = gdk_pixbuf_new_from_file ("./xiaomai.png",NULL);
    gdk_pixbuf_render_pixmap_and_mask(pixbuf, &pixmap, &bitmap, 128);
    gtk_widget_shape_combine_mask(window, bitmap, 0, 0);
    GtkStyle *style = gtk_style_copy(window->style);
    if(style->bg_pixmap[GTK_STATE_NORMAL])
        g_object_unref(style->bg_pixmap[GTK_STATE_NORMAL]);
    style->bg_pixmap[GTK_STATE_NORMAL] = g_object_ref(pixmap);
    gtk_widget_set_style(window, style);
    //=====================================================================
    gtk_widget_set_size_request(vbox,844,125);
    gtk_box_pack_start(GTK_BOX(main_box),vbox,FALSE,FALSE,0);
    gtk_widget_set_size_request(hbox,844,250);
    gtk_box_pack_start(GTK_BOX(main_box),hbox,FALSE,FALSE,0);
    gtk_widget_set_size_request(box1,350,250);
    gtk_box_pack_start(GTK_BOX(hbox),box1,FALSE,FALSE,0);
    gtk_widget_set_size_request(box2,316,250);
    gtk_box_pack_start(GTK_BOX(hbox),box2,FALSE,FALSE,0);
    gtk_window_set_title(GTK_WINDOW(window),"regist");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window),20);
    gtk_container_add(GTK_CONTAINER(window),main_box);
    //main box init
    box = gtk_vbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box2),box,FALSE,FALSE,5);
    urnamebox=gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),urnamebox,FALSE,FALSE,5);
    psdbox=gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),psdbox,FALSE,FALSE,5);
    repeatpsdbox=gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),repeatpsdbox,FALSE,FALSE,5);
    sep=gtk_hseparator_new();
    gtk_box_pack_start(GTK_BOX(box),sep,FALSE,FALSE,5);
    btnbox=gtk_hbox_new(TRUE,0);
    gtk_box_pack_start(GTK_BOX(box),btnbox,FALSE,FALSE,5);
    //uesrname box init
    urnamelabel = gtk_label_new("       uesrname:");
    urname=gtk_entry_new();
    gtk_box_pack_end(GTK_BOX(urnamebox),urname,FALSE,FALSE,5);
    gtk_box_pack_end(GTK_BOX(urnamebox),urnamelabel,FALSE,FALSE,5);
    //password box init
    psdlabel = gtk_label_new("       password:");
    password=gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password),FALSE);
    gtk_box_pack_end(GTK_BOX(psdbox),password,FALSE,FALSE,5);
    gtk_box_pack_end(GTK_BOX(psdbox),psdlabel,FALSE,FALSE,5);
    repeatpsdlabel = gtk_label_new("confirm password:");
    rpassword=gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(rpassword),FALSE);
    gtk_box_pack_end(GTK_BOX(repeatpsdbox),rpassword,FALSE,FALSE,5);
    gtk_box_pack_end(GTK_BOX(repeatpsdbox),repeatpsdlabel,FALSE,FALSE,5);
    //regist_msg regmsg;
    regmsg.widget=box;
    regmsg.fwidget=window;
    regmsg.username=urname;
    regmsg.password=password;
    regmsg.rpassword=rpassword;
    button = gtk_button_new_with_label("regist");
    loginbtn=gtk_button_new_with_label("return to login");
    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(regist_on_button_clicked),&regmsg);
    g_signal_connect(G_OBJECT(loginbtn),"clicked",G_CALLBACK(regist_to_login_on_button_clicked),window);
    gtk_box_pack_start(GTK_BOX(btnbox),button,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(btnbox),loginbtn,FALSE,FALSE,5);
    return window;
}
