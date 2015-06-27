/* Brian Kirkpatrick
 * Xql (eXtensible Query Language) parser
 * Begun 8/23/2009
 * 
 * Xql (different from XQuery Language) is used to simplify the reading of xml files by reducing searches to SQL-style
 * query strings. Child elements, attributes, and text nodes may be queried and returns from the specified element
 * source, where the queried nodes match the (optional) given conditions. Results are returned as a list of hash tables.
 * 
 * Consider the following Xml contents:
 * <head>
 *  <employee name="Alice">
 *      <ability skill="managing" level="2">
 *          <project name="v2.0 release" date="3/4/1999"/>
 *          <project name="support team"/>
 *      </ability>
 *      <ability skill="report writing" level="4" forte="budget reports"/>
 *      <hair color="brown" style="straight" length="14 inches"/>
 *      <age>27</age>
 *      Alice is a skilled employee with a passion for her work.
 *  </employee>
 *  <employee name="bob">
 *      <ability skill="typing" level="1"/>
 *      <ability skill="bragging" level="3"/>
 *      <age>31</age>
 *      Bob is a mediocre employee who gets on Alice's nerves.
 *  </employee>
 * </head>
 * 
 * Child elements may be selected by using a SELECT CHILDREN query. Note that the returned hash tables correspond to
 * children attributes, and that children of children and children text nodes are not returned.
 *  SELECT CHILDREN ability FROM employee
 *      returns
 *  [0] => ("skill"=>"managing", "level"=>"2")
 *  [1] => ("skill"=>"report writing", "level"=>"4", "forte"=>"budget reports")
 *  [2] => ("skill"=>"typing", "level"=>"1")
 *  [3] => ("skill"=>"bragging", "level"=>"1")
 *  
 * To be more specific, conditions may be added in a "WHERE" clause following the type and source specification.
 * Supported comparators include "=", ">", "<", ">=", "<=", "<>", and "CONTAINS". Comparators must match an attribute
 * or meta reference (on the left-hand side) to a string value (on the right-hand side).
 *  SELECT CHILDREN * FROM employee WHERE name="Alice"
 *      returns
 *  [0] => ("skill"=>"managing", "level"=>"2")
 *  [1] => ("skill"=>"report writing", "level"=>"4", "forte"=>"budget reports")
 *  [2] => ("color"=>"brown", "style"=>"straight", "length"=>"14 inches")
 *  
 * Attributes may be selected by using a SELECT ATTRIBUTES query.
 *  SELECT ATTRIBUTES skill, level FROM ability WHERE level > 1
 *      returns
 *  [0] => ("skill"=>"managing", "level"=>"2")
 *  [1] => ("skill"=>"report writing", "level"=>"4")
 *  [2] => ("skill"=>"bragging", "level"=>"3")
 *  
 * Text nodes may be also be selected from a source element, but no type is required. In this case, a meta reference
 * (_content) is also used. Meta references refer to nodes relative to the selected nodes from the given source, and
 * include _content (interior text content of node), _parent (node type of parent), and _children (array of child node
 * types). CONTAINS may be used with _children to match node types.
 *  SELECT TEXT FROM age WHERE _content CONTAINS "2%"
 *      returns
 *  [0] => ("_content"=>"27")
 *  
 */

using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;
using System.Text.RegularExpressions;

namespace PicassoGUI.Util
{
    public class XqlParser
    {
        private string _xmlContent;
        private const string _typePattern = "(ATTRIBUTES|CHILDREN|TEXT) ([A-Za-z0-9, _\\*]+)";
        private const string _sourcePattern = "([A-Za-z0-9_, ]+)";
        private const string _conditionPattern = "([A-Za-z0-9_]+) (=|>|<|>=|<=|<>|CONTAINS) \"([^\"]+)\"";
        private const string _lhCondPattern = "([A-Za-z0-9_]+) *";
        private const string _comparatorPattern = " *(=|>|<|>=|<=|<>|CONTAINS) *";
        private const string _rhCondPattern = " *\"([^\"]+)\"";

        public XqlParser()
        {
        }

        public XqlParser(string filename)
        {
            // If constructed with filename, loads xml into content string
            LoadXml(filename);
        }

