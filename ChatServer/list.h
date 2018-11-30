/*************************************************************************
    > File Name: list.h
    > Author: liuguangsheng
    > Mail: ma6174@163.com 
    > Created Time: 2018年11月27日 星期二 14时05分16秒
 ************************************************************************/
#ifndef _LIST_H_
#define _LIST_H_
#define lenth 256
#include <stdbool.h>
struct user_msg{
	int id;
	int socket;
	struct user_msg  *next;
	bool login_flag;
	char user_msg[lenth];
};
typedef struct user_msg  MSG;
typedef struct user_msg*  MSG_p;
MSG_p Create_List();
void  Add_List(MSG_p head, MSG msg);
void  Del_List(MSG_p head);
MSG_p   Fin_List(MSG_p head, MSG msg);
void  Delpos_List(MSG_p head, int pos);
#endif

