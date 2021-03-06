/* 
    Had to merge both person and task structs 
    so theres no dependency cycle.
*/

#ifndef PEOPLE_TASKS_LIST_H_INCLUDED
#define PEOPLE_TASKS_LIST_H_INCLUDED

#include "date.h"

typedef struct task * task_list;
typedef struct person * person_list;

typedef struct person{
    int id;
    char * name;
    task_list tasks;
    person_list next;
}person;

typedef struct task{
    int id;
    int priority;
    char * description;
    int pipeline_pos;
    person_list p;
    date genesis;
    date deadline;
    date end;
    task_list next;//next in general tasks list 
    task_list next_p;//next in person tasks list
}task;

task_list make_task_list();
void neighbours_task(task_list, date, task_list *, task_list *);
void find_person_task(task_list, int, task_list *, task_list *);
void find_person_task_neighbours(task_list, int, date, task_list *, task_list *);
void show_tasks(task_list);

person_list make_people_list();
void add_person(person_list, int, char *);
void find_person(person_list, int, person_list *);
void find_last_person(person_list, person_list *);
void show_people(person_list);
void show_person_tasks(person_list, int);

#endif
