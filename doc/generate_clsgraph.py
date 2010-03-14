#!/usr/bin/env python
# -*- coding: utf-8 -*-
# vim: set noexpandtab ts=4:

import re
import sys
import cgi

from pyparsing import (
	alphas, CharsNotIn, Dict, Group, Keyword, LineStart, LineEnd, Literal,
	Optional, Or, printables, Word, ZeroOrMore
)
import pydot

arrows = {}
stereotypes = {}

def define_arrow(arrow, **kws):
	global arrows
	arrows[arrow] = kws

def define_stereotype(stereotype, **kws):
	global stereotypes
	stereotypes[stereotype] = kws

def create_grammar():
	global arrows
	global stereotypes
	assert len(arrows) > 0
	assert len(stereotypes) > 0

	linechars = ''.join((c for c in printables if c not in '}\n')) + ' \t'
	norbracket = ''.join((c for c in printables if c != ']')) + ' \t'
	nogt = ''.join((c for c in printables if c != '>')) + ' \t'

	line = Word(linechars)
	cls_body = Group(ZeroOrMore(line))
	classkeyword = Keyword('class').setResultsName('type')

	st_names = stereotypes.keys()
	st = Literal(st_names[0])
	for s in st_names[1:]:
		st = st | Literal(s)
	stereotype = Group(Optional(Literal('<<').suppress() + st + Literal('>>').suppress()))

	identifier_list = Word(alphas) + ZeroOrMore(Literal(',').suppress() + Word(alphas))
	baseclasses = Group(Optional(Literal(':').suppress() + identifier_list))

	cls = Group(stereotype + classkeyword + Word(alphas) + baseclasses + \
		Literal('{').suppress() + cls_body + Literal('}').suppress())

	arrow_names = arrows.keys()
	arrow = Keyword(arrow_names[0])
	for ar in arrow_names[1:]:
		arrow = arrow | Keyword(ar)

	quantifier = Literal('[').suppress() + Word(norbracket) + Literal(']').suppress()
	relation = Group(Word(alphas) + Group(Optional(quantifier)) + \
		arrow.setResultsName('type') + \
		Word(alphas) + Group(Optional(quantifier)))

	grammar = ZeroOrMore(cls | relation)
	return grammar

def create_graph(tokens, filename):
	global arrows
	global stereotypes

	graph = pydot.Dot('Classgraph',
		graph_type='digraph',
		fontname='Bitstream Vera Sans', fontsize='8',
		)
	for token in tokens:
		if token['type'] == 'class':
			# (stereotype) "class" classname (baseclasses) lines
			st_name = stereotype = ''
			if token[0]:
				st_name = token[0][0]
				stereotype = '<font color="#607080">' + cgi.escape('<<' + st_name + '>>') + '</font><br align="center"/>'

			name = cgi.escape(token[2])
			baseclasses = token[3]
			lines = (cgi.escape(l) for l in token[4])

			n = pydot.Node(name,
				shape='plaintext',
				fontname='Bitstream Vera Sans', fontsize='8',
			)

			n.set_label('''<
			<table border="0" cellborder="1" cellspacing="0" cellpadding="4">
				<tr><td align="center" bgcolor="%(bgcolor)s">%(stereotype)s<font point-size="12">%(name)s</font></td></tr>
				<tr><td align="left">%(l)s</td></tr>
			</table>
			>''' % {
				'bgcolor': stereotypes.get(st_name, {}).get('bgcolor', '#c0c8d0'),
				'stereotype': stereotype,
				'name': name,
				'l': ''.join(('</td></tr><tr><td align="left">' if t == '--' else t + '<BR align="left" />') for t in lines),
			})
			graph.add_node(n)

			for base in baseclasses:
				e = pydot.Edge(base, name,
					fontname='Bitstream Vera Sans', fontsize='8',
					arrowtail='empty', arrowhead='none'
				)
				graph.add_edge(e)

		elif arrows.has_key(token['type']):
			left = token[0]
			left_q = token[1]
			right = token[3]
			right_q = token[4]

			kws = {
				'fontname': 'Bitstream Vera Sans',
				'fontsize': '8',
			}
			if left_q: kws['taillabel'] = left_q[0]
			if right_q: kws['headlabel'] = right_q[0]
			kws.update(arrows[token['type']])
			e = pydot.Edge(left, right, **kws)
			graph.add_edge(e)

	#print graph.create(prog='dot', format='dot')
	graph.write_png(filename, prog='dot')

if __name__ == '__main__':

	if len(sys.argv) != 3:
		print >>sys.stderr, "syntax: %s [inputfile.cls] [outputfile.png]" % sys.argv[0]
		sys.exit(1)

	inputfile, outputfile = sys.argv[1:]

	define_arrow('->', arrowtail='none', arrowhead='')
	define_arrow('<>-', arrowtail='diamond', arrowhead='none')
	define_arrow('{}-', arrowtail='ediamond', arrowhead='none')
	define_arrow('--', arrowtail='none', arrowhead='none')

	define_stereotype('Interface', bgcolor='#b0d0b8')
	define_stereotype('Singleton', bgcolor='#e09080')

	grammar = create_grammar()
	tokens = grammar.parseString(open(inputfile, 'r').read(), True)
	create_graph(tokens, outputfile)
