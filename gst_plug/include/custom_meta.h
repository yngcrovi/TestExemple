#include <gst/gst.h>
#include <gst/video/video.h>

#define PACKAGE "1.0"

typedef struct _MyGstCustomMeta MyGstCustomMeta;

struct _MyGstCustomMeta {
  GstMeta meta;          // Базовая структура метаданных
  guint frame_number;    // Номер кадра
  gint width, height;    // Разрешение
  GstVideoFormat format; // Формат пикселей
};

GType gst_custom_meta_api_get_type(void);
#define GST_CUSTOM_META_API_TYPE (gst_custom_meta_api_get_type())

const GstMetaInfo *gst_custom_meta_get_info(void);
#define GST_CUSTOM_META_INFO (gst_custom_meta_get_info())
