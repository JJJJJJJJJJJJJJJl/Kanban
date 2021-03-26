#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pdd_list.h"

//creates cards list (todo, doing, done)
list make_list(){
    list ward = (list)malloc(sizeof(list_node));

    if(ward != NULL){
        ward->size = 0; //to be changed later with file reading (!)
        ward->next = NULL;
    }
    return ward;
}

//finds node right before todo list split for insertation
void neighbours_todo_card(list todo_head, int priority_target, date genesis_target, list * prev, list * cur){
    * prev = todo_head;
    * cur = todo_head->next;
    
    //prev will be right before where linked list will get splitted (possibly. @↓↓)
    while(* cur != NULL && (*cur)->task_card->priority > priority_target){
        * prev = * cur;
        * cur = (* cur)->next; 
    }

    //this's for the case of multiple equal prioritys so it finds node right before the split by task genesis date
    while(* cur != NULL && date_cmp((* cur)->task_card->genesis, genesis_target) == -1){
        * prev = * cur;
        * cur = (* cur)->next; 
    }
    return;
}

//creates a task, gets added to both todo and tasks list
void add_task(list todo_head, task_list tasks_head, int id, int priority, char * description, int pos, date genesis){
    if(priority < 1 || priority > 10){
        printf("Invalid priority value.\n");
        return;
    }

    //adding to tasks_list
    task_list new_task = (task_list) malloc(sizeof(task));
    if(new_task != NULL){
        task_list prev;
        task_list cur;

        //setting task data
        new_task->id = id;
        new_task->priority = priority;
        new_task->description = description;
        new_task->pipeline_pos = 0;
        new_task->genesis = genesis;

        //setting task neighbours
        neighbours_task(tasks_head, genesis, &prev, &cur);
        new_task->next = cur;
        prev->next = new_task; 
    }

    //adding to todo_list
    list new_card = (list) malloc(sizeof(list_node));
    if(new_card != NULL){
        list prev;
        list cur;

        //settingcard data
        new_card->size = -1;
        new_card->task_card = new_task;

        //setting card neighbours
        neighbours_todo_card(todo_head, priority, genesis, &prev, &cur);
        new_card->next = cur;
        prev->next = new_card;
    }
    return;
}

//finds requested card by task id
void find_card(list list_head, int task_id_target, list * prev, list * cur){
    * prev = list_head;
    * cur = list_head->next;
    while(* cur != NULL && (* cur)->task_card->id != task_id_target){
        * prev = * cur;
        * cur = (* cur)->next;
    }
    return;
}

//finds node right before doing list split for insertation
void neighbours_doing_card(list doing_head, char * person_name_target, list * prev, list * cur){
    * prev = doing_head;
    * cur = doing_head->next;
    while(* cur != NULL && strcmp((* cur)->task_card->p->name, person_name_target) < 0){
        * prev = * cur;
        * cur = (* cur)->next;
    }
    return;
}

//finds node right before doing list split for insertation
void neighbours_done_card(list done_head, date end_target, list * prev, list * cur){
    * prev = done_head;
    * cur = done_head->next;
    while(* cur != NULL && date_cmp((* cur)->task_card->end, end_target) == 1){
        * prev = * cur;
        * cur = (* cur)->next;
    }
    return;
}

