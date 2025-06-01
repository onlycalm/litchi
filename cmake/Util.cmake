# 通用日志函数（同时输出到终端和文件）
function(PrintLog message)
    message(STATUS "${message}")
    file(APPEND "${BUILD_LOG}" "${message}\n")
endfunction()
