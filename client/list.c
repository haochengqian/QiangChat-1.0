/**************************************************/
/*名称：list.c
/*描述：群组和联系人菜单
/*作成日期： 2016.8.26
/*返回值：void
/*作者：郝程乾
/***************************************************/

#include"list.h"

extern int group_list_size;
extern 	int friend_list_size;
extern 	int n_g,t_t_g,f_g,l,n_f,l_f;
extern 	struct widget_pack sky_group,sky_friend;
extern 	GtkWidget *group_box[N];//eventbox
extern 	GtkWidget *label_grouplist[N];	//label_grouplist
extern 	GtkWidget *group_page;
extern 	int *num_group;
extern 	int *talk_to_group;
extern 	int *focus_group;	//刷新前面向的群组
extern 	char group_ID[100];//搜索时用
extern 	int *lastfocus;
extern 	struct args_grouplist args_for_callback_grouplist[N];//回调函数参数
extern 	struct group_profile attribute_group[N];		 //eventbox属性
extern struct group_profile groupdata[N]; //获取的群组属性
extern 	GtkWidget *friend_box[N];//eventbox
extern 	GtkWidget *label_friendlist[N];	//label_friendlist
extern 	GtkWidget *friend_page;
extern 	int *num_friend;
extern 	char talk_to_user[100];
extern 	char focus_user[100];
extern 	char friend_ID[100];//搜索时用
extern 	int lastfocus_friend;
extern 	struct args_friendlist args_for_callback_friendlist[N];//回调函数参数
extern 	struct user attribute_friend[N];		 //eventbox属性
extern 	struct user_profile userdata[N]; //获取的用户属性
extern int group_list_size;
extern int friend_list_size;

//===================== 一些函数 =======================
/**************************************************/
/*名称：cmp_grouplist cmp_friendlist
/*描述：群组列表对比 好友列表对比
/*作成日期： 2016.08.23
/*参数：
         参数1：参数名称 *a、参数类型 const void 、输入参数、参数含义 群组列表
         参数2：参数名称 *b、参数类型 const void 、输入参数、参数含义	群组列表
/*返回值：int
/*作者：郝程乾
/***************************************************/
int cmp_grouplist(const void *a,const void *b){
	struct group_profile *c = (struct group_profile *)a;
	struct group_profile *d = (struct group_profile *)b;
	return c->groupID - d->groupID;
}

int cmp_friendlist(const void *a,const void *b){
	struct user *c = (struct user *)a;
	struct user *d = (struct user *)b;
	if((*c).status != (*d).status) return (*d).status - (*c).status;
	else return strcmp((*c).username,(*d).username);
}
/**************************************************/
/*名称：str2int int2str
/*描述：string转换int int转换string
/*作成日期： 2016.08.23
/*参数：
         参数1：参数名称 *txt、参数类型 char 、输入参数、参数含义 需转换的 char*
         参数2：参数名称 *b、参数类型 const void 、输入\输出参数、参数含义	需转换的 char* 和int
/*返回值：int void
/*作者：郝程乾
/***************************************************/
int str2int(char *txt){
	int len = strlen(txt);
	int ret = 0;
	if(len>=9) return -1;
	int i;
	for(i = 0;i < len;i++){
		if(txt[i]>'9'||txt[i]<'0'){
			ret = -1;
			break;
		}
		ret += txt[i] - '0';
		ret *= 10;
	}
	return ret;
}

void int2str(char *txt,int a){
	int l = 0;
	char ch;
	while(a){
		txt[l++] = '0' + a%10;
		a /= 10;
	}
	txt[l] = 0;
	int i;
	for(i = 0;i < l/2;i++){
		ch = txt[i];
		txt[i] = txt[l - i - 1];
		txt[l-i-1] = ch;
	}
	if(l == 0){
		txt[0] = '0';
		txt[1] = 0;
	}
}

