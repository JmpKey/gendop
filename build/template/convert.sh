#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Использование: $0 имя_файла"
    exit 1
fi

input_file="$1"

if [ ! -f "$input_file" ]; then
    echo "Файл не найден: $input_file"
    exit 1
fi

# Задаем путь для выходных файлов
base_name="${input_file%.*}"  # Имя файла без расширения
output_odt="$base_name.odt"
output_pdf="$base_name.pdf"

# Конвертируем в ODT
libreoffice --headless --convert-to odt "$input_file" --outdir "$(dirname "$input_file")"

# Проверяем, успешна ли конвертация ODT
if [ $? -ne 0 ]; then
    echo "Ошибка при конвертации в ODT"
    exit 1
fi

# Конвертируем в PDF, используя только полученный ODT
libreoffice --headless --convert-to pdf "$output_odt" --outdir "$(dirname "$input_file")"

# Проверяем, успешна ли конвертация DOCX
if [ $? -ne 0 ]; then
    echo "Ошибка при конвертации в DOCX"
    exit 1
fi

echo "Конвертация завершена для файла: $input_file"

