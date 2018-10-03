#include <memory/low/Type.h>

// vtable
typedef struct FSM_VTable {
} FSM_VTable;

// vtable + private data problem solve
typedef struct FSM {
    FSM_VTable* vtable;
} FSM;

// init vtable
void fsm_init();

// new raw fsm
FSM* fsm_new();

// free raw fsm
void fsm_free(FSM* fsm);

// new fsm
FSM* fsm_new_object();