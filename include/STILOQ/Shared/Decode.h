#pragma once
#include <boost/multiprecision/cpp_int.hpp>
#include "STILOQ/Shared/List.h"

using LargeInt = boost::multiprecision::cpp_int;

namespace STILOQ
{
    inline void fromBinaryToVector(std::span<const int>& blist, std::vector<int>& mlist, int digitNum, int stringSize)
    {
        int c = 0;

        for(int i = 0; i < stringSize; i++)
        {
            mlist[i] = 0;
            int digi = 1;

            for(int j = 0; j < digitNum; j++)
            {
                if(blist[c] == 1)
                {
                    mlist[i] += digi;
                }

                digi *= 2;
                c++;
            }            
        }
    }

    inline LargeInt fromBinaryToIndex(std::span<const int>& blist, int bstringSize)
    {
        LargeInt digi = 1;
        LargeInt index = 0;

        for(int i = 0; i < bstringSize; i++)
        {
            if(blist[i] == 1)
            {
                index += digi;
            }

            digi *= 2;
        }

        return index;
    }

    inline void fromIndexToVector(LargeInt index, LargeInt digit, std::vector<int>& mlist,
        int stateNum, int stringLength)
    {
        for(int i = 0; i < stringLength; i++)
        {
            mlist[i] = (int)(index / digit);
            index -= mlist[i] * digit;
            digit /= stateNum;
        }
    }

    inline void fromIndexToPermutation(LargeInt index, LargeInt digit, std::vector<int>& plist,
        std::vector<int>& slist, int stringLength)
    {
        for(int i = 0; i < stringLength; i++)
        {
            slist[i] = i;
        }

        for(int i = 0; i < stringLength - 1; i++)
        {
            int ind = (int)(index / digit);
            plist[i] = slist[ind];
            deleteElement(slist, ind, stringLength - i);
            index -= ind * digit;
            digit /= stringLength - i - 1;            
        }

        plist[stringLength - 1] = slist[0];
    }
}