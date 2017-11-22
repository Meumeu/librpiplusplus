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

/**
 * @todo write docs
 */
class GPIO
{
	int pin;
public:
	enum class direction
	{
		input,
		output
	};

	enum class pull
	{
		up,
		down,
		off
	};

	GPIO(int pin, direction d);
	GPIO(int pin, direction d, pull pud);

	bool input();
	void output(bool value);
};
