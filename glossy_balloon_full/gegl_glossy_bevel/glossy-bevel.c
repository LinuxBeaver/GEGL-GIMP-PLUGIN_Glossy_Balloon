/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2006 Øyvind Kolås <pippin@gimp.org>
 * 2022 Beaver GEGL Glossy Balloon
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES




property_string (string, _("Graph1"), TUTORIAL)
    ui_meta     ("role", "output-extent")

#define TUTORIAL \
" color-overlay value=#f587ff median-blur  percentile=90 alpha-percentile=100 "\

property_string (string2, _("Graph2"), TUTORIAL2)
    ui_meta     ("role", "output-extent")

#define TUTORIAL2 \
" id=3 screen aux=[   ref=3 emboss  type=bumpmap azimuth=30  elevation=15 ] median-blur  percentile=90 alpha-percentile=100 gaussian-blur std-dev-x=1 std-dev-y=1 filter=fir id=3 screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] median-blur  percentile=50 alpha-percentile=100 screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] median-blur  percentile=50 alpha-percentile=100 screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] id=forceopacity dst-in  aux=[ ref=forceopacity ] id=makeopacity over  aux=[ ref=makeopacity ] id=forceopacity dst-in  aux=[ ref=forceopacity ] id=makeopacity over  aux=[ ref=makeopacity ]  id=forceopacity dst-in  aux=[ ref=forceopacity ] id=makeopacity over  aux=[ ref=makeopacity ] id=forceopacity dst-in  aux=[ ref=forceopacity ] id=makeopacity over  aux=[ ref=makeopacity ]  "\



property_double (gaus, _("Balloonification of text"), 6.0)
   description  (_("The lower the less balloonification. The higher the more balloonification.'"))
  value_range (0.5, 20.0)
  ui_range (0.5, 14)
  ui_gamma (1.5)

property_double (hue, _("Color rotation"),  0.0)
   description  (_("Color rotation. Don't like being locked into only a few colors? Desaturate and apply a color effect'"))
   value_range  (-180.0, 180.0)

property_double (lightness, _("Lightness"), -7)
   description  (_("Lightness adjustment"))
   value_range  (-15.0, 15)

property_double (saturation, _("Desaturation for Image File Upload"), 1.2)
   description  (_("Saturation"))
  ui_range (0.0, 1.5)
   value_range  (0.0, 1.5)

property_file_path(src, _("Image file overlay (Desaturation and bright light recommended)"), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...)"))

property_double (opacityall, _("Slide up to remove transparency puff around edges"), 1.0)
    description (_("Global opacity value that is always used on top of the optional auxiliary input buffer."))
    value_range (1, 2)
    ui_range    (1.0, 2)



property_double (opacity, _("Opacity of image file overlay"), 1.0)
    description (_("Global opacity value that is always used on top of the optional auxiliary input buffer."))
    value_range (0, 1.0)
    ui_range    (0.0, 1.0)
    ui_meta     ("role", "output-extent")

property_double (sharpen, _("Sharpen (only works on image overlays)"), 0.0)
    description(_("Scaling factor for unsharp-mask, the strength of effect"))
    value_range (0.0, 2.5)
    ui_range    (0.0, 2.5)
    ui_gamma    (3.0)
    ui_meta     ("role", "output-extent")

property_int (radius1, _("Blur to smooth edges (works best for image file overlays)"), 0)
   description(_("A mild box blur to smooth rough edges"))
   value_range (1, 2)
   ui_range    (1, 2)
   ui_gamma   (1.5)
    ui_meta     ("role", "output-extent")




#else

#define GEGL_OP_META
#define GEGL_OP_NAME     glossy_bevel
#define GEGL_OP_C_SOURCE glossy-bevel.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *blur, *graph, *graph2, *hue, *box, *layer, *opacity, *sharpen, *saturation, *opacityall, *multiply;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

  blur    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
   "filter", 1,
                                  NULL);

 graph   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl",
                                  NULL);

 graph2   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl",
                                  NULL);



 box   = gegl_node_new_child (gegl,
                                  "operation", "gegl:box-blur",
                                  NULL);

 sharpen   = gegl_node_new_child (gegl,
                                  "operation", "gegl:unsharp-mask",
                                  NULL);


 hue   = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

 layer   = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer",
                                  NULL);

 opacity   = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

 opacityall   = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);




 saturation   = gegl_node_new_child (gegl,
                                  "operation", "gegl:saturation",
                                  NULL);

 multiply   = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);




  gegl_node_link_many (input, graph, blur, graph2, box, hue, saturation, multiply, opacityall, output, NULL);
  gegl_node_connect_from (multiply, "aux", sharpen, "output");
  gegl_node_link_many (layer, sharpen, NULL);



  gegl_operation_meta_redirect (operation, "gaus", blur, "std-dev-x");
  gegl_operation_meta_redirect (operation, "gaus", blur, "std-dev-y");
  gegl_operation_meta_redirect (operation, "radius1", box, "radius");
  gegl_operation_meta_redirect (operation, "hue", hue, "hue");
  gegl_operation_meta_redirect (operation, "lightness", hue, "lightness");
  gegl_operation_meta_redirect (operation, "src", layer, "src");
  gegl_operation_meta_redirect (operation, "saturation", saturation, "scale");
  gegl_operation_meta_redirect (operation, "string",  graph, "string");
  gegl_operation_meta_redirect (operation, "string2",  graph2, "string");
  gegl_operation_meta_redirect (operation, "sharpen",  sharpen, "scale");
  gegl_operation_meta_redirect (operation, "opacityall",  opacityall, "value");





}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:glossy-balloon",
    "title",       _("Glossy Balloon Text Generation"),
    "categories",  "Aristic",
    "reference-hash", "45a35416a10a512570f0f25sb2ac",
    "description", _("Make Glossy Bevel text with GEGL. This will not work well on small text. "
                     ""),
    NULL);
}

#endif
