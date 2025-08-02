#include "custom_meta.h"
#include <gst/gst.h>
#include <gst/gstmeta.h> 

// Регистрация API типа метаданных
GType
gst_custom_meta_api_get_type(void)
{
    static GType type = 0;

    g_mutex_lock(&registration_mutex);
    if (!meta_type_registered) {
        const gchar *tags[] = {GST_META_TAG_MEMORY_STR, NULL};
        GType new_type = gst_meta_api_type_register("MyCustomMetadata", tags);
        if (new_type == 0) {
            GST_ERROR("Failed to register meta API type");
            g_once_init_leave(&type, G_TYPE_INVALID);
            return G_TYPE_INVALID;
        }
        g_once_init_leave(&type, new_type);
    }
    return type;
} 

// Функции обработки метаданных
// Инициализация метаданных
static gboolean 
custom_meta_init(GstMeta *meta, gpointer params, GstBuffer *buffer)
{
    MyCustomMeta *cmeta = (MyCustomMeta *)meta;
    cmeta->frame_id = 0;
    cmeta->custom_data = NULL;
    return TRUE;
}

// Освобождение памяти метаданных
static void
custom_meta_free(GstMeta *meta, GstBuffer *buffer)
{
    MyCustomMeta *cmeta = (MyCustomMeta *)meta;
    if (cmeta->custom_data) {
        g_free(cmeta->custom_data);
        cmeta->custom_data = NULL;
    }
}

// Преобразование метаданных при копировании буфера
static gboolean
custom_meta_transform(GstBuffer *dest, GstMeta *src_meta,
    GstBuffer *src, GQuark type, gpointer data)
{
    MyCustomMeta *src_custom = (MyCustomMeta *)src_meta;
    
    // Простое копирование данных
    buffer_add_custom_meta(
        dest, 
        src_custom->frame_id,
        src_custom->custom_data
    );
    
    return TRUE;
}

// Регистрация информации о метаданных
CUSTOM_META_API_EXPORT const GstMetaInfo*
gst_custom_meta_get_info(void)
{
    static GType meta_api_type = 0;
    static GstMetaInfo meta_info = { 0 };  

    if (G_UNLIKELY(meta_api_type == 0)) {
        meta_api_type = MY_CUSTOM_META_API_TYPE;
        if (meta_api_type == 0) return NULL;

        // Заполняем структуру
        meta_info.api = meta_api_type;
        meta_info.size = sizeof(MyCustomMeta);
        meta_info.init_func = custom_meta_init;
        meta_info.free_func = custom_meta_free;
        meta_info.transform_func = custom_meta_transform;
    }

      if (meta_info.api == 0) {
        GST_ERROR("Meta API type not registered");
        return NULL;
    }

    return &meta_info;
}

// API для работы с метаданными
// Добавление метаданных к буферу
CUSTOM_META_API_EXPORT MyCustomMeta*
buffer_add_custom_meta(GstBuffer *buffer, guint64 frame_id, const gchar *custom_data)
{
  MyCustomMeta *meta;
  
  g_return_val_if_fail(buffer != NULL, NULL);
  
  const GstMetaInfo *info = gst_custom_meta_get_info();
  if (!info) {
      GST_ERROR("Failed to get meta info");
      return NULL;
  }

  // Добавляем метаданные через GStreamer API
  meta = (MyCustomMeta *)gst_buffer_add_meta(
    buffer, 
    MY_CUSTOM_META_INFO,  // Информация о метаданных
    NULL                  
  );
  
  if (!meta) {
    return NULL;
  }
  
  // Заполняем поля
  meta->frame_id = frame_id;
  
  if (custom_data) {
    meta->custom_data = g_strdup(custom_data);  // Копируем строку
  } else {
    meta->custom_data = NULL;
  }
  
  return meta;
}

// Получение метаданных из буфера
CUSTOM_META_API_EXPORT MyCustomMeta*
buffer_get_custom_meta(GstBuffer *buffer)
{
  g_return_val_if_fail(buffer != NULL, NULL);
  
  return (MyCustomMeta *)gst_buffer_get_meta(
    buffer, 
    MY_CUSTOM_META_API_TYPE  // Тип метаданных
  );
}