/**************************************************/
/*名称：destroy_widget
/*描述：删除构建
/*作成日期： 2016.08.23
/*参数：
         参数1：参数名称 *widget、参数类型 GtkWidget 、输入参数、参数含义 需操作的窗口
         参数2：参数名称 *a、参数类型 GtkWidget 、输入参数、参数含义	需删除的构建
/*返回值：gint
/*作者：郝程乾
/***************************************************/
gint destroy_widget(GtkWidget *widget,GtkWidget *a){		      //删除构件
	gtk_widget_destroy(a);
}



//======================================= warning 前方高能 =============================================================

/**************************************************/
/*名称：create_grouplist
/*描述：创建群组
/*作成日期： 2016.08.23
/*参数：
         参数1：参数名称 *num_group、参数类型 int 、输入参数、参数含义 群组数目
         参数2：参数名称 *talk_to_group、参数类型 int 、输入参数、参数含义 当前说话的群组
		 参数3：参数名称 *group_ID、参数类型 char 、输入参数、参数含义	当前群组编号
		 参数4：参数名称 *focus_group、参数类型 int 、输入参数、参数含义	当前正在聊天的群组
		 参数5：参数名称 *lastfocus、参数类型 int 、输入参数、参数含义	之前聊天的群组
		 参数6：参数名称 *args_for_callback_grouplist、参数类型 struct args_grouplist  、输入参数、参数含义	给回调函数的群组信息
		 参数7：参数名称 *groupdata、参数类型 struct group_profile 、输入参数、参数含义	群组的聊天数据
		 参数8：参数名称 **label_grouplist、参数类型 GtkWidget 、输入参数、参数含义	群组标签
		 参数8：参数名称 **group_box、参数类型 GtkWidget 、输入参数、参数含义	群组盒子
/*返回值：struct widget_pack
/*作者：郝程乾
/***************************************************/


//********************************************* 群组列表函数 ***********************************************************

struct widget_pack create_grouplist(int *num_group,int *talk_to_group,char *group_ID,int *focus_group,int *lastfocus,struct args_grouplist *args_for_callback_grouplist,struct group_profile *attribute_group,struct group_profile *groupdata,GtkWidget **group_box,GtkWidget **label_grouplist){
	struct widget_pack temp;
	GtkWidget *scrolled_window;
	GtkWidget *Mainbox,*group_page,*box2;
	GtkWidget *button_add_group,*button_create_group;
	scrolled_window = gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	//盒容器，如果要设置内部子控件大小，要设置为非等宽，然后子控件设置为不可扩展，再设置最小大小
	Mainbox = gtk_vbox_new(FALSE,0);//装两个界面
	group_page = gtk_vbox_new(FALSE,0);//列表界面
	box2 = gtk_hbox_new(TRUE,0);//按钮界面
	//add
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window),group_page);
	gtk_widget_set_size_request(box2,400,45);
	button_add_group = gtk_button_new_with_label("添加群组");
	button_create_group = gtk_button_new_with_label("创建群组");
	gtk_box_pack_start(GTK_BOX(box2),button_add_group,FALSE,FALSE,2);
	gtk_box_pack_start(GTK_BOX(box2),button_create_group,FALSE,FALSE,2);
	gtk_box_pack_start(GTK_BOX(Mainbox),scrolled_window,TRUE,TRUE,2);
	gtk_box_pack_start(GTK_BOX(Mainbox),box2,FALSE,FALSE,2);
	struct args1 *ar;
	ar = (struct args1 *)malloc(sizeof(struct args1));
	ar->button_add_group = button_add_group;    //添加群组按钮
	ar->button_create_group = button_create_group; //创建群组按钮
	ar->buf = group_ID;				//群组ID
	g_signal_connect(G_OBJECT(button_add_group),"clicked",G_CALLBACK(add_group),ar);
	g_signal_connect(G_OBJECT(button_create_group),"clicked",G_CALLBACK(create_group),ar);
	update_grouplist(UPDATE_GROUPLIST);
	printf("create is ok\n");
	temp.Mainbox = Mainbox;
	temp.update_box = group_page;
	return temp;
}
/**************************************************/
/*名称：moveto_group leave_group
/*描述：移入群组移除群组 点击确定
/*作成日期： 2016.08.23
/*参数：
         参数1：参数名称 *widget、参数类型 GtkWidget 、输入参数、参数含义 需操作的窗口
         参数2：参数名称 *event、参数类型 GdkEventCrossing 、输入参数、参数含义 操作的事件
		 参数3：参数名称 *a、参数类型 struct args_grouplist 、输入参数、参数含义 操作的群组名
/*返回值：gint
/*作者：郝程乾
/***************************************************/

