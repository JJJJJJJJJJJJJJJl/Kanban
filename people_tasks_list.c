#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "date.h"
#include "people_tasks_list.h"

//creates tasks list
task_list make_task_list(){
    task_list ward = (task_list)malloc(sizeof(task));

    if(ward != NULL){
        ward->next = NULL;
    }   
    return ward;
}

//finds node right before tasks list split for insertation
void neighbours_task(task_list head, date genesis_target, task_list * prev, task_list * cur){
    * prev = head;
    * cur = head->next;
    
    //prev will be right before where linked list will get splitted
    while(* cur != NULL && date_cmp((* cur)->genesis, genesis_target) == -1){
        * prev = * cur;
        * cur = (* cur)->next; 
    }
    return;
}

//finds person task for insertation
void find_person_task(task_list tasks_head, int done_target, task_list * prev, task_list * cur){
    * prev = tasks_head;
    * cur = tasks_head->next;
    
    if(done_target == 1){
        while((* cur)->next != NULL && (* cur)->done != 1){
            * prev = * cur;
            * cur = (* cur)->next;
        }
    }
    return;
}

//prints all tasks
void show_tasks(task_list tasks_head){
    task_list cur = tasks_head->next;

    while(cur != NULL){
        printf("%s | ", cur->description);
        cur = cur->next;
    }
    printf("\n");
    return;
}

//creates people list
person_list make_people_list(){
    person_list ward = (person_list) malloc(sizeof(person));

    if(ward != NULL){
        ward->next = NULL;
    }
    return ward;
}

//adds new person so people list
void add_person(person_list people_head, int id, char * name){
    person_list new_person = (person_list) malloc(sizeof(person));
    if(new_person != NULL){
        task_list person_tasks = (task_list) malloc(sizeof(task));

        if(person_tasks != NULL){
            person_tasks->next = NULL;
            new_person->id = id;
            new_person->name = name;
            new_person->tasks = person_tasks;
        }
        person_list cur;
        find_last_person(people_head, &cur);
        cur->next = new_person;
        new_person->next = NULL;
    }
    return;
}

//finds target person
void find_person(person_list people_head, char * person_name_target, person_list * cur){
    * cur = people_head->next;
    while(* cur != NULL && strcmp((* cur)->name, person_name_target) != 0){
        * cur = (* cur)->next;
    }
    return;
}

//traverses all people list (insertation purposes)
void find_last_person(person_list people_head, person_list * cur){
    * cur = people_head;
    while((* cur)->next != NULL){
        * cur = (* cur)->next;
    }
    return;
}

//prints people 
void show_people(person_list people_head){
    person_list cur = people_head->next;
    while(cur != NULL){
        printf("%s | ", cur->name);
        cur = cur->next;
    }
    printf("\n");
    return;
}

//prints tasks of specific person   
void show_person_tasks(task_list tasks_head, int person_id_target, int done_size){
    char * doing[5];
    int doing_in = 0;
    char * done[done_size];
    int done_in = 0;
    task_list cur = tasks_head->next; 
    while(cur != NULL){
        if(cur->p != NULL && person_id_target == cur->p->id){
            if(cur->pipeline_pos == 1){
                doing[doing_in++] = cur->description;
            }
            else if(cur->pipeline_pos == 2){
                done[done_in++] = cur->description;
            }
        }
        cur = cur->next;
    }
    printf("Doing: "); 
    for(int i=0; i<doing_in; i++){
        printf("%s ", doing[i]);
    }
    printf("\nDone: ");
    for(int i=0; i<done_in; i++){
        printf("%s ", done[i]);
    }
    printf("\n");
    return;
}