using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Hsf.Elements;
using Hsf.Utility;

namespace Hsf.Components
{
    public class TargetDeck : HsfComponent
    {
        public List<Target> Targets;

        public TargetDeck()
        {
            ElementType = "TARGETDECK";
            Targets = new List<Target>();
        }

        public override void ToFile(string filename)
        {
            // Writes this targetdeck to a string, then a file
            string toWrite = "<" + ElementType + ">";
            foreach (Target t in Targets)
            {
                toWrite += t.ToXml();
            }
            toWrite += "</" + ElementType + ">";

            // Write string to file
            FileStream file = new FileStream(filename, FileMode.Create, FileAccess.Write);
            StreamWriter sw = new StreamWriter(file);
            sw.Write(toWrite);
            sw.Close();
            file.Close();
        }

        public override void FromFile(string filename)
        {
            // Attempts to load a list of targets from given file
            XqlParser parser = new XqlParser(filename);
            List<Hashtable> results = parser.Query("SELECT _outerXml FROM TARGET");
            foreach (Hashtable ht in results)
            {
                Target newTarget = new Target();
                newTarget.FromXml(ht["_outerXml"].ToString());
                Targets.Add(newTarget);
            }
        }
    }
}
