using System;
//using System.Collections.Generic;
//using System.Linq;
//using System.Text;
using System.Xml;
using System.Xml.Serialization;
using System.IO;
using System.ComponentModel;
using PicassoGUI.Util;

namespace PicassoGUI.Controls
{
    [Serializable]
    public class Dependency
    {
        [CategoryAttribute("Dependency Properties"),
        DisplayName("Dependency Name"),
        DescriptionAttribute("The Name of the the Dependency Between Subsystems")]
        public String Name { get; set; }

        [CategoryAttribute("Dependency Properties")]
        public String InterperterName { get; set; }

        [CategoryAttribute("Dependency Properties")]
        public String CustomLogicFile { get; set; }

        [CategoryAttribute("Dependency Properties")]
        public String CustomLogicFileDirectory { get; set; }
        
        private bool scripted;
        [CategoryAttribute("Dependency Properties"),
        DescriptionAttribute("A boolean which sets the scripting for the Dependency")]
        public bool Scripted
        {
            get { return this.scripted; }
            set
            {
                this.scripted = value;
                if (value)
                    this.HasFunction = value;
            }
        }

        private bool hasFunction;
        [CategoryAttribute("Dependency Properties")]
        public bool HasFunction
        {
            get { return this.hasFunction; }
            set
            {
                if (this.scripted)
                    this.hasFunction = true;
                else
                    this.hasFunction = value;
            }
        }
        
        [CategoryAttribute("Output Properties"),
        DisplayName("Output Type"),
        DescriptionAttribute("The variable typed being pased from the Output Subsystem")]
        public StateVarType OutputType { get; set; }

        [CategoryAttribute("Output Properties"),
        DisplayName("Output Name"),
        DescriptionAttribute("The name of the Output Subsystem")]
        public String OutputSubsystemName { get { return this.Output.Name; } }

        [CategoryAttribute("Output Properties"),
        DisplayName("Output Asset Name"),
        DescriptionAttribute("The name of the Output Subsystem Asset")]
        public String OutputAssetName
        {
            get
            {
                if (this.Output.GetType() == typeof(Subsystem))
                    return ((SubsystemProperties)this.Output.Properties).Asset.Name;
                else
                    return ((DependencyCollectorProperties)this.Output.Properties).Asset.Name;
            }
        }

        [CategoryAttribute("Input Properties"),
        DisplayName("Input Type"),
        DescriptionAttribute("The variable typed being pased to the Input Subsystem")]
        public StateVarType InputType { get; set; }

        [CategoryAttribute("Input Properties"),
        DisplayName("Input Name"),
        DescriptionAttribute("The name of the Input Subsystem")]
        public String InputSubsystemName { get { return this.Input.Name; } }

        [CategoryAttribute("Input Properties"),
        DisplayName("Input Asset Name"),
        DescriptionAttribute("The name of the Input Subsystem Asset")]        
        public String InputAssetName
        {
            get
            {
                if (this.Input.GetType() == typeof(Subsystem))
                    return ((SubsystemProperties)this.Input.Properties).Asset.Name;
                else
                    return ((DependencyCollectorProperties)this.Input.Properties).Asset.Name;
            }
        }

        [Browsable(false)]
        public LUAScript CustomDependency { get; set; }   
        [XmlIgnore, Browsable(false)]
        public DesignItem Output { get; set; }
        [XmlIgnore, Browsable(false)]
        public DesignItem Input { get; set; }

        public Dependency()
        {
            InitializeComponent();
            this.Name = "New Dependency";
        }

        public Dependency(String name)
        {
            InitializeComponent();
            this.Name = name;
        }
        public override string ToString()
        {
            return this.Name;
        }

        private void InitializeComponent()
        {
            this.scripted = false;
            this.hasFunction = false;
            this.CustomDependency = new LUAScript();
        }
    }
}