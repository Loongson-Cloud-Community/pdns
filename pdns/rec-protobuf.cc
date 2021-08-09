
#include "config.h"
#include "rec-protobuf.hh"

void RecProtoBufMessage::addRR(const DNSRecord& record)
{
#ifdef HAVE_PROTOBUF
  PBDNSMessage_DNSResponse* response = d_message.mutable_response();
  if (!response) {
    return;
  }

  if (record.d_place != DNSResourceRecord::ANSWER ||
      record.d_class != QClass::IN ||
      (record.d_type != QType::A &&
       record.d_type != QType::AAAA &&
       record.d_type != QType::CNAME)) {
    return;
  }

   PBDNSMessage_DNSResponse_DNSRR* pbRR = response->add_rrs();
   if (!pbRR) {
     return;
   }

   pbRR->set_name(record.d_name.toString());
   pbRR->set_type(record.d_type);
   pbRR->set_class_(record.d_class);
   pbRR->set_ttl(record.d_ttl);
   if (record.d_type == QType::A) {
     const ARecordContent& arc = dynamic_cast<const ARecordContent&>(*(record.d_content));
     ComboAddress data = arc.getCA();
     pbRR->set_rdata(&data.sin4.sin_addr.s_addr, sizeof(data.sin4.sin_addr.s_addr));
   }
   else if (record.d_type == QType::AAAA) {
     const AAAARecordContent& arc = dynamic_cast<const AAAARecordContent&>(*(record.d_content));
     ComboAddress data = arc.getCA();
     pbRR->set_rdata(&data.sin6.sin6_addr.s6_addr, sizeof(data.sin6.sin6_addr.s6_addr));
   } else if (record.d_type == QType::CNAME) {
     const CNAMERecordContent& crc = dynamic_cast<const CNAMERecordContent&>(*(record.d_content));
     DNSName data = crc.getTarget();
     pbRR->set_rdata(data.toString());
   }
#endif /* HAVE_PROTOBUF */
}

void RecProtoBufMessage::addRRs(const std::vector<DNSRecord>& records)
{
  for (const auto& record : records) {
    addRR(record);
  }
}

void RecProtoBufMessage::setAppliedPolicy(const std::string& policy)
{
#ifdef HAVE_PROTOBUF
  PBDNSMessage_DNSResponse* response = d_message.mutable_response();
  if (response && !policy.empty()) {
    response->set_appliedpolicy(policy);
  }
#endif /* HAVE_PROTOBUF */
}

void RecProtoBufMessage::setAppliedPolicyType(const DNSFilterEngine::PolicyType& type)
{
#ifdef HAVE_PROTOBUF
  PBDNSMessage_DNSResponse* response = d_message.mutable_response();
  if (response) {
    switch(type) {
    case DNSFilterEngine::PolicyType::None:
      response->set_appliedpolicytype(PBDNSMessage_PolicyType_UNKNOWN);
      break;
    case DNSFilterEngine::PolicyType::QName:
      response->set_appliedpolicytype(PBDNSMessage_PolicyType_QNAME);
      break;
    case DNSFilterEngine::PolicyType::ClientIP:
      response->set_appliedpolicytype(PBDNSMessage_PolicyType_CLIENTIP);
      break;
    case DNSFilterEngine::PolicyType::ResponseIP:
      response->set_appliedpolicytype(PBDNSMessage_PolicyType_RESPONSEIP);
      break;
    case DNSFilterEngine::PolicyType::NSDName:
      response->set_appliedpolicytype(PBDNSMessage_PolicyType_NSDNAME);
      break;
    case DNSFilterEngine::PolicyType::NSIP:
      response->set_appliedpolicytype(PBDNSMessage_PolicyType_NSIP);
      break;
    default:
      throw std::runtime_error("Unsupported protobuf policy type");
    }
  }
#endif /* HAVE_PROTOBUF */
}

void RecProtoBufMessage::setPolicyTags(const std::vector<std::string>& policyTags)
{
#ifdef HAVE_PROTOBUF
  PBDNSMessage_DNSResponse* response = d_message.mutable_response();
  if (response) {
    for (const auto& tag : policyTags) {
      response->add_tags(tag);
    }
  }
#endif /* HAVE_PROTOBUF */
}

std::string RecProtoBufMessage::getAppliedPolicy() const
{
  std::string result;
#ifdef HAVE_PROTOBUF
  const PBDNSMessage_DNSResponse& response = d_message.response();
  result = response.appliedpolicy();
#endif /* HAVE_PROTOBUF */
  return result;
}

std::vector<std::string> RecProtoBufMessage::getPolicyTags() const
{
  std::vector<std::string> result;
#ifdef HAVE_PROTOBUF
  const PBDNSMessage_DNSResponse& response = d_message.response();
  const int count = response.tags_size();
  for (int idx = 0; idx < count; idx++) {
    result.push_back(response.tags(idx));
  }
#endif /* HAVE_PROTOBUF */
  return result;
}
