#include "Auxillary.hpp"

#define ArgumentParser Auxiliary::ArgumentParser

template <typename R, typename... Args>
class TaskWithArguments : public TemplateTask<R> {
 public:
    class ICallable {
     public:
        virtual void Execute(MyTuple<Args...> args) = 0;
        virtual R NonVoidExecute(MyTuple<Args...> args) = 0;
        virtual ~ICallable() = default;
        ICallable() = default;
    };

    template<typename TCallable>
    class TCallableImpl : public ICallable {
     public:
        TCallableImpl(const TCallable& callable)
            : callable_(callable)
        {}

        void Execute(MyTuple<Args...> args) override {
            if (value_.get() == nullptr) {
                auto parsed_args = apply([](auto&&... args) {
                    return MyTuple(ArgumentParser(std::forward<Args>(args))...);
                }, args);

                value_ = std::make_unique<R>(apply(callable_, parsed_args));
            }
        }

        R NonVoidExecute(MyTuple<Args...> args) override {
            Execute(args);
            return *value_;
        }

     private:
        TCallable callable_;
        std::unique_ptr<R> value_;
    };

    R operator()(Args... args)  {
        return f_->Execute(args...);
    }

    template <class Func>
    explicit TaskWithArguments(Func f, Args... args) :
        f_(new TCallableImpl<Func>(f)),
        args_(std::forward<Args>(args)...)
    {}

    void Execute() { f_->Execute(args_); }
    R NonVoidExecute() { return f_->NonVoidExecute(args_); }

 private:
    std::unique_ptr<ICallable> f_;
    MyTuple<Args...> args_;
};
