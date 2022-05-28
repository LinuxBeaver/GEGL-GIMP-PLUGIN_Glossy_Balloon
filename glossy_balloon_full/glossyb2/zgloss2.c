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
 * Copyright 2016 Øyvind Kolås <pippin@gimp.org>
*Pixel wheel effect 2022 Beaver
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#define TUTORIAL \
" id=3 screen aux=[   ref=3 emboss  type=bumpmap azimuth=30  elevation=15 ] median-blur  percentile=90 alpha-percentile=190 gaussian-blur std-dev-x=1 std-dev-y=1 id=3 screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] median-blur  percentile=50 alpha-percentile=290 screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] median-blur  percentile=50 alpha-percentile=290 screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] dropshadow x=0.65 y=0.65 opacity=2 color=#ab0091 grow-radius=0 radius=0 gimp:threshold-alpha value=0.99 dropshadow x=0.65 y=0.65 opacity=2 color=#ab0091 grow-radius=0 radius=0 ] \n"\




#ifdef GEGL_PROPERTIES

property_string (string, _("Graph2"), TUTORIAL)




#else

#define GEGL_OP_META
#define GEGL_OP_NAME     zgloss2
#define GEGL_OP_C_SOURCE zgloss2.c

#include "gegl-op.h"
#include <unistd.h>


#include <stdio.h>
#include <stdlib.h>

static void
prepare (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *gegl, *input, *output;
  GError *error = NULL;

  gegl = operation->node;

  if (!o->user_data || !g_str_equal (o->user_data, o->string))
  {
    if (o->user_data)
      g_free (o->user_data);
    o->user_data = g_strdup (o->string);

  input  = gegl_node_get_input_proxy (gegl,  "input");
  output = gegl_node_get_output_proxy (gegl, "output");

  gegl_node_link_many (input, output, NULL);
  {
     gchar cwd[81920]; // XXX: should do better
     getcwd (cwd, sizeof(cwd));
  gegl_create_chain (o->string, input, output, 0.0,
                     gegl_node_get_bounding_box (input).height, cwd,
                     &error);
  }

  if (error)
  {
    gegl_node_set (gegl, "error", error->message, NULL);
    g_clear_error (&error);
  }
  else
  {
    g_object_set (operation, "error", "", NULL);
  }
  }
}

static void
attach (GeglOperation *operation)
{
  GeglNode *gegl, *input, *output;

  gegl    = operation->node;

  input  = gegl_node_get_input_proxy (gegl, "input");
  output = gegl_node_get_output_proxy (gegl, "output");

  gegl_node_link_many (input, output, NULL);
  prepare (operation);
}

static void
dispose (GObject *object)
{
  GeglProperties *o = GEGL_PROPERTIES (object);
  if (o->user_data)
  {
    g_free (o->user_data);
    o->user_data = NULL;
  }
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GObjectClass       *object_class;
  GeglOperationClass *operation_class;

  object_class = G_OBJECT_CLASS  (klass);
  operation_class = GEGL_OPERATION_CLASS (klass);

  object_class->dispose = dispose;
  operation_class->attach = attach;
  operation_class->prepare = prepare;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:zglossy2",
    "title",       _("zglossy2"),
    "categories",  "generic",
    "reference-hash", "29b1522a12gf4egh87ba97b41d8d2e",
    "description", _("another hidden operation meant to power gegl glossy bevel"),
    NULL);
}

#endif
