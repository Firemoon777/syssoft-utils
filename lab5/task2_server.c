#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/msg.h>
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
	struct msg msg;
	time_t start_time = time(NULL);

	time_t cur_time;
	key_t key;
    int msg_id;

	/* create ipc file */
	close(open(name, O_RDWR | O_CREAT, 0644));
	/* generate ipc key */
	key = ftok(name, id);

	/* setup message queue */
	msg_id = msgget(key, IPC_CREAT | 0666);

	msg.info = save_info();

	while(1) {
		cur_time = time(NULL);
		assert(cur_time > 0);

		msg.info.diff = cur_time - start_time;
		assert(getloadavg(msg.info.loadavg, AVG_ELEMENTS));
		msgsnd(msg_id, &msg, sizeof(msg), 0);
		sleep(1);
	}

	return 0;
}
