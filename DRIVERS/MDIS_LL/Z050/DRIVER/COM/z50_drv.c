/*********************  P r o g r a m  -  M o d u l e ***********************/
/*!
 *        \file  z50_drv.c
 *
 *      \author  ulrich.bogensperger@men.de
 *        $Date: 2017/04/28 14:14:21 $
 *    $Revision: 1.2 $
 *
 *      \brief Low-level driver for MEN D302 board (Model "Infineon") and
 *      Binary Input Output Controller (BIOC) implemented in Chameleon FPGA
 *
 *     Required: OSS, DESC, DBG libraries
 *
 *     \switches _ONE_NAMESPACE_PER_DRIVER_
 */
 /*-------------------------------[ History ]--------------------------------
 *
 * $Log: z50_drv.c,v $
 * Revision 1.2  2017/04/28 14:14:21  DPfeuffer
 * R: MDIS5 driver required
 * M: driver ported to MDIS5
 *
 * Revision 1.1  2004/11/29 09:00:28  ub
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2004 by MEN Mikro Elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/

#define _NO_LL_HANDLE       /* ll_defs.h: don't define LL_HANDLE struct */

#include <MEN/men_typs.h>   /* system dependent definitions   */
#include <MEN/maccess.h>    /* hw access macros and types     */
#include <MEN/dbg.h>        /* debug functions                */
#include <MEN/oss.h>        /* oss functions                  */
#include <MEN/desc.h>       /* descriptor functions           */
#include <MEN/mdis_api.h>   /* MDIS global defs               */
#include <MEN/mdis_com.h>   /* MDIS common defs               */
#include <MEN/mdis_err.h>   /* MDIS error codes               */
#include <MEN/ll_defs.h>    /* low-level driver definitions   */

#include "d302i.h"          /* register definitions for D302 Infineon */


/*-----------------------------------------+
|  DEFINES                                 |
+-----------------------------------------*/
/* general defines */
#define CH_NUMBER           4           /**< number of device channels */
#define USE_IRQ             TRUE        /**< interrupt required  */
#define ADDRSPACE_COUNT     1           /**< nbr of required address spaces */
#define ADDRSPACE_SIZE      256         /**< size of address space */
#define OSH                 llHdl->osHdl

/* debug defines */
#define DBG_MYLEVEL         llHdl->dbgLevel   /**< debug level */
#define DBH                 llHdl->dbgHdl     /**< debug handle */

/* hardware defines */
#define BIOC_INPUT          0x00        /**< offset for input register */
#define BIOC_OUTPUT         0x04        /**< output register */
#define BIOC_EDGE_RAISE     0x08        /**< raising edge register */
#define BIOC_EDGE_FALL      0x0c        /**< falling edge register */
#define BIOC_IRQ_EDGE_RAISE 0x10        /**< irq raising edge register */
#define BIOC_IRQ_EDGE_FALL  0x14        /**< irq falling edge register */
#define BIOC_CONTROL        0x20        /**< control register */
#define BIOC_DEBOUNCE       0x80        /**< debouncing register */


/*-----------------------------------------+
|  TYPEDEFS                                |
+-----------------------------------------*/
/** low-level handle */
typedef struct {
    /* general */
    int32           memAlloc;       /**< size allocated for the handle */
    OSS_HANDLE      *osHdl;         /**< oss handle */
    OSS_IRQ_HANDLE  *irqHdl;        /**< irq handle */
    DESC_HANDLE     *descHdl;       /**< desc handle */
    MACCESS         ma;             /**< hw access handle */
    MDIS_IDENT_FUNCT_TBL idFuncTbl; /**< id function table */
    /* debug */
    u_int32         dbgLevel;       /**< debug level */
    DBG_HANDLE      *dbgHdl;        /**< debug handle */
    /* misc */
    OSS_SIG_HANDLE  *portChangeSig; /**< signal for port change */

    int32           d302i;          /**< support for D302 Infineon */
    u_int32         edgeRaiseIrq[CH_NUMBER]; /**< inputs which got raising edge */
    u_int32         edgeFallIrq[CH_NUMBER];  /**< inputs which got falling edge */

    u_int32         irqCount;       /**< interrupt counter */
    u_int32         inputReg;       /**< offset of input register */
    u_int32         outputReg;      /**< offset of output register */
    u_int32         edgeRaiseReg;   /**< offset of raising edge register */
    u_int32         edgeFallReg;    /**< offset of falling edge register */
    u_int32         irqEdgeRaiseReg;/**< offset of raising edge irq mask */
    u_int32         irqEdgeFallReg; /**< offset of falling edge irq mask */
    u_int32         debounceReg;    /**< offset of debounce register */
    u_int32         controlReg;     /**< offset of control register */
} LL_HANDLE;

