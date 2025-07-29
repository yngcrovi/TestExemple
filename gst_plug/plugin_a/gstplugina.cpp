#include <gst/gst.h>
#include "custom_meta.h"
#include <gst/base/gstbasetransform.h>
#include "config.h"

#define GST_TYPE_PLUGIN_A (gst_plugin_a_get_type())
G_DECLARE_FINAL_TYPE(GstPluginA, gst_plugin_a, GST, PLUGIN_A, GstBaseTransform)

struct _GstPluginA {
  GstBaseTransform parent;
  guint64 frame_counter;
};

G_DEFINE_TYPE(GstPluginA, gst_plugin_a, GST_TYPE_BASE_TRANSFORM)

static GstFlowReturn gst_plugin_a_transform_ip(GstBaseTransform* trans, GstBuffer* buffer) {
  GstPluginA* self = GST_PLUGIN_A(trans);
  
  // Добавляем метаданные к буферу
  MyCustomMeta* meta = my_buffer_add_custom_meta(buffer, self->frame_counter, GST_BUFFER_PTS(buffer));
  if (!meta) {
    GST_WARNING_OBJECT(self, "Failed to add custom meta to buffer");
  }
  
  self->frame_counter++;
  return GST_FLOW_OK;
}

static void gst_plugin_a_class_init(GstPluginAClass* klass) {
  GstBaseTransformClass* base_transform_class = GST_BASE_TRANSFORM_CLASS(klass);
  base_transform_class->transform_ip = gst_plugin_a_transform_ip;
}

static void gst_plugin_a_init(GstPluginA* self) {
  self->frame_counter = 0;
}

static gboolean plugin_init(GstPlugin* plugin) {
  return gst_element_register(plugin, "plugina", GST_RANK_NONE, GST_TYPE_PLUGIN_A);
}

GST_PLUGIN_DEFINE(
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    plugina,  // Имя плагина (должно совпадать с именем библиотеки)
    "Adds custom metadata to buffers",
    plugin_init,
    PLUGIN_VERSION,  // Используем наше определение версии
    "LGPL",
    "gst-meta-plugin-example",
    "https://example.com"
)