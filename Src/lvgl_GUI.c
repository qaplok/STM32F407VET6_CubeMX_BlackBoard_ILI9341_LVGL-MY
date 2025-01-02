/*
 * lvgl_GUI.c
 *
 *  Created on: Nov 19, 2020
 *      Author: KlokockaJir
 */

/*********************
*      INCLUDES
*********************/
#include "lvgl.h"
#include "ili9341.h"
#include "lvgl_GUI.h"
#include <stdio.h>
#include "main.h"



/*
https://lvgl.io/tools/fontconverter

0x2a-0x3a,0x20,0xb0,0x43

*/

#define TEMP_BORDER 0
#define BTN_SZX 90
#define BTN_SZY 90

/**********************
*  STATIC VARIABLES
**********************/

static lv_obj_t* tv;
static lv_obj_t* tab1;
static lv_obj_t* tab2;
static lv_obj_t* tab3;
static lv_obj_t* btn_Tx;
static lv_obj_t* btn_Rx;
static lv_obj_t* btn_ILink;
static lv_obj_t* btn_CLink;
static lv_obj_t* btn_Date;
static lv_obj_t* btn_Time;
//--------------------------------------------
static lv_obj_t* my_screen;
static lv_obj_t* calendar_screen;
static lv_obj_t* time_screen;
static lv_obj_t* Err_scr;

static lv_obj_t* calendar_setings;
static lv_obj_t* time_setings;
//static lv_obj_t* calendar_gui;
static lv_obj_t* btn_back;
static lv_obj_t* btn_back_date;
static lv_obj_t* btn_set_date;
static lv_obj_t* btn_set_time;
static lv_obj_t *roller_day;
static lv_obj_t *roller_month;
static lv_obj_t *roller_year;
static lv_obj_t *roller_year_fake;
static lv_obj_t *roller_hour;
static lv_obj_t *roller_min;

static lv_obj_t* lblTime;
static lv_obj_t* lblDate;
uint8_t btn_size = 1;

lv_calendar_date_t today;
lv_time_date_t today_time; 
lv_calendar_date_t new_date;
lv_time_date_t new_time; 

/**********************
*   GLOBAL FUNCTIONS
**********************/


static void controls_create(lv_obj_t* parent);
static void controls_create2(lv_obj_t* parent);
static void settings_create(lv_obj_t* parent);

//static void JK_Test_Button(lv_obj_t* parent, const char* symbol, const char* text, lv_obj_t* btn);
static void button_event_handler(lv_obj_t* btn_Date, lv_event_t event);
static void rollers_event_handler(lv_obj_t * obj, lv_event_t event);

/**********************
*      FUNCTIONS
**********************/

void start_screen(void)
{
  lv_scr_load(tv);  
}
void lvgl_GUI(void)
{   //TabView
    tv = lv_tabview_create(my_screen, NULL);
    //lv_obj_set_style_local_pad_left(tv, LV_TABVIEW_PART_TAB_BG, LV_STATE_DEFAULT, LV_HOR_RES / 2);
    lv_tabview_set_anim_time(tv, 250);
    lv_tabview_set_btns_pos(tv, LV_TABVIEW_TAB_POS_BOTTOM);
    
    lblTime = lv_label_create(tv , NULL);
    lv_obj_set_size(lblTime, 10, 59);
    lv_obj_set_pos(lblTime, 180, 240);
    lblDate = lv_label_create(tv , NULL);
    lv_obj_set_size(lblDate, 10, 100);
    lv_obj_set_pos(lblDate, 10, 240);

    tab1 = lv_tabview_add_tab(tv, "ShortHop");
    tab2 = lv_tabview_add_tab(tv, "ILink");
    tab3 = lv_tabview_add_tab(tv, "Settings");

    controls_create(tab1);
    controls_create2(tab2);
    settings_create(tab3);
    
}



static void controls_create(lv_obj_t* parent) {
	lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_TOP);
	//JK_Test_Button(parent, LV_SYMBOL_WIFI, " Transmit", btn_Tx);
	//JK_Test_Button(parent, LV_SYMBOL_WIFI, " Receive", btn_Rx);

    btn_Tx = lv_btn_create(parent, NULL);
	lv_obj_set_size(btn_Tx, 200, 100);
	lv_obj_t* label1 = lv_label_create(btn_Tx, NULL);
	lv_label_set_text_fmt(label1, "%s %s", LV_SYMBOL_WIFI, " Transmit");
    lv_obj_set_event_cb(btn_Tx, button_event_handler);

    btn_Rx = lv_btn_create(parent, NULL);
	lv_obj_set_size(btn_Rx, 200, 100);
	lv_obj_t* label2 = lv_label_create(btn_Rx, NULL);
	lv_label_set_text_fmt(label2, "%s %s", LV_SYMBOL_WIFI,  "Receive");
    lv_obj_set_event_cb(btn_Rx, button_event_handler);
	
}

