#include <iostream>

class MyExceptions : std::exception {
 public:
    MyExceptions();
    explicit MyExceptions(const char* new_message);

    [[nodiscard]] const char* what() const noexcept override {
        return message_;
    }

    static void FunctionError();

    static void TupleError();

 private:
    const char* message_;
};
