#include "people_tasks_list.h"

typedef struct list_node * list; 
typedef struct list_node{
    int size; //Doing(5) list purposes only
    task_list task_card;
    person_list p;//Doing(5) list purposes only
    list next;
}list_node;

list make_list();
void neighbours_todo_card(list, int, date, list *, list *);
void add_task(list, task_list, int, int, char *, date);
void find_card(list, int, list *, list *);
void neighbours_doing_card(list, char *, list *, list *);
void neighbours_done_card(list, date, list *, list *);
void move_task(int, list, list, list, person_list, char *, int, date, date);
void change_doing_task_person(list, person_list, int, char *);
void show_list(list, int);