#include <stdlib.h>
#include <gtk/gtk.h>

const char *pszLorem = "@Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer id malesuada felis. Suspendisse egestas nulla dui, quis sollicitudin odio porta eget. @Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; In bibendum risus vitae eleifend accumsan. Cras id elit congue, gravida velit at, rhoncus magna. Aliquam quis dolor dui. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos.";

PangoLayout     *Pango;

void render (cairo_t *cr, PangoAttrShape *pShape, gboolean do_path, gpointer data)
{
    cairo_surface_t *img = (cairo_surface_t *)pShape->data;
    double  dx, dy;

    cairo_get_current_point(cr, &dx, &dy);
    cairo_set_source_surface(cr, img, dx, dy);
    cairo_rectangle (cr, dx, dy, pShape->ink_rect.width/PANGO_SCALE, pShape->ink_rect.height/PANGO_SCALE);
    cairo_fill(cr);
}

gboolean onDraw (GtkWidget *pWidget, cairo_t *cr, gpointer user_data)
{
    pango_cairo_show_layout (cr, Pango);
    return 1;
}

void init_drawingArea (GtkWidget *pWidget)
{
    cairo_surface_t *pImg = cairo_image_surface_create_from_png ("linux.png");
    PangoRectangle   r = {0, 0, PANGO_SCALE * cairo_image_surface_get_width (pImg),
                                PANGO_SCALE * cairo_image_surface_get_height(pImg)};
    PangoContext    *ctxt = gtk_widget_get_pango_context (pWidget);
    PangoAttrList   *attList = pango_attr_list_new();
    PangoAttribute  *attr;

    Pango = pango_layout_new (ctxt);

    pango_cairo_context_set_shape_renderer (ctxt, render, NULL, NULL);
    pango_layout_set_text (Pango, pszLorem, -1);
    pango_layout_set_width(Pango, PANGO_SCALE * 800);
    attr = pango_attr_shape_new_with_data(&r, &r, pImg, NULL, NULL);
    attr->start_index = 0; attr->end_index = 1;
    pango_attr_list_insert (attList, attr);

    attr = pango_attr_shape_new_with_data(&r, &r, pImg, NULL, NULL);
    attr->start_index = 152; attr->end_index = 153;
    pango_attr_list_insert (attList, attr);

    pango_layout_set_attributes (Pango, attList);
}

int main (int argc, char *argv[])
{
    /* Initialize GTK+ */
    gtk_init (&argc, &argv);

    /* Create the main window */
    GtkWidget *win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (win, 800, 400);
    g_signal_connect (win, "destroy", gtk_main_quit, NULL);

    //  Add a user drawing area and handle realize and draw signals
    GtkWidget   *userArea = gtk_drawing_area_new ();
    gtk_container_add (GTK_CONTAINER (win), userArea);
//    g_signal_connect (userArea, "realize", onRealize, NULL);
    g_signal_connect (userArea, "draw", onDraw, NULL);

    init_drawingArea (userArea);
    gtk_widget_realize (win);

    /* Enter the main loop */
    gtk_widget_show_all (win);
    gtk_main ();
    return 0;
}
