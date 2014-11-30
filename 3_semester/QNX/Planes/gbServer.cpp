#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cstring>

#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/neutrino.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
using namespace std;


struct KillData {
    int crashed;
    struct sigevent ev;
};
union KillMessage 
{
    struct KillData verdict;
    struct _pulse pulse;
    uint16_t type;
} killMessage;
struct CreateData {
    int sizeX;
    int sizeY;
    int sizeZ;
    struct sigevent ev;
};
union CreateMessage 
{
    struct CreateData data;
    struct _pulse pulse;
    uint16_t type;
} createMessage;

int alive = 1;

int sizeX, sizeY, sizeZ;

double posX, posY, posZ;
double dirX, dirY, dirZ, len;
void normalizeDir() {
	len = sqrt(dirX*dirX + dirY*dirY + dirZ*dirZ);
	dirX /= len; dirY /= len; dirZ /= len;
}

int sendData[3];

void updData(int crashed = 0 ) {
	if( !crashed ) {
		sendData[0] = (int)(posX+0.5);
		sendData[1] = (int)(posY+0.5);
		sendData[2] = (int)(posZ+0.5);
	} else {
		sendData[0] = -1;
		sendData[1] = -1;
		sendData[2] = -1;
	}
}

name_attach_t *att;

void * msgReciever(void *arg) {
	
	struct _msg_info msg_info;
	
	while( 1 ) {
		
		int rcvid = MsgReceive( att->chid, &killMessage, sizeof (killMessage), &msg_info );
		
		if ( 0 == rcvid ) {
    	 	printf("Client detached\n");
			ConnectDetach( killMessage.pulse.scoid );
		    exit(0);
	     }
		
		updData( killMessage.verdict.crashed );
		
		MsgReply(rcvid, 0, sendData, sizeof sendData );
		
		if( killMessage.verdict.crashed )
			alive = 0;		
	}
	
}

int main() {
	srand(unsigned(time(0))); rand(); rand();
	char sPref[] = "MSG_RECIEVER_";
	char sBuf[3], sName[111];
	int sNum = 0;
	do {
		memset(sName, 0, sizeof sName);
		memcpy(sName, sPref, sizeof sPref);
		itoa(sNum,sBuf,10);
		strcat(sName,sBuf);
		att = name_attach(NULL,sName,0);
		++sNum;
	} while( att == NULL );
	
	printf("%s is my att\n", sName);
	
	struct _msg_info msg_info;
	int rcvid = MsgReceive( att->chid, &createMessage, sizeof (createMessage), &msg_info );
	
	sizeX = createMessage.data.sizeX;
	sizeY = createMessage.data.sizeY;
	sizeZ = createMessage.data.sizeZ;
	
	printf("%d %d %d is required size\n", sizeX, sizeY, sizeZ);
	
	posX = rand() / double(RAND_MAX) * sizeX;
	posY = rand() / double(RAND_MAX) * sizeY;
	posZ = rand() / double(RAND_MAX) * sizeZ;
	
	printf("%.2lf %.2lf %.2lf is generated position\n", posX, posY, posZ);
		
	updData(); MsgReply( rcvid, 0, sendData, sizeof sendData );		
	
	double alpha = rand() / double(RAND_MAX) * 2 * 3.1415927;
	double beta = rand() / double(RAND_MAX) * 2 * 3.1415927 - 3.1415927;
	
	dirX = cos(beta) * cos(alpha); 
	dirY = cos(beta) * sin(alpha);
	dirZ = sin(beta);
	
	dirZ *= 0.1; // stop flying vertical)) 
	
	normalizeDir(); // if it's too short (it can't be too long)
	
	printf("%.2lf %.2lf %.2lf is generated direction\n", dirX, dirY, dirZ);
	
	pthread_t reciever;
	pthread_create(&reciever, NULL, msgReciever, NULL);

	double timeSpeed = 0.12345; // chosen by fair dice roll
	while( alive ) {
		posX += dirX * timeSpeed;
		posY += dirY * timeSpeed;
		posZ += dirZ * timeSpeed;
		usleep(50*1000);
	}
	
	return 0;
}