#include <algo/high/kre.h>

#include <dsa/high/ArrayList.h>
#include <dsa/high/Dequeue.h>
#include <dsa/low/String.h>
#include <memory/low/Heap.h>

struct KRE_ {
    // self public object
    KRE self;

    // constructor data
    String* kregexp;

    // private data
    ArrayList* graph_links;
    struct KREState* graph;
};

struct KREState {
    // item next pointer
    struct KREState* next;

    // item type
    enum KREStateType {
        KRE_ITEM_MATCH,
        KRE_ITEM_GROUP
    } type;

    // item value (match, group)
    union KREStateValue {
        struct KREStateValueMatch {
            String* match;
            int start;
            int stop;
        } match;
        struct KREStateValueGroup {
            ArrayList* items;
            ArrayList* links;
            int mode;
            int start;
            int stop;
        } group;
    } value;
};

struct KREItem {
    struct KREState* state;
    int value;
};

struct KRERecorder {
    String* record;
    int value;
};

// vtable
KRE_VTable* kre_vtable;

// link methods
bool kre_test(struct KRE* self, char* text);

ArrayList* kre_match(struct KRE* self, char* text);
String* kre_replace(struct KRE* self, char* text, char* replace);

ArrayList* kre_split(struct KRE* self, char* text);
String* kre_merge(struct KRE* self, ArrayList* data);

ArrayList* kre_export(struct KRE* self, char* text);
String* kre_import(struct KRE* self, ArrayList* data);

// local methods

///////////////////////////////////////////////////////////////
// match methods begin
///////////////////////////////////////////////////////////////

// link methods
bool kre_match_matches(struct KREState* state, char character);

// local methods
bool kre_match_is_whitespace(char character);
bool kre_match_is_alphanumeric(char character);
bool kre_match_is_digit(char character);
bool kre_match_set(String* set, char character);

///////////////////////////////////////////////////////////////
// match methods end
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// compile methods begin
///////////////////////////////////////////////////////////////

// link methods
struct KREState* kre_compile_new(KRE* kre);
void kre_compile_free(struct KREState* graph);
bool kre_compile_is_packable(struct KREState* graph, int strict);  // strict = 2 -> merge, 1 -> import, 0 -> X

// local methods
struct KREState* kre_compile_graph_link(KRE* kre, int begin, int end);
struct KREState* kre_compile_graph_new(KRE* kre, int begin, int end);
void kre_compile_graph_free(struct KREState* graph);

struct KREState* kre_compile_state_new(KRE* kre, int* cursor);
void kre_compile_state_free(struct KREState* state);

// character position detect a or b or ... or :{ or :} or :: or :< or :> or :* or :? or :+ or :[ or :] or :. or . or \t or \n or \w or
bool kre_compile_is_character_begin(String* kregexp, int cursor);
int kre_compile_find_character_end(String* kregexp, int begin);
int kre_compile_find_character_next(String* kregexp, int end);
struct KREState* kre_compile_new_character(KRE* kre, int begin, int end);

// set position detect [...]
bool kre_compile_is_set_begin(String* kregexp, int cursor);
int kre_compile_find_set_end(String* kregexp, int begin);
int kre_compile_find_set_next(String* kregexp, int end);
struct KREState* kre_compile_new_set(KRE* kre, int begin, int end);

// group position detect (...)
bool kre_compile_is_group_begin(String* kregexp, int cursor);
int kre_compile_find_group_end(String* kregexp, int begin);
int kre_compile_find_group_next(String* kregexp, int end);
int kre_compile_find_group_or(String* kregexp, int cursor, int begin, int end);
struct KREState* kre_compile_new_group(KRE* kre, int begin, int end);

// quantifier position detect {...}
bool kre_compile_is_quantifier_begin(String* kregexp, int cursor);
int kre_compile_find_quantifier_end(String* kregexp, int begin);
int kre_compile_find_quantifier_comma(String* kregexp, int begin, int end);
// quantifier data extract ?, +, {n}, {m,}, {m,M}
int kre_compile_get_quantifier_start(String* kregexp, int item_end);
int kre_compile_get_quantifier_stop(String* kregexp, int item_end);

// packable character detect . [ \s \S \w \W \d \D
bool kre_compile_is_match_multistate(struct KREState* state, int cursor, int strict);
bool kre_compile_is_group_multistate(struct KREState* state, int strict);

///////////////////////////////////////////////////////////////
// compile methods end
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// regex methods begin
///////////////////////////////////////////////////////////////

