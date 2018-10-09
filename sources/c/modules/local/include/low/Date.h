#include <low/Type.h>

// vtable
typedef struct Date_VTable {
    int (*get_year)(struct Date* self);
    int (*get_month)(struct Date* self);
    int (*get_day)(struct Date* self);
    int (*get_hours)(struct Date* self);
    int (*get_minutes)(struct Date* self);
    int (*get_seconds)(struct Date* self);
    int (*get_millis)(struct Date* self);

    int (*get_week_day)(struct Date* self);
    uint_64 (*get_epoch)(struct Date* self);

    uint_64 (*get_epoch)(struct Date* self);

    String* (*get_string)(struct Date* self);

} Date_VTable;

// vtable + private data problem solve
typedef struct Date {
    Date_VTable* vtable;
} Date;

// init vtable
void date_init();

// new raw date
Date* date_new();

// free raw date
void date_free(Date* date);

// new time
Date* date_new_object();
Date* date_new_epoch(uint_64 millis);
Date* date_new_string(char* date);
Date* date_new_date(int year, int month, int day);
Date* date_new_datetime(int hours, int minutes, int seconds, int millis);

// static
uint_64 date_get_epoch();