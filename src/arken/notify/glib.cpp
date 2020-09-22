// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/notify.h>
#include <gio/gio.h>


using namespace arken;

void notify::send(string title, string message, string status)
{
  GApplication *application = g_application_new("arken.notification", G_APPLICATION_FLAGS_NONE);
  g_application_register (application, NULL, NULL);
  GNotification *notification = g_notification_new(title.data());
  g_notification_set_body (notification, message.data());
  GIcon *icon = g_themed_icon_new("dialog-information");
  g_notification_set_icon(notification, icon);
  g_application_send_notification(application, NULL, notification);
  g_object_unref(icon);
  g_object_unref(notification);
  g_object_unref(application);
}
