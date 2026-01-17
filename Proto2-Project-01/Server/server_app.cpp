#include <iostream>
#include <fstream>
#include <string>

#include "server.h"
#include "main_proto.pb.h"

int main()
{
	std::cout << "Server starting...\n";

	const std::string requestPath = "request.bin";

	main_proto::TaskRequest request;

	std::ifstream in(requestPath, std::ios::binary);
	if (!in)
	{
		std::cerr << "\nServer: Cannot open/find " << requestPath;
		return 1;
	}

	if (!request.ParseFromIstream(&in))
	{
		std::cerr << "\nServer: Parse failed for '" << requestPath << "\n";
		return 2;
	}

	main_proto::TaskResponse response = server::HandleTaskRequest(request);
	std::string responsePath = "response.bin";
	std::ofstream out(responsePath, std::ios::binary);

	if (!out) {
		std::cerr << "\nServer: Cannot open/create " << responsePath;
		return 3;
	}

	if (!response.SerializeToOstream(&out)) {
		std::cerr << "\nServer: Failed to write response to " << responsePath;
		return 4;
	}

	std::cout << "\nServer finished successfully";
	return 0;
}