/* include files which need LL_HANDLE */
#include <MEN/ll_entry.h>   /* low-level driver jump table  */
#include <MEN/z50_drv.h>    /* Z50 driver header file */

/*-----------------------------------------+
|  PROTOTYPES                              |
+-----------------------------------------*/
static int32 Z50_Init(DESC_SPEC *descSpec, OSS_HANDLE *osHdl,
                       MACCESS *ma, OSS_SEM_HANDLE *devSemHdl,
                       OSS_IRQ_HANDLE *irqHdl, LL_HANDLE **llHdlP);
static int32 Z50_Exit(LL_HANDLE **llHdlP );
static int32 Z50_Read(LL_HANDLE *llHdl, int32 ch, int32 *value);
static int32 Z50_Write(LL_HANDLE *llHdl, int32 ch, int32 value);
static int32 Z50_SetStat(LL_HANDLE *llHdl,int32 ch, int32 code, INT32_OR_64 value32_or_64);
static int32 Z50_GetStat(LL_HANDLE *llHdl, int32 ch, int32 code, INT32_OR_64 *value32_or_64P);
static int32 Z50_BlockRead(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size,
                            int32 *nbrRdBytesP);
static int32 Z50_BlockWrite(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size,
                             int32 *nbrWrBytesP);
static int32 Z50_Irq(LL_HANDLE *llHdl );
static int32 Z50_Info(int32 infoType, ... );

static char* Ident( void );
static int32 Cleanup(LL_HANDLE *llHdl, int32 retCode);
static void  gpioReset( LL_HANDLE *llHdl );


/****************************** Z50_GetEntry ********************************/
/** Initialize driver's jump table
 *
 *  \param drvP     \OUT pointer to the initialized jump table structure
 */
#ifdef _ONE_NAMESPACE_PER_DRIVER_
    extern void LL_GetEntry( LL_ENTRY* drvP )
#else
    extern void __Z50_GetEntry( LL_ENTRY* drvP )
#endif
{
    drvP->init        = Z50_Init;
    drvP->exit        = Z50_Exit;
    drvP->read        = Z50_Read;
    drvP->write       = Z50_Write;
    drvP->blockRead   = Z50_BlockRead;
    drvP->blockWrite  = Z50_BlockWrite;
    drvP->setStat     = Z50_SetStat;
    drvP->getStat     = Z50_GetStat;
    drvP->irq         = Z50_Irq;
    drvP->info        = Z50_Info;
}

/******************************** Z50_Init **********************************/
/** Allocate and return low-level handle, initialize hardware
 *
 * The function initializes all channels with the definitions made
 * in the descriptor. The interrupt is disabled.
 *
 * The following descriptor keys are used:
 *
 * \code
 * Descriptor key        Default          Range
 * --------------------  ---------------  -------------
 * DEBUG_LEVEL_DESC      OSS_DBG_DEFAULT  see dbg.h
 * DEBUG_LEVEL           OSS_DBG_DEFAULT  see dbg.h
 * ID_CHECK              1                0..1
 * \endcode
 *
 *  \param descP      \IN  pointer to descriptor data
 *  \param osHdl      \IN  oss handle
 *  \param ma         \IN  hw access handle
 *  \param devSemHdl  \IN  device semaphore handle
 *  \param irqHdl     \IN  irq handle
 *  \param llHdlP     \OUT pointer to low-level driver handle
 *
 *  \return           \c 0 on success or error code
 */
