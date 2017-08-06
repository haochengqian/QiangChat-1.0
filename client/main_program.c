/**************************************************/
/*名称：main_program.c
/*描述：打开窗口，分派资源，与server进行通讯，初始化客户端
/*作成日期：2016.8.31
/*参数：void
/*返回值:void
/*作者：全组成员
/***************************************************/
#include "main_program.h"

extern int now_id;
extern int photo_id;
extern int group_list_size;
extern int friend_list_size;
extern int n_g,t_t_g,f_g,l,n_f,l_f;
extern struct widget_pack sky_group,sky_friend;
extern GtkWidget *group_box[N];//eventbox
extern GtkWidget *label_grouplist[N];	//label_grouplist
extern GtkWidget *group_page;
extern int *num_group;
extern int *talk_to_group; //current group
extern int *focus_group;
extern char group_ID[100];//群ID(string)
extern int *lastfocus;
extern struct args_grouplist args_for_callback_grouplist[N];//回调函数参数
extern struct group_profile attribute_group[N];		 //eventbox属性
extern struct group_profile groupdata[N]; //获取的群组属性
extern struct user_profile userdata[N]; //获取的用户属性
extern GtkWidget *friend_box[N];//eventbox
extern GtkWidget *label_friendlist[N];	//label_friendlist
extern GtkWidget *friend_page;
extern int *num_friend;
extern char talk_to_user[100]; //current user
extern char focus_user[100];
extern char friend_ID[100];//加好友用
extern int lastfocus_friend;
extern struct args_friendlist args_for_callback_friendlist[N];//回调函数参数
extern struct user attribute_friend[N];		 //eventbox属性

extern GtkWidget* login_window;
extern GtkWidget* mainUI_window;
extern GtkWidget* regist_window;
extern GtkWidget* mainUI_window_scwinlist;
extern GtkWidget* mainUI_window_group_list_box;
extern GtkWidget* mainUI_window_about_box;
extern GtkWidget* pop_window;
extern char position[1024];
//extern GtkWidget* new_small_window_label;

extern GtkWidget *window;
extern GtkWidget *vbox;
extern GtkWidget *hbox;
extern GtkWidget *bbox;
extern GtkWidget *button;
extern GtkWidget *label;
extern GtkWidget *image;
extern GtkWidget *sep;

extern GtkWidget *zaq_image,*zaq_button,*zaqfather_image;
extern GtkWidget *global_button,*global_image;
extern GtkWidget *hboxofzaqup,*hboxofzaqdown;
//=============================NETWORK===============================
int serverfd;
struct sockaddr_in server;
char current_username[32];

char buffer[BUFFER_SIZE], send_buffer[BUFFER_SIZE];
//=============================NETWORK===============================

//void set_chat_title(GtkWidget *cmlabel, const char *buf);  
// buf should be like "<span foreground=\"white\"><big>Communicating With Bilv</big></span>"

char *title_format_1 = "<span foreground=\"white\"><big>";
char *title_format_2 = "</big></span>";
char title_buffer[1024];
static pthread_mutex_t testlock;
//套接字转换==================================
struct Sock_ntop_Transform temp_transform_information;

/**************************************************/
/*名称：sock_ntop
/*描述：将套接字转化成ip
/*作成日期：2016.8.31
/*参数：
	const int sin_addr 套接字地址结构体 输入参数
/*返回值：char *str ,ip地址
/*作者：郝程乾
/***************************************************/
char *sock_ntop(const int sin_addr)
{
 	char portstr[7];
	static char str[128];
      if (inet_ntop(AF_INET, sin_addr, str, sizeof(str))==NULL)
        return NULL;
   //debug
   printf("套接字：%s",str);
      return str;
}
//=====================================================

/**************************************************/
/*名称：focusin_friend_callback
/*描述：与某一好友聊天的回调函数
/*作成日期：2016.8.31
/*参数：VOID
/*返回值：VOID
/*作者：郝程乾
/***************************************************/
void focusin_friend_callback() {
	*talk_to_group = 0;
	printf("focusin friend %s\n", talk_to_user);
	exec_cmd(15,talk_to_user,NULL);
	printf("this touxiang is :%d\n",photo_id);
	clear_chat_window();
	exec_cmd(13, "single", talk_to_user);
	// 藏 群成员列表
	change_chat_mode(1);
	sprintf(title_buffer, "%s%s%s", title_format_1, talk_to_user, title_format_2);
	set_chat_title(cmlabel, title_buffer);
}

/**************************************************/
/*名称：focusin_group_callback
/*描述：与某一组群聊天的回调函数
/*作成日期：2016.8.31
/*参数：VOID
/*返回值：VOID
/*作者：郝程乾
/***************************************************/
void focusin_group_callback() {
	memset(talk_to_user, '\0', sizeof(talk_to_user));
	printf("focusin group %d\n", *talk_to_group);
	clear_chat_window();
	exec_cmd(13, "group", talk_to_group);
	//显示 群成员列表	
	change_chat_mode(2);
	exec_cmd(8, talk_to_group, NULL);
	sprintf(title_buffer, "%s%d%s", title_format_1, *talk_to_group, title_format_2);
	set_chat_title(cmlabel, title_buffer);
	
}
//TODO: 弹窗！！！

//加群

/**************************************************/
/*名称：confirm_group_callback
/*描述：与某一组群聊天的加群回调函数
/*作成日期：2016.8.31
/*参数：VOID
/*返回值：VOID
/*作者：郝程乾
/***************************************************/
void confirm_group_callback() {
	printf("confirm group callback %s\n", group_ID);
	int groupID;
	sscanf(group_ID, "%d", &groupID);
	exec_cmd(10, &groupID, NULL);
	sleep(1);
	exec_cmd(7, NULL, NULL);
}

//创建群

/**************************************************/
/*名称：create_group_callback
/*描述：创建某一个组群的回调函数
/*作成日期：2016.8.31
/*参数：VOID
/*返回值：VOID
/*作者：郝程乾
/***************************************************/
void create_group_callback() {
	printf("create group callback\n");
	exec_cmd(9, NULL, NULL);
}

