using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;

namespace Hsf.Scripting
{
    public class ScriptedFunction
    {
        private string _innerSource;
        public bool IsRequired;
        public List<string> Arguments;
        public string _name;

        public string Name { get; set; }

        [Browsable(false)]
        public string InnerSource
        {
            get
            {
                return _innerSource;
            }
        }

        [Browsable(false)]
        public string Header
        {
            get
            {
                return Name + "(" + string.Join(", ", Arguments.ToArray()) + ")";
            }
        }

        [Browsable(false)]
        public string OuterSource
        {
            get
            {
                return "function " + Header + "\n" + InnerSource + "end";
            }
        }

        public ScriptedFunction()
        {
            Name = "Unnamed Function";
            Arguments = new List<string>();
            _innerSource = "";
            IsRequired = false;
        }
    }
}