static int32 Z50_Init(
    DESC_SPEC       *descP,
    OSS_HANDLE      *osHdl,
    MACCESS         *ma,
    OSS_SEM_HANDLE  *devSemHdl,
    OSS_IRQ_HANDLE  *irqHdl,
    LL_HANDLE       **llHdlP
)
{
    LL_HANDLE *llHdl = NULL;
    u_int32 gotsize;
    int32 error;
    u_int32 value;

    /*------------------------------+
    |  prepare the handle           |
    +------------------------------*/
    *llHdlP = NULL;     /* set low-level driver handle to NULL */

    /* alloc */
    if ((llHdl = (LL_HANDLE*)OSS_MemGet(
                    osHdl, sizeof(LL_HANDLE), &gotsize)) == NULL)
       return(ERR_OSS_MEM_ALLOC);

    /* clear */
    OSS_MemFill(osHdl, gotsize, (char*)llHdl, 0x00);

    /* init */
    llHdl->memAlloc   = gotsize;
    llHdl->osHdl      = osHdl;
    llHdl->irqHdl     = irqHdl;
    llHdl->ma         = *ma;

    /*------------------------------+
    |  init id function table       |
    +------------------------------*/
    /* driver's ident function */
    llHdl->idFuncTbl.idCall[0].identCall = Ident;
    /* library's ident functions */
    llHdl->idFuncTbl.idCall[1].identCall = DESC_Ident;
    llHdl->idFuncTbl.idCall[2].identCall = OSS_Ident;
    /* terminator */
    llHdl->idFuncTbl.idCall[3].identCall = NULL;

    /*------------------------------+
    |  prepare debugging            |
    +------------------------------*/
    DBG_MYLEVEL = OSS_DBG_DEFAULT;  /* set OS specific debug level */
    DBGINIT((NULL,&DBH));

    /*------------------------------+
    |  scan descriptor              |
    +------------------------------*/
    /* prepare access */
    if ((error = DESC_Init(descP, osHdl, &llHdl->descHdl)))
        return( Cleanup(llHdl,error) );

    /* DEBUG_LEVEL_DESC */
    if ((error = DESC_GetUInt32(llHdl->descHdl, OSS_DBG_DEFAULT,
                                &value, "DEBUG_LEVEL_DESC")) &&
        error != ERR_DESC_KEY_NOTFOUND)
        return( Cleanup(llHdl,error) );

    DESC_DbgLevelSet(llHdl->descHdl, value);    /* set level */

    /* DEBUG_LEVEL */
    if ((error = DESC_GetUInt32(llHdl->descHdl, OSS_DBG_DEFAULT,
                                &llHdl->dbgLevel, "DEBUG_LEVEL")) &&
        error != ERR_DESC_KEY_NOTFOUND)
        return( Cleanup(llHdl,error) );

    /* Z50_D302I */
    if ((error = DESC_GetUInt32(llHdl->descHdl, 0,
                                &llHdl->d302i, "Z50_D302I")) &&
        error != ERR_DESC_KEY_NOTFOUND)
        return( Cleanup(llHdl,error) );

    /* setup register offsets depending on kind of hardware */
    if( llHdl->d302i ) {
        llHdl->inputReg        = D302I_INPUT(0);
        llHdl->outputReg       = D302I_OUTPUT(0);
        llHdl->edgeRaiseReg    = D302I_EDGE_RAISE(0);
        llHdl->edgeFallReg     = D302I_EDGE_FALL(0);
        llHdl->irqEdgeRaiseReg = D302I_IRQ_EDGE_RAISE(0);
        llHdl->irqEdgeFallReg  = D302I_IRQ_EDGE_FALL(0);
        llHdl->debounceReg     = D302I_DEBOUNCE(0);
        llHdl->controlReg      = 0;                    /* not present */
    }
    else {
        llHdl->inputReg        = BIOC_INPUT;
        llHdl->outputReg       = BIOC_OUTPUT;
        llHdl->edgeRaiseReg    = BIOC_EDGE_RAISE;
        llHdl->edgeFallReg     = BIOC_EDGE_FALL;
        llHdl->irqEdgeRaiseReg = BIOC_IRQ_EDGE_RAISE;
        llHdl->irqEdgeFallReg  = BIOC_IRQ_EDGE_FALL;
        llHdl->debounceReg     = BIOC_DEBOUNCE;
        llHdl->controlReg      = BIOC_CONTROL;
    }

    DBGWRT_1((DBH, "LL - Z50_Init: base address = 0x%x\n", llHdl->ma));
    DBGWRT_3((DBH, "Using %s register set\n", 
              llHdl->d302i ? "D302I" : "BIOC" ));

    /*------------------------------+
    |  init hardware                |
    +------------------------------*/
    gpioReset( llHdl );

    *llHdlP = llHdl;    /* set low-level driver handle */

    DBGWRT_3((DBH, "Z50_Init() finished\n" ));
    return(ERR_SUCCESS);
}

/****************************** Z50_Exit ************************************/
/** De-initialize hardware and clean up memory
 *
 *  The function deinitializes all channels by setting them as inputs.
 *  The interrupt is disabled.
 *
 *  \param llHdlP      \IN  pointer to low-level driver handle
 *
 *  \return           \c 0 on success or error code
 */
static int32 Z50_Exit(
   LL_HANDLE    **llHdlP
)
{
    LL_HANDLE *llHdl = *llHdlP;
    int32 error = 0;

    DBGWRT_1((DBH, "LL - Z50_Exit\n"));

    /*------------------------------+
    |  de-init hardware             |
    +------------------------------*/
    gpioReset( llHdl );

    /*------------------------------+
    |  clean up memory               |
    +------------------------------*/
    *llHdlP = NULL;     /* set low-level driver handle to NULL */
    error = Cleanup(llHdl,error);

    return(error);
}