//========================== GTK CALLBACK FUNCTION ====================
gint moveto_group(GtkWidget *widget,GdkEventCrossing *event,struct args_grouplist *a){
	if(*(a->lastfocus) != a->id)
	gtk_widget_modify_bg(widget,GTK_STATE_NORMAL,&move);
}

gint leave_group(GtkWidget *widget,GdkEventCrossing *event,struct args_grouplist *a){
	if(*(a->lastfocus) != a->id)
	gtk_widget_modify_bg(widget,GTK_STATE_NORMAL,&blue);

}

gint focusin_group(GtkWidget *widget,GdkEventFocus *event,struct args_grouplist *a){
	gtk_widget_modify_bg(widget,GTK_STATE_NORMAL,&choose);
	if(*(a->lastfocus) != -1&&*(a->lastfocus) != a->id)
		gtk_widget_modify_bg((a->event_box)[*(a->lastfocus)],GTK_STATE_NORMAL,&blue);
	*(a->lastfocus) = a->id;
	*(a->focus_group) = (a->attribute_group)[a->id].groupID;
	*talk_to_group = *(a->focus_group);
	focusin_group_callback();
}
/**************************************************/
/*名称：confirm_group
/*描述：加群验证
/*作成日期： 2016.08.23
/*参数：
         参数1：参数名称 *widget、参数类型 GtkWidget 、输入参数、参数含义 需操作的窗口
		 参数2：参数名称 *a、参数类型 struct args1 、输入参数、参数含义 加群时传入的用户信息
/*返回值：gint
/*作者：郝程乾
/***************************************************/
//加群时调用
gint confirm_group(GtkWidget *widget,struct args1 *a){
	printf("confirm is ok\n");
	strcpy(a->buf, gtk_entry_get_text(GTK_ENTRY(a->entry)));
	int ret = str2int(a->buf);
	if(ret == -1){
		GtkWidget *window;
		GtkWidget *button;
		GtkWidget *label;
		GtkWidget *box;
		window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title(GTK_WINDOW(window),"警告");
		box = gtk_vbox_new(TRUE,0);
		button = gtk_button_new_with_label("确定");
		label = gtk_label_new("非法！请重新输入！");
		gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
		gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
		gtk_widget_set_size_request(window,250,100);
		gtk_container_add(GTK_CONTAINER(window),box);
		gtk_box_pack_start(GTK_BOX(box),label,TRUE,FALSE,5);
		gtk_box_pack_start(GTK_BOX(box),button,TRUE,FALSE,5);
		g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(destroy_widget),window);
		gtk_widget_show_all(window);
	}
	else{
		//strcpy(group_ID, a->buf);
		confirm_group_callback();
		gtk_widget_destroy(a->window);
	}
}
/**************************************************/
/*名称：click_group
/*描述：点击建群按钮
/*作成日期： 2016.08.23
/*参数：
         参数1：参数名称 *widget、参数类型 GtkWidget 、输入参数、参数含义 需操作的窗口
		 参数2：参数名称 *a、参数类型 struct args1 、输入参数、参数含义 建群人的信息
/*返回值：gint
/*作者：郝程乾
/***************************************************/
//jian qun
gint click_group(GtkWidget *widget,struct args1 *a){
	create_group_callback();
	gtk_widget_destroy(a->window);
}
/**************************************************/
/*名称：add_group
/*描述：加群
/*作成日期： 2016.08.23
/*参数：
         参数1：参数名称 *widget、参数类型 GtkWidget 、输入参数、参数含义 需操作的窗口
		 参数2：参数名称 *a、参数类型 struct args1 、输入参数、参数含义 建群人的信息
/*返回值：gint
/*作者：郝程乾
/***************************************************/
gint add_group(GtkWidget *widget,struct args1 *a){
	GtkWidget *Group_ID;
	GtkWidget *label;
	GtkWidget *button_confirm;
	GtkWidget *window;
	GtkWidget *Mainbox,*box1;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"加入群组");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
	gtk_widget_set_size_request(window,250,100);
	Mainbox = gtk_vbox_new(FALSE,0);
	box1 = gtk_hbox_new(FALSE,0);
	button_confirm = gtk_button_new_with_label("确定");
	label = gtk_label_new("群组ID:");
	Group_ID = gtk_entry_new();
	gtk_container_add(GTK_CONTAINER(window),Mainbox);
	gtk_container_set_border_width(GTK_CONTAINER(window),5);
	gtk_box_pack_start(GTK_BOX(Mainbox),box1,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(Mainbox),button_confirm,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(box1),label,FALSE,FALSE,10);
	gtk_box_pack_start(GTK_BOX(box1),Group_ID,TRUE,TRUE,10);
	struct args1 *tmp;
	tmp = (struct args1 *)malloc(sizeof(struct args1));//此处分配了内存
	*(tmp) = *a;
	tmp->window = window;
	tmp->entry = Group_ID;
	gtk_widget_set_sensitive(a->button_add_group,FALSE);
	gtk_widget_set_sensitive(a->button_create_group,FALSE);
	g_signal_connect(G_OBJECT(button_confirm),"clicked",G_CALLBACK(confirm_group),tmp);
	g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(close_window_group),tmp);
	gtk_widget_show_all(window);
}
/**************************************************/
/*名称：create_group
/*描述：建群
/*作成日期： 2016.08.23
/*参数：
         参数1：参数名称 *widget、参数类型 GtkWidget 、输入参数、参数含义 需操作的窗口
		 参数2：参数名称 *a、参数类型 struct args1 、输入参数、参数含义 建群人的信息
/*返回值：gint
/*作者：郝程乾
/***************************************************/
gint create_group(GtkWidget *widget,struct args1 *a){
	GtkWidget *label;
	GtkWidget *button_confirm;
	GtkWidget *window;
	GtkWidget *Mainbox,*box1;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"创建群组");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
	gtk_widget_set_size_request(window,250,100);
	button_confirm = gtk_button_new_with_label("确定");
	Mainbox = gtk_vbox_new(FALSE,0);
	box1 = gtk_hbox_new(FALSE,0);
	label = gtk_label_new("请稍候...");
	gtk_container_add(GTK_CONTAINER(window),Mainbox);
	gtk_container_set_border_width(GTK_CONTAINER(window),5);
	gtk_box_pack_start(GTK_BOX(Mainbox),box1,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(Mainbox),button_confirm,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(box1),label,TRUE,FALSE,10);
	struct args1 *tmp;
	tmp = (struct args1 *)malloc(sizeof(struct args1));//此处分配了内存
	*(tmp) = *a;
	tmp->window = window;
	gtk_widget_set_sensitive(a->button_add_group,FALSE);
	gtk_widget_set_sensitive(a->button_create_group,FALSE);
	printf("ok\n");
	g_signal_connect(G_OBJECT(button_confirm),"clicked",G_CALLBACK(click_group),tmp);
	g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(close_window_group),tmp);
	gtk_widget_show_all(window);
}

