# Запуск проекта

## Создание директории для сборки
```console
mkdir build
cd build
```

## Конфигурация проекта
```console
cmake ..
```

## Сборка
```console
make
```

## Установка плагинов
```console
sudo make install
```
Плагины должны установиться по маршруту /usr/local/lib/gstreamer-1.0/

## Настройка переменной окружения для поиска плагинов
```console
export GST_PLUGIN_PATH=/usr/local/lib/gstreamer-1.0/
```

## Запускаем плагины
```console
gst-launch-1.0 videotestsrc num-buffers=10 ! plugina ! pluginb ! fakesink
```