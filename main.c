#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pdd_list.h"

int cur_person_id = 0;

char* itoa(int num, char* buffer, int base) {
    int curr = 0;
 
    if (num == 0) {
        // Base case
        buffer[curr++] = '0';
        buffer[curr] = '\0';
        return buffer;
    }
 
    int num_digits = 0;
 
    if (num < 0) {
        if (base == 10) {
            num_digits ++;
            buffer[curr] = '-';
            curr ++;
            // Make it positive and finally add the minus sign
            num *= -1;
        }
        else
            // Unsupported base. Return NULL
            return NULL;
    }
 
    num_digits += (int)floor(log(num) / log(base)) + 1;
 
    // Go through the digits one by one
    // from left to right
    while (curr < num_digits) {
        // Get the base value. For example, 10^2 = 1000, for the third digit
        int base_val = (int) pow(base, num_digits-1-curr);
 
        // Get the numerical value
        int num_val = num / base_val;
 
        char value = num_val + '0';
        buffer[curr] = value;
 
        curr ++;
        num -= base_val * num_val;
    }
    buffer[curr] = '\0';
    return buffer;
}

void save_people(person_list people_head){
    person_list cur = people_head->next;
    FILE * fp = fopen("people.txt", "w");

    while(cur != NULL){
        if(fp != NULL){
            fprintf(fp, "%d ", cur->id);
            fprintf(fp, "%s ", cur->name);
        }
        cur = cur->next;
    }
    fclose(fp);
}

void recover_people(person_list people_head){
    FILE * fp = fopen("people.txt", "r");
    int person_id;
    char person_name[20];
    while(fscanf(fp, "%d %s ", &person_id, person_name) == 2){
        printf("%d | %s\n", person_id, person_name);
        add_person(people_head, person_id, person_name);
        cur_person_id++;
    }
    fclose(fp);
}

int main(){
    //sake of testing functions
    task_list tasks_list = make_task_list();
    list todo_list = make_list();
    list doing_list = make_list();
    list done_list = make_list();
    person_list people = make_people_list();
    /* add_person(people, 1, "jjjjjj");
    add_person(people, 2, "mandelbrot");
    add_person(people, 3, "moc"); */
    recover_people(people);
    show_people(people);
    add_person(people, cur_person_id++, "carti");
    show_people(people);
    save_people(people);

    /* add_task(todo_list, tasks_list, 1, 10, "aznag", 0, make_date("29-12-2000"));
    add_task(todo_list, tasks_list, 2, 6, "aznag2", 0, make_date("29-12-2002"));
    add_task(todo_list, tasks_list, 3, 6, "aznag3", 0 ,make_date("29-12-2001"));
    add_task(todo_list, tasks_list, 4, 8, "aznag4", 0, make_date("29-12-2005"));
    show_tasks(tasks_list);
    printf("###############\n");
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