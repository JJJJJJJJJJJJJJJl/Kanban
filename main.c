#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pdd_list.h"

int cur_person_id = 1;

void save_people(person_list people_head){
    person_list cur = people_head->next;
    FILE * fp = fopen("people.txt", "w");

    while(cur != NULL){
        if(fp != NULL){
            printf("%s\n", cur->name);
            fwrite(&(cur->id), sizeof(int), 1, fp);
            fwrite(cur->name, sizeof(char), strlen(cur->name) + 1, fp);
        }
        cur = cur->next;
    }
    fclose(fp);
}

void recover_people(person_list people_head){
    FILE * fp = fopen("people.dat", "rb");
    person p;

    while(1){
        fread(&(p.id), sizeof(int), 1, fp);
        fread(p.name, 1, sizeof(char)*70, fp);
        if(feof(fp)) {
            break;
        }
        printf("%d | %s\n", p.id, p.name);
        add_person(people_head, p.id, p.name);
        printf("ASIHDB22222\n");
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
    add_person(people, 1, "jjjjjj");
    add_person(people, 2, "mandelbrot");
    add_person(people, 3, "moc");
    /* recover_people(people);
    show_people(people);
    add_person(people, cur_person_id++, "carti");
    show_people(people); */
    //save_people(people);

    add_task(todo_list, tasks_list, 1, 10, "aznag", 0, make_date("29-12-2000"));
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
    show_person_tasks(people, 3, done_list->size);
    return 0;
}