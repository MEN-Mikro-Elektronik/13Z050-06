/***********************  I n c l u d e  -  F i l e  ***********************/
/*!
 *        \file  d302i.h
 *
 *      \author  ulrich.bogensperger@men.de
 *
 *       \brief  Definitions for D302 "Model Infineon" hardware registers
 *               
 *    \switches  
 *
 *
 *---------------------------------------------------------------------------
 * Copyright 2004-2019, MEN Mikro Elektronik GmbH
 ****************************************************************************/

 /*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