//加好友

/**************************************************/
/*名称：confirm_friend_callback
/*描述：加一个好友的函数
/*作成日期：2016.8.31
/*参数：VOID
/*返回值：VOID
/*作者：郝程乾
/***************************************************/
void confirm_friend_callback() {
	printf("confirm friend callback %s\n", friend_ID);
	exec_cmd(4, friend_ID, NULL);
	sleep(1);
	exec_cmd(6, NULL, NULL);
}

//FIXME: 切换到另一种模式时，把前一种的talk_to清零！！

/**************************************************/
/*名称：send_message_callback
/*描述：点击发送之后的回调函数
/*作成日期：2016.8.31
/*参数：
	char *message 发送的信息
/*返回值：VOID
/*作者：张宇森
/***************************************************/
void send_message_callback(char *message) {
	printf("======================================================\n%s %d\n", talk_to_user, *talk_to_group);
	printf("send message callback %s\n", message);
	if(strlen(talk_to_user) >= 1){
		exec_cmd(5, talk_to_user, message);
	}
	else if(*talk_to_group >= 1) {
		exec_cmd(12, talk_to_group, message);
	}
}

int group_member_count = 0;
struct group_username group_members[32];

const int SHOW_ALL_MESSAGE = 0;

int group_message_list_size;
struct message_list_item group_message_list[64];

int single_message_list_size;
struct message_list_item single_message_list[64];


/**************************************************/
/*名称：login
/*描述：点击登陆后的回调函数
/*作成日期：2016.8.31
/*参数：
	char *username 用户名称
	char *password 用户密码
/*返回值：int 1表示登陆成功
/*作者：张宇森
/***************************************************/
int login(char *username, char *password) {
	printf("login %s %s\n", username, password);
	exec_cmd(2, username, password);
	return 1;
}
/**************************************************/
/*名称：regist
/*描述：点击注册后的回调函数
/*作成日期：2016.8.31
/*参数：
	char *username 用户名称
	char *password 用户密码
/*返回值：int 1表示登陆成功
/*作者：张宇森
/***************************************************/
int regist(char *username, char *password) {
	printf("regist %s %s\n", username, password);
	exec_cmd(1, username, password);
	return 1;
}

//the database
sqlite3 *db;

//initialize the database
//initialize while user login

/**************************************************/
/*名称：db_init
/*描述：初始化数据库
/*作成日期：2016.8.31
/*参数：
	char *filename 数据库文件的名称
/*返回值：int 1表示登陆成功
/*作者：迟泽闻
/***************************************************/
int db_init(char *filename) {
	if(SQLITE_OK != sqlite3_open(filename, &db))
		return 0;
	if(SQLITE_OK != sqlite3_exec(db, "create table if not exists chatrecord_single(sendto TEXT NOT NULL, sendfrom TEXT NOT NULL, sendtime TEXT NOT NULL, content TEXT NOT NULL)",
		NULL, NULL, NULL)) return 0;
	if(SQLITE_OK != sqlite3_exec(db, "create table if not exists chatrecord_group(sendto INTEGER NOT NULL, sendfrom TEXT NOT NULL, sendtime TEXT NOT NULL,  content TEXT NOT NULL)",
		NULL, NULL, NULL)) return 0;
	return 1;
}

/**************************************************/
/*名称：init_main_window
/*描述：初始化主窗口
/*作成日期：2016.8.31
/*参数：VOID
/*返回值：VOID
/*作者：迟泽闻
/***************************************************/
void init_main_window() {
	sprintf(title_buffer, "%s%s%s", title_format_1, "请选择联系人或群组", title_format_2);
	set_chat_title(cmlabel, title_buffer);
	//clear_chat_window();
	//change_chat_mode(1);
}

/**************************************************/
/*名称：login_action
/*描述：登陆动作的实现（与数据库链接）
/*作成日期：2016.8.31
/*参数：
	char *username
/*返回值：VOID
/*作者：迟泽闻
/***************************************************/
void login_action(char *username) {
	char db_name[64];
	sprintf(db_name, "client-%s.db", username);
	
	if(!db_init(db_name)){
		printf("database initialization failed.\n");
	} else {
		strcpy(current_username, username);
		printf("log in as %s\n", current_username);
	}
	gdk_threads_enter();
	mainUI_window_show(username);
	init_main_window();
	gdk_threads_leave();
	exec_cmd(6, NULL, NULL);
	exec_cmd(7, NULL, NULL);
}

/**************************************************/
/*名称：logout_action
/*描述：登出动作的实现（与数据库链接）
/*作成日期：2016.8.31
/*参数：VOID
/*返回值：VOID
/*作者：迟泽闻
/***************************************************/
void logout_action() {
	//close database and clear current_username
	sqlite3_close(db);
	db = NULL;
	memset(current_username, '\0', sizeof(current_username));
}

/**************************************************/
/*名称：show_chatrecord_single
/*描述：显示和一个好友聊天的离线记录
/*作成日期：2016.8.31
/*参数：
	char *username 好友名称
/*返回值：VOID
/*作者：迟泽闻
/***************************************************/
void show_chatrecord_single(char *username) {
	if(strcmp(talk_to_user, username) != 0){
		return ;
	}
	printf("\n\nshow chatrecord single\n\n\n");
	clear_chat_window();
	char sqlcmd[1024];
	sprintf(sqlcmd, "select * from chatrecord_single where (sendto=\'%s\' and sendfrom=\'%s\') or (sendto=\'%s\' and sendfrom=\'%s\') order by sendtime asc limit 0, 50", current_username, username, username, current_username);
	char ** res; char * errmsg; int nrow = 0, ncol = 0;
	if(SQLITE_OK != sqlite3_get_table(db, sqlcmd, &res, &nrow, &ncol, &errmsg)) {
		printf("query chat record failed\n");	
		return ;
	}
	printf("chat record with user %s\n", username);
	int i, j; int nindex = ncol;
	single_message_list_size = nrow;
	char info_buf[1024];
	for(i=0;i<nrow;i++){
		for(j=0;j<ncol;j++){
			if(j == 1) {
				sprintf(single_message_list[i].message_prop, "%s ", res[nindex]);
			} else if(j == 2) {
				strcat(single_message_list[i].message_prop, res[nindex]);
			} else if(j == 3) {
				sprintf(single_message_list[i].content, res[nindex]);
			}
			//printf("%s ", res[nindex]);
			nindex++;
		}
		strcat(single_message_list[i].message_prop, "\n");
		printf("print msg %s\n", single_message_list[i].content);
		print_msg(&viewinfo, single_message_list[i].message_prop, single_message_list[i].content);
		//printf("\n");
	}
}

