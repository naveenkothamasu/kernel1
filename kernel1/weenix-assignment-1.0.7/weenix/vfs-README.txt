==================================================================================================================================================

Submitted By: kothamas@usc.edu, marella@usc.edu
Date  	    : 2013-11-15
==================================================================================================================================================
Section C
It has been verified that the code written by us has been covered. The details are given below.

==================================================================================================================================================
1. kmain.c
idleproc_run(): to run any of the below test cases, the code written here must be executed properly. So this will be indirectly 
		tested by below test cases.

2. vnode.c
special_file_read(): reading from the kshell
special_file_write(): writing to the kshell

3. namev.c
lookup()
dir_namev(): is tested indirectly by vfstest_read()
open_namev(): is tested indirectly by vfstest_read()


4. open.c
do_open(): is tested in vfstest_open()

5. vfs_syscall.c
do_read()	: vfstest_fd()
do_write()	: vfstest_fd()
do_close()	: vfstest_fd()
do_dup()	: vfstest_fd()
do_dup2()	: vfstest_fd()
do_mknod()	: 
do_mkdir()	: vfstest_mkdir()
do_rmdir()	: vfstest_open()
do_unlink()	: vfstest_open()
do_link()	: ****
do_rename()****
do_chdir()	: vfstest_read()
do_getdent()	: vfstest_fd()
do_lseek()	: vfstest_read()
do_stat()	: vfstest_stat()
==================================================================================================================================================
