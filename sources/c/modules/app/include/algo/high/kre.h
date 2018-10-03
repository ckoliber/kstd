#include <memory/low/Type.h>

// vtable
typedef struct KRE_VTable {
    bool (*test)(struct KRE* self, char* text);

    struct ArrayList* (*match)(struct KRE* self, char* text, int count);
    struct String* (*replace)(struct KRE* self, char* text, char* replace, int count);

    struct ArrayList* (*split)(struct KRE* self, char* text);
    struct String* (*merge)(struct KRE* self, struct ArrayList* data);

    struct ArrayList* (*export)(struct KRE* self, char* text);
    struct String* (*import)(struct KRE* self, struct ArrayList* data);
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