//===========================修改头像
/**************************************************/
/*名称：change_my_portrait
/*描述：更改个人的头像
/*作成日期：2016.8.31
/*参数：
	int status 表示要输入的头像的标号是几
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void change_my_portrait(int status){
	gtk_widget_destroy(global_image);
	gtk_widget_destroy(zaq_image);
	char buf_string[20];
	int i = status;
	photo_id = i;
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
/*名称：change_you_portrait
/*描述：更改对方头像的显示
/*作成日期：2016.8.31
/*参数：
	int status 表示要输入的头像的标号是几
/*返回值：VOID
/*作者：郑安庆
/***************************************************/
void change_you_portrait(int status){
	gtk_widget_destroy(zaqfather_image);
	char buf_string[20];
	int i = status;
	photo_id = i;
	sprintf(buf_string,"portrait/%d.png",i);
	printf("====================inchange you portrait\n==============\n");
	printf("%s\n",buf_string);
	printf("=======================================\n");
	zaqfather_image=gtk_image_new_from_file(buf_string);
	gtk_container_add(GTK_CONTAINER(hboxofzaqup),zaqfather_image);
	gtk_widget_show_all(hboxofzaqup);
}
//====================================

/**************************************************/
/*名称：show_chatrecord_group
/*描述：显示组群的聊天记录
/*作成日期：2016.8.31
/*参数：
	int groupID 表示要组群的id是几
/*返回值：VOID
/*作者：张宇森
/***************************************************/
void show_chatrecord_group(int groupID) {
	printf("%d %d\n", groupID, *talk_to_group);
	if(groupID != *talk_to_group){
		return ;
	}
	printf("\n\nshow chatrecord group\n\n\n");
	clear_chat_window();
	char sqlcmd[1024];
	sprintf(sqlcmd, "select * from chatrecord_group where (sendto=%d) order by sendtime asc limit 0, 50", groupID);
	char ** res; char * errmsg; int nrow = 0, ncol = 0;
	if(SQLITE_OK != sqlite3_get_table(db, sqlcmd, &res, &nrow, &ncol, &errmsg)) {
		printf("query chat record failed\n");	
		return ;
	}
	printf("chat record with group %d\n", groupID);
	int i, j; int nindex = ncol;
	group_message_list_size = nrow;
	for(i=0;i<nrow;i++){
		for(j=0;j<ncol;j++){
			if(j == 1) {
				sprintf(group_message_list[i].message_prop, "%s ", res[nindex]);
			} else if(j == 2) {
				strcat(group_message_list[i].message_prop, res[nindex]);
			} else if(j == 3) {
				sprintf(group_message_list[i].content, res[nindex]);
			}
			//printf("%s ", res[nindex]);
			nindex++;
		}
		strcat(group_message_list[i].message_prop, "\n");
		printf("print msg %s\n", group_message_list[i].content);
		print_msg(&viewinfo, group_message_list[i].message_prop, group_message_list[i].content);
		//printf("\n");
	}
}


/**************************************************/
/*名称：save_chatrecord_single
/*描述：保存个人聊天的聊天记录
/*作成日期：2016.8.31
/*参数：
	const char * jsonstr 聊天内容的JSON包
/*返回值：VOID
/*作者：张宇森
/***************************************************/
//save chat record for single chat
void save_chatrecord_single(const char * jsonstr) {
	printf("save chatrecord single %s\n", jsonstr);
	cJSON *root = cJSON_Parse(jsonstr);
	if(root == NULL) return ;
	char *sendfrom = cJSON_GetObjectItem(root, "sendfrom")->valuestring;
	char *sendto = cJSON_GetObjectItem(root, "sendto")->valuestring;
	char *sendtime = cJSON_GetObjectItem(root, "sendtime")->valuestring;
	char *content = cJSON_GetObjectItem(root, "content")->valuestring;
	char sqlcmd[2048];
	sprintf(sqlcmd, "insert into chatrecord_single values(\'%s\', \'%s\', \'%s\', \'%s\')", sendto, sendfrom, sendtime, content);
	if(db != NULL)	
		sqlite3_exec(db, sqlcmd, NULL, NULL, NULL);
	free(root);
	if(strcmp(sendto, current_username) == 0)
		show_chatrecord_single(sendfrom);
	if(strcmp(sendfrom, current_username) == 0)
		show_chatrecord_single(sendto);
}


/**************************************************/
/*名称：save_chatrecord_single
/*描述：保存组群聊天的聊天记录
/*作成日期：2016.8.31
/*参数：
	const char * jsonstr 聊天内容的JSON包
/*返回值：VOID
/*作者：张宇森
/***************************************************/
//save chat record for group chat
void save_chatrecord_group(const char *jsonstr) {
	printf("save chatrecord group %s\n", jsonstr);
	cJSON *root = cJSON_Parse(jsonstr);
	if(root == NULL) return ;

	int sendto = cJSON_GetObjectItem(root, "sendto")->valueint;
	printf("%d\n", sendto);
	char *sendfrom = cJSON_GetObjectItem(root, "sendfrom")->valuestring;
	printf("%s\n", sendfrom);
	char *sendtime = cJSON_GetObjectItem(root, "sendtime")->valuestring;
	printf("%s\n", sendtime);
	char *content = cJSON_GetObjectItem(root, "content")->valuestring;
	printf("len=  %d\n", strlen(content));
	printf("%s\n", content);

	char sqlcmd[2048];
	printf("before sprintf\n");
	sprintf(sqlcmd, "insert into chatrecord_group values(%d, \'%s\', \'%s\', \'%s\')", sendto, sendfrom, sendtime, content);
	printf("after sprintf\n");
	if(db != NULL)
		sqlite3_exec(db, sqlcmd, NULL, NULL, NULL);
	printf("wokao\n");
	show_chatrecord_group(sendto);
}

