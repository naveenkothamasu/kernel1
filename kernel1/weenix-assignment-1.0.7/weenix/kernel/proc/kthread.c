#include "config.h"
#include "globals.h"

#include "errno.h"

#include "util/init.h"
#include "util/debug.h"
#include "util/list.h"
#include "util/string.h"

#include "proc/kthread.h"
#include "proc/proc.h"
#include "proc/sched.h"

#include "mm/slab.h"
#include "mm/page.h"

kthread_t *curthr; /* global */
static slab_allocator_t *kthread_allocator = NULL;

#ifdef __MTP__
/* Stuff for the reaper daemon, which cleans up dead detached threads */
static proc_t *reapd = NULL;
static kthread_t *reapd_thr = NULL;
static ktqueue_t reapd_waitq;
static list_t kthread_reapd_deadlist; /* Threads to be cleaned */

static void *kthread_reapd_run(int arg1, void *arg2);
#endif

void
kthread_init()
{
        kthread_allocator = slab_allocator_create("kthread", sizeof(kthread_t));
        KASSERT(NULL != kthread_allocator);
}

/**
 * Allocates a new kernel stack.
 *
 * @return a newly allocated stack, or NULL if there is not enough
 * memory available
 */
static char *
alloc_stack(void)
{
        /* extra page for "magic" data */
        char *kstack;
        int npages = 1 + (DEFAULT_STACK_SIZE >> PAGE_SHIFT);
        kstack = (char *)page_alloc_n(npages);

        return kstack;
}

/**
 * Frees a stack allocated with alloc_stack.
 *
 * @param stack the stack to free
 */
static void
free_stack(char *stack)
{
        page_free_n(stack, 1 + (DEFAULT_STACK_SIZE >> PAGE_SHIFT));
}

/*
 * Allocate a new stack with the alloc_stack function. The size of the
 * stack is DEFAULT_STACK_SIZE.
 *
 * Don't forget to initialize the thread context with the
 * context_setup function. The context should have the same pagetable
 * pointer as the process.
 */
kthread_t *
kthread_create(struct proc *p, kthread_func_t func, long arg1, void *arg2)
{
        
        /*NOT_YET_IMPLEMENTED("PROCS: kthread_create");*/
	KASSERT(NULL != p); /* should have associated process */
	dbg_print("PASSED: should have associated process.\n");

        /*
	kthread_t *pThread = (kthread_t *) slab_allocator_create("kthread1" ,sizeof(kthread_t));  
	KASSERT(pThread != NULL && "ERROR: slab_allocator() failed, Unalbe to allocate memory for a thread struct.\n");
	*/
        context_t context; /*FIXME local variable fine?*/
	
        char *kstack=alloc_stack();
        context_setup(&context, func, arg1, arg2, kstack, DEFAULT_STACK_SIZE, p->p_pagedir);
	
	kthread_init();	
        curthr = (kthread_t *) kthread_allocator;
	curthr->kt_ctx = context;
        curthr->kt_kstack = kstack;
        curthr->kt_proc = p;
        curthr->kt_state = KT_NO_STATE; /* TODO: currently running or on runq */
	curthr->kt_wchan = NULL;
	curthr->kt_qlink.l_next = NULL;
	curthr->kt_qlink.l_prev = NULL;

	curthr->kt_plink.l_next = NULL;
	curthr->kt_plink.l_prev = NULL;
	/*curthr->kt_wchan = (ktqueue_t *)slab_allocator_create("kt_wchan", sizeof(ktqueue_t));
	KASSERT(curthr->kt_wchan != NULL);
       	sched_queue_init(curthr->kt_wchan); 
	*/
	/*FIXME:FIXME:pThread->kt_cancelled =*/
        list_insert_tail(&(p->p_threads), &(curthr->kt_plink)); 
	/*TODO kt_qlink*/
        KASSERT(curthr != NULL && "ERROR: kthread_create() failed.\n");
        return curthr;
}

