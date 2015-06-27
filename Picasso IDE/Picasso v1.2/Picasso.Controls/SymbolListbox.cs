using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Hsf.Scripting;
using Hsf.Utility;
using Picasso.Utility;

namespace Picasso.Controls
{
    public partial class SymbolListbox : ListBox
    {
        private ImageList _symbolIcons;
        private List<ScriptingSymbol> _symbols;
        private List<SymbolInfo> _symbolInfo;

        public ImageList SymbolIcons
        {
            get { return _symbolIcons; }
            set { _symbolIcons = value; }
        }

        public List<ScriptingSymbol> Symbols
        {
            get { return _symbols; }
        }

        public SymbolListbox()
        {
            DrawMode = DrawMode.OwnerDrawFixed;
            InitializeComponent();
            _symbols = new List<ScriptingSymbol>();
            _symbolInfo = new List<SymbolInfo>();
            _symbolIcons = new ImageList();

            // Add keyword symbol icon and info
            Bitmap key = global::Picasso.Controls.Properties.Resources.KeywordSymbol;
            key.MakeTransparent(Color.FromArgb(255, 0, 255));
            _symbolIcons.Images.Add(key);
            SymbolInfo keywordInfo = new SymbolInfo();
            keywordInfo.Ndx = 0;
            keywordInfo.Icon = (Bitmap)_symbolIcons.Images[keywordInfo.Ndx];
            keywordInfo.Icon.MakeTransparent(Color.FromArgb(255, 0, 255));
            keywordInfo.Shade = Color.Blue;
            keywordInfo.Type = LuaSymbolType.Keyword;
            _symbolInfo.Add(keywordInfo);

            // Add class symbol info
            Bitmap cls = global::Picasso.Controls.Properties.Resources.ClassSymbol;
            cls.MakeTransparent(Color.FromArgb(255, 0, 255));
            _symbolIcons.Images.Add(cls);
            SymbolInfo classInfo = new SymbolInfo();
            classInfo.Ndx = 1;
            classInfo.Icon = (Bitmap)_symbolIcons.Images[classInfo.Ndx];
            classInfo.Icon.MakeTransparent(Color.FromArgb(255, 0, 255));
            classInfo.Shade = Color.Teal;
            classInfo.Type = LuaSymbolType.Class;
            _symbolInfo.Add(classInfo);

            // Add local symbol info
            Bitmap lcl = global::Picasso.Controls.Properties.Resources.LocalSymbol;
            lcl.MakeTransparent(Color.FromArgb(255, 0, 255));
            _symbolIcons.Images.Add(lcl);
            SymbolInfo localInfo = new SymbolInfo();
            localInfo.Ndx = 2;
            localInfo.Icon = (Bitmap)_symbolIcons.Images[localInfo.Ndx];
            localInfo.Icon.MakeTransparent(Color.FromArgb(255, 0, 255));
            localInfo.Shade = Color.Black;
            localInfo.Type = LuaSymbolType.Local;
            _symbolInfo.Add(localInfo);

            // Add function symbol info
            Bitmap fcn = global::Picasso.Controls.Properties.Resources.FunctionSymbol;
            fcn.MakeTransparent(Color.FromArgb(255, 0, 255));
            _symbolIcons.Images.Add(fcn);
            SymbolInfo functionInfo = new SymbolInfo();
            functionInfo.Ndx = 3;
            functionInfo.Icon = (Bitmap)_symbolIcons.Images[functionInfo.Ndx];
            functionInfo.Icon.MakeTransparent(Color.FromArgb(255, 0, 255));
            functionInfo.Shade = Color.Black;
            functionInfo.Type = LuaSymbolType.Function;
            _symbolInfo.Add(functionInfo);

            // Add global symbol info
            Bitmap gbl = global::Picasso.Controls.Properties.Resources.GlobalSymbol;
            gbl.MakeTransparent(Color.FromArgb(255, 0, 255));
            _symbolIcons.Images.Add(gbl);
            SymbolInfo globalInfo = new SymbolInfo();
            globalInfo.Ndx = 4;
            globalInfo.Icon = (Bitmap)_symbolIcons.Images[globalInfo.Ndx];
            globalInfo.Icon.MakeTransparent(Color.FromArgb(255, 0, 255));
            globalInfo.Shade = Color.Black;
            globalInfo.Type = LuaSymbolType.Global;
            _symbolInfo.Add(globalInfo);

            // Add table info
            Bitmap tbl = global::Picasso.Controls.Properties.Resources.TableSymbol;
            tbl.MakeTransparent(Color.FromArgb(255, 0, 255));
            _symbolIcons.Images.Add(tbl);
            SymbolInfo tableInfo = new SymbolInfo();
            tableInfo.Ndx = 5;
            tableInfo.Icon = (Bitmap)_symbolIcons.Images[tableInfo.Ndx];
            tableInfo.Icon.MakeTransparent(Color.FromArgb(255, 0, 255));
            tableInfo.Shade = Color.Black;
            tableInfo.Type = LuaSymbolType.Table;
            _symbolInfo.Add(tableInfo);

            // Load symbols from xml resource
            Items.Clear();
            string intelContent = global::Picasso.Controls.Properties.Resources.intelData;
            LoadFromXml(intelContent);
        }

