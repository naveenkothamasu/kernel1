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
	int i = 0;
	proc_t *child = proc_create("child");
	if(child == NULL){
		return -1;
	}
	child->p_vmmap=vmmap_clone(curproc->p_vmmap);
	dbginfo(DBG_VMMAP, vmmap_mapping_info, curproc->p_vmmap);
	list_link_t *pList = &(curproc->p_vmmap->vmm_list);
	list_link_t *cList = &(child->p_vmmap->vmm_list);
	list_link_t *pLink;
	list_link_t *cLink;
	vmarea_t *aParent;
	vmarea_t *aChild;	
	for(pLink = pList->l_next, cLink = cList->l_next;
		pList != pLink; pLink = pLink->l_next, cLink = cLink->l_next){
		
		aParent = list_item(pLink, vmarea_t, vma_plink);
		aChild = list_item(cLink, vmarea_t, vma_plink);
		aChild->vma_obj = aParent->vma_obj;
		/*aChild->vma_obj->mmo_ops->ref(aChild->vma_obj);*/
		/*pt_unmap_range(curproc->p_pagedir, PN_TO_ADDR(aParent->vma_start), PN_TO_ADDR(aParent->vma_end));*/
		tlb_flush_all();
	}
	for(; i < NFILES; i++){
		/*fref(child->p_files[i]);*/
		child->p_files[i] = curproc->p_files[i];
	}
	child->p_cwd=curproc->p_cwd;
	/*vref(child->p_cwd);*/
	/* TODO? Shadow objects creation and check for the rules to create*/
	/*FIXME:From the help session : If mapping is private create a shadow object for both the parent and child and map them to the underlying
	memory object*/
	/*
	vmarea_t *vmareaParent,*vmareaChild;
	list_iterate_begin(&(curproc->p_vmmap->vmm_list), vmareaParent, vmarea_t, vma_plink){
		if(vmareaParent->vma_flags == MAP_PRIVATE){
			mmobj_t *Parent_shadowobj = shadow_create();
			Parent_shadowobj = vmareaParent->vma_obj;
			vmareaParent->vma_obj=Parent_shadowobj;
			
			pt_unmap_range(curproc->p_pagedir,vmareaParent->vma_start, vmareaParent->vma_end);
			tlb_flush_all();
		}
		if(vmareaParent->vma_flags == MAP_SHARED){
		}
	}list_iterate_end();
	*/
/*TODO? Is this the correct way to do the mapping after fork! We called vmmap_clone above so that both the curproc and childproc will have the same values */
	/*
	list_iterate_begin(&(child->p_vmmap->vmm_list), vmareaChild, vmarea_t, vma_plink){
		if(vmareaChild->vma_flags == MAP_PRIVATE){
			mmobj_t *Child_shadowobj = shadow_create();
			Child_shadowobj = vmareaChild->vma_obj;
			vmareaChild->vma_obj=Child_shadowobj;
		}
		if(vmareaChild->vma_flags == MAP_SHARED){
		}
	}list_iterate_end();
	*/
	/*TODO? How does fork know which one to return?return parent pid or zero*/
	kthread_t *childthread=kthread_clone(curthr);
	childthread->kt_proc=child;
	list_insert_tail(&(child->p_threads), &(childthread->kt_plink));
	(childthread->kt_ctx).c_eip = (uint32_t)userland_entry;
	regs->r_eax = 0;
	(childthread->kt_ctx).c_esp = fork_setup_stack(regs, childthread->kt_kstack); 
	(childthread->kt_ctx).c_pdptr = childthread->kt_proc->p_pagedir;
        (childthread->kt_ctx).c_kstack = (uintptr_t)childthread->kt_kstack;
 	(childthread->kt_ctx).c_kstacksz = DEFAULT_STACK_SIZE;

        /*context_setup(&childthread->kt_ctx, , 0, NULL, childthread->kt_kstack, DEFAULT_STACK_SIZE, childthread->kt_proc->p_pagedir);*/

	regs->r_eax = child->p_pid;
	sched_make_runnable(childthread);
	
	return child->p_pid;
}
