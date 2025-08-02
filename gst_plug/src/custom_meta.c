  #include "custom_meta.h"
  #include <gst/gst.h>
  #include <gst/gstmeta.h> 

  // Регистрация API типа метаданных
  GType
  custom_meta_api_get_type(void)
  {
    static volatile GType type = 0;
    
    // Потокобезопасная инициализация
    if (g_once_init_enter(&type)) {
      GType _type = gst_meta_api_type_register(
        "MyCustomMetaAPI",  // Имя типа
        NULL                 // Флаги (не требуется)
      );
      g_once_init_leave(&type, _type);
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
  const GstMetaInfo*
  custom_meta_get_info(void)
  {
    static GType meta_api_type = 0;  // Кэшируем тип метаданных
    static GstMetaInfo meta_info = {
      0,  // Временное значение, заменится ниже
      sizeof(MyCustomMeta),
      (GstMetaInitFunction)custom_meta_init,
      (GstMetaFreeFunction)custom_meta_free,
      custom_meta_transform
    };

    // Однократная инициализация
    if (G_UNLIKELY(meta_api_type == 0)) {
      meta_api_type = gst_meta_api_type_register("MyCustomMetaAPI", NULL);
      if (meta_api_type == 0) return NULL; // Ошибка регистрации
      ((GstMetaInfo*)&meta_info)->api = meta_api_type;  // Модифицируем поле
    }

    return &meta_info;
  }

  // API для работы с метаданными

  // Добавление метаданных к буферу
  MyCustomMeta*
  buffer_add_custom_meta(GstBuffer *buffer, guint64 frame_id, const gchar *custom_data)
  {
    MyCustomMeta *meta;
    
    g_return_val_if_fail(buffer != NULL, NULL);
    
    // Добавляем метаданные через GStreamer API
    meta = (MyCustomMeta *)gst_buffer_add_meta(
      buffer, 
      MY_CUSTOM_META_INFO,  // Информация о метаданных
      NULL                   // Параметры (не требуется)
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
  MyCustomMeta*
  buffer_get_custom_meta(GstBuffer *buffer)
  {
    g_return_val_if_fail(buffer != NULL, NULL);
    
    return (MyCustomMeta *)gst_buffer_get_meta(
      buffer, 
      MY_CUSTOM_META_API_TYPE  // Тип метаданных
    );
  }