        public void UpdateItems()
        {
            // Updates items in listbox from symbol table
            Items.Clear();
            foreach (ScriptingSymbol symb in _symbols)
            {
                if (symb.Enabled)
                {
                    SymbolListItem item = new SymbolListItem();
                    item.Name = symb.Name;
                    item.Ndx = getInfoByType(symb.Type).Ndx;
                    Items.Add(item);
                }
            }
            bool flag = true; if (flag) { flag = true; }
        }

        private SymbolInfo getInfoByType(LuaSymbolType type)
        {
            // Returns symbol info based on given type
            foreach (SymbolInfo i in _symbolInfo)
            {
                if (i.Type == type) { return i; }
            }
            return new SymbolInfo();
        }

        public void LoadFromScript(string script)
        {
            // Scans script in string for new symbols
            UpdateItems();
        }

        public void LoadFromScript(string[] script)
        {
            // Scans lines of script for new symbols
            UpdateItems();
        }

        public void LoadFromXml(string content)
        {
            // Parse Xml spec for symbols
            XqlParser parser = new XqlParser();
            parser.LoadData(content);

            // Search for keyword definitions
            List<Hashtable> results = parser.Query("SELECT name, type FROM Keyword");
            foreach (Hashtable result in results)
            {
                ScriptingSymbol newSymbol = new ScriptingSymbol();
                newSymbol.Name = result["name"].ToString();
                newSymbol.Type = LuaSymbolType.Keyword;
                newSymbol.Enabled = true;
                _symbols.Add(newSymbol);
            }

            // Search for class definitions and symbols within each class scope
            results = parser.Query("SELECT name, _outerXml FROM Class");
            XqlParser subParser = new XqlParser();
            foreach (Hashtable result in results)
            {
                ScriptingSymbol newSymbol = new ScriptingSymbol();
                newSymbol.Name = result["name"].ToString();
                newSymbol.Type = LuaSymbolType.Class;
                newSymbol.Enabled = true;
                _symbols.Add(newSymbol);

                // Add method and member symbols within this class scope (disabled until scope opened)
                subParser.LoadData(result["_outerXml"].ToString());
                List<Hashtable> subResults = subParser.Query("SELECT name, type FROM Method");
                foreach (Hashtable subResult in subResults)
                {
                    ScriptingSymbol classMethod = new ScriptingSymbol();
                    classMethod.Name = subResult["name"].ToString();
                    classMethod.Type = LuaSymbolType.Function;
                    classMethod.Enabled = false;
                    classMethod.ParentSymbol = newSymbol;
                    _symbols.Add(classMethod);
                }
                subResults = subParser.Query("SELECT name, type FROM Property");
                foreach (Hashtable subResult in subResults)
                {
                    ScriptingSymbol classProperty = new ScriptingSymbol();
                    classProperty.Name = subResult["name"].ToString();
                    classProperty.Type = LuaSymbolType.Local;
                    classProperty.Enabled = false;
                    classProperty.ParentSymbol = newSymbol;
                    _symbols.Add(classProperty);
                }
            }

            // Sort all symbols and update listbox content
            SortSymbols();
            UpdateItems();
        }

        public void SortSymbols()
        {
            // Sorts symbols using ToString comparison
            bool sorted = false;
            ScriptingSymbol tmpSym;
            while (!sorted)
            {
                sorted = true;
                for (int i = 0; i < _symbols.Count - 1; i++)
                {
                    if (String.Compare(_symbols[i + 1].ToString(), _symbols[i].ToString()) < 0)
                    {
                        sorted = false;
                        tmpSym = _symbols[i];
                        _symbols[i] = _symbols[i + 1];
                        _symbols[i + 1] = tmpSym;
                    }
                }
            }

        }

