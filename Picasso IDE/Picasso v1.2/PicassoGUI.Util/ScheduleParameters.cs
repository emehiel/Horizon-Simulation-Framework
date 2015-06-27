using System;

namespace PicassoGUI.Util
{
    [Serializable]
    public class ScheduleParameters
    {
        public Decimal SimStepSize { get; set; }
        public Decimal MaxSchedules { get; set; }
        public Decimal MinSchedules { get; set; }
    }
}
