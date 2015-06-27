#include "State.h"

namespace horizon {

State::State() : previous(0), eventStart(0), taskStart(0), taskEnd(0), eventEnd(0) {}

State::State(State* initialStateToCopy) : previous(initialStateToCopy->previous), eventStart(initialStateToCopy->eventStart), 
taskStart(initialStateToCopy->taskStart), taskEnd(initialStateToCopy->taskEnd), eventEnd(initialStateToCopy->eventEnd), 
idata(initialStateToCopy->idata), ddata(initialStateToCopy->ddata), fdata(initialStateToCopy->fdata), 
bdata(initialStateToCopy->bdata), mdata(initialStateToCopy->mdata), qdata(initialStateToCopy->qdata) {}


State::State(State* previous, const double newTaskStart) : previous(previous), eventStart(previous->getEventEnd()),
taskStart(newTaskStart), taskEnd(newTaskStart), eventEnd(newTaskStart) {}

//State::~State() {}

} // end namespace horizon