/**************************************************/
/*名称：close_window_group
/*描述：关闭群信息的窗口
/*作成日期： 2016.08.23
/*参数：
         参数1：参数名称 *widget、参数类型 GtkWidget 、输入参数、参数含义 需操作的窗口
		 参数2：参数名称 *a、参数类型 struct args1 、输入参数、参数含义 建群人的信息
/*返回值：gint
/*作者：郝程乾
/***************************************************/
gint close_window_group(GtkWidget *widget,struct args1 *a){
	gtk_widget_set_sensitive(a->button_add_group,TRUE);
	gtk_widget_set_sensitive(a->button_create_group,TRUE);
	free(a);
}
/**************************************************/
/*名称：update_grouplist
/*描述：刷新群组列表
/*作成日期： 2016.08.23
/*参数：
         参数1：参数名称 *tmp、参数类型 struct args_grouplist 、输入参数、参数含义 群组联系人信息
		 参数2：参数名称 *attribute_group、参数类型 struct group_profile 、输入参数、参数含义 群组信息
		 参数3：参数名称 *data、参数类型 struct group_profile 、输入参数、参数含义 群组信息
		 参数4：参数名称 *num_group、参数类型 int 、输入参数、参数含义 群组编号
		 参数5：参数名称 *lastfocus、参数类型 int 、输入参数、参数含义 之前的群组编号
		 参数6：参数名称 *talk_to_group、参数类型 int 、输入参数、参数含义 当前聊天群组
		 参数7：参数名称 *focus_group、参数类型 int 、输入参数、参数含义 点钱聊天群组
		 参数8：参数名称 *lastfocus、参数类型 int 、输入参数、参数含义 之前的群组
		 参数9：参数名称 *group_page、参数类型 GtkWidget 、输入参数、参数含义 群组页面
		 参数10：参数名称 **group_box、参数类型 GtkWidget 、输入参数、参数含义 群组信息盒
		 参数11：参数名称 **label_grouplist、参数类型 GtkWidget 、输入参数、参数含义 群组标签页面

/*返回值：gint
/*作者：郝程乾
/***************************************************/
//刷新群组列表
gint update_grouplist(struct args_grouplist *tmp,struct group_profile *attribute_group,struct group_profile *data,int *num_group,int *lastfocus,int *talk_to_group,int *focus_group,GtkWidget *group_page,GtkWidget **group_box,GtkWidget **label_grouplist){
	int i;
	for(i = 0;i < *num_group;i++){
		gtk_widget_destroy(label_grouplist[i]);
		gtk_widget_destroy(group_box[i]);
	}
	*num_group = group_list_size;
	for(i = 0;i < *num_group;i++){
		tmp[i].num_group = num_group;
		tmp[i].talk_to = talk_to_group;
		tmp[i].event_box = group_box;
		tmp[i].lastfocus = lastfocus;
		tmp[i].attribute_group = attribute_group;
		tmp[i].focus_group = focus_group;
		attribute_group[i].groupID = data[i].groupID;
	}
	printf("init is ok\n");
	qsort(attribute_group,(*num_group),sizeof(struct group_profile),cmp_grouplist);
	printf("qsort is ok\n");
	for(i = 0;i < *num_group;i++){
		tmp[i].id = i;
		group_box[i] = gtk_event_box_new();
		char txt[100];
		int2str(txt,attribute_group[i].groupID);
		gtk_widget_modify_bg(group_box[i],GTK_STATE_NORMAL,&blue);
		if(*(tmp[i].focus_group) == attribute_group[i].groupID){
			*lastfocus = i;
			gtk_widget_modify_bg(group_box[i],GTK_STATE_NORMAL,&choose);
		}
		label_grouplist[i] = gtk_label_new(txt);
		gtk_container_add(GTK_CONTAINER(group_box[i]),label_grouplist[i]);
		gtk_widget_set_events(group_box[i],GDK_BUTTON_PRESS);
		gtk_widget_set_size_request(group_box[i],0,50);
		g_signal_connect(G_OBJECT(group_box[i]),"enter_notify_event",G_CALLBACK(moveto_group),&tmp[i]);
		g_signal_connect(G_OBJECT(group_box[i]),"leave_notify_event",G_CALLBACK(leave_group),&tmp[i]);
		g_signal_connect(G_OBJECT(group_box[i]),"button_press_event",G_CALLBACK(focusin_group),&tmp[i]);
		gtk_box_pack_start(GTK_BOX(group_page),group_box[i],FALSE,FALSE,1);
	}
	gtk_widget_show_all(group_page);
	printf("update is ok\n");
}

