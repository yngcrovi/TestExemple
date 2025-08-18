#include <gst/gst.h>
#include <gst/video/video.h>
#include <custom_meta.h>
#include <gst/base/gstbasetransform.h>

G_BEGIN_DECLS

GST_DEBUG_CATEGORY_STATIC(gst_plugin_a_debug);
#define GST_CAT_DEFAULT gst_plugin_a_debug

#define GST_TYPE_PLUGIN_A (gst_plugin_a_get_type())
G_DECLARE_FINAL_TYPE(GstPluginA, gst_plugin_a, GST, PLUGIN_A, GstBaseTransform)


struct _GstPluginA {
  GstBaseTransform parent;
  guint frame_count;
};

G_DEFINE_TYPE(GstPluginA, gst_plugin_a, GST_TYPE_BASE_TRANSFORM)

static GstStaticPadTemplate sink_template = 
    GST_STATIC_PAD_TEMPLATE("sink",
        GST_PAD_SINK,
        GST_PAD_ALWAYS,
        GST_STATIC_CAPS(GST_VIDEO_CAPS_MAKE("{ I420, NV12, RGBA }")));

static GstStaticPadTemplate src_template = 
    GST_STATIC_PAD_TEMPLATE("src",
        GST_PAD_SRC,
        GST_PAD_ALWAYS,
        GST_STATIC_CAPS(GST_VIDEO_CAPS_MAKE("{ I420, NV12, RGBA }")));

static GstFlowReturn
gst_plugin_a_transform_ip(GstBaseTransform *trans, GstBuffer *buf) {
  GstPluginA *self = GST_PLUGIN_A(trans);
  GstVideoFrame frame;
  GstVideoInfo info;
  GstCaps *caps = gst_pad_get_current_caps(GST_BASE_TRANSFORM_SINK_PAD(trans));
  
  if (!gst_video_info_from_caps(&info, caps)) {
    GST_ERROR_OBJECT(self, "Failed to parse video caps");
    gst_caps_unref(caps);
    return GST_FLOW_ERROR;
  }
  
    self->frame_count++;
  
  // Добавляем метаданные в буфер
  GstMeta *meta = gst_buffer_add_meta(buf, GST_CUSTOM_META_INFO, NULL);
  if (meta) {
    MyGstCustomMeta *custom_meta = (MyGstCustomMeta *)meta;
    custom_meta->frame_number = self->frame_count;
    custom_meta->width = info.width;
    custom_meta->height = info.height;
    custom_meta->format = info.finfo->format;
  }

  gst_caps_unref(caps);
  return GST_FLOW_OK;
}

static void
gst_plugin_a_class_init(GstPluginAClass *klass) {
  GstElementClass *element_class = GST_ELEMENT_CLASS(klass);
  GstBaseTransformClass *base_transform_class = GST_BASE_TRANSFORM_CLASS(klass);
  
  gst_element_class_set_static_metadata(element_class,
      "Console video printer",
      "Filter/Video",
      "Prints video frame info to console",
      "Your Name <your.email@example.com>");
  
  gst_element_class_add_static_pad_template(element_class, &sink_template);
  gst_element_class_add_static_pad_template(element_class, &src_template);
  
  base_transform_class->transform_ip = gst_plugin_a_transform_ip;
}

static void
gst_plugin_a_init(GstPluginA *self) {
  self->frame_count = 0;
}

static gboolean plugin_init(GstPlugin *plugin) {

    GST_DEBUG_CATEGORY_INIT(gst_plugin_a_debug, "plugina", 0, "Custom plugin");

    if (!gst_element_register(plugin, "plugina", GST_RANK_NONE, GST_TYPE_PLUGIN_A)) {
        GST_ERROR("Failed to register element 'myplugin'");
        return FALSE; // Убедитесь, что возвращаете FALSE при неудаче
    }

    GST_INFO("Plugin 'plugina' initialized successfully");
    return TRUE;
}

GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    plugina,
    "Console video printer plugin",
    plugin_init,
    PACKAGE,
    "LGPL",
    "GStreamer",
    "http://gstreamer.net/"
);

G_END_DECLS