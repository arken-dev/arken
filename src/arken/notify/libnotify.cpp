// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <libnotify/notify.h>
#include <arken/notify.h>

namespace arken {

void notify::send(const char * title, const char * message, const char * icon)
{
  if ( string::equals(icon, "ok") ) {
    icon = "dialog-information";
  }

  if ( string::equals(icon, "failure") ) {
    icon = "error";
  }

  if ( string::equals(icon, "warning") ) {
    icon = "dialog-warning";
  }

  notify_init("Arken Notify");
  NotifyNotification * Notify = notify_notification_new(title, message, icon);
  notify_notification_show(Notify, NULL);
  g_object_unref(G_OBJECT(Notify));
  notify_uninit();
}

} // namespace arken