//********************************************* 好友列表函数 ***********************************************************
/**************************************************/
/*名称：widget_pack
/*描述：刷新群组列表
/*作成日期： 2016.08.23
/*参数：
		 参数1：参数名称 *num_group、参数类型 int 、输入参数、参数含义 群组编号
		 参数2：参数名称 *lastfocus、参数类型 int 、输入参数、参数含义 之前的群组编号
		 参数3：参数名称 *talk_to_group、参数类型 int 、输入参数、参数含义 当前聊天群组
		 参数4：参数名称 *focus_group、参数类型 int 、输入参数、参数含义 点钱聊天群组
		 参数5：参数名称 *lastfocus、参数类型 int 、输入参数、参数含义 之前的群组
		 参数6：参数名称 *group_page、参数类型 GtkWidget 、输入参数、参数含义 群组页面
		 参数7：参数名称 **group_box、参数类型 GtkWidget 、输入参数、参数含义 群组信息盒
		 参数8：参数名称 **label_grouplist、参数类型 GtkWidget 、输入参数、参数含义 群组标签页面

/*返回值：gint
/*作者：郝程乾
/***************************************************/
struct widget_pack create_friendlist(int *num_friend,char *talk_to_user,char *friend_ID,char *focus_user,int *lastfocus_friend,struct args_friendlist *args_for_callback_friendlist,struct user *attribute_friend,struct user_profile *userdata,GtkWidget **friend_box,GtkWidget **label_friendlist){
	struct widget_pack temp;
	GtkWidget *scrolled_window;
	GtkWidget *Mainbox,*friend_page,*box2;
	GtkWidget *button_add_friend;
	scrolled_window = gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	//盒容器，如果要设置内部子控件大小，要设置为非等宽，然后子控件设置为不可扩展，再设置最小大小
	Mainbox = gtk_vbox_new(FALSE,0);//装两个界面
	friend_page = gtk_vbox_new(FALSE,0);//列表界面
	box2 = gtk_hbox_new(TRUE,0);//按钮界面
	//add
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window),friend_page);
	gtk_widget_set_size_request(box2,400,45);
	button_add_friend = gtk_button_new_with_label("添加好友");
	gtk_box_pack_start(GTK_BOX(box2),button_add_friend,FALSE,FALSE,2);
	gtk_box_pack_start(GTK_BOX(Mainbox),scrolled_window,TRUE,TRUE,2);
	gtk_box_pack_start(GTK_BOX(Mainbox),box2,FALSE,FALSE,2);
	struct args2 *ar;
	strcpy(friend_ID,"heheda");
	ar = (struct args2 *)malloc(sizeof(struct args2));
	ar->button_add_friend = button_add_friend;    //创建群组按钮
	ar->buf = friend_ID;							 //群组ID
	printf("%s\n",ar->buf);
	g_signal_connect(G_OBJECT(button_add_friend),"clicked",G_CALLBACK(search_friend),ar);
	update_friendlist(UPDATE_FRIENDLIST);
	temp.Mainbox = Mainbox;
	temp.update_box = friend_page;
	return temp;
}