        public void VoidLocalScope()
        {
            foreach (ScriptingSymbol symbol in _symbols)
            {
                if (symbol.Type == LuaSymbolType.Local)
                {
                    symbol.Enabled = false;
                }
            }
        }

        public ScriptingSymbol FindSymbol(string name)
        {
            foreach (ScriptingSymbol s in _symbols)
            {
                if (s.Name == name)
                {
                    return s;
                }
            }
            return new ScriptingSymbol();
        }

        protected override void OnDrawItem(DrawItemEventArgs e)
        {
            // Draw a specific item in Items collection (should be a SymbolListItem)
            if (e.Index > _symbols.Count) { return; }
            e.DrawBackground();
            e.DrawFocusRectangle();
            SymbolListItem item;
            Rectangle bounds = e.Bounds;
            Size imgSize = _symbolIcons.ImageSize;
            try
            {
                item = (SymbolListItem)Items[e.Index];
                if (item.Ndx >= 0 && item.Ndx < _symbolIcons.Images.Count)
                {
                    // Draw icon
                    _symbolIcons.Draw(e.Graphics, bounds.Left, bounds.Top, item.Ndx);
                    e.Graphics.DrawString(item.Name, e.Font, new SolidBrush(e.ForeColor), bounds.Left + imgSize.Width, bounds.Top);
                }
                else
                {
                    // Just draw string
                    e.Graphics.DrawString(item.Name, e.Font, new SolidBrush(e.ForeColor), bounds.Left, bounds.Top);
                }
            }
            catch (InvalidCastException)
            {
                // Item e.Index in Items not a SymbolListItem
                base.OnDrawItem(e);
            }
        }

        public SymbolInfo FindSymbolInfo(string name)
        {
            // Returns the information for the symbol in the listbox with the given name, or blank ('none') info if not present
            foreach (ScriptingSymbol sym in _symbols)
            {
                if (sym.Name == name)
                {
                    return getInfoByType(sym.Type);
                }
            }
            return new SymbolInfo();
        }

        public void HighlightBestGuess(string guess)
        {
            // Highlights the closest match symbol to the current word
            SymbolListItem currItem;
            int bestGuess = -1;
            int compResult = 0;
            for (int i = 0; i < Items.Count; i++)
            {
                try
                {
                    currItem = (SymbolListItem)Items[i];
                    ScriptingSymbol sym = FindSymbol(currItem.Name);
                    if (sym.Enabled)
                    {
                        compResult = guess.CompareTo(sym.Name);
                        if (compResult < 0)
                        {
                            if (sym.Name.Contains(guess))
                            {
                                // Partial match; select and break
                                SelectedIndex = i;
                                return;
                            }
                            else
                            {
                                // Too far; deselect and break
                                UnselectAll();
                                return;
                            }
                        }
                        else if (compResult == 0)
                        {
                            // Perfect match; select and break
                            SelectedIndex = i;
                            return;
                        }
                        else
                        {
                            // Not there yet; continue
                            bestGuess = i;
                        }
                    }
                }
                catch (InvalidCastException)
                {
                    // Wasn't a symbol list item; shouldn't happen!
                    MessageBox.Show("What happened!?");
                }
            }

            // If we haven't set a match by this point, don't select anything
            UnselectAll();
        }

        public void UnselectAll()
        {
            for (int i = 0; i < Items.Count; i++)
            {
                SetSelected(i, false);
            }
        }

        public void LimitToNamespace(string name)
        {
            // Limits enabled list items to those with the given parent namespace (class, library, etc)
            foreach (ScriptingSymbol sym in _symbols)
            {
                if (sym.ParentSymbol != null && sym.ParentSymbol.ToString() == name)
                {
                    sym.Enabled = true;
                }
                else
                {
                    sym.Enabled = false;
                }
            }
            UpdateItems();
        }

        public void ResetNamespace()
        {
            // Disables all items with parent namespaces, enables all others
            foreach (ScriptingSymbol sym in _symbols)
            {
                if (sym.ParentSymbol != null)
                {
                    sym.Enabled = false;
                }
                else
                {
                    sym.Enabled = true;
                }
            }
            UpdateItems();
        }

        public string CompleteSelection(string firstHalf)
        {
            // Returns the second half of the current selection (to which the firstHalf is given)
            string wholeWord = ((SymbolListItem)SelectedItem).Name;
            string secondHalf = "";
            for (int i = firstHalf.Length; i < wholeWord.Length; i++)
            {
                secondHalf += wholeWord[i];
            }
            return secondHalf;
        }
    }
}
