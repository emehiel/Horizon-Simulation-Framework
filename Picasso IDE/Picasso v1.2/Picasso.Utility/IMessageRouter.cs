using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Picasso.Utility
{
    public interface IMessageRouter
    {
        void OnDoubleClick(object sender, MouseEventArgs e);
        void OnControlMove(object sender, MouseEventArgs e);
    }
}
