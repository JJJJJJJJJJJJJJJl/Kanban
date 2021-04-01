#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pdd_list.h"

int cur_person_id = 1;
int cur_task_id = 1;


//#####################################################################
//PEOPLE DATA HANDLING

void save_people_data(person_list people_head){
    person_list cur = people_head->next;
    FILE * fp = fopen("people.txt", "w");
    if(fp != NULL){
        while(cur != NULL){
            fprintf(fp, "%d %s ", cur->id, cur->name);
            cur = cur->next;
        }
    }
    fclose(fp);
    return;
}

void recover_people_data(person_list people_head){
    FILE * fp = fopen("people.txt", "r");
    if(fp != NULL){
        int person_id;
        char person_name[20];
        while(fscanf(fp, "%d %s ", &person_id, person_name) == 2){
            add_person(people_head, person_id, person_name);
            cur_person_id++;
        }
    }
    fclose(fp);
    return;
}


//#####################################################################
//TASKS DATA HANDLING

void save_tasks_data(task_list tasks_head){
    task_list cur = tasks_head->next;
    FILE * fp = fopen("tasks.txt", "w");
    if(fp != NULL){
        while(cur != NULL){
            fprintf(fp, "%d %d %s %d %d %d ", cur->id, cur->priority, cur->description, cur->genesis.d, cur->genesis.m, cur->genesis.y);
            cur = cur->next;
        }
    }
    return;
}

void recover_tasks_data(list todo_head, task_list tasks_head){
    FILE * fp = fopen("tasks.txt", "r");
    if(fp != NULL){
        int task_id;
        int task_priority;
        char task_description[50];
        date genesis;
        while(fscanf(fp, "%d %d %s %d %d %d ", &task_id, &task_priority, task_description, &genesis.d, &genesis.m, &genesis.y) == 6){
            add_task(todo_head, tasks_head, task_id, task_priority, task_description, genesis);
            cur_task_id++;
        }
    }
    return;
}


//#####################################################################
int main(){
    //sake of testing functions
    task_list tasks_list = make_task_list();
    list todo_list = make_list();
    list doing_list = make_list();
    list done_list = make_list();
    person_list people = make_people_list();
    add_person(people, 1, "jjjjjj");
    add_person(people, 2, "mandelbrot");
    add_person(people, 3, "moc");
    /* recover_people_data(people);
    show_people(people);
    add_person(people, cur_person_id++, "carti");
    show_people(people);
    save_people_data(people); */

    /* add_task(todo_list, tasks_list, 1, 10, "aznag", make_date("29-12-2000"));
    add_task(todo_list, tasks_list, 2, 6, "aznag2", make_date("29-12-2002"));
    add_task(todo_list, tasks_list, 3, 6, "aznag3", make_date("29-12-2001"));
    add_task(todo_list, tasks_list, 4, 8, "aznag4", make_date("29-12-2005")); */
    recover_tasks_data(todo_list, tasks_list);
    show_tasks(tasks_list);
    //add_task(todo_list, tasks_list, cur_task_id++, 7, "hm", make_date("29-12-2050"));
    show_tasks(tasks_list);
    save_tasks_data(tasks_list);
    show_list(todo_list, 1);
    /* printf("###############\n");
    move_task(1, todo_list, doing_list, done_list, people, 3, 4, make_date("29-12-2005"), make_date("29-12-2005"));
    printf("TASKS: ");
    show_tasks(tasks_list);
    show_list(todo_list, 1);
    show_list(doing_list,2);
    show_person_tasks(people, 3, done_list->size);
    move_task(1, todo_list, doing_list, done_list, people, 2, 2, make_date("29-12-2001"), make_date("29-12-2001"));
    printf("TASK2: ");
    show_tasks(tasks_list);
    show_list(todo_list, 1);
    show_list(doing_list,2);
    show_person_tasks(people, 3, done_list->size);
    show_person_tasks(people, 2, done_list->size);
    move_task(1, todo_list, doing_list, done_list, people, 1, 1, make_date("29-12-2000"), make_date("29-12-2000"));
    printf("TASK3: ");
    show_tasks(tasks_list);
    show_list(todo_list, 1);
    show_list(doing_list,2);
    printf("TASK4: ");
    show_tasks(tasks_list);
    //move_task(2, todo_list, doing_list, done_list, people, 1, 1, make_date("29-12-2001"), make_date("29-12-2003"));
    show_person_tasks(people, 1, done_list->size);
    show_person_tasks(people, 3, done_list->size);
    change_doing_task_person(doing_list, people, 1, 3);
    show_person_tasks(people, 1, done_list->size);
    show_person_tasks(people, 3, done_list->size);
    show_list(doing_list,2);
    //remove_task_todo(todo_list, 3);
    show_list(todo_list, 1);
    show_list(doing_list, 2);
    show_list(done_list, 1);
    printf("AFTER MOVING aznags\n");
    show_list(todo_list, 1);
    show_list(doing_list, 2);
    show_tasks(tasks_list);
    show_person_tasks(people, 1, done_list->size);
    show_person_tasks(people, 3, done_list->size);
    show_tasks(tasks_list);
    move_task(2, todo_list, doing_list, done_list, people, 2, 2, make_date("29-12-2001"), make_date("29-12-2001"));
    move_task(2, todo_list, doing_list, done_list, people, 2, 4, make_date("29-12-2005"), make_date("29-12-2005"));
    show_person_tasks(people, 1, done_list->size);
    show_person_tasks(people, 3, done_list->size);
    printf("############################\n");
    show_tasks(tasks_list);
    show_list(todo_list, 1);
    show_list(doing_list, 1);
    show_list(done_list, 1);
    printf("MOVING FROM DONE TO TODO\n");
    move_task(3, todo_list, doing_list, done_list, people, 2, 4, make_date("29-12-2005"), make_date("29-12-2005"));
    show_tasks(tasks_list);
    show_list(todo_list, 1);
    show_list(doing_list, 1);
    show_list(done_list, 1);
    printf("MOVING FROM DOING TO TODO\n");
    move_task(4, todo_list, doing_list, done_list, people, 2, 1, make_date("29-12-2005"), make_date("29-12-2005"));
    show_list(todo_list, 1);
    show_list(doing_list, 2);
    move_task(1, todo_list, doing_list, done_list, people, 2, 1, make_date("29-12-2005"), make_date("29-12-2005"));
    move_task(1, todo_list, doing_list, done_list, people, 2, 2, make_date("29-12-2005"), make_date("29-12-2005"));
    printf("$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    show_tasks(tasks_list);
    show_list(todo_list, 1);
    show_list(doing_list, 2);
    printf("CHANGING aznag TASK TO MOC\n");
    show_person_tasks(people, 3, done_list->size);
    change_doing_task_person(doing_list, people, 1, 3);
    show_list(doing_list, 2);
    move_task(2, todo_list, doing_list, done_list, people, 3, 4, make_date("29-12-2005"), make_date("29-12-2005"));
    show_person_tasks(people, 1, done_list->size);
    show_person_tasks(people, 3, done_list->size); */
    return 0;
}