static void controls_create2(lv_obj_t* parent) {
	lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_TOP);
	//JK_Test_Button(parent, LV_SYMBOL_WIFI, " PD - CLink", btn_ILink);
	//JK_Test_Button(parent, LV_SYMBOL_WIFI, " CLink - CLink", btn_CLink);
      
    btn_ILink = lv_btn_create(parent, NULL);
	lv_obj_set_size(btn_ILink, 200, 100);
	lv_obj_t* label3 = lv_label_create(btn_ILink, NULL);
	lv_label_set_text_fmt(label3, "%s %s", LV_SYMBOL_WIFI, " PD - CLink");
    lv_obj_set_event_cb(btn_ILink, button_event_handler);

    btn_CLink = lv_btn_create(parent, NULL);
	lv_obj_set_size(btn_CLink, 200, 100);
	lv_obj_t* label4 = lv_label_create(btn_CLink, NULL);
	lv_label_set_text_fmt(label4, "%s %s", LV_SYMBOL_WIFI,  " CLink testbox");
    lv_obj_set_event_cb(btn_CLink, button_event_handler);
}

static void settings_create(lv_obj_t* parent) {
	lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_TOP);
	//JK_Test_Button(parent, LV_SYMBOL_SETTINGS, " Date", btn_Date);
	//JK_Test_Button(parent, LV_SYMBOL_SETTINGS, " Time", btn_Time);	

    btn_Date = lv_btn_create(parent, NULL);
	lv_obj_set_size(btn_Date, 200, 100);
	lv_obj_t* label5 = lv_label_create(btn_Date, NULL);
	lv_label_set_text_fmt(label5, "%s %s", LV_SYMBOL_SETTINGS, " Date");
    lv_obj_set_event_cb(btn_Date, button_event_handler);

    btn_Time = lv_btn_create(parent, NULL);
	lv_obj_set_size(btn_Time, 200, 100);
	lv_obj_t* label6 = lv_label_create(btn_Time, NULL);
	lv_label_set_text_fmt(label6, "%s %s", LV_SYMBOL_SETTINGS, " Time");
    lv_obj_set_event_cb(btn_Time, button_event_handler);
	
}

/*
static void JK_Test_Button(lv_obj_t* parent, const char* symbol, const char* text, lv_obj_t* btn) {
	 btn = lv_btn_create(parent, NULL);
	    //lv_obj_set_pos(btn, 10, 10);
	    lv_obj_set_size(btn, 200, 100);
	    //Button label
	    lv_obj_t* label = lv_label_create(btn, NULL);
	    lv_label_set_text_fmt(label, "%s %s", symbol, text);
         lv_obj_set_event_cb(btn, button_event_handler);
}
*/

