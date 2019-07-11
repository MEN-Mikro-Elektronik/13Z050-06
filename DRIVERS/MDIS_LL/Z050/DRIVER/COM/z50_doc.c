/*********************  P r o g r a m  -  M o d u l e ***********************/
/*!
 *        \file  z50_doc.c
 *
 *      \author  ulrich.bogensperger@men.de
 *
 *      \brief   User documentation for BIOC driver
 *
 *     Required: -
 *
 *     \switches -
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

/*! \mainpage
    This is the documentation of the MDIS5 low-level driver for the GPIO
    controller implemented in the FPGA on F401. It also works on the D302
    "Model Infineon" (D302i).

    The GPIO controller supports the following features:

    - 32 bit general purpose I/O (128 bit on D302i) state of output port pins
    - readable on D302i ability to generate interrupts on rising, falling or
    - both edges on port hardware debouncing time adjustable for every port

    This driver supports the MEN Binary Input Output Controller (BIOC).  It
    uses call locking.

    \n
    \section Variants Variants
    (no variants)

    \n \section FuncDesc Functional Description

    \n \subsection General General

    The most functions return/take an 32 bit value. If not described
    differently every bit in the set/read values correspond to a single IO
    port pin. Bit 0 (LSB) to port 0, bit 31 (MSB) to port 31.


    \n \subsection channels Logical channels

    If used on the D302i board the driver supports four channels from which
    each one handles up to 32 inputs/outputs. This behaviour depends on the
    state of descriptor entry Z50_D302I.


    \n \subsection readwrite Reading and writing

    The driver supports multiple channels (selectable using MDIS SetStat 
    M_MK_CH_CURRENT) each one handling a goup of up to 32 input/output ports.
    The ports of one group can only be influenced together.

    The M_read() function call reads the current state of all ports of the 
    current channel at once. 
    M_write() forces all output ports to the given level or affects the 
    power switches associated with the ports, dependant on the hardware
    circuity connected to the port.
    
    - Bit=0: low level on port pin (or switch closed)
    - Bit=1: high level on port pin (or switch open)
    
    Additionally ports can be set/ cleared using SetStat Z50_SET_PORTS and
    Z50_CLEAR_PORTS. These functions affect only the ports whose corresponding
    bits are set in the given value. 
    
    Data direction of the ports cannot be programmed.


    \n \subsection edgedetection Edge detection

    Using GetStat Z50_EDGE_RISE the application can determine if a rising
    edge has happened on each of the inputs on one channel. The function 
    returns a 32 bit value in which each bit represents one input. 

    - Bit=1: rising edge on this port since last request
    - Bit=0: no rising edge

    This call clears all 32 bits of the current channel.

    GetStat Z50_EDGE_FALL works the same way but for falling edges.


    
    \n \subsection interrupts Interrupt handling and signals
    
    SetStats Z50_IRQ_EDGE_RISE and Z50_IRQ_EDGE_FALL enable interrupt 
    generation on rising and/or falling edge on any input of the current
    channel. The functions take a 32 bit value in which each bit corresponds 
    to an input.

    - Bit=1: generate interrupt on rising/falling edge of this input
    - Bit=0: no interrupt.

    If an interrupt happens the driver will send the signal which was 
    assigned using SetStat Z50_IRQ_SIGNAL to the application. 
    
    The current status can be read back using GetStats Z50_IRQ_EDGE_RISE 
    and Z50_IRQ_EDGE_FALL.


    \n \subsection debounce Debouncing
    
    Debouncing is enabled using block SetStat Z50_BLK_DEBOUNCE. This function
    takes the pointer to a Z50_DEBOUNCE_PB structure containing fields 
    \a input and \a time.

    - \b BIOC: Input designates the number of the input of the current
    channel whose debouncing value is to be set. Valid range is 0..31.
    - \b D302: Input is ignored. Time is valid for all inputs of the current
    channel.

    \a Time is the debouncing time measured in steps of 50µsec.  It is an
    unsigned 16bit integer, valid range is 0..0xffff. Zero turns the
    debouncing circuit off, 0xffff corresponds to a time of 3.267 sec (on
    D302).

    The current status can be read back using GetStat Z50_BLK_DEBOUNCE. 
    Like its counterpart it takes a pointer to a Z50_DEBOUNCE_PB structure. 
    \a Input has to be given by user, \a time is filled by the driver.

    When the debouncing time is updated, the new value is used after the
    current cycle is finished, i.e. the input has to be low for more than the
    old debouncing time.
    
    \n \subsection default Default values
    M_open() and M_close() set all ports to default values: 
    
    - All outputs are in state "off"
    - Interrupts disabled 
    - Debouncing disabled



    \n \section api_functions Supported API Functions

    <table border="0">
    <tr>
        <td><b>API function</b></td>
        <td><b>Functionality</b></td>
        <td><b>Corresponding low level function</b></td></tr>

    <tr><td>M_open()</td><td>Open device</td><td>Z50_Init()</td></tr>

    <tr><td>M_close()     </td><td>Close device             </td>
    <td>Z50_Exit())</td></tr>
    <tr><td>M_read()      </td><td>Read from device         </td>
    <td>Z50_Read()</td></tr>
    <tr><td>M_write()     </td><td>Write to device          </td>
    <td>Z50_Write()</td></tr>
    <tr><td>M_setstat()   </td><td>Set device parameter     </td>
    <td>Z50_SetStat()</td></tr>
    <tr><td>M_getstat()   </td><td>Get device parameter     </td>
    <td>Z50_GetStat()</td></tr>
    <tr><td>M_getblock()  </td><td>Block read from device   </td>
    <td>Z50_BlockRead()</td></tr>
    <tr><td>M_setblock()  </td><td>Block write from device  </td>
    <td>Z50_BlockWrite()</td></tr>
    <tr><td>M_errstringTs() </td><td>Generate error message </td>
    <td>-</td></tr>
    </table>


    \n \section descriptor_entries Descriptor Entries

    The low-level driver initialization routine decodes the following entries
    ("keys") in addition to the general descriptor keys:

    <table border="0">
    <tr><td><b>Descriptor entry</b></td>
        <td><b>Description</b></td>
        <td><b>Values</b></td>
    </tr>

    <tr><td>Z50_D302I</td>
        <td>Support for D302 Model Infineon enabled if 1</td>
        <td>0..1, default: 0</td>
    </tr>

    </table>

    
    \n \section codes Z50 specific Getstat/Setstat codes
    see \ref getstat_setstat_codes "section about Getstat/Setstat codes"

    \n \section Documents Overview of all Documents

    \subsection z50_simp  Simple example for using the driver
    z50_simp.c (see example section)

    \subsection z50_min   Minimum descriptor
    z50_min.dsc
    demonstrates the minimum set of options necessary for using the drver.

    \subsection z50_max   Minimum descriptor
    z50_max.dsc
    shows all possible configuration options for this driver.
*/

/** \example z50_simp.c
Simple example for driver usage
*/

/*! \page dummy
  \menimages
*/

