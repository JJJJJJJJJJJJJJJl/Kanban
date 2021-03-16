#include <stdlib.h>
#include <string.h>
#include "date.h"

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