def manhattan_word_distance ():
	print 'ManhattanWordDistance'

	order=3
	gap=0
	reverse='n' # bit silly to not use boolean, set 'r' to yield true

	from sg import sg
	sg('set_distance', 'MANHATTAN', 'WORD')
	sg('add_preproc', 'SORTWORDSTRING')

	sg('set_features', 'TRAIN', fm_train_dna, 'DNA')
	sg('convert', 'TRAIN', 'STRING', 'CHAR', 'STRING', 'WORD', order, order-1, gap, reverse)
	sg('attach_preproc', 'TRAIN')
	sg('init_distance', 'TRAIN')
	dm=sg('get_distance_matrix')

	sg('set_features', 'TEST', fm_test_dna, 'DNA')
	sg('convert', 'TEST', 'STRING', 'CHAR', 'STRING', 'WORD', order, order-1, gap, reverse)
	sg('attach_preproc', 'TEST')
	sg('init_distance', 'TEST')
	dm=sg('get_distance_matrix')

if __name__=='__main__':
	from tools.load import LoadMatrix
	lm=LoadMatrix()
	fm_train_dna=lm.load_dna('../data/fm_train_dna.dat')
	fm_test_dna=lm.load_dna('../data/fm_test_dna.dat')
	manhattan_word_distance()