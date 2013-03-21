/*
 *   FILE: uthread_sched.c 
 * AUTHOR: Peter Demoreuille
 *  DESCR: scheduling wack for uthreads
 *   DATE: Mon Oct  1 00:19:51 2001
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "uthread.h"
#include "uthread_private.h"
#include "uthread_ctx.h"
#include "uthread_queue.h"
#include "uthread_bool.h"

/* ---------- globals -- */

/* Remove __attribute__((unused)) when you use this variable. */

static utqueue_t __attribute__((unused)) runq_table[UTH_MAXPRIO + 1];
/* priority runqueues */


/* ----------- public code -- */

void new_thread_enqueue(uthread_t *uthr) {
    utqueue_enqueue(&runq_table[uthr->ut_prio],uthr);
}

/*
 * uthread_yield
 *
 * Causes the currently running thread to yield use of the processor to
 * another thread. The thread is still runnable however, so it should
 * be in the UT_RUNNABLE state and schedulable by the scheduler. When this
 * function returns, the thread should be executing again. A bit more clearly,
 * when this function is called, the current thread stops executing for some
 * period of time (allowing another thread to execute). Then, when the time
 * is right (ie when a call to uthread_switch() results in this thread
 * being swapped in), the function returns.
 */
void
uthread_yield(void)
{
        ut_curthr->ut_state = UT_RUNNABLE;
        utqueue_enqueue(&runq_table[ut_curthr->ut_prio], ut_curthr);
        uthread_switch();
	//NOT_YET_IMPLEMENTED("UTHREADS: uthread_yieldUTH_MAXPRIO");
}

/*
 * uthread_block
 *
 * Put the current thread to sleep, pending an appropriate call to 
 * uthread_wake().
 */
void
uthread_block(void) 
{
        ut_curthr->ut_state = UT_WAIT;
        uthread_switch();
	//NOT_YET_IMPLEMENTED("UTHREADS: uthread_block");
}


/*
 * uthread_wake
 *
 * Wakes up the supplied thread (schedules it to be run again).  The
 * thread may already be runnable or (well, if uthreads allowed for
 * multiple cpus) already on cpu, so make sure to only mess with it if
 * it is actually in a wait state.
 */
void
uthread_wake(uthread_t *uthr)
{
    if ( uthr->ut_state == UT_WAIT ){
        uthr->ut_state = UT_RUNNABLE;
        utqueue_enqueue(&runq_table[uthr->ut_prio], uthr);
    }
    
    //NOT_YET_IMPLEMENTED("UTHREADS: uthread_wake");
}


/*
 * uthread_setprio
 *
 * Changes the priority of the indicated thread.  Note that if the thread
 * is in the UT_RUNNABLE state (it's runnable but not on cpu) you should
 * change the list it's waiting on so the effect of this call is
 * immediate.
 */
void
uthread_setprio(uthread_id_t id, int prio)
{

    if ( uthreads[id].ut_state == UT_RUNNABLE) {
        utqueue_remove(&runq_table[uthreads[id].ut_prio], &uthreads[id]);
        uthreads[id].ut_prio = prio;
        utqueue_enqueue(&runq_table[uthreads[id].ut_prio], &uthreads[id]);
        return;
    }
    uthreads[id].ut_prio = prio;
    NOT_YET_IMPLEMENTED("UTHREADS: uthread_setprio");
}



/* ----------- private code -- */
/*
 * uthread_switch()
 *
 * This is where all the magic is.  Wait until there is a runnable thread, and
 * then switch to it using uthread_swapcontext().  Make sure you pick the
 * highest priority runnable thread to switch to. Also don't forget to take
 * care of setting the ON_CPU thread state and the current thread. Note that
 * it is okay to switch back to the calling thread if it is the highest
 * priority runnable thread.
 *
 * Every time uthread_switch() is called, uthread_idle() should be called at
 * least once.  In addition, when there are no runnable threads, you should
 * repeatedly call uthread_idle() until there are runnable threads.  Threads
 * with numerically higher priorities run first. For example, a thread with
 * priority 8 will run before one with priority 3.
 * */

//  ??? not sure if this is right
void uthread_switch(void)
{
	// NOT_YET_IMPLEMENTED("UTHREADS: uthread_switch");
        uthread_t *runnable_highest_priority_thread;
        int waiting = 1;
        while (waiting) {
        uthread_idle();
        int i;
        for ( i = UTH_MAXPRIO; i >= 0; i--){
            if ( utqueue_empty(&runq_table[i]) != 1) {
                    runnable_highest_priority_thread = utqueue_dequeue(&runq_table[i]);
                    waiting = 0;
                    break;
                }
            }
        }
        runnable_highest_priority_thread->ut_state = UT_ON_CPU;
        uthread_t *old = ut_curthr;
        ut_curthr = runnable_highest_priority_thread;
        uthread_swapcontext(&old->ut_ctx,&runnable_highest_priority_thread->ut_ctx);
}

/*
 * uthread_sched_init
 *
 * Setup the scheduler. This is called once from uthread_init().
 */
void
uthread_sched_init(void)
{
    int i;
    for ( i = 0; i < UTH_MAXPRIO + 1; i++) {
        utqueue_init(&runq_table[i]);
    }
    // NOT_YET_IMPLEMENTED("UTHREADS: uthread_sched_init");
}





