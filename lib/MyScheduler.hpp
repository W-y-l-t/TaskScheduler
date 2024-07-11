#include "TaskHelper.hpp"

const int32_t kStartIndex = -1;

class MyScheduler {
 public:
    MyScheduler() = default;
    ~MyScheduler() = default;

    template <typename Function, typename... Args>
    size_t add(Function&& function, Args&&... args) {
        using ReturnType = std::invoke_result_t<Function,
                                decltype(ArgumentParser(std::forward<Args>(args)))...>;

        tasks_.push_back(std::make_unique<TaskWithArguments<ReturnType, Args...>>
                                (std::forward<Function>(function), std::forward<Args>(args)...));
        current_task_id_ += 1;

        return current_task_id_;
    }

    template <typename T>
    SpecializedTask<T> getFutureResult(size_t task_id) {
        return SpecializedTask<T>(tasks_[task_id]);
    }

    template <typename T>
    T getResult(size_t task_id){
        return SpecializedTask<T>(tasks_[task_id]).NonVoidExecute();
    }

    void executeAll() {
        for (auto& task : tasks_) {
            task->Execute();
        }
    }

 private:
    size_t current_task_id_ = kStartIndex;
    std::vector<std::unique_ptr<Task>> tasks_{};
};
