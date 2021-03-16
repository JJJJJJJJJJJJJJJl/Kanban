#ifndef DATE_H_INCLUDED
#define DATE_H_INCLUDED

typedef struct date{
    int d;
    int m;
    int y;
}date;

date make_date(char *);
int date_cmp(date, date);

#endif