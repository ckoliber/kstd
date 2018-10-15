#include <memory/low/Type.h>

// structs
typedef struct KRE KRE;
typedef struct KRE_VTable KRE_VTable;

// implement structs
// vtable + private data problem solve
struct KRE {
    KRE_VTable* vtable;
};

// vtable
struct KRE_VTable {
    bool (*test)(KRE* self, char* text);

    struct ArrayList* (*match)(KRE* self, char* text);
    struct String* (*replace)(KRE* self, char* text, char* replace);

    struct ArrayList* (*split)(KRE* self, char* text);
    struct String* (*merge)(KRE* self, struct ArrayList* data);

    struct ArrayList* (*export)(KRE* self, char* text);
    struct String* (*import)(KRE* self, struct ArrayList* data);
};

// init vtable
void kre_init();

// new raw kre
KRE* kre_new();

// free raw kre
void kre_free(KRE* kre);

// new kre
KRE* kre_new_object(char* kregexp);