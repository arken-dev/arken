option( ARKEN_CONCURRENT_MAILBOX "enable arken.concurrent.mailbox" false )

if( ARKEN_CONCURRENT_MAILBOX )
  set( arken-concurrent-mailbox ${PROJECT_SOURCE_DIR}/src/arken/concurrent/mailbox.cpp )
endif()

message("ARKEN_CONCURRENT_MAILBOX....: ${ARKEN_CONCURRENT_MAILBOX}")
