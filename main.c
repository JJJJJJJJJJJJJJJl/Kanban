#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

        //cleaning stream to buffer so it doesnt mess scanf up
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

int op_valid_input(int flag){
    char * message;
    if(flag == 2 || flag == 7){
        message = "DIA";
    }
    else if(flag == 3 || flag == 8){
        message = "MÊS";
    }
    else if(flag == 4 || flag == 9){
        message = "ANO";
    }

    //pending value
    int valid_value;

    int valid_input = 0;
    while(valid_input == 0){
        if(flag == 0){
            printf("ID da tarefa a mover: ");
        }
        else if(flag == 1){ 
            printf("ID da pessoa: ");
        }
        else if(flag == 2 || flag == 3 || flag == 4 || flag == 7 || flag == 8 || flag == 9){
            if(flag < 5 && flag > 1){
                printf("%s do prazo de conclusão da tarefa: ", message);
            }
            else{
                printf("%s da data de conclusão da tarefa: ", message);
            }
        }
        else if(flag == 5){
            printf("ID da tarefa em mudança: ");
        }
        else if(flag == 6){
            printf("ID da nova pessoa responsável: ");
        }
        else if(flag == 420){
            printf("\nOperação: ");
        }
        valid_input = scanf("%d", &valid_value);

        //cleaning stream to buffer so it doesnt mess scanf up
        while ((getchar()) != '\n');
    }

    return valid_value;
}

void op_2(list todo_head, list doing_head, person_list people_head){
    int task_id_target = op_valid_input(0);
    int person_id_target = op_valid_input(1);
    date deadline;

    deadline.d = op_valid_input(2);
    deadline.m = op_valid_input(3);
    deadline.y = op_valid_input(4);
    //                                  mandatory                                                           mandatory
    //                                  so anything                                                         aswell
    //                                  works
    move_task(1, todo_head, doing_head, doing_head, people_head, person_id_target, task_id_target, deadline, make_date("0-0-0"));

    return;
}

void op_3(list todo_head, list doing_head, person_list people_head){
    int task_id_target = op_valid_input(0);

    move_task(4, todo_head, doing_head, doing_head, people_head, -1, task_id_target, make_date("0-0-0"), make_date("0-0-0"));
    
    return;
}

void op_4(list doing_head, list done_head, person_list people_head){
    int task_id_target = op_valid_input(0);
    date end;

    end.d = op_valid_input(7);
    end.m = op_valid_input(8);
    end.y = op_valid_input(9);

    move_task(2, doing_head, doing_head, done_head, people_head, -1, task_id_target, make_date("0-0-0"), end);

    return;
}

void op_5(list todo_head, list done_head, person_list people_head){
    int task_id_target = op_valid_input(0);

    move_task(3, todo_head, todo_head, done_head, people_head, -1, task_id_target, make_date("0-0-0"), make_date("0-0-0"));

    return;
}

void op_6(list doing_head, person_list people_head){
    int task_id_target = op_valid_input(5);
    int person_id_heir_target = op_valid_input(6);

    change_doing_task_person(doing_head, people_head, task_id_target, person_id_heir_target);

    return;
}

void op_7(list todo_head, list doing_head, list done_head){
    printf("Todo: "); show_list(todo_head, 1);
    printf("Doing: "); show_list(doing_head, 2);
    printf("Done: "); show_list(done_head, 2);

    return;
}

void op_8(person_list people_head){
    int person_id_target = op_valid_input(1);

    show_person_tasks(people_head, person_id_target);

    return;
}

void op_9(task_list tasks_head){
    show_tasks(tasks_head);

    return;
}

void op_10(person_list people_head){
    show_people(people_head);

    return;
}

void op_11(list doing_head, list done_head, task_list tasks_head, person_list people_head){
    save_data(doing_head, done_head, tasks_head, people_head);
    printf("Dados guardados.\n");

    return;
}

void print_menu(){
    printf("+++++++++++++++++++++++++++++++++++++++++++\n");
    printf("+  Operações                              +\n");
    printf("+                                         +\n");
    printf("+  1 - Criar tarefa                       +\n");
    printf("+  2 - Enviar tarefa de Todo para Doing   +\n");
    printf("+  3 - Enviar tarefa de Doing para Todo   +\n");
    printf("+  4 - Enviar tarefa de Doing para Done   +\n");
    printf("+  5 - Enviar tarefa de Done para Todo    +\n");
    printf("+  6 - Alterar pessoa em tarefa de Doing  +\n");
    printf("+  7 - Visualizar quadro de tarefas       +\n");
    printf("+  8 - Visualizar tarefas de X pessoa     +\n");
    printf("+  9 - Visualizar todas as tarefas        +\n");
    printf("+  10 - Visualizar pessoas                +\n");
    printf("+  11 - Guardar estado currente           +\n");
    printf("+  12 - Visualizar menu de operações      +\n");
    printf("+  0 - Sair (Alterações serão guardadas)  +\n");
    printf("+                                         +\n");
    printf("+++++++++++++++++++++++++++++++++++++++++++\n");
    
    return;
}

int main(){
    task_list tasks_list = make_task_list();
    list todo_list = make_list();
    list doing_list = make_list();
    list done_list = make_list();
    person_list people_list = make_people_list();

    //recovering data state
    recover_data(todo_list, doing_list, done_list, tasks_list, people_list);

    //introduction
    printf("\n◔ ◔ Kanban ◉ ◉\n\n");

    //printing menu
    print_menu();

    while(1){
        int op = op_valid_input(420);
        printf("\n");

         //create task
        if(op == 1){
            op_1(todo_list, tasks_list);
        }
        //send card from todo to doing
        else if(op == 2){
            op_2(todo_list, doing_list, people_list);
        }
        //send card from doing to todo
        else if(op == 3){
            op_3(todo_list, doing_list, people_list);
        }
        //send card from doing to done
        else if(op == 4){
            op_4(doing_list, done_list, people_list);
        }
        //send card from done to todo
        else if(op == 5){
            op_5(todo_list, done_list, people_list);
        }
        //change person assigned to task
        else if(op == 6){
            op_6(doing_list, people_list);
        }
        //visualize board
        else if(op == 7){
            op_7(todo_list, doing_list, done_list);
        }
        //visualize specific person tasks
        else if(op == 8){
            op_8(people_list);
        }
        //visualize all tasks
        else if(op == 9){
            op_9(tasks_list);
        }
        //visualize people
        else if(op == 10){
            op_10(people_list);
        }
        //save current state
        else if(op == 11){
            op_11(doing_list, done_list, tasks_list, people_list);
        }
        else if(op == 12){
            print_menu();
        }
        //exit program
        else if(op == 0){
            printf("Bye ^-^\n");
            break;
        }
        else{
            printf("Hmm, nope! ರ_ರ\n");
        }
    }
    
    //saving data state
    save_data(doing_list, done_list, tasks_list, people_list);
    
    return 0;
}