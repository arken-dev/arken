// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <libnotify/notify.h>
#include <charon/notify.h>

using namespace charon;


void notify::send(string title, string message)
{
  notify_init("Arken Notify");
  NotifyNotification * Notify = notify_notification_new(title, message, "dialog-information");
  notify_notification_show(Notify, NULL);
  g_object_unref(G_OBJECT(Notify));
  notify_uninit();
}
