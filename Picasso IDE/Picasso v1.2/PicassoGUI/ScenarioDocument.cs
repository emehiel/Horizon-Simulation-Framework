using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;
using System.Xml;
using System.IO;
using System.Windows.Forms;
using PicassoGUI.Controls;
using PicassoGUI.Util;
using Hsf.Utility;

namespace PicassoGUI
{
    [Serializable]
    public class ScenarioDocument
    {
        public List<DesignItem> DesignItems { get; set; }
        public List<Connector> Connectors { get; set; }
        public ScenarioParameters ScenarioParameters { get; set; }

        public ScenarioDocument()
        {
            this.DesignItems = new List<DesignItem>();
            this.Connectors = new List<Connector>();
            this.ScenarioParameters = new ScenarioParameters();
        }
    }
}
