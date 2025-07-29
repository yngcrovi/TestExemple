#pragma once

#include <gst/gst.h>

#ifndef PLUGIN_VERSION
#define PLUGIN_VERSION "1.0.0"  // Ваша версия плагина
#endif

G_BEGIN_DECLS

#define GST_TYPE_CUSTOM_META (gst_custom_meta_get_type())
#define GST_CUSTOM_META_TYPE GST_TYPE_CUSTOM_META  // Алиас для совместимости

typedef struct _MyCustomMeta MyCustomMeta;

struct _MyCustomMeta {
  GstMeta meta;
  guint64 frame_number;
  gint64 timestamp;
};

GType gst_custom_meta_api_get_type(void);
GType gst_custom_meta_get_type(void);
const GstMetaInfo* gst_custom_meta_get_info(void);
MyCustomMeta* my_buffer_add_custom_meta(GstBuffer* buffer, guint64 frame_number, gint64 timestamp);
MyCustomMeta* my_buffer_get_custom_meta(GstBuffer* buffer);

G_END_DECLS