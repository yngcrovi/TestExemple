#include <gst/gst.h>
#include "custom_meta.h"

/* 1. Предварительное объявление типа */
#define GST_TYPE_PLUGIN_B (gst_plugin_b_get_type())
G_DECLARE_FINAL_TYPE(GstPluginB, gst_plugin_b, GST, PLUGIN_B, GstElement)

/* 2. Структура элемента */
struct _GstPluginB {
    GstElement parent;
    
    /* Pads */
    GstPad *sinkpad;
    GstPad *srcpad;
};

/* 3. Регистрация типа */
G_DEFINE_TYPE(GstPluginB, gst_plugin_b, GST_TYPE_ELEMENT)

/* ========== Реализация методов ========== */

static void gst_plugin_b_class_init(GstPluginBClass *klass) {
    GstElementClass *element_class = GST_ELEMENT_CLASS(klass);
    
    /* Настройка pad templates */
    gst_element_class_add_pad_template(
        element_class,
        gst_pad_template_new(
            "sink",
            GST_PAD_SINK,
            GST_PAD_ALWAYS,
            GST_CAPS_ANY
        )
    );
    
    gst_element_class_add_pad_template(
        element_class,
        gst_pad_template_new(
            "src",
            GST_PAD_SRC,
            GST_PAD_ALWAYS,
            GST_CAPS_ANY
        )
    );
    
    /* Метаданные плагина */
    gst_element_class_set_static_metadata(
        element_class,
        "Plugin B - Извлекает метаданные",
        "Filter/Metadata",
        "Читает пользовательские метаданные из буферов",
        "Ваше Имя <your.email@example.com>"
    );
}

static GstFlowReturn gst_plugin_b_chain(GstPad *pad, GstObject *parent, GstBuffer *buf) {
    GstPluginB *plugin = GST_PLUGIN_B(parent);
    
    /* Извлечение метаданных */
    MyCustomMeta *meta = buffer_get_custom_meta(buf);
    
    if (meta) {
        /* Логирование метаданных */
        GST_LOG("Получены метаданные: frame_id=%" G_GUINT64_FORMAT ", data=%s",
               meta->frame_id,
               meta->custom_data ? meta->custom_data : "NULL");
    } else {
        GST_WARNING("Метаданные не найдены в буфере!");
    }
    
    /* Передача буфера дальше по pipeline */
    return gst_pad_push(plugin->srcpad, buf);
}

static void gst_plugin_b_init(GstPluginB *plugin) {
    /* Создание pad'ов */
    plugin->sinkpad = gst_pad_new_from_template(
        gst_element_class_get_pad_template(GST_ELEMENT_GET_CLASS(plugin), "sink"),
        "sink"
    );
    
    plugin->srcpad = gst_pad_new_from_template(
        gst_element_class_get_pad_template(GST_ELEMENT_GET_CLASS(plugin), "src"),
        "src"
    );
    
    /* Установка chain функции */
    gst_pad_set_chain_function(plugin->sinkpad, gst_plugin_b_chain);
    
    /* Добавление pad'ов к элементу */
    gst_element_add_pad(GST_ELEMENT(plugin), plugin->sinkpad);
    gst_element_add_pad(GST_ELEMENT(plugin), plugin->srcpad);
}

/* Регистрация плагина */
static gboolean plugin_init(GstPlugin *plugin) {
    return gst_element_register(
        plugin,
        "pluginb",
        GST_RANK_NONE,
        GST_TYPE_PLUGIN_B
    );
}

GST_PLUGIN_DEFINE(
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    pluginb,
    "Пользовательский плагин B",
    plugin_init,
    VERSION,
    "LGPL",
    "gst-meta-plugin-example",
    "https://example.com"
)