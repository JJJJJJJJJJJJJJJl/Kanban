#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct date{
    int d;
    int m;
    int y;
}date;

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
    person * p;
    date genesis;
    date deadline;
    date end;
    int done;//Printing a person tasks purposes only
    task_list next;
}task;

typedef struct list_node * list; 
typedef struct list_node{
    int size; //Doing(5) list purposes only
    task_list task_card;
    person_list p;//Doing(5) list purposes only
    list next;
}list_node;

//creates date
date make_date(char * datee){
    //DATE: DD-MM-YYYY @_@
    int date_nums[3];
    int in = 0;
    char * date1 = strdup(datee);
    char * token = strtok(date1, "-");
    while(token != NULL){
        date_nums[in++] = atoi(token);
        token = strtok(NULL, "-");
    }
    date d = {date_nums[0], date_nums[1], date_nums[2]};
    return d;
}

//compares dates (0 if equal @ 1 if first date latter than second @ -1 if first date older than second)
int date_cmp(date d1, date d2){
    if(d1.y == d2.y){
        if(d1.m == d2.m){
            if(d1.d == d2.d)
                return 0;
            else if(d1.d > d2.d)
                return 1;
            else return -1;
        }
        else if(d1.m > d2.m)
            return 1;
        else return -1;
    }
    else if(d1.y > d2.y)
        return 1;
    return -1;
}

//creates people list
person_list make_person_list(){
    person_list ward = (person_list) malloc(sizeof(person));

    if(ward != NULL){
        ward->next = NULL;
    }
    return ward;
}

