using System;
using System.ComponentModel;
using System.Xml.Serialization;

namespace PicassoGUI.Controls
{
    [Serializable]
    public class DesignItemProperties
    {
        [CategoryAttribute("General Properties"),
        DescriptionAttribute("The Name of the Model Element")]
        public String Name { get; set; }
        
        [Browsable(false)]
        public Guid ID { get; set; }

        public DesignItemProperties() { }
    }
}
