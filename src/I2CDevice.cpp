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

#include "I2CDevice.h"

#include <linux/i2c.h>
#include <linux/i2c-dev.h>


#include <unistd.h> // close
#include <fcntl.h> // open
#include <stropts.h> // ioctl
#include <exception>
#include <stdexcept>
#include <boost/lexical_cast.hpp>

I2CDevice::I2CDevice(const std::string& path, std::uint8_t address)
{
	fd = open(path.c_str(), O_RDWR);
	if (fd < 0)
		throw std::runtime_error("Cannot open " + path);

	if (ioctl(fd, I2C_SLAVE, address) < 0)
	{
		close(fd);
		throw std::runtime_error("Cannot open address " + boost::lexical_cast<std::string>(address));
	}
}

I2CDevice::I2CDevice::I2CDevice(I2CDevice && other) : fd(other.fd)
{
	other.fd = -1;
}

I2CDevice::~I2CDevice()
{
	if (fd >= 0)
		close(fd);
}

std::uint8_t I2CDevice::read8(int reg)
{
	i2c_smbus_data data;
	i2c_smbus_ioctl_data args;

	args.read_write = I2C_SMBUS_READ;
	args.command = reg;
	args.size = I2C_SMBUS_BYTE_DATA;
	args.data = &data;

	if (ioctl(fd, I2C_SMBUS, &args))
		throw I2CException();

	return data.byte;
}

std::uint16_t I2CDevice::read16(int reg)
{
	i2c_smbus_data data;
	i2c_smbus_ioctl_data args;

	args.read_write = I2C_SMBUS_READ;
	args.command = reg;
	args.size = I2C_SMBUS_WORD_DATA;
	args.data = &data;

	if (ioctl(fd, I2C_SMBUS, &args))
		throw I2CException();

	return data.word;
}

void I2CDevice::write8(int reg, std::uint8_t value)
{
	i2c_smbus_data data;
	i2c_smbus_ioctl_data args;

	args.read_write = I2C_SMBUS_WRITE;
	args.command = reg;
	args.size = I2C_SMBUS_BYTE_DATA;
	args.data = &data;
	data.byte = value;

	if (ioctl(fd, I2C_SMBUS, &args))
		throw I2CException();
}

void I2CDevice::write16(int reg, std::uint16_t value)
{
	i2c_smbus_data data;
	i2c_smbus_ioctl_data args;

	args.read_write = I2C_SMBUS_WRITE;
	args.command = reg;
	args.size = I2C_SMBUS_WORD_DATA;
	args.data = &data;
	data.word = value;

	if (ioctl(fd, I2C_SMBUS, &args))
		throw I2CException();
}

I2CException::I2CException(): std::runtime_error("I2C exception")
{
}
