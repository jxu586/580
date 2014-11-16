#ifndef _ERROR_H_
#define _ERROR_H_

enum ERROR_TYPE 
{
	ET_SUCCESS = 0,
	ET_SHADER_CREATE_FAILURE = 10,
	ET_SHADER_FILE_NO_FOUND = 11,
	ET_SHADER_CREATION_FAILURE = 12,
	ET_SHADER_COMPILE_ERROR = 13,
	ET_SHADER_LINK_ERROR = 14,
	ET_SHADER_VARIABLE_BINDING_ERROR = 15,
	ET_FRAMEBUFFER_CREATE_FAILURE = 20,
	ET_TEXTURE_FILE_NO_FOUND = 30,
	ET_TEXTURE_INVALID_FILE_FORMAT = 31,
	ET_TEXTURE_LOADING_INTERNAL_ERROR = 32,
	ET_CONFIG_FILE_NO_FOUND = 40,
	ET_CONFIG_FILE_FORMAT_ERROR = 41,
};

#define GET_ERROR_RETURN(id) (-(int)id) 
#define GET_ERROR_ID(ret) (-(ret))

#define IF_FAILED(r, proc, err_proc) \
	do {\
		(r) = (proc);\
		if ((r) < 0) { \
			std::cerr << #proc << " failed! at line " << __LINE__ << ", file " << __FILE__ << std::flush << std::endl; \
			goto err_proc; \
		} \
	} while(0)


#endif


