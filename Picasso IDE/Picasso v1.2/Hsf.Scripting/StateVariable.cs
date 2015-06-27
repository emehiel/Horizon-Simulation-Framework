using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using Hsf.Utility;

namespace Hsf.Scripting
{
    public class StateVariable
    {
        [Browsable(false)]
        public string Key { get; set; }

        public HsfType Type { get; set; }

        public string Value { get; set; }

        public string Name
        {
            get { return Key; }
            set { Key = value; }
        }

        public StateVariable()
        {
            Type = HsfType.Bool;
            Key = "Unnamed Variable";
            Value = "false";
        }
    }
}
