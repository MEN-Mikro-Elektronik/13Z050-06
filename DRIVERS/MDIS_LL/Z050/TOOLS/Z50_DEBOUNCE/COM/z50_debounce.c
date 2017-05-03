/****************************************************************************
 ************                                                    ************
 ************                  Z50_DEBOUNCE                      ************
 ************                                                    ************
 ****************************************************************************/
/*!
 *         \file z50_debounce.c
 *       \author ulrich.bogensperger@men.de
 *        $Date: 2017/04/28 14:14:39 $
 *    $Revision: 1.2 $
 *
 *        \brief Demonstration of Z50 debouncing capabilities
 *               This assumes that every input is wired to its corresponding
 *               output (out0-->in0, etc.)
 *
 *     Required: libraries: mdis_api, usr_oss, usr_utl
 *     \switches (none)
 */
 /*-------------------------------[ History ]--------------------------------
 *
 * $Log: z50_debounce.c,v $
 * Revision 1.2  2017/04/28 14:14:39  DPfeuffer
 * R: MDIS5 driver required
 * M: driver ported to MDIS5
 *
 * Revision 1.1  2004/11/29 09:00:42  ub
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2004 by MEN mikro elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/

static const char RCSid[]="$Id: z50_debounce.c,v 1.2 2017/04/28 14:14:39 DPfeuffer Exp $";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <MEN/men_typs.h>
#include <MEN/usr_oss.h>
#include <MEN/usr_utl.h>
#include <MEN/mdis_api.h>
#include <MEN/z50_drv.h>

/*--------------------------------------+
|   DEFINES                             |
+--------------------------------------*/
#define FAIL_UNLESS(expression) \
 if( !(expression)) {\
     printf("\n*** Error during: %s\nfile %s\nline %d\n", \
      #expression,__FILE__,__LINE__);\
      printf("%s\n",M_errstring(UOS_ErrnoGet()));\
     goto ABORT;\
 }

#define FAIL_UNLESS_(expression) \
 if( !(expression)) {\
     printf("\n*** Error during: %s\nfile %s\nline %d\n", \
      #expression,__FILE__,__LINE__);\
     goto ABORT;\
 }

#define MAX_INP         16        /* number of inputs to check */

/*--------------------------------------+
|   GLOBALS                             |
+--------------------------------------*/
static int G_verbose = 0;

/*--------------------------------------+
|   PROTOTYPES                          |
+--------------------------------------*/
static void usage(void);
static void PrintMdisError(char *info);
static void printmsg( int level, char *fmt, ... );
static char* bitString( char *s, u_int32 val, u_int32 nrBits );
static int setDebounce( MDIS_PATH path, u_int32 input, u_int16 value );
static int getDebounce( MDIS_PATH path, u_int32 input, u_int16 *value );


/********************************* usage ***********************************/
/**  Print program usage
 */
static void usage(void)
{
    printf("Usage: z50_debounce [<opts>] <device> [<opts>]\n");
    printf("Function: Demonstration of Z50 debouncing capabilities\n");
    printf("Options:\n");
    printf("  device       device name..................... [none]    \n");
    printf("  -c=<chan>    channel number (0..3)........... [0]       \n");
    printf("  -v=<n>       verbosity (0..3)................ [0]       \n");
    printf("  -d           use D302i ...................... [no]      \n");
    printf("\n");
    printf("(c) 2004 by MEN mikro elektronik GmbH\n\n");
}

/********************************* main ************************************/
/** Program main function
 *
 *  \param argc       \IN  argument counter
 *  \param argv       \IN  argument vector
 *
 *  \return           success (0) or error (1)
 */
int main(int argc, char *argv[])
{
    int32     chan, i, timeref, useD302i;
    MDIS_PATH path=0;
    u_int32   oldInp=0, oldTime=0;
    char      *device,*str,*errstr,buf[40];
    u_int32	  n;

    /*--------------------+
    |  check arguments    |
    +--------------------*/
    if ((errstr = UTL_ILLIOPT("c=v=d?", buf))) { /* check args */
        printf("*** %s\n", errstr);
        return(1);
    }

    if (UTL_TSTOPT("?")) {                      /* help requested ? */
        usage();
        return(1);
    }

    /*--------------------+
    |  get arguments      |
    +--------------------*/
    for (device=NULL, i=1; i<argc; i++)
        if (*argv[i] != '-') {
            device = argv[i];
            break;
        }

    if (!device) {
        usage();
        return(1);
    }

    chan      = ((str = UTL_TSTOPT("c=")) ? atoi(str) : 0);
    G_verbose = ((str = UTL_TSTOPT("v=")) ? atoi(str) : 0);
    useD302i  = !!UTL_TSTOPT("d");

    /*--------------------+
    |  open path          |
    +--------------------*/
    if ((path = M_open(device)) < 0) {
        PrintMdisError("open");
        return(1);
    }

    /*--------------------+
    |  config             |
    +--------------------*/
    /* set current channel */
    if ((M_setstat(path, M_MK_CH_CURRENT, chan)) < 0) {
        PrintMdisError("setstat M_MK_CH_CURRENT");
        goto ABORT;
    }

    /*--------------------+
    |  print info         |
    +--------------------*/
    if( useD302i )
        printf("channel number: %d\n",chan);

    /*--------------------+
    |  test debouncing    |
    +--------------------*/
    /* clear outputs */
    M_write( path, 0 );
    UOS_Delay( 100 );

    if( useD302i ) {
        /*
         * on the D302i all inputs of one channel
         * share the same debouncing time
         */
        FAIL_UNLESS( setDebounce( path, 0, 20000 ) == 0 );   /* 1sec */
    }
    else {
        /*
         * debouncing for inputs 0..31:
         * input 0: 100msec, 1:200msec, ... 31: 3200msec
         */

        for( n=0; n<MAX_INP; ++n ) {
            u_int16 dbValue;

            FAIL_UNLESS( setDebounce( path, n, (u_int16)((n+1) * 2000) ) == 0 );
            FAIL_UNLESS( getDebounce( path, n, &dbValue ) == 0 );
            printmsg( 1, "Input %d: debounce value = %d\n", n, dbValue );
        }
    }

    /* now set all outputs and watch the inputs changing one by one */
    M_write( path, 0xffffffff );
    timeref = UOS_MsecTimerGet();

    while( (UOS_MsecTimerGet() - timeref) < 2000 ) {
        u_int32 inp, time;
        char    buf[80];

        FAIL_UNLESS( M_read( path, &inp ) == 0 );

        if( inp != oldInp ) {
            time = UOS_MsecTimerGet() - timeref;
            printf( "After %4dmsec (+%3d): %s\n",
                    time, time - oldTime,
                    bitString( buf, inp, MAX_INP ) );
            oldInp = inp;
            oldTime = time;
        }
    }

    /*--------------------+
    |  cleanup            |
    +--------------------*/
 ABORT:
    if (M_close(path) < 0)
        PrintMdisError("close");

    return(0);
}

/********************************* PrintMdisError **************************/
/** Print MDIS error message
 *
 *  \param info       \IN  info string
*/
static void PrintMdisError(char *info)
{
    printf("*** can't %s: %s\n", info, M_errstring(UOS_ErrnoGet()));
}

/***************************************************************************/
static void printmsg( int level, char *fmt, ... )
{
    va_list ap;

    if( level <= G_verbose ){
        va_start(ap,fmt);
        vprintf( fmt, ap );
        va_end(ap);
    }
}

/**********************************************************************/
/** Convert an integer into its binary string representation
 *
 *  e.g. 0x4f is converted to "1 0 0 1 1 1 1 "
 *
 *  \param  s      \IN   pointer to where result is written
 *                       (needs room for 2 chars per binary digit)
 *  \param  val    \IN   value to convert
 *  \param  nrBits \IN   number of bits to convert (max 32)
 *
 *  \return pointer to resulting string
 */
static char* bitString( char *s, u_int32 val, u_int32 nrBits )
{
    u_int32 i, bit;

    *s = '\0';

    bit = 1 << (nrBits-1);

    for( i=0; i<nrBits; ++i, bit>>=1 ) {
        strcat( s, val & bit ? "1 " : "0 " );
    }

    return( s );
}

/**********************************************************************/
/** Set debouncer
 *
 *  \param  path   \IN   device path
 *  \param  input  \IN   input nbr
 *  \param  value  \IN   time to set
 *
 *  \return error code
 */
static int setDebounce( MDIS_PATH path, u_int32 input, u_int16 value )
{
    Z50_DEBOUNCE_PB db;
    M_SG_BLOCK      blk;

    db.input = input;
    db.time  = value;

    blk.size = sizeof( db );
    blk.data = (void*)&db;

    return( M_setstat( path, Z50_BLK_DEBOUNCE, (INT32_OR_64)&blk ) );
}

/**********************************************************************/
/** Get debouncer
 *
 *  \param  path   \IN   device path
 *  \param  input  \IN   input nbr
 *  \param  value  \OUT  read time
 *
 *  \return error code
 */
static int getDebounce( MDIS_PATH path, u_int32 input, u_int16 *value )
{
    int32 rc;

    Z50_DEBOUNCE_PB db;
    M_SG_BLOCK      blk;

    db.input = input;

    blk.size = sizeof( db );
    blk.data = (void*)&db;

    rc = M_getstat( path, Z50_BLK_DEBOUNCE, (int32*)&blk );

    if( rc == 0 )
        *value = (u_int16)db.time;

    return( rc );
}
