#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <algorithm>

#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/neutrino.h>
#include <time.h>
#include <unistd.h>

#include <errno.h>
using namespace std;

struct Message {
    char data[256];
    struct sigevent ev;
};
union RecievedMessage 
{
    struct Message client_msg;
    struct _pulse pulse;
    uint16_t type;
} recievedMessage;

bool timerShot;
void timerHandler(int signo) {
	timerShot = true;
}

void delayMS(int milliseconds) {
	
	timerShot = false;
	timer_t timerID;
	
	struct sigaction sa;	
	sa.sa_flags = 0;
	sa.sa_handler = &timerHandler;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGALRM, &sa, NULL);
	timer_create(CLOCK_REALTIME, NULL, &timerID);
	
	struct itimerspec its;
	its.it_interval.tv_sec = 0;
	its.it_interval.tv_nsec = 0;
	its.it_value.tv_sec = milliseconds/1000;
	its.it_value.tv_nsec = (milliseconds%1000) * 1000 * 1000;
	timer_settime(timerID, 0, &its, NULL);
}

int main() {
	
	cout << "Attaching name...\n";
	name_attach_t *att = name_attach(NULL,"MSG_RECEIVER",0);	
		
  	struct _msg_info msg_info;
	
	cout << "Awaiting for data\n";
	
	while( 1 ) {		
		 		
		 int rcvid = MsgReceive( att->chid, &recievedMessage, sizeof (recievedMessage), &msg_info );	
		 
		 if ( 0 == rcvid && recievedMessage.pulse.code == _PULSE_CODE_DISCONNECT ) {
    	 	cout << "Client detached\n";
			ConnectDetach( recievedMessage.pulse.scoid );
		    continue;
	     }
	     if ( recievedMessage.type == _IO_CONNECT ) {
		    cout << "Client attached\n";
		    MsgReply( rcvid, EOK, NULL, 0 );	       
		    continue;
	     }
	     if ( recievedMessage.type > _IO_BASE && recievedMessage.type <= _IO_MAX ) {
	       MsgError( rcvid, ENOSYS );
	       continue;
	     }
	     
	     char data[256];
	     memcpy(data, recievedMessage.client_msg.data, sizeof data);
	     cout << "Data recieved : " << data << "\n";
	     next_permutation(data, data + strlen(data));	     	     
	     delayMS(4750);	  
	     while( !timerShot ) sleep(1);   
	     cout << "Data processed : " << data << "\n";
	     MsgReply(rcvid, 0, data, sizeof data );
	}
	
	return 0;
}