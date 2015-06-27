using System;
using System.ComponentModel;
using System.Xml.Serialization;
using PicassoGUI.Util;

namespace PicassoGUI.Controls
{
    [Serializable]
    public class DependencyCollectorProperties : DesignItemProperties
    {
        [CategoryAttribute("Dependency Collector Properties"),
        DescriptionAttribute("The Asset to which the Dependency Collector Belongs")]
        public Asset Asset { get; set; }

        [CategoryAttribute("Dependency Collector Properties"),
        DescriptionAttribute("The Primary Author of the Subsystem Model")]
        public String Author { get; set; }

        [CategoryAttribute("Dependency Collector Properties"),
        System.ComponentModel.ReadOnly(true), DisplayName("Date Created"),
        DescriptionAttribute("The Date the Subsystem Model was initially created")]
        public DateTime DateCreated { get; set; }

        [CategoryAttribute("Dependency Collector Properties"),
        System.ComponentModel.ReadOnly(true), DisplayName("Date Modified"),
        DescriptionAttribute("The most recent Date the Subsystem Model was modified")]
        public DateTime DateModified { get; set; }

        //public Guid ID { get { return base.ID; } set { base.ID = value; } }

        public DependencyCollectorProperties()
        {
            this.DateCreated = new DateTime(2008, 7, 13);
            this.DateModified = new DateTime(2008, 7, 13);
            this.Author = "Eric A. Mehiel";
        }
    }
}
