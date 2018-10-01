#include "kre_compiler.c"

// link methods
struct KREItem* kre_hipe_compile(KRE* kre);
void kre_hipe_free(struct KREItem* graph);

// implement methods
struct KREItem* kre_hipe_compile(KRE* kre) {
    struct KRE_* kre_ = (struct KRE_*)kre;

    // base compiler
    struct KREItem* result = kre_graph_new((KRE*)kre_, 0, kre_->kregexp->vtable->length(kre_->kregexp));

    // TODO: refactor graph (merge same matchers) + convert \s, \S, \w, \W, \d, \D to [...], char, .

    return result;
}
void kre_hipe_free(struct KREItem* graph) {
    kre_graph_free(graph);
}