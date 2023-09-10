#ifndef _BABYUI_CPP_
#define _BABYUI_CPP_

#include "BabyUI.h"

//#define MUI_DEBUG(...) Serial.printf(__VA_ARGS__)

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

uint8_t mui_hrule(mui_t *ui, uint8_t msg)
{

  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      u8g2_DrawHLine(u8g2, 0, mui_get_y(ui), u8g2_GetDisplayWidth(u8g2));
      break;
  }
  return 0;
}

uint8_t mui_status_bar(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  
  switch(msg)
  {
    case MUIF_MSG_DRAW:

      u8g2_DrawUTF8(u8g2,mui_get_x(ui),mui_get_y(ui) - 2, DateTime.format(dateFormatStrings[dateFormat]).c_str());
      const uint8_t *prevFont = u8g2->font;

      u8g2_SetFont(u8g2, SYMBOL_FONT);  //save current font so we can return to it later

      u8g2_uint_t wifiSymbol = sigalStrength + WIFI_GLYPH_OFFSET;
      u8g2_uint_t batSymbol = batStatus + BAT_GLYPH_OFFSET;

      u8g2_uint_t wifiOffset = u8g2_GetGlyphWidth(u8g2,wifiSymbol);
      u8g2_uint_t batOffset = u8g2_GetGlyphWidth(u8g2,batSymbol);

      u8g2_DrawGlyph(u8g2, u8g2_GetDisplayWidth(u8g2) - wifiOffset, mui_get_y(ui)-2, wifiSymbol);
      u8g2_DrawGlyph(u8g2, u8g2_GetDisplayWidth(u8g2) - batOffset - wifiOffset - 2, mui_get_y(ui)-2, batSymbol);
      String temp = String(temperature,1)+ unitsText[units];

      u8g2_DrawUTF8(u8g2, u8g2_GetDisplayWidth(u8g2) - batOffset - wifiOffset - u8g2_GetStrWidth(u8g2,temp.c_str()) - 2,  mui_get_y(ui)-2, temp.c_str());
      u8g2_DrawHLine(u8g2, 0, mui_get_y(ui), u8g2_GetDisplayWidth(u8g2));

      u8g2_SetFont(u8g2, prevFont);

      
      break;
  }
  return 0;
}

uint8_t mui_temp(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {
    case MUIF_MSG_DRAW:

        String temp = String(temperature,1)+ unitsText[units];
        u8g2_DrawUTF8(u8g2,mui_get_x(ui),mui_get_y(ui), temp.c_str());
      break;
  }
  return 0;
}

uint8_t mui_date_time(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {
    case MUIF_MSG_DRAW:    
      {
        u8g2_DrawUTF8(u8g2,mui_get_x(ui),mui_get_y(ui), DateTime.format(dateFormatStrings[dateFormat]).c_str());
      }
      
      break;
  }
  return 0;
}

uint8_t mui_child_name (mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {
    case MUIF_MSG_DRAW:    
      {
        u8g2_DrawUTF8(u8g2,mui_get_x(ui),mui_get_y(ui), childName(currentChild));
      }
      
      break;
  }
  return 0;
}

void configPortalEnded()
{
  muiConfig.sendSelectWithExecuteOnSelectFieldSearch();
}

uint8_t mui_setup_wifi(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {
    case MUIF_MSG_FORM_START:
      startConfigPortal();

      break;
  }
  return 0;
}

uint8_t mui_start_activity(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
      startActivity(ui->arg);

      break;
  }
  return mui_u8g2_goto_form_w1_pi(ui,  msg);
}

uint8_t mui_end_activity(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {

    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
      stopActivity(ui->arg);

      break;
  }
  ui->arg = 1;
  return mui_u8g2_btn_goto_wm_fi(ui,  msg);
}

uint8_t mui_calibrate_scale(mui_t *ui, uint8_t msg) {
  float weight;
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {

    case MUIF_MSG_FORM_END:
      calibrateScale(ui->arg);
      break;
      
  }

  return 0;
}