//traverses all people list (insertation purposes)
void find_last_person(person_list people_head, person_list * cur){
    * cur = people_head;
    while((* cur)->next != NULL){
        * cur = (* cur)->next;
    }
    return;
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

//creates cards list (todo, doing, done)
list make_list(){
    list ward = (list)malloc(sizeof(list_node));

    if(ward != NULL){
        ward->size = 0; //to be changed later with file reading (!)
        ward->next = NULL;
    }
    return ward;
}

//creates tasks list
task_list make_task_list(){
    task_list ward = (task_list)malloc(sizeof(task));

    if(ward != NULL){
        ward->next = NULL;
    }   
    return ward;
}

//finds nnode right before tasks list split for insertation
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
void add_task(list todo_head, task_list tasks_head, int id, int priority, char * description, date genesis, date end){
    if(priority < 1 || priority > 10){
        printf("Invalid priority value.\n");
        return;
    }

    //adds to tasks_list
    task_list new_task = (task_list) malloc(sizeof(task));
    if(new_task != NULL){
        task_list prev;
        task_list cur;

        //sets task data
        new_task->id = id;
        new_task->priority = priority;
        new_task->description = description;
        new_task->genesis = genesis;
        new_task->end = end;

        //sets task neighbours
        neighbours_task(tasks_head, genesis, &prev, &cur);
        new_task->next = cur;
        prev->next = new_task; 
    }

    //adds to todo_list
    list new_card = (list) malloc(sizeof(list_node));
    if(new_card != NULL){
        list prev;
        list cur;

        //sets card data
        new_card->size = -1;
        new_card->task_card = new_task;

        //sets card neighbours
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

/* void remove_task_todo(list todo_head, int task_id_target){
    list prev;
    list cur;
    find_task(todo_head, task_id_target, &prev, &cur);                                //MAYBE find_todo_task(L.211) and neighbours_doing_task(L.237)
    if(cur == NULL){                                                                       //DO THE SAME THING BUT..NOT SURE IF THEY SHOULD BE MERGED..HM 
        printf("Task does not exist");
    }
    else{
        list temp = cur;
        prev->next = cur->next;
        free(temp);
    }
    return;
} */

//finds node right before doing list split for insertation
void neighbours_doing_card(list doing_head, char * person_name_target, list * prev, list * cur){
    * prev = doing_head;
    * cur = doing_head->next;
    while(* cur != NULL && strcmp((* cur)->p->name, person_name_target) < 0){
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


//finds person to assign task
void find_person(person_list people_head, char * person_name_target, person_list * cur){
    * cur = people_head->next;
    while(* cur != NULL && strcmp((* cur)->name, person_name_target) != 0){
        * cur = (* cur)->next;
    }
    return;
}

//finds person task for insertation
void find_person_task_spot(task_list tasks_head, int done_target, task_list * prev, task_list * cur){
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

//moves task between lists
//flag == 1: Todo -> Doing
//flag == 2: Doing -> Done
//flag == 3: Done -> Todo
//flag == 4: Doing -> Todo
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

            //assigning person to card
            card->p = person_target;  

            //adding task to person tasks
            task_list new_person_task = (task_list) malloc(sizeof(task));
            if(new_person_task != NULL){
                new_person_task = cur_card_to_move->task_card;
                task_list last_person_task_prev;
                task_list last_person_task_cur;//this aint useless
                find_person_task_spot(person_target->tasks, new_person_task->done, &last_person_task_prev, &last_person_task_cur);
                last_person_task_prev->next = new_person_task;
                new_person_task->next = last_person_task_cur;

                //removing card from todo list
                prev_card_to_move->next = cur_card_to_move->next;
                free(cur_card_to_move);
            }
            else{
                printf("Allocation on %s tasks went wrong.\n", person_name_target);
            }
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
            list prev_done_card;
            list cur_done_card;
            neighbours_done_card(done_head, end, &prev_done_card, &cur_done_card);
            //adding card to done list
            card->next = cur_done_card;
            prev_done_card->next = card;

            //setting done card task
            card->task_card = cur_card_to_move->task_card;

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

        //searching for done in todo list
        find_card(done_head, task_id_target, &prev_card_to_move, &cur_card_to_move);
        if(cur_card_to_move != NULL){
            list prev_todo_card;
            list cur_todo_card;
            neighbours_todo_card(todo_head, cur_card_to_move->task_card->priority, cur_card_to_move->task_card->genesis, &prev_todo_card, &cur_todo_card);
            //adding card to todo list
            card->next = cur_todo_card;
            prev_todo_card->next = card;

            //setting done card task
            card->task_card = cur_card_to_move->task_card;

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

void change_doing_task_person(list doing_head, person_list people_head, int task_id_target, char * person_name_heir){
    person_list person_target;
    find_person(people_head, person_name_heir, &person_target);
    if(person_target != NULL){
        list prev_card_to_change;
        list cur_card_to_change;
        find_card(doing_head, task_id_target, &prev_card_to_change, &cur_card_to_change);
        if(cur_card_to_change != NULL){
            cur_card_to_change->p = person_target;
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

void show_people(person_list people_head){
    person_list cur = people_head->next;
    while(cur != NULL){
        printf("%s | ", cur->name);
        cur = cur->next;
    }
    printf("\n");
    return;
}

void show_tasks(task_list tasks_head){
    task_list cur = tasks_head->next;

    while(cur != NULL){
        printf("%s | ", cur->description);
        cur = cur->next;
    }
    printf("\n");
    return;
}

void show_list(list head, int flag){
    list cur = head->next;

    while(cur != NULL){
        if(flag == 1) printf("%s | ", cur->task_card->description);
        else printf("(%s,%s) ", cur->p->name, cur->task_card->description);
        cur = cur->next;
    }
    printf("\n");
    return;
}

void show_person_tasks(person_list people_head, char * person_name_target){
    person_list cur;
    int bz = 0;//this is just so "Done: " only gets printed once, i mean, can you find a better name for this?
    find_person(people_head, person_name_target, &cur);
    if(cur != NULL){
        task_list person_tasks = cur->tasks->next;
        printf("%s tasks: ", person_name_target);
        printf("\n\t\tDoing: ");
        while(person_tasks != NULL){
            printf("%s | ", person_tasks->description);
            if(person_tasks->done == 1 && bz == 0){
                printf("\n\t\tDone: ");
                bz = 1;
            }
            person_tasks = person_tasks->next;
        }
        if(bz == 0){
            printf("\n\t\tDone: ");
        }
        printf("\n");
    }
    else{
        printf("Person %s does not exist.\n", person_name_target);
    }
    return;
}

int main(){
    task_list tasks_list = make_task_list();
    list todo_list = make_list();
    list doing_list = make_list();
    list done_list = make_list();
    person_list people = make_person_list();
    add_person(people, 1, "jjjjjj");
    add_person(people, 2, "mandelbrot");
    add_person(people, 3, "moc");
    show_people(people);
    add_task(todo_list, tasks_list, 1, 10, "ganza", make_date("29-12-2000"), make_date("29-12-2000"));
    add_task(todo_list, tasks_list, 2, 6, "ganza2", make_date("29-12-2002"), make_date("29-12-2001"));
    add_task(todo_list, tasks_list, 3, 6, "ganza3", make_date("29-12-2001"), make_date("29-12-2001"));
    add_task(todo_list, tasks_list, 4, 8, "ganza4", make_date("29-12-2005"), make_date("29-12-2005"));
    show_tasks(tasks_list);
    printf("###############\n");
    //remove_task_todo(todo_list, 3);
    show_list(todo_list, 1);
    show_list(doing_list, 2);
    show_list(done_list, 1);
    printf("AFTER MOVING GANZAs\n");
    move_task(1, todo_list, doing_list, done_list, people, "mandelbrot", 3, make_date("29-12-2001"), make_date("29-12-2001"));
    move_task(1, todo_list, doing_list, done_list, people, "moc", 1, make_date("29-12-2000"), make_date("29-12-2000"));
    move_task(1, todo_list, doing_list, done_list, people, "jjjjjj", 4, make_date("29-12-2005"), make_date("29-12-2005"));
    show_list(todo_list, 1);
    show_list(doing_list, 2);
    show_person_tasks(people, "jjjjjj");
    show_person_tasks(people, "moc");
    move_task(2, todo_list, doing_list, done_list, people, "mandelbrot", 3, make_date("29-12-2001"), make_date("29-12-2001"));
    move_task(2, todo_list, doing_list, done_list, people, "mandelbrot", 4, make_date("29-12-2005"), make_date("29-12-2005"));
    printf("############################\n");
    show_list(todo_list, 1);
    show_list(doing_list, 1);
    show_list(done_list, 1);
    printf("MOVING FROM DONE TO TODO\n");
    move_task(3, todo_list, doing_list, done_list, people, "mandelbrot", 4, make_date("29-12-2005"), make_date("29-12-2005"));
    show_list(todo_list, 1);
    show_list(doing_list, 1);
    show_list(done_list, 1);
    printf("MOVING FROM DOING TO TODO\n");
    move_task(4, todo_list, doing_list, done_list, people, "mandelbrot", 1, make_date("29-12-2005"), make_date("29-12-2005"));
    show_list(todo_list, 1);
    show_list(doing_list, 2);
    move_task(1, todo_list, doing_list, done_list, people, "mandelbrot", 1, make_date("29-12-2005"), make_date("29-12-2005"));
    move_task(1, todo_list, doing_list, done_list, people, "mandelbrot", 2, make_date("29-12-2005"), make_date("29-12-2005"));
    printf("$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    show_list(todo_list, 2);
    show_list(doing_list, 2);
    printf("CHANGING GANZA TASK TO MOC\n");
    change_doing_task_person(doing_list, people, 1, "moc");
    show_list(doing_list, 2);
    //testing some git commands
    return 0;
}