/**************************************************/
/*名称：moveto_friend leave_friend focusin_friend
/*描述：移动到/移动/点击出好友列表回调函数
/*作成日期： 2016.08.23
/*参数：
		 参数1：参数名称 *event、参数类型 struct args_friendlist 、输入参数、参数含义 好友列表窗口
		 参数2：参数名称 *widget、参数类型 GtkWidget  、输入参数、参数含义 触发的时间
		 参数3：参数名称 *a、参数类型 struct args_friendlist 、输入参数、参数含义 当前聊天人信息

/*返回值：gint
/*作者：郝程乾
/***************************************************/
//========================== GTK CALLBACK FUNCTION ====================
gint moveto_friend(GtkWidget *widget,GdkEventCrossing *event,struct args_friendlist *a){
	if(*(a->lastfocus_friend) != a->id)
	gtk_widget_modify_bg(widget,GTK_STATE_NORMAL,&move);
}

gint leave_friend(GtkWidget *widget,GdkEventCrossing *event,struct args_friendlist *a){
	if(*(a->lastfocus_friend) != a->id)
		if((a->attribute_friend)[a->id].status)
			gtk_widget_modify_bg(widget,GTK_STATE_NORMAL,&blue);
		else
			gtk_widget_modify_bg(widget,GTK_STATE_NORMAL,&grey);
}

