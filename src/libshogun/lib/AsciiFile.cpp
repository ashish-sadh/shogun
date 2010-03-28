#include "features/SparseFeatures.h"
#include "lib/File.h"
#include "lib/AsciiFile.h"
#include "lib/Mathematics.h"

using namespace shogun;

CAsciiFile::CAsciiFile(FILE* f, const char* name) : CFile(f, name)
{
}

CAsciiFile::CAsciiFile(char* fname, char rw, const char* name) : CFile(fname, rw, name)
{
}

CAsciiFile::~CAsciiFile()
{
}

void CAsciiFile::set_variable_name(const char* name)
{
}

char* CAsciiFile::get_variable_name()
{
	return NULL;
}

#define GET_VECTOR(fname, mfname, sg_type) \
void CAsciiFile::fname(sg_type*& vec, int32_t& len) \
{													\
	vec=NULL;										\
	len=0;											\
	int32_t num_feat=0;								\
	int32_t num_vec=0;								\
	mfname(vec, num_feat, num_vec);					\
	if ((num_feat==1) || (num_vec==1))				\
	{												\
		if (num_feat==1)							\
			len=num_vec;							\
		else										\
			len=num_feat;							\
	}												\
	else											\
	{												\
		delete[] vec;								\
		vec=NULL;									\
		len=0;										\
		SG_ERROR("Could not read vector from"		\
				" file %s (shape %dx%d found but "	\
				"vector expected).\n", filename,	\
				num_vec, num_feat);					\
	}												\
}

GET_VECTOR(get_byte_vector, get_byte_matrix, uint8_t)
GET_VECTOR(get_char_vector, get_char_matrix, char)
GET_VECTOR(get_int_vector, get_int_matrix, int32_t)
GET_VECTOR(get_shortreal_vector, get_shortreal_matrix, float32_t)
GET_VECTOR(get_real_vector, get_real_matrix, float64_t)
GET_VECTOR(get_short_vector, get_short_matrix, int16_t)
GET_VECTOR(get_word_vector, get_word_matrix, uint16_t)
#undef GET_VECTOR

#define GET_MATRIX(fname, conv, sg_type)										\
void CAsciiFile::fname(sg_type*& matrix, int32_t& num_feat, int32_t& num_vec)	\
{																				\
	struct stat stats;															\
	if (stat(filename, &stats)!=0)												\
		SG_ERROR("Could not get file statistics.\n");							\
																				\
	char* data=new char[stats.st_size+1];										\
	memset(data, 0, sizeof(char)*(stats.st_size+1));							\
	size_t nread=fread(data, sizeof(char), stats.st_size, file);				\
	if (nread<=0)																\
		SG_ERROR("Could not read data from %s.\n");								\
																				\
	SG_DEBUG("data read from file:\n%s\n", data);								\
																				\
	/* determine num_feat and num_vec, populate dynamic array */ 				\
	int32_t nf=0;																\
	num_feat=0;																	\
	num_vec=0;																	\
	char* ptr_item=NULL;														\
	char* ptr_data=data;														\
	CDynamicArray<char*>* items=new CDynamicArray<char*>();						\
																				\
	while (*ptr_data)															\
	{																			\
		if (*ptr_data=='\n')													\
		{																		\
			if (ptr_item)														\
				nf++;															\
																				\
			if (num_feat!=0 && nf!=num_feat)									\
				SG_ERROR("Number of features mismatches (%d != %d) in vector"	\
						" %d in file %s.\n", num_feat, nf, num_vec, filename);	\
																				\
			append_item(items, ptr_data, ptr_item);								\
			num_feat=nf;														\
			num_vec++;															\
			nf=0;																\
			ptr_item=NULL;														\
		}																		\
		else if (!isblank(*ptr_data) && !ptr_item)								\
		{																		\
			ptr_item=ptr_data;													\
		}																		\
		else if (isblank(*ptr_data) && ptr_item)								\
		{																		\
			append_item(items, ptr_data, ptr_item);								\
			ptr_item=NULL;														\
			nf++;																\
		}																		\
																				\
		ptr_data++;																\
	}																			\
																				\
	SG_DEBUG("num feat: %d, num_vec %d\n", num_feat, num_vec);					\
	delete[] data;																\
																				\
	/* now copy data into matrix */ 											\
	matrix=new sg_type[num_vec*num_feat];										\
	for (int32_t i=0; i<num_vec; i++)											\
	{																			\
		for (int32_t j=0; j<num_feat; j++)										\
		{																		\
			char* item=items->get_element(i*num_feat+j);						\
			matrix[i*num_feat+j]=conv(item);									\
			delete[] item;														\
		}																		\
	}																			\
	delete items;																\
	CMath::transpose_matrix(matrix, num_feat, num_vec);							\
}

