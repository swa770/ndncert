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

#ifndef NDNCERT_NDNCERT_COMMON_HPP
#define NDNCERT_NDNCERT_COMMON_HPP

#include "ndncert-config.hpp"

#ifdef HAVE_TESTS
#define VIRTUAL_WITH_TESTS virtual
#define PUBLIC_WITH_TESTS_ELSE_PROTECTED public
#define PUBLIC_WITH_TESTS_ELSE_PRIVATE public
#define PROTECTED_WITH_TESTS_ELSE_PRIVATE protected
#else
#define VIRTUAL_WITH_TESTS
#define PUBLIC_WITH_TESTS_ELSE_PROTECTED protected
#define PUBLIC_WITH_TESTS_ELSE_PRIVATE private
#define PROTECTED_WITH_TESTS_ELSE_PRIVATE private
#endif

#include <cstddef>
#include <cstdint>

#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/data.hpp>
#include <ndn-cxx/name.hpp>
#include <ndn-cxx/face.hpp>
#include <ndn-cxx/link.hpp>
#include <ndn-cxx/encoding/block.hpp>
#include <ndn-cxx/lp/nack.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include <ndn-cxx/security/v2/certificate.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/assert.hpp>
#include <boost/noncopyable.hpp>
#include <boost/throw_exception.hpp>

namespace ndn {
namespace ndncert {

using std::size_t;
using boost::noncopyable;
using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;
using std::make_shared;
using ndn::make_unique;
using std::enable_shared_from_this;
using std::function;
using std::bind;
using ndn::Interest;
using ndn::Data;
using ndn::Name;
using ndn::PartialName;
using ndn::Block;
using ndn::time::system_clock;
using ndn::time::toUnixTimestamp;

enum : uint32_t {
  tlv_ca_prefix = 129,
  tlv_ca_info = 131,
  tlv_parameter_key = 133,
  tlv_parameter_value = 135,
  tlv_ca_certificate = 137,
  tlv_max_validity_period = 139,
  tlv_probe_response = 141,
  tlv_allow_longer_name = 143,
  tlv_ecdh_pub = 145,
  tlv_cert_request = 147,
  tlv_salt = 149,
  tlv_request_id = 151,
  tlv_challenge = 153,
  tlv_status = 155,
  tlv_initialization_vector = 157,
  tlv_encrypted_payload = 159,
  tlv_selected_challenge = 161,
  tlv_challenge_status = 163,
  tlv_remaining_tries = 165,
  tlv_remaining_time = 167,
  tlv_issued_cert_name = 169,
  tlv_error_code = 171,
  tlv_error_info = 173
};

// Parse CA Configuration file
const std::string CONFIG_CA_PREFIX = "ca-prefix";
const std::string CONFIG_CA_INFO = "ca-info";
const std::string CONFIG_MAX_VALIDITY_PERIOD = "max-validity-period";
const std::string CONFIG_PROBE_PARAMETERS = "probe-parameters";
const std::string CONFIG_PROBE_PARAMETER = "probe-parameter-key";
const std::string CONFIG_SUPPORTED_CHALLENGES = "supported-challenges";
const std::string CONFIG_CHALLENGE = "challenge";

// JSON format for Certificate Issuer (CA)
const std::string JSON_CA_NAME = "name";
const std::string JSON_CA_CONFIG = "ca-config";
const std::string JSON_CA_ECDH = "ecdh-pub";
const std::string JSON_CA_SALT = "salt";
const std::string JSON_CA_REQUEST_ID = "request-id";
const std::string JSON_CA_STATUS = "status";
const std::string JSON_CA_CHALLENGES = "challenges";
const std::string JSON_CA_CHALLENGE_ID = "challenge-id";
const std::string JSON_CA_CERT_ID = "certificate-id";

// JSON format for Challenge Module
const std::string JSON_CHALLENGE_STATUS = "challenge-status";
const std::string JSON_CHALLENGE_REMAINING_TRIES = "remaining-tries";
const std::string JSON_CHALLENGE_REMAINING_TIME = "remaining-time";
const std::string JSON_CHALLENGE_ISSUED_CERT_NAME = "issued-cert-name";

// JSON format for Certificate Requester
const std::string JSON_CLIENT_PROBE_INFO = "probe-info";
const std::string JSON_CLIENT_ECDH = "ecdh-pub";
const std::string JSON_CLIENT_CERT_REQ = "cert-request";
const std::string JSON_CLIENT_SELECTED_CHALLENGE = "selected-challenge";

// NDNCERT Status Enum
enum {
  STATUS_BEFORE_CHALLENGE = 0,
  STATUS_CHALLENGE = 1,
  STATUS_PENDING = 2,
  STATUS_SUCCESS = 3,
  STATUS_FAILURE = 4,
  STATUS_NOT_STARTED = 5
};

// Pre-defined challenge status
const std::string CHALLENGE_STATUS_SUCCESS = "success";
const std::string CHALLENGE_STATUS_FAILURE_TIMEOUT = "failure-timeout";
const std::string CHALLENGE_STATUS_FAILURE_MAXRETRY = "failure-max-retry";
const std::string CHALLENGE_STATUS_UNKNOWN_CHALLENGE = "unknown-challenge";

} // namespace ndncert
} // namespace ndn

#endif // NDNCERT_NDNCERT_COMMON_HPP
