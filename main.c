#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
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
            fprintf(fp, "%d %s\n", cur->id, cur->name);
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
        while(fscanf(fp, "%d %[^\n]\n*c", &person_id, person_name) == 2){
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
            fprintf(fp, "%d %d %d %d %d %s\n", cur->id, cur->priority, cur->genesis.d, cur->genesis.m, cur->genesis.y, cur->description);
            cur = cur->next;
        }
    }
    fclose(fp);
    return;
}

void recover_tasks_data(list todo_head, task_list tasks_head){
    FILE * fp = fopen("tasks.txt", "r");
    if(fp != NULL){
        int task_id;
        int task_priority;
        char task_description[50];
        date genesis;
        while(fscanf(fp, "%d %d %d %d %d %[^\n]\n*c", &task_id, &task_priority, &genesis.d, &genesis.m, &genesis.y, task_description) == 6){
            add_task(todo_head, tasks_head, task_id, task_priority, task_description, genesis);
            cur_task_id++;
        }
    }
    fclose(fp);
    return;
}


//#####################################################################
//DOING LIST DATA HANDLING

void save_doing_list_data(list doing_head){
    list cur = doing_head->next;
    FILE * fp = fopen("doing_list.txt", "w");
    if(fp != NULL){
        while(cur != NULL){
            fprintf(fp, "%d %d %d %d %d ", cur->task_card->id, cur->task_card->p->id, cur->task_card->deadline.d, cur->task_card->deadline.m, cur->task_card->deadline.y);
            cur = cur->next;
        }
    }
    fclose(fp);
    return;
}

void recover_doing_list_data(list todo_head, list doing_head, person_list people_head){
    FILE * fp = fopen("doing_list.txt", "r");
    if(fp != NULL){
        int task_id;
        int person_id;
        date deadline;
        while(fscanf(fp, "%d %d %d %d %d ", &task_id, &person_id, &deadline.d, &deadline.m, &deadline.y) == 5){
            //                                  useless                                               useless  (just cuz move_task receives this parameters)
            move_task(1, todo_head, doing_head, doing_head, people_head, person_id, task_id, deadline, deadline);
        }
    }
    fclose(fp);
    return;
}

//#####################################################################
//DONE LIST DATA HANDLING

void save_done_list_data(list done_head){
    list cur = done_head->next;
    FILE * fp = fopen("done_list.txt", "w");
    if(fp != NULL){
        while(cur != NULL){
            fprintf(fp, "%d %d %d %d %d ", cur->task_card->id, cur->task_card->p->id, cur->task_card->end.d, cur->task_card->end.m, cur->task_card->end.y);
            cur = cur->next;
        }
    }
    fclose(fp);
    return;
}

void recover_done_list_data(list todo_head, list doing_head, list done_head, person_list people_head){
    FILE * fp = fopen("done_list.txt", "r");
    if(fp != NULL){
        int task_id;
        int person_id;
        date end;
        while(fscanf(fp, "%d %d %d %d %d ", &task_id, &person_id, &end.d, &end.m, &end.y) == 5){
            //same thing
            move_task(1, todo_head, doing_head, done_head, people_head, person_id, task_id, make_date("0-0-9999999"), end);
            move_task(2, todo_head, doing_head, done_head, people_head, person_id, task_id, make_date("0-0-0"), end);
        }
    }
    fclose(fp);
    return;
} 

//saves all data
void save_data(list doing_head, list done_head, task_list tasks_head, person_list people_head){
    save_people_data(people_head);
    save_tasks_data(tasks_head);
    save_doing_list_data(doing_head);
    save_done_list_data(done_head);

    return;
}

//recovers all data
void recover_data(list todo_head, list doing_head, list done_head, task_list tasks_head, person_list people_head){
    recover_people_data(people_head);
    recover_tasks_data(todo_head, tasks_head);
    recover_done_list_data(todo_head, doing_head, done_head, people_head);
    recover_doing_list_data(todo_head, doing_head, people_head);

    return;
}

