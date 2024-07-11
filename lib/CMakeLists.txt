add_library(MyScheduler
            MyScheduler.hpp
        utils/MyFunction.hpp
            utils/MyExceptions.cpp
            utils/MyExceptions.hpp
            utils/MyTuple.hpp
            TaskHelper.hpp
        Auxillary.hpp
)

set_target_properties(MyScheduler PROPERTIES LINKER_LANGUAGE CXX)