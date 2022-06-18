/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) James Pearman, All rights reserved.                       */
/*                                                                            */
/*    Module:     v5lvgl.c                                                    */
/*    Author:     James Pearman                                               */
/*    Created:    15 Sept 2019                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "v5.h"
#include "v5lvgl.h"

// necessary private V5 API functions
void  vexTaskAdd( int (* callback)(void), int interval, char const *label );
void  vexTaskSleep( uint32_t time );

#define V5_HOR_RES_MAX          (480)
#define V5_VER_RES_MAX          (240)

/*----------------------------------------------------------------------------*/
/* Flush the content of the internal buffer the specific area on the display  */
/* You can use DMA or any hardware acceleration to do this operation in the   */
/* background but 'lv_disp_flush_ready()' has to be called when finished.     */
/*----------------------------------------------------------------------------*/
//
static void
disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
  vexDisplayCopyRect(area->x1, area->y1, area->x2, area->y2, (uint32_t*)color_p, area->x2 - area->x1 + 1);
  lv_disp_flush_ready(disp_drv);
}

/*----------------------------------------------------------------------------*/
/* Read the touchpad and store it in 'data'                                   */
/* Return false if no more data read; true for ready again                    */
/*----------------------------------------------------------------------------*/
//
static void
touch_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
  V5_TouchStatus status;
  vexTouchDataGet( &status );

  if( status.lastEvent == kTouchEventPress )
    data->state = LV_INDEV_STATE_PR;
  else
  if( status.lastEvent == kTouchEventRelease )
    data->state = LV_INDEV_STATE_REL;

  data->point.x = status.lastXpos;
  data->point.y = status.lastYpos;

  // false: no more data to read because we are not buffering
  return;   
}

/*----------------------------------------------------------------------------*/
/* Task to refresh internal lvgl counters and allow display updates           */
/*----------------------------------------------------------------------------*/
#define V5_LVGL_RATE    4

int
lvgltask() {
  while(1) {
      // this just increments internal counter, may as well put here to simplify
      lv_tick_inc(V5_LVGL_RATE);
      lv_task_handler();

      // Allow other tasks to run
      vexTaskSleep(V5_LVGL_RATE);
  }
  return(0);
}

/*----------------------------------------------------------------------------*/
/* Initialize lvgl for V5 running VEXcode projects                            */
/*----------------------------------------------------------------------------*/
void
v5_lv_init() {
    // init lvgl
    lv_init();

    // create a display buffer
    static lv_disp_draw_buf_t disp_buf_2;
    // A buffer for full screen res 
    static lv_color_t buf2_1[V5_HOR_RES_MAX * V5_VER_RES_MAX];
    // A buffer for full screen res 
    static lv_color_t buf2_2[V5_HOR_RES_MAX * V5_VER_RES_MAX];
    // Initialize the display buffer
    lv_disp_draw_buf_init(&disp_buf_2, buf2_1, buf2_2, V5_HOR_RES_MAX * V5_VER_RES_MAX);

    // create display driver
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);

    // Set the resolution of the display
    disp_drv.hor_res = V5_HOR_RES_MAX;
    disp_drv.ver_res = V5_VER_RES_MAX;

    // callback to copy to screen
    disp_drv.flush_cb = disp_flush; 

    // Set a display buffer
    disp_drv.draw_buf = &disp_buf_2;

    // register the driver
    lv_disp_drv_register(&disp_drv);

    // touch screen input
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_read;
    lv_indev_drv_register(&indev_drv);

    lv_obj_t* page = lv_obj_create(NULL);
    lv_obj_set_size(page, 480, 240);
    lv_scr_load(page);

    // add the update task
    vexTaskAdd( lvgltask, 2, "LVGL" );
}
