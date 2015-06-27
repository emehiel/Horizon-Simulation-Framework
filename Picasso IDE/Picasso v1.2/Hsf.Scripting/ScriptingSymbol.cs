using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Hsf.Scripting
{
    public class ScriptingSymbol
    {
        public LuaSymbolType Type;
        public string Name;
        public bool Enabled;
        public ScriptingSymbol ParentSymbol;

        public ScriptingSymbol()
        {
            Type = LuaSymbolType.None;
            Name = "Unknown";
            Enabled = false;
            ParentSymbol = null;
        }

        public override string ToString()
        {
            return Name;
        }
    }

    public enum LuaSymbolType
    {
        Keyword,
        Class,
        Local,
        Function,
        Global,
        Table,
        None
    }
}
