#ifndef __PING_H
#define __PING_H

void ping_signal(int pid, int sig_num);
void control_z(int sig);
void control_c(int sig);

#endif