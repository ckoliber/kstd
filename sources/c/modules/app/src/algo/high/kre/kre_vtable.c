#include "kre_hipe.c"

// vtable
KRE_VTable* kre_vtable;

// link methods
bool kre_test(struct KRE* self, char* text);
ArrayList* kre_match(struct KRE* self, char* text, int count);
String* kre_replace(struct KRE* self, char* text, char* replace, int count);
ArrayList* kre_split(struct KRE* self, char* text);
String* kre_merge(struct KRE* self, ArrayList* data);
ArrayList* kre_export(struct KRE* self, char* text);
String* kre_import(struct KRE* self, ArrayList* data);

// local methods

// implement methods
// vtable operators
bool kre_test(struct KRE* self, char* text) {
}
ArrayList* kre_match(struct KRE* self, char* text, int count) {
}
String* kre_replace(struct KRE* self, char* text, char* replace, int count) {
}
ArrayList* kre_split(struct KRE* self, char* text) {
}
String* kre_merge(struct KRE* self, ArrayList* data) {
}
ArrayList* kre_export(struct KRE* self, char* text) {
}
String* kre_import(struct KRE* self, ArrayList* data) {
}

// object allocation and deallocation operators
void kre_init() {
    // init vtable
    kre_vtable = heap_alloc(sizeof(KRE_VTable));
    kre_vtable->test = kre_test;
    kre_vtable->match = kre_match;
    kre_vtable->replace = kre_replace;
    kre_vtable->split = kre_split;
    kre_vtable->merge = kre_merge;
    kre_vtable->export = kre_export;
    kre_vtable->import = kre_import;
}
KRE* kre_new() {
    struct KRE_* kre_ = heap_alloc(sizeof(struct KRE_));

    // set vtable
    kre_->self.vtable = kre_vtable;

    // set constructor data
    kre_->kregexp = NULL;

    // set private data
    kre_->graph_links = NULL;
    kre_->graph = NULL;

    return (KRE*)kre_;
}
void kre_free(KRE* kre) {
    struct KRE_* kre_ = (struct KRE_*)kre;

    // free private data
    if (kre_->graph_links != NULL) {
        arraylist_free(kre_->graph_links);
    }
    if (kre_->graph != NULL) {
        kre_hipe_free(kre_->graph);
    }

    // free constructor data
    if (kre_->kregexp != NULL) {
        string_free(kre_->kregexp);
    }

    // free self
    heap_free(kre_);
}
KRE* kre_new_object(char* kregexp) {
    struct KRE_* kre_ = (struct KRE_*)kre_new();

    // set constructor data
    kre_->kregexp = string_new_copy(kregexp);

    // set private data
    kre_->graph_links = arraylist_new_object(0, 2, NULL);
    kre_->graph = kre_hipe_compile((KRE*)kre_);

    return (KRE*)kre_;
}