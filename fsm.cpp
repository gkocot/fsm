#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
using namespace std;

// TBD State entry/exit actions.
// TBD List of ections instead of just one.
// TBD Event object instead of event_id, object could contain event specific parameters.
#define state_init      0
#define state_1         1
#define state_2         2

#define event_init      0
#define event_10        10
#define event_11        11
#define event_2         2

template <class T>
struct transition {
    int start_state;
    int event;
    void (T:: * action) (void *);
    int end_state;

    transition(
        int start_state,
        int event,
        void (T:: * action) (void *),
        int end_state
    ) :
        start_state(start_state),
        event(event),
        action(action),
        end_state(end_state)
    {}
};

template <class T>
struct fsm {
    int state;
    const vector<transition<T>> ttable;

    fsm(const vector<transition<T>> & ttable) : ttable(ttable)
    {
        state = ttable[0].start_state;
        exec(event_init);
    }

    void exec(int event)
    {
        auto it = find_if(
            ttable.begin(),
            ttable.end(),
            [this, &event](transition<T> const & t) { return t.start_state == state && t.event == event; }
        );

        if (it != ttable.end())
        {
            if (it->action != nullptr)
            {
                (static_cast<T *>(this)->*(it->action))(nullptr);
            }
            state = it->end_state;
        }
    }
};

struct my_fsm : public fsm<my_fsm> {
    my_fsm() : fsm(
        {
            {state_init,    event_init, &my_fsm::action_init,   state_1},
            {state_1,       event_10,    &my_fsm::action_10,      state_1},
            {state_1,       event_11,    &my_fsm::action_11,      state_2},
            {state_2,       event_2,    &my_fsm::action_2,      state_1},
        }
        ) {}

    void action_init(void *) { cout << __FUNCTION__ << endl; }
    void action_10(void *) { cout << __FUNCTION__ << endl; }
    void action_11(void *) { cout << __FUNCTION__ << endl; }
    void action_2(void *) { cout << __FUNCTION__ << endl; }
} my_fsm1;


void main()
{
    //my_fsm1.exec(event_init);
    my_fsm1.exec(event_10);
    my_fsm1.exec(event_11);
    my_fsm1.exec(event_2);
}
