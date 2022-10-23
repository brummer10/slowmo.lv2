
#define CONTROLS 8

#define GUI_ELEMENTS 2

#define TAB_ELEMENTS 0


#define PLUGIN_UI_URI "urn:brummer:slowmo_ui"


#include "lv2_plugin.h"


#include "xresources.h"

void set_costum_theme(Widget_t *w) {
    w->color_scheme->normal = (Colors) {
         /* cairo    / r  / g  / b  / a  /  */
        .fg =       { 0.850, 0.850, 0.850, 1.000},
        .bg =       { 0.094, 0.267, 0.364, 1.000},
        .base =     { 0.000, 0.000, 0.000, 1.000},
        .text =     { 0.900, 0.900, 0.900, 1.000},
        .shadow =   { 0.000, 0.000, 0.000, 0.200},
        .frame =    { 0.000, 0.000, 0.000, 1.000},
        .light =    { 0.100, 0.100, 0.100, 1.000}
    };

    w->color_scheme->prelight = (Colors) {
         /* cairo    / r  / g  / b  / a  /  */
        .fg =       { 1.000, 0.000, 1.000, 1.000},
        .bg =       { 0.250, 0.250, 0.250, 1.000},
        .base =     { 0.300, 0.300, 0.300, 1.000},
        .text =     { 1.000, 1.000, 1.000, 1.000},
        .shadow =   { 0.100, 0.100, 0.100, 0.400},
        .frame =    { 0.300, 0.300, 0.300, 1.000},
        .light =    { 0.300, 0.300, 0.300, 1.000}
    };

    w->color_scheme->selected = (Colors) {
         /* cairo    / r  / g  / b  / a  /  */
        .fg =       { 0.900, 0.900, 0.900, 1.000},
        .bg =       { 0.200, 0.200, 0.200, 1.000},
        .base =     { 0.500, 0.180, 0.180, 1.000},
        .text =     { 1.000, 1.000, 1.000, 1.000},
        .shadow =   { 0.800, 0.180, 0.180, 0.200},
        .frame =    { 0.500, 0.180, 0.180, 1.000},
        .light =    { 0.500, 0.180, 0.180, 1.000}
    };

    w->color_scheme->active = (Colors) {
         /* cairo    / r  / g  / b  / a  /  */
        .fg =       { 0.000, 1.000, 1.000, 1.000},
        .bg =       { 0.000, 0.000, 0.000, 1.000},
        .base =     { 0.180, 0.380, 0.380, 1.000},
        .text =     { 0.750, 0.750, 0.750, 1.000},
        .shadow =   { 0.180, 0.380, 0.380, 0.500},
        .frame =    { 0.180, 0.380, 0.380, 1.000},
        .light =    { 0.180, 0.380, 0.380, 1.000}
    };

    w->color_scheme->insensitive = (Colors) {
         /* cairo    / r  / g  / b  / a  /  */
        .fg =       { 0.850, 0.850, 0.850, 0.500},
        .bg =       { 0.100, 0.100, 0.100, 0.500},
        .base =     { 0.000, 0.000, 0.000, 0.500},
        .text =     { 0.900, 0.900, 0.900, 0.500},
        .shadow =   { 0.000, 0.000, 0.000, 0.100},
        .frame =    { 0.000, 0.000, 0.000, 0.500},
        .light =    { 0.100, 0.100, 0.100, 0.500}
    };
}

#include "lv2_plugin.cc"



void plugin_value_changed(X11_UI *ui, Widget_t *w, PortIndex index) {
    // do special stuff when needed
}

void plugin_set_window_size(int *w,int *h,const char * plugin_uri, float scale) {
    (*w) = 771 * scale; //set initial width of main window
    (*h) = 220 * scale; //set initial height of main window
}

const char* plugin_set_name() {
    return "slowmo"; //set plugin name to display on UI
}

