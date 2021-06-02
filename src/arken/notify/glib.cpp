// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/notify.h>
#include <gio/gio.h>


namespace arken {

void notify::send(const char * title, const char * message, const char * icon)
{
  GApplication *application = g_application_new("arken.notification", G_APPLICATION_FLAGS_NONE);
  g_application_register (application, nullptr, nullptr);
  GNotification *notification = g_notification_new(title);
  g_notification_set_body (notification, message);
  GIcon *gicon = g_themed_icon_new("dialog-information");
  g_notification_set_icon(notification, gicon);
  g_application_send_notification(application, nullptr, notification);
  g_object_unref(gicon);
  g_object_unref(notification);
  g_object_unref(application);
}

} // namespace arken
