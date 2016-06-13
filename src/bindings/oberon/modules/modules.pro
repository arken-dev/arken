TEMPLATE = subdirs

SUBDIRS = http \
          md5  \
          odebug
unix:SUBDIRS += pgsql