// link methods
// low level methods
ArrayList* kre_regex_parse(struct KRE* self, String* text, int begin, int* end, bool record);
String* kre_regex_pack(struct KRE* self, ArrayList* data, bool record);

// high level methods
ArrayList* kre_regex_match_new(struct KRE* self, char* text, int begin, int count);
void kre_regex_match_free(ArrayList* matches);

// local methods
struct KREItem* kre_regex_item_new(struct KREState* state);
void kre_regex_item_free(struct KREItem* item);

///////////////////////////////////////////////////////////////
// regex methods end
///////////////////////////////////////////////////////////////

// implement methods

///////////////////////////////////////////////////////////////
// match methods begin
///////////////////////////////////////////////////////////////

// implement methods
bool kre_match_is_whitespace(char character) {
    if (
        character == '\t' ||
        character == '\f' ||
        character == '\r' ||
        character == '\n' ||
        character == '\v' ||
        character == ' ') {
        return true;
    }

    return false;
}
bool kre_match_is_alphanumeric(char character) {
    if (
        (character >= 'a' && character <= 'z') ||
        (character >= 'A' && character <= 'Z') ||
        (character >= '0' && character <= '9') ||
        character == '_') {
        return true;
    }

    return false;
}
bool kre_match_is_digit(char character) {
    if (
        (character >= '0' && character <= '9')) {
        return true;
    }

    return false;
}
bool kre_match_set(String* set, char character) {
    // detect type of set
    bool is_not = set->vtable->value(set)[1] == '^';

    // iterate set characters and match with character
    for (int cursor = (is_not ? 2 : 1); cursor < set->vtable->length(set) - 1; cursor++) {
        if (set->vtable->value(set)[cursor] == '-') {
            // range type

            // return if character was in range
            if (
                character >= set->vtable->value(set)[cursor - 1] &&
                character <= set->vtable->value(set)[cursor + 1]) {
                return !is_not;
            }
        } else if (set->vtable->value(set)[cursor] == '\\') {
            // special character
            cursor++;
            char meta_character = set->vtable->value(set)[cursor];

            // return if character matches with this part of set
            if (
                (meta_character == 't' && character == '\t') ||
                (meta_character == 'f' && character == '\f') ||
                (meta_character == 'r' && character == '\r') ||
                (meta_character == 'n' && character == '\n') ||
                (meta_character == 'v' && character == '\v') ||
                (meta_character == character)) {
                return !is_not;
            }
        } else {
            // normal character

            // return if character matches with this
            if (set->vtable->value(set)[cursor] == character) {
                return !is_not;
            }
        }
    }

    return is_not;
}

// kre base matcher
bool kre_match_matches(struct KREState* state, char character) {
    // check item type is match
    if (state->type != KRE_ITEM_MATCH) {
        return false;
    }

    // match character
    char first_match = state->value.match.match->vtable->value(state->value.match.match)[0];
    char second_match = state->value.match.match->vtable->value(state->value.match.match)[1];
    if (first_match == '.') {
        // dot type (any character)
        return true;
    } else if (first_match == '\\') {
        if (second_match == 's') {
            // Whitespace, \t \f \r \n \v and spaces
            return kre_match_is_whitespace(character);
        } else if (second_match == 'S') {
            // Non-whitespace
            return !kre_match_is_whitespace(character);
        } else if (second_match == 'w') {
            // Alphanumeric, [a-zA-Z0-9_]
            return kre_match_is_alphanumeric(character);
        } else if (second_match == 'W') {
            // Non-alphanumeric
            return !kre_match_is_alphanumeric(character);
        } else if (second_match == 'd') {
            // Digits, [0-9]
            return kre_match_is_digit(character);
        } else if (second_match == 'D') {
            // Non-digits
            return !kre_match_is_digit(character);
        }
    } else if (first_match == '[') {
        // set type
        return kre_match_set(state->value.match.match, character);
    } else {
        // character type
        return first_match == character;
    }

    // return false when there were no conditions
    return false;
}

///////////////////////////////////////////////////////////////
// match methods end
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// compile methods begin
///////////////////////////////////////////////////////////////

// implement methods
struct KREState* kre_compile_graph_link(KRE* kre, int begin, int end) {
    struct KRE_* kre_ = (struct KRE_*)kre;