/****************************** Z50_Read ************************************/
/** Read a value from the device
 *
 *  The function reads the current state of all port pins.
 *
 *  \param llHdl      \IN  low-level handle
 *  \param ch         \IN  current channel
 *  \param valueP     \OUT read value
 *
 *  \return           \c 0 on success or error code
 */
static int32 Z50_Read(
    LL_HANDLE *llHdl,
    int32 ch,
    int32 *valueP
)
{
    DBGWRT_1((DBH, "LL - Z50_Read: ch=%d\n",ch));

    *valueP = MREAD_D32( llHdl->ma, llHdl->inputReg+ch*4 );

    return(ERR_SUCCESS);
}

/****************************** Z50_Write ***********************************/
/** Description:  Write a value to the device
 *
 *  The function writes a value to the ports which are programmed as outputs.
 *
 *  \param llHdl      \IN  low-level handle
 *  \param ch         \IN  current channel
 *  \param value      \IN  value to write
 *
 *  \return           \c 0 on success or error code
 */
static int32 Z50_Write(
    LL_HANDLE *llHdl,
    int32 ch,
    int32 value
)
{
    DBGWRT_1((DBH, "LL - Z50_Write: ch=%d  val=0x%x\n",ch, value));

    MWRITE_D32( llHdl->ma, llHdl->outputReg+ch*4, value );

    return(ERR_SUCCESS);
}

/****************************** Z50_SetStat *********************************/
/** Set the driver status
 *
 *  The driver supports \ref getstat_setstat_codes "these status codes"
 *  in addition to the standard codes (see mdis_api.h).
 *
 *  \param llHdl         \IN  low-level handle
 *  \param code          \IN  \ref getstat_setstat_codes "status code"
 *  \param ch            \IN  current channel
 *  \param value32_or_64 \IN  data or
 *                            pointer to block data structure (M_SG_BLOCK) for
 *                            block status codes
 *  \return           \c 0 on success or error code
 */
static int32 Z50_SetStat(
    LL_HANDLE *llHdl,
    int32  code,
    int32  ch,
    INT32_OR_64 value32_or_64
)
{
    MACCESS ma = llHdl->ma;
    int32   error = ERR_SUCCESS;
    int32       value = (int32)value32_or_64; /* 32bit value     */
    INT32_OR_64 valueP = value32_or_64; /* stores 32/64bit pointer */
    M_SG_BLOCK  *blk = (M_SG_BLOCK*)valueP;

    DBGWRT_1((DBH, "LL - Z50_SetStat: ch=%d code=0x%04x value=0x%x\n",
              ch,code,value));

    switch(code) {
        /*--------------------------+
        |  debug level              |
        +--------------------------*/
        case M_LL_DEBUG_LEVEL:
            llHdl->dbgLevel = value;
            break;
        /*--------------------------+
        |  enable interrupts        |
        +--------------------------*/
        case M_MK_IRQ_ENABLE:
            if( value == 0 ) {
                int i, numRegs;

                numRegs = (llHdl->d302i) ? CH_NUMBER : 1;

                /* for all registers */
                for( i=0; i<numRegs; ++i ) {

                    /* disable all interrupts */
                    MWRITE_D32( ma, llHdl->irqEdgeRaiseReg+i*4, 0 );
                    MWRITE_D32( ma, llHdl->irqEdgeFallReg+i*4, 0 );
                }
            }
            break;

        /*--------------------------+
        |  set irq counter          |
        +--------------------------*/
        case M_LL_IRQ_COUNT:
            llHdl->irqCount = value;
            break;
        /*--------------------------+
        |  channel direction        |
        +--------------------------*/
        case M_LL_CH_DIR:
            if( value != M_CH_INOUT ) {
                error = ERR_LL_ILL_DIR;
            }

            break;

        /*--------------------------+
        |  set IO ports             |
        +--------------------------*/
        case Z50_SET_PORTS:
            MSETMASK_D32( ma, llHdl->outputReg+ch*4, value );
            break;

        /*--------------------------+
        |  clear IO ports           |
        +--------------------------*/
        case Z50_CLR_PORTS:
            MCLRMASK_D32( ma, llHdl->outputReg+ch*4, value );
            break;

        /*--------------------------+
        |  irq on rising edge       |
        +--------------------------*/
        case Z50_IRQ_EDGE_RAISE:
            MWRITE_D32( ma, llHdl->irqEdgeRaiseReg+ch*4, value );
            break;

        /*--------------------------+
        |  irq on falling edge      |
        +--------------------------*/
        case Z50_IRQ_EDGE_FALL:
            MWRITE_D32( ma, llHdl->irqEdgeFallReg+ch*4, value );
            break;

        /*--------------------------+
        |  debouncer                |
        +--------------------------*/
        case Z50_BLK_DEBOUNCE:
        {
            Z50_DEBOUNCE_PB *pb = (Z50_DEBOUNCE_PB*)blk->data;

            /* ignore pb->input on D302I, use channel number instead */
            if( llHdl->d302i ) {
                MWRITE_D32( ma, llHdl->debounceReg + ch*4, pb->time );
            }
            else {
                MWRITE_D32( ma, llHdl->debounceReg + (pb->input)*4, pb->time );
            }

            break;
        }

        /*--------------------------+
        |  register signal          |
        +--------------------------*/
        case Z50_SET_SIGNAL:

            /* signal already installed ? */
            if( llHdl->portChangeSig ) {
                error = ERR_OSS_SIG_SET;
                break;
            }

            error = OSS_SigCreate( OSH, value, &llHdl->portChangeSig );
            break;

        /*--------------------------+
        |  unregister signal        |
        +--------------------------*/
        case Z50_CLR_SIGNAL:

            /* signal already installed ? */
            if( llHdl->portChangeSig == NULL ) {
                error = ERR_OSS_SIG_CLR;
                break;
            }

            error = OSS_SigRemove( OSH, &llHdl->portChangeSig );
            break;

        /*--------------------------+
        |  (unknown)                |
        +--------------------------*/
        default:
            error = ERR_LL_UNK_CODE;
    }

    return(error);
}

