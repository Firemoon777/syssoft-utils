#ifndef _TASK_H_
#define _TASK_H_

#define AVG_ELEMENTS 3

const char* name = "/tmp/s207210_ipc";

struct server_info {
	pid_t pid;
	uid_t uid;
	gid_t gid;
	time_t diff;
	double loadavg[AVG_ELEMENTS];
};

#endif
