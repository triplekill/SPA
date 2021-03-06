/*
 * Copyright (c) 2011 The Native Client Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * NativeClient pthread library semaphores API
 */

#ifndef NATIVE_CLIENT_SRC_UNTRUSTED_PTHREAD_NC_SEMPAHPORE_H_
#define NATIVE_CLIENT_SRC_UNTRUSTED_PTHREAD_NC_SEMPAHPORE_H_ 1

#include <sys/types.h>


/* A handle to a semaphore object */
typedef struct {
  int handle;
} sem_t;

/*
 * Maximum value the semaphore can have. Using OSX 10.5 value -
 * the lowest of all platforms.
 */
#define SEM_VALUE_MAX (32767)


#ifdef __cplusplus
extern "C" {
#endif

/* Initialize semaphore object SEM to VALUE.  If PSHARED then share it
   with other processes.  */
extern int sem_init(sem_t *sem, int pshared, unsigned int value);

/* Free resources associated with semaphore object SEM.  */
extern int sem_destroy(sem_t *sem);

/* Wait for SEM being posted.  */
extern int sem_wait(sem_t *sem);

/* TODO(gregoryd) - add support for sem_timedwait later */
#if 0
/* Similar to `sem_wait' but wait only until ABSTIME.  */
extern int sem_timedwait(sem_t *sem,
                         const struct timespec *abstime);
#endif

/* Test whether SEM is posted.  */
extern int sem_trywait(sem_t *sem);

/* Post SEM.  */
extern int sem_post(sem_t *sem);

/* Get current value of SEM and store it in *SVAL.  */
extern int sem_getvalue(sem_t *sem, int *sval);

#ifdef __cplusplus
}
#endif

#endif  /* NATIVE_CLIENT_SRC_UNTRUSTED_PTHREAD_NC_SEMPAHPORE_H_ */
