#include <gtest/gtest.h>
#include "server.h"

TEST(Server, NoNumbers) {
	main_proto::TaskRequest task;
	task.set_id(2);
	task.set_type(main_proto::TASK_ARITH);

	main_proto::ArithmeticJob* job = task.mutable_arithmetic();
	job->set_operation(main_proto::AR_SUM);

	main_proto::TaskResponse response = server::HandleTaskRequest(task);
	ASSERT_EQ(response.has_error(), true);

}

TEST(Server, Suma) {
	main_proto::TaskRequest task;
	task.set_id(2);
	task.set_type(main_proto::TASK_ARITH);

	main_proto::ArithmeticJob* job = task.mutable_arithmetic();
	job->set_operation(main_proto::AR_SUM);
	job->add_numbers(2);
	job->add_numbers(3);

	main_proto::TaskResponse response = server::HandleTaskRequest(task);
	ASSERT_EQ(response.id(), 2);
	ASSERT_EQ(response.has_arith_result(), true);
	ASSERT_EQ(response.has_error(), false);
	EXPECT_EQ(response.arith_result().sum(), 5);
}

TEST(Server, Multiply) {
	main_proto::TaskRequest task;
	task.set_id(3);
	task.set_type(main_proto::TASK_ARITH);

	main_proto::ArithmeticJob* job = task.mutable_arithmetic();
	job->set_operation(main_proto::AR_MULTIPLY);
	job->add_numbers(2);
	job->add_numbers(3);

	main_proto::TaskResponse response = server::HandleTaskRequest(task);
	ASSERT_EQ(response.id(), 3);
	ASSERT_EQ(response.has_arith_result(), true);
	ASSERT_EQ(response.has_error(), false);
	EXPECT_EQ(response.arith_result().multiply(), 6);
}

TEST(Server, Average) {
	main_proto::TaskRequest task;
	task.set_id(4);
	task.set_type(main_proto::TASK_ARITH);

	main_proto::ArithmeticJob* job = task.mutable_arithmetic();
	job->set_operation(main_proto::AR_AVERAGE);
	job->add_numbers(2);
	job->add_numbers(3);

	main_proto::TaskResponse response = server::HandleTaskRequest(task);
	ASSERT_EQ(response.id(), 4);
	ASSERT_EQ(response.has_arith_result(), true);
	ASSERT_EQ(response.has_error(), false);
	EXPECT_EQ(response.arith_result().average(), 2.5);
}

TEST(Server, Upper) {
	main_proto::TaskRequest task;
	task.set_id(5);
	task.set_type(main_proto::TASK_STRING);

	main_proto::StringJob* job = task.mutable_string();
	job->set_operation(main_proto::STR_UPPER);
	job->set_input("test");

	main_proto::TaskResponse response = server::HandleTaskRequest(task);
	ASSERT_EQ(response.id(), 5);
	ASSERT_EQ(response.has_str_result(), true);
	ASSERT_EQ(response.has_error(), false);
	EXPECT_EQ(response.str_result().output(), "TEST");
}

TEST(Server, Reverse) {
	main_proto::TaskRequest task;
	task.set_id(6);
	task.set_type(main_proto::TASK_STRING);

	main_proto::StringJob* job = task.mutable_string();
	job->set_operation(main_proto::STR_REVERSE);
	job->set_input("test");

	main_proto::TaskResponse response = server::HandleTaskRequest(task);
	ASSERT_EQ(response.id(), 6);
	ASSERT_EQ(response.has_str_result(), true);
	ASSERT_EQ(response.has_error(), false);
	EXPECT_EQ(response.str_result().output(), "tset");
}

TEST(Server, StringErrors) {
	main_proto::TaskRequest task;
	task.set_id(7);
	task.set_type(main_proto::TASK_STRING);

	main_proto::StringJob* job = task.mutable_string();
	job->set_input("test");

	main_proto::TaskResponse response1 = server::HandleTaskRequest(task);
	ASSERT_EQ(response1.has_error(), true);
	job->set_operation(main_proto::STR_UPPER);

	job->clear_input();
	main_proto::TaskResponse response2 = server::HandleTaskRequest(task);
	ASSERT_EQ(response2.has_error(), true);
	
}