
/* abmain.c */

/* exitButtonArm.c */
int exitButtonArm ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* sendRequestButtonArm.c */
void initConnection ( void );
void sendToServer ( void );
const char *getStr ( PtWidget_t *widget , long type );
int sendRequestButtonArm ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
