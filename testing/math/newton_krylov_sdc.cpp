
#include<iostream>
#include<cstddef>
#include <cmath>
#include <iterator>
#include<algorithm>

#include "math/ode_solver/sdc/newton_krylov_sdc.hpp"
#include "rhs_functions.hpp"

int main()
{
    typedef double value_type;
    {
        function_type<1> F;
        value_type time = 0, dt = .01;

        const int N = 20;
        value_type p[N] = {0};
        value_type Fp[N] = {0}, f0 = 0;
        for(int i = 0; i < N; ++i)
            p[i] = -1+.1*i;
        NewtonKrylovSdc<value_type,function_type<1> > sdc(F);
        sdc.m_F.init(1,dt,&f0);
        for(int i = 0; i < N; ++i)
            sdc.m_F(&p[i],&Fp[i]);

        std::cout << "p = [";
        std::copy(p,p+N,std::ostream_iterator<value_type>(std::cout," "));
        std::cout << "]\n";
        std::cout << "Fp = [";
        std::copy(Fp,Fp+N,std::ostream_iterator<value_type>(std::cout," "));
        std::cout << "]\n";
        
        const int size = 2;
        value_type error[size] = {0};
        
        value_type x = 1, f = 0;
        F(time,&x,&f);
        for (size_t i = 0; i < size-1; ++i)
        {
            sdc(time,&x,&f,dt);
            time += dt;
            error[i+1] = x - std::exp(std::sin(time));
        }
        
        std::cout << "error = [";
        std::copy(error,error+size,std::ostream_iterator<value_type>(std::cout," "));
        std::cout << "]\n";
    }
//     {
//         function_type<3> F;
// 
//         ExplicitSDC<value_type, function_type<3>, SDCSpectralIntegrator<value_type, 0, 5, 2, 5>, 5, 4 > sdc(F);
// 
//         const int size = 30;
//         value_type x[size][3] = {{1,0,0}};
//         value_type f[size][3] = {0};
//         value_type time = 0, dt = .001;
//         F(time,x[0],f[0]);
//         for (size_t i = 0; i < size - 1; ++i)
//         {
//             sdc(time,x[i+1],x[i],f[i+1],f[i],dt);
//             time += dt;
//         }
//         std::cout << "x = [";
//         for (int i = 0; i < size; ++i)
//             std::copy(x[i], x[i] + 3, std::ostream_iterator<value_type>(std::cout, " "));
//         std::cout << "] \n";
//     }
//     {
//         const size_t size = 100;
//         const size_t spatial_size = 40;
//         const size_t ode_size = 2*spatial_size;
// 
//         diffusion_type<value_type,ode_size> F;
// 
//         ExplicitSDC<value_type, diffusion_type<value_type,ode_size>, SDCSpectralIntegrator<value_type, 0, 5, 2, 5>, 5, 4 > sdc(F);
// 
//         value_type **x = new value_type*[size];
//         value_type **f = new value_type*[size];
//         for(size_t i = 0; i < size; ++i)
//         {
//             x[i] = new value_type[ode_size];
//             f[i] = new value_type[ode_size];            
//         }
//         value_type time = 0, dt = .1;
//         F.init(0,1,x[0]);
//         F(time,x[0],f[0]);
//         for (size_t i = 0; i < size - 1; ++i)
//         {
//             sdc(time,x[i+1],x[i],f[i+1],f[i],dt);
//             time += dt;
//         }
//         std::ofstream output("./data.m");
//         output << "u = [";
//         for(int i = 0; i < size; ++i)
//             for(int j = 0; j < ode_size; j += 2)
//                 output << x[i][j] << " ";
//             output << "]; u = reshape(u," << spatial_size << "," << size << ")'; \n";
//         output << "v = [";
//         for(int i = 0; i < size; ++i)
//             for(int j = 1; j < ode_size; j += 2)
//                 output<< x[i][j] << " ";
//             output << "]; v = reshape(v," << spatial_size << "," << size << ")';  \n";
//         
//         output << "[X Y] = meshgrid(linspace(0,1," << spatial_size << "),linspace(0," << size*dt << "," << size << "));\n";
//         output << "mesh(X,Y,u)\n";
//         output << "axis equal\n";
//         for(size_t i = 0; i < size; ++i)
//         {
//             delete [] x[i];
//             delete [] f[i];
//         }
//         delete [] x;
//         delete [] f;
//     }
}

