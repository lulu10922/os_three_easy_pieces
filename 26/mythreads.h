#ifndef _MYTHREADS_H_
#define _MYTHREADS_H_

#include <pthread.h>

void Pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
void Pthread_join(pthread_t thread, void **vlueptr);
void Pthread_mutex_lock(pthread_mutex_t *mutex);
void Pthread_mutex_unlock(pthread_mutex_t *mutex);
void Pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);

#endif