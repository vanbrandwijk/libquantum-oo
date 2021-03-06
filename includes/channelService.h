/*
 * channelService.h
 */

#ifndef __QUANTUMCHANNEL_CHANNEL_SERVICE_H
#define __QUANTUMCHANNEL_CHANNEL_SERVICE_H

#include <memory>

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_context.h>
#include <grpc++/support/status.h>
#include "quantumMessage.grpc.pb.h"
#include "quantumMessage.pb.h"

using namespace std;

namespace QuantumChannel {
class ChannelService final : public QuantumMessage::QuantumChannel::Service {
	public:
	grpc::Status SendQubit(grpc::ServerContext* context,
		const QuantumMessage::QubitMessage* request,
		QuantumMessage::RemoteIndexMessage* reply) override;

	grpc::Status SendMeasurementMessage(grpc::ServerContext* context,
		const QuantumMessage::MeasurementMessage* request,
		QuantumMessage::VoidMessage* reply) override;

	grpc::Status SendClassicData(grpc::ServerContext* context,
		const QuantumMessage::ClassicMessage* request,
		QuantumMessage::VoidMessage* reply) override;

/*
	grpc::Status SendEntangledRegister(grpc::ServerContext* context, 
		const QuantumMessage::EntangledRegisterMessage* request, 
		QuantumMessage::InetAddr* reply) override;
	grpc::Status EventPairMeasureFinish(
		grpc::ServerContext* context, 
		const QuantumMessage::EntangledMeasurementMessage* request, 
		QuantumMessage::VoidMessage* reply) override;
*/
};
}

#endif