void op_1(list todo_head, task_list tasks_head){
    int task_priority = 0;
    char task_description_buffer[50];
    date genesis;

    //keeps asking for input while its not a number and its outside [1..10] range 
    int valid_input = 0;
    while(valid_input == 0 || task_priority < 1 || task_priority > 10){
        printf("Enter task priority: ");
        valid_input = scanf("%d", &task_priority);

        //cleaning stream to buffer so it doesnt mess up scanf
        while ((getchar()) != '\n');
    }

    //task description input, could be anything
    printf("Enter task description: ");
    fgets(task_description_buffer, 50, stdin);
    task_description_buffer[strlen(task_description_buffer)-1] = '\0';
    char * task_description = strdup(task_description_buffer);

    //task genesis date set to system current time 
    time_t rawtime;
    struct tm * info;
    time(&rawtime);
    info = localtime(&rawtime);
    genesis.d = info->tm_mday;
    genesis.m = info->tm_mon+1;// +1 cuz tm_mon goes from 0 to 11
    genesis.y = info->tm_year+1900;// +1900 cuz tm_year returns number of years since 1900 
    add_task(todo_head, tasks_head, cur_task_id++, task_priority, task_description, genesis);

    return;
}

int op_2_valid_input(int flag){
    char * message;
    if(flag == 2){
        message = "DAY";
    }
    else if(flag == 3){
        message = "MONTH";
    }
    else if(flag == 4){
        message = "YEAR";
    }
    int valid_value;

    int valid_input = 0;
    while(valid_input == 0){
        if(flag == 0) printf("Enter moving task id: ");
        else if(flag == 1) printf("Enter assigned person id : ");
        else printf("Enter task deadline %s: ", message);
        valid_input = scanf("%d", &valid_value);

        //cleaning stream to buffer so it doesnt mess up scanf
        while ((getchar()) != '\n');
    }
    return valid_value;
}

void op_2(list todo_head, list doing_head, person_list people_head){
    int task_id_target = op_2_valid_input(0);
    int person_id_target = op_2_valid_input(1);
    date deadline;

    deadline.d = op_2_valid_input(2);
    deadline.m = op_2_valid_input(3);
    deadline.y = op_2_valid_input(4);
    //                                  mandatory                                                           mandatory
    //                                  so anything                                                         aswell
    //                                  works
    move_task(1, todo_head, doing_head, doing_head, people_head, person_id_target, task_id_target, deadline, make_date("0-0-0"));

    return;
}

int main(){
    task_list tasks_list = make_task_list();
    list todo_list = make_list();
    list doing_list = make_list();
    list done_list = make_list();
    person_list people_list = make_people_list();
    /* add_person(people, 1, "jjjjjj");
    add_person(people, 2, "mandelbrot marin");
    add_person(people, 3, "mocristino morais"); */
    recover_data(todo_list, doing_list, done_list, tasks_list, people_list);
    /* add_task(todo_list, tasks_list, 1, 10, "paint the wall", make_date("29-12-2000"));
    add_task(todo_list, tasks_list, 2, 6, "purchase 7top", make_date("29-12-2002"));
    add_task(todo_list, tasks_list, 3, 6, "read david goggins", make_date("29-12-2001"));
    add_task(todo_list, tasks_list, 4, 8, "follow the rules", make_date("29-12-2005")); */
    //add_task(todo_list, tasks_list, cur_task_id++, 10, "free time", make_date("29-12-2070"));

    while(1){
        int op;
        int valid_operation_input = 0;
        while(valid_operation_input == 0){
            printf("Operation: ");
            valid_operation_input = scanf("%d", &op);
            while ((getchar()) != '\n');
        }

        //create task
        if(op == 1){
            op_1(todo_list, tasks_list);
        }
        //send card from todo to doing
        else if(op == 2){
            op_2(todo_list, doing_list, people_list);
        }
        //exit program
        else if(op == 0){
            printf("Bye ^-^\n");
            break;
        }
        else{
            printf("suspicious move my guy\n");
        }

        show_people(people_list);
        show_tasks(tasks_list);
        show_list(todo_list, 1);
        show_list(doing_list,2);
        show_list(done_list, 1);
        printf("###########\n");
    }
    
    save_data(doing_list, done_list, tasks_list, people_list);
    
    return 0;
}