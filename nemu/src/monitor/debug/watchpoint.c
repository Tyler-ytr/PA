#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head=NULL, *free_;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp(char *str,int value )
{	WP* TEMPH=head;
	 if(free_==NULL)
	 {
		 printf("TOO MANY WATCHPOINT!!!\n WHY NOT CHANGE THE NR_WP IN WATCHPOINT.C?\n");
		 assert(0);
	 }
	WP* TEMPF=NULL;
	TEMPF=free_;
	free_=free_->next;
    TEMPF->value=value;
   	strcpy(TEMPF->str,str);
	if(TEMPH==NULL)
	{
	head=TEMPF;
	head->next=NULL;
	}else
	{
	while(TEMPH->next!=NULL)
	{
	TEMPH=TEMPH->next;//travel until it reaches the end of the list
	}
	TEMPH->next=TEMPF;
	TEMPF->next=NULL;
	}
	return TEMPF;
}

void free_wp(WP *wp)
{
	WP* TEMPH=head;
	WP* TEMPF=free_;

	if(TEMPH==wp)
	{
	head=head->next;	
	
	}else
	{
	while(TEMPH->next!=wp)
	{TEMPH=TEMPH->next;
	}
	TEMPH->next=wp->next;//delete the wp from the head list;
	}
//assert(0);	
	if(TEMPF!=NULL)
	{
	while(TEMPF->next!=NULL)
	{
	TEMPF=TEMPF->next;
	}
	TEMPF->next=wp;
	TEMPF->next->next=NULL;
	}
	else
	{
	free_=wp;
	free_->next=NULL;
	}
}


WP* Whereislist()
{

return head;


}
