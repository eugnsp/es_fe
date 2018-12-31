#pragma once
#include <es_fe/types/index.hpp>

namespace es_fe
{
//////////////////////////////////////////////////////////////////////////
/** Mesh-related forward declarations */

class Mesh1;
class Mesh2;

template<class Element_tag, class Circ_tag>
class End_circular_iterator
{};

template<class Element_tag, class Mesh>
class Random_access_iterator;

template<class Element_tag, class Circ_tag>
class Circulator_vertex_face;

template<class Element_tag, class Circ_tag>
class Circulator_halfedge_edge;

template<class Element_tag, class Mesh>
class Element_view;

//////////////////////////////////////////////////////////////////////////
template<class Element, Local_index dim, class... Bnd_conds>
class Var;

//////////////////////////////////////////////////////////////////////////
/** Finite elements */

template<Local_index order, Local_index dim = 2>
class Lagrange;

template<Local_index order, Local_index dim = 2>
class Discontinuous_lagrange;

//////////////////////////////////////////////////////////////////////////

template<Local_index, Local_index dim>
class Quadr;

template<class Element, class Quadr>
class Element_quadr;

template<class Var_list, template<class> class T_Dof_mapper>
class System;

template<class System>
class Dof_tools;
} // namespace es_fe