uint8_t mui_measure(mui_t *ui, uint8_t msg) {
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {
    case MUIF_MSG_DRAW:

      String measurementText = String(measurement,1) + String(unitsText[units + FLUID_UNIT_OFFSET]);
      u8g2_DrawUTF8(u8g2,mui_get_x(ui),mui_get_y(ui), measurementText.c_str());
      
      break; 
  }

  return 0;
}

uint8_t mui_elapsed_time(mui_t *ui, uint8_t msg) {
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {

    case MUIF_MSG_DRAW:
      struct tm* t = localtime(&elapsedTime);
      char buf[64];
      strftime(buf, sizeof(buf), "T", t);
      u8g2_DrawUTF8(u8g2, mui_get_x(ui), mui_get_y(ui), buf);
      break; 
  }

  return 0;
}

uint8_t mui_feed_choose_next_form(mui_t *ui, uint8_t msg) {
  u8g2_t *u8g2 = mui_get_U8g2(ui);

  if(*currentFeedingMethod != 0)
  {
    String(" Begin ").toCharArray(ui->text,42);
    ui->arg = 22;
  }

  Serial.printf("%d\n", ui->arg);
  return mui_u8g2_btn_goto_wm_fi(ui,  msg);
}

uint8_t mui_feed_choose_end_form(mui_t *ui, uint8_t msg) {
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  
  if(*currentFeedingMethod == 0)
    return mui_u8g2_btn_goto_wm_fi(ui,  msg);

  return mui_end_activity(ui, msg);
}

uint8_t mui_config_options(mui_t *ui, uint8_t msg) {
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  
  if(ui->arg == 2)
  {

    return mui_u8g2_btn_exit_wm_fi(ui,  msg);
  }

  return mui_u8g2_goto_form_w1_pi(ui, msg);
}

uint8_t mui_exit_and_return(mui_t *ui, uint8_t msg) 
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  

  if(mui_u8g2_btn_exit_wm_fi(ui, msg))
  {
    activeUI = returnUI;
    return 1;
  }

  return 0;
}

uint8_t mui_exit_to_main(mui_t *ui, uint8_t msg) 
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  

  if(mui_u8g2_btn_exit_wm_fi(ui, msg))
  {
    activeUI = muiMain.getMUI();
    return 1;
  }

  return 0;
}

uint8_t mui_exit_to_config(mui_t *ui, uint8_t msg) 
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  

  if(mui_u8g2_btn_exit_wm_fi(ui, msg))
  {
    activeUI = muiConfig.getMUI();
    return 1;
  }

  return 0;
}

uint8_t mui_child_select(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);

  switch(msg)
  {
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
      currentChild = ui->arg + ui->form_scroll_top;
      if(currentChild < totalChildren)
      {
        
        mui_SaveForm(ui);
        mui_LeaveForm(ui);
        setActiveChild();
        activeUI = getCurrentChildMUI();
        return 0;
      }
  }

  return mui_u8g2_u16_list_goto_w1_pi(ui, msg);
}

uint8_t mui_save_child(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);

  switch(msg)
  {
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
      setDateOfBirth();
      addChild(&firstName[0], &lastName[0], &dateOfBirth[0]);
      break;
  }

  return mui_u8g2_btn_goto_wm_fi(ui, msg);
}

uint8_t mui_new_child(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);

  switch(msg)
  {
    case MUIF_MSG_FORM_START:
      firstName[0]= '\0'; 
      lastName[0]= '\0';
      dayPart = 0; 
      monthPart = 0; 
      yearPart = 0;
      break;
  }

  return mui_u8g2_btn_goto_wm_fi(ui, msg);
}

uint16_t dateFormats_get_cnt(void *data) {
  return sizeof(dateFormats)/sizeof(*dateFormats);    
}

const char * dateFormats_get_str(void *data, uint16_t index) {
  return dateFormats[index];
}

uint16_t children_get_cnt(void *data) {
  return totalChildren + 1;    
}

const char * childMenuString;
char  menuName[42];

static const char * children_get_str(void *data, uint16_t index) {
  const char *  name = "Add Child";
  
  if(index != totalChildren)
  {
    
      name = childName(index);
  }
  sprintf(menuName,MUI_2 "%s",name);

  return menuName;
}

uint16_t feedingMethods_get_cnt(void *data) {
  return feedingMethodsCount; 
}

