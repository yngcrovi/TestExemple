#pragma once

#include <gst/gst.h>

#ifndef VERSION
#define VERSION "1.0.0"
#endif

#ifndef PACKAGE
#define PACKAGE "gst-meta-plugin-example"
#endif

G_BEGIN_DECLS

/* Определение структуры наших метаданных */
typedef struct _MyCustomMeta MyCustomMeta;

struct _MyCustomMeta {
  GstMeta meta;       // Базовая структура GstMeta (обязательно первый элемент)
  guint64 frame_id;   // Пример пользовательского поля - ID кадра
  gchar* custom_data; // Пример дополнительных пользовательских данных
};

/* API для работы с метаданными */

// Функция регистрации типа метаданных в GStreamer
GType custom_meta_api_get_type(void);
#define MY_CUSTOM_META_API_TYPE (custom_meta_api_get_type())

// Получение информации о метаданных (используется при регистрации)
const GstMetaInfo* custom_meta_get_info(void);
#define MY_CUSTOM_META_INFO (custom_meta_get_info())

// Добавление метаданных к буферу
MyCustomMeta* buffer_add_custom_meta(GstBuffer* buffer, guint64 frame_id, const gchar* custom_data);

// Получение метаданных из буфера
MyCustomMeta* buffer_get_custom_meta(GstBuffer* buffer);

static gboolean custom_meta_init(GstMeta *meta, gpointer params, GstBuffer *buffer);
static void custom_meta_free(GstMeta *meta, GstBuffer *buffer);

static gboolean
custom_meta_transform(GstBuffer *dest, GstMeta *src_meta,
    GstBuffer *src, GQuark type, gpointer data);

G_END_DECLS