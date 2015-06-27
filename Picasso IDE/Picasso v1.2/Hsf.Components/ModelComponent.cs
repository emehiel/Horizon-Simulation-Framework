using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Hsf.Utility;
using Hsf.Elements;
using MementoModel;

namespace Hsf.Components
{
    public class ModelComponent : HsfComponent, IMCloneable
    {
        public LuaElement Lua;
        public ScheduleEvaluatorElement ScheduleEvaluator;
        public List<AssetElement> Assets;

        public ModelComponent()
        {
            ElementType = "MODEL";
            Lua = new LuaElement();
            ScheduleEvaluator = new ScheduleEvaluatorElement();
            Assets = new List<AssetElement>();
        }

        public override string ToXml()
        {
            // Serializes model to xml string
            string toReturn = "<" + ElementType + ">";
            toReturn += Lua.ToXml();
            toReturn += ScheduleEvaluator.ToXml();
            foreach (AssetElement a in Assets)
            {
                toReturn += a.ToXml();
            }
            toReturn += "</" + ElementType + ">";
            return toReturn;
        }

        public override void FromXml(string source)
        {
            // Deserializes model from xml string
            XqlParser parser = new XqlParser();
            parser.LoadData(source);

            // Start with Lua
            List<Hashtable> results = parser.Query("SELECT _outerXml FROM " + Lua.ElementType);
            if (results.Count == 0)
            {
                // LUA tages are optional; if there are no tags present, scripting is disabled and no scripts are specified
                //throw new HsfComponentException("Unable to parse " + ElementType + " from xml");
                Lua.IsScriptingEnabled = false;
                Lua.Files.Clear();
            }
            else
            {
                Lua.FromXml(results[0]["_outerXml"].ToString());
            }
            
            // Next, schedule evaluator
            results = parser.Query("SELECT _outerXml FROM " + ScheduleEvaluator.ElementType);
            if (results.Count == 0)
            {
                throw new HsfComponentException("Unable to parse " + ElementType + " from xml");
            }
            ScheduleEvaluator.FromXml(results[0]["_outerXml"].ToString());

            // Lastly, assets
            AssetElement blankAsset = new AssetElement();
            Assets.Clear();
            results = parser.Query("SELECT _outerXml FROM " + blankAsset.ElementType);
            foreach (Hashtable ht in results)
            {
                AssetElement newAsset = new AssetElement();
                newAsset.FromXml(ht["_outerXml"].ToString());
                Assets.Add(newAsset);
            }
        }

        public object Clone()
        {
            // Returns a cloned copy of this object (deep)
            ModelComponent newModel = new ModelComponent();
            newModel.FromXml(ToXml());
            return newModel;
        }

        public void Clone(object value)
        {
            // Copies target object into this object (deep)
            FromXml(((ModelComponent)value).ToXml());
        }
    }
}