const char * feedingMethods_get_str(void *data, uint16_t index) {
  return currentfeedingMethodText(index);
}

uint16_t feedingTypes_get_cnt(void *data) {
  return feedingTypesCount; 
}

const char * feedingTypes_get_str(void *data, uint16_t index) {
  return currentfeedingTypeText(index);
}

const char * dayList[] = 
{
  "DD","01","02","03","04","05",
  "06","07","08","09","10",
  "11","12","13","14","15",
  "16","17","18","19","20",
  "21","22","23","24","25",
  "26","27","28","29","30",
  "31",
};

uint16_t dayPart_get_cnt(void *data) {
  switch(monthPart)
  {
    case 2:

      if (isLeapYear(getYear(yearPart))) {
        return 30;
      }
      else {
        return 29;
      }
    case 4: case 6: case 9: case 11:
      return 31;
    default:
      return 32;   
  }
     
}

uint8_t mui_date_field(mui_t *ui, uint8_t msg)
{
  mui_u8g2_u16_list_line_wa_mse_pi(ui, msg);

  uint16_t validDate = dayPart_get_cnt(NULL);

  if(dayPart >= validDate - 1)
    dayPart = 0;

  return 0;
}

const char * dayPart_get_str(void *data, uint16_t index) 
{
  return dayList[index];
}
#define YEAR_CNT 5

const char * monthList[] = 
{
  "MM","01","02","03","04","05",
  "06","07","08","09","10",
  "11","12"
};

