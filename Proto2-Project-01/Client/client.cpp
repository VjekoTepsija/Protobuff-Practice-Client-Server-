#include <iostream>
#include <fstream>
#include <string>

#include "main_proto.pb.h"

void StringTask(int id);
void ArithTask(int id);

void TaskCreation(int id)
{
    int choice = 0;

    while (true) {
        std::cout << "\nEnter your choice: 1. String Task 2. Arithmetic Task Other: Exit ";
        std::cin >> choice;

        if (choice == 1) {
            StringTask(id);
            return;
        }
        else if (choice == 2) {
            ArithTask(id);
            return;
        }
        else {
            std::cerr << "Exiting, no task created.";
            return;
        }
    }
}

void SendTask(const main_proto::TaskRequest& task)
{
    std::string requestPath = "request.bin";

    std::ofstream out(requestPath, std::ios::binary);
    if (!out) {
        std::cerr << "\nFailed to open/create file: " << requestPath;
        return;
    }

    if (!task.SerializeToOstream(&out)) {
        std::cerr << "\nClient: Failed to write request to " << requestPath;
        return;
    }

    std::cout << "\nTask serialized to " << requestPath;
}

void StringTask(int id)
{
    main_proto::TaskRequest request;
    request.set_id(id);
    request.set_type(main_proto::TASK_STRING);

    main_proto::StringJob* job = request.mutable_string();

    std::cout << "\nEnter word: ";
    std::string input;
    std::cin >> input;
    job->set_input(input);

    int choice = 0;

    while (true) {
        std::cout << "Enter type of operation: 1. Reverse String 2. Uppercase String: ";
        std::cin >> choice;

        if (choice == 1) {
            job->set_operation(main_proto::STR_REVERSE);
            break;
        }
        else if (choice == 2) {
            job->set_operation(main_proto::STR_UPPER);
            break;
        }
        else {
            std::cerr << "Invalid operation. Try again";
            continue;
        }
    }

    SendTask(request);
}

void ArithTask(int id)
{
    main_proto::TaskRequest request;
    request.set_id(id);
    request.set_type(main_proto::TASK_ARITH);

    main_proto::ArithmeticJob* job = request.mutable_arithmetic();

    int choice = 0;

    while (true) {
        std::cout << "Enter type of operation: 1. Sum 2. Multiply 3. Average: ";
        std::cin >> choice;

        if (choice == 1) {
            job->set_operation(main_proto::AR_SUM);
            break;
        }
        else if (choice == 2) {
            job->set_operation(main_proto::AR_MULTIPLY);
            break;
        }
        else if (choice == 3) {
            job->set_operation(main_proto::AR_AVERAGE);
            break;
        }
        else {
            std::cerr << "Invalid operation. Try again";
            continue;
        }
    }

    int n = 0;
    while (true) {
        std::cout << "How many numbers? ";
        std::cin >> n;

        if (n > 0) {
            break;
        }
        else {
            std::cerr << "Invalid count. Try again";
            continue;
        }
    }
    int x = 0;
    for (int i = 0; i < n; i++) {
        std::cout << "Number " << (i + 1) << ": ";
        std::cin >> x;
        job->add_numbers(x);
    }

    SendTask(request);
}
void ReadAndPrintResponse()
{
    std::string responsePath = "response.bin";
    std::ifstream in(responsePath, std::ios::binary);

    if (!in) {
        std::cerr << "\nClient: Cannot open/find " << responsePath;
        return;
    }

    main_proto::TaskResponse response;
    if (!response.ParseFromIstream(&in)) {
        std::cerr << "\nClient: Failed to parse " << responsePath;
        return;
    }

    std::cout << "\n--- Response ---";

    if (response.has_id()) {
        std::cout << "\nId: " << response.id();
    }

    if (response.has_error()) {
        std::cout << "\nError code: " << response.error().code();
        if (response.error().has_message()) {
            std::cout << "\nError message: " << response.error().message();
        }
        std::cout << "\n--------------\n";
        return;
    }

    if (response.has_arith_result()) {
        const main_proto::ArithmeticResult& r = response.arith_result();

        if (r.has_sum()) {
            std::cout << "\nSum: " << r.sum();
        }
        else if (r.has_multiply()) {
            std::cout << "\nMultiply: " << r.multiply();
        }
        else if (r.has_average()) {
            std::cout << "\nAverage: " << r.average();
        }
        else {
            std::cout << "\nArithmetic result present but no fields set.";
        }

        std::cout << "\n--------------\n";
        return;
    }

    if (response.has_str_result()) {
        const main_proto::StringResult& r = response.str_result();

        if (r.has_output()) {
            std::cout << "\nOutput: " << r.output();
        }
        else {
            std::cout << "\nString result present but output not set.";
        }

        std::cout << "\n--------------\n";
        return;
    }

    std::cout << "\nNo result and no error present.";
    std::cout << "\n--------------\n";
}

int main()
{
    std::cout << "\nClient starting\n";

    int id = 1;

    TaskCreation(id);

    std::cout << "\nTask created and sent, turning on server\n";


    int code = std::system("server_app.exe");
    //Planira sam ih imat separate pa da se pokrecu kao dva odvojena programa ali 
    // radi jednostavnosti sam nabrzinu ubacio da client pokrene server

    if (code != 0) {
        std::cerr << "\nClient: server failed to run";
        return 1;
    }

    ReadAndPrintResponse();

    return 0;
}

