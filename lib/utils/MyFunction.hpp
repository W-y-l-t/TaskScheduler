#include "MyExceptions.hpp"
#include "MyTuple.hpp"

#include <memory>
#include <functional>

template<typename TFunc>
class MyFunction;

template<typename R, typename... Args>
class MyFunction<R(Args...)> {
 public:
    class ICallable {
     public:
        virtual R Execute(Args... args) = 0;
        virtual ~ICallable() = default;
        ICallable() = default;
    };

    template<typename TCallable>
    class TCallableImpl : public ICallable {
     public:
        TCallableImpl(const TCallable& callable)
            : callable_(callable)
        {}

        R Execute(Args... args) override {
            return callable_(args...);
        }

     private:
        TCallable callable_;
    };

    MyFunction() = default;

    template<typename TCallable>
    MyFunction(TCallable callable)
        : callable_(new TCallableImpl<TCallable>(callable))
    {}

    R operator()(Args... args)  {
        return callable_->Execute(args...);
    }

 private:
    std::unique_ptr<ICallable> callable_;
};