//moves task between lists
//flag == 1: Todo -> Doing
//flag == 2: Doing -> Done
//flag == 3: Done -> Todo
//flag == 4: Doing -> Todo
//pos == 0 -> Todo
//pos == 1 -> Doing
//pos == 2 -> Done
void move_task(int flag, list todo_head, list doing_head, list done_head, person_list people_head, 
                char * person_name_target, int task_id_target, date deadline, date end){

    list card = (list) malloc(sizeof(list_node));
    if(card == NULL){
        printf("Allocation went wrong.");
        return;
    }
    
    //pointers to find requested task
    list prev_card_to_move;
    list cur_card_to_move;

    if(flag == 1){
        if(todo_head->next == NULL){
            printf("Todo list is empty.\n");
            return;
        }

        person_list person_target;
        find_person(people_head, person_name_target, &person_target);

        if(person_target == NULL){
            printf("Person %s does not exist. (Create it before assinging task)\n", person_name_target);
            return;
        }
        if(doing_head->size > 4){
            printf("Doing list is full.\n");
            return;
        }

        //searching for card in todo list
        find_card(todo_head, task_id_target, &prev_card_to_move, &cur_card_to_move);
        //if task actually exists
        if(cur_card_to_move != NULL){
            list prev_doing_card;
            list cur_doing_card;
            neighbours_doing_card(doing_head, person_name_target, &prev_doing_card, &cur_doing_card);
            //adding card to doing list
            card->next = cur_doing_card;
            prev_doing_card->next = card;

            //incrementing doing list size
            doing_head->size++;

            //setting doing card task
            card->task_card = cur_card_to_move->task_card;
            card->task_card->pipeline_pos = 1;

            //assigning person to card
            card->task_card->p = person_target;  

            //removing card from todo list
            prev_card_to_move->next = cur_card_to_move->next;
            free(cur_card_to_move);
        }
        else{
            printf("Task %d does not exist.\n", task_id_target);
        }
    }
    
    else if(flag == 2){
        if(doing_head->size == 0){
            printf("Doing list is empty.\n");
            return;
        }

        //searching for card in doing list
        find_card(doing_head, task_id_target, &prev_card_to_move, &cur_card_to_move);

        if(cur_card_to_move != NULL){
            if(date_cmp(cur_card_to_move->task_card->genesis, end) == 1){
                printf("End date must come after genesis date.\n");
                return;
            }

            list prev_done_card;
            list cur_done_card;
            neighbours_done_card(done_head, end, &prev_done_card, &cur_done_card);

            done_head->size++;

            //adding card to done list
            card->next = cur_done_card;
            prev_done_card->next = card;

            //setting done card task
            card->task_card = cur_card_to_move->task_card;
            card->task_card->pipeline_pos = 2;

            //assigning end date to task card
            card->task_card->end = end;

            //removing card from doing list
            prev_card_to_move->next = cur_card_to_move->next;
            free(cur_card_to_move);
        }
        else{
            printf("Task %d does not exist.\n", task_id_target);
        }
    }

    else if(flag == 3){
        if(done_head->next == NULL){
            printf("Done list is empty.\n");
            return;
        }

        //searching for card in done list
        find_card(done_head, task_id_target, &prev_card_to_move, &cur_card_to_move);
        if(cur_card_to_move != NULL){
            list prev_todo_card;
            list cur_todo_card;
            neighbours_todo_card(todo_head, cur_card_to_move->task_card->priority, cur_card_to_move->task_card->genesis, &prev_todo_card, &cur_todo_card);

            done_head->size--;

            //adding card to todo list
            card->next = cur_todo_card;
            prev_todo_card->next = card;

            //setting done card task
            card->task_card = cur_card_to_move->task_card;
            card->task_card->pipeline_pos = 0;

            //removing card from done list
            prev_card_to_move->next = cur_card_to_move->next;
            free(cur_card_to_move);
        }
        else{
            printf("Task %d does not exist.\n", task_id_target);
        }
    }

    else if(flag == 4){
        if(doing_head->size == 0){
            printf("Doing list is empty.\n");
            return;
        }

        //searching for done in todo list
        find_card(doing_head, task_id_target, &prev_card_to_move, &cur_card_to_move);
        if(cur_card_to_move != NULL){
            list prev_todo_card;
            list cur_todo_card;
            neighbours_todo_card(todo_head, cur_card_to_move->task_card->priority, cur_card_to_move->task_card->genesis, &prev_todo_card, &cur_todo_card);
            //adding card to todo list
            card->next = cur_todo_card;
            prev_todo_card->next = card;

            //setting done card task
            card->task_card = cur_card_to_move->task_card;
            card->task_card->pipeline_pos = 0;

            //removing card from doing list
            prev_card_to_move->next = cur_card_to_move->next;
            free(cur_card_to_move);
        }
        else{
            printf("Task %d does not exist.\n", task_id_target);
        }
    }

    else{
        printf("%d is not a valid flag number.\n", flag);
    }
    return;
}

//changes person assigned to specific task
void change_doing_task_person(list doing_head, person_list people_head, int task_id_target, char * person_name_heir){
    person_list person_target;
    find_person(people_head, person_name_heir, &person_target);
    if(person_target != NULL){
        list prev_card_to_change;
        list cur_card_to_change;
        find_card(doing_head, task_id_target, &prev_card_to_change, &cur_card_to_change);
        if(cur_card_to_change != NULL){
            cur_card_to_change->task_card->p = person_target;
        }
        else{
            printf("Task %d does not exist.\n", task_id_target);
        }
    }
    else{
        printf("Person %s does not exist.\n", person_name_heir);
    }
    return;
}

//prints w/e list (todo, doing, done)
void show_list(list head, int flag){
    list cur = head->next;

    while(cur != NULL){
        if(flag == 1) printf("%s | ", cur->task_card->description);
        else printf("(%s,%s) ", cur->task_card->p->name, cur->task_card->description);
        cur = cur->next;
    }
    printf("\n");
    return;
}