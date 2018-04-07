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
 *  The original copyright, to be found in the directory one level higher
 *  still aplies.
 */
/**
 * @file
 * Implementation file for Benchmarker.
 */

#include "myhayai/Benchmarker.hpp"

#include "myhayai/ConsoleOutputter.hpp"
#include "myhayai/Fixture.hpp"
#include "myhayai/TestFactory.hpp"
#include "myhayai/TestParametersDescriptor.hpp"
#include "myhayai/TestResult.hpp"

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <limits>
#include <random>
#include <string>
#include <vector>

using namespace std;

namespace myhayai {

Benchmarker::~Benchmarker()
{
        // Release all test descriptors.
        size_t index = m_test_descriptors.size();
        while (index--) {
                delete m_test_descriptors[index];
        }
}

void Benchmarker::AddOutputter(Outputter& outputter) { Instance().m_outputters.push_back(&outputter); }

void Benchmarker::ApplyPatternFilter(const char* pattern)
{
        Benchmarker& instance = Instance();

        // Split the filter at '-' if it exists.
        const char* const dash = strchr(pattern, '-');
        string            positive;
        string            negative;
        if (dash == nullptr) {
                positive = pattern;
        } else {
                positive = string(pattern, dash);
                negative = string(dash + 1);
                if (positive.empty())
                        positive = "*";
        }

        // Iterate across all tests and test them against the patterns.
        size_t index = 0;
        while (index < instance.m_test_descriptors.size()) {
                TestDescriptor* desc = instance.m_test_descriptors[index];
                if ((!FilterMatchesString(positive.c_str(), desc->CanonicalName)) ||
                    (FilterMatchesString(negative.c_str(), desc->CanonicalName))) {
                        instance.m_test_descriptors.erase(instance.m_test_descriptors.begin() +
                                                          vector<TestDescriptor*>::difference_type(index));
                        delete desc;
                } else {
                        ++index;
                }
        }
}

bool Benchmarker::FilterMatchesString(const char* filter, const string& str)
{
        const char* patternStart = filter;
        while (true) {
                if (PatternMatchesString(patternStart, str.c_str()))
                        return true;
                // Finds the next pattern in the filter.
                patternStart = strchr(patternStart, ':');
                // Returns if no more pattern can be found.
                if (!patternStart)
                        return false;
                // Skips the pattern separater (the ':' character).
                patternStart++;
        }
}

vector<TestDescriptor*> Benchmarker::GetTests() const
{
        vector<TestDescriptor*> tests;
        size_t                  index = 0;
        while (index < m_test_descriptors.size()) {
                tests.push_back(m_test_descriptors[index++]);
        }
        return tests;
}

Benchmarker& Benchmarker::Instance()
{
        static Benchmarker singleton;
        return singleton;
}

bool Benchmarker::PatternMatchesString(const char* pattern, const char* str)
{
        switch (*pattern) {
        case '\0':
        case ':': return (*str == '\0');
        case '?': // Matches any single character.
                return ((*str != '\0') && (PatternMatchesString(pattern + 1, str + 1)));
        case '*': // Matches any string (possibly empty) of characters.
                return (((*str != '\0') && (PatternMatchesString(pattern, str + 1))) ||
                        (PatternMatchesString(pattern + 1, str)));
        default: return ((*pattern == *str) && (PatternMatchesString(pattern + 1, str + 1)));
        }
}

TestDescriptor* Benchmarker::RegisterTest(const char* fixtureName, const char* testName, size_t runs,
                                          TestFactory testFactory, TestParametersDescriptor parameters)
{
        // Determine if the test has been disabled.
        static const char* disabledPrefix = "DISABLED_";
        bool               isDisabled     = ((::strlen(testName) >= 9) && (!::memcmp(testName, disabledPrefix, 9)));

        if (isDisabled)
                testName += 9;

        // Add the descriptor.
        TestDescriptor* descriptor =
            new TestDescriptor(fixtureName, testName, runs, std::move(testFactory), std::move(parameters), isDisabled);
        Instance().m_test_descriptors.push_back(descriptor);
        return descriptor;
}

void Benchmarker::RunAllTests()
{
        ConsoleOutputter   defaultOutputter;
        vector<Outputter*> defaultOutputters;
        defaultOutputters.push_back(&defaultOutputter);
        Benchmarker&        instance   = Instance();
        vector<Outputter*>& outputters = (instance.m_outputters.empty() ? defaultOutputters : instance.m_outputters);

        // Get the test_descriptors for execution.
        auto         test_descriptors = instance.GetTests();
        const size_t totalCount       = test_descriptors.size();
        size_t       disabledCount    = 0;
        auto         testsIt          = test_descriptors.cbegin();

        while (testsIt != test_descriptors.end()) {
                if ((*testsIt)->IsDisabled)
                        ++disabledCount;
                ++testsIt;
        }
        const size_t enabledCount = totalCount - disabledCount;

        // Begin output.
        for (auto& o : outputters) {
                o->Begin(enabledCount, disabledCount);
        }

        // Run through all the test_descriptors in ascending order.
        size_t index = 0;
        while (index < test_descriptors.size()) {
                // Get the test descriptor.
                TestDescriptor* t_d = test_descriptors[index++];

                // Check if test matches include filters
                if (!instance.m_include_filters.empty()) {
                        bool   included = false;
                        string name     = t_d->FixtureName + "." + t_d->TestName;
                        for (size_t i = 0; i < instance.m_include_filters.size(); i++) {
                                if (name.find(instance.m_include_filters[i]) != string::npos) {
                                        included = true;
                                        break;
                                }
                        }
                        if (!included)
                                continue;
                }

                // Check if test is not disabled.
                if (t_d->IsDisabled) {
                        for (auto& o : outputters)
                                o->SkipDisabledTest(t_d->FixtureName, t_d->TestName, t_d->Parameters, t_d->Runs);
                        continue;
                }

                // Describe the beginning of the run.
                for (auto& o : outputters)
                        o->BeginTest(t_d->FixtureName, t_d->TestName, t_d->Parameters, t_d->Runs);

                // Execute each individual run.
                vector<uint64_t> runTimes(t_d->Runs);
                size_t           run = 0;
                while (run < t_d->Runs) {
                        Fixture  test = t_d->Factory();
                        uint64_t time = test.Run();
                        runTimes[run] = time;
                        ++run;
                }

                // Calculate the test result.
                TestResult testResult(runTimes);

                // Describe the end of the run.
                for (auto& o : outputters) {
                        o->EndTest(t_d->FixtureName, t_d->TestName, t_d->Parameters, testResult);
                }
        }

        // End output.
        for (size_t outputterIndex = 0; outputterIndex < outputters.size(); outputterIndex++) {
                outputters[outputterIndex]->End(enabledCount, disabledCount);
        }
}

void Benchmarker::ShuffleTests()
{
        Benchmarker&  instance = Instance();
        random_device rd;
        mt19937       g(rd());
        shuffle(instance.m_test_descriptors.begin(), instance.m_test_descriptors.end(), g);
}

} // namespace myhayai
