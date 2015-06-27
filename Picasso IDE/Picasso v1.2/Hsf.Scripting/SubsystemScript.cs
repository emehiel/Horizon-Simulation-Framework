using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Hsf.Elements;

namespace Hsf.Scripting
{
    public class SubsystemScript
    {
        private long _id;
        public string Name { get; set; }
        public List<StateVariable> States;
        public List<SubsystemParameter> Parameters;
        public List<ScriptedFunction> Functions;

        public long Id
        {
            get { return _id; }
        }

        public SubsystemScript()
        {
            Name = "Unnamed Subsystem";
            _id = -1;

            // Set up blank sets of states, parameters, functions
            States = new List<StateVariable>();
            Parameters = new List<SubsystemParameter>();
            Functions = new List<ScriptedFunction>();

            // Add required functions
            ScriptedFunction canPerform = new ScriptedFunction();
            canPerform.Name = "canPerform";
            Functions.Add(canPerform);
            ScriptedFunction canExtend = new ScriptedFunction();
            canExtend.Name = "canExtend";
            Functions.Add(canExtend);
        }

        public void LoadFromElements(SubsystemElement element)
        {
            // Refreshes subsystem information from element
            Name = element.Name;
            _id = element.Id;

            States.Clear();
            foreach (IcElement ic in element.Ics)
            {
                StateVariable newVariable = new StateVariable();
                newVariable.Type = ic.Type;
                newVariable.Key = ic.Key;
                newVariable.Value = ic.Value;
            }

            Parameters.Clear();
            foreach (object key in element.Attributes.Keys)
            {
                SubsystemParameter newParam = new SubsystemParameter();
                newParam.Name = key.ToString();
                newParam.Value = element.Attributes[key].ToString();
            }

            Functions.Clear();
            ScriptedFunction canPerform = new ScriptedFunction();
            canPerform.Name = element.CanPerformLuaFcn;
            canPerform.IsRequired = true;
            Functions.Add(canPerform);
            ScriptedFunction canExtend = new ScriptedFunction();
            canExtend.Name = element.CanExtendLuaFcn;
            canExtend.IsRequired = true;
            Functions.Add(canExtend);
            ScriptedFunction init = new ScriptedFunction();
            init.Name = element.InitLuaFcn;
            init.IsRequired = true;
            Functions.Add(init);
        }
    }
}
