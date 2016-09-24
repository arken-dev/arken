TEMPLATE = subdirs

SUBDIRS = http   \
          digest \
          sha3   \
          odebug
unix:SUBDIRS += pgsql
