using System;

namespace Hsf.Utility
{
    [Serializable]
    public class ScheduleParameters
    {
        public Decimal SimStepSize { get; set; }
        public Decimal MaxSchedules { get; set; }
        public Decimal MinSchedules { get; set; }
    }
}
