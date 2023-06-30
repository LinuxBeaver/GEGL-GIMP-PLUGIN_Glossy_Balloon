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

/* This is a Glossy Balloon's GEGL Graph made in mid 2022 with some modifications.
 It is not 100% the same but the closest I can find in my archives. So this is how you can test this filter without installing it.

 
color-overlay value=#f587ff median-blur  percentile=90 alpha-percentile=100 
median-blur  percentile=90 alpha-percentile=290
gaussian-blur std-dev-x=4 std-dev-y=4
id=3
screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] 
median-blur  percentile=90 alpha-percentile=290
gaussian-blur std-dev-x=4 std-dev-y=4
id=3
screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] 
median-blur  percentile=90 alpha-percentile=290
screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] 
median-blur  percentile=90 alpha-percentile=290
screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] 
opacity value=10
median-blur radius=0
*/

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

#define TUTORIAL \
" color-overlay value=#f587ff median-blur  percentile=90 alpha-percentile=100 "\
/* This GEGL Graph is the base pink color that is needed to bring out glossy balloon's glossy shine and a median blur to make it "fatter" like a balloon. */


#define TUTORIAL2 \
" id=3 screen aux=[   ref=3 emboss  type=bumpmap azimuth=30  elevation=15 ] median-blur  percentile=90 alpha-percentile=100 gaussian-blur std-dev-x=1 std-dev-y=1 filter=fir id=3 screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] median-blur  percentile=50 alpha-percentile=100 screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] median-blur  percentile=50 alpha-percentile=100 screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ]  "\
/* This GEGL graph is a instruction for emboss to fuss with screen like a normal bevel but done five times over*/


/* Above are two GEGL Graphs being called by Glossy Balloon */

property_double (gaus, _("Balloonification of text"), 6.0)
   description  (_("Makes the text blow up in size using an internal gaussian blur.'"))
  value_range (0.5, 16.0)
  ui_range (0.5, 9)
  ui_gamma (1.5)

property_double (hue, _("Color rotation"),  0.0)
   description  (_("Color rotation. Manually apply a color overlay after applying to make it any color you want.'"))
   value_range  (-180.0, 180.0)

property_double (lightness, _("Lightness"), -7)
   description  (_("Lightness adjustment"))
   value_range  (-15.0, 15)

property_double (saturation, _("Saturation"), 1.2)
   description  (_("Saturation control - for image file overlays full desaturation is recommended."))
  ui_range (0.0, 2)
   value_range  (0.0, 2)

property_file_path(src, _("Image file overlay (Desaturation and bright light recommended)"), "")
    description (_("Upload an image, files permitted are (png, jpg, raw, svg, bmp, tif, ...)"))

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     glossy_bevel
#define GEGL_OP_C_SOURCE glossy-bevel.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *blur, *graph, *graph2, *nop, *hue,  *layer, *smooth, *saturation, *opacityall, *multiply, *repairgeglgraph;


  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

  blur    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
   "filter", 1,
                                  NULL);
/* Filter one is code for Gaussian Blur Fir*/


 graph   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", TUTORIAL,  NULL);
                             

 graph2   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl",  "string", TUTORIAL2, 
                                  NULL);

/*JUNE 24 2023 I FINALLY FIGURED OUT HOW TO HIDE GEGL GRAPH STRINGS. My Plugins have never been more professional*/

 hue   = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

 layer   = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer",
                                  NULL);



 opacityall  = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity", "value", 10.0,
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
/* I don't know why but this NOP removes a trivial warning in GEGL. This is a common theme that many of my plugins have*/

 smooth   = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-reduction", "iterations", 1,
                                  NULL);
/* When two text edges of glossy balloon touch they have rough edges. This solves that problem*/


  repairgeglgraph      = gegl_node_new_child (gegl, "operation", "gegl:median-blur",
                                         "radius",       0,
                                         NULL);

 /*Repair GEGL Graph is a critical operation for Gimp's non-destructive future.
A median blur at zero radius is confirmed to make no changes to an image. 
This option resets gegl:opacity's value to prevent a known bug where
plugins like clay, glossy balloon and custom bevel glitch out when
drop shadow is applied in a gegl graph below them.*/
 
 
  gegl_operation_meta_redirect (operation, "gaus", blur, "std-dev-x");
  gegl_operation_meta_redirect (operation, "gaus", blur, "std-dev-y");
  gegl_operation_meta_redirect (operation, "hue", hue, "hue");
  gegl_operation_meta_redirect (operation, "lightness", hue, "lightness");
  gegl_operation_meta_redirect (operation, "src", layer, "src");
  gegl_operation_meta_redirect (operation, "saturation", saturation, "scale");
  gegl_operation_meta_redirect (operation, "string",  graph, "string");
  gegl_operation_meta_redirect (operation, "string2",  graph2, "string");


  gegl_node_link_many (input, graph, blur, graph2, hue, saturation, opacityall, smooth, multiply, repairgeglgraph, output, NULL);
  gegl_node_connect_from (multiply, "aux", layer, "output");
  gegl_node_link_many (layer, nop,  NULL);

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
    "categories",  "Artistic",
    "reference-hash", "45a35416a10a512570f0f25sb2ac",
    "description", _("Make Glossy Bevel text with GEGL. This will not work well on small text."
                     ""),
    NULL);
}

#endif