/****************************** Z50_GetStat *********************************/
/** Get the driver status
 *
 *  The driver supports \ref getstat_setstat_codes "these status codes"
 *  in addition to the standard codes (see mdis_api.h).
 *
 *  \param llHdl          \IN  low-level handle
 *  \param code           \IN  \ref getstat_setstat_codes "status code"
 *  \param ch             \IN  current channel
 *  \param value32_or_64P \IN  pointer to block data structure (M_SG_BLOCK) for
 *                             block status codes
 *  \param value32_or_64P \OUT data pointer or pointer to block data structure
 *                             (M_SG_BLOCK) for block status codes
 *
 *  \return           \c 0 on success or error code
 */
static int32 Z50_GetStat(
    LL_HANDLE *llHdl,
    int32  code,
    int32  ch,
    INT32_OR_64 *value32_or_64P
)
{
    MACCESS ma = llHdl->ma;
    int32 error = ERR_SUCCESS;
    int32       *valueP = (int32*)value32_or_64P; /* pointer to 32bit value  */
    INT32_OR_64 *value64P = value32_or_64P;       /* stores 32/64bit pointer  */
    M_SG_BLOCK *blk = (M_SG_BLOCK*)value32_or_64P;

    DBGWRT_1((DBH, "LL - Z50_GetStat: ch=%d code=0x%04x\n",
              ch,code));

    switch(code)
    {
        /*--------------------------+
        |  debug level              |
        +--------------------------*/
        case M_LL_DEBUG_LEVEL:
            *valueP = llHdl->dbgLevel;
            break;
        /*--------------------------+
        |  number of channels       |
        +--------------------------*/
        case M_LL_CH_NUMBER:
            if( llHdl->d302i ) {
                *valueP = CH_NUMBER;
            }
            else {
                *valueP = 1;
            }
            break;
        /*--------------------------+
        |  channel direction        |
        +--------------------------*/
        case M_LL_CH_DIR:
            *valueP = M_CH_INOUT;
            break;
        /*--------------------------+
        |  channel length [bits]    |
        +--------------------------*/
        case M_LL_CH_LEN:
            *valueP = 8;
            break;
        /*--------------------------+
        |  channel type info        |
        +--------------------------*/
        case M_LL_CH_TYP:
            *valueP = M_CH_BINARY;
            break;
        /*--------------------------+
        |  irq counter              |
        +--------------------------*/
        case M_LL_IRQ_COUNT:
            *valueP = llHdl->irqCount;
            break;
        /*--------------------------+
        |  ID PROM check enabled    |
        +--------------------------*/
        case M_LL_ID_CHECK:
            *valueP = 0;
            break;
        /*--------------------------+
        |   ident table pointer     |
        |   (treat as non-block!)   |
        +--------------------------*/
        case M_MK_BLK_REV_ID:
           *value64P = (INT32_OR_64)&llHdl->idFuncTbl;
           break;

        /*--------------------------+
        |  rising edge              |
        +--------------------------*/
        case Z50_EDGE_RAISE:
        {
            OSS_IRQ_STATE irqState;
            u_int32 bitsNotFromIrq;

            /* disable interrupts */
            irqState = OSS_IrqMaskR( OSH, llHdl->irqHdl );

            /*
             * combine bits:
             *  - which are set set by irq routine
             *  - from inputs not causing irqs
             */
            bitsNotFromIrq = MREAD_D32( ma, llHdl->edgeRaiseReg + ch*4) &
                ~MREAD_D32( ma, llHdl->irqEdgeRaiseReg + ch*4);

            *valueP = llHdl->edgeRaiseIrq[ch] | bitsNotFromIrq;

            /* clear bits for next request */
            MWRITE_D32( ma, llHdl->edgeRaiseReg, bitsNotFromIrq );
            llHdl->edgeRaiseIrq[ch] = 0;
            OSS_IrqRestore( OSH, llHdl->irqHdl, irqState );
            break;
        }

        /*--------------------------+
        |  falling edge             |
        +--------------------------*/
        case Z50_EDGE_FALL:
        {
            OSS_IRQ_STATE irqState;
            u_int32 bitsNotFromIrq;

            /* see code for rising edge */

            irqState = OSS_IrqMaskR( OSH, llHdl->irqHdl );
            bitsNotFromIrq = MREAD_D32( ma, llHdl->edgeFallReg + ch*4) &
                ~MREAD_D32( ma, llHdl->irqEdgeFallReg + ch*4);

            *valueP = llHdl->edgeFallIrq[ch] | bitsNotFromIrq;

            MWRITE_D32( ma, llHdl->edgeFallReg, bitsNotFromIrq );
            llHdl->edgeFallIrq[ch] = 0;
            OSS_IrqRestore( OSH, llHdl->irqHdl, irqState );

            break;
        }

        /*--------------------------+
        |  irq on raising edge      |
        +--------------------------*/
        case Z50_IRQ_EDGE_RAISE:
            *valueP = MREAD_D32( ma, llHdl->irqEdgeRaiseReg+ch*4 );
            break;

        /*--------------------------+
        |  irq on falling edge      |
        +--------------------------*/
        case Z50_IRQ_EDGE_FALL:
            *valueP = MREAD_D32( ma, llHdl->irqEdgeFallReg+ch*4 );
            break;

        /*--------------------------+
        |  debouncer                |
        +--------------------------*/
        case Z50_BLK_DEBOUNCE:
        {
            Z50_DEBOUNCE_PB *pb = (Z50_DEBOUNCE_PB*)blk->data;

            /* ignore pb->input on D302I, use channel number instead */
            if( llHdl->d302i ) {
                pb->time = MREAD_D32( ma, llHdl->debounceReg + ch*4 );
            }
            else {
                pb->time = MREAD_D32( ma, llHdl->debounceReg + (pb->input)*4 );
            }

            break;
        }

        /*--------------------------+
        |  (unknown)                |
        +--------------------------*/
        default:
            error = ERR_LL_UNK_CODE;
    }

    return(error);
}

