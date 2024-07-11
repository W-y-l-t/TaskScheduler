# TaskScheduler

> A task scheduler console application. \
> In other words, a class responsible for performing data-related tasks.

## Usage example

Often, in order to solve any task, it is necessary to perform a graph of calculations, where the node of the graph is a task, an edge is the connection between the result of one task and the parameter for starting another.
TaskScheduler solves a similar problem.

An example of such a problem is finding the roots of a quadratic equation (assuming that the coefficients guarantee that the root is 2). 
Solving such a problem head-on implies calculating the root of the discriminant twice. Obviously, this is not optimal.

This is how the code for solving this problem using TaskScheduler might look like.

```cpp
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
```

## Public interface

--------------------------------------------------
**add** - takes a task for it as an argument. \
Returns an object describing the added longing.

--------------------------------------------------
**getFutureResult** - returns an object from which in the future you can get the result of a task passed as a result of type T.

--------------------------------------------------
**getResult** - returns the result of a specific type of task. \
Calculates it if it has not been calculated yet. 

--------------------------------------------------
**executeAll** - executes all scheduled tasks

--------------------------------------------------

## Features

- The standard library is not used, except for containers and smart pointers.
- std::tuple, std::apply and std::function were implemented by me
- An arbitrary number of arguments