GET_MATRIX(get_byte_matrix, atoi, uint8_t)
GET_MATRIX(get_char_matrix, atoi, char)
GET_MATRIX(get_int_matrix, atoi, int32_t)
GET_MATRIX(get_uint_matrix, atoi, uint32_t)
GET_MATRIX(get_long_matrix, atoll, int64_t)
GET_MATRIX(get_ulong_matrix, atoll, uint64_t)
GET_MATRIX(get_shortreal_matrix, atof, float32_t)
GET_MATRIX(get_real_matrix, atof, float64_t)
GET_MATRIX(get_longreal_matrix, atof, floatmax_t)
GET_MATRIX(get_short_matrix, atoi, int16_t)
GET_MATRIX(get_word_matrix, atoi, uint16_t)
#undef GET_MATRIX

void CAsciiFile::get_byte_ndarray(uint8_t*& array, int32_t*& dims, int32_t& num_dims)
{
}

void CAsciiFile::get_char_ndarray(char*& array, int32_t*& dims, int32_t& num_dims)
{
}

void CAsciiFile::get_int_ndarray(int32_t*& array, int32_t*& dims, int32_t& num_dims)
{
}

void CAsciiFile::get_shortreal_ndarray(float32_t*& array, int32_t*& dims, int32_t& num_dims)
{
}

void CAsciiFile::get_real_ndarray(float64_t*& array, int32_t*& dims, int32_t& num_dims)
{
}

void CAsciiFile::get_short_ndarray(int16_t*& array, int32_t*& dims, int32_t& num_dims)
{
}

void CAsciiFile::get_word_ndarray(uint16_t*& array, int32_t*& dims, int32_t& num_dims)
{
}

void CAsciiFile::get_real_sparsematrix(TSparse<float64_t>*& matrix, int32_t& num_feat, int32_t& num_vec)
{
	size_t blocksize=1024*1024;
	size_t required_blocksize=blocksize;
	uint8_t* dummy=new uint8_t[blocksize];

	if (file)
	{
		num_vec=0;
		num_feat=0;

		SG_INFO("counting line numbers in file %s\n", filename);
		size_t sz=blocksize;
		size_t block_offs=0;
		size_t old_block_offs=0;
		fseek(file, 0, SEEK_END);
		size_t fsize=ftell(file);
		rewind(file);

		while (sz == blocksize)
		{
			sz=fread(dummy, sizeof(uint8_t), blocksize, file);
			bool contains_cr=false;
			for (size_t i=0; i<sz; i++)
			{
				block_offs++;
				if (dummy[i]=='\n' || (i==sz-1 && sz<blocksize))
				{
					num_vec++;
					contains_cr=true;
					required_blocksize=CMath::max(required_blocksize, block_offs-old_block_offs+1);
					old_block_offs=block_offs;
				}
			}
			SG_PROGRESS(block_offs, 0, fsize, 1, "COUNTING:\t");
		}

		SG_INFO("found %d feature vectors\n", num_vec);
		delete[] dummy;
		blocksize=required_blocksize;
		dummy = new uint8_t[blocksize+1]; //allow setting of '\0' at EOL
		matrix=new TSparse<float64_t>[num_vec];

		rewind(file);
		sz=blocksize;
		int32_t lines=0;
		while (sz == blocksize)
		{
			sz=fread(dummy, sizeof(uint8_t), blocksize, file);

			size_t old_sz=0;
			for (size_t i=0; i<sz; i++)
			{
				if (i==sz-1 && dummy[i]!='\n' && sz==blocksize)
				{
					size_t len=i-old_sz+1;
					uint8_t* data=&dummy[old_sz];

					for (size_t j=0; j<len; j++)
						dummy[j]=data[j];

					sz=fread(dummy+len, sizeof(uint8_t), blocksize-len, file);
					i=0;
					old_sz=0;
					sz+=len;
				}

				if (dummy[i]=='\n' || (i==sz-1 && sz<blocksize))
				{

					size_t len=i-old_sz;
					uint8_t* data=&dummy[old_sz];

					int32_t dims=0;
					for (size_t j=0; j<len; j++)
					{
						if (data[j]==':')
							dims++;
					}

					if (dims<=0)
					{
						SG_ERROR("Error in line %d - number of"
								" dimensions is %d line is %d characters"
								" long\n line_content:'%.*s'\n", lines,
								dims, len, len, (const char*) data);
					}

					TSparseEntry<float64_t>* feat=new TSparseEntry<float64_t>[dims];

					//skip label part
					size_t j=0;
					for (; j<len; j++)
					{
						if (data[j]==':')
						{
							j=-1; //file without label
							break;
						}

						if (data[j]==' ')
						{
							data[j]='\0';

							//skip label part
							break;
						}
					}

					int32_t d=0;
					j++;
					uint8_t* start=&data[j];
					for (; j<len; j++)
					{
						if (data[j]==':')
						{
							data[j]='\0';

							feat[d].feat_index=(int32_t) atoi((const char*) start)-1;
							num_feat=CMath::max(num_feat, feat[d].feat_index+1);

							j++;
							start=&data[j];
							for (; j<len; j++)
							{
								if (data[j]==' ' || data[j]=='\n')
								{
									data[j]='\0';
									feat[d].entry=(float64_t) atof((const char*) start);
									d++;
									break;
								}
							}

							if (j==len)
							{
								data[j]='\0';
								feat[dims-1].entry=(float64_t) atof((const char*) start);
							}

							j++;
							start=&data[j];
						}
					}

					matrix[lines].vec_index=lines;
					matrix[lines].num_feat_entries=dims;
					matrix[lines].features=feat;

					old_sz=i+1;
					lines++;
					SG_PROGRESS(lines, 0, num_vec, 1, "LOADING:\t");
				}
			}
		}

		SG_INFO("file successfully read\n");
	}

	delete[] dummy;
}


