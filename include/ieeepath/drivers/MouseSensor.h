#ifndef MOUSESENSOR_H
#define MOSUESENSOR_H

#include <string>
#include <stdexcept>

namespace ieeepath {
	class MouseSensor {
		public:
			class Error : public std::runtime_error {
				public:
					inline Error(const std::string &msg) : runtime_error(msg) { }
			};
		
			struct Reading {
				int dx, dy;
			};
			
			MouseSensor(const std::string &path);
			
			Reading getReading();
			
		private:
			int fd;
	};
}

#endif
