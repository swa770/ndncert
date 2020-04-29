/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2017-2020, Regents of the University of California.
 *
 * This file is part of ndncert, a certificate management system based on NDN.
 *
 * ndncert is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * ndncert is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received copies of the GNU General Public License along with
 * ndncert, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndncert authors and contributors.
 */

#include "ca-config.hpp"
#include "challenge-module.hpp"

#include <boost/filesystem.hpp>
#include <ndn-cxx/util/io.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/info_parser.hpp>

namespace ndn {
namespace ndncert {

void
CaConfig::load(const std::string& fileName)
{
  JsonSection configJson;
  try {
    boost::property_tree::read_json(fileName, configJson);
  }
  catch (const std::exception& error) {
    BOOST_THROW_EXCEPTION(Error("Failed to parse configuration file " + fileName + ", " + error.what()));
  }
  if (configJson.begin() == configJson.end()) {
    BOOST_THROW_EXCEPTION(Error("Error processing configuration file: " + fileName + " no data"));
  }
  parse(configJson);
}

void
CaConfig::parse(const JsonSection& configJson)
{
  // CA prefix
  m_caPrefix = Name(configJson.get(CONFIG_CA_PREFIX, ""));
  if (m_caPrefix.empty()) {
    BOOST_THROW_EXCEPTION(Error("Cannot parse ca-prefix from the config file"));
  }
  // CA info
  m_caInfo = configJson.get(CONFIG_CA_INFO, "");
  // CA max validity period
  m_maxValidityPeriod = time::seconds(configJson.get(CONFIG_MAX_VALIDITY_PERIOD, 3600));
  // probe
  m_probeParameterKeys.clear();
  auto probeParameters = configJson.get_child_optional(CONFIG_PROBE_PARAMETERS);
  if (probeParameters) {
    parseProbeParameters(*probeParameters);
  }
  // optional supported challenges
  m_supportedChallenges.clear();
  auto challengeList = configJson.get_child_optional(CONFIG_SUPPORTED_CHALLENGES);
  if (!challengeList) {
    BOOST_THROW_EXCEPTION(Error("Cannot parse challenge list"));
  }
  parseChallengeList(*challengeList);
}

void
CaConfig::parseProbeParameters(const JsonSection& section)
{
  auto it = section.begin();
  for (; it != section.end(); it++) {
    auto probeParameter = it->second.get(CONFIG_PROBE_PARAMETER, "");
    if (probeParameter == "") {
      BOOST_THROW_EXCEPTION(Error("Cannot read probe-parameter-key in probe-parameters from the config file"));
    }
    probeParameter = boost::algorithm::to_lower_copy(probeParameter);
    m_probeParameterKeys.push_back(probeParameter);
  }
}

void
CaConfig::parseChallengeList(const JsonSection& section)
{
  auto it = section.begin();
  for (; it != section.end(); it++) {
    auto challengeType = it->second.get(CONFIG_CHALLENGE, "");
    if (challengeType == "") {
      BOOST_THROW_EXCEPTION(Error("Cannot read type in supported-challenges from the config file"));
    }
    challengeType = boost::algorithm::to_lower_copy(challengeType);
    if (!ChallengeModule::supportChallenge(challengeType)) {
      BOOST_THROW_EXCEPTION(Error("Does not support challenge read from the config file"));
    }
    m_supportedChallenges.push_back(challengeType);
  }
  if (m_supportedChallenges.size() == 0) {
    BOOST_THROW_EXCEPTION(Error("At least one challenge should be identified under supported-challenges"));
  }
}

}  // namespace ndncert
}  // namespace ndn
