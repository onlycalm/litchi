#!/bin/bash
# print_banner.sh

# 接收日志文件路径参数
JSON_FILE=${1:-"analyze_build_log.json"}
LOG_FILE=${2:-"build.log"}

if [ ! -f "$JSON_FILE" ]; then
    echo "错误：日志文件 $JSON_FILE 不存在"
    exit 1
fi

# 从 JSON 文件中读取 errors 和 warnings 的值，并设置默认值
ERRORS=$(jq -r '.errors // 0' "$JSON_FILE" 2>/dev/null || echo 0)
WARNINGS=$(jq -r '.warnings // 0' "$JSON_FILE" 2>/dev/null || echo 0)

# 确保 ERRORS 和 WARNINGS 是整数
ERRORS=$((ERRORS))
WARNINGS=$((WARNINGS))

# 根据 errors 和 warnings 设置颜色
if [ "$ERRORS" -gt 0 ]; then
    COLOR_CODE="\033[31m" # 红色
elif [ "$WARNINGS" -gt 0 ]; then
    COLOR_CODE="\033[33m" # 黄色
else
    COLOR_CODE="\033[32m" # 绿色
fi

# 定义彩色字符图案
print_banner() {
    # 同时输出到终端和日志文件
    tee -a "$LOG_FILE" << EOF
$(echo -e "$COLOR_CODE")
██████╗ ██████╗ ██████╗ ██████╗  ██████╗ ████████╗
██╔═══██╗██╔══██╗██╔══██╗██╔══██╗██╔═══██╗╚══██╔══╝
██║   ██║██████╔╝██║  ██║██████╔╝██║   ██║   ██║
██║   ██║██╔══██╗██║  ██║██╔══██╗██║   ██║   ██║
╚██████╔╝██║  ██║██████╔╝██████╔╝╚██████╔╝   ██║
 ╚═════╝ ╚═╝  ╚═╝╚═════╝ ╚═════╝  ╚═════╝    ╚═╝
$(echo -e "\033[0m")
EOF
}

# 调用函数
print_banner

exit 0
