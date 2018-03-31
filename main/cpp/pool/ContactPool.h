#pragma once
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
 *  Copyright 2017, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Header for the core ContactPool class.
 */

#include "contact/ContactLogMode.h"
#include "pool/ContactPoolType.h"

#include <array>
#include <vector>

namespace stride {

class Person;

/**
 * Represents a group of Persons for potential contacts.
 */
class ContactPool
{
public:
        /// Initializing constructor.
        ContactPool(std::size_t pool_id, ContactPoolType::Id type);

        /// Add the given Person.
        void AddMember(const Person* p);

        /// Get member at index.
        Person* GetMember(unsigned int index) const;

        /// Get the entire pool of members.
        const std::vector<std::pair<Person*, bool>>& GetPool() const;

        /// Get size (number of members).
        std::size_t GetSize() const;

private:
        /// Sort members w.r.t. health status.
        /// Sort order: exposed/infected/recovered, susceptible, immune).
        std::tuple<bool, size_t> SortMembers();

        /// Infector calculates contacts and transmissions.
        template <ContactLogMode::Id LL, bool TIC, typename LIP, bool TO>
        friend class Infector;

        /// Calculate which members are present on the current day.
        void UpdateMemberPresence();

private:
        std::size_t                           m_pool_id;      ///< The ID of the ContactPool (for logging purposes).
        ContactPoolType::Id                   m_pool_type;    ///< The type of the ContactPool (for logging purposes).
        std::size_t                           m_index_immune; ///< Index of the first immune member in the ContactPool.
        std::vector<std::pair<Person*, bool>> m_members;      ///< Container with pointers to contactpool members.
};

} // namespace stride