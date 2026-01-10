#include "temp_widget.h"

#define BAR_MIN 0
#define BAR_MAX 40

#define BAR_WIDTH 20
#define BAR_HEIGHT 150

#define ANIM_DURATION 2000

static void set_temp(void *bar, int32_t temp) {
    lv_bar_set_value((lv_obj_t *)bar, temp, LV_ANIM_ON);
}

// gradient from blue to red, looks neat
static void init_style(lv_style_t *style) {
    lv_style_init(style);
    lv_style_set_bg_opa(style, LV_OPA_COVER);
    lv_style_set_bg_color(style, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_bg_grad_color(style, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_bg_grad_dir(style, LV_GRAD_DIR_VER);
}

static void event_cb(lv_event_t *e) {
    lv_obj_t *obj = lv_event_get_target_obj(e);

    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    label_dsc.font = LV_FONT_DEFAULT;

    char buf[8];
    int val = lv_bar_get_value(obj);
    lv_snprintf(buf, sizeof(buf), "%d", val);

    lv_point_t txt_size;
    lv_text_get_size(&txt_size, buf, label_dsc.font, label_dsc.letter_space,
                     label_dsc.line_space, LV_COORD_MAX, label_dsc.flag);

    lv_area_t bar_area;
    lv_obj_get_coords(obj, &bar_area);
    lv_coord_t bar_height = lv_area_get_height(&bar_area);

    lv_area_t indic_area;
    lv_coord_t indic_height = bar_height * val / BAR_MAX;

    indic_area.x1 = bar_area.x1;
    indic_area.x2 = bar_area.x2;
    indic_area.y2 = bar_area.y2;
    indic_area.y1 = bar_area.y2 - indic_height;

    lv_area_t txt_area;
    txt_area.x1 = 0;
    txt_area.x2 = txt_size.x - 1;
    txt_area.y1 = 0;
    txt_area.y2 = txt_size.y - 1;

    int32_t h = lv_area_get_height(&indic_area);
    int32_t thresh = txt_size.y * 2;

    if (h < thresh) {
        lv_area_align(&indic_area, &txt_area, LV_ALIGN_OUT_TOP_MID, 0, 0);
        label_dsc.color = lv_color_black();
    } else if (h <= thresh * 2 - 5) {
        lv_area_t fixed_area = bar_area;
        fixed_area.y1 = bar_area.y2 - thresh;
        fixed_area.y2 = fixed_area.y1;

        lv_area_align(&fixed_area, &txt_area, LV_ALIGN_OUT_TOP_MID, 0, 0);
        if (h < thresh + txt_size.y) {
            label_dsc.color = lv_color_black();
        } else {
            label_dsc.color = lv_color_white();
        }
    } else {
        lv_area_align(&indic_area, &txt_area, LV_ALIGN_TOP_MID, 0, 10);
        label_dsc.color = lv_color_white();
    }

    label_dsc.text = buf;
    label_dsc.text_local = true;
    lv_layer_t *layer = lv_event_get_layer(e);
    lv_draw_label(layer, &label_dsc, &txt_area);
}

static void set_animation(lv_obj_t *bar) {
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, set_temp);
    lv_anim_set_duration(&a, ANIM_DURATION);
    lv_anim_set_reverse_duration(&a, ANIM_DURATION);
    lv_anim_set_var(&a, bar);
    lv_anim_set_values(&a, BAR_MIN, BAR_MAX);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);
}

void temp_widget_init(lv_display_t *disp) {
    lv_obj_t *scr = lv_display_get_screen_active(disp);

    lv_lock();

    lv_obj_t *new_bar = lv_bar_create(scr);

    static lv_style_t style;
    init_style(&style);
    lv_obj_add_style(new_bar, &style, LV_PART_INDICATOR);

    lv_obj_t *lbl = lv_label_create(scr);
    lv_label_set_text_static(lbl, "°C");

    lv_obj_set_size(new_bar, BAR_WIDTH, BAR_HEIGHT);
    lv_obj_center(new_bar);
    lv_bar_set_range(new_bar, BAR_MIN, BAR_MAX);

    lv_obj_align_to(lbl, new_bar, LV_ALIGN_OUT_TOP_MID, 0, 0);

    lv_obj_add_event_cb(new_bar, event_cb, LV_EVENT_DRAW_MAIN_END, NULL);

    set_animation(new_bar);

    lv_unlock();
}