void CAsciiFile::get_byte_string_list(T_STRING<uint8_t>*& strings, int32_t& num_str, int32_t& max_string_len)
{
	strings=NULL;
	num_str=0;
	max_string_len=0;
}

void CAsciiFile::get_char_string_list(T_STRING<char>*& strings, int32_t& num_str, int32_t& max_string_len)
{
	size_t blocksize=1024*1024;
	size_t required_blocksize=0;
	char* dummy=new char[blocksize];
	char* overflow=NULL;
	int32_t overflow_len=0;

	if (file)
	{
		num_str=0;
		max_string_len=0;

		SG_INFO("counting line numbers in file %s\n", filename);
		size_t sz=blocksize;
		size_t block_offs=0;
		size_t old_block_offs=0;
		fseek(file, 0, SEEK_END);
		size_t fsize=ftell(file);
		rewind(file);

		while (sz == blocksize)
		{
			sz=fread(dummy, sizeof(char), blocksize, file);
			bool contains_cr=false;
			for (size_t i=0; i<sz; i++)
			{
				block_offs++;
				if (dummy[i]=='\n' || (i==sz-1 && sz<blocksize))
				{
					num_str++;
					contains_cr=true;
					required_blocksize=CMath::max(required_blocksize, block_offs-old_block_offs);
					old_block_offs=block_offs;
				}
			}
			SG_PROGRESS(block_offs, 0, fsize, 1, "COUNTING:\t");
		}

		SG_INFO("found %d strings\n", num_str);
		SG_DEBUG("block_size=%d\n", required_blocksize);
		delete[] dummy;
		blocksize=required_blocksize;
		dummy=new char[blocksize];
		overflow=new char[blocksize];
		strings=new T_STRING<char>[num_str];

		rewind(file);
		sz=blocksize;
		int32_t lines=0;
		size_t old_sz=0;
		while (sz == blocksize)
		{
			sz=fread(dummy, sizeof(char), blocksize, file);

			old_sz=0;
			for (size_t i=0; i<sz; i++)
			{
				if (dummy[i]=='\n' || (i==sz-1 && sz<blocksize))
				{
					int32_t len=i-old_sz;
					max_string_len=CMath::max(max_string_len, len+overflow_len);

					strings[lines].length=len+overflow_len;
					strings[lines].string=new char[len+overflow_len];

					for (int32_t j=0; j<overflow_len; j++)
						strings[lines].string[j]=overflow[j];
					for (int32_t j=0; j<len; j++)
						strings[lines].string[j+overflow_len]=dummy[old_sz+j];

					// clear overflow
					overflow_len=0;

					//CMath::display_vector(strings[lines].string, len);
					old_sz=i+1;
					lines++;
					SG_PROGRESS(lines, 0, num_str, 1, "LOADING:\t");
				}
			}

			for (size_t i=old_sz; i<sz; i++)
				overflow[i-old_sz]=dummy[i];

			overflow_len=sz-old_sz;
		}
		SG_INFO("file successfully read\n");
		SG_INFO("max_string_length=%d\n", max_string_len);
		SG_INFO("num_strings=%d\n", num_str);
	}

	delete[] dummy;
	delete[] overflow;
}