        public void LoadXml(string filename)
        {
            // Loads xml into content string
            FileStream file = new FileStream(filename, FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(file);
            _xmlContent = sr.ReadToEnd();
            sr.Close();
            file.Close();
        }

        public List<Hashtable> Query(string qry)
        {
            // Supported:
            //   SELECT [attribute names] FROM [element types]
            string queryPattern = "^SELECT ([A-Za-z0-9_,\\s]+) FROM ([A-Za-z0-9_,\\s]+)$";
            List<Hashtable> toReturn = new List<Hashtable>();
            XmlNodeList nodeList;
            XmlDocument xDoc = new XmlDocument();
            xDoc.LoadXml(_xmlContent);
            Match match = Regex.Match(qry, queryPattern);

            if (match.Success)
            {
                // Get attributes and elements
                string[] splitter = { ", " };
                string attributes = match.Groups[1].ToString(); //.Split(splitter, StringSplitOptions.RemoveEmptyEntries);
                string[] elements = match.Groups[2].ToString().Split(splitter, StringSplitOptions.RemoveEmptyEntries);
                foreach (string element in elements)
                {
                    nodeList = xDoc.GetElementsByTagName(element);
                    foreach (XmlNode node in nodeList)
                    {
                        toReturn.Add(HashFromElement((XmlElement)node, attributes));
                    }
                }
            }
            else
            {
                throw new QueryFormatException("Invalid query pattern provided");
            }
            return toReturn;
        }

        public List<Hashtable> TestQuery(string qry)
        {
            // Parses given query and returns children, text, or element attributes selected by and from query conditions and source
            List<Hashtable> prequalResults = new List<Hashtable>();
            List<Hashtable> finalResults = new List<Hashtable>();
            Hashtable line = new Hashtable();
            Hashtable currLine = new Hashtable();
            string typeString, sourceString, conditionString = "";
            XqlQueryType queryType;

            // Check for query type (elements, text, attributes)
            if (Regex.Match(qry, "^SELECT CHILDREN ").Success)
            {
                queryType = XqlQueryType.Children;
            }
            else if (Regex.Match(qry, "^SELECT TEXT ").Success)
            {
                queryType = XqlQueryType.Text;
            }
            else if (Regex.Match(qry, "^SELECT ATTRIBUTES ").Success || Regex.Match(qry, "^SELECT * ").Success)
            {
                queryType = XqlQueryType.Attributes;
            }
            else
            {
                throw new QueryFormatException("Invalid SELECT type specified");
            }

            // Get / check for type in query (required)
            typeString = GetTypeString(qry);
            if (typeString == "")
            {
                throw new QueryFormatException("Invalid or missing type in query \"" + qry + "\"");
            }

            // Get / check for source in query (required)
            sourceString = GetSourceString(qry);
            if (sourceString == "")
            {
                throw new QueryFormatException("Invalid or missing source in query \"" + qry + "\"");
            }

            // Get / check for conditions in query (optional)
            conditionString = GetConditionString(qry);

            // Get list of all source elements, types, and conditions
            List<XmlElement> sourceElements = GetSourceElements(sourceString);
            List<string> types = GetTypes(typeString);
            List<string> conditions = GetConditions(conditionString);

            switch (queryType)
            {
                case XqlQueryType.Attributes:
                    // Compile pre-qualifying list of all requested source node attributes
                    foreach (XmlElement sourceElement in sourceElements)
                    {
                        Hashtable currHash = HashFromElement(sourceElement, typeString);
                        prequalResults.Add(currHash);
                    }

                    // If conditions given, compare before copying
                    if (conditionString == "")
                    {
                        finalResults = prequalResults;
                    }
                    else
                    {
                        foreach (Hashtable currHash in prequalResults)
                        {
                            // Check this row against conditions
                            if (RowMatchesCondition(currHash, conditionString))
                            {
                                finalResults.Add(currHash);
                            }
                        }
                    }
                    break;
                case XqlQueryType.Children:
                    break;
                case XqlQueryType.Text:
                    break;
                default:
                    throw new QueryFormatException("Invalid SELECT type specified");
            }

            return finalResults;
        }

        public bool RowMatchesCondition(Hashtable row, string condString)
        {
            // Return true if the condition is met in the row
            string[] splitter = {", "};
            string[] conditions = condString.Split(splitter, StringSplitOptions.RemoveEmptyEntries);
            string lh, comp, rh;
            foreach (string condition in conditions)
            {
                lh = GetLhCondString(condString);
                comp = GetComparatorString(condString);
                rh = GetRhCondString(condString);
                switch (comp)
                {
                    case "=":
                        break;
                    case ">":
                        break;
                    case "<":
                        break;
                    case ">=":
                        break;
                    case "<=":
                        break;
                    case "CONTAINS":
                        break;
                    default:
                        throw new QueryFormatException("Incorrectly formatted condition string \"" + condString + "\"");
                }
            }
            return true;
        }

        public Hashtable HashFromElement(XmlElement element, string typeString)
        {
            Hashtable toReturn = new Hashtable();
            if (typeString == "*")
            {
                // Generate hashtable from all attributes/values using regex
                string elementText = element.OuterXml;
                if (element.InnerXml != "") elementText = elementText.Replace(element.InnerXml, "");
                MatchCollection matches = Regex.Matches(elementText, "([A-Za-z0-9]+)\\s*=\\s*\"([A-Za-z0-9 ]+)\"");
                foreach (Match match in matches)
                {
                    toReturn.Add(match.Groups[1].ToString(), match.Groups[2].ToString());
                }
            }
            else
            {
                // Generate hashtable only from given types
                List<string> types = GetTypes(typeString);
                foreach (string type in types)
                {
                    toReturn.Add(type, element.GetAttribute(type));
                }
            }
            return toReturn;
        }

        public string StringFromElement(XmlElement element)
        {
            string elementText = element.OuterXml;
            if (element.InnerXml != "") elementText = elementText.Replace(element.InnerXml, "");
            return elementText;
        }

        public List<XmlElement> GetSourceElements(string sourceString)
        {
            // Given a string representing potential source element types, return a list of all such elements in current xml
            string[] types;
            List<XmlElement> toReturn = new List<XmlElement>();
            XmlDocument xDoc = new XmlDocument();
            XmlNodeList nodeList;

            // Determine desired types
            if (sourceString == "*")
            {
                types = GetNodeTypes();
            }
            else
            {
                string[] splitter = { ", " };
                types = sourceString.Split(splitter, StringSplitOptions.None);
            }

            // Retreive element objects from xml document
            xDoc.LoadXml(_xmlContent);
            foreach (string type in types)
            {
                nodeList = xDoc.GetElementsByTagName(type);
                foreach (XmlNode node in nodeList)
                {
                    toReturn.Add((XmlElement)node);
                }
            }
            return toReturn;
        }

        public List<string> GetTypes(string typeString)
        {
            // Returns a list of the types specified in the given type string from a query, assuming not *
            string[] splitter = {", "};
            string[] types = typeString.Split(splitter, StringSplitOptions.RemoveEmptyEntries);
            List<string> toReturn = new List<string>();
            foreach (string type in types)
            {
                toReturn.Add(type);
            }
            return toReturn;
        }

        public List<string> GetConditions(string condString)
        {
            // Returns a list of the types specified in the given type string from a query, assuming not *
            string[] splitter = { ", " };
            string[] types = condString.Split(splitter, StringSplitOptions.RemoveEmptyEntries);
            List<string> toReturn = new List<string>();
            foreach (string type in types)
            {
                toReturn.Add(type);
            }
            return toReturn;
        }

        public string[] GetNodeTypes()
        {
            // Returns a string array of all node types in current xml
            MatchCollection nodeTypes = Regex.Matches(_xmlContent, "<([A-Za-z0-9]+)[\\s>]");
            List<string> results = new List<string>();
            string tmp;
            foreach (Match match in nodeTypes)
            {
                tmp = match.Groups[1].ToString();
                if (!results.Contains(tmp) && tmp != "")
                {
                    results.Add(tmp);
                }
            }
            string[] toReturn = new string[results.Count];
            int i = 0;
            foreach (string match in results)
            {
                toReturn[i] = match;
                i++;
            }
            return toReturn;
        }


        public string GetTypeString(string qry) {
            Match match = Regex.Match(qry, "^SELECT " + _typePattern + " FROM");
            if (match.Success) {
                return match.Groups[2].ToString();
            } else {
                return "";
            }
        }

        public string GetSourceString(string qry) {
            Match match = Regex.Match(qry, "^SELECT " + _typePattern + " FROM " + _sourcePattern + " WHERE");
            if (match.Success)
            {
                return match.Groups[2].ToString();
            } else {
                match = Regex.Match(qry, "^SELECT " + _typePattern + " FROM " + _sourcePattern + "$");
                if (match.Success)
                {
                    return match.Groups[3].ToString();
                }
                return "";
            }
        }

        public string GetLhCondString(string condString)
        {
            Match match = Regex.Match(condString, "^" + _lhCondPattern + " *" + _comparatorPattern + " *" + _rhCondPattern + "$");
            string grp0 = match.Groups[0].ToString();
            string grp1 = match.Groups[1].ToString();
            string grp2 = match.Groups[2].ToString();
            string grp3 = match.Groups[3].ToString();
            string grp4 = match.Groups[4].ToString();
            string grp5 = match.Groups[5].ToString();
            if (match.Success)
            {
                return match.Groups[3].ToString();
            }
            else
            {
                return "";
            }
        }

        public string GetComparatorString(string condString)
        {
            Match match = Regex.Match(condString, "^" + _lhCondPattern + " *" + _comparatorPattern + " *" + _rhCondPattern + "$");
            string grp0 = match.Groups[0].ToString();
            string grp1 = match.Groups[1].ToString();
            string grp2 = match.Groups[2].ToString();
            string grp3 = match.Groups[3].ToString();
            string grp4 = match.Groups[4].ToString();
            string grp5 = match.Groups[5].ToString();
            if (match.Success)
            {
                return match.Groups[4].ToString();
            }
            else
            {
                return "";
            }
        }

        public string GetRhCondString(string condString)
        {
            Match match = Regex.Match(condString, "^" + _lhCondPattern + " *" + _comparatorPattern + " *" + _rhCondPattern + "$");
            string grp0 = match.Groups[0].ToString();
            string grp1 = match.Groups[1].ToString();
            string grp2 = match.Groups[2].ToString();
            string grp3 = match.Groups[3].ToString();
            string grp4 = match.Groups[4].ToString();
            string grp5 = match.Groups[5].ToString();
            if (match.Success)
            {
                return match.Groups[5].ToString();
            }
            else
            {
                return "";
            }
        }
        
        public string GetConditionString(string qry)
        {
            Match match = Regex.Match(qry, "^SELECT " + _typePattern + " FROM " + _sourcePattern + " WHERE " + _lhCondPattern + " *" + _comparatorPattern + " *" + _rhCondPattern + "$");
            string grp0 = match.Groups[0].ToString();
            string grp1 = match.Groups[1].ToString();
            string grp2 = match.Groups[2].ToString();
            string grp3 = match.Groups[3].ToString();
            string grp4 = match.Groups[4].ToString();
            string grp5 = match.Groups[5].ToString();
            if (match.Success)
            {
                return match.Groups[3].ToString() + match.Groups[4].ToString() + "\"" + match.Groups[5].ToString() + "\"";
            } else {
                return "";
            }
        }

        private Hashtable select(XmlElement el, List<string> items)
        {
            // Returns a hashtable filled with key-value pairs cooresponding to the provided items, selected from the given element
            // First, check attributes
            Hashtable results = new Hashtable();
            foreach (string item in items) {
                results.Add(item, el.Attributes[item]);
            }
            return results;
        }
    }

    public enum XqlQueryType {
        Children,
        Attributes,
        Text
    }

    // --- Xql Parsing Exceptions --- //
    public class QueryFormatException : ApplicationException
    {
        // Thrown when a query is improperly formatted
        public QueryFormatException() { }
        public QueryFormatException(string mess) : base(mess) { }
        public QueryFormatException(string mess, Exception inn) : base(mess, inn) { }
        protected QueryFormatException(System.Runtime.Serialization.SerializationInfo info, System.Runtime.Serialization.StreamingContext content) {}

        public string GetReport() {
            string toReturn = "An Xql query has been improperly formatted:\n" + Message;
            return toReturn;
        }
    }
}
