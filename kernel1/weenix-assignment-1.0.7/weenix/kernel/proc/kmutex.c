#include "globals.h"
#include "errno.h"

#include "util/debug.h"

#include "proc/kthread.h"
#include "proc/kmutex.h"

/*My includes*/
#include "mm/kmalloc.h"
#include "util/string.h"
/*
 * IMPORTANT: Mutexes can _NEVER_ be locked or unlocked from an
 * interrupt context. Mutexes are _ONLY_ lock or unlocked from a
 * thread context.
 */

void
kmutex_init(kmutex_t *mtx)
{
        /*NOT_YET_IMPLEMENTED("PROCS: kmutex_init");*/
        mtx = (kmutex_t *) kmalloc(sizeof(*mtx));
        if( mtx == NULL){
                /*fprintf(stderr, "unable to allocate memory\n")*/      
        }
        
        memset(mtx, '\0', sizeof(*mtx));        
}

/*
 * This should block the current thread (by sleeping on the mutex's
 * wait queue) if the mutex is already taken.
 *
 * No thread should ever try to lock a mutex it already has locked.
 */
void
kmutex_lock(kmutex_t *mtx)
{
        /*NOT_YET_IMPLEMENTED("PROCS: kmutex_lock");*/
        
        if(mtx->km_holder != NULL){
                /* mutex locked */
                list_insert_tail(&(mtx->km_waitq.tq_list), &curthr->kt_qlink);        
                (mtx->km_waitq.tq_size)++;
        }else{
                /* mutex is avalialbe */
                mtx->km_holder = curthr;
        }
}

/*
 * This should do the same as kmutex_lock, but use a cancellable sleep
 * instead.
 */
int
kmutex_lock_cancellable(kmutex_t *mtx)
{
        /*NOT_YET_IMPLEMENTED("PROCS: kmutex_lock_cancellable"); */
        if(mtx->km_holder != NULL){
                /* mutex locked */
                curthr->kt_cancelled = 1;       
                list_insert_tail(&(mtx->km_waitq.tq_list), &curthr->kt_qlink);        
                (mtx->km_waitq.tq_size)++;
        }else{
                /* mutex is avalialbe*/
                mtx->km_holder = curthr;
                return 0;
        }
        
        return 0;
        /*
        and -EINTR if
 * the sleep was cancelled and this thread does not hold the mutex      
        */
}

/*
 * If there are any threads waiting to take a lock on the mutex, one
 * should be woken up and given the lock.
 *
 * Note: This should _NOT_ be a blocking operation!
 *
 * Note: Don't forget to add the new owner of the mutex back to the
 * run queue.
 *
 * Note: Make sure that the thread on the head of the mutex's wait
 * queue becomes the new owner of the mutex.
 *
 * @param mtx the mutex to unlock
 */
void
kmutex_unlock(kmutex_t *mtx)
{
        /*NOT_YET_IMPLEMENTED("PROCS: kmutex_unlock");*/
	list_link_t *link = NULL;
	kthread_t *pThread = NULL;	
	if(list_empty(&(mtx->km_waitq.tq_list)) != 1){
		link = (mtx->km_waitq.tq_list).l_next;
		list_remove_head(&((mtx->km_waitq).tq_list)); /*this is kt_qlink of the thread*/
		pThread = list_item(link, kthread_t, kt_qlink);
		/*wake up the thread*/
		mtx->km_holder = pThread;
		/*add pThread to run queue*/ 
		pThread->kt_state = KT_RUN;
	}
}

