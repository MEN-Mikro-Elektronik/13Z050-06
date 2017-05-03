/***********************  I n c l u d e  -  F i l e  ***********************/
/*!
 *        \file  z50_drv.h
 *
 *      \author  ulrich.bogensperger@men.de
 *        $Date: 2017/04/28 14:14:41 $
 *    $Revision: 2.2 $
 *
 *       \brief  Header file for Z50 driver containing
 *               Z50 specific status codes and
 *               Z50 function prototypes
 *
 *    \switches  _ONE_NAMESPACE_PER_DRIVER_
 *               _LL_DRV_
 */
 /*-------------------------------[ History ]--------------------------------
 *
 * $Log: z50_drv.h,v $
 * Revision 2.2  2017/04/28 14:14:41  DPfeuffer
 * R: MDIS5 driver required
 * M: driver ported to MDIS5
 *
 * Revision 2.1  2004/11/29 09:00:45  ub
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2004 by MEN mikro elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/

#ifndef _Z50_DRV_H
#define _Z50_DRV_H

#ifdef __cplusplus
      extern "C" {
#endif


/*-----------------------------------------+
|  TYPEDEFS                                |
+-----------------------------------------*/
typedef struct {
    int input;                         /**< Number of input to get/set time */
    int time;                          /**< Debouncing time (see docs) */
} Z50_DEBOUNCE_PB;

/*-----------------------------------------+
|  DEFINES                                 |
+-----------------------------------------*/
/** \name Z50 specific Getstat/Setstat standard codes 
 *  \anchor getstat_setstat_codes
 */
/**@{*/
#define Z50_SET_PORTS      M_DEV_OF+0x00   /**<   S: Set IO ports without 
                                              affecting others */
#define Z50_CLR_PORTS      M_DEV_OF+0x01   /**<   S: Clear IO ports without 
                                              affecting others */
#define Z50_EDGE_RAISE     M_DEV_OF+0x02   /**< G  : Get inputs which have
                                              encountered raising edge since
                                              last request  */
#define Z50_EDGE_FALL      M_DEV_OF+0x03   /**< G  : Get inputs which have 
                                              encountered falling edge 
                                              since last request  */
#define Z50_IRQ_EDGE_RAISE M_DEV_OF+0x04   /**< G,S: Enable IRQ on raising 
                                              edge on inputs */
#define Z50_IRQ_EDGE_FALL  M_DEV_OF+0x05   /**< G,S: Enable IRQ on falling 
                                              edge on inputs */
#define Z50_SET_SIGNAL     M_DEV_OF+0x06   /**<   S: Set signal sent on IRQ  */
#define Z50_CLR_SIGNAL     M_DEV_OF+0x07   /**<   S: Uninstall signal        */

#define Z50_BLK_DEBOUNCE   M_DEV_BLK_OF+0x00 /**< G,S: Get/set debouncing
                                                time */
/**@}*/


#ifndef  Z50_VARIANT
# define Z50_VARIANT       Z50
#endif

# define _Z50_GLOBNAME(var,name) var##_##name
#ifndef _ONE_NAMESPACE_PER_DRIVER_
# define Z50_GLOBNAME(var,name) _Z50_GLOBNAME(var,name)
#else
# define Z50_GLOBNAME(var,name) _Z50_GLOBNAME(Z50,name)
#endif

#define __Z50_GetEntry          Z50_GLOBNAME(Z50_VARIANT, GetEntry)

/*-----------------------------------------+
|  PROTOTYPES                              |
+-----------------------------------------*/
#ifdef _LL_DRV_
#ifndef _ONE_NAMESPACE_PER_DRIVER_
	extern void __Z50_GetEntry(LL_ENTRY* drvP);
#endif
#endif /* _LL_DRV_ */

/*-----------------------------------------+
|  BACKWARD COMPATIBILITY TO MDIS4         |
+-----------------------------------------*/
#ifndef U_INT32_OR_64
 /* we have an MDIS4 men_types.h and mdis_api.h included */
 /* only 32bit compatibility needed!                     */
 #define INT32_OR_64  int32
 #define U_INT32_OR_64 u_int32
 typedef INT32_OR_64  MDIS_PATH;
#endif /* U_INT32_OR_64 */

#ifdef __cplusplus
      }
#endif

#endif /* _Z50_DRV_H */
