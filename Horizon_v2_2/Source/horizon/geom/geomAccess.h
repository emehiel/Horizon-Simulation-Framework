#ifndef GEOM_ACCESS_H
#define GEOM_ACCESS_H

#include <vector>
#include <map>

#include "../Task.h"
#include "../System.h"


namespace horizon {
	namespace geom {

		using horizon::Task;
		using horizon::System;


vector< vector<const Task*> > getCurrentAccesses(vector<const Task*>& taskList, const vector< vector< map<double, bool> > >& access_pregen, double time);

vector< vector< map<double, bool> > > pregenerateAccesses(System& system, vector<const Task*>& tasks, double stepLength, double endTime);

}}
#endif