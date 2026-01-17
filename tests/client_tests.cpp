#include <gtest/gtest.h>
#include "main_proto.pb.h"

TEST(UserInput, ArithmeticSum) {
	main_proto::TaskRequest task;
	task.set_id(123);
	task.set_type(main_proto::TASK_ARITH);

	EXPECT_FALSE(task.has_version());
	EXPECT_TRUE(task.version() == 1);

	main_proto::ArithmeticJob* job = task.mutable_arithmetic();
	job->set_operation(main_proto::AR_SUM);
	job->add_numbers(5);
	job->add_numbers(5);

	EXPECT_TRUE(task.has_arithmetic());
	EXPECT_FALSE(task.has_string());

	main_proto::TaskResponse response;
	response.set_id(321);

	EXPECT_FALSE(response.has_version());
	EXPECT_TRUE(response.version() == 1);

	main_proto::ArithmeticResult* result = response.mutable_arith_result();
	result->set_sum(10);

	EXPECT_TRUE(response.has_arith_result());
	EXPECT_FALSE(response.has_str_result());
	EXPECT_FALSE(response.has_error());
}