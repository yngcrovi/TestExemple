#include <custom_meta.h>

static const gchar *meta_tags[] = {"frame", "video", "metadata", NULL};

GType gst_custom_meta_api_get_type(void) {
  static GType type = 0;
  if (g_once_init_enter(&type)) {
    GType _type = gst_meta_api_type_register("MyGstCustomMetaAPI", meta_tags);
    g_once_init_leave(&type, _type);
  }
  return type;
}

static gboolean custom_meta_init(GstMeta *meta, gpointer params, GstBuffer *buffer) {
  MyGstCustomMeta *cmeta = (MyGstCustomMeta *)meta;
  cmeta->frame_number = 0;
  cmeta->width = cmeta->height = 0;
  cmeta->format = GST_VIDEO_FORMAT_UNKNOWN;
  return TRUE;
}

static void custom_meta_free(GstMeta *meta, GstBuffer *buffer) {
}


const GstMetaInfo *gst_custom_meta_get_info(void) {
    static GstMetaInfo info = {0};  // Инициализируем нулями
    static gsize init = 0;

    if (g_once_init_enter(&init)) {
        info.api = gst_custom_meta_api_get_type();  // Теперь можно вызывать функции
        info.size = sizeof(MyGstCustomMeta);
        info.init_func = custom_meta_init;
        info.free_func = custom_meta_free;
        info.transform_func = NULL;
        g_once_init_leave(&init, 1);
    }

    return &info;
}

