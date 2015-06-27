using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using MementoModel;
using Hsf.Utility;

namespace Hsf.Components
{
    public class EnvironmentComponent : HsfComponent, IMCloneable
    {
        public bool IsSunVecConstant;

        public EnvironmentComponent()
        {
            ElementType = "ENVIRONMENT";
            IsSunVecConstant = true;
        }

        public override string ToXml()
        {
            // Converts component into xml string
            string toReturn = "<" + ElementType + "> <SUN isSunVecConstant=\"";
            toReturn += IsSunVecConstant ? "True" : "False";
            toReturn += "\"></SUN></" + ElementType + ">";
            return toReturn;
        }

        public override void FromXml(string source)
        {
            // Loads component from raw xml string
            XqlParser parser = new XqlParser();
            parser.LoadData(source);
            List<Hashtable> results = parser.Query("SELECT isSunVecConstant FROM SUN");
            if (results.Count == 0)
            {
                throw new HsfComponentException("Unable to parse " + ElementType + " from file");
            }

            // Write values
            IsSunVecConstant = results[0]["isSunVecConstant"].ToString() == "True";
        }

        public object Clone()
        {
            // Returns a deep (value-wise) copy of the object
            EnvironmentComponent clone = new EnvironmentComponent();
            clone.FromXml(ToXml());
            return clone;
        }

        public void Clone(object value)
        {
            // Writes a deep (value-wise) copy onto the object from value
            FromXml(((EnvironmentComponent)value).ToXml());
        }
    }
}
