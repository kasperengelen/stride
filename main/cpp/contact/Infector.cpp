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
 * Implementation of Infector algorithms.
 */

#include "Infector.h"

#include "pool/ContactPool.h"

using namespace std;

namespace {

using namespace stride;

inline double GetContactRate(const AgeContactProfile& profile, const Person* p, size_t pool_size)
{
        const double reference_num_contacts{profile[EffectiveAge(static_cast<unsigned int>(p->GetAge()))]};
        const double potential_num_contacts{static_cast<double>(pool_size - 1)};

        double individual_contact_rate = reference_num_contacts / potential_num_contacts;
        if (individual_contact_rate >= 1) {
                individual_contact_rate = 0.999;
        }
        // Contacts are reciprocal, so one needs to apply only half of the contacts here.
        individual_contact_rate = individual_contact_rate / 2;
        // Contacts are bi-directional: contact probability for 1=>2 and 2=>1 = indiv_cnt_rate*indiv_cnt_rate
        individual_contact_rate += (individual_contact_rate * individual_contact_rate);

        return individual_contact_rate;
}

} // namespace

namespace stride {

/// Primary R0_POLICY: do nothing i.e. track all cases.
/// \tparam TIC         TrackIndexCase
template <bool TIC>
class R0_POLICY
{
public:
        static void Exec(Person*) {}
};

/// Specialized R0_POLICY: track only the index case.
template <>
class R0_POLICY<true>
{
public:
        static void Exec(Person* p) { p->GetHealth().StopInfection(); }
};

/// Primary LOG_POLICY policy, implements LogMode::None.
/// \tparam LL
template <ContactLogMode::Id LL>
class LOG_POLICY
{
public:
        static void Contact(const shared_ptr<spdlog::logger>&, Person*, Person*, ContactPoolType::Id,
                            const shared_ptr<const Calendar>&)
        {
        }

        static void Transmission(const shared_ptr<spdlog::logger>&, Person*, Person*, ContactPoolType::Id,
                                 const shared_ptr<const Calendar>&)
        {
        }
};

/// Specialized LOG_POLICY policy LogMode::Transmissions.
template <>
class LOG_POLICY<ContactLogMode::Id::Transmissions>
{
public:
        static void Contact(const shared_ptr<spdlog::logger>&, Person*, Person*, ContactPoolType::Id,
                            const shared_ptr<const Calendar>&)
        {
        }

        static void Transmission(const shared_ptr<spdlog::logger>& contact_logger, Person* p1, Person* p2,
                                 ContactPoolType::Id type, const shared_ptr<const Calendar>& calendar)
        {
                contact_logger->info("[TRAN] {} {} {} {}", p1->GetId(), p2->GetId(), ContactPoolType::ToString(type),
                                     calendar->GetSimulationDay());
        }
};

/// Specialized LOG_POLICY policy LogMode::Contacts.
template <>
class LOG_POLICY<ContactLogMode::Id::All>
{
public:
        static void Contact(const shared_ptr<spdlog::logger>& contact_logger, Person* p1, Person* p2,
                            ContactPoolType::Id type, const shared_ptr<const Calendar>& calendar)
        {
                const auto home                = (type == ContactPoolType::Id::Household);
                const auto work                = (type == ContactPoolType::Id::Work);
                const auto school              = (type == ContactPoolType::Id::School);
                const auto primary_community   = (type == ContactPoolType::Id::PrimaryCommunity);
                const auto secundary_community = (type == ContactPoolType::Id::SecondaryCommunity);

                contact_logger->info("[CONT] {} {} {} {} {} {} {} {} {}", p1->GetId(), p1->GetAge(), p2->GetAge(),
                                     static_cast<unsigned int>(home), static_cast<unsigned int>(school),
                                     static_cast<unsigned int>(work), static_cast<unsigned int>(primary_community),
                                     static_cast<unsigned int>(secundary_community), calendar->GetSimulationDay());
        }