void plugin_create_controller_widgets(X11_UI *ui, const char * plugin_uri, float scale) {
    set_costum_theme(ui->win);
    ui->elem[0] = add_lv2_frame (ui->elem[0], ui->win, -1, "Slow Gate", ui, 105,  43, 366 * scale, 120 * scale);
    ui->elem[1] = add_lv2_frame (ui->elem[1], ui->win, -1, "Delay", ui, 475,  42, 287 * scale, 120 * scale);
    ui->widget[0] = add_lv2_image_toggle (ui->widget[0], ui->win, 2, "Bypass", ui, 29,  57, 60 * scale, 80 * scale);
    widget_get_png(ui->widget[0], LDVAR(switch_gbxc_png));

    ui->widget[1] = add_lv2_knob (ui->widget[1], ui->elem[0], 3, "DOWNTIME", ui, 19,  15, 65 * scale, 80 * scale);
    widget_get_png(ui->widget[1], LDVAR(knob_sprite_caccaebi_png));
    set_adjustment(ui->widget[1]->adj, 5.0, 5.0, 0.0, 1000.0, 1.0, CL_CONTINUOS);

    ui->widget[2] = add_lv2_knob (ui->widget[2], ui->elem[0], 4, "TRESHOLD", ui, 109,  15, 65 * scale, 80 * scale);
    widget_get_png(ui->widget[2], LDVAR(knob_sprite_caccaebi_png));
    set_adjustment(ui->widget[2]->adj, 1.0, 1.0, 0.0, 2.0, 0.01, CL_CONTINUOS);

    ui->widget[3] = add_lv2_knob (ui->widget[3], ui->elem[0], 5, "UPTIME", ui, 199,  15, 65 * scale, 80 * scale);
    widget_get_png(ui->widget[3], LDVAR(knob_sprite_caccaebi_png));
    set_adjustment(ui->widget[3]->adj, 100.0, 100.0, 0.0, 1000.0, 1.0, CL_CONTINUOS);

    ui->widget[4] = add_lv2_knob (ui->widget[4], ui->elem[0], 6, "VOLUME", ui, 289,  15, 65 * scale, 80 * scale);
    widget_get_png(ui->widget[4], LDVAR(knob_sprite_caccaebi_png));
    set_adjustment(ui->widget[4]->adj, 6.0, 6.0, -20.0, 20.0, 0.1, CL_CONTINUOS);

    ui->widget[5] = add_lv2_knob (ui->widget[5], ui->elem[1], 7, "BPM", ui, 37,  20, 60 * scale, 80 * scale);
    widget_get_png(ui->widget[5], LDVAR(knob_sprite_caccaebi_png));
    set_adjustment(ui->widget[5]->adj, 120.0, 120.0, 24.0, 360.0, 1.0, CL_CONTINUOS);

    ui->widget[6] = add_lv2_knob (ui->widget[6], ui->elem[1], 8, "GAIN", ui, 112,  20, 60 * scale, 80 * scale);
    widget_get_png(ui->widget[6], LDVAR(knob_sprite_caccaebi_png));
    set_adjustment(ui->widget[6]->adj, 0.0, 0.0, -20.0, 20.0, 0.1, CL_CONTINUOS);

    ui->widget[7] = add_lv2_knob (ui->widget[7], ui->elem[1], 9, "FEEDBACK", ui, 187,  20, 60 * scale, 80 * scale);
    widget_get_png(ui->widget[7], LDVAR(knob_sprite_caccaebi_png));
    set_adjustment(ui->widget[7]->adj, 0.5, 0.5, 0.0, 1.0, 0.01, CL_CONTINUOS);


    set_widget_color(ui->elem[0], 0, 1,0.100, 0.100, 0.100, 1.000);

    set_widget_color(ui->elem[1], 0, 1,0.100, 0.100, 0.100, 1.000);

    set_widget_color(ui->widget[0], 0, 1,0.100, 0.100, 0.100, 1.000);

    set_widget_color(ui->widget[1], 0, 1,0.100, 0.100, 0.100, 1.000);

    set_widget_color(ui->widget[2], 0, 1,0.100, 0.100, 0.100, 1.000);

    set_widget_color(ui->widget[3], 0, 1,0.100, 0.100, 0.100, 1.000);

    set_widget_color(ui->widget[4], 0, 1,0.100, 0.100, 0.100, 1.000);

    set_widget_color(ui->widget[5], 0, 1,0.100, 0.100, 0.100, 1.000);

    set_widget_color(ui->widget[6], 0, 1,0.100, 0.100, 0.100, 1.000);
}

void plugin_cleanup(X11_UI *ui) {
    // clean up used sources when needed
}


void plugin_port_event(LV2UI_Handle handle, uint32_t port_index,
                        uint32_t buffer_size, uint32_t format,
                        const void * buffer) {
    // port value change message from host
    // do special stuff when needed
}

