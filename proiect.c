#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct proc_info
{
	uid_t user;
	pid_t pid;
	pid_t ppid;
	int nr_children;
	pid_t *pid_children;
	time_t time;
	long time_ns;
};

void dfs(pid_t pid, pid_t ppid)
{
	int i;

	struct proc_info *proc = (struct proc_info*)malloc(sizeof(struct proc_info));

	// nr. of children
	int nr = syscall(331, pid);
	
	proc->pid = pid;
	proc->ppid = ppid;
	proc->nr_children = nr;
	proc->pid_children = (pid_t *)malloc(sizeof(pid_t) * nr);

	int x = syscall(332, pid, proc);
	if (x == -1)
	{
		printf("Nu exista procesul cu pid = %d\n", pid);
		return;
	}

	printf("UID: %d, PID: %d, PPID: %d, Nr. of children: %d, TIME: %d:%ld\n",
		proc->user, proc->pid, proc->ppid, proc->nr_children, proc->time, proc->time_ns);

	for (i = 0; i<nr; i++)
	{
		dfs(proc->pid_children[i], pid);
	}
}

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		pid_t pid = atoi(argv[1]);
		dfs(pid, -1);
	}
	else
		printf("Programul primeste un pid ca parametru.\n");
	return 0;
}