/**************************************************/
/*名称：send_main
/*描述：发送线程与服务器端的交互
/*作成日期：2016.8.31
/*参数：
	void *target_temp  目标地址
/*返回值：VOID
/*作者：迟泽闻
/***************************************************/
void send_main( void *target_temp ){  //传送线程
    struct Sock_ntop_Transform target = *(struct Sock_ntop_Transform *) target_temp;
    char * target_ip = target.ip;
    struct sockaddr_in client_addr;  
    bzero(&client_addr, sizeof(client_addr));  
    client_addr.sin_family = AF_INET; // internet协议族  
    client_addr.sin_addr.s_addr = htons(INADDR_ANY); // INADDR_ANY表示自动获取本机地址  
    client_addr.sin_port = htons(0); // auto allocated, 让系统自动分配一个空闲端口  
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);  
    if (client_socket < 0)  
    {  
        printf("Create Socket Failed!\n");  
        exit(1);  
    }  
    if (bind(client_socket, (struct sockaddr*)&client_addr, sizeof(client_addr)))  
    {  
        printf("Client Bind Port Failed!\n");  
        exit(1);  
    }
    struct sockaddr_in  server_addr;  
    bzero(&server_addr, sizeof(server_addr));  
    server_addr.sin_family = AF_INET;  
    if (inet_aton(target_ip, &server_addr.sin_addr) == 0)  
    {  
        printf("Server IP Address Error!\n");  
        exit(1);  
    }   
    server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);  
    socklen_t server_addr_length = sizeof(server_addr);  
    if (connect(client_socket, (struct sockaddr*)&server_addr, server_addr_length) < 0)  
    {  
        printf("Can Not Connect To %s!\n", target_ip);  
        exit(1);  
    }  
  
    char file_name[FILE_NAME_MAX_SIZE + 1];  
    bzero(file_name, sizeof(file_name));  
    printf("Please Input File Name That You Want To Send.\t");  
    strcpy(file_name,target.position);//复制地址进 file_name
    
    char buffer[FILE_BUFFER_SIZE];  
    bzero(buffer, sizeof(buffer));  
    strncpy(buffer, file_name, strlen(file_name) > FILE_BUFFER_SIZE ? FILE_BUFFER_SIZE : strlen(file_name));  
    send(client_socket, buffer, FILE_BUFFER_SIZE, 0);
    int length = recv(client_socket, buffer, FILE_BUFFER_SIZE, 0);
    if(buffer[0] == 'O' && buffer[1] == 'K') {
        FILE *fp = fopen(file_name, "r");  
        if (fp == NULL)  
        {  
            printf("File:\t%s Not Found!\n", file_name);  
        }  
        else  
        {  
            bzero(buffer, FILE_BUFFER_SIZE);  
            int file_block_length = 0;  
            while( (file_block_length = fread(buffer, sizeof(char), FILE_BUFFER_SIZE, fp)) > 0)  
            {  
                printf("file_block_length = %d\n", file_block_length);  
                if (send(client_socket, buffer, file_block_length, 0) < 0)  
                {  
                    printf("Send File:\t%s Failed!\n", file_name);  
                    break;  
                }  
                bzero(buffer, sizeof(buffer));  
            }  
            fclose(fp);  
            printf("File:\t%s Transfer Finished!\n", file_name);  
        }  
        close(client_socket);  
    }
    else {
        close(client_socket);
        printf("send req rej\n");
    }
}

