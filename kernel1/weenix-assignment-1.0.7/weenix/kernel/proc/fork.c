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
	/*FIXME:From the help session : If mapping is private create a shadow object for both the parent and child and map them to the underlying
	memory object*/
	vmarea_t *vmareaParent,*vmareaChild;
	list_iterate_begin(&(curproc->p_vmmap->vmm_list), vmareaParent, vmarea_t, vma_plink){
		if(vmareaParent->vma_flags == MAP_PRIVATE){
			mmobj_t *Parent_shadowobj = shadow_create();
			Parent_shadowobj = vmareaParent->vma_obj;
			vmareaParent->vma_obj=Parent_shadowobj;
		}
		if(vmareaParent->vma_flags == MAP_SHARED){
			/*TODO:*/
		}
	}
/*TODO? Is this the correct way to do the mapping after fork! We called vmmap_clone above so that both the curproc and childproc will have the same values */
	list_iterate_begin(&(child->p_vmmap->vmm_list), vmareaChild, vmarea_t, vma_plink){
		if(vmareaChild->vma_flags == MAP_PRIVATE){
			mmobj_t *Child_shadowobj = shadow_create();
			Child_shadowobj = vmareaChild->vma_obj;
			vmareaChild->vma_obj=Child_shadowobj;
		}
		if(vmareaChild->vma_flags == MAP_SHARED){
			/*TODO:*/
		}
	}

	kthread_t *childthread=kthread_clone(curthr);
	childthread->kt_proc=child;
	list_insert_tail(&(child->p_threads), &(childthread->kt_plink));
	(childthread->kt_ctx).c_esp = fork_setup_stack(regs, childthread->kt_kstack);
        return 0;
}
