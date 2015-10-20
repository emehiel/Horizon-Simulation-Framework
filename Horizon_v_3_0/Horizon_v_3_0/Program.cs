using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Horizon.Universe;
using Horizon_Utilities;

namespace Horizon
{
    class Program
    {
        static void Main(string[] args)
        {
            SortedList<double, Matrix> state = new SortedList<double, Matrix>();
            DynamicState myState = new DynamicState(state, DynamicStateType.DYNAMIC_ECI);
            Target newTarget = new Target("newTarget", TargetType.FACILITY, myState, 5);
            Task newTask = new Task(taskType.COMM, newTarget, 5);

            Console.WriteLine(state);
            Console.WriteLine(myState);
            Console.WriteLine(newTarget);
            Console.WriteLine(newTask);

            Console.ReadLine();
        }
    }
}