bool isLeapYear(uint16_t year)
{
  return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

uint16_t getYear(uint16_t index)
{
  return DateTime.getParts().getYear()- YEAR_CNT + index;
}

uint16_t monthPart_get_cnt(void *data) {
  return sizeof(dayList)/sizeof(*dayList);   
}

const char * monthPart_get_str(void *data, uint16_t index) {
  return monthList[index];
}



uint16_t yearPart_get_cnt(void *data) {
  return YEAR_CNT;   
}

const char * yearPart_get_str(void *data, uint16_t index) {
  if(index == 0)
    return "YYYY";

  return String(getYear(index)).c_str();
}

void setDateOfBirth()
{
  tm date;

  date.tm_year = DateTime.getParts().getYear() - 1900  - YEAR_CNT + yearPart;
  date.tm_mon = monthPart - 1;
  date.tm_mday = dayPart;

  strftime(dateOfBirth, 12, DateFormatter::DATE_ONLY, &date);
}

muif_t child_activity_muif_list[] = {
    MUIF_U8G2_FONT_STYLE(0, u8g2_font_helvR08_tf),
    MUIF_U8G2_FONT_STYLE(1, u8g2_font_helvB08_tr),

    MUIF_U8G2_LABEL(),

    MUIF_RO("GP",mui_u8g2_goto_data),
    MUIF_BUTTON("SA", mui_start_activity),
    MUIF_RO("SB", mui_status_bar),
    MUIF_RO("TP", mui_temp),
    MUIF_RO("DT", mui_date_time),
    MUIF_RO("MF", mui_measure),
    MUIF_RO("CN", mui_child_name),
    MUIF_BUTTON("EA", mui_end_activity),
    MUIF_BUTTON("FN", mui_feed_choose_next_form),
    MUIF_BUTTON("FE", mui_feed_choose_end_form),
    MUIF_BUTTON("CH", mui_exit_to_main),
  
    /* Feed Method muif */
    MUIF_U8G2_U16_LIST("FM",currentFeedingMethod, NULL, feedingMethods_get_str, feedingMethods_get_cnt, mui_u8g2_u16_list_line_wa_mse_pi),
    MUIF_U8G2_U16_LIST("FT",currentFeedingType, NULL, feedingTypes_get_str, feedingTypes_get_cnt, mui_u8g2_u16_list_line_wa_mse_pi),

    /* a button for the menu... */
    MUIF_BUTTON("GO", mui_u8g2_btn_goto_wm_fi)
    
};

fds_t child_activity_fds_data[] = 
/* Main Page */
MUI_FORM(1)
MUI_STYLE(0)
MUI_XY("SB", 0,11)
MUI_DATA("GP", 
    MUI_20 "Start Feeding|"
    MUI_30 "Start Sleeping|"
    MUI_40 "Start Tummy Time")
MUI_LABEL(5, 24, "Child:")
MUI_XY("CN", 35, 24) 
MUI_XYAT("CH", 125, 24, 60, " Change ") 
MUI_XYA("SA", 5, 36, 0) 
MUI_XYA("SA", 5, 48, 1) 
MUI_XYA("SA", 5, 60, 2) 

/* Feeding Start Page */
MUI_FORM(20)
MUI_STYLE(1)
MUI_LABEL(5, 24, "Feeding")
MUI_STYLE(0)
MUI_XY("SB", 0,11)
MUI_LABEL(5, 36, "Method:")
MUI_XYA("FM", 40, 36, 60)
MUI_LABEL(5, 48, "Type:")
MUI_XYA("FT", 40, 48, 40)
MUI_XYAT("FN", 20, 60, 21, " Next ") 

/* Feeding - Start Bottle Weigh in */
MUI_FORM(21)
MUI_STYLE(1)
MUI_LABEL(5, 24, "Feeding")
MUI_STYLE(0)
MUI_XY("SB", 0,11)
MUI_XY("SB", 0,11)
MUI_LABEL(5, 36, "Place bottle on scale")
MUI_XY("MF", 0,48)
MUI_XYAT("GO", 20, 60, 22, " Begin ") 

/* Feeding Page */
MUI_FORM(22)
MUI_STYLE(1)
MUI_LABEL(5, 36, "Feeding")
MUI_STYLE(0)
MUI_XY("SB", 0,11)
MUI_LABEL(5, 24, "Child:")
MUI_XY("CN", 35, 24) 
MUI_XYAT("CH", 125, 24, 60, " Change ") 
MUI_XY("TP", 20, 48) 
MUI_XYAT("FE", 20, 60, 23, " End ") 

/* Feeding - End Bottle Weigh in */
MUI_FORM(23)
MUI_STYLE(1)
MUI_LABEL(5, 24, "Feeding")
MUI_STYLE(0)
MUI_XY("SB", 0,11)
MUI_LABEL(5, 36, "Place bottle on scale")
MUI_XY("MF", 0,48)
MUI_XYAT("EA", 20, 60, 0, " Done ") 

/* sleeping Page */
MUI_FORM(30)
MUI_STYLE(1)
MUI_LABEL(5, 36, "Sleeping")
MUI_STYLE(0)
MUI_XY("SB", 0,11)
MUI_LABEL(5, 24, "Child:")
MUI_XY("CN", 35, 24) 
MUI_XYAT("CH", 125, 24, 60, " Change ") 
MUI_XY("TP", 20, 48) 
MUI_XYAT("EA", 20, 60, 1, " End ") 

/* Tummy Time Page */
MUI_FORM(40)
MUI_STYLE(1)
MUI_LABEL(5, 36, "Tummy Time")
MUI_STYLE(0)
MUI_XY("SB", 0,11)
MUI_LABEL(5, 24, "Child:")
MUI_XY("CN", 35, 24) 
MUI_XYAT("CH", 125, 24, 60, " Change ") 
MUI_XY("TP", 20, 48) 
MUI_XYAT("EA", 20, 60, 2, " End ") 
;

muif_t config_muif_list[] = {
    MUIF_U8G2_FONT_STYLE(0, u8g2_font_helvR08_tr),
    MUIF_U8G2_FONT_STYLE(1, u8g2_font_helvB08_tr),

    MUIF_U8G2_LABEL(),


    MUIF_RO("GP",mui_u8g2_goto_data),
    MUIF_BUTTON("GC", mui_config_options),
    MUIF_RO("HR", mui_hrule),
   
    /* wifi muif */
    MUIF_RO("CW", mui_setup_wifi),

    /* display muif */
    MUIF_VARIABLE("UN",&units,mui_u8g2_u8_opt_line_wa_mse_pi),
    MUIF_U8G2_U16_LIST("DT", &dateFormat, NULL, dateFormats_get_str, dateFormats_get_cnt, mui_u8g2_u16_list_line_wa_mse_pi),
    MUIF_U8G2_U16_LIST("DT", &dateFormat, NULL, dateFormats_get_str, dateFormats_get_cnt, mui_u8g2_u16_list_line_wa_mse_pi),
    MUIF_VARIABLE("TG",&units,mui_u8g2_u8_opt_line_wa_mse_pi),

    /* a button for the menu */
    MUIF_BUTTON("GO", mui_u8g2_btn_goto_wm_fi),
    MUIF_BUTTON("EX", mui_exit_and_return),
};

fds_t config_fds_data[] = 
/* Config Menu */
MUI_FORM(1)
MUI_STYLE(1)
MUI_LABEL(5, 8, "Config")
MUI_STYLE(0)
MUI_XY("HR", 0,11)
MUI_DATA("GP", 
    MUI_2 "WiFi|"
    MUI_3 "Display|"
    MUI_4 "Calibrate")
MUI_XYA("GC", 5, 24, 0) 
MUI_XYA("GC", 5, 36, 1) 
MUI_XYA("GC", 5, 48, 2) 
MUI_XYAT("EX", 5, 60, 0, " EXIT ") 

/* WIFI Setup */
MUI_FORM(2)
MUI_STYLE(1)
MUI_LABEL(5, 8, "Baby Budy Config")
MUI_STYLE(0)
MUI_XY("HR", 0,11)
MUI_LABEL(5,23, "Connect to BabyAP wifi")
MUI_LABEL(5,35, "Complete form that loads")
MUI_XYAT("GO", 20, 60, 1, " Back ") 

/* Display */
MUI_FORM(3)
MUI_STYLE(1)
MUI_LABEL(5, 8, "Display")
MUI_STYLE(0)
MUI_LABEL(5,23, "Units:")
MUI_LABEL(5,35, "Date Format:")
MUI_LABEL(5,48, "Timezone:")
MUI_XYAT("UN", 20, 23, 40, "Metric|Imperial")
MUI_XYA("DT", 20, 35, 20)
MUI_XYA("TZ", 20, 48, 20)
MUI_XYAT("GO", 20, 60, 1, " Back ") 
;

muif_t calibrate_muif_list[] = {
    MUIF_U8G2_FONT_STYLE(0, u8g2_font_helvR08_tr),
    MUIF_U8G2_FONT_STYLE(1, u8g2_font_helvB08_tr),

    MUIF_U8G2_LABEL(),

    MUIF_RO("HR", mui_hrule),
    MUIF_RO("MF", mui_measure),
   
    MUIF_RO("CB", mui_calibrate_scale),

    /* a button for the menu */
    MUIF_BUTTON("GO", mui_u8g2_btn_goto_wm_fi),
    MUIF_BUTTON("EX", mui_exit_to_config),
};

fds_t calibrate_fds_data[] = 
/* Calibrate */
MUI_FORM(70)
MUI_STYLE(1)
MUI_LABEL(5, 8, "Calibrate")
MUI_STYLE(0)
MUI_XY("HR", 0,11)
MUI_LABEL(5,23, "Place a container that can hold ")
MUI_LABEL(5,35, "more than 100mL on scale")
MUI_XYAT("GO", 20, 60, 71, " Next ") 

MUI_FORM(71)
MUI_STYLE(1)
MUI_LABEL(5, 8, "Calibrate")
MUI_STYLE(0)
MUI_XY("HR", 0,11)
MUI_LABEL(5,23, "Please add 50mL to the container")
MUI_XYAT("GO", 20, 60, 72, " Next ") 

MUI_FORM(72)
MUI_STYLE(1)
MUI_LABEL(5, 8, "Calibrate")
MUI_STYLE(0)
MUI_LABEL(5,23, "Please add 100mL to the container")
MUI_XYAT("GO", 20, 60, 73, " Next ") 

MUI_FORM(73)
MUI_STYLE(1)
MUI_LABEL(5, 8, "Calibrate")
MUI_STYLE(0)
MUI_XY("HR", 0,11)
MUI_LABEL(5,23, "Please tip out the container and fill ")
MUI_LABEL(5,35, "with 20mL. Place it back on scale")
MUI_XYAT("GO", 20, 60, 74, " Next ") 

MUI_FORM(74)
MUI_STYLE(1)
MUI_LABEL(5, 8, "Calibrate")
MUI_STYLE(0)
MUI_XY("HR", 0,11)
MUI_LABEL(5,23, "Measured: ")
MUI_XY("MF",40,23)
MUI_XYAT("EX", 20, 60, 0, " Done ") 
MUI_XYAT("GO", 20, 60, 70, " Retry ") 
;

muif_t main_muif_list[] = {
    MUIF_U8G2_FONT_STYLE(0, u8g2_font_helvR08_tf),
    MUIF_U8G2_FONT_STYLE(1, u8g2_font_helvB08_tf),

    MUIF_U8G2_LABEL(),

    MUIF_RO("SB", mui_status_bar),
    MUIF_RO("NC", mui_new_child),
    MUIF_U8G2_U16_LIST("GC",&currentChild,NULL,children_get_str,children_get_cnt, mui_child_select),
    MUIF_U8G2_STRING("FN",&firstName[0],42,MUI_STRING_ALPHA,mui_u8g2_string_wm_mud_pi),
    MUIF_U8G2_STRING("LN",&lastName[0],42,MUI_STRING_ALPHA,mui_u8g2_string_wm_mud_pi),
    MUIF_U8G2_U16_LIST("DD", &dayPart, NULL, dayPart_get_str, dayPart_get_cnt, mui_u8g2_u16_list_line_wa_mse_pi),
    MUIF_U8G2_U16_LIST("DM", &monthPart, NULL, monthPart_get_str, monthPart_get_cnt, mui_u8g2_u16_list_line_wa_mse_pi),
    MUIF_U8G2_U16_LIST("DY", &yearPart, NULL, yearPart_get_str, yearPart_get_cnt, mui_u8g2_u16_list_line_wa_mse_pi),
    MUIF_BUTTON("SC", mui_save_child),
    MUIF_BUTTON("GO", mui_u8g2_btn_goto_wm_fi),
};

fds_t main_fds_data[] = 

/* Select Active Child */
MUI_FORM(1)
MUI_STYLE(1)
MUI_STYLE(0)
MUI_XY("SB", 0,11)
MUI_XYA("GC", 5, 24, 0) 
MUI_XYA("GC", 5, 36, 1) 
MUI_XYA("GC", 5, 48, 2) 
MUI_XYA("GC", 5, 60, 3) 


/* Edit Child */
MUI_FORM(2)
MUI_STYLE(1)
MUI_XY("CM",5, 8)
MUI_STYLE(0)
MUI_XY("SB", 0,11)
MUI_AUX("NC")
MUI_LABEL(5,23, "First Name:")
MUI_LABEL(6,35, "Last Name:")
MUI_LABEL(5,48, "DoB:")
MUI_XYA("FN",60,23,7)
MUI_XYA("LN",60,35,7)
MUI_XYA("DD",60,48,15)
MUI_XYA("DM",80,48,20)
MUI_XYA("DY",100,48,20)
MUI_XYAT("SC",25,60,3, " Save ")
MUI_XYAT("RC",25,60,5, " Remove ")
MUI_XYAT("GO",125,60,1, " Cancel ")

/* Saving Child */
MUI_FORM(3)
MUI_STYLE(1)
MUI_LABEL(5, 35, "Saving...")
MUI_STYLE(0)
MUI_XY("SB", 0,11)

/* Save Child Failed */
MUI_FORM(4)
MUI_STYLE(1)
MUI_LABEL(5, 35, "Save Failed")
MUI_STYLE(0)
MUI_XY("SB", 0,11)
MUI_XYAT("GO", 35,60,1," OK ")

/* Confirm Remove Child */
MUI_FORM(5)
MUI_STYLE(1)
MUI_LABEL(5, 23, "Are you sure?")
MUI_STYLE(0)
MUI_LABEL(5, 35, "Cannot be undone")
MUI_LABEL(5, 48, "All data will be lost")
MUI_XY("SB", 0,11)
MUI_XYT("RC", 35,60," OK ")
MUI_XYAT("GO",125,60,1, " Cancel ")

/* Removing Child */
MUI_FORM(6)
MUI_STYLE(1)
MUI_LABEL(5, 35, "Removing...")
MUI_STYLE(0)
MUI_XY("SB", 0,11)

/* Remove Child Failed */
MUI_FORM(7)
MUI_STYLE(1)
MUI_LABEL(5, 35, "Remove Failed")
MUI_STYLE(0)
MUI_XY("SB", 0,11)
MUI_XYT("GO", 35,60," OK ")
;

void childAddOutcome(bool success)
{
  if(success)
  {
    muiMain.gotoForm(1,0);

  }
  else
  {
    muiMain.gotoForm(4,0);
  }

  redraw_screen = true;
}

void selectButtonClicked()
{
  Serial.println("select button pressed");
  if(mui_IsFormActive(activeUI))
  {
    mui_SendSelectWithExecuteOnSelectFieldSearch(activeUI);
    redraw_screen = true;
  }
}

void selectButtonHolding()
{
  if(selectButton.isLongPressed() && !heldBeeped)
  {
      Serial.println("select button held");
    
    if(!muiCalibrate.isFormActive() || !muiConfig.isFormActive())
    {
      returnUI = activeUI;
      mui_SaveForm(activeUI);
      mui_LeaveForm(activeUI);
      activeUI = muiConfig.getMUI();
      beep();
    }

    heldBeeped = true;
  }
}

void selectButtonHoldReleased()
{
   heldBeeped = false;
}

void mui_restore_or_start(mui_t *ui)
{
  if(ui->last_form_fds != NULL)
  {
    mui_RestoreForm(ui);
  }
  else
  {
    mui_GotoFormAutoCursorPosition(ui,1);
  }
}


void nextButtonClicked()
{
  Serial.println("next button pressed");
  if(mui_IsFormActive(activeUI))
  {
    mui_NextField(activeUI);
    redraw_screen = true;
  }
}

void prevButtonClicked()
{
  Serial.println("previous button pressed");
  if(mui_IsFormActive(activeUI))
  {
    mui_PrevField(activeUI);
    redraw_screen = true;
  }
}

void beep()
{
  buzzer.beep(DURATION);
}

//call this to initialise the menu of a new child record
void initialiseChildUI(uint16_t index)
{
  mui_Init(getCurrentChildMUI(index), (void *)u8g2.getU8g2(), child_activity_fds_data, child_activity_muif_list, sizeof(child_activity_muif_list)/sizeof(muif_t));
}

void initialiseUI()
{
  selectButton = OneButton(SELECT_PIN);
  nextButton = OneButton(NEXT_PIN);
  prevButton = OneButton(PREV_PIN);

  selectButton.attachClick(selectButtonClicked);
  selectButton.attachDuringLongPress(selectButtonHolding);
  selectButton.attachLongPressStop(selectButtonHoldReleased);
  heldBeeped = false;
  nextButton.attachClick(nextButtonClicked);
  prevButton.attachClick(prevButtonClicked);
  
Serial.println("buttons started");
  u8g2.begin();

  muiMain.begin(u8g2, main_fds_data, main_muif_list, sizeof(main_muif_list)/sizeof(muif_t));
  muiConfig.begin(u8g2, config_fds_data, config_muif_list, sizeof(config_muif_list)/sizeof(muif_t));
  muiCalibrate.begin(u8g2, calibrate_fds_data, calibrate_muif_list, sizeof(calibrate_muif_list)/sizeof(muif_t));

  for(uint16_t i=0; i < totalChildren; i++)
  {
    Serial.printf("start child %d", i);
    initialiseChildUI(i);
  }
Serial.println("child menus done");
  activeUI = muiMain.getMUI();

}


void updateUI()
{

    /* check whether the menu is active */
  if ( mui_IsFormActive(activeUI)) {

    /* if so, then draw the menu */
    if ( redraw_screen ) {
          
      /* update the display */
      u8g2.firstPage();
      do {
          mui_Draw(activeUI);
      } while( u8g2.nextPage() );
      //u8g2.w
      redraw_screen = false;
    }
  }
  else 
  {
    /* a menu should always be active, by default thsi should be the main one */
    mui_restore_or_start(activeUI);
    
    redraw_screen = true;
  }

  selectButton.tick();
  nextButton.tick();
  prevButton.tick();
  buzzer.loop();
}

#endif
