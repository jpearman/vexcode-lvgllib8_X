/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       james                                                     */
/*    Created:      Sun Sep 15 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
#include "v5lvgl.h"

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;

// define your global instances of motors and other devices here
lv_obj_t * myButton;
lv_obj_t * myButtonLabel;

static void btn_click_action(lv_event_t * event) {
    lv_event_code_t code = lv_event_get_code(event);

    if(code == LV_EVENT_CLICKED)
    {
  		lv_label_set_text(myButtonLabel, "clicked");
    }

    return ;
}

int demo() {
    /*Init the pressed style*/
    static lv_style_t style_pr;
    lv_style_init(&style_pr);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_BLUE, 3));

    myButton = lv_btn_create(lv_scr_act()); //create button, lv_scr_act() is deafult screen object
    lv_obj_add_event_cb(myButton, btn_click_action, LV_EVENT_ALL, NULL); //set function to be called on button click
    lv_obj_add_style(myButton, &style_pr, LV_STATE_PRESSED);
    lv_obj_set_size(myButton, 200, 50); //set the button size
    lv_obj_align(myButton, LV_ALIGN_TOP_LEFT, 10, 10); //set the position to top mid

    myButtonLabel = lv_label_create(myButton); //create label and puts it inside of the button
    lv_label_set_text(myButtonLabel, "Click the Button"); //sets label text
    lv_obj_center(myButtonLabel);

    return(0);
}

int main() {
    //int count = 0;
   
    v5_lv_init();
    demo();

    while(1) {
        //Brain.Screen.printAt( 10, 50, "Hello V5 %d", count++ );
        // Allow other tasks to run
        this_thread::sleep_for(10);
    }
}
