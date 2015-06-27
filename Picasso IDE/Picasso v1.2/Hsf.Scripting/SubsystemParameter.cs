using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Hsf.Scripting
{
    public class SubsystemParameter
    {
        public string Name { get; set; }
        
        public string Value { get; set; }

        public SubsystemParameter()
        {
            Name = "Unnamed Parameter";
            Value = "1";
        }
    }
}
