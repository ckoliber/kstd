#include <algo/high/kre.h>

#include <dsa/high/ArrayList.h>
#include <dsa/high/Dequeue.h>
#include <memory/low/Heap.h>

struct KRE_ {
    // self public object
    KRE self;

    // constructor data
    String* kregexp;

    // private data
    ArrayList* graph_links;
    struct KREItem* graph;
};

struct KREItem {
    // item next pointer
    struct KREItem* next;

    // item type
    enum KREItemType {
        KRE_ITEM_MATCH,
        KRE_ITEM_GROUP
    } type;

    // item value (match, group)
    union KREItemValue {
        struct KREItemValueMatch {
            String* match;
            int start;
            int stop;
        } match;
        struct KREItemValueGroup {
            ArrayList* items;
            ArrayList* links;
            int mode;
            int start;
            int stop;
        } group;
    } value;
};

// vtable
KRE_VTable* kre_vtable;

// link methods

// local methods
// kre compiler methods
struct KREItem* kre_graph_new(KRE* kre, int begin, int end);
void kre_graph_free(struct KREItem* graph);

struct KREItem* kre_graph_item_new(KRE* kre, int* cursor);
void kre_graph_item_free(struct KREItem* item);

// character position detect a or b or ... or :{ or :} or :: or :< or :> or :* or :? or :+ or :[ or :] or :. or . or \t or \n or \w or
bool is_kre_graph_item_character_begin(String* kregexp, int cursor);
int find_kre_graph_item_character_end(String* kregexp, int begin);
int find_kre_graph_item_character_next(String* kregexp, int end);
struct KREItem* kre_graph_item_new_character(KRE* kre, int begin, int end);

// set position detect [...]
bool is_kre_graph_item_set_begin(String* kregexp, int cursor);
int find_kre_graph_item_set_end(String* kregexp, int begin);
int find_kre_graph_item_set_next(String* kregexp, int end);
struct KREItem* kre_graph_item_new_set(KRE* kre, int begin, int end);

// group position detect (...)
bool is_kre_graph_item_group_begin(String* kregexp, int cursor);
int find_kre_graph_item_group_end(String* kregexp, int begin);
int find_kre_graph_item_group_next(String* kregexp, int end);
struct KREItem* kre_graph_item_new_group(KRE* kre, int begin, int end);

// quantifier position detect {...}
bool is_kre_quantifier_begin(String* kregexp, int cursor);
int find_kre_quantifier_end(String* kregexp, int begin);
int find_kre_quantifier_comma(String* kregexp, int begin, int end);
// quantifier data extract ?, +, {n}, {m,}, {m,M}
int get_kre_quantifier_start(String* kregexp, int item_end);
int get_kre_quantifier_stop(String* kregexp, int item_end);

// implement methods
struct KREItem* kre_graph_new(KRE* kre, int begin, int end) {
    // create new dequeue(stack) for linking items and return first
    Dequeue* stack = dequeue_new_object(0, -1, NULL);

    // iterate kre and create next item+quantifier and add to stack
    int cursor = begin;
    while (cursor < end) {
        // create item
        struct KREItem* item = kre_graph_item_new(kre, &cursor);

        stack->vtable->enqueue(stack, 0, item, 0);
    }

    // iterate stack and link items and get first item as result
    struct KREItem* result = NULL;
    while (stack->vtable->size(stack) > 0) {
        struct KREItem* temp = stack->vtable->dequeue(stack, 0, 0);
        temp->next = result;
        result = temp;
    }

    // destroy dequeue(stack)
    dequeue_free(stack);

    return result;
}
void kre_graph_free(struct KREItem* graph) {
    while (graph != NULL) {
        struct KREItem* temp = graph->next;

        // destroy item
        kre_graph_item_free(graph);

        graph = temp;
    }
}

struct KREItem* kre_graph_item_new(KRE* kre, int* cursor) {
    struct KRE_* kre_ = (struct KRE_*)kre;

    // result item
    struct KREItem* result = NULL;

    // detect type of item and fill it
    if (is_kre_graph_item_character_begin(kre_->kregexp, *cursor)) {
        int end = find_kre_graph_item_character_end(kre_->kregexp, *cursor);

        // create item
        result = kre_graph_item_new_character(kre, *cursor, end);

        // set cursor to begining of next item
        *cursor = find_kre_graph_item_character_next(kre_->kregexp, end);
    } else if (is_kre_graph_item_set_begin(kre_->kregexp, *cursor)) {
        int end = find_kre_graph_item_set_end(kre_->kregexp, *cursor);

        // create item
        result = kre_graph_item_new_set(kre, *cursor, end);

        // set cursor to begining of next item
        *cursor = find_kre_graph_item_set_next(kre_->kregexp, end);
    } else if (is_kre_graph_item_group_begin(kre_->kregexp, *cursor)) {
        int end = find_kre_graph_item_group_end(kre_->kregexp, *cursor);

        // create item
        result = kre_graph_item_new_group(kre, *cursor, end);

        // set cursor to begining of next item
        *cursor = find_kre_graph_item_group_next(kre_->kregexp, end);
    }

