using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Hsf.Utility;

namespace Hsf.Elements
{
    public class EomsElement : HsfElement
    {
        public EomsType Type;
        private string _initialFunction;
        private string _evaluationFunction;

        public string InitialFunction
        {
            get { if (Type == EomsType.Scripted) return _initialFunction; else return ""; }
            set { if (Type == EomsType.Scripted) _initialFunction = value; }
        }

        public string EvaulationFunction
        {
            get { if (Type == EomsType.Scripted) return _evaluationFunction; else return ""; }
            set { if (Type == EomsType.Scripted) _evaluationFunction = value; }
        }

        public EomsElement()
        {
            ElementType = "EOMS";
            Type = EomsType.None;
            _initialFunction = "";
            _evaluationFunction = "";
        }

        public override string ToXml()
        {
            // Serializes this object to an xml string
            if (Type == EomsType.None) return "";
            string toReturn = "<" + ElementType + " EOMSType=\"" + Enum.GetName(typeof(EomsType), Type) + "\"";
            if (Type == EomsType.Scripted)
            {
                toReturn += " initFcn=\"" + _initialFunction + "\" evalFcn=\"" + _evaluationFunction + "\"";
            }
            toReturn += ">";
            toReturn += "</" + ElementType + ">";
            return toReturn;
        }

        public override void FromXml(string source)
        {
            // Deserializes object from xml string
            XqlParser parser = new XqlParser();
            parser.LoadData(source);
            List<Hashtable> results = parser.Query("SELECT EOMSType, initFcn, evalFcn FROM " + ElementType);
            if (results.Count == 0)
            {
                throw new HsfElementException("Unable to parse " + ElementType + " from xml");
            }

            // Load enum
            Array posValues = Enum.GetValues(typeof(EomsType));
            foreach (EomsType v in posValues)
            {
                if (Enum.GetName(typeof(EomsType), v) == results[0]["EOMSType"].ToString())
                {
                    Type = v;
                }
            }

            // Load function?
            if (Type == EomsType.Scripted)
            {
                _initialFunction = results[0]["initFcn"].ToString();
                _evaluationFunction = results[0]["evalFcn"].ToString();
            }
            else
            {
                _initialFunction = "";
                _evaluationFunction = "";
            }
        }
    }
}
