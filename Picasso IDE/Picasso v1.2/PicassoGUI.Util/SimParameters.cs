using System;
using System.Xml.Serialization;

namespace PicassoGUI.Util
{
    [Serializable]
    public class SimParameters
    {
        public String Name { get; set; }
        public DateTime simStart { get; set; }
        public DateTime simEnd { get; set; }

        public SimParameters() { }
    }
}
