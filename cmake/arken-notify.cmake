set(ARKEN_NOTIFY "" CACHE STRING
  "notify implementation")

set_property(
  CACHE ARKEN_NOTIFY PROPERTY STRINGS
  "" qt libnotify glib opengl
)

if(NOT ARKEN_NOTIFY STREQUAL "")
  file(GLOB files ${PROJECT_SOURCE_DIR}/src/arken/notify/${ARKEN_NOTIFY}/*.cpp)
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
  PKG_SEARCH_MODULE(GDK_PIXBUF ${_pkgconfig_REQUIRED} "gdk-pixbuf-2.0")
  pkg_check_modules(GLIB REQUIRED glib-2.0)
  pkg_check_modules(LIB_NOTIFY REQUIRED libnotify)
  #pkg_check_modules(GDK_PIXBUF, gio-2.0)
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

if(ARKEN_NOTIFY STREQUAL "")
  message("ARKEN NOTIFY............: OFF")
else()
  message("ARKEN NOTIFY............: ${ARKEN_NOTIFY}")
endif()
