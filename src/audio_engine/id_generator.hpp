
#ifndef OPENVOICE_ID_GENERATOR_HPP
#define OPENVOICE_ID_GENERATOR_HPP


class id_generator
{

    inline static unsigned int next_id = 1;


public:

    static unsigned int get_id()
    {
        return next_id++;
    }

};





#endif //OPENVOICE_ID_GENERATOR_HPP
