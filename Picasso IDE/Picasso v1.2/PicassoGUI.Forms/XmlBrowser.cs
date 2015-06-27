using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace PicassoGUI.Forms
{
    public partial class XmlBrowser : Form
    {
        public XmlBrowser(string Url, bool showWindow)
        {
            InitializeComponent();
            xmlWindow.Navigate(Url);
            Visible = showWindow;
        }

        public void GoTo(string Url)
        {
            xmlWindow.Navigate(Url);
        }
    }
}
