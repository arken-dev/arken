set(ARKEN_NOTIFY "" CACHE STRING "notify implementation")

set_property(
  CACHE ARKEN_NOTIFY PROPERTY STRINGS "" qt libnotify glib opengl
)

if(NOT ARKEN_NOTIFY STREQUAL "")
  file(GLOB files ${PROJECT_SOURCE_DIR}/src/arken/notify/${ARKEN_NOTIFY}/*.cpp)

  if(ARKEN_NOTIFY STREQUAL "opengl")
    # windows.cpp / darwin.cpp / default.cpp implementam a mesma interface
    # (platform.h) para SOs diferentes: só o arquivo do SO atual deve entrar
    # no build (os outros dois dependem de headers/APIs que nem existem
    # fora da sua plataforma, e colidiriam por definir os mesmos símbolos).
    set(opengl-platform-files
      ${PROJECT_SOURCE_DIR}/src/arken/notify/opengl/windows.cpp
      ${PROJECT_SOURCE_DIR}/src/arken/notify/opengl/darwin.cpp
      ${PROJECT_SOURCE_DIR}/src/arken/notify/opengl/default.cpp
    )
    list(REMOVE_ITEM files ${opengl-platform-files})

    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
      list(APPEND files ${PROJECT_SOURCE_DIR}/src/arken/notify/opengl/windows.cpp)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
      enable_language(OBJCXX)
      list(APPEND files ${PROJECT_SOURCE_DIR}/src/arken/notify/opengl/darwin.cpp)
      set_source_files_properties(
        ${PROJECT_SOURCE_DIR}/src/arken/notify/opengl/darwin.cpp
        PROPERTIES LANGUAGE OBJCXX
      )
    else()
      list(APPEND files ${PROJECT_SOURCE_DIR}/src/arken/notify/opengl/default.cpp)
    endif()
  endif()

  set( arken-notify ${files} )
endif()

if(ARKEN_NOTIFY STREQUAL "qt")
  set(CMAKE_AUTOMOC ON)
  find_package(Qt5Core)
  find_package(Qt5Network)
  find_package(Qt5Gui)
  find_package(Qt5Widgets)
  include_directories(${Qt5Widgets_INCLUDE_DIRS})
  include_directories(${PROJECT_SOURCE_DIR}/src/arken/notify/qt)
endif()

if(ARKEN_NOTIFY STREQUAL "libnotify")
  find_package(PkgConfig REQUIRED)
  find_package(GTK2 REQUIRED gtk)
  pkg_search_module(GDK_PIXBUF ${_pkgconfig_REQUIRED} "gdk-pixbuf-2.0")
  pkg_check_modules(GLIB REQUIRED glib-2.0)
  pkg_check_modules(LIB_NOTIFY REQUIRED libnotify)
  include_directories(${GLIB_INCLUDE_DIRS})
  include_directories(${NOTIFY_INCLUDE_DIRS})
  find_path(GDK_PIXBUF_INCLUDE_DIRS gdk-pixbuf/gdk-pixbuf.h)
  include_directories(${GDK_PIXBUF_INCLUDE_DIRS})
endif()

if(ARKEN_NOTIFY STREQUAL "glib")
  find_package(PkgConfig REQUIRED)
  #find_package(GTK2 REQUIRED gtk)
  #PKG_SEARCH_MODULE(GDK_PIXBUF ${_pkgconfig_REQUIRED} "gdk-pixbuf-2.0")
  #pkg_check_modules(GLIB REQUIRED glib-2.0)
  pkg_check_modules(LIB_GIO REQUIRED gio-2.0)
  #PKG_CHECK_MODULES(GDK_PIXBUF, gio-2.0)
  include_directories(${GLIB_INCLUDE_DIRS})
  #find_path(GDK_PIXBUF_INCLUDE_DIRS gdk-pixbuf/gdk-pixbuf.h)
  #include_directories(${GDK_PIXBUF_INCLUDE_DIRS})
endif()

if(ARKEN_NOTIFY STREQUAL "opengl")
  find_package(PkgConfig REQUIRED)
  find_package(OpenGL REQUIRED)
  pkg_check_modules(GLFW REQUIRED glfw3)
  pkg_check_modules(FREETYPE REQUIRED freetype2)
  include_directories(${GLFW_INCLUDE_DIRS})
  include_directories(${FREETYPE_INCLUDE_DIRS})
  include_directories(${OPENGL_INCLUDE_DIR})

  if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows" AND NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    pkg_check_modules(FONTCONFIG REQUIRED fontconfig)
    pkg_check_modules(X11 REQUIRED x11)
    include_directories(${FONTCONFIG_INCLUDE_DIRS})
    include_directories(${X11_INCLUDE_DIRS})
  endif()
endif()

if(ARKEN_NOTIFY STREQUAL "")
  message("ARKEN_NOTIFY................: OFF")
else()
  message("ARKEN_NOTIFY................: ${ARKEN_NOTIFY}")
endif()