/******************************* Z50_BlockRead ******************************/
/** Read a data block from the device
 *
 *  \param llHdl       \IN  low-level handle
 *  \param ch          \IN  current channel
 *  \param buf         \IN  data buffer
 *  \param size        \IN  data buffer size
 *  \param nbrRdBytesP \OUT number of read bytes
 *
 *  \return            \c 0 on success or error code
 */
static int32 Z50_BlockRead(
     LL_HANDLE *llHdl,
     int32     ch,
     void      *buf,
     int32     size,
     int32     *nbrRdBytesP
)
{
    DBGWRT_1((DBH, "LL - Z50_BlockRead: ch=%d, size=%d\n",ch,size));

    /* return number of read bytes */
    *nbrRdBytesP = 0;

    return( ERR_LL_ILL_FUNC );
}

/****************************** Z50_BlockWrite *****************************/
/** Write a data block from the device
 *
 *  \param llHdl       \IN  low-level handle
 *  \param ch          \IN  current channel
 *  \param buf         \IN  data buffer
 *  \param size        \IN  data buffer size
 *  \param nbrWrBytesP \OUT number of written bytes
 *
 *  \return            \c 0 on success or error code
 */
static int32 Z50_BlockWrite(
     LL_HANDLE *llHdl,
     int32     ch,
     void      *buf,
     int32     size,
     int32     *nbrWrBytesP
)
{
    DBGWRT_1((DBH, "LL - Z50_BlockWrite: ch=%d, size=%d\n",ch,size));

    /* return number of written bytes */
    *nbrWrBytesP = 0;

    return( ERR_LL_ILL_FUNC );
}


