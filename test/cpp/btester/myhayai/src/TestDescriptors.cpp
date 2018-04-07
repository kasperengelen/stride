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
 *  Copyright 2018, Kuylen E, Willem L, Broeckhove J
 *
 *  This software has been altered form the hayai software by Nick Bruun.
 *  The original copyright, to be found in the directory two levels higher
 *  still aplies.
 */
/**
 * @file
 * Implementation file for TestDescriptors container.
 */

#include "myhayai/TestDescriptors.hpp"

#include <algorithm>

namespace myhayai {

size_t TestDescriptors::CountDisabled()
{
        return static_cast<size_t>(count_if(this->begin(), this->end(), [](const auto& t_d){return t_d.IsDisabled;}));
}

} // namespace myhayai
