cat > README.md <<'EOF'
# UML Sequence Tool

Консольное приложение для генерации UML-диаграмм (class + sequence)
из JSON-описаний. Собрано на C++17, стандартная библиотека + POSIX.
Внешних зависимостей нет, права root не требуются.

## Сборка

    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build -j

## Запуск

Из корня проекта (важно: пути `data/`, `plugins/`, `output/` относительные):

    ./build/uml_tool
    ./build/uml_tool data/class-diagram.json data/sequence-diagram.json

## Структура

- `src/` — исходники (JSON-парсер, парсеры диаграмм, анализ циклов, экспорт в Markdown, загрузчик плагинов)
- `plugins/` — исходники .so-плагинов (html, json) и собранные `libexport_*.so`
- `data/` — примеры входных JSON
- `output/` — сгенерированные отчёты (.md) и вывод плагинов
- `logs/` — журнал работы (`app.log`)
- `tests/` — тестовые JSON и `run.sh` для прогона крайних случаев

## Что делает

1. Читает `data/class-diagram.json` → печатает `Class Diagram:`
2. Читает `data/sequence-diagram.json` → печатает `Sequence Diagram:`
3. Анализирует граф связей (DFS) → выдаёт `ВНИМАНИЕ: обнаружен цикл: A -> B -> C -> A`
4. Пишет `output/class-diagram.md` и `output/sequence-diagram.md`
5. Загружает `plugins/*.so` через `dlopen`/`dlsym` и вызывает `create_plugin()`

## Плагины

Плагин — это `.so` в `plugins/`, экспортирующий C-функцию:

    extern "C" DiagramPlugin* create_plugin();

Интерфейс `DiagramPlugin` объявлен в `src/plugin_loader.h`.
Плагин получает `const std::vector<ClassInfo>*` / `const std::vector<SequenceStep>*`
и путь для сохранения результата.

Сборка плагина — отдельная цель CMake (`export_html`, `export_json`),
результат автоматически кладётся в `plugins/libexport_*.so`.

## Обработка ошибок

- Битый JSON → сообщение об ошибке в stderr, код возврата 1
- Отсутствует поле (`name`, `actor`, `action`, `target`) → ошибка с указанием индекса
- Пустой список классов/шагов → нормальный вывод, `Циклов не обнаружено`
- Цикл в связях → предупреждение в stdout (программа не падает)

## Тестирование

    ./tests/run.sh

Прогоняет сценарии из `tests/*.json`: пустые списки, отсутствие полей,
self-cycle, ABC-цикл, битый JSON.

## Ограничения

- ALT Linux, g++ 10+, CMake 3.16+
- Кроссплатформенность не требуется
- Только стандартная библиотека C++ и POSIX API
EOF
echo "README.md создан:"; wc -l README.md# UML-Sequence-Tool
