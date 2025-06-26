#include "BridgeClasses.h"
#include <algorithm>
#include <memory>

namespace bridge_device {

	class IOFile : public IODevice 
	{
	public:
		virtual size_t read(std::vector<std::byte>& buffer, size_t n) const override {
            std::fill(buffer.begin(), buffer.end(), std::byte{1});
			// read into the buffer
			return buffer.size();
		}

		virtual size_t write(const std::vector<std::byte>& buffer) const override {
			// flush the buffer
			return buffer.size();
		}
	};

	class IOSocket : public IODevice 
	{
	public:
		virtual size_t read(std::vector<std::byte>& buffer, size_t n) const override {
            std::fill(buffer.begin(), buffer.end(), std::byte{1});
			// read into the buffer
			return buffer.size();
		}

		virtual size_t write(const std::vector<std::byte>& buffer) const override {
			// flush the buffer
			return buffer.size();
		}
	};

	size_t IOOperator::load(std::vector<std::byte>& buffer, size_t n) const
	{
		return size_t();
	}

	size_t IOOperator::write(const std::vector<std::byte>& buffer) const
	{
		return size_t();
	}

	size_t IOZipOperator::load(std::vector<std::byte>& buffer, size_t n) const
	{
		return size_t();
	}

	size_t IOZipOperator::write(const std::vector<std::byte>& buffer) const
	{
		return size_t();
	}

    std::shared_ptr<const IODevice> makeFileDevice()
    {
        return std::make_shared<IOFile>();
    }

}
