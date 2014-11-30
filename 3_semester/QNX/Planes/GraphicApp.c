// gcc GraphicApp.c -o GraphicApp.out -lphrender -lph

#include <stdlib.h>
#include <mem.h>
#include <photon/PhRender.h>
#include <Pt.h>

void
create_image( PhImage_t *image, PhDim_t *dim)
{
	PhPoint_t translation = {0, 0};
	PhPoint_t center;
	PhPoint_t radii;
	PmMemoryContext_t *mc;
	
	mc = PmMemCreateMC( image, dim, &translation);
	
	PmMemStart( mc );
	
	center.x = dim->w / 2;
	center.y = dim->h / 2;
	radii = center;
	PgSetFillColor( Pg_WHITE );
	PgSetStrokeColor( Pg_RED );
	PgDrawEllipse( &center, &radii, Pg_DRAW_FILL_STROKE );
	PgSetStrokeColor( Pg_GREEN );
	PgDrawILine(0, 0, dim->w - 1, dim->h - 1);
	
	PmMemFlush( mc, image );
	PmMemStop( mc );
	
	PmMemReleaseMC( mc );	
}

int
main ( int argc, char *argv[] )
{

	PhArea_t    area = { {80,20}, {80,40} };
	PhDim_t     dim = {240, 80};
	PhImage_t   image;
	PtArg_t     args[3];
	PtWidget_t  *button, *window;
	short       bytes_per_pixel = 3;
	
	if( PtInit(NULL) == -1 )
		exit(EXIT_FAILURE);
	
	PtSetArg( &args[0], Pt_ARG_WINDOW_TITLE,
		"Memory Context Sample", 0);
	PtSetArg( &args[1], Pt_ARG_DIM, &dim, 0);
	if( (window = PtCreateWidget(PtWindow, Pt_NO_PARENT, 2, args)) == NULL )
		PtExit(EXIT_FAILURE);
	
	memset( &image, 0, sizeof(PhImage_t) );
	image.type = Pg_IMAGE_DIRECT_888;
	image.size = dim;
	image.image = PgShmemCreate(
					dim.w * dim.h * bytes_per_pixel,
					NULL);
	create_image( &image, &area.size );
	 
	PtSetArg( &args[0], Pt_ARG_LABEL_TYPE, Pt_IMAGE, 0);
	PtSetArg( &args[1], Pt_ARG_AREA, &area, 0);
	PtSetArg( &args[2], Pt_ARG_LABEL_IMAGE, &image, 0);
	button = PtCreateWidget( PtButton, Pt_DEFAULT_PARENT, 3, args);
	
	PtRealizeWidget( window );

	PtMainLoop( );

	return 0;
}
