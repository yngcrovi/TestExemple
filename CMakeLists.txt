cmake_minimum_required(VERSION 3.10)
project(gst_plugin_a)

find_package(PkgConfig REQUIRED)
include(GNUInstallDirs)

pkg_check_modules(GST REQUIRED gstreamer-1.0 gstreamer-base-1.0 gstreamer-video-1.0)

# set(CMAKE_INSTALL_PREFIX "/usr/local/")
include_directories(
    ${GST_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/gst_plug/include
)

# Сначала библиотека метаданных
add_library(custom_meta SHARED gst_plug/src/custom_meta.c)

target_link_libraries(custom_meta ${GST_LIBRARIES})

# Затем основной плагин
add_library(plugina MODULE gst_plug/plugin_a/gstplugina.c)  # MODULE вместо SHARED для плагинов GStreamer
add_library(pluginb MODULE gst_plug/plugin_b/gstpluginb.c)


target_link_libraries(plugina PUBLIC
    ${GST_LIBRARIES}
    custom_meta
)

target_link_libraries(pluginb PUBLIC
    ${GST_LIBRARIES}
    custom_meta
)

set_target_properties(plugina PROPERTIES
    PREFIX ""
    OUTPUT_NAME "plugina"
)

set_target_properties(pluginb PROPERTIES
    PREFIX ""
    OUTPUT_NAME "pluginb"
)

install(TARGETS custom_meta
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(TARGETS plugina
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/gstreamer-1.0/
)

install(TARGETS pluginb
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/gstreamer-1.0/
)