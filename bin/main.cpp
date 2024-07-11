#include <cmath>
#include "..\lib\MyScheduler.hpp"

int32_t main(int32_t, char**) {
    MyScheduler scheduler;

    auto id1 = scheduler.add([](float a, float c) {return -4 * a * c; }, 1.0, 0.0);
    auto id2 = scheduler.add([](float b, float v) {return b * b + v; }, -2.0, scheduler.getFutureResult<float>(id1));
    auto id3 = scheduler.add([](float b, float d) {return -b + std::sqrt(d); }, -2.0, scheduler.getFutureResult<float>(id2));
    auto id4 = scheduler.add([](float b, float d) {return -b - std::sqrt(d); }, -2.0, scheduler.getFutureResult<float>(id2));
    auto id5 = scheduler.add([](float a, float v) {return v / (2 * a); }, 1.0, scheduler.getFutureResult<float>(id3));
    auto id6 = scheduler.add([](float a, float v) {return v / (2 * a); }, 1.0, scheduler.getFutureResult<float>(id4));

    scheduler.executeAll();

    std::cout << scheduler.getResult<float>(id5) << std::endl;
    std::cout << scheduler.getResult<float>(id6) << std::endl;
}
