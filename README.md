# Создание директории для сборки
mkdir build
cd build

# Конфигурация проекта
cmake ..

# Сборка
make

# Установка плагинов
sudo make install

# Плагины должны установиться по маршруту /usr/local/lib/gstreamer-1.0/

# Настройка переменной окружения для поиска плагинов
export GST_PLUGIN_PATH=/usr/local/lib/gstreamer-1.0/

# Запускаем плагины
gst-launch-1.0 videotestsrc num-buffers=10 ! plugina ! pluginb ! fakesink