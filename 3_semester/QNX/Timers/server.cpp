#include <cstdlib>
#include <iostream>
#include <errno.h>
#include <stdint.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include <sched.h>
#include <unistd.h>
using namespace std;

struct reg_msg {
    uint16_t type;
    struct sigevent ev;
};
union recv_msgs 
{
    struct reg_msg client_msg;
    struct _pulse pulse;
    uint16_t type;
} recv_buf;

#define REG_MSG 1
#define MY_FIRST_SIGNAL 2
#define MY_SECOND_SIGNAL 3

int save_rcvid = 0;
int save_scoid = 0;
struct sigevent save_event;

int main() {

	name_attach_t *att = name_attach(NULL,"MSG_RECEIVER",0);
	if( att == NULL ) 
		cout << "Error attaching name, client won't find me\n", exit( EXIT_FAILURE );
	
	int rcvid;
  	struct _msg_info msg_info;
	
	while( 1 ) {		 
		
		 rcvid = MsgReceive( att->chid, &recv_buf, sizeof (recv_buf), &msg_info );		 
		 
    	 if( -1 == rcvid ) {
      		cout << "MsgRecieve failed!\n";
      		continue;
    	 }    	 
    	 
    	 if ( 0 == rcvid && recv_buf.pulse.code == _PULSE_CODE_DISCONNECT ) {
    	 	 cout << "Detaching client...\n";
			 if( recv_buf.pulse.scoid == save_scoid ) {            
				save_rcvid = 0;
			 	save_scoid = 0;
			 	cout << "Our client went away\n";
			 } else {
			 	cout << "Different client went away\n";	
			 }
			 ConnectDetach( recv_buf.pulse.scoid );
		     continue;
	     }
	     
	     if ( recv_buf.type == _IO_CONNECT ) {
	       cout << "Attaching client\n";
	       MsgReply( rcvid, EOK, NULL, 0 );	       
	       continue;
	     }	     
	     if ( recv_buf.type > _IO_BASE && recv_buf.type <= _IO_MAX ) {
	       MsgError( rcvid, ENOSYS );
	       continue;
	     }
	     
	     switch( recv_buf.type ) {
		     case REG_MSG:
		       cout << "Registering client " << rcvid << " ...\n";
		       if( save_rcvid ) {
		       	 cout << "I already have one!\n";
		         MsgError( rcvid, EBUSY );
		       } else {
		         save_rcvid = rcvid;
		         save_scoid = msg_info.scoid;
		         save_event = recv_buf.client_msg.ev;
		         MsgReply(rcvid, 0, NULL, 0 );
		       }
		       break;
		     case MY_FIRST_SIGNAL:
			   cout << "First timer ellapsed ("<< save_rcvid << ")\n";
			   MsgReply(rcvid, 0, NULL, 0 );
			   break;
			 case MY_SECOND_SIGNAL:
			   cout << "Second timer ellapsed ("<< save_rcvid << ")\n";
			   MsgReply(rcvid, 0, NULL, 0 );
			   break;  
		     default:
		       MsgError(rcvid, ENOSYS );
		       break;
	    }
	    
	}
	

	return 0;
}