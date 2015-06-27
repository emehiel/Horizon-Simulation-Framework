using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Hsf.Elements;
using Hsf.Utility;
using MementoModel;

namespace Hsf.Components
{
    public class TargetdeckComponent : HsfComponent, IMCloneable
    {
        public List<TargetElement> Targets;

        public TargetdeckComponent()
        {
            ElementType = "TARGETDECK";
            Targets = new List<TargetElement>();
        }

        public override string ToXml()
        {
            // Writes targetdeck and children to string
            string toReturn = "<" + ElementType + ">";
            foreach (TargetElement t in Targets)
            {
                toReturn += t.ToXml();
            }
            toReturn += "</" + ElementType + ">";
            return toReturn;
        }

        public override void FromXml(string source)
        {
            // Loads targetdeck and children from given raw string
            XqlParser parser = new XqlParser();
            parser.LoadData(source);
            List<Hashtable> results = parser.Query("SELECT _outerXml FROM TARGET");
            foreach (Hashtable ht in results)
            {
                TargetElement newTarget = new TargetElement();
                newTarget.FromXml(ht["_outerXml"].ToString());
                Targets.Add(newTarget);
            }
        }

        public object Clone()
        {
            // Returns a deep (value-wise) copy of the object
            TargetdeckComponent clone = new TargetdeckComponent();
            clone.FromXml(ToXml());
            return clone;
        }

        public void Clone(object value)
        {
            // Writes a deep (value-wise) copy onto the object from value
            FromXml(((TargetdeckComponent)value).ToXml());
        }
    }
}
