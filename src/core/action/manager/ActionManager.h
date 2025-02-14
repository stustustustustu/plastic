#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include "../Action.h"

class ActionManager {
    private:
        std::map<std::string, Action> actions;

    public:
        void registerAction(const std::string& name, const Action::Callback& callback);

        void executeAction(const std::string& name);
};



#endif //ACTIONMANAGER_H
