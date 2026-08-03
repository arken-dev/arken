#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <arken/base>

// Structure representing a 128-bit UUID
typedef struct {
    uint8_t bytes[16];
} uuid_v7_t;

// Function to generate a UUIDv7
int generate_uuidv7(char *buffer) {
    uuid_v7_t my_uuid;
    uuid_v7_t *uuid = &my_uuid;

    struct timespec ts;

    // 1. Get current time with nanosecond precision
    if (clock_gettime(CLOCK_REALTIME, &ts) != 0) {
        return -1;
    }

    // Convert seconds and nanoseconds into a 48-bit millisecond timestamp
    uint64_t ts_ms = ((uint64_t)ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);

    // 2. Fill the entire 16-byte array with secure random bytes
    // getentropy() is standard on modern Linux (glibc 2.25+) and macOS
    if (getentropy(uuid->bytes, 16) != 0) {
        return -1;
    }

    // 3. Overwrite the first 6 bytes with the big-endian 48-bit timestamp
    uuid->bytes[0] = (uint8_t)(ts_ms >> 40);
    uuid->bytes[1] = (uint8_t)(ts_ms >> 32);
    uuid->bytes[2] = (uint8_t)(ts_ms >> 24);
    uuid->bytes[3] = (uint8_t)(ts_ms >> 16);
    uuid->bytes[4] = (uint8_t)(ts_ms >> 8);
    uuid->bytes[5] = (uint8_t)(ts_ms);

    // 4. Set the 4-bit Version to 7 (bits 4-7 of byte 6)
    uuid->bytes[6] = (uuid->bytes[6] & 0x0F) | 0x70;

    // 5. Set the 2-bit Variant to 2 (bits 6-7 of byte 8 -> "10xx")
    uuid->bytes[8] = (uuid->bytes[8] & 0x3F) | 0x80;

    snprintf(buffer, 37, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x\n",
           uuid->bytes[0],  uuid->bytes[1],  uuid->bytes[2],  uuid->bytes[3],
           uuid->bytes[4],  uuid->bytes[5],  uuid->bytes[6],  uuid->bytes[7],
           uuid->bytes[8],  uuid->bytes[9],  uuid->bytes[10], uuid->bytes[11],
           uuid->bytes[12], uuid->bytes[13], uuid->bytes[14], uuid->bytes[15]);

    return 0;
}

string os::uuid()
{
  auto my_uuid = new char[37];

  generate_uuidv7(my_uuid);

  return string(std::move(my_uuid), 36);
}
