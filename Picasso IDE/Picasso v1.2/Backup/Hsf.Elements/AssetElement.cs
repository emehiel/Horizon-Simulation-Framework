using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Hsf.Utility;
using MementoModel;

namespace Hsf.Elements
{
    public class AssetElement : HsfElement, IMCloneable
    {
        public string Name;
        public Int64 Id;
        public PositionElement Position;
        public List<SubsystemElement> Subsystems;
        public List<ConstraintElement> Constraints;

        public AssetElement()
        {
            ElementType = "ASSET";
            Name = "Unknown Asset";
            Id = 0;
            Position = new PositionElement();
            Subsystems = new List<SubsystemElement>();
            Constraints = new List<ConstraintElement>();
        }

        public override string ToXml()
        {
            // Serializes asset to xml string
            string toReturn = "<" + ElementType + " name=\"" + Name + "\" assetId=\"" + Id + "\">";
            toReturn += Position.ToXml();
            foreach (SubsystemElement sub in Subsystems)
            {
                toReturn += sub.ToXml();
            }
            foreach (ConstraintElement cons in Constraints)
            {
                toReturn += cons.ToXml();
            }
            toReturn += "</" + ElementType + ">";
            return toReturn;
        }

        public override void FromXml(string source)
        {
            // Deserialize asset from xml string
            XqlParser parser = new XqlParser();
            parser.LoadData(source);
            List<Hashtable> results = parser.Query("SELECT _innerXml, name, assetId FROM " + ElementType);
            if (results.Count == 0)
            {
                throw new HsfElementException("Unable to parse " + ElementType + " from xml");
            }

            // Load string values
            Name = results[0]["name"].ToString();
            if (Name == "") { Name = "Unknown Asset"; }
            try
            {
                Id = Convert.ToInt64(results[0]["assetId"]);
            }
            catch (FormatException)
            {
                Id = 0;
            }
            
            // Load subsystems
            parser.LoadData(results[0]["_innerXml"].ToString());
            SubsystemElement sampleSub = new SubsystemElement();
            results = parser.Query("SELECT _outerXml FROM " + sampleSub.ElementType);
            foreach (Hashtable ht in results)
            {
                SubsystemElement newSub = new SubsystemElement();
                newSub.FromXml(ht["_outerXml"].ToString());
                Subsystems.Add(newSub);
            }

            // Load constraints
            ConstraintElement sampleCons = new ConstraintElement();
            results = parser.Query("SELECT _outerXml FROM " + sampleCons.ElementType);
            foreach (Hashtable ht in results)
            {
                ConstraintElement newCons = new ConstraintElement();
                newCons.FromXml(ht["_outerXml"].ToString());
                Constraints.Add(newCons);
            }

            // Load position
            results = parser.Query("SELECT _outerXml FROM " + Position.ElementType);
            Position.FromXml(results[0]["_outerXml"].ToString());
        }

        public object Clone()
        {
            // Returns a deep (value-wise) copy of the object
            AssetElement clone = new AssetElement();
            clone.FromXml(ToXml());
            return clone;
        }

        public void Clone(object value)
        {
            // Writes a deep (value-wise) copy onto the object from value
            FromXml(((AssetElement)value).ToXml());
        }
    }
}
