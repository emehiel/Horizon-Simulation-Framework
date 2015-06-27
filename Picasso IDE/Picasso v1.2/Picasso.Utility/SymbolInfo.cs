using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using Hsf.Scripting;

namespace Picasso.Utility
{
    public class SymbolInfo
    {
        public LuaSymbolType Type;
        public Bitmap Icon;
        public Color Shade;
        public int Ndx;

        public SymbolInfo()
        {
            Type = LuaSymbolType.None;
            Icon = null;
            Shade = Color.Black; // Default null to gray
            Ndx = -1;
        }
    }
}
