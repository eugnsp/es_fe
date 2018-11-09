#pragma once

namespace es_fe
{

// template<class Solver, void>
// struct Has_assemble_on_cell : std::false_type { };
//
// template<class Solver, std::void_t<decltype(&Solver::assemble_on_cell)>
// struct Has_assemble_on_cell : std::false_type { };

class Seq_assembler
{
public:
	void init()
	{}

	template<class System, class Solver>
	void assemble(const System& system, Solver& solver)
	{
		for (const auto& cell : solver.mesh().cells())
			solver.assemble_on_cell(cell);

		for_each_var<typename System::Var_list>([&system, &solver](auto var) {
			const auto& v = system.variable(var);
			v.for_each_non_ess_bnd_cond([&solver](const auto& bc) {
				// TODO : view!
				for (const auto& halfedge : bc.halfedges())
					solver.assemble_on_halfedge(halfedge, bc);
			});
		});
	}
};

} // namespace es_fe