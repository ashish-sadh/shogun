from tools.load import LoadMatrix
from sg import sg
lm=LoadMatrix()

traindna=lm.load_dna('../data/fm_train_dna.dat')
testdna=lm.load_dna('../data/fm_test_dna.dat')
parameter_list=[[traindna,testdna,10,3,0,'n',False,'FULL'],
		[traindna,testdna,11,4,0,'n',False,'FULL']]

def kernel_commwordstring (fm_train_dna=traindna,fm_test_dna=testdna,
			    size_cache=10,
			    order=3,gap=0,reverse='n',
			    use_sign=False,normalization='FULL'):

	sg('add_preproc', 'SORTWORDSTRING')
	sg('set_features', 'TRAIN', fm_train_dna, 'DNA')
	sg('convert', 'TRAIN', 'STRING', 'CHAR', 'STRING', 'WORD', order, order-1, gap, reverse)
	sg('attach_preproc', 'TRAIN')

	sg('set_features', 'TEST', fm_test_dna, 'DNA')
	sg('convert', 'TEST', 'STRING', 'CHAR', 'STRING', 'WORD', order, order-1, gap, reverse)
	sg('attach_preproc', 'TEST')

	sg('set_kernel', 'COMMSTRING', 'WORD', size_cache, use_sign, normalization)
	km=sg('get_kernel_matrix', 'TRAIN')
	km=sg('get_kernel_matrix', 'TEST')
	return km

if __name__=='__main__':
	print 'CommWordString'
	kernel_commwordstring(*parameter_list[0])
