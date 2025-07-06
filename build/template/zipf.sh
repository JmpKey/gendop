#!/bin/bash

# Проверка наличия аргумента
if [ -z "$1" ]; then
  echo "Использование: $0 <директория>"
  exit 1
fi

# Проверка существования директории
if [ ! -d "$1" ]; then
  echo "Ошибка: Директория '$1' не найдена."
  exit 1
fi

# Получение имени директории
dir_name=$(basename "$1")

# Переход в директорию
cd "$1"

# Создание zip-архива, запаковывая содержимое текущей директории
zip -r "../$dir_name.zip" *

# Возврат в исходную директорию
cd ..

# Изменение расширения на docx
mv "$dir_name.zip" "$dir_name.docx"

echo "Директория '$1' запакована в файл '$dir_name.docx'."

# Попытка открыть файл docx (зависит от системы)
# xdg-open "$dir_name.docx" 2>/dev/null || open "$dir_name.docx" 2>/dev/null || start "$dir_name.docx" 2>/dev/null

exit 0


