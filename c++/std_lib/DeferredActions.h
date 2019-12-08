#ifndef _DEFERRED_ACTIONS_H
#define _DEFERRED_ACTIONS_H

#include <list>
#include <functional>
#include <memory>

template <typename F, typename C, typename... ARGS>
class Action
{
public:
    Action(F&& f, const std::weak_ptr<C>& c, ARGS&&...args) 
    : mWeak(c)
    , mCallable(std::bind(std::forward<F>(f), std::placeholders::_1, std::forward<ARGS>(args)...))
    {

    }

    void operator()() const {
        std::shared_ptr<C> strong = mWeak.lock();
        if (strong != NULL) {
            mCallable(strong.get());
        } else {
        }
    }

private:
    std::weak_ptr<C> mWeak;
    std::function<void(C*)> mCallable;
};

template <typename F, typename C, typename... ARGS>
auto makeAction(F&& f, const std::weak_ptr<C>& c, ARGS&&... args)  -> std::function<void()>
{
    return Action<F,C,ARGS...>(std::forward<F>(f), c, std::forward<ARGS>(args)...);
}

class ActionProcesser
{
public:
    ActionProcesser() {}
    ~ActionProcesser() {}

    void add(const std::function<void()>& func) {
        mActions.push_back(func);
    }

    void run(const std::function<bool()>& condition =[]{return true;}) {
        if (!condition()) {
            return;
        }

        while (!mActions.empty()) {
            auto f = *mActions.begin();
            f();
            mActions.erase(mActions.begin());
        }
    }

private:
    std::list<std::function<void()>> mActions;

    ActionProcesser(ActionProcesser&) = delete;
    ActionProcesser& operator=(const ActionProcesser&) = delete;
};


#endif
