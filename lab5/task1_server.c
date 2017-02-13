#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/loadavg.h>  /* getloadavg */
#include <string.h>       /* memset */

#include "task.h"


struct server_info save_info(void) {
	struct server_info info;
	info.pid = getpid();
	info.uid = getuid();
	info.gid = getgid();
	info.diff = 0;
	assert(getloadavg(info.loadavg, AVG_ELEMENTS));
	return info;	
}

int main(void) {
	struct server_info *info;
	time_t start_time = time(NULL);

	time_t cur_time;
	key_t key;
    int mem_id;

	/* create ipc file */
	close(open(name, O_RDWR | O_CREAT, 0644));
	/* generate ipc key */
	key = ftok(name, id);

	/* setup */
	mem_id = shmget(key, sizeof(info), IPC_CREAT | 0644);

	/* attach memory */
	info = (struct server_info*)shmat(mem_id, NULL, 0);

	*info = save_info();

	while(1) {
		cur_time = time(NULL);
		assert(cur_time > 0);

		info->diff = cur_time - start_time;
		assert(getloadavg(info->loadavg, AVG_ELEMENTS));
		sleep(1);
	}

	return 0;
}
