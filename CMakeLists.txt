cmake_minimum_required(VERSION 3.10)
project(gst-meta-plugin-example)

# Настройка версии
set(PACKAGE "gst-meta-plugin-example")
set(PACKAGE_VERSION "1.0.0")

# Поиск зависимостей
find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0>=1.14 gstreamer-base-1.0>=1.14)

include_directories(
    ${GSTREAMER_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/gst_plug/include
)

# Определения для компилятора
add_definitions(
    -DPACKAGE="${PACKAGE}"
    -DPACKAGE_VERSION="${PACKAGE_VERSION}"
    -DVERSION="${PACKAGE_VERSION}"
)

# Сборка библиотеки метаданных
add_library(custom_meta SHARED ./gst_plug/src/custom_meta.cpp)
target_link_libraries(custom_meta ${GSTREAMER_LIBRARIES})

# Сборка плагинов
add_library(gstplugina SHARED ./gst_plug/plugin_a/gstplugina.cpp)
target_link_libraries(gstplugina custom_meta ${GSTREAMER_LIBRARIES})

add_library(gstpluginb SHARED ./gst_plug/plugin_b/gstpluginb.cpp)
target_link_libraries(gstpluginb custom_meta ${GSTREAMER_LIBRARIES})

# Установка в директорию GStreamer
install(TARGETS custom_meta gstplugina gstpluginb
        LIBRARY DESTINATION lib/gstreamer-1.0)
