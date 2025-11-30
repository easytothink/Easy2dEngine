#ifndef _IO_EXCEPTION_H_
#define _IO_EXCEPTION_H_

#include <string>
#include <exception>

class IOException : public std::exception
{
public:
	IOException(const char* path_file) {
		error = path_file;
	};
	~IOException() {
		//TODO
	};

	const char* what() const throw() {
		return error.c_str();
	}

private:
	std::string error;
};

#endif // !_IO_EXCEPTION_H_