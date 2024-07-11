#include "MyExceptions.hpp"

MyExceptions::MyExceptions() {
    this->message_ = "Unpredictable error!";
}

MyExceptions::MyExceptions(const char *new_message) {
    this->message_ = new_message;
}

void MyExceptions::FunctionError() {
    MyExceptions exception("The function object being called "
                           "does not exist");

    std::cerr << exception.what();
    exit(EXIT_FAILURE);
}

void MyExceptions::TupleError() {
    MyExceptions exception("Tuple's index is out of range!");

    std::cerr << exception.what();
    exit(EXIT_FAILURE);
}
