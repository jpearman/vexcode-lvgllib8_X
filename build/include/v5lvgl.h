/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) James Pearman, All rights reserved.                       */
/*                                                                            */
/*    Module:     v5lvgl.h                                                    */
/*    Author:     James Pearman                                               */
/*    Created:    15 Sept 2019                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

//
// header for using lvgl with VEXcode.

#ifndef V5_LVGL_H_
#define V5_LVGL_H_

#include <stdint.h>
#include <stdbool.h>

#include "../lvgl/lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

void    v5_lv_init( void );

#ifdef __cplusplus
}
#endif
#endif /* V5_LVGL_H_ */
