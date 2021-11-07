/**
 * https://gist.github.com/kvelakur/9069c9896577c3040030
 * @brief Generate a Version 4 UUID according to RFC-4122
 *
 * Uses the openssl RAND_bytes function to generate a
 * Version 4 UUID.
 *
 * @param buffer A buffer that is at least 38 bytes long.
 * @retval 1 on success, 0 otherwise.
 */
extern "C" {
#include <openssl/rand.h>
}


#include <arken/base>

int uuid_v4_gen(char *buffer)
{
	union
	{
		struct
		{
			uint32_t time_low;
			uint16_t time_mid;
			uint16_t time_hi_and_version;
			uint8_t  clk_seq_hi_res;
			uint8_t  clk_seq_low;
			uint8_t  node[6];
		};
		uint8_t __rnd[16];
	} uuid;

	int rc = RAND_bytes(uuid.__rnd, sizeof(uuid));

	// Refer Section 4.2 of RFC-4122
	// https://tools.ietf.org/html/rfc4122#section-4.2
	uuid.clk_seq_hi_res = (uint8_t) ((uuid.clk_seq_hi_res & 0x3F) | 0x80);
	uuid.time_hi_and_version = (uint16_t) ((uuid.time_hi_and_version & 0x0FFF) | 0x4000);

	snprintf(buffer, 37, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
			uuid.time_low, uuid.time_mid, uuid.time_hi_and_version,
			uuid.clk_seq_hi_res, uuid.clk_seq_low,
			uuid.node[0], uuid.node[1], uuid.node[2],
			uuid.node[3], uuid.node[4], uuid.node[5]);

	return rc;
}


string os::uuid()
{
  char * uuidv4 = new char[37];
  uuid_v4_gen(uuidv4);

  return string(std::move(uuidv4), 36);
}
