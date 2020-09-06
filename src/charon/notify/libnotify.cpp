// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <libnotify/notify.h>
#include <charon/notify.h>

using namespace charon;


void notify::send(string title, string message, string icon)
{
  if ( icon.equals("ok") ) {
    icon = "dialog-information";
  }

  if ( icon.equals("failure") ) {
    icon = "error";
  }

  if ( icon.equals("warning") ) {
    icon = "dialog-warning";
  }

  notify_init("Arken Notify");
  NotifyNotification * Notify = notify_notification_new(title, message, icon);
  notify_notification_show(Notify, NULL);
  g_object_unref(G_OBJECT(Notify));
  notify_uninit();
}
