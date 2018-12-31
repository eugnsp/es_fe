import gdb
import gdb.printing
import gdb.types
import re

def strip_namespace_name(str):
	return str.split('::', 1).pop()

def get_vector_element(vec, index):
	type = gdb.types.get_basic_type(vec.type)
	return gdb.parse_and_eval('(*(%s*)(%s))[%d]' % (type, vec.address, index))

class Point_printer(object):
	def __init__(self, val):
		self.val = val

	def to_string(self):
		x = get_vector_element(self.val['data_']['data_'], 0)
		y = get_vector_element(self.val['data_']['data_'], 1)
		return '(%s, %s)' % (x, y)

class Index_printer(object):
	def __init__(self, val):
		self.val = val

	def to_string(self):
		type = gdb.types.get_basic_type(self.val.type)
		type_name = str(gdb.types.get_basic_type(type))
		name = strip_namespace_name(type_name).replace('_index', '')
		value = self.val.cast(gdb.lookup_type('es_fe::Index'))

		if value != type['%s::invalid' % type_name].enumval:
			return '%s {%s}' % (name, value)
		else:
			return '%s {invalid}' % name

class Mesh2_vertex_view_printer(object):
	def __init__(self, val):
		self.val = val

	def to_string(self):
		index = self.val['index_']
		return '%s' % index

class Mesh2_halfedge_view_printer(object):
	def __init__(self, val):
		self.val = val

	def to_string(self):
		index = self.val['index_']
		halfedges = self.val['mesh_']['halfedges_']
		vertex_from = get_vector_element(halfedges, int(index))['vertex']
		vertex_to = get_vector_element(halfedges, int(index) ^ 1)['vertex']
		return '%s: %s -> %s' % (index, vertex_from, vertex_to)

def build_pretty_printer():
	printer = gdb.printing.RegexpCollectionPrettyPrinter('es_fe')

	printer.add_printer('Point', '^es_fe::Point$', Point_printer)
	printer.add_printer('Element_index', '^es_fe::(Vertex|Halfedge|Edge|Face)_index$', Index_printer)
	printer.add_printer('Mesh2::Vertex_view', '^es_fe::Element_view<es_fe::Vertex_tag, es_fe::Mesh2>$', Mesh2_vertex_view_printer)
	printer.add_printer('Mesh2::Halfedge_view', '^es_fe::Element_view<es_fe::Halfedge_tag, es_fe::Mesh2>$', Mesh2_halfedge_view_printer)

	return printer

def register_printers():
	gdb.printing.register_pretty_printer(gdb.current_objfile(), build_pretty_printer())