gint focusin_friend(GtkWidget *widget,GdkEventFocus *event,struct args_friendlist *a){
	gtk_widget_modify_bg(widget,GTK_STATE_NORMAL,&choose);
	if(*(a->lastfocus_friend) != -1&&*(a->lastfocus_friend) != a->id){
		if((a->attribute_friend)[*(a->lastfocus_friend)].status)
			gtk_widget_modify_bg((a->event_box)[*(a->lastfocus_friend)],GTK_STATE_NORMAL,&blue);
		else
			gtk_widget_modify_bg((a->event_box)[*(a->lastfocus_friend)],GTK_STATE_NORMAL,&grey);
	}
	*(a->lastfocus_friend) = a->id;
	strcpy(a->focus_user,(a->attribute_friend)[a->id].username);
	strcpy(talk_to_user,a->focus_user);
	focusin_friend_callback();
}
/**************************************************/
/*名称：confirm_friend
/*描述：验证好友
/*作成日期： 2016.08.23
/*参数：
		 参数1：参数名称 *widget、参数类型 GtkWidget 、输入参数、参数含义 当前窗口
		 参数2：参数名称 *a、参数类型 struct args2  、输入参数、参数含义 操作的好友信息

/*返回值：gint
/*作者：郝程乾
/***************************************************/
gint confirm_friend(GtkWidget *widget,struct args2 *a){
	strcpy(a->buf,gtk_entry_get_text(GTK_ENTRY(a->entry)));
	confirm_friend_callback();
	gtk_widget_destroy(a->window);
}
/**************************************************/
/*名称：search_friend
/*描述：查找好友
/*作成日期： 2016.08.23
/*参数：
		 参数1：参数名称 *widget、参数类型 GtkWidget 、输入参数、参数含义 当前窗口
		 参数2：参数名称 *a、参数类型 struct args2  、输入参数、参数含义 操作的好友信息

/*返回值：gint
/*作者：郝程乾
/***************************************************/
gint search_friend(GtkWidget *widget,struct args2 *a){
	printf("%s\n",a->buf);//============
	GtkWidget *Friend_ID;
	GtkWidget *label;
	GtkWidget *button_confirm;
	GtkWidget *window;
	GtkWidget *Mainbox,*box1;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"添加好友");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
	gtk_widget_set_size_request(window,250,100);
	Mainbox = gtk_vbox_new(FALSE,0);
	box1 = gtk_hbox_new(FALSE,0);
	button_confirm = gtk_button_new_with_label("确定");
	label = gtk_label_new("好友ID:");
	Friend_ID = gtk_entry_new();
	gtk_container_add(GTK_CONTAINER(window),Mainbox);
	gtk_container_set_border_width(GTK_CONTAINER(window),5);
	gtk_box_pack_start(GTK_BOX(Mainbox),box1,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(Mainbox),button_confirm,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(box1),label,FALSE,FALSE,10);
	gtk_box_pack_start(GTK_BOX(box1),Friend_ID,TRUE,TRUE,10);
	struct args2 *tmp;
	tmp = (struct args2 *)malloc(sizeof(struct args2));//此处分配了内存
	*(tmp) = *a;
	tmp->window = window;
	tmp->entry = Friend_ID;
	printf("%s\n",tmp->buf);
	gtk_widget_set_sensitive(a->button_add_friend,FALSE);
	printf("ok\n");
	g_signal_connect(G_OBJECT(button_confirm),"clicked",G_CALLBACK(confirm_friend),tmp);
	g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(close_window_friend),tmp);
	gtk_widget_show_all(window);
}

gint close_window_friend(GtkWidget *widget,struct args2 *a){
	gtk_widget_set_sensitive(a->button_add_friend,TRUE);
	free(a);
}

//刷新好友列表

