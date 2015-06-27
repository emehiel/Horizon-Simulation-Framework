using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Hsf.Utility;

namespace Hsf.Elements
{
    public class ScheduleEvaluatorElement : HsfElement
    {
        public ScheduleEvaluatorType Type;
        private string _evaluateLuaFcn;
        public string EvaulateLuaFcn
        {
            get
            {
                if (Type == ScheduleEvaluatorType.scripted) return _evaluateLuaFcn;
                else return "";
            }
            set
            {
                if (Type == ScheduleEvaluatorType.scripted) _evaluateLuaFcn = value;
            }
        }

        public ScheduleEvaluatorElement() {
            ElementType = "SCHEDULE_EVALUATOR";
        }

        public override string ToXml()
        {
            // Returns string representation of this object in XML
            string toReturn = "<" + ElementType + " type=\"" + Enum.GetName(typeof(ScheduleEvaluatorType), Type) + "\"";
            toReturn += Type == ScheduleEvaluatorType.scripted ? " evaluateLUAFcn=\"" + _evaluateLuaFcn + "\"" : "";
            toReturn += "></" + ElementType + ">";
            return toReturn;
        }

        public override void FromXml(string source)
        {
            // Attempts to load a position from the given xml string
            XqlParser parser = new XqlParser();
            parser.LoadData(source);
            List<Hashtable> results = parser.Query("SELECT type, evaluateLUAFcn FROM " + ElementType);
            if (results.Count == 0)
            {
                throw new HsfElementException("Unable to parse " + ElementType + " from xml");
            }

            // Load enum
            Array posValues = Enum.GetValues(typeof(ScheduleEvaluatorType));
            foreach (ScheduleEvaluatorType v in posValues)
            {
                if (Enum.GetName(typeof(ScheduleEvaluatorType), v) == results[0]["type"].ToString())
                {
                    Type = v;
                }
            }

            // Load LUA function?
            if (Type == ScheduleEvaluatorType.scripted)
            {
                _evaluateLuaFcn = results[0]["evaluateLUAFcn"].ToString();
            }
            else
            {
                _evaluateLuaFcn = "";
            }
        }
    }
}
