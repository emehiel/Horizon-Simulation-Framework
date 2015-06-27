using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Xml;
using System.Xml.Serialization;
using AssetPosition;

namespace PicassoGUI.Util
{
    public class Asset
    {
        public String Name { get; set; }
        public Guid AssetID { get; set; }
        public Color Color { get; set; }
        public OrbitalPropagator Position { get; set; }

        public COEs COEs
        {
            get { return this.Position.COEs; }
            set { this.Position.COEs = value; }
        }
        public Asset()
        {
            this.Name = "New Asset";
            this.Position = new OrbitalPropagator();
            this.Color = Color.Blue;
        }

        public Asset(String assetName)
        {
            this.Name = assetName;
            this.Position = new OrbitalPropagator(this.Name);
            this.Color = Color.Blue;
        }

        public override string ToString()
        {
            return this.Name;
        }

        public override bool Equals(object obj)
        {
            return (this.AssetID == ((Asset)obj).AssetID);
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }
    }
}
