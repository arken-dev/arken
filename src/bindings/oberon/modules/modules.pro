TEMPLATE = subdirs

SUBDIRS = base64 \
          digest \
          http   \
          sha3   \
          odebug
unix:SUBDIRS += pgsql
