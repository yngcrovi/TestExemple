#include <gst/gst.h>
#include "custom_meta.h"
#include "config.h"
#include <gst/base/gstbasetransform.h>

#define GST_TYPE_PLUGIN_B (gst_plugin_b_get_type())
G_DECLARE_FINAL_TYPE(GstPluginB, gst_plugin_b, GST, PLUGIN_B, GstBaseTransform)

struct _GstPluginB {
  GstBaseTransform parent;
};

G_DEFINE_TYPE(GstPluginB, gst_plugin_b, GST_TYPE_BASE_TRANSFORM)

static GstFlowReturn gst_plugin_b_transform_ip(GstBaseTransform* trans, GstBuffer* buffer) {
  GstPluginB* self = GST_PLUGIN_B(trans);
  
  // Получаем метаданные из буфера
  MyCustomMeta* meta = my_buffer_get_custom_meta(buffer);
  if (meta) {
    GST_LOG_OBJECT(self, "Frame %lu, PTS %" GST_TIME_FORMAT, 
                  meta->frame_number,
                  GST_TIME_ARGS(meta->timestamp));
  } else {
    GST_WARNING_OBJECT(self, "No custom metadata found in buffer");
  }
  
  return GST_FLOW_OK;
}

static void gst_plugin_b_class_init(GstPluginBClass* klass) {
  GstBaseTransformClass* base_transform_class = GST_BASE_TRANSFORM_CLASS(klass);
  base_transform_class->transform_ip = gst_plugin_b_transform_ip;
}

static void gst_plugin_b_init(GstPluginB* self) {
  // Ничего не нужно инициализировать
}

static gboolean plugin_init(GstPlugin* plugin) {
  return gst_element_register(plugin, "pluginb", GST_RANK_NONE, GST_TYPE_PLUGIN_B);
}

GST_PLUGIN_DEFINE(
  GST_VERSION_MAJOR,
  GST_VERSION_MINOR,
  pluginb,
  "Reads custom metadata from buffers",
  plugin_init,
  PACKAGE_VERSION,  // Используем определённый в config.h макрос
  "LGPL",
  PACKAGE,
  GST_PACKAGE_ORIGIN
)