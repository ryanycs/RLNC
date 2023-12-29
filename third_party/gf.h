/* gf.h 
 * Yang Yuwang, Zhao Wei, Wang Lei 
 * June, 2012

Fast Galois Field Arithmetic in C/C++
Copright (C) 2012 Yang Yuwang, Zhao Wei, Wang Lei 

These files are free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

These files are distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

Yang Yuwang, Zhao Wei, Wang Lei 
Department of Computer Science
Nanjing University of Science and Technology

yuwangyang@mail.njust.edu.cn
http://www.sensor608.com/gf.html

 */

#ifndef _GF_H
#define _GF_H

// #include <stdint-gcc.h>
#include <stdint.h>

#ifdef _cplusplus
extern "C" {	
#endif

typedef uint16_t GFType;

extern GFType prim_poly[13];
extern GFType* table_alpha;
extern GFType* table_index;
extern GFType** table_mul;
extern GFType** table_div;
extern int gFieldSize;

void gf_init(unsigned int m, unsigned int prim); 
void gf_uninit();
void gf_print();

#define  gf_alpha(n)   (table_alpha[n])
#define  gf_index(n)   (table_index[n])

#define  gf_add(a,b)	(a^b)
#define  gf_sub(a,b)	(a^b)

#define  gf_mul(a,b)	(table_mul[a][b])
#define  gf_div(a,b)	(table_div[a][b])

#define  gf_neg(a)		(a)
#define  gf_inv(a)		(table_div[1][a])
GFType gf_exp(GFType a, GFType n);

/*!
 * return a gauss_inv matrix by the certain random coef
 * @ensures gf_list is full-trace
 * @param gf_list the rand_coef
 * @param vec_size the size of this vector_list
 * @return the inv matrix
 */
GFType** gauss_inv(GFType** gf_list, int vec_size);

/*!
 * Get the new encoding matrix
 * @ensures both coef1 and coef2 are full-trace
 * @param coef1 first encoding coef
 * @param coef2 second encoding coef
 * @param vec_size the size of the coef-matrix
 * @param vec_x the x_size of the coef_old matrix
 * @return the new encoding coef matrix
 */
GFType** gf_newcoef(GFType** coef_new, GFType** coef_old, int vec_size, int vec_x);

#ifdef _cplusplus
}	
#endif


#endif