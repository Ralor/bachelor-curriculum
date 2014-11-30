#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <time.h>
#include <errno.h>
#include <stdint.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include <sched.h>
using namespace std;

struct reg_msg {
    uint16_t type;
    struct sigevent ev;
};
#define REG_MSG 1
#define MY_FIRST_SIGNAL 2
#define MY_SECOND_SIGNAL 3

#define _MY_PULSE_CODE ( _PULSE_CODE_MINAVAIL + 3 )

int timerCnt = 1;
timer_t timerArr[50];

int server_coid, self_coid;

void sendSignal(int msg_type) {
	struct reg_msg msg; msg.type = msg_type;
	
	SIGEV_PULSE_INIT( &msg.ev, self_coid, getprio(0), _MY_PULSE_CODE, 0 );
	
	if( MsgSend(server_coid, &msg, sizeof msg, NULL, 0) ) 
		cout << "Error sending message\n", exit( EXIT_FAILURE );
}

void firstTimerHandler() {
	sendSignal(MY_FIRST_SIGNAL);
	cout << "first Timer" << "\n";
}
void secondTimerHandler() {
	sendSignal(MY_SECOND_SIGNAL);
	cout << "second Timer" << "\n";
}

void timerHandler(int sig, siginfo_t *si, void *uc) {
	timer_t tmp = *static_cast<timer_t*>(si->si_value.sival_ptr);
	if( tmp == timerArr[1] )	
		firstTimerHandler();
	else
	if( tmp == timerArr[2] )
		secondTimerHandler();
	else
		cout << "Shit\n";
}

int makeTimer(timer_t *timerID, int expireMS, int intervalMS) {
	
	
	struct sigaction sa;	
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = &timerHandler;
	sigemptyset(&sa.sa_mask);
	if( -1 == sigaction(sigNo, &sa, NULL) ) 
		return -1;
	
	struct sigevent te;
	te.sigev_notify = SIGEV_SIGNAL;
	te.sigev_signo = SIGRTMIN;
	te.sigev_value.sival_ptr = timerID;
	timer_create(CLOCK_REALTIME, &te, timerID);
	
	struct itimerspec its;
	its.it_interval.tv_sec = intervalMS/1000;
	its.it_interval.tv_nsec = (intervalMS%1000) * 1000 * 1000;
	its.it_value.tv_sec = expireMS/1000;
	its.it_value.tv_nsec = (expireMS%1000) * 1000 * 1000;
	timer_settime(*timerID, 0, &its, NULL);
	
	return 0;
}

int main() {
	
	server_coid = name_open("MSG_RECEIVER", 0);
	while( -1 == server_coid ) 
		sleep(1), server_coid = name_open("MSG_RECEIVER", 0);
	
	cout << "server found\n";
	
	int chid = ChannelCreate(0);
	if( -1 == chid ) 
		cout << "Error creating channel\n", exit( EXIT_FAILURE );
		
	self_coid = ConnectAttach(0, 0, chid, _NTO_SIDE_CHANNEL, 0);
	if( -1 == self_coid )
		cout << "Error attaching channel\n", exit( EXIT_FAILURE );

	sendSignal(REG_MSG);
	
	if( -1 == makeTimer(timerArr+1, 2000, 3000)
	||  -1 == makeTimer(timerArr+2, 1000, 4400) ) 
			cout << "Error starting timers\n",exit( EXIT_FAILURE );
			
	while( 1 ) sleep(10);
	
	return 0;
}