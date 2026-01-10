#ifndef TEMP_WIDGET_H
#define TEMP_WIDGET_H

#include "lvgl.h"

void temp_widget_init(lv_display_t *disp);
void temp_widget_init_on_container(lv_obj_t *container);

void temp_bar_set_val(int32_t temp);
void temp_bar_animate_to_val(int32_t temp);
lv_obj_t *get_ui_bar();

#endif
