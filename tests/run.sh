#!/bin/bash
# Прогоняет все class-тесты и печатает результат + код возврата
TOOL="./build/uml_tool"
PASS=0
FAIL=0
for f in tests/*.json; do
    base=$(basename "$f")
    case "$base" in
        seq-*|_empty-seq.json)
            # это sequence-файл — прогоним с ним как с первым аргументом
            out=$("$TOOL" data/class-diagram.json "$f" 2>&1)
            rc=$?
            ;;
        *)
            out=$("$TOOL" "$f" tests/_empty-seq.json 2>&1)
            rc=$?
            ;;
    esac
    echo "=== $base (rc=$rc) ==="
    echo "$out" | tail -5
    echo
    if [ "$rc" -le 1 ]; then PASS=$((PASS+1)); else FAIL=$((FAIL+1)); fi
done
echo "Пройдено: $PASS, зависло/упало: $FAIL"
