/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.03  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* CS headers */
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/neutrino.h>

/* Local headers */
#include "ablibs.h"
#include "abimport.h"
#include "proto.h"

const char noConnect[] = "Establishing connection...";
const char waitForResponse[] = "Awaiting server response...";

int server_coid = -1, self_coid = -1;

struct Message {
    char data[256];
    struct sigevent ev;
};
int initiated = 0;
void initConnection() {
	initiated = 1;
	server_coid = name_open("MSG_RECEIVER", 0);
	while( -1 == server_coid ) 
		server_coid = name_open("MSG_RECEIVER", 0);	
	if( self_coid == -1 ) {
		int chid = ChannelCreate(0);
		self_coid = ConnectAttach(0, 0, chid, _NTO_SIDE_CHANNEL, 0);
	}	
}

char data[256];
char res[256];
void sendToServer() {	
	struct Message msg; 
	strcpy(msg.data, data);
	SIGEV_PULSE_INIT( &msg.ev, self_coid, getprio(0), _PULSE_CODE_MINAVAIL + 3, 0 );		
	if( MsgSend(server_coid, &msg, sizeof msg, res, sizeof res) ) { 
		initiated = 0;
		PtSetResource(ABW_OutputString, Pt_ARG_TEXT_STRING, noConnect, sizeof noConnect);
		initConnection();
		sendToServer();
	}
	strcpy(data, res);	
}

const char *getStr(PtWidget_t *widget, long type) {
	PtArg_t arg;
	PtSetArg(&arg, type, 0, 0);
	PtGetResources(widget, 1, &arg);
	return (char*)arg.value;	
}

char *userText;
PtArg_t arg;

int
sendRequestButtonArm( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo ) {

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	
	userText = getStr(ABW_InputString, Pt_ARG_TEXT_STRING);
	PtSetResource(ABW_OutputString, Pt_ARG_TEXT_STRING, waitForResponse, sizeof waitForResponse);
	sleep(1);	
	strcpy(data, userText);
	sendToServer();
	PtSetResource(ABW_OutputString, Pt_ARG_TEXT_STRING, data, sizeof data);
	
	return( Pt_CONTINUE );
}