void settings_window(void)
{
    calendar_setings = lv_win_create(calendar_screen, NULL);
    lv_win_set_title(calendar_setings, "Calendar settings");
    lv_win_set_header_height(calendar_setings, 20);
   

   //-------Calendar not work with year in  uint16 format
    /*
    calendar_gui = lv_calendar_create(calendar_setings, NULL);
    lv_obj_set_size(calendar_gui, 205, 210);
    //lv_obj_set_pos(calendar_gui, 5, 10);
    lv_calendar_set_today_date(calendar_gui, &today);
    lv_calendar_set_showed_date(calendar_gui, &today);
*/
    roller_year_fake = lv_roller_create(calendar_setings, NULL);
    lv_roller_set_options(roller_year_fake, "20\n", LV_ROLLER_MODE_INIFINITE);
    lv_roller_set_visible_row_count(roller_year_fake, 1);
    lv_obj_align(roller_year_fake, NULL, LV_ALIGN_IN_RIGHT_MID, -70, -25);
    lv_obj_set_event_cb(roller_year_fake, rollers_event_handler);

    roller_year = lv_roller_create(calendar_setings, NULL);
    lv_roller_set_options(roller_year,
                        "23\n" "24\n" "25\n" "26\n" "27\n" "28\n" "29\n" "30\n" "31\n" "32\n" "33\n" "34\n" "35\n"
                        "36\n" "37\n" "38\n" "39\n" "40\n" "41\n" "42\n" "43\n" "44\n" "45\n"
                        "46\n" "47\n" "48\n" "49\n" "50\n" "51\n" "52\n" "53\n" "54\n" "55\n"
                        "56\n" "57\n" "58\n" "59\n" "60\n" "61\n" "62\n" "63\n" "64\n" "65",
                        LV_ROLLER_MODE_INIFINITE);
    lv_roller_set_visible_row_count(roller_year, 4);
    lv_obj_align(roller_year, NULL, LV_ALIGN_IN_RIGHT_MID, -20, -25);
    lv_obj_set_event_cb(roller_year, rollers_event_handler);
        
    roller_month = lv_roller_create(calendar_setings, NULL);
    lv_roller_set_options(roller_month,
                        "1\n" "2\n" "3\n" "4\n" "5\n" "6\n" "7\n" "8\n" "9\n" "10\n" "11\n" "12",
                        LV_ROLLER_MODE_INIFINITE);
    lv_roller_set_visible_row_count(roller_month, 4);
    lv_obj_align(roller_month, NULL, LV_ALIGN_CENTER, -30, -25);
    lv_obj_set_event_cb(roller_month, rollers_event_handler);
    
    roller_day = lv_roller_create(calendar_setings, NULL);
    lv_roller_set_options(roller_day,
                        "1\n" "2\n" "3\n" "4\n" "5\n""6\n" "7\n" "8\n" "9\n" "10\n"
                        "11\n" "12\n" "13\n" "14\n" "15\n" "16\n" "17\n" "18\n" "19\n" "20\n"
                        "21\n" "22\n" "23\n" "24\n" "25\n" "26\n" "27\n" "28\n" "29\n" "30\n"
                        "31", LV_ROLLER_MODE_INIFINITE);
    lv_roller_set_visible_row_count(roller_day, 4);
    lv_obj_align(roller_day, NULL, LV_ALIGN_IN_LEFT_MID, 10, -25);
    lv_obj_set_event_cb(roller_day, rollers_event_handler);

    btn_back_date = lv_btn_create(calendar_setings, NULL);
    lv_obj_set_pos(btn_back_date, 15, 240);
	lv_obj_set_size(btn_back_date, 90, 40);
	lv_obj_t* label7 = lv_label_create(btn_back_date, NULL);
	lv_label_set_text_fmt(label7, "%s %s", LV_SYMBOL_HOME,  "Back");
    lv_obj_set_event_cb(btn_back_date, button_event_handler);
   

    btn_set_date = lv_btn_create(calendar_setings, NULL);
    lv_obj_set_pos(btn_set_date, 130, 240);
	lv_obj_set_size(btn_set_date, 90, 40);
	lv_obj_t* label8 = lv_label_create(btn_set_date, NULL);
	lv_label_set_text_fmt(label8, "%s %s", LV_SYMBOL_SETTINGS,  "Set");
    lv_obj_set_event_cb(btn_set_date, button_event_handler);
   

}

void time_window(void)
{
    time_setings = lv_win_create(time_screen, NULL);
    lv_win_set_title(time_setings, "Time settings");
    lv_win_set_header_height(time_setings, 20);
   
    roller_hour = lv_roller_create(time_setings, NULL);
    lv_roller_set_options(roller_hour,
                        "0\n" "1\n" "2\n" "3\n" "4\n" "5\n" "6\n" "7\n" "8\n" "9\n" "10\n" "11\n" "12\n"
                        "13\n" "14\n" "15\n" "16\n" "17\n" "18\n" "19\n" "20\n" "21\n" "22\n" "23",
                        LV_ROLLER_MODE_INIFINITE);
    lv_roller_set_visible_row_count(roller_hour, 4);
    lv_obj_align(roller_hour, NULL, LV_ALIGN_CENTER, -50, -25);
    lv_obj_set_event_cb(roller_hour, rollers_event_handler);
    
    roller_min = lv_roller_create(time_setings, NULL);
    lv_roller_set_options(roller_min,
                        "0\n" "1\n" "2\n" "3\n" "4\n" "5\n""6\n" "7\n" "8\n" "9\n" "10\n"
                        "11\n" "12\n" "13\n" "14\n" "15\n" "16\n" "17\n" "18\n" "19\n" "20\n"
                        "21\n" "22\n" "23\n" "24\n" "25\n" "26\n" "27\n" "28\n" "29\n" "30\n"
                        "31\n" "32\n" "33\n" "34\n" "35\n" "36\n" "37\n" "38\n" "39\n" "40\n"
                        "41\n" "42\n" "43\n" "44\n" "45\n" "46\n" "47\n" "48\n" "49\n" "50\n"
                        "51\n" "52\n" "53\n" "54\n" "55\n" "56\n" "57\n" "58\n" "59",
                         LV_ROLLER_MODE_INIFINITE);
    
    lv_roller_set_visible_row_count(roller_min, 4);
    lv_obj_align(roller_min, NULL, LV_ALIGN_CENTER, 50, -25);
    lv_obj_set_event_cb(roller_min, rollers_event_handler);

    btn_back = lv_btn_create(time_setings, NULL);
    lv_obj_set_pos(btn_back, 15, 240);
	lv_obj_set_size(btn_back, 90, 40);
	lv_obj_t* label9 = lv_label_create(btn_back, NULL);
	lv_label_set_text_fmt(label9, "%s %s", LV_SYMBOL_HOME,  "Back");
    lv_obj_set_event_cb(btn_back, button_event_handler);
   

    btn_set_time = lv_btn_create(time_setings, NULL);
    lv_obj_set_pos(btn_set_time, 130, 240);
	lv_obj_set_size(btn_set_time, 90, 40);
	lv_obj_t* label10 = lv_label_create(btn_set_time, NULL);
	lv_label_set_text_fmt(label10, "%s %s", LV_SYMBOL_SETTINGS,  "Set");
    lv_obj_set_event_cb(btn_set_time, button_event_handler);
   

}


