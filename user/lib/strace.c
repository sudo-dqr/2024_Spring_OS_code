#include <lib.h>

void strace_barrier(u_int env_id) {
	int straced_bak = straced;
	straced = 0;
	while (envs[ENVX(env_id)].env_status == ENV_RUNNABLE) {
		syscall_yield();
	}
	straced = straced_bak;
}

void strace_send(int sysno) {
	if (!((SYS_putchar <= sysno && sysno <= SYS_set_tlb_mod_entry) ||
	      (SYS_exofork <= sysno && sysno <= SYS_panic)) ||
	    sysno == SYS_set_trapframe) {
		return;
	}

	// Your code here. (1/2)
	if (straced != 0) {
		int temp = straced;
		straced = 0;
		//struct Env * e;
		//try(env2envid(0,&e,1));
		int parentid = env->env_parent_id;
		ipc_send(parentid, sysno, 0,0);
		syscall_set_env_status(env->env_id,ENV_NOT_RUNNABLE);
		// 要不要从队列中删除
		straced = temp;
	}

}

void strace_recv() {
	// Your code here. (2/2)
	while(1) {
		u_int child_id;
		int sysno = ipc_recv(&child_id,0,0);
		strace_barrier(child_id);
		recv_sysno(child_id,sysno);
		syscall_set_env_status(child_id,ENV_RUNNABLE);
		if (sysno == SYS_env_destroy) {
			break;
		}
	}
}
