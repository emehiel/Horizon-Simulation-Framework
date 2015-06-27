using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Hsf.Utility;
using MementoModel;

namespace Hsf.Elements
{
    public class TargetElement : HsfElement, IMCloneable
    {
        private string _targetName;
        public TargetType TargetType;
        public TaskType TaskType;
        public double MaxTimes;
        public double Value;
        public PositionElement Position;

        public string TargetName
        {
            get { return _targetName; }
            set { _targetName = value; }
        }

        public TargetElement()
        {
            ElementType = "TARGET";
            TargetName = "Unknown Target";
            TargetType = TargetType.UnknownTarget;
            TaskType = TaskType.NoTask;
            MaxTimes = 1000.0;
            Value = 1.0;
            Position = new PositionElement();
        }

        public override string ToXml()
        {
            // Writes this element into an xml string
            string toReturn = "<" + ElementType + " TargetName=\"" + TargetName + "\" TargetType=\"" + Enum.GetName(typeof(TargetType), TargetType) + "\"";
            toReturn += " TaskType=\"" + Enum.GetName(typeof(TaskType), TaskType) + "\" MaxTimes=\"" + MaxTimes + "\" Value=\"" + Value + "\">";
            toReturn += Position.ToXml();
            toReturn += "</" + ElementType + ">";
            return toReturn;
        }

        public override void FromXml(string source)
        {
            // Attempts to load this element and children from given xml content string
            XqlParser parser = new XqlParser();
            parser.LoadData(source);
            List<Hashtable> results = parser.Query("SELECT TargetName, TargetType, TaskType, MaxTimes, Value, _innerXml FROM " + ElementType);
            if (results.Count == 0)
            {
                throw new HsfElementException("Unable to parse " + ElementType + " from xml");
            }

            // Load values
            TargetName = results[0]["TargetName"].ToString();
            MaxTimes = Convert.ToDouble(results[0]["MaxTimes"]);
            Value = Convert.ToDouble(results[0]["Value"]);

            // Load enums
            Array posValues = Enum.GetValues(typeof(TargetType));
            foreach (TargetType v in posValues)
            {
                if (Enum.GetName(typeof(TargetType), v) == results[0]["TargetType"].ToString())
                {
                    TargetType = v;
                }
            }
            posValues = Enum.GetValues(typeof(TaskType));
            foreach (TaskType v in posValues)
            {
                if (Enum.GetName(typeof(TaskType), v) == results[0]["TaskType"].ToString())
                {
                    TaskType = v;
                }
            }

            // Load children
            Position.FromXml(results[0]["_innerXml"].ToString());
        }

        public object Clone()
        {
            // Returns a deep (value-wise) copy of the object
            TargetElement copy = new TargetElement();
            copy.FromXml(ToXml());
            return copy;
        }

        public void Clone(object value)
        {
            // Writes a deep (value-wise) copy onto the object from value
            TargetElement copy = (TargetElement)value;
            FromXml(copy.ToXml());
        }
    }
}
