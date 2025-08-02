#pragma once

#include <gst/gst.h>

#ifndef VERSION
#define VERSION "1.0.0"
#endif

#ifndef PACKAGE
#define PACKAGE "gst-meta-plugin-example"
#endif

#ifndef CUSTOM_META_API_EXPORT
#define CUSTOM_META_API_EXPORT __attribute__((visibility("default")))
#endif

G_BEGIN_DECLS

/* Определение структуры метаданных */
typedef struct _MyCustomMeta MyCustomMeta;

struct _MyCustomMeta {
  GstMeta meta;       // Базовая структура GstMeta
  guint64 frame_id;   // ID кадра
  gchar* custom_data; // Дополнительные данных
};

/* API для работы с метаданными */

// Функция регистрации типа метаданных
CUSTOM_META_API_EXPORT GType gst_custom_meta_api_get_type(void);
#define MY_CUSTOM_META_API_TYPE (gst_custom_meta_api_get_type())

// Получение информации о метаданных (используется при регистрации)
CUSTOM_META_API_EXPORT const GstMetaInfo* gst_custom_meta_get_info(void);
#define MY_CUSTOM_META_INFO (gst_custom_meta_get_info())

// Добавление метаданных к буферу
CUSTOM_META_API_EXPORT MyCustomMeta* buffer_add_custom_meta(GstBuffer* buffer, guint64 frame_id, const gchar* custom_data);

// Получение метаданных из буфера
CUSTOM_META_API_EXPORT MyCustomMeta* buffer_get_custom_meta(GstBuffer* buffer);

static gboolean custom_meta_init(GstMeta *meta, gpointer params, GstBuffer *buffer);
static void custom_meta_free(GstMeta *meta, GstBuffer *buffer);

static gboolean
custom_meta_transform(GstBuffer *dest, GstMeta *src_meta,
    GstBuffer *src, GQuark type, gpointer data);


static volatile gboolean meta_type_registered = FALSE;
static GMutex registration_mutex;

G_END_DECLS