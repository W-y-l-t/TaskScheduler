#include "..\lib\MyScheduler.hpp"

#include <gtest/gtest.h>

#include <cmath>

struct func2 {
    int64_t operator()(int64_t a, int64_t b) {
        return a + b;
    }
};

int64_t func3(int64_t a, int64_t b) {
    return a + b;
}

TEST(MyFunction, Test) {
    auto func1 = [](int64_t a, int64_t b) {return a + b; };

    MyFunction<int64_t(int64_t, int64_t)> f;

    f = func1;
    EXPECT_EQ(f(12389, 98), 12487);

    f = func2{};
    EXPECT_EQ(f(-1398339, 9990010), 8591671);

    f = func3;
    EXPECT_EQ(f(983789754, -325898432), 657891322);

    auto func4 = [](std::string& s, char c) { s += c; return s; };
    MyFunction<std::string(std::string&, char)> f2;

    f2 = func4;

    std::string s = "Hello, ";
    EXPECT_EQ(f2(s, 'w'), "Hello, w");
}

TEST(MyTuple, Test) {
    MyTuple<int32_t, double, std::string> tuple(1, 2.0, "hhhh");

    EXPECT_EQ(get<0>(tuple), 1);
    EXPECT_EQ(get<1>(tuple), 2.0);
    EXPECT_EQ(get<2>(tuple), "hhhh");

    MyTuple<int32_t, double, std::string, char> tuple2(1, 2.0, "hh", 'a');

    EXPECT_EQ(get<0>(tuple2), 1);
    EXPECT_EQ(get<1>(tuple2), 2.0);
    EXPECT_EQ(get<2>(tuple2), "hh");
    EXPECT_EQ(get<3>(tuple2), 'a');

    MyTuple<int32_t> tuple3(1);

    EXPECT_EQ(get<0>(tuple3), 1);

    MyTuple<int32_t, int32_t, int32_t> tuple4(1, 2, 4);
    auto sum = [](int32_t a, int32_t b, int32_t c) { return a + b + c; };

    EXPECT_EQ(apply(sum, tuple4), 7);
}

TEST(MySchedulerTestSuite, Test) {
    MyScheduler scheduler;

    auto id1 = scheduler.add([](float a, float c) {return -4 * a * c; }, 1.0, 0.0);
    auto id2 = scheduler.add([](float b, float v) {return b * b + v; }, -2.0, scheduler.getFutureResult<float>(id1));
    auto id3 = scheduler.add([](float b, float d) {return -b + std::sqrt(d); }, -2.0, scheduler.getFutureResult<float>(id2));
    auto id4 = scheduler.add([](float b, float d) {return -b - std::sqrt(d); }, -2.0, scheduler.getFutureResult<float>(id2));
    auto id5 = scheduler.add([](float a, float v) {return v / (2 * a); }, 1.0, scheduler.getFutureResult<float>(id3));
    auto id6 = scheduler.add([](float a, float v) {return v / (2 * a); }, 1.0, scheduler.getFutureResult<float>(id4));

    scheduler.executeAll();

    EXPECT_EQ(scheduler.getResult<float>(id5), 2.0);
    EXPECT_EQ(scheduler.getResult<float>(id6), 0.0);

    auto id7 = scheduler.add([](float a, float b, float c) {return a + b + c; }, 1.0, -2.0, 0.0);
    auto id8 = scheduler.add([](float a, float b, float c, float v) {return a + b + c + v; }, 1.0, -2.0, 0.0, scheduler.getFutureResult<float>(id7));

    EXPECT_EQ(scheduler.getResult<float>(id8), -2.0);
    EXPECT_EQ(scheduler.getResult<float>(id7), -1.0);
}

TEST(MySchedulerTestSuite, Test2) {
    MyScheduler scheduler;

    auto id1 = scheduler.add([](int64_t a, int64_t b) { return a + b; },
                             1,
                             2);
    auto id2 = scheduler.add([](int64_t a, int64_t b) { return a + b; },
                             3,
                             scheduler.getFutureResult<int64_t>(id1));
    auto id3 = scheduler.add([](int64_t a, int64_t b) { return a + b; },
                             4,
                             scheduler.getFutureResult<int64_t>(id2));
    auto id4 = scheduler.add([](int64_t a, int64_t b) { return a + b; },
                             5,
                             scheduler.getFutureResult<int64_t>(id3));

    EXPECT_EQ(scheduler.getResult<int64_t>(id3), 10);
    EXPECT_EQ(scheduler.getResult<int64_t>(id2), 6);
    EXPECT_EQ(scheduler.getResult<int64_t>(id4), 15);
    EXPECT_EQ(scheduler.getResult<int64_t>(id1), 3);
}

TEST(MySchedulerTestSuite, Test3) {
    MyScheduler scheduler;

    auto id1 = scheduler.add([](std::vector<char> a, std::vector<char> b) { return a == b; },
                             std::vector<char>{'a', 'b', 'c'},
                             std::vector<char>{'a', 'b', 'c'});

    EXPECT_EQ(scheduler.getResult<bool>(id1), true);

    auto id2 = scheduler.add([](std::vector<char> a, std::vector<char> b) { return a == b; },
                             std::vector<char>{'a', 'b', 'c'},
                             std::vector<char>{'a', 'b', 'd'});

    EXPECT_EQ(scheduler.getResult<bool>(id2), false);
}

TEST(MySchedulerTestSuite, Test4) {
    MyScheduler scheduler;

    auto id1 = scheduler.add([](std::map<int, int> mp) { return mp[1]; },
                             std::map<int, int>{{1, 2}, {2, 1}});

    auto id2 = scheduler.add([](std::map<int, int> mp) { return mp[3]; },
                             std::map<int, int>{{3, 3}, {1, 1}});

    auto id3 = scheduler.add([](int a, int b) { return a + b; },
                             scheduler.getFutureResult<int>(id1),
                             scheduler.getFutureResult<int>(id2));

    EXPECT_EQ(scheduler.getResult<int>(id3), 5);
    EXPECT_EQ(scheduler.getResult<int>(id1), 2);
    EXPECT_EQ(scheduler.getResult<int>(id2), 3);
}

TEST(MySchedulerTestSuite, Test5) {
    MyScheduler scheduler;

    auto id1 = scheduler.add([](std::vector<int> a) { return a[0]; },
                             std::vector<int>{1, 2, 3});

    auto id2 = scheduler.add([](std::vector<int> a) { return a[1]; },
                             std::vector<int>{1, 2, 3});

    auto id3 = scheduler.add([](int a, int b) { return a + b; },
                             scheduler.getFutureResult<int>(id1),
                             scheduler.getFutureResult<int>(id2));

    scheduler.executeAll();

    auto id4 = scheduler.add([](int a, int b) { return a + b; },
                  scheduler.getFutureResult<int>(id1),
                  scheduler.getFutureResult<int>(id2));

    EXPECT_EQ(scheduler.getResult<int>(id3), 3);
    EXPECT_EQ(scheduler.getResult<int>(id1), 1);
    EXPECT_EQ(scheduler.getResult<int>(id2), 2);

    EXPECT_EQ(scheduler.getResult<int>(id4), 3);
}
