/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2011 Sergey Lisitsyn
 * Copyright (C) 2011 Berlin Institute of Technology and Max-Planck-Society
 */

#ifndef KERNELLOCALLYLINEAREMBEDDING_H_
#define KERNELLOCALLYLINEAREMBEDDING_H_
#include <shogun/lib/config.h>
#ifdef HAVE_LAPACK
#include <shogun/converter/LocallyLinearEmbedding.h>
#include <shogun/features/Features.h>
#include <shogun/distance/Distance.h>
#include <shogun/kernel/Kernel.h>

namespace shogun
{

class CFeatures;
class CKernel;

/** @brief the class KernelLocallyLinearEmbedding used to preprocess
 * data using kernel extension of Locally Linear Embedding algorithm as
 * described in
 *
 * Kayo, O. (2006). Locally linear embedding algorithm. Extensions and applications. October.
 * Retrieved from: http://herkules.oulu.fi/isbn9514280415/isbn9514280415.pdf
 *
 */
class CKernelLocallyLinearEmbedding: public CLocallyLinearEmbedding
{
public:

	/** constructor */
	CKernelLocallyLinearEmbedding();

	/** constructor
	 * @param kernel kernel to be used
	 */
	CKernelLocallyLinearEmbedding(CKernel* kernel);

	/** destructor */
	virtual ~CKernelLocallyLinearEmbedding();

	virtual CFeatures* apply(CFeatures* features);

	/** get name */
	virtual const char* get_name() const;

/// HELPERS
protected:

	/** construct weight matrix */
	virtual SGMatrix<float64_t> construct_weight_matrix(SGMatrix<float64_t> kernel_matrix,
	                                                    SGMatrix<int32_t> neighborhood_matrix);

	/** construct neighborhood matrix by kernel matrix
	 * @param kernel_matrix kernel matrix to be used
	 * @param k k 
	 * @return matrix containing indexes of neighbors of i-th object
	 * in i-th column
	 */
	virtual SGMatrix<int32_t> get_neighborhood_matrix(SGMatrix<float64_t> kernel_matrix, int32_t k);


/// THREADS
protected:

	/** runs neighborhood determination thread
	 * @param p thread params
	 */
	static void* run_neighborhood_thread(void* p);

	/** runs linear reconstruction thread
	 * @param p thread params
	 */
	static void* run_linearreconstruction_thread(void* p);

};
}

#endif /* HAVE_LAPACK */
#endif /* KERNELLOCALLYLINEAREMBEDDING_H_ */