    // check item is link (\i) then get target linked item
    struct KREState* result = NULL;
    if (
        kre_->kregexp->vtable->value(kre_->kregexp)[begin] == '\\' &&
        kre_->kregexp->vtable->value(kre_->kregexp)[begin + 1] >= '0' &&
        kre_->kregexp->vtable->value(kre_->kregexp)[begin + 1] <= '9') {
        // parse string to number
        String* number = string_new_cut(kre_->kregexp->vtable->value(kre_->kregexp), begin + 1, end);
        int link_number = number->vtable->to_int(number);
        string_free(number);
        result = (struct KREState*)kre_->graph_links->vtable->get(kre_->graph_links, link_number);
    }

    return result;
}
struct KREState* kre_compile_graph_new(KRE* kre, int begin, int end) {
    // create new dequeue(stack) for linking items and return first
    Dequeue* stack = dequeue_new_object(0, -1, NULL);

    // iterate kre and create next item+quantifier and add to stack
    int cursor = begin;
    while (cursor < end) {
        // create item
        struct KREState* state = kre_compile_state_new(kre, &cursor);

        stack->vtable->enqueue(stack, 0, state, 0);
    }

    // iterate stack and link items and get first item as result
    struct KREState* result = NULL;
    while (stack->vtable->size(stack) > 0) {
        struct KREState* temp = stack->vtable->dequeue(stack, 0, 0);
        temp->next = result;
        result = temp;
    }

    // destroy dequeue(stack)
    dequeue_free(stack);

    return result;
}
void kre_compile_graph_free(struct KREState* graph) {
    while (graph != NULL) {
        struct KREState* temp = graph->next;

        // destroy item
        kre_compile_state_free(graph);

        graph = temp;
    }
}

struct KREState* kre_compile_state_new(KRE* kre, int* cursor) {
    struct KRE_* kre_ = (struct KRE_*)kre;

    // result item
    struct KREState* result = NULL;

    // detect type of item and fill it
    if (kre_compile_is_character_begin(kre_->kregexp, *cursor)) {
        int end = kre_compile_find_character_end(kre_->kregexp, *cursor);

        // create item
        result = kre_compile_new_character(kre, *cursor, end);

        // set cursor to begining of next item
        *cursor = kre_compile_find_character_next(kre_->kregexp, end);
    } else if (kre_compile_is_set_begin(kre_->kregexp, *cursor)) {
        int end = kre_compile_find_set_end(kre_->kregexp, *cursor);

        // create item
        result = kre_compile_new_set(kre, *cursor, end);

        // set cursor to begining of next item
        *cursor = kre_compile_find_set_next(kre_->kregexp, end);
    } else if (kre_compile_is_group_begin(kre_->kregexp, *cursor)) {
        int end = kre_compile_find_group_end(kre_->kregexp, *cursor);

        // create item
        result = kre_compile_new_group(kre, *cursor, end);

        // set cursor to begining of next item
        *cursor = kre_compile_find_group_next(kre_->kregexp, end);
    }

    return result;
}
void kre_compile_state_free(struct KREState* state) {
    if (state->type == KRE_ITEM_MATCH) {
        // free match string
        string_free(state->value.match.match);
    } else if (state->type == KRE_ITEM_GROUP) {
        // iterate items and free them
        for (int cursor = 0; cursor < state->value.group.items->vtable->size(state->value.group.items); cursor++) {
            kre_compile_graph_free(state->value.group.items->vtable->get(state->value.group.items, cursor));
        }

        // don't iterate links (infinity loop and SegFault)

        // free group links
        arraylist_free(state->value.group.items);
        arraylist_free(state->value.group.links);
    }
    heap_free(state);
}

// character position detect a or b or ... or :{ or :} or :: or :< or :> or :* or :? or :+ or :[ or :] or :. or . or \t or \n or \w or
bool kre_compile_is_character_begin(String* kregexp, int cursor) {
    if (kregexp->vtable->value(kregexp)[cursor] != '[' &&
        kregexp->vtable->value(kregexp)[cursor] != ']' &&
        kregexp->vtable->value(kregexp)[cursor] != '(' &&
        kregexp->vtable->value(kregexp)[cursor] != ')' &&
        kregexp->vtable->value(kregexp)[cursor] != '{' &&
        kregexp->vtable->value(kregexp)[cursor] != '}' &&
        kregexp->vtable->value(kregexp)[cursor] != '?' &&
        kregexp->vtable->value(kregexp)[cursor] != '+' &&
        kregexp->vtable->value(kregexp)[cursor] != '*') {
        return true;
    }
    return false;
}
int kre_compile_find_character_end(String* kregexp, int begin) {
    // try find last !!! close metacharacter, count opened inner metacharacter
    if (kregexp->vtable->value(kregexp)[begin] == '\\') {
        return begin + 2;
    }
    return begin + 1;
}
int kre_compile_find_character_next(String* kregexp, int end) {
    // check part has quantifier or not (if it has, return endof(quantifier) + 1, else return end + 1)
    if (kre_compile_is_quantifier_begin(kregexp, end + 1)) {
        int quantifier_end = kre_compile_find_quantifier_end(kregexp, end + 1);
        return quantifier_end + 1;
    }
    return end + 1;
}
struct KREState* kre_compile_new_character(KRE* kre, int begin, int end) {
    struct KRE_* kre_ = (struct KRE_*)kre;