/**************************************************/
/*名称：handle_message
/*描述：发送线程与服务器端的交互
/*作成日期：2016.8.31
/*参数：
	char *message 处理发过来信息
/*返回值：VOID
/*作者：迟泽闻
/***************************************************/
void handle_message(char *message){
	//gdk_threads_enter();
	printf("handle message start, serverfd = %d\n", serverfd);
    cJSON *root = cJSON_Parse(message);
    if(root == NULL){
        printf("json parse error, message is \"%s\"\n", message);
        return ;
    }
    char *type = cJSON_GetObjectItem(root, "type")->valuestring;

	printf("handle_message: %s\n" ,type);


    if(strcmp(type, "register-receipt") == 0){
        int status = cJSON_GetObjectItem(root, "status")->valueint;
        char *username = cJSON_GetObjectItem(root, "username")->valuestring;
		gdk_threads_enter();
        if(status == 1 && username != NULL){
            printf("registered new user %s successfully\n", username);
			create_new_pop_window("regist successful!");
        } else {
			printf("register new user %s fail\n", username);
			create_new_pop_window("regist failed!");
		}
		gdk_threads_leave();
    } else if(strcmp(type, "login-receipt") == 0){
		//received the login receipt from server
        int status = cJSON_GetObjectItem(root, "status")->valueint;
        char *username = cJSON_GetObjectItem(root, "username")->valuestring;
		int userimage = cJSON_GetObjectItem(root, "image")->valueint;
        if(status == 1 && username != NULL){
			login_action(username);
			gdk_threads_enter();
			change_my_portrait(userimage);
			gdk_threads_leave();
        } else {
			printf("qunimabi\n");
			create_new_pop_window("login failed\n");
			exit(1);
		}
    } else if(strcmp(type, "force-logout-notif") == 0){
        char *username = cJSON_GetObjectItem(root, "username");
        if(strcmp(current_username, username) == 0){
            printf("user %s force logout\n", username);
			logout_action();
        }
    } else if(strcmp(type, "message/text") == 0){
		char *sendfrom = cJSON_GetObjectItem(root, "sendfrom")->valuestring;
		char *sendtime = cJSON_GetObjectItem(root, "sendtime")->valuestring;
		char *content = cJSON_GetObjectItem(root, "content")->valuestring;
		/*
		printf("user %s sent a message to you at %s, \nmessage is %s\n",
			sendfrom, sendtime, content);*/
		//save chat record when receiving new message
		gdk_threads_enter();
		save_chatrecord_single(message);	
		gdk_threads_leave();
		//exec_cmd(13, "single", sendfrom);
    } else if(strcmp(type, "return-message-filetransform") == 0){//收到传文件的指令后新建线程传文件
		char *ipfd = cJSON_GetObjectItem(root, "Ipfd")->valuestring;
		char *sendfrom = cJSON_GetObjectItem(root, "sendfrom")->valuestring;
		if(ipfd == NULL){
			exec_cmd(5,sendfrom,"Sorry your friend is not online.");
		}
		strcpy (temp_transform_information.ip,ipfd);
		strcpy (temp_transform_information.position,cJSON_GetObjectItem(root, "content")->valuestring);
		pthread_mutex_lock(&testlock);
		pthread_t client_thread; 
		printf("caonima");
	    	int client_thread_res = pthread_create(&client_thread, NULL, send_main, (void *)&temp_transform_information);
		pthread_mutex_unlock(&testlock);

    } else if(strcmp(type, "message/text/group") == 0){
		printf("[%d]\n", root == NULL);
		int sendto = cJSON_GetObjectItem(root, "sendto")->valueint;
		printf("%d\n", sendto);
		char *sendfrom = cJSON_GetObjectItem(root, "sendfrom")->valuestring;
		printf("%s\n", sendfrom);
		char *sendtime = cJSON_GetObjectItem(root, "sendtime")->valuestring;
		printf("%s\n", sendtime);
		char *content = cJSON_GetObjectItem(root, "content")->valuestring;
		printf("%s\n", content);
		
		/*
		char content_text[1025];
		int i = 0;
		putchar('\"');
		for(i = 0; *(content+i) != 0; i++) {
			content_text[i] = *(content+i);
			putchar(*(content+i));
		}
		putchar('\"');
		content_text[i] = '\0';
		printf("you have a message from group %d, sent by %s at %s:%s", sendto, sendfrom, sendtime, content_text);
		//for group message, save chat record only when receiving a new message
		*/
		gdk_threads_enter();
		save_chatrecord_group(message);
		
		gdk_threads_leave();
		//exec_cmd(13, "group", &sendto);
    } else if(strcmp(type, "friend-list") == 0){
		//friend list
		int size = cJSON_GetObjectItem(root, "size")->valueint;
		cJSON *list = cJSON_GetObjectItem(root, "list");
		int i;
		printf("friend list of user %s:\n", current_username);
		friend_list_size = size;
		for(i = 0; i < size; i++){
			cJSON *item = cJSON_GetArrayItem(list, i);
			char *username = cJSON_GetObjectItem(item, "username")->valuestring;
			int status = cJSON_GetObjectItem(item, "status")->valueint;
			printf("%s [%s]\n", username, (status ? "online":"offline"));
			strcpy(userdata[i].username, username);
			userdata[i].status = status;
		}
		printf("friend list size = %d\n", friend_list_size);
		gdk_threads_enter();
		update_friendlist(UPDATE_FRIENDLIST);
		gdk_threads_leave();
    } else if(strcmp(type, "group-create-receipt") == 0){
		int status = cJSON_GetObjectItem(root, "status")->valueint;
		if(status == 1) {
			int groupID = cJSON_GetObjectItem(root, "groupID")->valueint;
			printf("create group success, new group ID is %d\n", groupID);
		} else {
			printf("create group failed");
		}
    } else if(strcmp(type, "group-list") == 0){
		//group list
		int size = cJSON_GetObjectItem(root, "size")->valueint;
		cJSON *list = cJSON_GetObjectItem(root, "list");
		int i;
		printf("groups of user %s:\n", current_username);
		group_list_size = size;
		for(i = 0; i < size; i++){
			cJSON *item = cJSON_GetArrayItem(list, i);
			int groupID = cJSON_GetObjectItem(item, "groupID")->valueint;
			printf("group %d\n", groupID);
			groupdata[i].groupID = groupID;
		}
		gdk_threads_enter();
		update_grouplist(UPDATE_GROUPLIST);
		gdk_threads_leave();
    } else if(strcmp(type, "group-profile") == 0){
		//group profile
		//FIXME: maybe there is some bug...
		int size = cJSON_GetObjectItem(root, "member-count")->valueint;
		int groupID = cJSON_GetObjectItem(root, "groupID")->valueint;
		printf("profile of group %d:\n%d members in total\n", groupID, size);
		cJSON *list = cJSON_GetObjectItem(root, "list");
		int i;
		group_member_count = size;
		printf("size = %d\n", group_member_count);
		for(i = 0; i < size; i++){
			cJSON *item = cJSON_GetArrayItem(list, i);
			char *username = cJSON_GetObjectItem(item, "username")->valuestring;
			//printf("%s\n", username);
			memset(group_members[i].username, '\0', sizeof(group_members[i].username));
			if(i < 1024) {strcpy(group_members[i].username, username);
				printf("%s\n", group_members[i].username);			
			}
		}
		gdk_threads_enter();
		update_group_friend_list(group_member_count, group_members);
		gdk_threads_leave();
    } else if(strcmp(type, "group-join-receipt") == 0){
		int groupID = cJSON_GetObjectItem(root, "groupID")->valueint;
		int status = cJSON_GetObjectItem(root, "status")->valueint;
		printf("join group %d %s\n", groupID, (status ? "success" : "fail"));
		gdk_threads_enter();
		update_grouplist(UPDATE_GROUPLIST);
		gdk_threads_leave();
    } else if(strcmp(type, "group-quit-receipt") == 0){
		int groupID = cJSON_GetObjectItem(root, "groupID")->valueint;
		int status = cJSON_GetObjectItem(root, "status")->valueint;
		printf("quit group %d %s\n", groupID, (status ? "success" : "fail"));
    } else if(strcmp(type, "add-to-contact-receipt") == 0){
		char *contact = cJSON_GetObjectItem(root, "contact")->valuestring;
		int status = cJSON_GetObjectItem(root, "status")->valueint;
		printf("add contact %s %s\n", contact, (status ? "success" : "fail"));
		gdk_threads_enter();
		update_friendlist(UPDATE_FRIENDLIST);
		gdk_threads_leave();
    } else if(strcmp(type, "friend-image-receipt") == 0){
		cJSON_Print(root);
		int status = cJSON_GetObjectItem(root,"image")->valueint;
		char *username = cJSON_GetObjectItem(root,"sendto")->valuestring;
		char *sendfrom = cJSON_GetObjectItem(root,"sendfrom")->valuestring;
		if(strcmp(username,sendfrom)==0){
			printf("friend-image-receipt:aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
			gdk_threads_enter();
			change_my_portrait(status);
			gdk_threads_leave();
		}
		else{
			printf("friend-image-receipt:bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\n");
			gdk_threads_enter();
			change_you_portrait(status);
			gdk_threads_leave();
		}
	}
	printf("handle message over, serverfd = %d\n", serverfd);
	//gdk_threads_leave();
}

/**************************************************/
/*名称：exec_cmd
/*描述：上面所有相关操作的执行部分的实现
/*作成日期：2016.8.31
/*参数：
	int op 操作类型
	void *arg1 对于不同操作类型有不同的意义
	void *arg2 对于不同操作类型有不同的意义
/*返回值：VOID
/*作者：张宇森
/***************************************************/
void exec_cmd(int op, void *arg1, void *arg2) {
    cJSON *root = cJSON_CreateObject();
	printf("exec_cmdinit==============\n");
    if(root == NULL){
        printf("create json object failed\n");
        return;
    }
    if(op >= 3 && strlen(current_username) == 0){
        printf("haven't logged in");
        return ;
    }
	printf("%d===================\n",op);
    switch(op){
	printf("exec_cmdinit switch==============\n");
    //send to server:      0 <any string>
    case 0:
    send_function((char *) arg1);
    break;

	//register:            1 <username> <password>
	case 1:
    cJSON_AddStringToObject(root, "type", "register-message");
    cJSON_AddStringToObject(root, "username", (char *) arg1);
    cJSON_AddStringToObject(root, "password", (char *) arg2);
    send_function(cJSON_Print(root));
    break;

	//login:               2 <username> <password>
	case 2:
    cJSON_AddStringToObject(root, "type", "login-message");
    cJSON_AddStringToObject(root, "username", (char *) arg1);
    cJSON_AddStringToObject(root, "password", (char *) arg2);
    send_function(cJSON_Print(root));
    break;

	//logout:              3\n
	case 3:
    cJSON_AddStringToObject(root, "type", "logout-message");
    cJSON_AddStringToObject(root, "username", current_username);
	send_function(cJSON_Print(root));
	logout_action();
    break;

	//add friend:          4 <username>
	case 4:
    cJSON_AddStringToObject(root, "type", "add-to-contact-request");
    cJSON_AddStringToObject(root, "username", current_username);
    cJSON_AddStringToObject(root, "contact", (char *) arg1);
    send_function(cJSON_Print(root));
    break;


	//text message:        5 <sendto> <content>
	case 5:
	if(strlen((char*)arg2) > 1024){
		return;
	}
    cJSON_AddStringToObject(root, "type", "message/text");
    cJSON_AddStringToObject(root, "sendto", (char *) arg1);
    cJSON_AddStringToObject(root, "sendfrom", current_username);
    cJSON_AddStringToObject(root, "sendtime", get_formatted_time());
    cJSON_AddStringToObject(root, "content", (char *) arg2);
    send_function(cJSON_Print(root));
	//save the chat record while sending message
	save_chatrecord_single(cJSON_Print(root));
    break;

	//show friend list:    6
	case 6:
	cJSON_AddStringToObject(root, "type", "friend-list-request");
    cJSON_AddStringToObject(root, "username", current_username);
    send_function(cJSON_Print(root));
    break;

	//show group list:     7
	case 7:
	cJSON_AddStringToObject(root, "type", "group-list-request");
    cJSON_AddStringToObject(root, "username", current_username);
    send_function(cJSON_Print(root));
    break;

	//show group profile:  8 <groupID>
	case 8:
    cJSON_AddStringToObject(root, "type", "group-profile-request");
    cJSON_AddStringToObject(root, "username", current_username);
    cJSON_AddNumberToObject(root, "groupID", *(int*)arg1);
    send_function(cJSON_Print(root));
    break;

	//create new group:    9
	case 9:
    cJSON_AddStringToObject(root, "type", "group-create-request");
    cJSON_AddStringToObject(root, "username", current_username);
    send_function(cJSON_Print(root));
    break;

	//join group:          10 <groupID>
	case 10:
    cJSON_AddStringToObject(root, "type", "group-join-request");
    cJSON_AddStringToObject(root, "username", current_username);
    cJSON_AddNumberToObject(root, "groupID", *(int*)arg1);
    send_function(cJSON_Print(root));
    break;

	//quit group:          11 <groupID>
	case 11:
    cJSON_AddStringToObject(root, "type", "group-quit-request");
    cJSON_AddStringToObject(root, "username", current_username);
    cJSON_AddNumberToObject(root, "groupID", *(int*)arg1);
    send_function(cJSON_Print(root));
    break;

	//send group message:  12 <content>
	case 12:
	if(strlen((char*)arg2) > 1024){
		return;
	}
    cJSON_AddStringToObject(root, "type", "message/text/group");
    cJSON_AddNumberToObject(root, "sendto", *(int*)arg1);
    cJSON_AddStringToObject(root, "sendfrom", current_username);
    cJSON_AddStringToObject(root, "sendtime", get_formatted_time());
    cJSON_AddStringToObject(root, "content", (char *)arg2);
	cJSON_AddStringToObject(root, "msgID", "segmentfault");
    send_function(cJSON_Print(root));
    break;

	//show history message:13 <group|single> <groupID|username>
	case 13:
	if(strcmp((char *)arg1, "single") == 0) {
		show_chatrecord_single((char *)arg2);
	} else if(strcmp((char *)arg1, "group") == 0) {
		show_chatrecord_group(*(int*)arg2);
	}
    break;
	
	case 14:
//	printf("%s=============================\n",(char *)arg1);
	if(strlen((char *)arg1)>1024){
		return ;	
	}
	
    cJSON_AddStringToObject(root, "type", "message/file");
    cJSON_AddStringToObject(root, "sendto", (char *) arg1);//发给谁
	printf("=========%s==========\n",(char *)arg1);
    cJSON_AddStringToObject(root, "sendfrom", current_username);//当前发送人
    cJSON_AddStringToObject(root, "sendtime", get_formatted_time());//当前时间
    cJSON_AddStringToObject(root, "content", (char *) arg2);//文件名
    send_function(cJSON_Print(root));
	//save the chat record while sending message
	save_chatrecord_single(cJSON_Print(root));
    break;

	case 15:
	if(strlen((char *)arg1)>1024)
		return ;
	cJSON_AddStringToObject(root,"type","friend-image-request");
	cJSON_AddStringToObject(root,"friendname",(char *) arg1);
	cJSON_AddStringToObject(root,"myname",current_username);
	send_function(cJSON_Print(root));
	save_chatrecord_single(cJSON_Print);
	break;

	case 16:
	if(strlen((char *)arg1)>1024)
		return;
	cJSON_AddStringToObject(root,"type","image-modify-request");
	cJSON_AddStringToObject(root,"username",(char *)arg1);
	cJSON_AddNumberToObject(root,"userimage",*(int *)arg2);

	send_function(cJSON_Print(root));

    default:break;
	};
}

/**************************************************/
/*名称：send_function
/*描述：负责发送信息的函数
/*作成日期：2016.8.31
/*参数：
	char *message 所要发送的函数
/*返回值：VOID
/*作者：张宇森
/***************************************************/
//TODO: send in another thread?
int send_function(char *message) {
    memset(send_buffer, '\0', sizeof(send_buffer));
    strcpy(send_buffer, message);
    printf("sending message to server:\n%s\n", send_buffer);
	return send(serverfd, send_buffer, BUFFER_SIZE, 0);
}

/**************************************************/
/*名称：auto_update_thread
/*描述：自动更新的进程
/*作成日期：2016.8.31
/*参数：VOID
/*返回值：VOID
/*作者：郝程乾
/***************************************************/
void auto_update_thread() {
	while(1) {
		sleep(5);
		exec_cmd(6, NULL, NULL);
		sleep(10);
		exec_cmd(7, NULL, NULL);
		sleep(10);
	}
}

/**************************************************/
/*名称：recv_thread
/*描述：接受信息的进程
/*作成日期：2016.8.31
/*参数：VOID
/*返回值：VOID
/*作者：郝程乾
/***************************************************/
void recv_thread() {
	int numbytes;
	while(1) {
		printf("recv serverfd = %d\n", serverfd);
		memset(buffer, 0, BUFFER_SIZE);
		numbytes = recv(serverfd, buffer, BUFFER_SIZE, 0);
		printf("recv over, serverfd = %d, numbytes = %d\n", serverfd);
		if(numbytes == 0){
			printf("server offline.\n");
			exit(1);
		}
		if(numbytes == -1){
			perror("error receiving message\n");
			exit(1);
		}
		printf("recv judge over, serverfd = %d\n", serverfd);
		//buffer[numbytes] = '\0';
        	printf("received message from server: \"%s\"\n", buffer);
		printf("before handle message, serverfd = %d\n", serverfd);
		//gdk_threads_enter();
        handle_message(buffer);
		//gdk_threads_leave(); 
	}
}
//=======================收文件
int is_file_accepted_temp=-1;
/**************************************************/
/*名称：on_ok_clicked
/*描述：点击发送信息按钮的回调函数
/*作成日期：2016.8.31
/*参数：VOID
/*返回值：VOID
/*作者：郝程乾
/***************************************************/
void on_ok_clicked(GtkButton* button,gpointer data){
	is_file_accepted_temp = 1;
}

/**************************************************/
/*名称：on_cancel_clicked
/*描述：点击取消发送信息按钮的回调函数
/*作成日期：2016.8.31
/*参数：VOID
/*返回值：VOID
/*作者：郝程乾
/***************************************************/
void on_cancel_clicked(GtkButton* button,gpointer data){
	is_file_accepted_temp = 0;
}

/**************************************************/
/*名称：is_file_accepted_new_object
/*描述：产生是否接受文件的对话框
/*作成日期：2016.8.31
/*参数：VOID
/*返回值：VOID
/*作者：郝程乾
/***************************************************/
void is_file_accepted_new_object(){
	printf("wocaonima\n");
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
//	gtk_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
	gtk_window_set_title(GTK_WINDOW(window),"是否接收文件");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window),10);
	vbox=gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(window),vbox);
	hbox=gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,5);
	image=gtk_image_new_from_stock(GTK_STOCK_DIALOG_WARNING,GTK_ICON_SIZE_DIALOG);
	gtk_box_pack_start(GTK_BOX(hbox),image,FALSE,FALSE,5);
	label=gtk_label_new("如果确认收取点击确认，都则点击取消\n");
	gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,5);
	sep=gtk_hseparator_new();
	gtk_box_pack_start(GTK_BOX(vbox),sep,FALSE,FALSE,5);
	bbox=gtk_hbutton_box_new();
	gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox),GTK_BUTTONBOX_SPREAD);
	gtk_box_pack_start(GTK_BOX(vbox),bbox,FALSE,FALSE,5);
	button=gtk_button_new_from_stock(GTK_STOCK_OK);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(on_ok_clicked),NULL);
	g_signal_connect_swapped(G_OBJECT(button),"clicked",G_CALLBACK(gtk_widget_destroy),window);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,0);
	button=gtk_button_new_from_stock(GTK_STOCK_CANCEL);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(on_cancel_clicked),NULL);
	g_signal_connect_swapped(G_OBJECT(button),"clicked",G_CALLBACK(gtk_widget_destroy),window);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,0);
	gtk_widget_show_all(window);
}

