/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2019, ACED.
 */

/**
 * @file
 * Header file for the EpiReaderException class.
 */

#pragma once

#include <exception>
#include <string>

namespace stride {
namespace visualiser {

/**
 * @class EpiReaderException
 * Exception that is thrown when something does wrong in an EpiReader.
 */
class EpiReaderException : public std::exception
{
public:
		/**
		 * Constructor.
		 *
		 * @param message An std::string that contains a diagnostic message.
		 */
        EpiReaderException(const std::string& message) : m_message{message} {}

        /**
         * Getter that returns a c-string with diagnostic information about the exception.
         */
        const char* what() const noexcept { return m_message.c_str(); }

private:
        /// Stores the diagnostic message.
        const std::string m_message;
};

} // namespace visualiser
} // namespace stride

