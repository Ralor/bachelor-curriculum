// gcc GraphicApp.c -o GraphicApp.out -lm -lph 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

// graphics
#include <Pt.h>
#include <photon/PtTimer.h>

// data exchange
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/neutrino.h>

struct KillData {
    int crashed;
    struct sigevent ev;
} kData;
struct CreateData {
    int sizeX;
    int sizeY;
    int sizeZ;
    struct sigevent ev;
} cData;
int tmp;
int r,y,w;
int smth;
#define SQR(x) ((x)*(x))
#define DIST(fx,fy,fz,sx,sy,sz) sqrt(0.0 + SQR(fx-sx) + SQR(fy-sy) + SQR(fz-sz))
#define SWAP(a,b) tmp=a,a=b,b=tmp
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

int i,j,k;
int planesCount;
int sizeX, sizeY, sizeZ;
int circleR = 20, scale = 30;
int prevX[100], prevY[100], prevZ[100];
int curX[100], curY[100], curZ[100];
int color[100], any = 1, one_and_only;
const int white = Pg_WHITE;
const int yellow = Pg_YELLOW;
const int red = Pg_RED;
const int undefCoord = -500;

short getRealR(int ind) {
	tmp = (int)(circleR * 1.0 * curZ[ind] / sizeZ + 0.5);
	tmp = MAX(tmp,10);
	//tmp = MIN(tmp,100); // no upper bounds for radius
	return (short)tmp;;
}
int inField(int ind) {
	return curX[ind] > 0 && (curX[ind] + getRealR(ind)) < sizeX
	 	&& curY[ind] > 0 && (curY[ind] + getRealR(ind)) < sizeY
	 	&& curZ[ind] > 0;// && (curZ[ind] + 2*circleR) < sizeZ; // fly away if you want
}

PhDim_t      dim;
PhArea_t area; 
PtArg_t      args[3];
PtWidget_t * window;
PtWidget_t * timer;
PtWidget_t * circle[100];
int *pInt;

int server_coid[100];
int self_coid[100];
int chid;
// Every server can give me just this
int respData[3];

void initCoords(int ind) {
	curX[i] = respData[0];
	curY[i] = respData[1];
	curZ[i] = respData[2];	
}
void nextStep() {
	for(i = 0; i < planesCount; i++)
		prevX[i] = curX[i], prevY[i] = curY[i], prevZ[i] = curZ[i]; 	
}
void renderFrame() {
	for(i = 0; i < planesCount; i++) {
		area.pos.x = (short)curX[i];
		area.pos.y = (short)curY[i];
		area.size.h = area.size.w = getRealR(i);
		PtSetArg(&args[0], Pt_ARG_COLOR, color[i], 0);
		PtSetArg(&args[1], Pt_ARG_AREA, &area, 0);		
		PtSetResources(circle[i], 2, args);
	}
}
int segmentIntersect(int fir, int sec) {	
	//printf("%.2lf is dist\n");
	//printf("%.2lf is radius\n");
	return (0.5 * ( getRealR(fir) + getRealR(sec) ) 
		>= DIST(curX[fir], curY[fir], curZ[fir], curX[sec], curY[sec], curZ[sec]));
}
int segmentMayIntersect(int fir, int sec) {
	return (2.5 * ( getRealR(fir) + getRealR(sec) ) 
		>= DIST(curX[fir], curY[fir], curZ[fir], curX[sec], curY[sec], curZ[sec]));
}

