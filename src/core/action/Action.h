#ifndef ACTION_H
#define ACTION_H

#include <functional>
#include "../src/config.h"

class Action {
    public:
        using Callback = std::function<void()>;

        void addCallback(const Callback& callback);

        void execute();

    private:
        std::vector<Callback> callbacks;
};



#endif //ACTION_H
