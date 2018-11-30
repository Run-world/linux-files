/*************************************************************************
    > File Name: list.c
    > Author: liuguangsheng
    > Mail: ma6174@163.com 
    > Created Time: 2018年11月27日 星期二 14时15分44秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"


MSG_p Create_List()
{
	MSG_p head = (MSG_p)malloc(sizeof(MSG));
        if(NULL == head){
		perror("head");
		return NULL;	
	}	

	head->next = NULL;
	return head;
}

void  Add_List(MSG_p head, MSG msg)
{
	MSG_p node = (MSG_p)malloc(sizeof(MSG));
        if(NULL == node){
                 perror("head"); 
                 return NULL;
         }
	
	node->id = msg->id;
	node->login_flag = msg->login_flag;
	strncpy(node->user_msg, msg->user_msg, sizeof(msg->user_msg);

        MSG_p temp = head->next;
	msg->next = temp;
	head->next = msg;

}

void  Delpos__List(MSG_p head, int pos)
{
	MSG_p obs  = head;
	MSG_p temp = head;
	int flag = 0;
	while(pos == flag)
	{
		temp = temp->next;
		flag++;
		if(2 == flag){
			obs = head->next;
			flag = 0;
		}
	}

	obs->next = temp->next;
	free(temp);
}

void  Del__List(MSG_p head)
{
	MSG_p obs  = head;
	MSG_p temp = head;
	int flag = 0;
	while(NULL != (temp = temp->next))
	{       
		flag++;
		if(2 == flag){
			obs = head->next;
			flag = 0;
		}
	}
	
	temp = obs->next;
	obs->next = NULL;
	free(temp);
}

MSG_p   Fin_List(MSG_p head, MSG msg);
