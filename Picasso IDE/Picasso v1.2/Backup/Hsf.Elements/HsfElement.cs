using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;

namespace Hsf.Elements
{
    public abstract class HsfElement
    {
        public string ElementType;
        public abstract string ToXml();
        public abstract void FromXml(string source);
    }

    class HsfElementException : ApplicationException
    {
        // Thrown when an element method fails
        public HsfElementException() { }
        public HsfElementException(string mess) : base(mess) { }
        public HsfElementException(string mess, Exception inn) : base(mess, inn) { }
        protected HsfElementException(System.Runtime.Serialization.SerializationInfo info, System.Runtime.Serialization.StreamingContext content) {}

        public string GetReport() {
            string toReturn = "An HsfElement has failed: " + Message;
            return toReturn;
        }
    }
}
