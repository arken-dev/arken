TEMPLATE = subdirs

SUBDIRS = http \
          odebug
unix:SUBDIRS += pgsql
