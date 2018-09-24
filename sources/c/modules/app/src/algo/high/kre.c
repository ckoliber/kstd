#include <algo/high/kre.h>

struct KRE_ {
    // self public object
    KRE self;

    // constructor data

    // private data
    struct KREItem* graph;
};

struct KREItem {
    // item next pointer
    struct KREItem* next;

    // item type
    enum KREItemType {
        KRE_ITEM_MATCH,
        KRE_ITEM_GROUP,
        KRE_ITEM_RECORD
    } type;

    // item value (match, group, record)
    union KREItemValue {
        struct KREItemValueMatch {
            char* match;
            int start;
            int stop;
        } match;
        struct KREItemValueGroup {
            struct KREItem** group;
            int size;
            int start;
            int stop;
        } group;
        struct KREItemValueRecord {
            struct KREItem* record;
            int start;
            int stop;
        } record;
    } value;
};

// vtable
KRE_VTable* kre_vtable;

// link methods

// local methods
// kre compiler methods
struct KREItem* kre_graph_new(String* kregexp);
void kre_graph_free(struct KREItem* graph);

// character position detect a or b or ... or :{ or :} or :: or :< or :> or :* or :? or :+ or :[ or :] or :. or . or \t or \n or \w or

// set position detect [...]
bool is_kre_set_begin(String* kregexp, int cursor);
int find_kre_set_end(String* kregexp, int begin);

// group position detect (...)
bool is_kre_group_begin(String* kregexp, int cursor);
int find_kre_group_end(String* kregexp, int begin);

// record position detect <...>
bool is_kre_record_begin(String* kregexp, int cursor);
int find_kre_record_end(String* kregexp, int begin);

// quantifier position detect {...}
bool is_kre_quantifier_begin(String* kregexp, int cursor);
int find_kre_quantifier_end(String* kregexp, int begin);
int find_kre_quantifier_comma(String* kregexp, int begin, int end);

// get part quantifier ? or + or * or {n} or {m,} or {m,M}
int get_kre_quantifier_start(String* kregexp, int cursor);
int get_kre_quantifier_stop(String* kregexp, int cursor);

// implement methods
struct KREItem* kre_graph_new(String* kregexp) {
    int cursor = 0;
    while (cursor < kregexp->vtable->length(kregexp)) {
        if (is_kre_set_start(kregexp, cursor)) {
            int end = find_kre_set_end(kregexp, cursor);

        } else if (is_kre_counter_start(kregexp, cursor)) {
            int end = find_kre_set_end(kregexp, cursor);
        } else if (is_kre_group_start(kregexp, cursor)) {
            int end = find_kre_set_end(kregexp, cursor);
        } else if (is_kre_record_start(kregexp, cursor)) {
            int end = find_kre_set_end(kregexp, cursor);
        } else {
        }

        if (character == 'a' || character == 'b' || character == 'c' || character == '[' || character == ']') {
            // check next part is counter range
        } else if (character == '(' || character == ')') {
            // check next part is counter range
        } else if (character == '*' || character == '+' || character == '?' || character == '{' || character == '}') {
        }
    }

    struct KREItem* item = heap_alloc(sizeof(struct KREItem));
    item->value.group =
}
void kre_graph_free(struct KREItem* graph) {
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

    // set private data
    kre_->graph = NULL;

    return (KRE*)kre_;
}
void kre_free(KRE* kre) {
    struct KRE_* kre_ = (struct KRE_*)kre;

    // free private data
    if (kre_->graph != NULL) {
        kre_graph_free(kre_->graph);
    }

    // free self
    heap_free(kre_);
}
KRE* kre_new_object(char* kregexp) {
    struct KRE_* kre_ = (struct KRE_*)kre_new();

    // set constructor data

    // set private data
    String* kregexp_string = string_new_copy(kregexp);
    kre_->graph = kre_graph_new(kregexp_string);
    string_free(kregexp_string);

    return (KRE*)kre_;
}