/****************************** Z50_Irq ************************************/
/** Interrupt service routine
 *
 *  The interrupt is triggered when one or more input ports change state.
 *  Depends on current setting via Z16_IRQ_SENSE.
 *
 *  If the driver can detect the interrupt's cause it returns
 *  LL_IRQ_DEVICE or LL_IRQ_DEV_NOT, otherwise LL_IRQ_UNKNOWN.
 *
 *  \param llHdl       \IN  low-level handle
 *  \return LL_IRQ_DEVICE   irq caused by device
 *          LL_IRQ_DEV_NOT  irq not caused by device
 *          LL_IRQ_UNKNOWN  unknown
 */
static int32 Z50_Irq(
    LL_HANDLE *llHdl
)
{
    MACCESS ma = llHdl->ma;
    u_int8  myIrq=0, i, numRegs;
    u_int32 inputsCausingIrqRaise=0, inputsCausingIrqFall=0;

    IDBGWRT_1((DBH, ">>> Z50_Irq:\n"));

    /* how many registers to handle ? */
    numRegs = (llHdl->d302i) ? CH_NUMBER : 1;

    for( i=0; i<numRegs; ++i ) {
        /* for raising edge: get mask of inputs which caused irq */
        inputsCausingIrqRaise = MREAD_D32( ma, llHdl->edgeRaiseReg+i*4 ) &
            MREAD_D32( ma, llHdl->irqEdgeRaiseReg+i*4 );

        /* clear irq by setting corresponding bits */
        MWRITE_D32( ma, llHdl->edgeRaiseReg+i*4, inputsCausingIrqRaise);

        /* save bits for user */
        llHdl->edgeRaiseIrq[i] |= inputsCausingIrqRaise;

        /* same for falling edge */
        inputsCausingIrqFall = MREAD_D32( ma, llHdl->edgeFallReg+i*4 ) &
            MREAD_D32( ma, llHdl->irqEdgeFallReg+i*4 );
        MWRITE_D32( ma, llHdl->edgeFallReg+i*4, inputsCausingIrqFall );
        llHdl->edgeFallIrq[i] |= inputsCausingIrqFall;

        /* this interrupt caused by GPIO controller ? */
        if( inputsCausingIrqRaise || inputsCausingIrqFall ) {
            myIrq = 1;
        }
    }

    if( myIrq ) {

        /* if requested send signal to application */
        if( llHdl->portChangeSig ) {
            OSS_SigSend( OSH, llHdl->portChangeSig );
        }

        llHdl->irqCount++;

        return( LL_IRQ_DEVICE );
    }

    return( LL_IRQ_DEV_NOT );
}

/****************************** Z50_Info ***********************************/
/** Get information about hardware and driver requirements
 *
 *  The following info codes are supported:
 *
 * \code
 *  Code                      Description
 *  ------------------------  -----------------------------
 *  LL_INFO_HW_CHARACTER      hardware characteristics
 *  LL_INFO_ADDRSPACE_COUNT   nr of required address spaces
 *  LL_INFO_ADDRSPACE         address space information
 *  LL_INFO_IRQ               interrupt required
 *  LL_INFO_LOCKMODE          process lock mode required
 * \endcode
 *
 *  The LL_INFO_HW_CHARACTER code returns all address and
 *  data modes (ORed) which are supported by the hardware
 *  (MDIS_MAxx, MDIS_MDxx).
 *
 *  The LL_INFO_ADDRSPACE_COUNT code returns the number
 *  of address spaces used by the driver.
 *
 *  The LL_INFO_ADDRSPACE code returns information about one
 *  specific address space (MDIS_MAxx, MDIS_MDxx). The returned
 *  data mode represents the widest hardware access used by
 *  the driver.
 *
 *  The LL_INFO_IRQ code returns whether the driver supports an
 *  interrupt routine (TRUE or FALSE).
 *
 *  The LL_INFO_LOCKMODE code returns which process locking
 *  mode the driver needs (LL_LOCK_xxx).
 *
 *  \param infoType    \IN  info code
 *  \param ...         \IN  argument(s)
 *
 *  \return            \c 0 on success or error code
 */
