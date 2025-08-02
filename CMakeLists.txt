cmake_minimum_required(VERSION 3.10)
project(gst_meta_plugins LANGUAGES C)

# Настройка путей относительно расположения CMakeLists.txt
set(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/gst_plug")

# 1. Поиск зависимостей
find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0>=1.14)

# 2. Настройка путей установки
set(GST_PLUGIN_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib/gstreamer-1.0"
    CACHE PATH "Каталог установки плагинов")

# 3. Настройка include-директорий
include_directories(
    ${GSTREAMER_INCLUDE_DIRS}
    ${PROJECT_ROOT}/include
)

# 4. Сборка плагина A (с полными путями)
add_library(gstplugina MODULE
    ${PROJECT_ROOT}/plugin_a/gstplugina.c
    ${PROJECT_ROOT}/src/custom_meta.c
)
target_link_libraries(gstplugina ${GSTREAMER_LIBRARIES})
set_target_properties(gstplugina PROPERTIES
    PREFIX ""
    OUTPUT_NAME "plugina"
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)

# 5. Сборка плагина B (с полными путями)
add_library(gstpluginb MODULE
    ${PROJECT_ROOT}/plugin_b/gstpluginb.c
    ${PROJECT_ROOT}/src/custom_meta.c
)
target_link_libraries(gstpluginb ${GSTREAMER_LIBRARIES})
set_target_properties(gstpluginb PROPERTIES
    PREFIX ""
    OUTPUT_NAME "pluginb"
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)

# 6. Установка плагинов
install(TARGETS gstplugina gstpluginb
    DESTINATION ${GST_PLUGIN_INSTALL_DIR}
)

# 7. Тестовая команда
add_custom_target(run_test
    COMMAND GST_PLUGIN_PATH=${CMAKE_BINARY_DIR}/plugins gst-launch-1.0 videotestsrc num-buffers=10 ! plugina ! pluginb ! fakesink
    DEPENDS gstplugina gstpluginb
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)