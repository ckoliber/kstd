#include <low/itc/high/Semaphore.h>

#include <io/memory/Memory.h>
#include <low/itc/low/Cond.h>
#include <pthread.h>
#include <stdint.h>

struct Semaphore_ {
    struct Semaphore self;
    struct Cond* cond;
    int value;
};

struct Arg {
    struct Semaphore_* semaphore_;
    int count;
    int type;
};

// link methods
int semaphore_wait(struct Semaphore* self, int count);
int semaphore_timewait(struct Semaphore* self, int count, int timeout);
int semaphore_post(struct Semaphore* self, int count);
int semaphore_get(struct Semaphore* self);

// local methods
int condition(void* arg);
void critical(void* arg);

int condition(void* arg) {
    struct Arg* arg_ = arg;
    return arg_->semaphore_->value < arg_->count;
}
void critical(void* arg) {
    struct Arg* arg_ = arg;
    if (arg_->type == 0) {
        arg_->semaphore_->value -= arg_->count;
    } else {
        arg_->semaphore_->value += arg_->count;
    }
}

int semaphore_wait(struct Semaphore* self, int count) {
    struct Semaphore_* semaphore_ = self;

    // wait on cond to signal and decrease value
    struct Arg arg = {semaphore_, count, 0};
    semaphore_->cond->wait(semaphore_->cond, condition, &arg, critical, &arg);

    return 0;
}
int semaphore_timewait(struct Semaphore* self, int count, int timeout) {
    struct Semaphore_* semaphore_ = self;

    // timewait on cond to signal and decrease value
    struct Arg arg = {semaphore_, count, 0};
    int result = semaphore_->cond->timewait(semaphore_->cond, condition, &arg, critical, &arg, timeout);

    return result;
}
int semaphore_post(struct Semaphore* self, int count) {
    struct Semaphore_* semaphore_ = self;

    // signal on cond and increase value
    struct Arg arg = {semaphore_, count, 1};
    semaphore_->cond->signal(semaphore_->cond, critical, &arg);

    return 0;
}
int semaphore_get(struct Semaphore* self) {
    struct Semaphore_* semaphore_ = self;

    // get semaphore value
    int result = semaphore_->value;

    return result;
}

struct Semaphore* semaphore_new(int value) {
    struct Semaphore_* semaphore_ = memory_alloc(sizeof(struct Semaphore_));

    // init private methods
    semaphore_->self.wait = semaphore_wait;
    semaphore_->self.timewait = semaphore_timewait;
    semaphore_->self.post = semaphore_post;
    semaphore_->self.get = semaphore_get;

    // init internal Cond and Default Value
    semaphore_->cond = cond_new();
    semaphore_->value = value;

    return semaphore_;
}

void semaphore_free(struct Semaphore* semaphore) {
    struct Semaphore_* semaphore_ = semaphore;

    // destry internal Cond
    cond_free(semaphore_->cond);

    memory_free(semaphore_);
}