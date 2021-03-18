#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pdd_list.h"

void save_people(person_list people_head){
    person_list cur = people_head->next;
    FILE * fp = fopen("people.txt", "wb");

    while(cur != NULL){
        if(fp != NULL){
            printf("%s\n", cur->name);
            fwrite(&(* cur), sizeof(person), 1, fp);
        }
        cur = cur->next;
    }
    fclose(fp);
}

void recover_people(person_list people_head){
    FILE * fp = fopen("people.txt", "rb");

    while(1){
        person_list p = (person_list) malloc(sizeof(person));
        fread(p, sizeof(person), 1, fp);
        printf("%s\n", p->name);
        add_person(people_head, p->id, p->name);
        if(feof(fp)) {
            break;
        }
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
    printf("xD\n");
    show_people(people);
    add_person(people, 4, "carti"); */
    show_people(people);
    save_people(people);

    add_task(todo_list, tasks_list, 1, 10, "aznag", make_date("29-12-2000"));
    add_task(todo_list, tasks_list, 2, 6, "aznag2", make_date("29-12-2002"));
    add_task(todo_list, tasks_list, 3, 6, "aznag3", make_date("29-12-2001"));
    add_task(todo_list, tasks_list, 4, 8, "aznag4", make_date("29-12-2005"));
    show_tasks(tasks_list);
    printf("###############\n");
    //remove_task_todo(todo_list, 3);
    show_list(todo_list, 1);
    show_list(doing_list, 2);
    show_list(done_list, 1);
    printf("AFTER MOVING aznags\n");
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
    printf("CHANGING aznag TASK TO MOC\n");
    change_doing_task_person(doing_list, people, 1, "moc");
    show_list(doing_list, 2);
    return 0;
}