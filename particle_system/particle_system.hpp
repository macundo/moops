#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP
/****************************************************************************
** MOOPS -- Modular Object Oriented Particle Simulator
** Copyright (C) 2011-2012  Ricardo Ortiz <ortiz@unc.edu>
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/
#include<limits>
#include<algorithm>

/** \class ParticleSystem
 *  \ingroup ParticleSystem_Module
 *  \brief Provides basic handling and the high level storage for the particle data.
 *  \tparam Derived is the derived type, ie the surface being modeled
 */
template<typename Derived>
class ParticleSystem
{
    public:
        typedef typename Traits<Derived>::value_type value_type;
        typedef typename Traits<Derived>::particle_type particle_type;
        typedef typename Traits<Derived>::storage_type storage_type;
        typedef typename storage_type::particle_iterator particle_iterator;
        typedef typename storage_type::data_iterator data_iterator;
        

    private:
        value_type            m_time;         ///< Current time.
        storage_type          m_storage;
        size_t         m_num_particles;
        value_type            m_domain[2][3];
        
    public:

        ParticleSystem(size_t num_particles) : m_time(0.0), m_storage(num_particles), m_num_particles(num_particles)
        {
        }
        ~ParticleSystem() { };

    public:

        inline value_type & time() { return m_time; }
        inline value_type const & time() const { return m_time; }
        inline value_type *positions()  { return m_storage.positions(); }
        inline value_type const *positions()  const { return m_storage.positions(); }
        inline value_type *velocities() { return m_storage.velocities(); }
        inline value_type const *velocities() const { return m_storage.velocities(); }
        inline value_type *forces()     { return m_storage.forces(); }
        inline value_type const *forces() const { return m_storage.forces(); }
        inline particle_type const *particles() const { return m_storage.particles(); }
        inline particle_type *particles()             { return m_storage.particles(); }

        inline particle_iterator particles_begin() { return m_storage.particles_begin(); }
        inline particle_iterator particles_end() { return m_storage.particles_end(); }
        inline data_iterator positions_begin() { return m_storage.positions_begin(); }
        inline data_iterator positions_end() { return m_storage.positions_end(); }
        inline data_iterator velocities_begin() { return m_storage.velocities_begin(); }
        inline data_iterator velocities_end() { return m_storage.velocities_end(); }
        inline data_iterator forces_begin() { return m_storage.forces_begin(); }
        inline data_iterator forces_end() { return m_storage.forces_end(); }

        inline void setDomain()
        {
            particle_type *p = m_storage.particles();
            value_type min[3] = {p[0].position[0], p[0].position[1], p[0].position[2]};
            value_type max[3] = {p[0].position[0], p[0].position[1], p[0].position[2]};
            // center
            m_domain[0][0] = m_domain[0][1] = m_domain[0][2] = 0;
            // extent
            m_domain[1][0] = m_domain[1][1] = m_domain[1][2] = 0;
            for (int i = 0; i < m_num_particles; i++)
            {
                for (int k = 0 ; k < 3; ++k)
                {
                    if (p[i].position[k] > max[k])
                        max[k] = p[i].position[k];
                    if (p[i].position[k] < min[k])
                        min[k] = p[i].position[k];
                    m_domain[0][k] += p[i].position[k];
                }
            }

            value_type R0 = 0;
            for (int i = 0; i < 3; ++i)
            {
                m_domain[0][i] /= m_num_particles;
                m_domain[0][i] = int(m_domain[0][i] + .5); // shift center to nearest integer
                R0 = std::max(max[i] - m_domain[0][i], R0);
                R0 = std::max(m_domain[0][i] - min[i], R0);
                max[i] = std::abs(max[i] - m_domain[0][i]) + .001;
                min[i] = std::abs(min[i] - m_domain[0][i]) + .001;
            }

            m_domain[1][0] = m_domain[1][1] = m_domain[1][2] = R0 * 1.000001;
        }

        void clearTime() { m_time = 0.; }
        inline void clearForces() { std::fill(forces(), forces() + data_size(), 0.0); }
        inline void clearVelocities() { std::fill(velocities(), velocities() + data_size(), 0.0); }
        inline std::size_t particles_size() { return m_num_particles; }
        inline std::size_t data_size() { return m_storage.data_size(); }
        storage_type &storage() { return m_storage; }

        template<typename out_stream>
        out_stream &writeForces(out_stream &out = std::cout, bool fortran = false)
        {
            value_type *p = this->forces();
            if (!fortran)
            {
                out << "f = [";
                for (size_t i = 0, idx = 0; i < this->particles_size(); ++i, idx += 3)
                    out << p[idx] << "," << p[idx + 1] << "," << p[idx + 2] << ";";
                out << "];" << std::endl;
            }
            else
            {
                out << this->particles_size() << " " << 3 << " " << 3*this->particles_size() << std::endl;
                for (size_t i = 0, idx = 0; i < this->particles_size(); ++i, idx += 3)
                    out << p[idx] << " " << p[idx + 1] << " " << p[idx + 2] << "\n";
                out << std::endl;
            }
            return out;
        }

        template<typename out_stream>
        out_stream &writeVelocities(out_stream &out = std::cout, bool fortran = false)
        {
            value_type *p = this->velocities();
            if (!fortran)
            {
                out << "v = [";
                for (size_t i = 0, idx = 0; i < this->particles_size(); ++i, idx += 3)
                    out << p[idx] << "," << p[idx + 1] << "," << p[idx + 2] << ";";
                out << "];" << std::endl;
            }
            else
            {
                out << this->particles_size() << " " << 3 << " " << 3*this->particles_size() << std::endl;
                for (size_t i = 0, idx = 0; i < this->particles_size(); ++i, idx += 3)
                    out << p[idx] << " " << p[idx + 1] << " " << p[idx + 2] << "\n";
                out << std::endl;
            }
            return out;
        }

        template<typename out_stream>
        out_stream &writePositions(out_stream &out = std::cout, bool fortran = false)
        {
            value_type *p = this->positions();
            if (!fortran)
            {
                out << "p = [";
                for (size_t i = 0, idx = 0; i < this->particles_size(); ++i, idx += 3)
                    out << p[idx] << "," << p[idx + 1] << "," << p[idx + 2] << ";";
                out << "];" << std::endl;
            }
            else
            {
                out << this->particles_size() << " " << 3 << " " << 3*this->particles_size() << std::endl;
                for (size_t i = 0, idx = 0; i < this->particles_size(); ++i, idx += 3)
                    out << p[idx] << " " << p[idx + 1] << " " << p[idx + 2] << "\n";
                out << std::endl;
            }
            return out;
        }
        
        template<typename out_stream>
        void writeData(out_stream &out)
        {
            writePositions(out);
            writeForces(out);
            writeVelocities(out);
        }
        
        template<typename out_stream>
        friend out_stream &operator<<(out_stream &out,ParticleSystem<Derived> &system)
        {
            system.writeData(out);
            return out;
        }
        
};

#endif
