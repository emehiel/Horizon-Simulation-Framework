using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using Horizon.Universe;

namespace Horizon
{
    /**
    * A named location denoting where a task is to be performed.
    * @author Einar Pehrson
    */
    class Target
    {
        /** The name of the target */
        String Name { get; set; }

        /** The type of the target */
        TargetType TargetType { get; set; }

        /** The position of the target */
        DynamicState DynamicState { get; set; }

        /** The value of the target */
        int Value { get; set; }

        // TODO: What should we do with these?
        /*
        int minQualCM;

        int freq_days;
        string CC;
        string WX_Reg;
        */

        String Comment { get; set; }

        public Target(String name, TargetType type, DynamicState dynamicState, int value)
        {
            this.Name = name;
            this.TargetType = type;
            this.DynamicState = dynamicState;
            this.Value = value;
        }

        /**
        * Creates a new target from the xmlNode data
        * @param targetNode the xmlNode which contains the relevant target information
        */
        public Target(XmlNode targetXmlNode)
        {

        }

    }

    public enum TargetType { FACILITY, LOCATION }
}
