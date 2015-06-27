using System;
using System.ComponentModel;
using System.Globalization;
using Hsf.Utility;

namespace PicassoGUI.Util
{
    public class PositionConverter : ExpandableObjectConverter
    {
        public override bool CanConvertTo(ITypeDescriptorContext context, System.Type destinationType)
        {
            if (destinationType == typeof(Position))
                return true;

            return base.CanConvertTo(context, destinationType);
        }

        public override object ConvertTo(ITypeDescriptorContext context, CultureInfo culture, object value, System.Type destinationType)
        {
            if (destinationType == typeof(System.String) &&
                 value is Position)
            {

                Position p = (Position)value;

                return p.Type.ToString() + ": " + p.ToString();
            }
            return base.ConvertTo(context, culture, value, destinationType);
        }

        public override bool CanConvertFrom(ITypeDescriptorContext context, System.Type sourceType)
        {
            if (sourceType == typeof(string))
                return true;

            return base.CanConvertFrom(context, sourceType);
        }

        public override object ConvertFrom(ITypeDescriptorContext context,
                              CultureInfo culture, object value)
        {
            if (value is string)
            {
                try
                {
                    String s = (String)value;
                    int colon = s.IndexOf(':');
                    String[] vals = (s.Substring(colon + 1).Trim()).Split(',');

                    if (colon != -1)
                    {
                        Position p = new Position();
                        String type = s.Substring(0, colon);
                        p.Type = (PositionType)Enum.Parse(typeof(PositionType), type);
                        p.InitialConditions[0] = Convert.ToDouble(vals[0]);
                        p.InitialConditions[1] = Convert.ToDouble(vals[1]);
                        p.InitialConditions[2] = Convert.ToDouble(vals[2]);

                        return p;
                    }
                }
                catch
                {
                    throw new ArgumentException(
                        "Can not convert '" + (string)value +
                                           "' to type Position");
                }
            }
            return base.ConvertFrom(context, culture, value);
        }
    }
}