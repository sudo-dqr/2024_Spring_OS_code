// exam
static int barrier = -1, barriered_cnt = 0;
static u_int barriered_env[64];
void sys_barrier_alloc(int n) {
	barrier = n;
}

void sys_barrier_wait(void) {
	if(barrier<0) return;
	//printk("env %x barrier wait, barrier %d remains.\n", curenv->env_id, barrier-barriered_cnt-1);
	barriered_env[barriered_cnt++] = curenv->env_id;
	curenv->env_status = ENV_NOT_RUNNABLE;
	TAILQ_REMOVE(&env_sched_list, curenv, env_sched_link);
	if(barriered_cnt >= barrier) {
		barrier = -1;
		for (int i = barriered_cnt - 1; i >= 0; i--) {
			struct Env *env;
			envid2env(barriered_env[i], &env, 0);
			env->env_status = ENV_RUNNABLE;
			TAILQ_INSERT_TAIL(&env_sched_list, env, env_sched_link);
		}
		barriered_cnt = 0;
	} else {
		schedule(1);
	}
}


// extra
#define NSEMA 10
struct Sema {
    char name[32];
    u_int owner;
    int value;
    int checkperm;
    int used;
    u_int envs[32];
    int envcnt;
};
static struct Sema semaphore[NSEMA] = {{.used = 0, .envcnt = 0}};

int sys_sem_init(const char *name, int init_value, int checkperm) {
	struct Sema *sem = NULL;
	for(struct Sema *i = semaphore; i - semaphore < NSEMA; i = i + 1) {
		if (!i->used) {
			sem = i;
			break;
		}
	}
	if (!sem) return -E_NO_SEM;
	sem->used = 1;
	strcpy(sem->name, name);
	sem->value = init_value;
	sem->checkperm = checkperm;
	sem->owner = curenv->env_id;
	return sem - semaphore;
}
extern struct Env envs[NENV];
static int valid_sem_perm(struct Sema *sem) {
	if (!sem->checkperm) return 1;
	struct Env *env = curenv;
	do {
		//printk("1 this is env %x, parent is %x.\n", env->env_id, env->env_parent_id);
		if(env->env_id == sem->owner) return 1;
		if(env->env_parent_id) {
			//int r = envid2env(env->env_parent_id, &env, 0);
			//if (r) break;
			env = &envs[ENVX(env->env_parent_id)];
		}
		else break;
		//printk("2 this is env %x, parent is %x.\n", env->env_id, env->env_parent_id);
	} while(1);
	return 0;
}
int sys_sem_wait(int sem_id) {
	if(sem_id < 0 || sem_id >= NSEMA) return -E_NO_SEM;
	if(!valid_sem_perm(semaphore + sem_id)) return -E_NO_SEM;
	struct Sema *sem = semaphore + sem_id;
	if(sem->value) --(sem->value);
	else {
		sem->envs[sem->envcnt++] = curenv->env_id;
		curenv->env_status = ENV_NOT_RUNNABLE;
		TAILQ_REMOVE(&env_sched_list, curenv, env_sched_link);
		((struct Trapframe *)KSTACKTOP - 1)->regs[2] = 0;
		schedule(1);
	}
	return 0;
}
int sys_sem_post(int sem_id) {
	if(sem_id < 0 || sem_id >= NSEMA) return -E_NO_SEM;
        if(!valid_sem_perm(semaphore + sem_id)) return -E_NO_SEM;
        struct Sema *sem = semaphore + sem_id;
	++(sem->value);
	if(sem->envcnt > 0) {
		--(sem->value);
		struct Env *env;
		envid2env(sem->envs[--(sem->envcnt)], &env, 0);
		env->env_status = ENV_RUNNABLE;
		TAILQ_INSERT_TAIL(&env_sched_list, env, env_sched_link);
	}
	return 0;
}
int sys_sem_getvalue(int sem_id) {
	if(sem_id < 0 || sem_id >= NSEMA) return -E_NO_SEM;
        if(!valid_sem_perm(semaphore + sem_id)) return -E_NO_SEM;
	return semaphore[sem_id].value;
}
int sys_sem_getid(const char *name) {
	for(int i = 0; i < NSEMA; i++) {
		if(!semaphore[i].used) continue;
		if(strcmp(name, semaphore[i].name) == 0 && valid_sem_perm(semaphore + i)) {
			return i;
		}
	}
	return -E_NO_SEM;
}
