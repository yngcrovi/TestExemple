#include <gst/gst.h>
#include <gst/video/video.h>
#include <custom_meta.h>
#include <gst/base/gstbasetransform.h>

G_BEGIN_DECLS

GST_DEBUG_CATEGORY_STATIC(gst_plugin_b_debug);
#define GST_CAT_DEFAULT gst_plugin_b_debug

#define GST_TYPE_PLUGIN_B (gst_plugin_b_get_type())
G_DECLARE_FINAL_TYPE(GstPluginB, gst_plugin_b, GST, PLUGIN_B, GstBaseTransform)


struct _GstPluginB {
  GstBaseTransform parent;
  guint frame_count;
};

G_DEFINE_TYPE(GstPluginB, gst_plugin_b, GST_TYPE_BASE_TRANSFORM)

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
gst_plugin_b_transform_ip(GstBaseTransform *trans, GstBuffer *buf) {
  GstPluginB *self = GST_PLUGIN_B(trans);
  GstCaps *caps = gst_pad_get_current_caps(GST_BASE_TRANSFORM_SINK_PAD(trans));
  
  
  MyGstCustomMeta *custom_meta = (MyGstCustomMeta *)gst_buffer_get_meta(buf, GST_CUSTOM_META_API_TYPE);
  
  if (custom_meta) {
    g_print("Meta found: frame=%d, %dx%d, format=%d\n", 
          custom_meta->frame_number, custom_meta->width, custom_meta->height, custom_meta->format);
  } 

  gst_caps_unref(caps);
  return GST_FLOW_OK;
}

static void
gst_plugin_b_class_init(GstPluginBClass *klass) {
  GstElementClass *element_class = GST_ELEMENT_CLASS(klass);
  GstBaseTransformClass *base_transform_class = GST_BASE_TRANSFORM_CLASS(klass);
  
  gst_element_class_set_static_metadata(element_class,
      "Console video printer",
      "Filter/Video",
      "Prints video frame info to console",
      "Your Name <your.email@example.com>");
  
  gst_element_class_add_static_pad_template(element_class, &sink_template);
  gst_element_class_add_static_pad_template(element_class, &src_template);
  
  base_transform_class->transform_ip = gst_plugin_b_transform_ip;
}

static void
gst_plugin_b_init(GstPluginB *self) {
  self->frame_count = 0;
}

static gboolean plugin_init(GstPlugin *plugin) {

    GST_DEBUG_CATEGORY_INIT(gst_plugin_b_debug, "pluginb", 0, "Custom plugin");

    if (!gst_element_register(plugin, "pluginb", GST_RANK_NONE, GST_TYPE_PLUGIN_B)) {
        GST_ERROR("Failed to register element 'pluginb'");
        return FALSE; // Убедитесь, что возвращаете FALSE при неудаче
    }

    return TRUE;
}

GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    pluginb,
    "Console video printer plugin",
    plugin_init,
    PACKAGE,
    "LGPL",
    "GStreamer",
    "http://gstreamer.net/"
);

G_END_DECLS