void
kthread_destroy(kthread_t *t)
{
        KASSERT(t && t->kt_kstack);
        free_stack(t->kt_kstack);
        if (list_link_is_linked(&t->kt_plink))
                list_remove(&t->kt_plink);

        slab_obj_free(kthread_allocator, t);
}

/*
 * If the thread to be cancelled is the current thread, this is
 * equivalent to calling kthread_exit. Otherwise, the thread is
 * sleeping and we need to set the cancelled and retval fields of the
 * thread.
 *
 * If the thread's sleep is cancellable, cancelling the thread should
 * wake it up from sleep.
 *
 * If the thread's sleep is not cancellable, we do nothing else here.
 */
void
kthread_cancel(kthread_t *kthr, void *retval)
{
	KASSERT(NULL != kthr); /* should have thread */
	dbg_print("GRADING1 3.b PASSED: should have thread.\n");

        /*NOT_YET_IMPLEMENTED("PROCS: kthread_cancel");*/

        if(curthr == kthr){
                kthread_exit(retval);   
        }else{
	
		if(kthr->kt_state == KT_SLEEP_CANCELLABLE){
        		kthr->kt_retval = retval;
			kthr->kt_cancelled = 1;	
			sched_wakeup_on(kthr->kt_wchan);		
		}
	}
}

/*
 * You need to set the thread's retval field, set its state to
 * KT_EXITED, and alert the current process that a thread is exiting
 * via proc_thread_exited.
 *
 * It may seem unneccessary to push the work of cleaning up the thread
 * over to the process. However, if you implement MTP, a thread
 * exiting does not necessarily mean that the process needs to be
 * cleaned up.
 */
void
kthread_exit(void *retval)
{
        /*NOT_YET_IMPLEMENTED("PROCS: kthread_exit");*/
        curthr->kt_retval = retval;
        curthr->kt_state = KT_EXITED;
  	
	KASSERT(!curthr->kt_wchan); /* queue should be empty */
	dbg_print("GRADING1 3.c PASSED: queue should be empty.\n");
	
       	KASSERT(!curthr->kt_qlink.l_next && !curthr->kt_qlink.l_prev); /* queue should be empty */
	dbg_print("GRADING1 3.c PASSED: queue should be empty.\n");
        
	KASSERT(curthr->kt_proc == curproc);
	dbg_print("GRADING1 3.c PASSED: current process and the process associated with current thread are same.\n");
        
        proc_thread_exited(retval);
}

/*
 * The new thread will need its own context and stack. Think carefully
 * about which fields should be copied and which fields should be
 * freshly initialized.
 *
 * You do not need to worry about this until VM.
 */
kthread_t *
kthread_clone(kthread_t *thr)
{
        NOT_YET_IMPLEMENTED("VM: kthread_clone");
        return NULL;
}

/*
 * The following functions will be useful if you choose to implement
 * multiple kernel threads per process. This is strongly discouraged
 * unless your weenix is perfect.
 */
#ifdef __MTP__
int
kthread_detach(kthread_t *kthr)
{
        NOT_YET_IMPLEMENTED("MTP: kthread_detach");
        return 0;
}

int
kthread_join(kthread_t *kthr, void **retval)
{
        NOT_YET_IMPLEMENTED("MTP: kthread_join");
        return 0;
}

/* ------------------------------------------------------------------ */
/* -------------------------- REAPER DAEMON ------------------------- */
/* ------------------------------------------------------------------ */
static __attribute__((unused)) void
kthread_reapd_init()
{
        NOT_YET_IMPLEMENTED("MTP: kthread_reapd_init");
}
init_func(kthread_reapd_init);
init_depends(sched_init);

void
kthread_reapd_shutdown()
{
        NOT_YET_IMPLEMENTED("MTP: kthread_reapd_shutdown");
}

static void *
kthread_reapd_run(int arg1, void *arg2)
{
        NOT_YET_IMPLEMENTED("MTP: kthread_reapd_run");
        return (void *) 0;
}
#endif
