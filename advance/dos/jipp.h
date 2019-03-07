/*      __                              __                     
 *     /\ \                            /\ \                    
 *     \_\ \     __       __    ____   \_\ \     __       __   
 *     /'_` \  /'__`\   /'__`\ /',__\  /'_` \  /'__`\   /'__`\ 
 *    /\ \L\ \/\ \L\.\_/\  __//\__, `\/\ \L\ \/\ \L\.\_/\  __/ 
 *    \ \___,_\ \__/.\_\ \____\/\____/\ \___,_\ \__/.\_\ \____\
 *     \/__,_ /\/__/\/_/\/____/\/___/  \/__,_ /\/__/\/_/\/____/
 *
 *
 *    Driver for IPP Arcade joystick (Interfaz Puerto Paralelo).
 *
 *    By daesdae.
 * 
 *    Supports 1 joystick with up to 56 buttons.
 * 
 *    See <http://www.retrovicio.org/tutoriales/interfaz-puerto-paralelo>
 *    for details on how to build the interface.
 *
 *           |  Pin 10   |  Pin 11   |  Pin 12   |  Pin 13   |  Pin 15
 *    -------------------------------------------------------------------
 *    Pin  2 | UP          DOWN        LEFT        RIGHT       button_1
 *    Pin  3 | button_2    button_3    button_4    button_5    button_6
 *    Pin  4 | button_7    button_8    button_9    button_10   button_11
 *    Pin  5 | button_12   button_13   button_14   button_15   button_16
 *    Pin  6 | button_17   button_18   button_19   button_20   button_21
 *    Pin  7 | button_22   button_23   button_24   button_25   button_26
 *    Pin  8 | button_27   button_28   button_29   button_30   button_31
 *    Pin  9 | button_32   button_33   button_34   button_35   button_36
 *    Pin  1 | button_37   button_38   button_39   button_40   button_41
 *    Pin 14 | button_42   button_43   button_44   button_45   button_46
 *    Pin 16 | button_47   button_48   button_49   button_50   button_51
 *    Pin 17 | button_52   button_53   button_54   button_55   button_56
 *  
 */

/** \file
 * Joystick driver "ipp".
 */

#ifndef __JIPP_H
#define __JIPP_H

#include "joydrv.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Joystick driver "ipp".
 * \ingroup Joystick
 */
extern joystickb_driver joystickb_ipp_driver;

#ifdef __cplusplus
}
#endif

#endif
