using System;

namespace PicassoGUI.Util
{
    public class ScenarioParameters
    {
        public String ScenarioName { get; set; }
        public DateTime simStartTime { get; set; }
        public DateTime simEndTime { get; set; }

        public Decimal simTimeStep { get; set; }
        public Decimal maxSchedules { get; set; }
        public Decimal minSchedules { get; set; }

        public ScenarioParameters()
        {
        }

        public ScenarioParameters(String name, DateTime simStart, DateTime simEnd, Decimal simStep, Decimal max, Decimal min)
        {
            this.ScenarioName = name;
            this.simStartTime = simStart;
            this.simEndTime = simEnd;
            this.simTimeStep = simStep;
            this.maxSchedules = max;
            this.minSchedules = min;
        }

    }
}
