
#ifndef OPENVOICE_ID_GENERATOR_HPP
#define OPENVOICE_ID_GENERATOR_HPP


class id_generator
{

    unsigned int next_id;

public:
    id_generator()
    {
        next_id =  1;
    }

    unsigned int get_id()
    {
        return next_id++;
    }

};





#endif //OPENVOICE_ID_GENERATOR_HPP
