using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Xml.Serialization;
using PicassoGUI.Util;

namespace PicassoGUI.Controls
{
    [Serializable]
    public class SubsystemProperties : DesignItemProperties
    {

        [CategoryAttribute("Subsystem Model Properties"),
        DescriptionAttribute("The Asset to which the Subsystem Model Belongs")]
        public Asset Asset { get; set; }

        [CategoryAttribute("Subsystem Model Properties"),
        DescriptionAttribute("The Primary Author of the Subsystem Model")]
        public String Author { get; set; }
        
        [CategoryAttribute("Subsystem Model Properties"),
        System.ComponentModel.ReadOnly(true), DisplayName("Date Created"),
        DescriptionAttribute("The Date the Subsystem Model was initially created")]
        public DateTime DateCreated { get; set; }

        [CategoryAttribute("Subsystem Model Properties"),
        System.ComponentModel.ReadOnly(true), DisplayName("Date Modified"),
        DescriptionAttribute("The most recent Date the Subsystem Model was modified")]
        public DateTime DateModified { get; set; }

        [CategoryAttribute("Subsystem Scripting"),
        DescriptionAttribute("Determines if the subsystem is scripted")]
        public bool Scripted { get; set; }

        [CategoryAttribute("Subsystem Scripting"), DisplayName("LUA Script File Name"),
        DescriptionAttribute("The custom LUA script file used by this subsystem")]
        public String CustomScriptFileName { get; set; }

        [CategoryAttribute("Subsystem Scripting"), DisplayName("LUA Script Directory"),
        DescriptionAttribute("The directory locaiton of the custom LUA script file used by this subsystem")]
        public String CustomScriptFileDirectory { get; set; }

        [CategoryAttribute("Subsystem Model Properties"),
        ReadOnly(true), DisplayName("Unique ID"),
        DescriptionAttribute("The unique identification number for this subsystem")]
        public int IntID { get { return base.ID.GetHashCode(); } }

        [Browsable(false)]
        public List<StateVariable> States { get; set; }

        public SubsystemProperties()
        {
            this.DateCreated = new DateTime(2008, 7, 13);
            this.DateModified = new DateTime(2008, 7, 13);
            this.Author = "Eric A. Mehiel";
            this.States = new List<StateVariable>();
            this.Asset = new Asset();
            //this.SubsystemID = Guid.NewGuid();
        }
        public SubsystemProperties(Asset myAsset)
        {
            this.DateCreated = new DateTime(2008, 7, 13);
            this.DateModified = new DateTime(2008, 7, 13);
            this.Author = "Eric A. Mehiel";
            this.States = new List<StateVariable>();
            this.Asset = myAsset;
            //this.SubsystemID = Guid.NewGuid();
        }

    }
}
