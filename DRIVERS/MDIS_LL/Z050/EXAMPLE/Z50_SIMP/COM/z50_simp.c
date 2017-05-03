/****************************************************************************
 ************                                                    ************
 ************                   Z50_SIMP                         ************
 ************                                                    ************
 ****************************************************************************/
/*!
 *         \file z50_simp.c
 *       \author ulrich.bogensperger@men.de
 *        $Date: 2017/04/28 14:14:37 $
 *    $Revision: 1.2 $
 *
 *       \brief  Simple example program for the Z50 driver
 *
 *               Reads and writes some values from/to GPIO ports,
 *               generating interrupts.
 *
 *     Required: libraries: mdis_api, usr_oss
 *     \switches (none)
 */
 /*-------------------------------[ History ]--------------------------------
 *
 * $Log: z50_simp.c,v $
 * Revision 1.2  2017/04/28 14:14:37  DPfeuffer
 * R: MDIS5 driver required
 * M: driver ported to MDIS5
 *
 * Revision 1.1  2004/11/29 09:00:40  ub
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2004 by MEN mikro elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/

static const char RCSid[]="$Id: z50_simp.c,v 1.2 2017/04/28 14:14:37 DPfeuffer Exp $";

#include <stdio.h>
#include <string.h>
#include <MEN/men_typs.h>
#include <MEN/mdis_api.h>
#include <MEN/usr_oss.h>
#include <MEN/z50_drv.h>

/*--------------------------------------+
|   DEFINES                             |
+--------------------------------------*/
#define DELAY           1000
#define MAXINP          5
#define BIT_SHIFT       0

/*--------------------------------------+
|   GLOBALS                             |
+--------------------------------------*/
static int G_sigCount = 0;

/*--------------------------------------+
|   PROTOTYPES                          |
+--------------------------------------*/
static void PrintError(char *info);
static void __MAPILIB SignalHandler( u_int32 sig );
static char* bitString( char *s, u_int32 val, u_int32 nrBits );
static int setDebounce( MDIS_PATH path, u_int32 input, u_int16 value );
static int getDebounce( MDIS_PATH path, u_int32 input, u_int16 *value );

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
    MDIS_PATH path;
    u_int32   inp;
    u_int16   dbValue;
    char      *device;
    char      buf1[40], buf2[40];
    int       i, maxCnt, bitShift=BIT_SHIFT;

    if (argc < 2 || strcmp(argv[1],"-?")==0) {
        printf("Syntax: z50_simp <device>\n");
        printf("Function: Example program for using the Z50 GPIO driver\n");
        printf("Options:\n");
        printf("    device       device name\n");
        printf("\n");
        return(1);
    }

    device = argv[1];

    /*--------------------+
    |  open path          |
    +--------------------*/
    if ((path = M_open(device)) < 0) {
        PrintError("open");
        return(1);
    }

    /*--------------------+
    |  config             |
    +--------------------*/
    /*
     * install handler for signal which will be received
     * at change of input ports
     */
    UOS_SigInit( SignalHandler );
    UOS_SigInstall( UOS_SIG_USR1 );
    M_setstat( path, Z50_SET_SIGNAL, UOS_SIG_USR1 );

    /* generate interrupts on rising edge of input 1 */
    M_setstat( path, Z50_IRQ_EDGE_RAISE, 0x00000002 );

    /* generate interrupts on falling edge of inputs 2 and 3 */
    M_setstat( path, Z50_IRQ_EDGE_FALL, 0x0000000c );

    /* set debouncing */
    setDebounce( path, 0, 100 );    /* 100*50usec */

    /* read back */
    getDebounce( path, 0, &dbValue );
    printf( "Debouncing set to %d\n", dbValue );

    if( bitShift )
        maxCnt = MAXINP;
    else
        maxCnt = 1<<MAXINP;

    /*--------------------+
    |  output values      |
    +--------------------*/
    for( i=0; i<maxCnt; ++i ) {
        u_int32 raise, fall, out;

        if( bitShift )
            out = 1 << i;
        else
            out = i;

        M_write( path, out );

        UOS_Delay( 10 );
        M_read( path, &inp );

        M_getstat( path, Z50_EDGE_RAISE, &raise );
        M_getstat( path, Z50_EDGE_FALL, &fall );

        printf( "%2d: Out: %s  In: %s (0x%04x)  raise: 0x%04x  fall: 0x%04x  "
                "IRQs: %d\n",
                out, bitString( buf1, out, 5 ), bitString( buf2, inp, 5 ),
                inp, raise, fall, G_sigCount );

        UOS_Delay( DELAY );
    }

    /* clear ports 0 and 1 and read back values from input */
    M_setstat( path, Z50_CLR_PORTS, 0x0f );
    UOS_Delay( 1 );
    M_read( path, &inp );
    printf( "Inputs after clearing mask 0x0f: %s\n", bitString( buf1, inp, 8));

    /* set port 0 */
    M_setstat( path, Z50_SET_PORTS, 0xa5 );
    UOS_Delay( 10 );
    M_read( path, &inp );
    printf( "Inputs after setting mask 0xa5 : %s\n", bitString( buf1, inp, 8));


    /*--------------------+
    |  cleanup            |
    +--------------------*/
    M_setstat( path, Z50_CLR_SIGNAL, 0 );
    UOS_SigRemove( UOS_SIG_USR1 );
    UOS_SigExit();

    if (M_close(path) < 0)
        PrintError("close");

    return(0);
}

/********************************* PrintError ******************************/
/** Print MDIS error message
 *
 *  \param info       \IN  info string
*/
static void PrintError(char *info)
{
    printf("*** can't %s: %s\n", info, M_errstring(UOS_ErrnoGet()));
}

/**********************************************************************/
/** Signal handler
 *
 *  \param  sig    \IN   received signal
 */
static void __MAPILIB SignalHandler( u_int32 sig )
{
    if( sig == UOS_SIG_USR1 ) {
        ++G_sigCount;
    }
}

/**********************************************************************/
/** Convert an integer into its binary string representation
 *
 *  e.g. 0x4f is converted to "1 0 0 1 1 1 1 "
 *
 *  \param  s      \IN   pointer to where result is written
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
