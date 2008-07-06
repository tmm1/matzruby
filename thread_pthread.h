/**********************************************************************

  thread_pthread.h -

  $Author$

  Copyright (C) 2004-2007 Koichi Sasada

**********************************************************************/

#ifndef RUBY_THREAD_PTHREAD_H
#define RUBY_THREAD_PTHREAD_H

#include <pthread.h>
typedef pthread_t rb_thread_id_t;
typedef pthread_mutex_t rb_thread_lock_t;
typedef pthread_cond_t rb_thread_cond_t;
#define RB_THREAD_LOCK_INITIALIZER PTHREAD_MUTEX_INITIALIZER

typedef struct native_thread_data_struct {
    void *signal_thread_list;
    pthread_cond_t sleep_cond;
} native_thread_data_t;

#define ruby_native_thread_lock(lock) pthread_mutex_lock(lock)
#define ruby_native_thread_unlock(lock) pthread_mutex_unlock(lock)

#endif /* RUBY_THREAD_PTHREAD_H */
