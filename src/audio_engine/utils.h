//
// Created by samehh on 8/26/26.
//

#ifndef OPENVOICE_UTILS_H
#define OPENVOICE_UTILS_H



inline ma_result result;

inline void check_result(const std::string msg)
{
    if (result != MA_SUCCESS)
    {
        std::cout << "error : " << msg << std::endl;
    }
}




#endif //OPENVOICE_UTILS_H