    // create new item
    struct KREState* result = heap_alloc(sizeof(struct KREState));

    // fill item
    result->next = NULL;
    result->type = KRE_ITEM_MATCH;
    result->value.match.match = string_new_cut(kre_->kregexp->vtable->value(kre_->kregexp), begin, end);
    result->value.match.start = kre_compile_get_quantifier_start(kre_->kregexp, end);
    result->value.match.stop = kre_compile_get_quantifier_stop(kre_->kregexp, end);

    return result;
}

// set position detect [...]
bool kre_compile_is_set_begin(String* kregexp, int cursor) {
    if (kregexp->vtable->value(kregexp)[cursor] == '[') {
        return true;
    }
    return false;
}
int kre_compile_find_set_end(String* kregexp, int begin) {
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
int kre_compile_find_set_next(String* kregexp, int end) {
    // check part has quantifier or not (if it has, return endof(quantifier) + 1, else return end + 1)
    if (kre_compile_is_quantifier_begin(kregexp, end + 1)) {
        int quantifier_end = kre_compile_find_quantifier_end(kregexp, end + 1);
        return quantifier_end + 1;
    }
    return end + 1;
}
struct KREState* kre_compile_new_set(KRE* kre, int begin, int end) {
    struct KRE_* kre_ = (struct KRE_*)kre;

    // create new item
    struct KREState* result = heap_alloc(sizeof(struct KREState));

    // fill item
    result->next = NULL;
    result->type = KRE_ITEM_MATCH;
    result->value.match.match = string_new_cut(kre_->kregexp->vtable->value(kre_->kregexp), begin, end);
    result->value.match.start = kre_compile_get_quantifier_start(kre_->kregexp, end);
    result->value.match.stop = kre_compile_get_quantifier_stop(kre_->kregexp, end);

    return result;
}

// group position detect (...)
bool kre_compile_is_group_begin(String* kregexp, int cursor) {
    if (kregexp->vtable->value(kregexp)[cursor] == '(') {
        return true;
    }
    return false;
}
int kre_compile_find_group_end(String* kregexp, int begin) {
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
int kre_compile_find_group_next(String* kregexp, int end) {
    // check part has quantifier or not (if it has, return endof(quantifier) + 1, else return end + 1)
    if (kre_compile_is_quantifier_begin(kregexp, end + 1)) {
        int quantifier_end = kre_compile_find_quantifier_end(kregexp, end + 1);
        return quantifier_end + 1;
    }
    return end + 1;
}
int kre_compile_find_group_or(String* kregexp, int cursor, int begin, int end) {
    int open = 0;

    // not started from begining of group (set open to 1)
    if (cursor > begin) {
        open = 1;
    }

    // check this cursor is not or
    if (kregexp->vtable->value(kregexp)[cursor] == '|') {
        cursor++;
    }

    // iterate group characters
    while (cursor < end) {
        if (kregexp->vtable->value(kregexp)[cursor] == '(') {
            // open internal group
            open++;
        } else if (kregexp->vtable->value(kregexp)[cursor] == ')') {
            // close internal group
            open--;
        } else if (kregexp->vtable->value(kregexp)[cursor] == '|') {
            // now, check open is 1 (we are in main group (not internal group))
            if (open == 1) {
                return cursor;
            }
        }

        // add cursor
        cursor++;
    }

    return -1;
}
struct KREState* kre_compile_new_group(KRE* kre, int begin, int end) {
    struct KRE_* kre_ = (struct KRE_*)kre;

    // create new item
    struct KREState* result = heap_alloc(sizeof(struct KREState));

    // group: (mode|item1|item2|item3|...)
    // fill item
    result->next = NULL;
    result->type = KRE_ITEM_GROUP;
    result->value.group.items = arraylist_new_object(0, 2, NULL);
    result->value.group.links = arraylist_new_object(0, 2, NULL);
    result->value.group.mode = kre_->kregexp->vtable->value(kre_->kregexp)[begin + 1] - '0';
    result->value.group.start = kre_compile_get_quantifier_start(kre_->kregexp, end);
    result->value.group.stop = kre_compile_get_quantifier_stop(kre_->kregexp, end);

    // if group type is capture (1) or record + capture (3) then add it to graph_links
    if (result->value.group.mode == 1 || result->value.group.mode == 3) {
        kre_->graph_links->vtable->add(kre_->graph_links, result);
    }

    // find or positions and iterate group items
    int item_cursor = begin + 1;
    do {
        int item_begin_before = kre_compile_find_group_or(kre_->kregexp, item_cursor, begin, end);
        int item_end_after = kre_compile_find_group_or(kre_->kregexp, item_cursor + 1, begin, end);

        // check end has error (end of group)
        if (item_end_after < 0) {
            item_end_after = end;
        }

        // check item is link get item else create new item and add to links or items
        struct KREState* state = kre_compile_graph_link(kre, item_begin_before + 1, item_end_after - 1);
        if (state != NULL) {
            // add link item to links
            result->value.group.links->vtable->add(result->value.group.links, state);
        } else {
            // add new item to items
            state = kre_graph_new(kre, item_begin_before + 1, item_end_after - 1);
            result->value.group.items->vtable->add(result->value.group.items, state);
        }

        item_cursor = item_end_after - 1;
    } while (item_cursor >= 0 && item_cursor < end - 1);

    return result;
}

// quantifier position detect {...}
bool kre_compile_is_quantifier_begin(String* kregexp, int cursor) {
    if (
        kregexp->vtable->value(kregexp)[cursor] == '{' ||
        kregexp->vtable->value(kregexp)[cursor] == '?' ||
        kregexp->vtable->value(kregexp)[cursor] == '+' ||
        kregexp->vtable->value(kregexp)[cursor] == '*') {
        return true;
    }
    return false;
}
int kre_compile_find_quantifier_end(String* kregexp, int begin) {
    if (kregexp->vtable->value(kregexp)[begin] == '{') {
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
    } else {
        // quantifier type is ?, +, *
        return begin + 1;
    }
}
int kre_compile_find_quantifier_comma(String* kregexp, int begin, int end) {
    for (int cursor = begin; cursor < end; cursor++) {
        if (kregexp->vtable->value(kregexp)[cursor] == ',') {
            return cursor;
        }
    }
    return -1;
}
// quantifier data extract ?, +, {n}, {m,}, {m,M}
int kre_compile_get_quantifier_start(String* kregexp, int item_end) {
    // move one char to next (from set or group or record character)
    item_end++;

    // detect end is quantifier
    if (kre_compile_is_quantifier_begin(kregexp, item_end)) {
        if (kregexp->vtable->value(kregexp)[item_end] == '?') {
            return 0;
        } else if (kregexp->vtable->value(kregexp)[item_end] == '+') {
            return 1;
        } else if (kregexp->vtable->value(kregexp)[item_end] == '*') {
            return 0;
        } else {
            // get min or n
            int end = kre_compile_find_quantifier_end(kregexp, item_end);
            int comma = kre_compile_find_quantifier_comma(kregexp, item_end, end);
            if (comma >= 0) {
                // finite => {min,max}
                String* min_string = string_new_cut(kregexp->vtable->value(kregexp), item_end + 1, comma - 1);
                int result = min_string->vtable->to_int(min_string);
                string_free(min_string);
                return result;
            } else {
                // number => {n}
                String* number_string = string_new_cut(kregexp->vtable->value(kregexp), item_end + 1, end - 1);
                int result = number_string->vtable->to_int(number_string);
                string_free(number_string);
                return result;
            }
        }
    } else {
        return 1;
    }
}
int kre_compile_get_quantifier_stop(String* kregexp, int item_end) {
    // move one char to next (from set or group or record character)
    item_end++;

    // detect end is quantifier
    if (kre_compile_is_quantifier_begin(kregexp, item_end)) {
        if (kregexp->vtable->value(kregexp)[item_end] == '?') {
            return 1;
        } else if (kregexp->vtable->value(kregexp)[item_end] == '+') {
            return -1;
        } else if (kregexp->vtable->value(kregexp)[item_end] == '*') {
            return -1;
        } else {
            // get infinity or max or n
            int end = kre_compile_find_quantifier_end(kregexp, item_end);
            int comma = kre_compile_find_quantifier_comma(kregexp, item_end, end);
            if (comma >= 0) {
                if (comma + 1 == end) {
                    // infinity => {min,}
                    return -1;
                } else {
                    // finite => {min,max}
                    String* max_string = string_new_cut(kregexp->vtable->value(kregexp), comma + 1, end - 1);
                    int result = max_string->vtable->to_int(max_string);
                    string_free(max_string);
                    return result;
                }
            } else {
                // number => {n}
                String* number_string = string_new_cut(kregexp->vtable->value(kregexp), item_end + 1, end - 1);
                int result = number_string->vtable->to_int(number_string);
                string_free(number_string);
                return result;
            }
        }
    } else {
        return 1;
    }
}

// packable character detect . [ \s \S \w \W \d \D
bool kre_compile_is_match_multistate(struct KREState* state, int cursor, int strict) {
    if (strict == 2 || strict == 1) {
        // check is multi state match character
        if (state->value.match.match->vtable->value(state->value.match.match)[cursor] == '.' ||
            state->value.match.match->vtable->value(state->value.match.match)[cursor] == '[' ||
            (state->value.match.match->vtable->value(state->value.match.match)[cursor] == '\\' && state->value.match.match->vtable->value(state->value.match.match)[cursor + 1] == 's') ||
            (state->value.match.match->vtable->value(state->value.match.match)[cursor] == '\\' && state->value.match.match->vtable->value(state->value.match.match)[cursor + 1] == 'S') ||
            (state->value.match.match->vtable->value(state->value.match.match)[cursor] == '\\' && state->value.match.match->vtable->value(state->value.match.match)[cursor + 1] == 'w') ||
            (state->value.match.match->vtable->value(state->value.match.match)[cursor] == '\\' && state->value.match.match->vtable->value(state->value.match.match)[cursor + 1] == 'W') ||
            (state->value.match.match->vtable->value(state->value.match.match)[cursor] == '\\' && state->value.match.match->vtable->value(state->value.match.match)[cursor + 1] == 'd') ||
            (state->value.match.match->vtable->value(state->value.match.match)[cursor] == '\\' && state->value.match.match->vtable->value(state->value.match.match)[cursor + 1] == 'D')) {
            return true;
        }

        // check quantifier range is absolute
        if (state->value.match.start != state->value.match.stop) {
            return true;
        }
    }

    return false;
}
bool kre_compile_is_group_multistate(struct KREState* state, int strict) {
    if (strict == 2) {
        // check mode is 0 (no capture, no record)
        if (state->value.group.mode != 0) {
            return true;
        }

        // check group items length is 1
        if (state->value.group.items->vtable->size(state->value.group.items) == 1) {
            // check internal group item
            if (!kre_compile_is_packable(state->value.group.items->vtable->get(state->value.group.items, 0), strict)) {
                return true;
            }
        } else {
            return true;
        }

        // check group items length is 0
        if (state->value.group.links->vtable->size(state->value.group.links) > 0) {
            return true;
        }

        // check quantifier range is absolute
        if (state->value.group.start != state->value.group.stop) {
            return true;
        }
    } else if (strict == 1) {
        // check mode
        if (state->value.group.mode == 0 || state->value.group.mode == 1) {
            // x capture, no record mode

            // check group items length is 1
            if (state->value.group.items->vtable->size(state->value.group.items) == 1) {
                // check internal group item
                if (!kre_compile_is_packable(state->value.group.items->vtable->get(state->value.group.items, 0), strict)) {
                    return true;
                }
            } else {
                return true;
            }

            // check group items length is 0
            if (state->value.group.links->vtable->size(state->value.group.links) > 0) {
                return true;
            }
        } else if (state->value.group.mode == 2 || state->value.group.mode == 3) {
            // x capture, yes record mode

            // iterate items and check with strict 0 (record group internal)
            for (int cursor = 0; cursor < state->value.group.items->vtable->size(state->value.group.items); cursor++) {
                if (!kre_compile_is_packable(state->value.group.items->vtable->get(state->value.group.items, cursor), 0)) {
                    return true;
                }
            }
        }

        // check quantifier range is absolute and is 1
        if (state->value.group.start != 1 || state->value.group.stop != 1) {
            return true;
        }
    } else if (strict == 0) {
        // check mode is 0 (no capture, no record)
        if (state->value.group.mode != 0) {
            return true;
        }

        // iterate items and check with strict 0 (record group internal)
        for (int cursor = 0; cursor < state->value.group.items->vtable->size(state->value.group.items); cursor++) {
            if (!kre_compile_is_packable(state->value.group.items->vtable->get(state->value.group.items, cursor), strict)) {
                return true;
            }
        }
    }

    return false;
}

// kre base compiler
struct KREState* kre_compile_new(KRE* kre) {
    struct KRE_* kre_ = (struct KRE_*)kre;

    // set regex to group mode for handling or
    String* kregexp = string_new_printf("(0|%s)", kre_->kregexp->vtable->value(kre_->kregexp));
    string_free(kre_->kregexp);
    kre_->kregexp = kregexp;

    // base compiler
    struct KREState* result = kre_compile_graph_new((KRE*)kre_, 0, kre_->kregexp->vtable->length(kre_->kregexp));

    return result;
}
void kre_compile_free(struct KREState* graph) {
    kre_compile_graph_free(graph);
}
bool kre_compile_is_packable(struct KREState* graph, int strict) {
    while (graph != NULL) {
        if (graph->type == KRE_ITEM_MATCH) {
            // check is not multi state match
            if (kre_compile_is_match_multistate(graph->value.match.match, 0, strict)) {
                return false;
            }
        } else if (graph->type = KRE_ITEM_GROUP) {
            // check is not multi state group
            if (kre_compile_is_group_multistate(graph, strict)) {
                return false;
            }
        }

        // move to next graph item
        graph = graph->next;
    }

    return true;
}

///////////////////////////////////////////////////////////////
// compile methods end
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// regex methods begin
///////////////////////////////////////////////////////////////

// implement methods
// high level methods
ArrayList* kre_regex_match_new(struct KRE* self, char* text, int begin, int count) {
    struct KRE_* kre_ = (struct KRE_*)self;

    // try match text parts with regex
    ArrayList* result = arraylist_new_object(0, 2, NULL);
    String* text_string = string_new_copy(text);

    for (int cursor = begin; cursor < text_string->vtable->length(text_string) && count > 0; cursor++) {
        // try get end of match
        int match_end = -1;
        kre_regex_parse(self, text_string, cursor, &match_end, false);

        // check end of match
        if (match_end >= 0) {
            // add item (begin, end) to result array
            int* item = heap_alloc(sizeof(int) * 2);
            item[0] = cursor;     // begin
            item[1] = match_end;  // end
            result->vtable->add(result, item);

            // add to cursor
            cursor = match_end + 1;

            // reduce counter
            count--;
        }
    }

    // free allocated items
    string_free(text_string);

    return result;
}
void kre_regex_match_free(ArrayList* matches) {
    // free matches items
    while (matches->vtable->size(matches) > 0) {
        heap_free(matches->vtable->remove(matches, matches->vtable->size(matches) - 1));
    }

    // free array
    arraylist_free(matches);
}

// local methods
struct KREItem* kre_regex_item_new(struct KREState* state) {
    struct KREItem* result = heap_alloc(sizeof(struct KREItem));

    // init item
    result->state = state;
    result->value = 0;

    return result;
}
void kre_regex_item_free(struct KREItem* item) {
    heap_free(item);
}

// low level methods
// kre base regex
ArrayList* kre_regex_parse(struct KRE* self, String* text, int begin, int* end, bool record) {
    struct KRE_* kre_ = (struct KRE_*)self;

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

    // init requirements
    bool force = false;
    Dequeue* items = dequeue_new_object(0, -1, NULL);
    Dequeue* recorders = dequeue_new_object(0, -1, NULL);
    ArrayList* result = arraylist_new_object(0, 2, NULL);

    // push first item
    items->vtable->enqueue(items, 0, kre_regex_item_new(kre_->graph), 0);

    while (items->vtable) {
        // pop item
        struct KREItem* item = items->vtable->dequeue(items, 0, 0);

        if (item->state->type == KRE_ITEM_MATCH) {
            if (kre_regex_match_in_range(item) && !force) {
                items->vtable->enqueue(items, 0, item, 0);

                if (item->state->next != NULL) {
                    // add next item
                    items->vtable->enqueue(items, 0, kre_regex_item_new(item->state->next), 0);
                } else {
                    // pop and free items while not get group type
                    struct KREItem* group_item = items->vtable->dequeue(items, 0, 0);
                    while (group_item != NULL && group_item->state->type != KRE_ITEM_GROUP) {
                        kre_regex_item_free(group_item);
                        group_item = items->vtable->dequeue(items, 0, 0);
                    }

                    // check not end
                    if (group_item != NULL) {
                        if (kre_regex_group_in_range(group_item)) {
                            group_item->value++;
                            items->vtable->enqueue(items, 0, group_item, 0);
                            if (group_item->state->next != NULL) {
                                items->vtable->enqueue(items, 0, kre_regex_item_new(group_item->state->next), 0);
                            } else {
                            }
                        } else {
                        }
                    } else {
                    }
                }
            }
        } else if (item->state->type == KRE_ITEM_GROUP) {
        }
    }

    // free items stack
    while (items->vtable->size(items) > 0) {
        kre_regex_item_free(items->vtable->dequeue(items, 0, 0));
    }
    dequeue_free(items);

    // free recorders stack
    while (recorders->vtable->size(recorders) > 0) {
        kre_regex_item_free(recorders->vtable->dequeue(recorders, 0, 0));
    }
    dequeue_free(recorders);

    return result;
}
String* kre_regex_pack(struct KRE* self, ArrayList* data, bool record) {
}

///////////////////////////////////////////////////////////////
// regex methods end
///////////////////////////////////////////////////////////////

// vtable operators
bool kre_test(struct KRE* self, char* text) {
    struct KRE_* kre_ = (struct KRE_*)self;

    // match items
    bool result = false;
    ArrayList* matches = kre_regex_match_new(self, text, 0, 1);

    // check matches count
    if (matches->vtable->size(matches) == 1) {
        // get item
        int* item = matches->vtable->get(matches, 0);

        // check item is matches with entire of text
        if (item[0] == 0 && item[1] == string_get_length(text) - 1) {
            result = true;
        }
    }

    // free allocated items
    kre_regex_match_free(matches);

    return result;
}

ArrayList* kre_match(struct KRE* self, char* text) {
    struct KRE_* kre_ = (struct KRE_*)self;

    // match items
    ArrayList* result = kre_regex_match_new(self, text, 0, string_get_length(text));

    return result;
}
String* kre_replace(struct KRE* self, char* text, char* replace) {
    struct KRE_* kre_ = (struct KRE_*)self;

    // match items
    ArrayList* matches = kre_regex_match_new(self, text, 0, 1);

    // check matches count
    String* result = string_new_copy(text);
    if (matches->vtable->size(matches) == 1) {
        // get item
        int* item = matches->vtable->get(matches, 0);

        // replace string
        result->vtable->replace(result, item[0], item[1], replace);
    }

    // free allocated items
    kre_regex_match_free(matches);

    return result;
}

ArrayList* kre_split(struct KRE* self, char* text) {
    struct KRE_* kre_ = (struct KRE_*)self;

    // match items
    ArrayList* matches = kre_regex_match_new(self, text, 0, string_get_length(text));

    // reverse matches items for result
    ArrayList* result = arraylist_new_object(0, 2, NULL);

    int start = 0;
    int end = -1;
    for (int cursor = 0; cursor < matches->vtable->size(matches); cursor++) {
        // get item
        int* item = matches->vtable->get(matches, 0);

        // inverse item start to end
        end = item[0];

        if (start != end) {
            // add item (begin, end) to result array
            int* item = heap_alloc(sizeof(int) * 2);
            item[0] = start;  // begin
            item[1] = end;    // end
            result->vtable->add(result, item);
        }

        // inverse item end to start
        start = item[1];
    }

    // free allocated items
    kre_regex_match_free(matches);

    return result;
}
String* kre_merge(struct KRE* self, ArrayList* data) {
    struct KRE_* kre_ = (struct KRE_*)self;

    // pack data with regex (merge - no record - strict 2)
    String* result = NULL;
    if (kre_compile_is_packable(kre_->graph, 2)) {
        result = kre_regex_pack(self, data, false);
    }

    return result;
}

ArrayList* kre_export(struct KRE* self, char* text) {
    struct KRE_* kre_ = (struct KRE_*)self;

    // parse text with regex (export - record - strict 1)
    int end = -1;
    String* text_string = string_new_copy(text);
    ArrayList* result = kre_regex_parse(self, text_string, 0, &end, true);

    // free text string
    string_free(text_string);

    return result;
}
String* kre_import(struct KRE* self, ArrayList* data) {
    struct KRE_* kre_ = (struct KRE_*)self;

    // pack data with regex (import - record - strict 1)
    String* result = NULL;
    if (kre_compile_is_packable(kre_->graph, 1)) {
        result = kre_regex_pack(self, data, true);
    }

    return result;
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
        kre_compile_free(kre_->graph);
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
    kre_->graph = kre_compile_new((KRE*)kre_);

    return (KRE*)kre_;
}