/**************************************************/
/*名称：is_file_accepted
/*描述：判断是否接受文件的函数
/*作成日期：2016.8.31
/*参数：VOID
/*返回值：VOID
/*作者：迟泽闻
/***************************************************/
void is_file_accepted() {
	gdk_threads_init();
	gdk_threads_enter();
	g_thread_create((GThreadFunc)is_file_accepted_new_object, NULL, FALSE, NULL);  
}

/**************************************************/
/*名称：rev_FILE_thread
/*描述：监听接受文件的线程函数
/*作成日期：2016.8.31
/*参数：VOID
/*返回值：VOID
/*作者：迟泽闻
/***************************************************/
void rev_FILE_thread(){
    struct sockaddr_in   server_addr;  
    bzero(&server_addr, sizeof(server_addr));  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);  
    server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);
    int server_socket = socket(PF_INET, SOCK_STREAM, 0);  
    if (server_socket < 0)  
    {  
        printf("Create Socket Failed!\n");  
        exit(1);  
    }  
    
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))  
    {  
        printf("Server Bind Port: %d Failed!\n", HELLO_WORLD_SERVER_PORT);  
        exit(1);  
    }  
    if (listen(server_socket, LENGTH_OF_LISTEN_QUEUE))  
    {  
        printf("Server Listen Failed!\n");  
        exit(1);  
    }  
    printf("server init ok\n");
    while(1)  
    {   
        struct sockaddr_in client_addr;  
        socklen_t          length = sizeof(client_addr );  
        int new_server_socket = accept(server_socket, (struct sockaddr*)&client_addr, &length);  
        if (new_server_socket < 0)  
        {  
            printf("Server Accept Failed!\n");  
            break;  
        }  
  
        char buffer[FILE_BUFFER_SIZE];  
        bzero(buffer, sizeof(buffer));  
        length = recv(new_server_socket, buffer, FILE_BUFFER_SIZE, 0);  
        if (length < 0)  
        {  
            printf("Server Recieve Data Failed!\n");  
            break;  
        }  

        char file_name[FILE_NAME_MAX_SIZE + 1];
	char file_name_temp[FILE_NAME_MAX_SIZE + 1]; 
        bzero(file_name, sizeof(file_name));  
        strncpy(file_name, buffer,  
                strlen(buffer) > FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE : strlen(buffer));  
	int s;int temp_counter=0;
	temp_counter = strlen(file_name);
	for(s = temp_counter - 1;s >= 0; s--){
		if(file_name[s]=='/'){
			break;			
		}
	}
	s++;
	int temp_position;
	strcpy(file_name_temp,"rec_files/");
	for(temp_position = 0;s < temp_counter;s++,temp_position++){
		file_name_temp[temp_position + 10]=file_name[s];
	}
	memset(file_name,0,sizeof(file_name));
	for(s = 0;s < temp_position + 10;s++){
		file_name[s] = file_name_temp[s];	
	}
	printf("file_name:%s\nsbsbsbsbsbsbsbsbsbsbsbsbsbsbsb",file_name);
        is_file_accepted();
	sleep(1);
	gdk_threads_leave();
	if(is_file_accepted_temp == -1){
		while(is_file_accepted_temp == -1);
	}
        if(is_file_accepted_temp == 0) {
	    	close(new_server_socket);
			is_file_accepted_temp=-1;
            printf("Rejected!\n");
            continue;
        }
        strncpy(buffer, "OK",  2);
        send(new_server_socket, buffer, FILE_BUFFER_SIZE, 0);
	
        FILE *fp = fopen(file_name, "w");  
          
        bzero(buffer, sizeof(buffer));  
        int length2 = 0;  
        while(length2 = recv(new_server_socket, buffer, FILE_BUFFER_SIZE, 0))  
        {  
            if (length2 < 0)  
            {  
                printf("Recieve Data From Server Failed!\n");  
                break;  
            }  
    
            int write_length = fwrite(buffer, sizeof(char), length2, fp);  
            if (write_length < length2)  
            {  
                printf("File:\t%s Write Failed!\n", file_name);  
                break;  
            }  
            bzero(buffer, FILE_BUFFER_SIZE);  
        }  
    	
        printf("Recieve File:\t %s From Server Finished!\n", file_name);
	is_file_accepted_temp=-1;
        close(new_server_socket);  
        fclose(fp);
    }
    close(server_socket);
}
//================================

/**************************************************/
/*名称：init_client
/*描述：初始化客户端
/*作成日期：2016.8.31
/*参数：
	char * serverIP 服务器端的IP
/*返回值：
	int 1  表示初始化成功
/*作者：张宇森
/***************************************************/
int init_client(char * serverIP) {
	if(-1 == (serverfd = socket(AF_INET, SOCK_STREAM, 0))) {
		perror("create socket");
		return 0;
	}

	struct hostent *he;
	if((he=gethostbyname(serverIP))==NULL){
		printf("gethostbyname() error\n");
		exit(1);
	}
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(LISTEN_PORT);
	server.sin_addr=*((struct in_addr *)he->h_addr);

	if(-1 == connect(serverfd, (struct sockaddr *)(&server), sizeof(struct sockaddr))) {
		perror("connect");
		return 0;
	}
	return 1;
	/*
	int numbytes;
	//receive the welcome message from server
	
	if((numbytes=recv(serverfd,buffer,BUFFER_SIZE,0))==-1){
		perror("recv");
		exit(1);
	}
	buffer[numbytes]='\0';
	return (strcmp(buffer, "welcome") == 0);
	*/
}


