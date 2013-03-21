/*
 *   FILE: uthread_mtx.c 
 * AUTHOR: Peter Demoreuille
 *  DESCR: userland mutexes
 *   DATE: Sat Sep  8 12:40:00 2001
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "uthread.h"
#include "uthread_mtx.h"


/*
 * uthread_mtx_init
 *
 * Initialize the fields of the specified mutex.
 */
void
uthread_mtx_init(uthread_mtx_t *mtx)
{
        mtx->m_owner = NULL;
        utqueue_init(&mtx->m_waiters);
        //NOT_YET_IMPLEMENTED("UTHREADS: uthread_mtx_init");
}

/*
 * uthread_mtx_lock
 *
 * Lock the mutex.  This call will block if it's already locked.  When the
 * thread wakes up from waiting, it should own the mutex (see _unlock()).
 */
void
uthread_mtx_lock(uthread_mtx_t *mtx)
{
        if (mtx->m_owner) {
            ut_curthr->ut_state = UT_WAIT;
            utqueue_enqueue(&mtx->m_waiters, ut_curthr);
            uthread_block();
        }

        else {
            mtx->m_owner = ut_curthr;
        }
	//NOT_YET_IMPLEMENTED("UTHREADS: uthread_mtx_lock");
}


/*
 * uthread_mtx_trylock
 *
 * Try to lock the mutex, return 1 if we get the lock, 0 otherwise.
 * This call should not block.
 */
int
uthread_mtx_trylock(uthread_mtx_t *mtx)
{
    if (!mtx->m_owner) {
        mtx->m_owner = ut_curthr;
        return 1;
    }
    //NOT_YET_IMPLEMENTED("UTHREADS: uthread_mtx_trylock");
    return 0;
}


/*
 * uthread_mtx_unlock
 *
 * Unlock the mutex.  If there are people waiting to get this mutex,
 * explicitly hand off the ownership of the lock to a waiting thread and
 * then wake that thread.
 */
void
uthread_mtx_unlock(uthread_mtx_t *mtx)
{
    if (utqueue_empty(&mtx->m_waiters)) {
        mtx->m_owner = NULL;
    }
    else {
        uthread_t *next_thread = utqueue_dequeue(&mtx->m_waiters);
        mtx->m_owner = next_thread;
        uthread_wake(next_thread);
    }
    //NOT_YET_IMPLEMENTED("UTHREADS: uthread_mtx_unlock");
}










