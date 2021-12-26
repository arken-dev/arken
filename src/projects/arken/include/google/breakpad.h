#include "client/linux/handler/exception_handler.h"
#include <arken/base>
#include <arken/log>
#include <iostream>

static bool dumpCallback(const google_breakpad::MinidumpDescriptor& descriptor,
void* context, bool succeeded) {
  arken::Log log("dumps");
  log.info(descriptor.path());
  log.dump();
  printf("Dump path: %s\n", descriptor.path());
  return succeeded;
}

#define BREAKPAD() \
  arken::string path = os::pwd(); \
  path.append("/logs/dumps"); \
  google_breakpad::MinidumpDescriptor descriptor(path.data()); \
  google_breakpad::ExceptionHandler eh(descriptor, NULL, dumpCallback, NULL, true, -1); \
  std::cout << "google breakpad enabled..." << std::endl; \
  std::cout << "path descriptor " << path << std::endl;
