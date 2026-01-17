#include "server.h"
#include <string>

namespace server {


	main_proto::TaskResponse HandleString(const main_proto::TaskRequest& request) {
		main_proto::TaskResponse response;

		if (request.has_id())
		{
			response.set_id(request.id());
		}

		if (!request.has_string()) {
			auto* err = response.mutable_error();
			err->set_code(main_proto::ERR_BAD_TASK);
			err->set_message("\nNo string job");

			return response;
		}

		const main_proto::StringJob& job = request.string();

		if (!job.has_operation()) {
			auto* err = response.mutable_error();
			err->set_code(main_proto::ERR_BAD_TASK);
			err->set_message("\nNo operation");

			return response;
		}

		if (job.operation() != main_proto::STR_REVERSE && job.operation() != main_proto::STR_UPPER) {
			auto* err = response.mutable_error();
			err->set_code(main_proto::ERR_UNSUPPORTED);
			err->set_message("\nNot a valid operation");

			return response;
		}

		if (!job.has_input()) {
			auto* err = response.mutable_error();
			err->set_code(main_proto::ERR_BAD_TASK);
			err->set_message("\nNo input");

			return response;
		}

		std::string word = job.input();

		if (job.operation() == main_proto::STR_REVERSE) {
			word = std::string(word.rbegin(), word.rend());
		}
		else if (job.operation() == main_proto::STR_UPPER) {
			for (int i = 0; i < word.size(); i++) {
				if (word[i] >= 'a' && word[i] <= 'z') {
					word[i] = word[i] - ('a' - 'A');
				}
			}

		}
		response.mutable_str_result()->set_output(word);
		return response;
	}

	main_proto::TaskResponse HandleArithmetic(const main_proto::TaskRequest& request) {
		main_proto::TaskResponse response;

		if (request.has_id())
		{
			response.set_id(request.id());
		}

		if (!request.has_arithmetic()) {
			auto* err = response.mutable_error();
			err->set_code(main_proto::ERR_BAD_TASK);
			err->set_message("\nNo arithmetic job");

			return response;
		}
		const main_proto::ArithmeticJob& job = request.arithmetic();
		if (!job.has_operation() || job.numbers_size() == 0) {
			auto* err = response.mutable_error();
			err->set_code(main_proto::ERR_BAD_TASK);
			err->set_message("\nNo operation/numbers in arith. job");

			return response;
		}
		int sum = 0;
		double avg = 0;
		int multiply = 1;
		switch (job.operation()) {
		case main_proto::AR_SUM:
			for (int i = 0; i < job.numbers_size(); i++) {
				sum += job.numbers(i);
			}
			response.mutable_arith_result()->set_sum(sum);
			return response;
		case main_proto::AR_MULTIPLY:
			for (int i = 0; i < job.numbers_size(); i++) {
				multiply *= job.numbers(i);
			}
			response.mutable_arith_result()->set_multiply(multiply);
			return response;
		case main_proto::AR_AVERAGE:
			for (int i = 0; i < job.numbers_size(); i++) {
				sum += job.numbers(i);
			}
			avg = (double)sum / job.numbers_size();
			response.mutable_arith_result()->set_average(avg);

			return response;
		default:
			auto* err = response.mutable_error();
			err->set_code(main_proto::ERR_UNSUPPORTED);
			err->set_message("\n...what did you do to end up here?");

			return response;
		}
	}

	main_proto::TaskResponse HandleTaskRequest(const main_proto::TaskRequest& request)
	{
		main_proto::TaskResponse response;

		if (!request.has_type()) {
			auto* err = response.mutable_error();
			err->set_code(main_proto::ERR_BAD_TASK);
			err->set_message("\nTask type missing");

			return response;
		}

		if (request.type() == main_proto::TASK_STRING) {
			response = HandleString(request);
		}
		else if (request.type() == main_proto::TASK_ARITH) {
			response = HandleArithmetic(request);
		}
		else {
			auto* err = response.mutable_error();
			err->set_code(main_proto::ERR_UNSUPPORTED);
			err->set_message("Unsupported task type");
		}
		return response;
	}
}
