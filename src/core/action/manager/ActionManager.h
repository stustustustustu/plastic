#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include "../Action.h"

class ActionManager {
    private:
        std::map<std::string, Action> actions;
        std::map<std::string, bool> states;

    public:
        void registerAction(const std::string& name, const Action::Callback& callback);

        void executeAction(const std::string& name);

        bool getActionState(const std::string& name) const;
        void setActionState(const std::string& name, bool state);
};

#endif //ACTIONMANAGER_H
