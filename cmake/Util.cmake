# PrintLog
# parameter:
#   MsgTyp - message type [STATUS|WARNING|AUTHOR_WARNING|SEND_ERROR|FATAL_ERROR|DEPRECATION]
#   Msg    - message
function(PrintLog MsgTyp Msg)
    message(${MsgTyp} "${Msg}")
    file(APPEND "${BUILD_LOG}" "${Msg}\n")
endfunction()
