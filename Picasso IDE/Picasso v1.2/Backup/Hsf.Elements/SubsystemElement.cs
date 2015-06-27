using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Hsf.Utility;

namespace Hsf.Elements
{
    public class SubsystemElement : HsfElement
    {
        public Int64 Id;
        public SubsystemType Type;
        public Hashtable Attributes;
        public List<IcElement> Ics;
        public List<DependencyElement> Dependencies;
        public DependencyFunctionElement DependencyFunction;
        private string _name;
        private string _initLuaFcn;
        private string _canPerformLuaFcn;
        private string _canExtendLuaFcn;

        public string Name
        {
            get { return _name; }
            set
            {
                if (_name == "Unknown Subsystem")
                {
                    _name = value;
                    AutoGenerateFunctionName();
                }
                else
                {
                    _name = value;
                }
            }
        }

        public string InitLuaFcn
        {
            get { if (Type == SubsystemType.Scripted) return _initLuaFcn; else return ""; }
            set { if (Type == SubsystemType.Scripted) _initLuaFcn = value; }
        }

        public string CanPerformLuaFcn
        {
            get { if (Type == SubsystemType.Scripted) return _canPerformLuaFcn; else return ""; }
            set { if (Type == SubsystemType.Scripted) _canPerformLuaFcn = value; }
        }

        public string CanExtendLuaFcn
        {
            get { if (Type == SubsystemType.Scripted) return _canExtendLuaFcn; else return ""; }
            set { if (Type == SubsystemType.Scripted) _canExtendLuaFcn = value; }
        }

        public SubsystemElement()
        {
            ElementType = "SUBSYSTEM";
            Id = 0;
            Name = "Unknown Subsystem";
            Attributes = new Hashtable();
            Ics = new List<IcElement>();
            Dependencies = new List<DependencyElement>();
            DependencyFunction = new DependencyFunctionElement();
            Type = SubsystemType.Scripted;
            AutoGenerateFunctionName();
        }

        public void AutoGenerateFunctionName()
        {
            _initLuaFcn = Name.Replace(" ", "") + "_init";
            _canPerformLuaFcn = Name.Replace(" ", "") + "_canPerform";
            _canExtendLuaFcn = Name.Replace(" ", "") + "_canExtend";
        }

        public override string ToXml()
        {
            // Serializes subsystem to xml string
            string toReturn = "<" + ElementType + " SubId=\"" + Id + "\" Name=\"" + Name + "\" Type=\"" + Enum.GetName(typeof(SubsystemType), Type) + "\"";
            if (Type == SubsystemType.Scripted)
            {
                toReturn += " initLUAFcn=\"" + _initLuaFcn + "\" canPerformLUAFcn=\"" + _canPerformLuaFcn + "\" canExtendLUAFcn=\"" + _canExtendLuaFcn + "\"";
            }
            else
            {
                foreach (object key in Attributes.Keys)
                {
                    toReturn += " " + key + "=\"" + Attributes[key] + "\"";
                }
            }
            toReturn += ">";
            
            // Serialize child elements
            foreach (IcElement ic in Ics)
            {
                toReturn += ic.ToXml();
            }
            foreach (DependencyElement dep in Dependencies) {
                toReturn += dep.ToXml();
            }
            if (DependencyFunction.Enabled)
                toReturn += DependencyFunction.ToXml();

            toReturn += "</" + ElementType + ">";
            return toReturn;
        }

        public override void FromXml(string source)
        {
            // Deserialize subsystem object from xml string
            XqlParser parser = new XqlParser();
            parser.LoadData(source);
            List<Hashtable> results = parser.Query("SELECT Type, SubId, Name, _innerXml FROM " + ElementType);
            if (results.Count == 0)
            {
                throw new HsfElementException("Unable to parse " + ElementType + " from xml");
            }

            // Load basic fields and type enum
            string innerXml = results[0]["_innerXml"].ToString();
            Name = results[0]["Name"].ToString();
            Id = Convert.ToInt64(results[0]["SubId"]);
            Array posValues = Enum.GetValues(typeof(SubsystemType));
            foreach (SubsystemType v in posValues)
            {
                if (Enum.GetName(typeof(SubsystemType), v) == results[0]["Type"].ToString())
                {
                    Type = v;
                }
            }

            // Load others based on type
            if (Type == SubsystemType.Scripted)
            {
                // Set number of fields that must be loaded for scripted subsystems
                results = parser.Query("SELECT initLUAFcn, canPerformLUAFcn, canExtendLUAFcn FROM " + ElementType);
                if (results.Count == 0)
                {
                    throw new HsfElementException("Unable to parse " + ElementType + " from xml");
                }
                _initLuaFcn = results[0]["initLUAFcn"].ToString();
                _canPerformLuaFcn = results[0]["canPerformLUAFcn"].ToString();
                _canExtendLuaFcn = results[0]["canExtendLUAFcn"].ToString();
            }
            else
            {
                // Load ALL possible fields for non-scripted subsystem
                results = parser.Query("SELECT * FROM " + ElementType);
                Attributes.Clear();
                foreach (object key in results[0].Keys)
                {
                    Attributes[key] = results[0][key];
                }
            }

            // Load ICs
            parser.LoadData(innerXml);
            IcElement sampleIc = new IcElement();
            results = parser.Query("SELECT _outerXml FROM " + sampleIc.ElementType);
            Ics.Clear();
            foreach (Hashtable ht in results)
            {
                IcElement newIc = new IcElement();
                newIc.FromXml(ht["_outerXml"].ToString());
                Ics.Add(newIc);
            }

            // Load dependencies
            DependencyElement sampleDep = new DependencyElement();
            results = parser.Query("SELECT _outerXml FROM " + sampleDep.ElementType);
            Dependencies.Clear();
            foreach (Hashtable ht in results)
            {
                DependencyElement newDep = new DependencyElement();
                newDep.FromXml(ht["_outerXml"].ToString());
                Dependencies.Add(newDep);
            }

            // Load dependency function
            results = parser.Query("SELECT _outerXml FROM " + DependencyFunction.ElementType);
            if (results.Count > 0)
            {
                DependencyFunction.Enabled = true;
                DependencyFunction.FromXml(results[0]["_outerXml"].ToString());
            }
            else
            {
                DependencyFunction.Enabled = false;
            }
        }
    }
}
