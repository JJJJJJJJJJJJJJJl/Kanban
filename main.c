#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pdd_list.h"

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
    show_people(people);
    add_task(todo_list, tasks_list, 1, 10, "ganza", make_date("29-12-2000"));
    add_task(todo_list, tasks_list, 2, 6, "ganza2", make_date("29-12-2002"));
    add_task(todo_list, tasks_list, 3, 6, "ganza3", make_date("29-12-2001"));
    add_task(todo_list, tasks_list, 4, 8, "ganza4", make_date("29-12-2005"));
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
    return 0;
}