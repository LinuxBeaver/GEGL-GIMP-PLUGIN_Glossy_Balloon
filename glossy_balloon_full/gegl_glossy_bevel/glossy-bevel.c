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

property_string (string3, _("Remove Transparency"), TUTORIAL3)
    ui_meta     ("role", "output-extent")




#define TUTORIAL2 \
" id=3 screen aux=[   ref=3 emboss  type=bumpmap azimuth=30  elevation=15 ] median-blur  percentile=90 alpha-percentile=100 gaussian-blur std-dev-x=1 std-dev-y=1 filter=fir id=3 screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] median-blur  percentile=50 alpha-percentile=100 screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] median-blur  percentile=50 alpha-percentile=100 screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ]  "\


#define TUTORIAL3 \
" id=forceopacity over  aux=[ ref=forceopacity ] id=makeopacity over  aux=[ ref=makeopacity ] id=forceopacity over  aux=[ ref=forceopacity ] id=makeopacity over  aux=[ ref=makeopacity ]  id=forceopacity over  aux=[ ref=forceopacity ] id=makeopacity over  aux=[ ref=makeopacity ] id=forceopacity over  aux=[ ref=forceopacity ] "\


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

property_double (opacityall, _("Reduce transparency puff around edges"), 5.0)
    description (_("Global opacity value that is always used on top of the optional auxiliary input buffer."))
    value_range (1, 5.0)
    ui_range    (1.0, 5.0)

property_boolean (removetransparency, _("Entirely Remove transparent puff on edges"), FALSE)
  description    (_("In Pango Markup mode this will clip the borders of letters. So disable it there."))








#else

#define GEGL_OP_META
#define GEGL_OP_NAME     glossy_bevel
#define GEGL_OP_C_SOURCE glossy-bevel.c

#include "gegl-op.h"

typedef struct
{
  GeglNode *input;
  GeglNode *nop;
  GeglNode *graph;
  GeglNode *blur;
  GeglNode *graph2;
  GeglNode *hue;
  GeglNode *saturation;
  GeglNode *multiply;
  GeglNode *opacityall;
  GeglNode *layer;
  GeglNode *removetransparency;
  GeglNode *output;
} State; 




static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *input, *output, *blur, *graph, *graph2, *removetransparency, *nop, *hue,  *layer, *saturation, *opacityall, *multiply;

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

 removetransparency   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl",
                                  NULL);

 hue   = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

 layer   = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer",
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

 nop   = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);



  gegl_operation_meta_redirect (operation, "gaus", blur, "std-dev-x");
  gegl_operation_meta_redirect (operation, "gaus", blur, "std-dev-y");
  gegl_operation_meta_redirect (operation, "hue", hue, "hue");
  gegl_operation_meta_redirect (operation, "lightness", hue, "lightness");
  gegl_operation_meta_redirect (operation, "src", layer, "src");
  gegl_operation_meta_redirect (operation, "saturation", saturation, "scale");
  gegl_operation_meta_redirect (operation, "string",  graph, "string");
  gegl_operation_meta_redirect (operation, "string2",  graph2, "string");
  gegl_operation_meta_redirect (operation, "string3",  removetransparency, "string");
  gegl_operation_meta_redirect (operation, "opacityall",  opacityall, "value");




  gegl_node_link_many (input, graph, blur, graph2, hue, saturation, multiply, opacityall, output, NULL);
  gegl_node_connect_from (multiply, "aux", layer, "output");
  gegl_node_link_many (layer, nop,  NULL);


 /* Now save points to the various gegl nodes so we can rewire them in
   * update_graph() later
   */
  State *state = g_malloc0 (sizeof (State));
  state->input = input;
  state->graph = graph;
  state->blur = blur;
  state->graph2 = graph2;
  state->hue = hue;
  state->saturation = saturation;
  state->multiply = multiply;
  state->opacityall = opacityall;
  state->layer = layer;
  state->nop = nop;
  state->removetransparency = removetransparency;
  state->output = output;
  o->user_data = state;
}

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

  if (o->removetransparency)
  {
  gegl_node_link_many (state->input, state->graph, state->blur, state->graph2, state->removetransparency, state->hue, state->saturation, state->multiply,  state->output, NULL);
  gegl_node_connect_from (state->multiply, "aux", state->layer, "output");
  gegl_node_link_many (state->layer, state->nop, NULL);
  }
  else
  {
  gegl_node_link_many (state->input, state->graph, state->blur, state->graph2, state->hue, state->saturation, state->multiply, state->opacityall,  state->output, NULL);
  gegl_node_connect_from (state->multiply, "aux", state->layer, "output");
  gegl_node_link_many (state->layer, state->nop, NULL);
  }
}


static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class = GEGL_OPERATION_CLASS (klass);
   GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);

  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

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
