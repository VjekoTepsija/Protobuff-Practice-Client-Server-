#pragma once

#include "main_proto.pb.h"

namespace server {

	main_proto::TaskResponse HandleTaskRequest(const main_proto::TaskRequest& request);

}
