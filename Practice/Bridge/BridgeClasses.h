#pragma once
#include <vector>
#include <memory>

namespace bridge_device {

	class IOFile;
	class IOSocket;

	class IODevice {
	public:
		virtual size_t read(std::vector<std::byte>& buffer, size_t n) const = 0;
		virtual size_t write(const std::vector<std::byte>& buffer) const = 0;
	};

	class IOBaseOperator {
	protected:
		std::shared_ptr<const IODevice> dev;

	public:
		IOBaseOperator(std::shared_ptr<const IODevice> dev) : dev{dev}
		{
		}
	};

	class IOOperator : public IOBaseOperator {
	public:
		IOOperator(std::shared_ptr<const IODevice> dev) : IOBaseOperator(dev)
		{
		}

		virtual size_t load(std::vector<std::byte>& buffer, size_t n) const;
		virtual size_t write(const std::vector<std::byte>& buffer) const;
	};

	class IOZipOperator : public IOBaseOperator {
	public:
		IOZipOperator(std::shared_ptr<const IODevice> dev) : IOBaseOperator(dev)
		{
		}

		virtual size_t load(std::vector<std::byte>& buffer, size_t n) const;
		virtual size_t write(const std::vector<std::byte>& buffer) const;
	};

     std::shared_ptr<const IODevice> makeFileDevice();
}


