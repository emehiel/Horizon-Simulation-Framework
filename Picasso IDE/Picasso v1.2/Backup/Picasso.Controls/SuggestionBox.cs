using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Hsf.Utility;

namespace Picasso.Controls
{
    public partial class SuggestionBox : RichTextBox
    {
        private string _functionName;
        private HsfType _functionType;
        private List<string> _parameterNames;
        private List<HsfType> _parameterTypes;
        private int _currentParameter;

        public int CurrentParameter
        {
            get { return _currentParameter; }
            set
            {
                if (value < 0 || value > _parameterNames.Count)
                {
                    // Index out of bounds; un-bold everything
                    Select(0, Text.Length);
                    Font currentFont = SelectionFont;
                    FontStyle boldFont = FontStyle.Bold;
                    SelectionFont = new Font(currentFont.FontFamily, currentFont.Size, boldFont);
                    SelectionStart = 0;
                    return;
                }
                else
                {
                    // Format rich text function header (with current parameter bolded); reconstruct header to determine selection
                    string header = typeToString(_functionType) + " " + _functionName + "(";
                    int startNdx = 0; int endNdx = 0;
                    for (int i = 0; i < _parameterNames.Count; i++)
                    {
                        if (i == value)
                        {
                            startNdx = header.Length;
                        }
                        header += typeToString(_parameterTypes[i]) + " " + _parameterNames[i];
                        if (i == value)
                        {
                            endNdx = header.Length;
                        }
                        if (i != _parameterNames.Count - 1)
                        {
                            header += ", ";
                        }
                    }
                    Select(startNdx, endNdx - startNdx);
                    Font currentFont = SelectionFont;
                    FontStyle boldFont = FontStyle.Bold;
                    SelectionFont = new Font(currentFont.FontFamily, currentFont.Size, boldFont);
                    SelectionStart = 0;
                }
            }
        }

        public string RawHeader
        {
            get
            {
                string name = typeToString(_functionType) + " " + _functionName;
                List<string> parameters = new List<string>();
                for (int i = 0; i < _parameterNames.Count; i++)
                {
                    parameters.Add(typeToString(_parameterTypes[i]) + " " + _parameterNames[i]);
                }
                return name + "(" + String.Join(", ", parameters.ToArray()) + ")";
            }
        }

        public SuggestionBox()
        {
            InitializeComponent();
            _parameterNames = new List<string>();
            _parameterTypes = new List<HsfType>();
            _currentParameter = -1;
            SelectionCharOffset = 1;
        }

        public void LoadFunction(string functionHeader)
        {
            // Parses and loads function from given header
            // Sample: Double getValueAtTime(String SubsystemKey, Double time)
            // ASSUMES CORRECT FORMAT
            string[] parenSplitter = {"("};
            string[] spaceSplitter = {" "};
            string[] paramSplitter = {", "};
            string[] header = functionHeader.Split(parenSplitter, StringSplitOptions.RemoveEmptyEntries);

            // Parse type and name from first two strings
            string[] title = header[0].Split(spaceSplitter, StringSplitOptions.RemoveEmptyEntries);
            _functionType = stringToType(title[0]);
            _functionName = title[1];

            // Parse function parameters
            _parameterNames.Clear();
            _parameterTypes.Clear();
            string[] parameters = header[1].Replace(")", "").Split(paramSplitter, StringSplitOptions.RemoveEmptyEntries);
            foreach (string param in parameters)
            {
                string[] pair = param.Split(spaceSplitter, StringSplitOptions.RemoveEmptyEntries);
                _parameterTypes.Add(stringToType(pair[0]));
                _parameterNames.Add(pair[1]);
            }

            // Set contents and resize accordingly
            Text = RawHeader;
            Width = Text.Length * 6 + Text.Length / 3;
        }

        private string typeToString(HsfType type)
        {
            // Converts given HsfType to string equivalent
            return Enum.GetName(typeof(HsfType), type);
        }

        private HsfType stringToType(string type)
        {
            // Attempts to convert given string to HsfType (bool by default)
            string[] types = Enum.GetNames(typeof(HsfType));
            for (int i = 0; i < types.Length; i++)
            {
                if (types[i].ToLower() == type.ToLower())
                {
                    return (HsfType)i;
                }
            }
            return HsfType.None;
        }
    }
}
