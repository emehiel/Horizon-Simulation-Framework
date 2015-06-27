using System;
using System.Drawing;
using System.Runtime.Serialization;

namespace PicassoGUI.Controls
{
    [Serializable]
    public class SubsystemMemento
    {
        //private Rectangle rect;
        // TODO:  Add other properties for momento - need to do dependencyCollector too
        public Rectangle Rect { get; set; }
        /*
        {
            get
            {
                return rect;
            }
            set
            {
                rect = value;
            }
        }
         */
    }
}
