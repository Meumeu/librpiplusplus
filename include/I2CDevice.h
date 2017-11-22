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

#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>

class I2CDevice
{
	int fd = -1;

public:
	I2CDevice(const std::string & path, std::uint8_t address);
	~I2CDevice();

	I2CDevice(const I2CDevice &) = delete;
	I2CDevice(I2CDevice &&);
	I2CDevice & operator=(const I2CDevice &) = delete;

	std::uint8_t read8(int reg);
	std::uint16_t read16(int reg);
	void write8(int reg, std::uint8_t value);
	void write16(int reg, std::uint16_t value);
};

class I2CException: public std::runtime_error
{
public:
	I2CException();
};
