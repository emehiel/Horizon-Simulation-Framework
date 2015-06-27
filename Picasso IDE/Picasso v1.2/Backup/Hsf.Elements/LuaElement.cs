using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Hsf.Utility;

namespace Hsf.Elements
{
    public class LuaElement : HsfElement
    {
        public bool IsScriptingEnabled;
        private List<LuaFileElement> _files;
        public List<LuaFileElement> Files
        {
            get { if (IsScriptingEnabled) return _files; else return new List<LuaFileElement>(); }
            set { if (IsScriptingEnabled) _files = value; }
        }

        public LuaElement()
        {
            ElementType = "LUA";
            IsScriptingEnabled = false;
            _files = new List<LuaFileElement>();
        }

        public override string ToXml()
        {
            // Serializes element to xml string
            if (!IsScriptingEnabled) return "";
            string toReturn = "<" + ElementType + " enableScripting=\"True\">";
            foreach (LuaFileElement file in _files)
            {
                toReturn += file.ToXml();
            }
            toReturn += "</" + ElementType + ">";
            return toReturn;
        }

        public override void FromXml(string source)
        {
            // Deserializes element and children from xml string
            XqlParser parser = new XqlParser();
            parser.LoadData(source);
            List<Hashtable> results = parser.Query("SELECT enableScripting, _innerXml FROM " + ElementType);
            if (results.Count == 0)
            {
                throw new HsfElementException("Unable to parse " + ElementType + " from xml");
            }

            // Load flag
            if (results[0]["enableScripting"].ToString() == "True")
            {
                IsScriptingEnabled = true;
            }
            else
            {
                IsScriptingEnabled = false;
            }

            // Load children
            Files.Clear();
            LuaFileElement test = new LuaFileElement();
            results = parser.Query("SELECT _outerXml FROM " + test.ElementType);
            foreach (Hashtable ht in results)
            {
                LuaFileElement newFile = new LuaFileElement();
                newFile.FromXml(ht["_outerXml"].ToString());
                Files.Add(newFile);
            }
        }

        public void RemoveScriptByName(string name)
        {
            // Removes the script with name 'name' from the list of files, if present
            LuaFileElement toRemove = null;
            foreach (LuaFileElement file in Files)
            {
                if (file.Src == name)
                {
                    toRemove = file;
                    break;
                }
            }
            if (toRemove != null)
            {
                Files.Remove(toRemove);
            }
        }
    }
}
