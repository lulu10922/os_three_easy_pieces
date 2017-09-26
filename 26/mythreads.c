#include "mythreads.h"
#include <assert.h>
#include <pthread.h>

void Pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg)
{
	int rc = pthread_create(thread, attr, start_routine, arg);
	assert(rc == 0);
}

void Pthread_join(pthread_t thread, void **vlueptr)
{
	int rc = pthread_join(thread, vlueptr);
	assert(rc == 0);
}

void Pthread_mutex_lock(pthread_mutex_t *mutex)
{
	int rc = pthread_mutex_lock(mutex);
	assert(rc == 0);
}
void Pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	int rc = pthread_mutex_unlock(mutex);
	assert(rc == 0);
}

void Pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t * attr)
{
	int rc = pthread_mutex_init(mutex, attr);
	assert(rc == 0);
}