int timerCallback(PtWidget_t *widget, void *userdata, PtCallbackInfo_t *info) {
	widget = widget, userdata = userdata, info = info;			
	//curInd = (int)userdata;	
	
	// get new positions
	for(i = 0; i < planesCount; i++) {
		kData.crashed = (color[i] == red || prevX[i] == undefCoord);
		SIGEV_PULSE_INIT( &kData.ev, self_coid[i], getprio(0), _PULSE_CODE_MINAVAIL + 3, 0 );	
		if( -1 != MsgSend(server_coid[i], &kData, sizeof kData, respData, sizeof respData) )
			initCoords(i);
		else
			curX[i] = curY[i] = curZ[i] = undefCoord; // connection lost
	}	
	
	if( !any ) exit(0);
	
	any = 0;
	for(i = 0; i < planesCount ; i++)
		any += (color[i] != red), one_and_only = i;
	if( any == 1 )
		color[one_and_only] = white;
	
	for(i = 0; i < planesCount; i++) {
		if( prevX[i] == undefCoord ) {
			color[i] = red, printf("%d on the ground\n");
		} else if( !inField(i) ) {
			color[i] = red, printf("%d out of field\n",i);
		}
	}
	
	for(i = 0; i < planesCount; i++) {
		if( prevX[i] == undefCoord )
			continue;	
		r = (color[i] == red);		
		for(j = 0; j < planesCount && !r; j++)
			if( i != j && prevX[j] != undefCoord )
				r += segmentIntersect(i,j);
		if( r ) {
			printf("%d crashed\n", i);
			color[i] = red; continue;
		}
		y = 0;
		for(j = 0; j < planesCount && !y; j++)
			if( i != j && prevX[j] != undefCoord )
				y += segmentMayIntersect(i,j);
		if( y ) {
			printf("%d in danger\n", i);
			color[i] = yellow; continue;
		}
		color[i] = white;
	}
		
	for(i = 0; i < planesCount; i++)
		printf("%d %d %d\n", curX[i], curY[i], curZ[i]); printf("\n");
	
	// crushed planes will be red
	renderFrame();
	for(i = 0; i < planesCount; i++)
		if( color[i] == red )
			curX[i] = curY[i] = curZ[i] = undefCoord;
	nextStep();	
}


char attPref[] = "MSG_RECIEVER_";
char attName[100], bf[3];

int main() {
	freopen("input.txt","r",stdin);

	scanf("%d", &planesCount );
	scanf("%d%d%d", &sizeX, &sizeY, &sizeZ );
	
	for(i = 0; i < planesCount; i++) {
    	memset(attName,0,sizeof attName);
    	memcpy(attName,attPref,sizeof attPref);
    	itoa(i,bf,10);
    	strcat(attName, bf);
	    server_coid[i] = name_open(attName, 0);  
		while( -1 == server_coid[i] ) 
			usleep(60*1000), server_coid[i] = name_open(attName, 0);
		printf("%s found\n", attName);
		// creating channel	
		chid = ChannelCreate(0);
		// attaching channel		
		self_coid[i] = ConnectAttach(0, 0, chid, _NTO_SIDE_CHANNEL, 0);    	
    }
    
    // preparing graphic
	if( PtInit(NULL) == -1 )
		exit(EXIT_FAILURE);	
	// creating main window
	PtSetArg( &args[0], Pt_ARG_WINDOW_TITLE, "Aeroport", 0);	
	dim.w = sizeX;
    dim.h = sizeY;
	PtSetArg( &args[1], Pt_ARG_DIM, &dim, 0);
	if( (window = PtCreateWidget(PtWindow, Pt_NO_PARENT, 2, args)) == NULL )
		PtExit(EXIT_FAILURE);
    
    for(i = 0; i < planesCount; i++) 
		color[i] = white, circle[i] = PtCreateWidget(PtEllipse, window, 0, NULL);
    
    cData.sizeX = sizeX;
    cData.sizeY = sizeY;
    cData.sizeZ = sizeZ;
    
    for(i = 0; i < planesCount; i++) {
    	SIGEV_PULSE_INIT( &cData.ev, self_coid[i], getprio(0), _PULSE_CODE_MINAVAIL + 3, 0 );		
		if( MsgSend(server_coid[i], &cData, sizeof cData, respData, sizeof respData) )
			exit(0);
		initCoords(i);
    }       
    
	renderFrame();
	nextStep();
		
	//pInt = malloc(sizeof(int)); *pInt = i;
	PtSetArg( &args[0], Pt_ARG_TIMER_INITIAL, 1000, 0);
	PtSetArg( &args[1], Pt_ARG_TIMER_REPEAT, 1000, 0);
	//PtSetArg( &args[2], Pt_ARG_USER_DATA, (void*)pInt, 0);
	if( (timer = PtCreateWidget(PtTimer, window, 2, args)) == NULL )	
		PtExit(EXIT_FAILURE);
	PtAddCallback( timer, Pt_CB_TIMER_ACTIVATE, timerCallback, NULL);
	
	// showing window and it's childrens
	PtRealizeWidget( window );
	// starting photon library
	PtMainLoop();	
	
	return 0;
}