void CAsciiFile::get_int_string_list(T_STRING<int32_t>*& strings, int32_t& num_str, int32_t& max_string_len)
{
	strings=NULL;
	num_str=0;
	max_string_len=0;
}

void CAsciiFile::get_uint_string_list(T_STRING<uint32_t>*& strings, int32_t& num_str, int32_t& max_string_len)
{
	strings=NULL;
	num_str=0;
	max_string_len=0;
}

void CAsciiFile::get_short_string_list(T_STRING<int16_t>*& strings, int32_t& num_str, int32_t& max_string_len)
{
	strings=NULL;
	num_str=0;
	max_string_len=0;
}

void CAsciiFile::get_word_string_list(T_STRING<uint16_t>*& strings, int32_t& num_str, int32_t& max_string_len)
{
	strings=NULL;
	num_str=0;
	max_string_len=0;
}

void CAsciiFile::get_long_string_list(T_STRING<int64_t>*& strings, int32_t& num_str, int32_t& max_string_len)
{
	strings=NULL;
	num_str=0;
	max_string_len=0;
}

void CAsciiFile::get_ulong_string_list(T_STRING<uint64_t>*& strings, int32_t& num_str, int32_t& max_string_len)
{
	strings=NULL;
	num_str=0;
	max_string_len=0;
}

void CAsciiFile::get_shortreal_string_list(T_STRING<float32_t>*& strings, int32_t& num_str, int32_t& max_string_len)
{
	strings=NULL;
	num_str=0;
	max_string_len=0;
}

void CAsciiFile::get_real_string_list(T_STRING<float64_t>*& strings, int32_t& num_str, int32_t& max_string_len)
{
	strings=NULL;
	num_str=0;
	max_string_len=0;
}

void CAsciiFile::get_longreal_string_list(T_STRING<floatmax_t>*& strings, int32_t& num_str, int32_t& max_string_len)
{
	strings=NULL;
	num_str=0;
	max_string_len=0;
}


/** set functions - to pass data from shogun to the target interface */

#define SET_VECTOR(fname, mfname, sg_type)	\
void CAsciiFile::fname(const sg_type* vec, int32_t len)	\
{															\
	mfname(vec, len, 1);									\
}
SET_VECTOR(set_byte_vector, set_byte_matrix, uint8_t)
SET_VECTOR(set_char_vector, set_char_matrix, char)
SET_VECTOR(set_int_vector, set_int_matrix, int32_t)
SET_VECTOR(set_shortreal_vector, set_shortreal_matrix, float32_t)
SET_VECTOR(set_real_vector, set_real_matrix, float64_t)
SET_VECTOR(set_short_vector, set_short_matrix, int16_t)
SET_VECTOR(set_word_vector, set_word_matrix, uint16_t)
#undef SET_VECTOR

#define SET_MATRIX(fname, sg_type, fprt_type, type_str) \
void CAsciiFile::fname(const sg_type* matrix, int32_t num_feat, int32_t num_vec)	\
{																					\
	if (!(file && matrix))															\
		SG_ERROR("File or matrix invalid.\n");										\
																					\
	for (int32_t i=0; i<num_feat; i++)												\
	{																				\
		for (int32_t j=0; j<num_vec; j++)											\
		{																			\
			sg_type v=matrix[num_feat*j+i];											\
			if (j==num_vec-1)														\
				fprintf(file, type_str "\n", (fprt_type) v);						\
			else																	\
				fprintf(file, type_str " ", (fprt_type) v);							\
		}																			\
	}																				\
}
SET_MATRIX(set_char_matrix, char, char, "%c")
SET_MATRIX(set_byte_matrix, uint8_t, uint8_t, "%u")
SET_MATRIX(set_int_matrix, int32_t, int32_t, "%i")
SET_MATRIX(set_uint_matrix, uint32_t, uint32_t, "%u")
SET_MATRIX(set_long_matrix, int64_t, long long int, "%lli")
SET_MATRIX(set_ulong_matrix, uint64_t, long long unsigned int, "%llu")
SET_MATRIX(set_short_matrix, int16_t, int16_t, "%i")
SET_MATRIX(set_word_matrix, uint16_t, uint16_t, "%u")
SET_MATRIX(set_shortreal_matrix, float32_t, float32_t, "%f")
SET_MATRIX(set_real_matrix, float64_t, float64_t, "%f")
SET_MATRIX(set_longreal_matrix, floatmax_t, floatmax_t, "%Lf")
#undef SET_MATRIX

