#include "Event.h"

namespace horizon {

Event::Event(const Task* task, State* state) : task(task), state(state) {}

Event::Event(Event* eventToCopyExactly) : task(eventToCopyExactly->getTask()), state(new State(eventToCopyExactly->getState())) {}

Event::Event(shared_ptr<Event> eventToCopyExactly) : task(eventToCopyExactly->getTask()), state(new State(eventToCopyExactly->getState())) {}

Event::~Event() { /*delete state;*/ }

} // end namespace horizon
