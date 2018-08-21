#include <low/concurrency/Semaphore.h>

#include <low/local/time/Time.h>
#include <pthread.h>
#include <stdint.h>

typedef struct Semaphore {
    int (*wait)(struct Semaphore* self);
    int (*timewait)(struct Semaphore* self, int timeout);
    int (*post)(struct Semaphore* self);
} Semaphore;

struct Semaphore* semaphore_new(int value);
void semaphore_free(struct Semaphore* semaphore);

struct Semaphore_ {
    struct Semaphore self;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int semaphore_value;
};

// link methods
int semaphore_wait(struct Semaphore* self, int count);
int semaphore_post(struct Semaphore* self, int count);
int semaphore_trywait(struct Semaphore* self, int count, int timeout);
int semaphore_get(struct Semaphore* self);

int semaphore_wait(struct Semaphore* self, int count) {
    struct Semaphore_* semaphore_ = self;

    // semaphore decrease value (lock into critical section and wait to decrease)
    pthread_mutex_lock(&(semaphore_->mutex));
    while (semaphore_->semaphore_value == 0) {
        pthread_cond_wait(&(semaphore_->cond), &(semaphore_->mutex));
    }
    semaphore_->semaphore_value--;
    pthread_mutex_unlock(&(semaphore_->mutex));

    return 0;
}
int semaphore_post(struct Semaphore* self, int count) {
    struct Semaphore_* semaphore_ = self;

    // semaphore increase value(lock into critical section and increase then signal)
    pthread_mutex_lock(&(semaphore_->mutex));
    semaphore_->semaphore_value++;
    pthread_cond_signal(&(semaphore_->cond));
    pthread_mutex_unlock(&(semaphore_->mutex));

    return 0;
}
int semaphore_trywait(struct Semaphore* self, int count, int timeout) {
    struct Semaphore_* semaphore_ = self;

    return 0;
}
int semaphore_get(struct Semaphore* self) {
    struct Semaphore_* semaphore_ = self;

    // get semaphore value
    int result = semaphore_->semaphore_value;

    return result;
}

struct Semaphore* semaphore_new(int value) {
    struct Semaphore_* semaphore_ = memory_alloc(sizeof(struct Semaphore_));

    // init private methods
    semaphore_->this.wait = semaphore_wait;
    semaphore_->this.post = semaphore_post;
    semaphore_->this.trywait = semaphore_trywait;
    semaphore_->this.get = semaphore_get;

    // init the core of semaphore (mutex and conditional variable)
    pthread_mutex_init(&(semaphore_->mutex), NULL);
    pthread_cond_init(&(semaphore_->cond), NULL);
    semaphore_->semaphore_value = value;

    return semaphore_;
}

void semaphore_free(struct Semaphore* semaphore) {
    struct Semaphore_* semaphore_ = semaphore;

    // destry mutex and conditional variable
    pthread_mutex_destroy(&(semaphore_->mutex));
    pthread_cond_destroy(&(semaphore_->cond));

    memory_free(semaphore_);
}