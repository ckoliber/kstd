#include <dsa/high/LinkedList.h>
#include <dsa/low/String.h>
#include <memory/low/Type.h>

// vtable
typedef struct KRE_VTable {
    int (*match)(struct KRE* self, char* text, int count);
    int (*replace)(struct KRE* self, char* text, char* replace, int count);

    LinkedList* (*export)(struct KRE* self, char* text);
    String* (*import)(struct KRE* self, LinkedList* linkedlist);
} KRE_VTable;

// vtable + private data problem solve
typedef struct KRE {
    KRE_VTable* vtable;
} KRE;

// init vtable
void kre_init();

// new raw kre
KRE* kre_new();

// free raw kre
void kre_free(KRE* kre);

// new kre
KRE* kre_new_object(char* kregexp);