    return result;
}
void kre_graph_item_free(struct KREItem* item) {
    if (item->type == KRE_ITEM_MATCH) {
        // free match string
        string_free(item->value.match.match);
    } else if (item->type == KRE_ITEM_GROUP) {
        // iterate items and free them
        for (int cursor = 0; cursor < item->value.group.items->vtable->size(item->value.group.items); cursor++) {
            kre_graph_free(item->value.group.items->vtable->get(item->value.group.items, cursor));
        }

        // don't iterate links (infinity loop and SegFault)

        // free group links
        arraylist_free(item->value.group.items);
        arraylist_free(item->value.group.links);
    }
    heap_free(item);
}

// character position detect a or b or ... or :{ or :} or :: or :< or :> or :* or :? or :+ or :[ or :] or :. or . or \t or \n or \w or
bool is_kre_graph_item_character_begin(String* kregexp, int cursor) {
    if (kregexp->vtable->value(kregexp)[cursor] == '.') {
        return true;
    }
    return false;
}
int find_kre_graph_item_character_end(String* kregexp, int begin) {
    // try find last !!! close metacharacter, count opened inner metacharacter
    if (kregexp->vtable->value(kregexp)[begin] == '\\') {
        return begin + 2;
    }
    return begin + 1;
}
int find_kre_graph_item_character_next(String* kregexp, int end) {
}
struct KREItem* kre_graph_item_new_character(KRE* kre, int begin, int end) {
    struct KRE_* kre_ = (struct KRE_*)kre;

    // create new item
    struct KREItem* result = heap_alloc(sizeof(struct KREItem));

    result->next = NULL;
    result->type = KRE_ITEM_MATCH;
    result->value.match.match = string_new_cut(kre_->kregexp->vtable->value(kre_->kregexp), begin, end);
    result->value.match.start =

        return result;
}

struct KREItem* kre_graph_item_new_match(String* kregexp, int begin, int end, int quantifier_start, int quantifier_stop) {
    // create new match item
    struct KREItem* result = heap_alloc(sizeof(struct KREItem));
    result->next = NULL;
    result->type = KRE_ITEM_MATCH;
    result->value.match.match = string_new_cut(kregexp->vtable->value(kregexp), begin, end);
    result->value.match.start = quantifier_start;
    result->value.match.stop = quantifier_stop;

    return result;
}
struct KREItem* kre_graph_item_new_group(String* kregexp, int begin, int end, int quantifier_start, int quantifier_stop) {
    // create new group item
    struct KREItem* result = heap_alloc(sizeof(struct KREItem));
    result->next = NULL;
    result->type = KRE_ITEM_GROUP;

    result->value.group.links = heap_alloc(sizeof(struct KREItem*) * 1);
    result->value.group.size = 1;
    result->value.group.mode = 0;

    result->value.group.start = quantifier_start;
    result->value.group.stop = quantifier_stop;

    return result;
}
void kre_graph_item_free(struct KREItem* item) {
    if (item->type == KRE_ITEM_MATCH) {
        string_free(item->value.match.match);
    } else if (item->type == KRE_ITEM_GROUP) {
        heap_free(item->value.group.links);
    }

    heap_free(item);
}

struct KREItem* kre_graph_item_new(String* kregexp, int cursor) {
}
void kre_graph_item_free(struct KREItem* item) {
}

// set position detect [...]
bool is_kre_set_begin(String* kregexp, int cursor) {
    if (kregexp->vtable->value(kregexp)[cursor] == '[') {
        return true;
    }
    return false;
}
int find_kre_set_end(String* kregexp, int begin) {
    // try find last !!! close metacharacter, count opened inner metacharacter
    int open = 0;
    for (int cursor = begin; cursor < kregexp->vtable->length(kregexp); cursor++) {
        if (kregexp->vtable->value(kregexp)[cursor] == '[') {
            open++;
        } else if (kregexp->vtable->value(kregexp)[cursor] == ']') {
            open--;
            if (open == 0) {
                return cursor;
            }
        }
    }
    return -1;
}

// group position detect (...)
bool is_kre_group_begin(String* kregexp, int cursor) {
    if (kregexp->vtable->value(kregexp)[cursor] == '(') {
        return true;
    }
    return false;
}
int find_kre_group_end(String* kregexp, int begin) {
    // try find last !!! close metacharacter, count opened inner metacharacter
    int open = 0;
    for (int cursor = begin; cursor < kregexp->vtable->length(kregexp); cursor++) {
        if (kregexp->vtable->value(kregexp)[cursor] == '(') {
            open++;
        } else if (kregexp->vtable->value(kregexp)[cursor] == ')') {
            open--;
            if (open == 0) {
                return cursor;
            }
        }
    }
    return -1;
}

