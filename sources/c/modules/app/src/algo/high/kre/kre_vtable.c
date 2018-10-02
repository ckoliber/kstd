#include "kre_compile.c"
#include "kre_refactor.c"

struct KREStackItem {
    struct KREItem* item;
    int value;
};

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
int find_kre_match_end(struct KRE* self, String* text, int begin);

// implement methods
int find_kre_match_end(struct KRE* self, String* text, int begin) {
    /*
        Algorithm:
            1. force_add = false;
            2. init stack graph
            3. init stack recorder
            3. push KREStackItem{head, 0}
            4. while(stack->size > 0)
                1. pop sitem
                2. if(sitem->item->type == MATCH)
                    1. if(sitem->value range sitem->item && !force)
                        1. push sitem
                        2. if(sitem->item->next != null)
                            1. push sitem->item->next
                        3. else
                            1. while((sitem_group = pop) != GROUP);
                            2. if(sitem_group != NULL)
                                1. push sitem_group->next  
                    2. else if(text[cursor] match sitem->item && sitem->value + 1 range sitem->item)
                        1. cursor++;
                        2. sitem->value++;
                        3. push sitem
                        4. force_add = false;
                    3. else
                        1. cursor -= sitem->value;
                        2. force_add = true;
                3. else if(sitme->item->type == GROUP)
                    1. if(sitem->value range items)
                        1. sitem->value++;
                        2. push sitem
                        3. push sitem->get(sitem->value - 1)
                



    */
    Dequeue* stack = dequeue_new(0);
}

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
    kre_->graph = kre_graph_compile((KRE*)kre_);
    kre_->graph = kre_graph_refactor(kre_->graph);

    return (KRE*)kre_;
}