#include "custom_meta.h"

GType gst_custom_meta_api_get_type(void) {
  static GType type = 0;
  if (g_once_init_enter(&type)) {
    GType _type = gst_meta_api_type_register("MyCustomMetaAPI", NULL);
    g_once_init_leave(&type, _type);
  }
  return type;
}

static gboolean custom_meta_init(GstMeta* meta, gpointer params, GstBuffer* buffer) {
  MyCustomMeta* cmeta = (MyCustomMeta*)meta;
  cmeta->frame_number = 0;
  cmeta->timestamp = 0;
  return TRUE;
}

static gboolean custom_meta_transform(GstBuffer* transbuf, GstMeta* meta,
                                    GstBuffer* buffer, GQuark type, gpointer data) {
  MyCustomMeta* tmeta = (MyCustomMeta*)meta;
  my_buffer_add_custom_meta(transbuf, tmeta->frame_number, tmeta->timestamp);
  return TRUE;
}

static void custom_meta_free(GstMeta* meta, GstBuffer* buffer) {
  // Ничего не нужно освобождать
}

const GstMetaInfo* gst_custom_meta_get_info(void) {
  static const GstMetaInfo meta_info = {
    .api = gst_custom_meta_api_get_type(),
    .type = GST_CUSTOM_META_TYPE,  // Если определён
    .size = sizeof(MyCustomMeta),
    .init_func = custom_meta_init,
    .free_func = custom_meta_free,
    .transform_func = custom_meta_transform
  };
  return &meta_info;
}

MyCustomMeta* gst_buffer_add_custom_meta(GstBuffer* buffer, guint64 frame_number, gint64 timestamp) {
  MyCustomMeta* meta = (MyCustomMeta*)gst_buffer_add_meta(
    buffer, gst_custom_meta_get_info(), NULL);
  if (meta) {
    meta->frame_number = frame_number;
    meta->timestamp = timestamp;
  }
  return meta;
}

MyCustomMeta* gst_buffer_get_custom_meta(GstBuffer* buffer) {
  return (MyCustomMeta*)gst_buffer_get_meta(buffer, gst_custom_meta_api_get_type());
}