// record position detect <...>
bool is_kre_record_begin(String* kregexp, int cursor) {
    if (kregexp->vtable->value(kregexp)[cursor] == '<') {
        return true;
    }
    return false;
}
int find_kre_record_end(String* kregexp, int begin) {
    // try find last !!! close metacharacter, count opened inner metacharacter
    int open = 0;
    for (int cursor = begin; cursor < kregexp->vtable->length(kregexp); cursor++) {
        if (kregexp->vtable->value(kregexp)[cursor] == '<') {
            open++;
        } else if (kregexp->vtable->value(kregexp)[cursor] == '>') {
            open--;
            if (open == 0) {
                return cursor;
            }
        }
    }
    return -1;
}

// quantifier position detect {...}
bool is_kre_quantifier_begin(String* kregexp, int cursor) {
    if (kregexp->vtable->value(kregexp)[cursor] == '{') {
        return true;
    }
    return false;
}
int find_kre_quantifier_end(String* kregexp, int begin) {
    // try find last !!! close metacharacter, count opened inner metacharacter
    int open = 0;
    for (int cursor = begin; cursor < kregexp->vtable->length(kregexp); cursor++) {
        if (kregexp->vtable->value(kregexp)[cursor] == '{') {
            open++;
        } else if (kregexp->vtable->value(kregexp)[cursor] == '}') {
            open--;
            if (open == 0) {
                return cursor;
            }
        }
    }
    return -1;
}
int find_kre_quantifier_comma(String* kregexp, int begin, int end) {
    for (int cursor = begin; cursor < end; cursor++) {
        if (kregexp->vtable->value(kregexp)[cursor] == ',') {
            return cursor;
        }
    }
    return -1;
}

// get part quantifier ? or + or * or {n} or {m,} or {m,M}
int get_kre_quantifier_start(String* kregexp, int cursor) {
    // move one char to next (from set or group or record character)
    cursor++;

    // detect type of quantifier
    if (kregexp->vtable->value(kregexp)[cursor] == '?') {
        return 0;
    } else if (kregexp->vtable->value(kregexp)[cursor] == '+') {
        return 1;
    } else if (kregexp->vtable->value(kregexp)[cursor] == '*') {
        return 0;
    } else if (is_kre_quantifier_begin(kregexp, cursor)) {
        // get min or n
        int end = find_kre_quantifier_end(kregexp, cursor);
        int comma = find_kre_quantifier_comma(kregexp, cursor, end);
        if (comma >= 0) {
            String* min_string = string_new_cut(kregexp->vtable->value(kregexp), cursor + 1, comma - 1);
            int result = min_string->vtable->to_int(min_string);
            string_free(min_string);
            return result;
        } else {
            String* number_string = string_new_cut(kregexp->vtable->value(kregexp), cursor + 1, end - 1);
            int result = number_string->vtable->to_int(number_string);
            string_free(number_string);
            return result;
        }
    } else {
        return 1;
    }
}
int get_kre_quantifier_stop(String* kregexp, int cursor) {
    // move one char to next (from set or group or record character)
    cursor++;

    // detect type of quantifier
    if (kregexp->vtable->value(kregexp)[cursor] == '?') {
        return 1;
    } else if (kregexp->vtable->value(kregexp)[cursor] == '+') {
        return -1;
    } else if (kregexp->vtable->value(kregexp)[cursor] == '*') {
        return -1;
    } else if (is_kre_quantifier_begin(kregexp, cursor)) {
        // get max or n
        int end = find_kre_quantifier_end(kregexp, cursor);
        int comma = find_kre_quantifier_comma(kregexp, cursor, end);
        if (comma >= 0) {
            String* max_string = string_new_cut(kregexp->vtable->value(kregexp), comma + 1, end - 1);
            int result = max_string->vtable->to_int(max_string);
            string_free(max_string);
            return result;
        } else {
            String* number_string = string_new_cut(kregexp->vtable->value(kregexp), cursor + 1, end - 1);
            int result = number_string->vtable->to_int(number_string);
            string_free(number_string);
            return result;
        }
    } else {
        return 1;
    }
}

// vtable operators

// object allocation and deallocation operators
void kre_init() {
    // init vtable
    kre_vtable = heap_alloc(sizeof(KRE_VTable));
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
        kre_graph_free(kre_->graph);
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
    kre_->graph = kre_graph_new((KRE*)kre_, 0, kre_->kregexp->vtable->length(kre_->kregexp));

    return (KRE*)kre_;
}