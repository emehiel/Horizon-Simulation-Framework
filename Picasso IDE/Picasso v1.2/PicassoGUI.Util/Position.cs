using System;
using System.ComponentModel;
using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;
using Hsf.Utility;

namespace PicassoGUI.Util
{
    [Serializable]
    // The TypeConverter attribute applied to the Position class.
    [TypeConverterAttribute(typeof(PositionConverter)),
    DescriptionAttribute("Expand to see the Target Position Details.")]
    public class Position
    {
        public PositionType Type { get; set; }

        private List<Double> initialConditions;
        public List<Double> InitialConditions
        {
            get
            {
                if (this.initialConditions == null)
                    this.initialConditions = new List<Double>(3);
                return this.initialConditions;
            }
            set { this.initialConditions = value; }
        }

        public Position()
        {
            this.InitialConditions = new List<Double>(3) { 0, 0, 0 };
        }

        public Position(XmlNode positionNode)
        {
            this.InitialConditions = new List<Double>(3) { 0, 0, 0 };
            XmlNamedNodeMap mapAttributes = positionNode.Attributes;
            //Add the attributes to the TreeView
            foreach (XmlNode xnodAttribute in mapAttributes)
            {
                // We're going to find nodes by name here people!
                if (xnodAttribute.Name == "TaskType")
                    this.Type = (PositionType)Enum.Parse(typeof(PositionType), xnodAttribute.Value);
                else if (xnodAttribute.Name == "ICs")
                {
                    String temp = xnodAttribute.Value.Split(new char[] { '{', '}' })[1];
                    String[] vals = temp.Split(',');
                    this.InitialConditions[1] = Convert.ToDouble(vals[0]);
                    this.InitialConditions[0] = Convert.ToDouble(vals[1]);
                    this.InitialConditions[2] = Convert.ToDouble(vals[2]);
                }
            }
        }

        public override string ToString()
        {
            //String temp = "";
            //foreach (Double d in this.InitialConditions)
            //    temp += d.ToString() + ", ";

            //temp = temp.Substring(0, temp.Length - 2);
            //return temp;
            return this.InitialConditions[1].ToString() + ", " + this.InitialConditions[0].ToString() + ", " + this.InitialConditions[2].ToString();
        }

    }
}
