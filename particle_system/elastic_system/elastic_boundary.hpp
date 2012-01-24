#ifndef ELASTIC_BOUNDARY_HPP
#define ELASTIC_BOUNDARY_HPP
//=========================================================================
//
//  Program:   Modular Object Oriented Particle Simulator
//  Module:    ElasticBoundary
//
//  Copyright (c) Ricardo Ortiz
//  All rights reserved.
//     This software is distributed WITHOUT ANY WARRANTY; without even
//     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//     PURPOSE.
//
//=========================================================================
/// @name ElasticBoundary
/// @section Description
/// @section See also

#include "particle_system/elastic_system/spring_system.hpp"

template<typename Derived>
class ElasticBoundary : public SpringSystem<Derived>
{
    public:
        typedef typename surface_traits<Derived>::value_type    value_type;
        typedef typename surface_traits<Derived>::particle_type particle_type;
        typedef SpringSystem<Derived>    	       base_type;
        typedef typename base_type::spring_type       spring_type;
        typedef typename base_type::spring_iterator   spring_iterator;

    public:

        inline Derived &derived()
        {
            return *static_cast<Derived*>(this);
        }
        
        inline void setSprings(std::vector<size_t> &col_ptr, std::vector<size_t> &col_idx, std::vector<value_type> &strength)
        {
            logger.startTimer("setSprings");
            particle_type *particles = derived().particles();
            for(size_t p = 0; p < col_ptr.size() - 1; ++p)
                for(size_t i = col_ptr[p], end = col_ptr[p + 1]; i < end; ++i)
                    if(!this->existSpring(&particles[p], &particles[col_idx[i]]))
                    {
                        spring_iterator s = this->addSpring(&particles[p], &particles[col_idx[i]], strength[i]);
                        s->getAidx() = 3 * p;
                        s->getBidx() = 3 * col_idx[i];
                    }
            logger.stopTimer("setSprings");                   	    
        }
};


#endif
