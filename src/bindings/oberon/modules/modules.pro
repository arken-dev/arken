TEMPLATE = subdirs

SUBDIRS = base64 \
          digest \
          http   \
          regex  \
          sha3   \
          odebug
unix:SUBDIRS += pgsql
