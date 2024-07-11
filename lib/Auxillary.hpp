#include "lib\utils\MyFunction.hpp"

#include <memory>

class Task {
 public:
    Task() = default;
    virtual ~Task() = default;
    virtual void Execute() = 0;
};

template <typename T>
class TemplateTask : public Task {
 public:
    virtual T NonVoidExecute() = 0;
};

template <typename T>
class SpecializedTask {
 public:
    explicit SpecializedTask(std::unique_ptr<Task>& task) :
        task_(*dynamic_cast<TemplateTask<T>*>(task.get())) {}

    T NonVoidExecute() { return task_.NonVoidExecute(); }

 private:
    TemplateTask<T>& task_;
};

class Auxiliary {
 public:
    template <typename T>
    static T ArgumentParser(T&& arg) {
        return std::forward<T>(arg);
    }

    template <typename T>
    static T ArgumentParser(SpecializedTask<T>&& view) {
        return view.NonVoidExecute();
    }

    template <typename... Args>
    static auto ArgumentParser(std::tuple<Args...> args) {
        return std::make_tuple(ArgumentParser(std::forward<Args>(args))...);
    }
};
