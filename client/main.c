/**************************************************/
/*名称：main.c
/*描述：整个工程的主函数所在位置
/*作成日期：2016.8.31
/*参数：void
/*返回值：void
/*作者：张宇森
/***************************************************/
#include "main_program.h"

extern GtkWidget* login_window;
extern GtkWidget* mainUI_window;
extern GtkWidget* regist_window;
extern GtkWidget* pop_window;
/**************************************************/
/*名称：main
/*描述：函数的主要接口
/*作成日期：2106.8.31
/*参数：
	int argc 参数个数 输入参数
	char* argv[] 参数列表 输入参数
/*返回值：int 0,表示函数退出成功
/*作者：张宇森
/***************************************************/
int main(int argc,char * argv[]){
	char server_ip[15];
	FILE *fp = fopen("serverip.cfg","r");
	fscanf(fp,"%s",server_ip);
	fclose(fp);

	if(!init_client(server_ip)) {
		printf("init failed\n");
		exit(0);
	}
	if (!g_thread_supported())
		g_thread_init(NULL);
	gdk_threads_init();
	gtk_init(&argc, &argv);
	login_window=loginUI();
	regist_window=registUI();
	mainUI_window=mainUI();
	pop_window=new_small_window();
	gtk_widget_show_all(login_window);
	//g_thread_create((GThreadFunc)UI_thread, NULL, FALSE, NULL);  
	g_thread_create((GThreadFunc)recv_thread, NULL, FALSE, NULL);  
	g_thread_create((GThreadFunc)auto_update_thread, NULL, FALSE, NULL);
	g_thread_create((GThreadFunc)rev_FILE_thread, NULL, FALSE, NULL);  
	//gdk_threads_enter(); 
	gtk_main(); 
	//gdk_threads_leave(); 

	return 0;
}
