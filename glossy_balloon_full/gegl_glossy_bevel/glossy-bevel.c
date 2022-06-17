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
 * 2022 Beaver GEGL Glossy Baloon
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES



property_double (gaus, _("Balloonification of text"), 6.0)
   description  (_("The lower the less balloonification. The higher the more balloonification.'"))
  value_range (0.5, 20.0)
  ui_range (0.5, 14)
  ui_gamma (1.5)

property_int (radius1, _("Radius"), 2)
   description(_("A mild box blur to smooth rough edges"))
   value_range (1, 3)
   ui_range    (1, 3)
   ui_gamma   (1.5)

property_double (hue, _("Color rotation"),  0.0)
   description  (_("Color rotation. Don't like being locked into only a few colors? Manually desaturate and apply a color effect'"))
   value_range  (-180.0, 180.0)

property_double (lightness, _("Lightness"), 0.0)
   description  (_("Lightness adjustment"))
   value_range  (-15.0, 15.0)

property_double (saturation, _("Desaturation for Image File Upload"), 1)
   description  (_("Saturation"))
  ui_range (0.0, 1.0)
   value_range  (0.0, 1.0)

property_file_path(src, _("Image file overlay (Desaturation  strongly recommended)"), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...)"))



#else

#define GEGL_OP_META
#define GEGL_OP_NAME     glossy_bevel
#define GEGL_OP_C_SOURCE glossy-bevel.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *blur, *graph, *graph2, *hue, *box, *layer, *saturation;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

   blur = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);

 graph   = gegl_node_new_child (gegl,
                                  "operation", "gegl:zglossy",
                                  NULL);

 graph2   = gegl_node_new_child (gegl,
                                  "operation", "gegl:zglossy2",
                                  NULL);



 box   = gegl_node_new_child (gegl,
                                  "operation", "gegl:box-blur",
                                  NULL);


 hue   = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

 layer   = gegl_node_new_child (gegl,
                                  "operation", "gegl:zzmlayer",
                                  NULL);

 saturation   = gegl_node_new_child (gegl,
                                  "operation", "gegl:saturation",
                                  NULL);




  gegl_node_link_many (input, graph, blur, graph2, box, hue, saturation, layer, output, NULL);

  gegl_operation_meta_redirect (operation, "gaus", blur, "std-dev-x");
  gegl_operation_meta_redirect (operation, "gaus", blur, "std-dev-y");
  gegl_operation_meta_redirect (operation, "radius1", box, "radius");
  gegl_operation_meta_redirect (operation, "hue", hue, "hue");
  gegl_operation_meta_redirect (operation, "lightness", hue, "lightness");
  gegl_operation_meta_redirect (operation, "src", layer, "src");
  gegl_operation_meta_redirect (operation, "saturation", saturation, "scale");





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
