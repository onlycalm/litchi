#!/bin/bash
# analyze_build_log.sh

LOG_FILE=$1
TARGET_NAME=$2
JSON_FILE=$3

if [ ! -f "$LOG_FILE" ]; then
    echo "错误：日志文件 $LOG_FILE 不存在"
    exit 1
fi

# 多模式匹配（兼容GCC/Clang/MSVC/中文环境）
ERROR_PATTERNS=('error:' 'Error:' 'fatal error:' '错误:' 'error C')
WARNING_PATTERNS=('warning:' 'Warning:' '警告:' 'warning C')

count_matches() {
    local pattern="$1"
    local count=0
    # 使用临时文件避免命令替换问题
    tmpfile=$(mktemp)
    grep -E -c "$pattern" "$LOG_FILE" 2>/dev/null > "$tmpfile" || echo 0 > "$tmpfile"
    count=$(< "$tmpfile")
    rm -f "$tmpfile"
    echo $count
}

ERRORS=0
for pattern in "${ERROR_PATTERNS[@]}"; do
    ERRORS=$((ERRORS + $(count_matches "$pattern")))
done

WARNINGS=0
for pattern in "${WARNING_PATTERNS[@]}"; do
    WARNINGS=$((WARNINGS + $(count_matches "$pattern")))
done

echo "=== $TARGET_NAME build report ===" | tee -a "$LOG_FILE"
echo "errors: $ERRORS" | tee -a "$LOG_FILE"
echo "warnings: $WARNINGS" | tee -a "$LOG_FILE"

echo "{\"errors\": $ERRORS, \"warnings\": $WARNINGS}" > "$JSON_FILE"

exit 0
