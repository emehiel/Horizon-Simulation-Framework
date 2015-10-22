#ifndef UI_H_
#define UI_H_

#include <string>

namespace horizon {
namespace ui {

using std::string;

/**
 * A user interface to the application, providing support for interactivity.
 * @author Einar Pehrson
 */
class UI {

protected:
	// const Properties simulationProperties;
	// const IOManager ioManager;

public:
	UI();
	// virtual ~UI() = 0;
	// virtual void show() = 0;
	// void runScheduler();

protected:
	// virtual void loadSimulationProperties() = 0;
	// virtual void loadEnvironment() = 0;
	// virtual void loadSystem() = 0; // system, subsystem, initial state, constraints
	// virtual void loadTargets() = 0;
	// virtual void loadTasks() = 0;

	// virtual bool confirm(string title, void* message, bool allowCancel, bool warning);
	// virtual void showError(void* error);
	// virtual void showSchedulingProgress(float timeProgress, int plansGenerated, int plansEvaluated);
};

}} // end namespace horizon::ui

#endif /*UI_H_*/
