#include "globals.h"
#include "errno.h"
#include "util/debug.h"

#include "mm/mm.h"
#include "mm/page.h"
#include "mm/mman.h"

#include "vm/mmap.h"
#include "vm/vmmap.h"

#include "proc/proc.h"

/*
 * This function implements the brk(2) system call.
 *
 * This routine manages the calling process's "break" -- the ending address
 * of the process's "dynamic" region (often also referred to as the "heap").
 * The current value of a process's break is maintained in the 'p_brk' member
 * of the proc_t structure that represents the process in question.
 *
 * The 'p_brk' and 'p_start_brk' members of a proc_t struct are initialized
 * by the loader. 'p_start_brk' is subsequently never modified; it always
 * holds the initial value of the break. Note that the starting break is
 * not necessarily page aligned!
 *
 * 'p_start_brk' is the lower limit of 'p_brk' (that is, setting the break
 * to any value less than 'p_start_brk' should be disallowed).
 *
 * The upper limit of 'p_brk' is defined by the minimum of (1) the
 * starting address of the next occuring mapping or (2) USER_MEM_HIGH.
 * That is, growth of the process break is limited only in that it cannot
 * overlap with/expand into an existing mapping or beyond the region of
 * the address space allocated for use by userland. (note the presence of
 * the 'vmmap_is_range_empty' function).
 *
 * The dynamic region should always be represented by at most ONE vmarea.
 * Note that vmareas only have page granularity, you will need to take this
 * into account when deciding how to set the mappings if p_brk or p_start_brk
 * is not page aligned.
 *
 * You are guaranteed that the process data/bss region is non-empty.
 * That is, if the starting brk is not page-aligned, its page has
 * read/write permissions.
 *
 * If addr is NULL, you should NOT fail as the man page says. Instead,
 * "return" the current break. We use this to implement sbrk(0) without writing
 * a separate syscall. Look in user/libc/syscall.c if you're curious.
 *
 * Also, despite the statement on the manpage, you MUST support combined use
 * of brk and mmap in the same process.
 *
 * Note that this function "returns" the new break through the "ret" argument.
 * Return 0 on success, -errno on failure.
 */
int
do_brk(void *addr, void **ret)
{
        /*NOT_YET_IMPLEMENTED("VM: do_brk");*/
	uintptr_t *start_brk = (uintptr_t *) curproc->p_start_brk;
	uintptr_t *old = (uintptr_t *)curproc->p_brk;
	uintptr_t *next_map_addr;
	vmmap_t *map = curproc->p_vmmap;
	vmarea_t *vma;
	if(addr == NULL){
		*ret = old;
		return 0;
	}
	list_iterate_begin(&map->vmm_list, vma, vmarea_t, vma_plink){
		if(vma->vma_start > ADDR_TO_PN(addr) ){
			next_map_addr = (uintptr_t *) PN_TO_ADDR(vma->vma_start);
			break;
		}	
	}list_iterate_end();
	if( next_map_addr != NULL && *next_map_addr < USER_MEM_HIGH){
		*next_map_addr = ADDR_TO_PN(USER_MEM_HIGH);
	}
	if(next_map_addr != NULL &&
		 ( (uintptr_t *)addr < start_brk) || ( (uintptr_t *)addr > *next_map_addr) ){
		return -1;
	}
	
	if(!PAGE_ALIGNED(start_brk)){
		addr = PAGE_ALIGN_DOWN(start_brk);	
	}
	curproc->p_brk = addr;	
        return 0;
}
