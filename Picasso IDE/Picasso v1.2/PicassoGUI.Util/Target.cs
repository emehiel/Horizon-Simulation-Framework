using System;
using System.ComponentModel;
using System.Drawing;
using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;
using Hsf.Utility;

namespace PicassoGUI.Util
{
    [Serializable]
    public class Target
    {
        public String Name { get; set; }
        public Position Position { get; set; }
        public TargetType TargetType { get; set; }
        public TaskType TaskType { get; set; }
        public int MaxTimes { get; set; }
        public double Value { get; set; }

        [XmlIgnore]
        [Browsable(false)]
        public Bitmap Image { get; set; }

        private bool selected = false;
        [XmlIgnore]
        [Browsable(false)]
        public bool Selected
        {
            get { return this.selected; }
            set
            {
                if (this.TargetType == TargetType.LocationTarget)
                {
                    if (value)
                        this.Image = global::PicassoGUI.Util.Properties.Resources.target_selected;
                    else
                        this.Image = global::PicassoGUI.Util.Properties.Resources.target;
                }
                this.Image.MakeTransparent(Color.Blue);
                this.selected = value;
            }
        }

        private List<Bitmap> imageList = new List<Bitmap>();

        public Target()
        {
            this.Initialize();
            this.Image.MakeTransparent(Color.Blue);
        }

        public Target(XmlNode targetNode)
        {
            this.Initialize();

            if (targetNode.HasChildNodes)
                this.Position = new Position(targetNode.ChildNodes[0]);
            else
                this.Position = new Position();

            XmlNamedNodeMap mapAttributes = targetNode.Attributes;
            //Add the attributes to the TreeView
            foreach (XmlNode xnodAttribute in mapAttributes)
            {
                // We're going to find nodes by name here people!
                if (xnodAttribute.Name == "TargetName")
                    this.Name = xnodAttribute.Value;
                else if (xnodAttribute.Name == "MaxTimes")
                    this.MaxTimes = Convert.ToInt16(xnodAttribute.Value);
                else if (xnodAttribute.Name == "Value")
                    this.Value = Convert.ToDouble(xnodAttribute.Value);
                else if (xnodAttribute.Name == "TargetType")
                    this.TargetType = (TargetType)Enum.Parse(typeof(TargetType), xnodAttribute.Value);
                else if (xnodAttribute.Name == "TaskType")
                    this.TaskType = (TaskType)Enum.Parse(typeof(TaskType), xnodAttribute.Value);

                this.Image = this.imageList[(int)this.TargetType];
                this.Image.MakeTransparent(Color.Blue);
            }
        }

        public override string ToString()
        {
            return this.Name;
        }
        
        public String[] ToStringArray()
        {
            return new String[] { Name, Position.ToString(), TargetType.ToString(), TaskType.ToString(), MaxTimes.ToString(), Value.ToString() };
        }
 
        private void Initialize()
        {
            this.Position = new Position();
            this.Image = global::PicassoGUI.Util.Properties.Resources.target;
            this.imageList.Add(global::PicassoGUI.Util.Properties.Resources.target);
            this.imageList.Add(global::PicassoGUI.Util.Properties.Resources.ground);
            this.imageList.Add(global::PicassoGUI.Util.Properties.Resources.target);
            this.imageList.Add(global::PicassoGUI.Util.Properties.Resources.target);
            this.imageList.Add(global::PicassoGUI.Util.Properties.Resources.target);
            this.imageList.Add(global::PicassoGUI.Util.Properties.Resources.target);
            this.imageList.Add(global::PicassoGUI.Util.Properties.Resources.target);
            this.imageList.Add(global::PicassoGUI.Util.Properties.Resources.target);
        }
    }
}