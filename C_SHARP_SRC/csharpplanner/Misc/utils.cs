
// misc utils
using System;
using NSActionAbstract;

namespace NSUtils {

    public static class Utils
    {
        public static float GetRandom(float min, float max) 
        {
            Random rand = new Random();
            return ((float)rand.NextDouble()) * (max - min) + min;
        }

         public static void Quicksort(IComparable[] elements, int left, int right)
       //public static void Quicksort(ENUM_ACTION_TYPE[] types, int left, int right)
        {
           // Array elements = types;

            int i = left, j = right;
            IComparable pivot = elements[(left + right) / 2];

            while (i <= j)
            {
                while (elements[i].CompareTo(pivot) < 0)
                {
                    i++;
                }

                while (elements[j].CompareTo(pivot) > 0)
                {
                    j--;
                }

                if (i <= j)
                {
                    // Swap
                    IComparable tmp = elements[i];
                    elements[i] = elements[j];
                    elements[j] = tmp;

                    i++;
                    j--;
                }
            }

            // Recursive calls
            if (left < j)
            {
                Quicksort(elements, left, j);
            }

            if (i < right)
            {
                Quicksort(elements, i, right);
            }
        }


        // get a random number between the two given values
       // public static float GetRandom(float min, float max)
       // {
        //    float randNum = (float)rand() / RAND_MAX;
         //   float num = min + (max - min) * randNum;
          //  return num;
       // }

        //
        //given two values, this will determine the smaller of the two values using the < operator
        // template<typename T>
        // inline T GetMin(T a, T b)					{ return ( (a < b) ? a : b ); }

        //given two values this will return the larger of the two values using the < operator
        // template<typename T>
        // inline T GetMax(T a, T b)					{ return ( (a < b) ? b : a ); }

        //given a value and two extents, this will ensure that min <= val <= max using the < operator
        // template<typename T>
        // inline T Clamp(T a, T min, T max)		{ return ((a < min) ? min : ((a < max) ? a : max)); }

    }

}