#include "types.h"
#include "globals.h"
#include "errno.h"

#include "util/debug.h"
#include "util/string.h"

#include "proc/proc.h"
#include "proc/kthread.h"

#include "mm/mm.h"
#include "mm/mman.h"
#include "mm/page.h"
#include "mm/pframe.h"
#include "mm/mmobj.h"
#include "mm/pagetable.h"
#include "mm/tlb.h"

#include "fs/file.h"
#include "fs/vnode.h"

#include "vm/shadow.h"
#include "vm/vmmap.h"

#include "api/exec.h"

#include "main/interrupt.h"

/* Pushes the appropriate things onto the kernel stack of a newly forked thread
 * so that it can begin execution in userland_entry.
 * regs: registers the new thread should have on execution
 * kstack: location of the new thread's kernel stack
 * Returns the new stack pointer on success. */
static uint32_t
fork_setup_stack(const regs_t *regs, void *kstack)
{
        /* Pointer argument and dummy return address, and userland dummy return
         * address */
        uint32_t esp = ((uint32_t) kstack) + DEFAULT_STACK_SIZE - (sizeof(regs_t) + 12);
        *(void **)(esp + 4) = (void *)(esp + 8); /* Set the argument to point to location of struct on stack */
        memcpy((void *)(esp + 8), regs, sizeof(regs_t)); /* Copy over struct */
        return esp;
}


/*
 * The implementation of fork(2). Once this works,
 * you're practically home free. This is what the
 * entirety of Weenix has been leading up to.
 * Go forth and conquer.
 */
int
do_fork(struct regs *regs)
{
        /*NOT_YET_IMPLEMENTED("VM: do_fork");*/
	proc_t *child = proc_create("child");
	if(child == NULL){
		return -1;
	}
	child->p_pproc = curproc->p_pproc;
	/*child->p_files = curproc->p_files;*/
	child->p_threads = curproc->p_threads;
	child->p_vmmap=vmmap_clone(curproc->p_vmmap);
	child->p_cwd=curproc->p_cwd;
	/* TODO? Shadow objects creation and check for the rules to create*/	
	kthread_t *childthread=kthread_create(child,NULL,0,NULL);
	childthread=kthread_clone(curthr);
	childthread->kt_proc=child;
	(childthread->kt_ctx).c_esp = fork_setup_stack(regs, childthread->kt_kstack);
        return 0;
}
