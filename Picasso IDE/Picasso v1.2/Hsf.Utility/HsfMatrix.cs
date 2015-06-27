using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace Hsf.Utility
{
    public class HsfMatrix
    {
        private int height;
        private int width;
        private double[] values;

        public double this[int i, int j]
        {
            // Define matrix (2-index) accessors
            get
            {
                int ndx = i * width + j;
                if (ndx >= height * width)
                {
                    throw new IndexOutOfRangeException("Indices " + i + "," + j + " are out of matrix bounds");
                }
                else
                {
                    return values[ndx];
                }
            }
            set
            {
                int ndx = i * width + j;
                if (ndx >= height * width)
                {
                    throw new IndexOutOfRangeException("Indices " + i + "," + j + " are out of matrix bounds");
                }
                else
                {
                    values[ndx] = value;
                }
            }
        }

        public double this[int i]
        {
            // Define single-parameter accessor (matrix as vector)
            get
            {
                if (i >= height * width)
                {
                    throw new IndexOutOfRangeException("Index " + i + " out of matrix bounds");
                }
                else
                {
                    return values[i];
                }
            }
            set
            {
                if (i >= height * width)
                {
                    throw new IndexOutOfRangeException("Index " + i + " out of matrix bounds");
                }
                else
                {
                    values[i] = value;
                }
            }
        }

        public HsfMatrix(int h, int w)
        {
            // Dimensions of matrix must be specified upon creation
            height = h;
            width = w;
            values = new double[height * width];
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    if (i == j)
                        this[i, j] = 1.0;
                    else
                        this[i, j] = 0.0;
                }
            }
            h = height;
        }

        public void ChangeDimensions(int newH, int newW)
        {
            // Changes dimensions, while attempting to keep old values
            if (newH * newW >= height * width)
            {
                // Same size or bigger; copy all over and fill with zeros
                double[] oldValues = new double[height * width];
                for (int i = 0; i < height; i++)
                {
                    for (int j = 0; j < width; j++)
                    {
                        oldValues[i * width + j] = values[i * width + j];
                    }
                }
                values = new double[newH * newW];
                for (int i = 0; i < newH; i++)
                {
                    for (int j = 0; j < newW; j++)
                    {
                        if (i * newW + j >= height * width) { values[i * newW + j] = 0.0; }
                        else { values[i * newW + j] = oldValues[i * newW + j]; }
                    }
                }
            }
            else
            {
                // Smaller; copy based on new size
                double[] oldValues = new double[height * width];
                for (int i = 0; i < height; i++)
                {
                    for (int j = 0; j < width; j++)
                    {
                        oldValues[i * width + j] = values[i * width + j];
                    }
                }
                values = new double[newW * newH];
                for (int i = 0; i < newH; i++)
                {
                    for (int j = 0; j < newW; j++)
                    {
                        values[i * newW + j] = oldValues[i * newW + j];
                    }
                }
            }
            height = newH;
            width = newW;
        }

        public override string ToString()
        {
            // Writes matrix in Hsf format
            string toReturn = "Matrix(" + height + "," + width + ",{";
            for (int i = 0; i < values.Length; i++)
            {
                if (i == 0) toReturn += Convert.ToString(values[i]);
                else toReturn += "," + Convert.ToString(values[i]);
            }
            toReturn += "})";
            return toReturn;
        }

        public void FromString(string source)
        {
            // Attempts to load matrix values from string source using regex
            Match m = Regex.Match(source, "Matrix\\(([0-9]+),([0-9]+),{([0-9,\\s-\\.]+)}\\)");
            height = Convert.ToInt32(m.Groups[1].Value);
            width = Convert.ToInt32(m.Groups[2].Value);
            values = new double[height * width];
            string[] splitter = { "," };
            string rawValues = m.Groups[3].Value;
            string[] valuesArr = rawValues.Split(splitter, StringSplitOptions.RemoveEmptyEntries);
            int ndx = 0;
            foreach (string v in valuesArr)
            {
                values[ndx] = Convert.ToDouble(v.Trim());
                ndx++;
            }
            if (ndx != height * width)
            {
                throw new IndexOutOfRangeException("Matrix dimensions did not match size of values list");
            }
        }
    }
}
