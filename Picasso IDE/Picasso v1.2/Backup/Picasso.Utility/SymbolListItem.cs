using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace Picasso.Utility
{
    public class SymbolListItem
    {
        private string _name;
        private int _ndx;

        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }

        public int Ndx
        {
            get { return _ndx; }
            set { _ndx = value; }
        }

        public SymbolListItem()
        {
            _name = "";
            _ndx = -1;
        }

        public override string ToString()
        {
            return _name;
        }
    }
}
