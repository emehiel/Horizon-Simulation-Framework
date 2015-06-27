using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.ComponentModel;
using System.Xml.Serialization;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Text.RegularExpressions;
using MementoModel;

namespace PicassoGUI.Components
{
    public enum PositionType
    { 
        StaticLLA,
        StaticECI,
        PredeterminedLLA,
        PredeterminedECI,
        DynamicLLA,
        DynamicECI
    }

    public enum EomsType
    {
        orbital_EOMS,
        scripted
    }

    [XmlType("EOMS")]
    public class EquationsOfMotion : PicassoNode, IMCloneable
    {
        [XmlAttribute("EOMSType")]
        public EomsType Type;

        [XmlAttribute("initFcn")]
        public string InitialFunction;

        [XmlAttribute("evalFcn")]
        public string EvaluationFunction;

        public EquationsOfMotion()
        {
            Type = EomsType.scripted;
            InitialFunction = "print";
            EvaluationFunction = "print";
        }

        public object Clone()
        {
            // Returns a deep (value-wise) copy of the object
            EquationsOfMotion copy = new EquationsOfMotion();
            copy.EvaluationFunction = EvaluationFunction;
            copy.InitialFunction = InitialFunction;
            copy.Type = Type;
            return copy;
        }

        public void Clone(object value)
        {
            // Writes a deep (value-wise) copy onto the object from value
            if (value.GetType() == typeof(EquationsOfMotion))
            {
                EquationsOfMotion v = (EquationsOfMotion)value;
                EvaluationFunction = v.EvaluationFunction;
                InitialFunction = v.InitialFunction;
                Type = v.Type;
            }
        }
    }

    [XmlType("POSITION")]
    public class Position : PicassoNode, IMCloneable
    {
        [XmlAttribute("PositionType")]
        public PositionType Type;

        [XmlAttribute("ICs")]
        public List<double> InitialConditions;

        public EquationsOfMotion Eoms;

        public Position()
        {
            Type = PositionType.StaticLLA;
            double[] initValues = { 0.0, 0.0, 0.0 };
            InitialConditions = new List<double>(initValues);
        }

        public Position(double latitude, double longitude)
        {
            Type = PositionType.StaticLLA;
            double[] initValues = { longitude, latitude, 0.0 };
            InitialConditions = new List<double>(initValues);
        }

        public Position(XmlElement el)
        {
            // Attempts to load a position from a "POSITION" xml element
            switch (el.GetAttribute("PositionType"))
            {
                case "StaticECI":
                    Type = PositionType.StaticECI;
                    break;
                case "DynamicECI":
                    Type = PositionType.DynamicECI;
                    break;
                case "DynamicLLA":
                    Type = PositionType.DynamicLLA;
                    break;
                case "PredeterminedECI":
                    Type = PositionType.PredeterminedECI;
                    break;
                case "PredeterminedLLA":
                    Type = PositionType.PredeterminedLLA;
                    break;
                default:
                    Type = PositionType.StaticLLA;
                    break;
            }

            Match match = Regex.Match(el.GetAttribute("ICs"), "{([0-9.,\\s-]+)}");
            string[] splitter = { "," };
            string[] values = match.Groups[1].ToString().Split(splitter, StringSplitOptions.RemoveEmptyEntries);
            InitialConditions = new List<double>();
            foreach (string s in values) {
                InitialConditions.Add(Convert.ToDouble(s));
            }
        }

        public override string ToString()
        {
            return "{" + InitialConditions[0] + ", " + InitialConditions[1] + ", " + InitialConditions[2] + "}";
        }

        public object Clone()
        {
            // Returns a deep (value-wise) copy of the object
            Position copy = new Position();
            copy.Eoms = (EquationsOfMotion)Eoms.Clone();
            copy.InitialConditions = InitialConditions;
            copy.Type = Type;
            return copy;
        }

        public void Clone(object value)
        {
            // Writes a deep (value-wise) copy onto the object from value
            if (value.GetType() == typeof(Position))
            {
                Position v = (Position)value;
                Eoms.Clone(v.Eoms);
                InitialConditions = v.InitialConditions;
                Type = v.Type;
            }
        }
    }
}
