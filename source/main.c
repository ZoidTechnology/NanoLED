#include "tusb.h"
#include "led.h"

#define PIO_INSTANCE pio0
#define STATE_MACHINE 0
#define HEADER_SIZE 2

uint8_t packet[HEADER_SIZE + 4096];
uint8_t buffer[256];
uint packetIndex;
uint8_t blockSize;
uint8_t blockIndex;
uint8_t checksum;
uint dataSize;

int main()
{
	tusb_init();
	led_init(PIO_INSTANCE, STATE_MACHINE);

	while (true)
	{
		tud_task();

		if (!tud_connected() || !tud_cdc_available())
		{
			continue;
		}

		uint bufferSize = tud_cdc_read(buffer, sizeof(buffer));

		for (uint bufferIndex = 0; bufferIndex < bufferSize; bufferIndex++)
		{
			uint8_t byte = buffer[bufferIndex];

			if (byte == 0)
			{
				packetIndex = 0;
				blockSize = 0xFF;
				blockIndex = 0xFE;
				checksum = 0;
				continue;
			}

			if (packetIndex >= sizeof(packet))
			{
				continue;
			}

			if (++blockIndex == blockSize)
			{
				bool overhead = blockSize == 0xFF;

				blockSize = byte;
				blockIndex = 0;

				if (overhead)
				{
					continue;
				}

				byte = 0;
			}

			packet[packetIndex] = byte;
			checksum ^= byte;

			if (packetIndex == HEADER_SIZE - 1)
			{
				dataSize = (packet[0] & 0x0F) << 8 | packet[1];
			}

			if (packetIndex == HEADER_SIZE + dataSize)
			{
				led_write(PIO_INSTANCE, STATE_MACHINE, packet[0] >> 4, packet + HEADER_SIZE, dataSize + 1);

				tud_cdc_write_char(checksum);
				tud_cdc_write_flush();
			}

			packetIndex++;
		}
	}
}