/**************************************************/
/*名称：update_friendlist
/*描述：查找好友
/*作成日期： 2016.08.23
/*参数：
		 参数1：参数名称 *tmp、参数类型 struct args_friendlist、输入参数、参数含义 当前窗口
		 参数2：参数名称 *attribute_friend、参数类型 struct user  、输入参数、参数含义 操作的好友信息
		 参数2：参数名称 *data、参数类型 struct user_profile  、输入参数、参数含义 操作的好友信息
		 参数2：参数名称 *num_friend、参数类型 int  、输入参数、参数含义 好友数量
		 参数2：参数名称 *lastfocus_friend、参数类型 int  、输入参数、参数含义 当前操作的好友信息
		 参数1：参数名称 *talk_to_user、参数类型 char、输入参数、参数含义 当前操作的好友名字
		 参数2：参数名称 *focus_user、参数类型 char  、输入参数、参数含义 当前鼠标对着的人
		 参数2：参数名称 *friend_page、参数类型 widget  、输入参数、参数含义 好友页面
		 参数2：参数名称 **friend_box、参数类型 widget  、输入参数、参数含义 好友box
		 参数2：参数名称 **label_friendlist、参数类型 widget  、输入参数、参数含义 好友的标签

/*返回值：gint
/*作者：郝程乾
/***************************************************/

gint update_friendlist(struct args_friendlist *tmp,struct user *attribute_friend,struct user_profile *data,int *num_friend,int *lastfocus_friend,char *talk_to_user,char *focus_user,GtkWidget *friend_page,GtkWidget **friend_box,GtkWidget **label_friendlist){
	int i;
	for(i = 0;i < *num_friend;i++){
		gtk_widget_destroy(label_friendlist[i]);
		gtk_widget_destroy(friend_box[i]);
	}
	printf("%d\n",*num_friend);
	*num_friend = friend_list_size;
	printf("%d\n",*num_friend);
	for(i = 0;i < *num_friend;i++){
		tmp[i].num_friend = num_friend;
		tmp[i].talk_to = talk_to_user;
		tmp[i].event_box = friend_box;
		tmp[i].lastfocus_friend = lastfocus_friend;
		tmp[i].attribute_friend = attribute_friend;
		tmp[i].focus_user = focus_user;
		attribute_friend[i].status = data[i].status;
		attribute_friend[i].username = data[i].username;
		printf("%s\n",attribute_friend[i].username);
	}
	printf("init is ok\n");
	qsort(attribute_friend,(*num_friend),sizeof(struct user),cmp_friendlist);
	printf("qsort is ok\n");
	for(i = 0;i < *num_friend;i++){
		tmp[i].id = i;
		friend_box[i] = gtk_event_box_new();
		char txt[100];
		strcpy(txt,attribute_friend[i].username);
		int len = strlen(txt);
		if(attribute_friend[i].status){
			strcpy(txt+len,"(在线)");
			gtk_widget_modify_bg(friend_box[i],GTK_STATE_NORMAL,&blue);
		}
		else{
			strcpy(txt+len,"(离线)");
			gtk_widget_modify_bg(friend_box[i],GTK_STATE_NORMAL,&grey);
		}
		if(strcmp(tmp[i].focus_user,attribute_friend[i].username) == 0){
			*lastfocus_friend = i;
			gtk_widget_modify_bg(friend_box[i],GTK_STATE_NORMAL,&choose);
		}
		label_friendlist[i] = gtk_label_new(txt);
		gtk_container_add(GTK_CONTAINER(friend_box[i]),label_friendlist[i]);
		gtk_widget_set_events(friend_box[i],GDK_BUTTON_PRESS);
		gtk_widget_set_size_request(friend_box[i],0,50);
		g_signal_connect(G_OBJECT(friend_box[i]),"enter_notify_event",G_CALLBACK(moveto_friend),&tmp[i]);
		g_signal_connect(G_OBJECT(friend_box[i]),"leave_notify_event",G_CALLBACK(leave_friend),&tmp[i]);
		g_signal_connect(G_OBJECT(friend_box[i]),"button_press_event",G_CALLBACK(focusin_friend),&tmp[i]);
		gtk_box_pack_start(GTK_BOX(friend_page),friend_box[i],FALSE,FALSE,1);
	}
	gtk_widget_show_all(friend_page);
}