void LCD_UpdateTime(uint8_t hrs, uint8_t mnts, uint8_t scnds) {
    today_time.Hours = hrs;
    today_time.Minutes = mnts;
    today_time.Seconds = scnds;
    
    lv_label_set_text_fmt(lblTime, "%02i%:%02i:%02i", today_time.Hours, today_time.Minutes, today_time.Seconds);
    //lv_label_set_text_fmt(lblTimeBig, "%02i%c%02i", hrs, blinkDots, mnts);
}

void LCD_UpdateDate(uint8_t wd, uint8_t dy, uint8_t mnth, uint16_t yr) {
    today.year = yr;
    today.month = mnth;
    today.day = dy;
    today.wd = wd;
    //static const char* dayNames[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    //lv_label_set_text_fmt(lblDate, "%s %02i.%02i.%04d", dayNames[wd], today.day, today.month, 0x7D0 +today.year);
    lv_label_set_text_fmt(lblDate, "%02i.%02i.%04d", today.day, today.month, 0x7D0 +today.year);

}

//------------------------------------------------------------------------------------------EVENTS
static void rollers_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED) {
        if (obj == roller_year) {
           new_date.year = 23 + lv_roller_get_selected(obj) ;
           
        }
        if (obj == roller_day) {
           new_date.day = 1 + lv_roller_get_selected(obj) ;
          
        }
        if (obj == roller_month) {
           new_date.month = 1 + lv_roller_get_selected(obj) ;
          
        }
         if (obj == roller_hour) {
           new_time.Hours =  lv_roller_get_selected(obj) ;
          
        }
        if (obj == roller_min) {
           new_time.Minutes =  lv_roller_get_selected(obj) ;
          
        }
        //char buf[32];
        //lv_roller_get_selected_str(obj, buf, sizeof(buf));
        //printf("Selected month: %s\n", buf);
    }
}

static void button_event_handler(lv_obj_t* btn, lv_event_t event)
{
 if (event == LV_EVENT_PRESSED) {
    //lcdPrintf("%d", btn);
    
    if (btn == btn_Date) {
        new_date.day = today.day;
        new_date.month = today.month;
        new_date.year = today.year;
        new_date.wd = today.wd;
        lv_roller_set_selected(roller_year, today.year - 23, LV_USE_ANIMATION);
        lv_roller_set_selected(roller_month, today.month - 1, LV_USE_ANIMATION);
        lv_roller_set_selected(roller_day, today.day-1, LV_USE_ANIMATION);
        lv_scr_load(calendar_setings);
    }
    if (btn == btn_Time) {
        new_time.Hours = today_time.Hours;
        new_time.Minutes = today_time.Minutes;
        lv_roller_set_selected(roller_min, today_time.Minutes, LV_USE_ANIMATION);
        lv_roller_set_selected(roller_hour, today_time.Hours, LV_USE_ANIMATION);
        lv_scr_load(time_setings);
    }
    if (btn == btn_back) {
        lv_scr_load(tv);
    }
    if (btn == btn_back_date) {
        lv_scr_load(tv);
    }
    if (btn == btn_set_time) {
        RTC_set(new_time.Hours, new_time.Minutes);
        lv_scr_load(tv);
        //lcdPrintf("@%di %di ",  new_time.Hours, new_time.Minutes);
        
    }
    if (btn == btn_set_date) {
        DATE_set(new_date.wd, new_date.day, new_date.month, new_date.year);
        lv_scr_load(tv);
        //lcdPrintf("@%di %di %di %di ", new_date.wd, new_date.day, new_date.month, new_date.year);
    }
    if (btn == btn_Tx) {

    }
    if (btn == btn_Rx) {
        Start_PWM_24();
    }
    if (btn == btn_ILink) {

    }
    if (btn == btn_CLink) {

    }
 }
}
//----------------------------------------------------------------------------------------------
void Error_scr(void)
{
    Err_scr = lv_win_create(lv_scr_act(), NULL);
    lv_win_set_title(Err_scr, "Time settings");
    lv_win_set_header_height(Err_scr, 20);
    lv_obj_t* label_err = lv_label_create(btn_set_time, NULL);
	lv_label_set_text_fmt(label_err, "%s %s", LV_SYMBOL_STOP,  "ERROR");   
}