/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2017  Guillaume Meunier <guillaume.meunier@centraliens.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GPIO.h"
#include <cstdint>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdexcept>

// From https://sourceforge.net/p/raspberry-gpio-python/code/ci/default/tree/source/c_gpio.c

namespace
{
	const int FSEL_OFFSET = 0;          // 0x0000
	const int SET_OFFSET = 7;           // 0x001c / 4
	const int CLR_OFFSET = 10;          // 0x0028 / 4
	const int PINLEVEL_OFFSET = 13;     // 0x0034 / 4
// 	const int EVENT_DETECT_OFFSET = 16; // 0x0040 / 4
// 	const int RISING_ED_OFFSET = 19;    // 0x004c / 4
// 	const int FALLING_ED_OFFSET = 22;   // 0x0058 / 4
// 	const int HIGH_DETECT_OFFSET = 25;  // 0x0064 / 4
// 	const int LOW_DETECT_OFFSET = 28;   // 0x0070 / 4
	const int PULLUPDN_OFFSET = 37;     // 0x0094 / 4
	const int PULLUPDNCLK_OFFSET = 38;  // 0x0098 / 4

// 	const int PAGE_SIZE = 4 * 1024;
	const int BLOCK_SIZE = 4 * 1024;

// 	const int PUD_OFF = 0;
	const int PUD_DOWN = 1;
	const int PUD_UP = 2;

	volatile std::uint32_t * open_gpio_map()
	{
		int mem_fd = open("/dev/gpiomem", O_RDWR | O_SYNC);

		if (mem_fd < 0)
			return nullptr;

		return static_cast<volatile std::uint32_t *>(mmap(nullptr, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, 0));
	}

	volatile std::uint32_t * gpio_map = open_gpio_map();
	void short_wait()
	{
		for(int i = 0; i < 150; i++)
		{
			// wait 150 cycles
			asm volatile("nop");
		}
	}
}

GPIO::GPIO(int pin, direction dir): pin(pin)
{
	if (!gpio_map)
		throw std::runtime_error("Cannot open GPIO");

	int offset = FSEL_OFFSET + pin / 10;
	int shift = (pin % 10) * 3;

	switch(dir)
	{
		case direction::input:
			gpio_map[offset] = gpio_map[offset] & ~(7<<shift);
			break;
		case direction::output:
			gpio_map[offset] = (gpio_map[offset] & ~(7<<shift)) | (1<<shift);
			break;
	}

}

GPIO::GPIO(int gpio, direction dir, pull pud): GPIO(gpio, dir)
{
	int clk_offset = PULLUPDNCLK_OFFSET + gpio / 32;
	int shift = gpio % 32;

	switch(pud)
	{
		case pull::down:
			gpio_map[PULLUPDN_OFFSET] = (gpio_map[PULLUPDN_OFFSET] & ~3) | PUD_DOWN;
			break;
		case pull::up:
			gpio_map[PULLUPDN_OFFSET] = (gpio_map[PULLUPDN_OFFSET] & ~3) | PUD_UP;
			break;
		case pull::off:
			gpio_map[PULLUPDN_OFFSET] &= ~3;
			break;
	}

	short_wait();
	gpio_map[clk_offset] = 1 << shift;
	short_wait();
	gpio_map[PULLUPDN_OFFSET] &= ~3;
	gpio_map[clk_offset] = 0;
}

bool GPIO::input()
{
	int offset = PINLEVEL_OFFSET + pin / 32;
	int mask = 1 << pin % 32;
	int value = gpio_map[offset] & mask;
	return value != 0;
}

void GPIO::output(bool value)
{
	int offset = (value ? SET_OFFSET : CLR_OFFSET) + pin / 32;
	int shift = pin % 32;
	gpio_map[offset] = 1 << shift;
}