void CAsciiFile::set_real_sparsematrix(const TSparse<float64_t>* matrix, int32_t num_feat, int32_t num_vec, int64_t nnz)
{
	if (!(file && matrix))
		SG_ERROR("File or matrix invalid.\n");

	for (int32_t i=0; i<num_vec; i++)
	{
		TSparseEntry<float64_t>* vec = matrix[i].features;
		int32_t len=matrix[i].num_feat_entries;

		for (int32_t j=0; j<len; j++)
		{
			if (j<len-1)
				fprintf(file, "%d:%f ", (int32_t) vec[j].feat_index+1, (double) vec[j].entry);
			else
				fprintf(file, "%d:%f\n", (int32_t) vec[j].feat_index+1, (double) vec[j].entry);
		}
	}
}

void CAsciiFile::set_byte_string_list(const T_STRING<uint8_t>* strings, int32_t num_str)
{
}

void CAsciiFile::set_char_string_list(const T_STRING<char>* strings, int32_t num_str)
{
	if (!(file && strings))
		SG_ERROR("File or strings invalid.\n");

	for (int32_t i=0; i<num_str; i++)
	{
		int32_t len = strings[i].length;
		fwrite(strings[i].string, sizeof(char), len, file);
		fprintf(file, "\n");
	}
}

void CAsciiFile::set_int_string_list(const T_STRING<int32_t>* strings, int32_t num_str)
{
}

void CAsciiFile::set_uint_string_list(const T_STRING<uint32_t>* strings, int32_t num_str)
{
}

void CAsciiFile::set_short_string_list(const T_STRING<int16_t>* strings, int32_t num_str)
{
}

void CAsciiFile::set_word_string_list(const T_STRING<uint16_t>* strings, int32_t num_str)
{
}

void CAsciiFile::set_long_string_list(const T_STRING<int64_t>* strings, int32_t num_str)
{
}

void CAsciiFile::set_ulong_string_list(const T_STRING<uint64_t>* strings, int32_t num_str)
{
}

void CAsciiFile::set_shortreal_string_list(const T_STRING<float32_t>* strings, int32_t num_str)
{
}

void CAsciiFile::set_real_string_list(const T_STRING<float64_t>* strings, int32_t num_str)
{
}

void CAsciiFile::set_longreal_string_list(const T_STRING<floatmax_t>* strings, int32_t num_str)
{
}

template <class T> void CAsciiFile::append_item(
	CDynamicArray<T>* items, char* ptr_data, char* ptr_item)
{
	size_t len=(ptr_data-ptr_item)/sizeof(char);
	char* item=new char[len+1];
	memset(item, 0, sizeof(char)*(len+1));
	item=strncpy(item, ptr_item, len);

	SG_DEBUG("current %c, len %d, item %s\n", *ptr_data, len, item);
	items->append_element(item);
}

/*
bool load(char* fname)
{
			int64_t length=0;
			max_string_length=0;

			CFile f(fname, 'r', F_CHAR);
			char* feature_matrix=f.load_char_data(NULL, length);

			SG_DEBUG("char data now at %p of length %ld\n", 
					feature_matrix, (int64_t) length);

			num_vectors=0;

			if (f.is_ok())
			{
				for (int64_t i=0; i<length; i++)
				{
					if (feature_matrix[i]=='\n')
						num_vectors++;
				}

				SG_INFO( "file contains %ld vectors\n", num_vectors);
				features= new T_STRING<ST>[num_vectors];

				int64_t index=0;
				for (int32_t lines=0; lines<num_vectors; lines++)
				{
					char* p=&feature_matrix[index];
					int32_t columns=0;

					for (columns=0; index+columns<length && p[columns]!='\n'; columns++);

					if (index+columns>=length && p[columns]!='\n') {
						SG_ERROR( "error in \"%s\":%d\n", fname, lines);
					}

					features[lines].length=columns;
					features[lines].string=new ST[columns];

					max_string_length=CMath::max(max_string_length,columns);

					for (int32_t i=0; i<columns; i++)
						features[lines].string[i]= ((ST) p[i]);

					index+= features[lines].length+1;
				}

				num_symbols=4; //FIXME
				return true;
			}
			else
				SG_ERROR( "reading file failed\n");

			return false;
} */
