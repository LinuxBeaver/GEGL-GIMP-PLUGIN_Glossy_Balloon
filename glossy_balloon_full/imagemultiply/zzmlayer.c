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
 *2022 Beaver ZZ crap
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_file_path(src, _("Source"), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...)"))

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     zzmlayer
#define GEGL_OP_C_SOURCE zzmlayer.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *multiply, *layer;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

  multiply = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

  layer    = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer",
                                  NULL);

  gegl_node_link_many (input, multiply, output, NULL);
  gegl_node_link (input, layer);
  gegl_node_connect_from (multiply, "aux", layer, "output");

  gegl_node_connect_from (multiply, "aux", layer, "output");

  gegl_operation_meta_redirect (operation, "src", layer, "src");

}
static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:zzmlayer",
    "title",       _("Multiplication of image upload"),
    "categories",  "hidden",
    "reference-hash", "13ed5656a5g38a512570xfc1a001bvva",
    "description", _("Filter only exist to power Beaver's custom GEGL filters'"),
    NULL);
}

#endif
