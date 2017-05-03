/***********************  I n c l u d e  -  F i l e  ***********************/
/*!
 *        \file  d302i.h
 *
 *      \author  ulrich.bogensperger@men.de
 *        $Date: 2004/11/29 09:00:31 $
 *    $Revision: 1.1 $
 *
 *       \brief  Definitions for D302 "Model Infineon" hardware registers
 *               
 *    \switches  
 */
 /*-------------------------------[ History ]--------------------------------
 *
 * $Log: d302i.h,v $
 * Revision 1.1  2004/11/29 09:00:31  ub
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2004 by MEN mikro elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/

#ifndef _D302I_H
#define _D302I_H

#ifdef __cplusplus
      extern "C" {
#endif


/*-----------------------------------------+
|  TYPEDEFS                                |
+-----------------------------------------*/
/* (none) */

/*-----------------------------------------+
|  DEFINES                                 |
+-----------------------------------------*/

/* Register definitions */

#define D302I_INPUT( n )        (0x00+(n)*4)    /**< input registers (n<4) */
#define D302I_OUTPUT( n )       (0x10+(n)*4)    /**< output registers */
#define D302I_EDGE_RAISE( n )   (0x20+(n)*4)    /**< raising edge registers */
#define D302I_EDGE_FALL( n )    (0x30+(n)*4)    /**< falling edge registers */
#define D302I_IRQ_EDGE_RAISE( n )(0x40+(n)*4)   /**< irq mask rising edge */
#define D302I_IRQ_EDGE_FALL( n )(0x50+(n)*4)    /**< irq mask falling edge */
#define D302I_DEBOUNCE( n )     (0x60+(n)*4)    /**< debouncer registers */
#define D302I_CTRL              0x104           /**< control/status */

/* Bit definitions */

/* for D302I_CTRL: */
#define D302I_CTRL_GIEN         0x00020000      /**< global interrupt enable */
#define D302I_CTRL_GIP          0x00010000      /**< global interrupt pending*/


/*-----------------------------------------+
|  PROTOTYPES                              |
+-----------------------------------------*/
/* (none) */

#ifdef __cplusplus
      }
#endif

#endif /* _D302I_H */
