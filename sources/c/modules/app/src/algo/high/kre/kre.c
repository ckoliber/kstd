#include <algo/high/kre.h>

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