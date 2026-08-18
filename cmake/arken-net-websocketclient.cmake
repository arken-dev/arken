option(ARKEN_NET_WEBSOCKETCLIENT "enable arken.net.WebSocketClient" false)

if(ARKEN_NET_WEBSOCKETCLIENT)
  set(arken-net-websocketclient ${PROJECT_SOURCE_DIR}/src/arken/net/websocketclient.cpp)
endif()

message("ARKEN_NET_WEBSOCKETCLIENT...: ${ARKEN_NET_WEBSOCKETCLIENT}")