        static void Transmission(const shared_ptr<spdlog::logger>& logger, Person* p1, Person* p2,
                                 ContactPoolType::Id type, const shared_ptr<const Calendar>& calendar)
        {
                logger->info("[TRAN] {} {} {} {}", p1->GetId(), p2->GetId(), ContactPoolType::ToString(type),
                             calendar->GetSimulationDay());
        }
};

/// Specialized LOG_POLICY policy LogMode::SusceptibleContacts.
template <>
class LOG_POLICY<ContactLogMode::Id::Susceptibles>
{
public:
        static void Contact(const shared_ptr<spdlog::logger>& contact_logger, Person* p1, Person* p2,
                            ContactPoolType::Id, const shared_ptr<const Calendar>&)
        {
                if (p1->GetHealth().IsSusceptible() && p2->GetHealth().IsSusceptible()) {
                        contact_logger->info("[CONT] {} {}", p1->GetId(), p2->GetId());
                }
        }

        static void Transmission(const shared_ptr<spdlog::logger>&, Person*, Person*, ContactPoolType::Id,
                                 const shared_ptr<const Calendar>&)
        {
        }
};

//-------------------------------------------------------------------------------------------------
// Definition for primary template covers the situation for ContactLogMode::None & ContactLogMode::Transmissions,
// both with track_index_case false and true.
// And every local information policy except NoLocalInformation
//-------------------------------------------------------------------------------------------------
template <ContactLogMode::Id LL, bool TIC, typename LIP, bool TO>
void Infector<LL, TIC, LIP, TO>::Exec(ContactPool& pool, AgeContactProfile& profile,
                                      TransmissionProfile disease_profile, ContactHandler contact_handler,
                                      shared_ptr<const Calendar> calendar, shared_ptr<spdlog::logger> contact_logger)
{
        using LP = LOG_POLICY<LL>;
        using RP = R0_POLICY<TIC>;

        pool.UpdateMemberPresence();

        // set up some stuff
        const auto  c_type    = pool.m_pool_type;
        const auto& c_members = pool.m_members;
        const auto  t_rate    = disease_profile.GetRate();

        // check all contacts
        for (size_t i_person1 = 0; i_person1 < c_members.size(); i_person1++) {
                // check if member is present today
                if (c_members[i_person1].second) {
                        auto         p1     = c_members[i_person1].first;
                        const double c_rate = GetContactRate(profile, p1, pool.m_members.size());
                        // loop over possible contacts (contacts can be initiated by each member)
                        for (size_t i_person2 = 0; i_person2 < c_members.size(); i_person2++) {
                                // check if not the same person
                                if (i_person1 != i_person2) {
                                        // check if member is present today
                                        if (c_members[i_person2].second) {
                                                auto p2 = c_members[i_person2].first;
                                                // check for contact
                                                if (contact_handler.HasContact(c_rate)) {
                                                        // log contact if person 1 is participating in survey
                                                        if (p1->IsParticipatingInSurvey()) {
                                                                LP::Contact(contact_logger, p1, p2, c_type, calendar);
                                                        }
                                                        // log contact if person 2 is participating in survey
                                                        if (p2->IsParticipatingInSurvey()) {
                                                                LP::Contact(contact_logger, p2, p1, c_type, calendar);
                                                        }

                                                        // exchange info about health state & beliefs
                                                        LIP::Update(p1, p2);

                                                        // transmission & infection.
                                                        if (contact_handler.HasTransmission(t_rate)) {
                                                                if (p1->GetHealth().IsInfectious() &&
                                                                    p2->GetHealth().IsSusceptible()) {
                                                                        LP::Transmission(contact_logger, p1, p2, c_type,
                                                                                         calendar);
                                                                        p2->GetHealth().StartInfection();
                                                                        RP::Exec(p2);
                                                                } else if (p2->GetHealth().IsInfectious() &&
                                                                           p1->GetHealth().IsSusceptible()) {
                                                                        LP::Transmission(contact_logger, p2, p1, c_type,
                                                                                         calendar);
                                                                        p1->GetHealth().StartInfection();
                                                                        RP::Exec(p1);
                                                                }
                                                        }
                                                }
                                        }
                                }
                        }
                }
        }
}

//-------------------------------------------------------------------------------------------
// Time optimized implementation for NoLocalInformationPolicy and None || Transmission logging.
//-------------------------------------------------------------------------------------------
template <ContactLogMode::Id LL, bool TIC>
void Infector<LL, TIC, NoLocalInformation, true>::Exec(ContactPool& pool, AgeContactProfile& profile,
                                                       TransmissionProfile disease_profile, ContactHandler ch,
                                                       shared_ptr<const Calendar> calendar,
                                                       shared_ptr<spdlog::logger> contact_logger)
{
        using LP = LOG_POLICY<LL>;
        using RP = R0_POLICY<TIC>;

        // check for infected members and sort
        bool   infectious_cases;
        size_t num_cases;
        tie(infectious_cases, num_cases) = pool.SortMembers();

        if (infectious_cases) {
                pool.UpdateMemberPresence();

                // set up some stuff
                const auto  c_type    = pool.m_pool_type;
                const auto  c_immune  = pool.m_index_immune;
                const auto& c_members = pool.m_members;
                const auto  t_rate    = disease_profile.GetRate();

                // match infectious and susceptible members, skip last part (immune members)
                for (size_t i_infected = 0; i_infected < num_cases; i_infected++) {
                        // check if member is present today
                        if (c_members[i_infected].second) {
                                const auto p1 = c_members[i_infected].first;
                                if (p1->GetHealth().IsInfectious()) {
                                        const double c_rate_p1 = GetContactRate(profile, p1, pool.m_members.size());
                                        // loop over possible susceptible contacts
                                        for (size_t i_contact = num_cases; i_contact < c_immune; i_contact++) {
                                                // check if member is present today
                                                if (c_members[i_contact].second) {
                                                        auto         p2 = c_members[i_contact].first;
                                                        const double c_rate_p2 =
                                                            GetContactRate(profile, p2, pool.m_members.size());
                                                        if (ch.HasContactAndTransmission(c_rate_p1, t_rate) ||
                                                            ch.HasContactAndTransmission(c_rate_p2, t_rate)) {
                                                                if (p1->GetHealth().IsInfectious() &&
                                                                    p2->GetHealth().IsSusceptible()) {
                                                                        p2->GetHealth().StartInfection();
                                                                        RP::Exec(p2);
                                                                        LP::Transmission(contact_logger, p1, p2, c_type,
                                                                                         calendar);
                                                                }
                                                        }
                                                }
                                        }
                                }
                        }
                }
        }
}

//--------------------------------------------------------------------------
// All explicit instantiations.
//--------------------------------------------------------------------------
template class Infector<ContactLogMode::Id::None, false, NoLocalInformation>;
template class Infector<ContactLogMode::Id::None, false, LocalDiscussion>;
template class Infector<ContactLogMode::Id::None, true, NoLocalInformation>;
template class Infector<ContactLogMode::Id::None, true, LocalDiscussion>;

template class Infector<ContactLogMode::Id::Transmissions, false, NoLocalInformation>;
template class Infector<ContactLogMode::Id::Transmissions, false, LocalDiscussion>;
template class Infector<ContactLogMode::Id::Transmissions, true, NoLocalInformation>;
template class Infector<ContactLogMode::Id::Transmissions, true, LocalDiscussion>;

template class Infector<ContactLogMode::Id::All, false, NoLocalInformation>;
template class Infector<ContactLogMode::Id::All, false, LocalDiscussion>;
template class Infector<ContactLogMode::Id::All, true, NoLocalInformation>;
template class Infector<ContactLogMode::Id::All, true, LocalDiscussion>;

template class Infector<ContactLogMode::Id::Susceptibles, false, NoLocalInformation>;
template class Infector<ContactLogMode::Id::Susceptibles, false, LocalDiscussion>;
template class Infector<ContactLogMode::Id::Susceptibles, true, NoLocalInformation>;
template class Infector<ContactLogMode::Id::Susceptibles, true, LocalDiscussion>;

} // namespace stride