static int32 Z50_Info(
   int32  infoType,
   ...
)
{
    int32   error = ERR_SUCCESS;
    va_list argptr;

    va_start(argptr, infoType );

    switch(infoType) {
        /*-------------------------------+
        |  hardware characteristics      |
        |  (all addr/data modes ORed)   |
        +-------------------------------*/
        case LL_INFO_HW_CHARACTER:
        {
            u_int32 *addrModeP = va_arg(argptr, u_int32*);
            u_int32 *dataModeP = va_arg(argptr, u_int32*);

            *addrModeP = MDIS_MA08;
            *dataModeP = MDIS_MD08 | MDIS_MD16;
            break;
        }
        /*-------------------------------+
        |  nr of required address spaces |
        |  (total spaces used)           |
        +-------------------------------*/
        case LL_INFO_ADDRSPACE_COUNT:
        {
            u_int32 *nbrOfAddrSpaceP = va_arg(argptr, u_int32*);

            *nbrOfAddrSpaceP = ADDRSPACE_COUNT;
            break;
        }
        /*-------------------------------+
        |  address space type            |
        |  (widest used data mode)       |
        +-------------------------------*/
        case LL_INFO_ADDRSPACE:
        {
            u_int32 addrSpaceIndex = va_arg(argptr, u_int32);
            u_int32 *addrModeP = va_arg(argptr, u_int32*);
            u_int32 *dataModeP = va_arg(argptr, u_int32*);
            u_int32 *addrSizeP = va_arg(argptr, u_int32*);

            if (addrSpaceIndex >= ADDRSPACE_COUNT)
                error = ERR_LL_ILL_PARAM;
            else {
                *addrModeP = MDIS_MA24;
                *dataModeP = MDIS_MD32;
                *addrSizeP = ADDRSPACE_SIZE;
            }

            break;
        }
        /*-------------------------------+
        |   interrupt required           |
        +-------------------------------*/
        case LL_INFO_IRQ:
        {
            u_int32 *useIrqP = va_arg(argptr, u_int32*);

            *useIrqP = USE_IRQ;
            break;
        }
        /*-------------------------------+
        |   process lock mode            |
        +-------------------------------*/
        case LL_INFO_LOCKMODE:
        {
            u_int32 *lockModeP = va_arg(argptr, u_int32*);

            *lockModeP = LL_LOCK_CALL;
            break;
        }
        /*-------------------------------+
        |   (unknown)                    |
        +-------------------------------*/
        default:
          error = ERR_LL_ILL_PARAM;
    }

    va_end(argptr);
    return(error);
}

/*******************************  Ident  ***********************************/
/** Return ident string
 *
 *  \return            pointer to ident string
 */
static char* Ident( void )
{
    return( "Z50 - Z50 low level driver: $Id: z50_drv.c,v 1.2 2017/04/28 14:14:21 DPfeuffer Exp $" );
}

/********************************* Cleanup *********************************/
/** Close all handles, free memory and return error code
 *
 *  \warning The low-level handle is invalid after this function is called.
 *
 *  \param llHdl      \IN  low-level handle
 *  \param retCode    \IN  return value
 *
 *  \return           \IN   retCode
 */
static int32 Cleanup(
   LL_HANDLE    *llHdl,
   int32        retCode
)
{
    /*------------------------------+
    |  close handles                |
    +------------------------------*/
    /* clean up desc */
    if (llHdl->descHdl)
        DESC_Exit(&llHdl->descHdl);

    /* clean up debug */
    DBGEXIT((&DBH));

    /*------------------------------+
    |  free memory                  |
    +------------------------------*/
    /* free my handle */
    OSS_MemFree(llHdl->osHdl, (int8*)llHdl, llHdl->memAlloc);

    /*------------------------------+
    |  return error code            |
    +------------------------------*/
    return(retCode);
}

/**********************************************************************/
/** Reset GPIO controller
 *
 *  Sets the controller registers to default values:
 *  - all interrupt disabled
 *  - all outputs off
 *  - debouncing disabled
 *
 *  \param llHdl      \IN  low-level handle
 */

static void
gpioReset( LL_HANDLE *llHdl )
{
    MACCESS ma = llHdl->ma;
    int i, numRegs;

    /* reset internal control logic (BIOC only) */
    if( ! llHdl->d302i ) {
        MWRITE_D32( ma, llHdl->controlReg, 1 );
    }

    numRegs = (llHdl->d302i) ? CH_NUMBER : 1;

    /* for all registers */
    for( i=0; i<numRegs; ++i ) {

        /* disable all interrupts */
        MWRITE_D32( ma, llHdl->irqEdgeRaiseReg+i*4, 0 );
        MWRITE_D32( ma, llHdl->irqEdgeFallReg+i*4, 0 );

        /* switch off all outputs */
        MWRITE_D32( ma, llHdl->outputReg+i*4, 0 );

        /* reset edge registers */
        MWRITE_D32( ma, llHdl->edgeRaiseReg+i*4, 0xffffffff );
        MWRITE_D32( ma, llHdl->edgeFallReg+i*4,  0xffffffff );
    }

    /* disable debouncer */
    numRegs = (llHdl->d302i) ? CH_NUMBER : 32;

    for( i=0; i<numRegs; ++i ) {
        MWRITE_D32( ma, llHdl->debounceReg+i*4, 0 );
    }
}
